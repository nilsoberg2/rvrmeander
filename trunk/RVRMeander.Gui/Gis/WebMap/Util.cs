using DotSpatial.Controls;
using RVRMeander.Gui.Gis.WebMap;
using DotSpatial.Projections;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RVRMeander.Gui.Gis.WebMap.Tiling;
using DotSpatial.Data;
using DotSpatial.Topology;
using RVRMeander.Gui.Gis.WebMap.Resources;

namespace RVRMeander.Gui.Gis.WebMap
{
  class Util
  {
    #region Constants and Fields

    private const string Other = "Other";
    private const string PluginName = "FetchBasemap";
    private const string STR_KeyOpacityDropDown = "kOpacityDropDown";
    private const string STR_KeyServiceDropDown = "kServiceDropDown";

    private readonly ProjectionInfo WebMercProj = ProjectionInfo.FromEsriString(KnownCoordinateSystems.Projected.World.WebMercator.ToEsriString());
    private readonly ProjectionInfo Wgs84Proj = ProjectionInfo.FromEsriString(KnownCoordinateSystems.Geographic.World.WGS1984.ToEsriString());

    private MapImageLayer baseMapLayer;
    private BackgroundWorker bw;
    private ServiceProvider emptyProvider;
    private IMapFeatureLayer featureSetLayer;
    private Int16 _opacity = 100;
    private TileManager tileManager;
    private bool busySet;
    private Dictionary<string, ServiceProvider> providerList;
    private IMap theMap;
    private ProjectionInfo originalProjection;
    private IBasemapWindow basemapWindow;

    //private DropDownActionItem _opacityDropDown;
    //private DropDownActionItem _serviceDropDown;
    //private SimpleActionItem _optionsAction;

    #endregion

    public Util(IMap map)
    {
      this.theMap = map;

      this.busySet = false;

      this.providerList = new Dictionary<string, ServiceProvider>();

      //Setup the background worker
      bw = new BackgroundWorker { WorkerSupportsCancellation = true, WorkerReportsProgress = true };
      bw.DoWork += BwDoWork;
      bw.RunWorkerCompleted += BwRunWorkerCompleted;
      bw.ProgressChanged += BwProgressChanged;
    }

    #region Public Methods

    ///// <summary>
    ///// Initialize the DotSpatial plugin
    ///// </summary>
    //public override void Activate()
    //{
    //    //// Add Menu or Ribbon buttons.
    //    //AddServiceDropDown(App.HeaderControl);

    //    //_optionsAction = new SimpleActionItem("Configure", delegate
    //    //{
    //    //    var p = CurrentProvider;
    //    //    if (p == null) return;
    //    //    var cf = p.Configure;
    //    //    if (cf != null)
    //    //    {
    //    //        if (cf())
    //    //        {
    //    //            // Update map if configuration changed
    //    //            EnableBasemapFetching(p);
    //    //        }
    //    //    }
    //    //});

    //    //_serviceDropDown.SelectedValueChanged += ServiceSelected;
    //    //_serviceDropDown.SelectedItem = _emptyProvider;

    //    ////Add handlers for saving/restoring settings
    //    //App.SerializationManager.Serializing += SerializationManagerSerializing;
    //    //App.SerializationManager.Deserializing += SerializationManagerDeserializing;
    //    //App.SerializationManager.NewProjectCreated += SerializationManagerNewProject;


    //    //base.Activate();
    //}

    ///// <summary>
    ///// Fires when the plugin should become inactive
    ///// </summary>
    //public override void Deactivate()
    //{
    //    App.HeaderControl.RemoveAll();

    //    DisableBasemapLayer();

    //    //remove handlers for saving/restoring settings
    //    App.SerializationManager.Serializing -= SerializationManagerSerializing;
    //    App.SerializationManager.Deserializing -= SerializationManagerDeserializing;
    //    App.SerializationManager.NewProjectCreated -= SerializationManagerNewProject;

    //    base.Deactivate();
    //}

    
    public void SetCurrentProvider(string providerName)
    {
      if (this.providerList.ContainsKey(providerName))
      {
        CurrentProvider = this.providerList[providerName];
      }
      else
      {
        CurrentProvider = this.emptyProvider;
      }

      ServiceSelected();
    }

    public void AddServiceProvidersToComboItem(IMapComboToolbarItem menuItem)
    {
      this.emptyProvider = new ServiceProvider(Resources.Resources.None);
      menuItem.AddComboItem(this.emptyProvider.ToString());

      // Default providers
      foreach (var provider in ServiceProviderFactory.GetDefaultServiceProviders())
      {
        string pid = provider.ToString();
        menuItem.AddComboItem(pid);
        this.providerList[pid] = provider;
      }

      // "Other" provider
      var otherItem = ServiceProviderFactory.Create(Other);
      string oid = otherItem.ToString();
      menuItem.AddComboItem(oid);
      this.providerList[oid] = otherItem;
    }

    #endregion

    #region Methods

    private ServiceProvider CurrentProvider { get; set; }

    private void AddBasemapLayerToMap()
    {
      //if (!InsertBaseMapLayer(this.theMap.MapFrame))
      //{
        this.theMap.MapFrame.Layers.Insert(this.theMap.MapFrame.Layers.Count - 1, this.baseMapLayer);
      //}
    }

    private bool InsertBaseMapLayer(IMapGroup group)
    {
      for (var i = @group.Layers.Count-1; i >= 0; i--)
      {
        var layer = @group.Layers[i];
        var childGroup = layer as IMapGroup;
        if (childGroup != null)
        {
          var ins = InsertBaseMapLayer(childGroup);
          if (ins) return true;
        }

        if (layer is IMapPointLayer || layer is IMapLineLayer)
        {
          var grp = layer.GetParentItem() as DotSpatial.Symbology.IGroup;
          if (grp != null)
          {
            grp.Insert(i, this.baseMapLayer);
            return true;
          }
        }
      }
      return false;
    }

    //private void AddOpaticyDropDown(IHeaderControl header)
    //{
    //  string defaultOpacity = null;
    //  _opacityDropDown = new DropDownActionItem
    //                         {
    //                           AllowEditingText = true,
    //                           Caption = Properties.Resources.Opacity_Box_Text,
    //                           ToolTipText = Properties.Resources.Opacity_Box_ToolTip,
    //                           Width = 45,
    //                           Key = STR_KeyOpacityDropDown
    //                         };

    //  //Make some opacity settings
    //  for (var i = 100; i > 0; i -= 10)
    //  {
    //    var opacity = i.ToString(CultureInfo.InvariantCulture);
    //    if (i == 100)
    //    {
    //      defaultOpacity = opacity;
    //    }
    //    _opacityDropDown.Items.Add(opacity);
    //  }

    //  _opacityDropDown.GroupCaption = Properties.Resources.Panel_Name;
    //  _opacityDropDown.SelectedValueChanged += OpacitySelected;
    //  _opacityDropDown.RootKey = HeaderControl.HomeRootItemKey;

    //  //Add it to the Header
    //  header.Add(_opacityDropDown);
    //  if (defaultOpacity != null)
    //    _opacityDropDown.SelectedItem = defaultOpacity;
    //}

    private void BaseMapLayerRemoveItem(object sender, EventArgs e)
    {
      if (this.baseMapLayer != null)
      {
        this.baseMapLayer.RemoveItem -= BaseMapLayerRemoveItem;
      }
      this.baseMapLayer = null;
      //_serviceDropDown.SelectedItem = _emptyProvider;
      this.theMap.MapFrame.ViewExtentsChanged -= MapFrameExtentsChanged;
    }

    private void BwDoWork(object sender, DoWorkEventArgs e)
    {
      var worker = sender as BackgroundWorker;
      if (this.theMap != null && !this.busySet)
      {
        this.busySet = true;
        this.theMap.IsBusy = true;
      }

      if (worker != null && this.baseMapLayer != null)
      {
        if (worker.CancellationPending)
        {
          e.Cancel = true;
        }
        else
        {
          worker.ReportProgress(10);
          this.basemapWindow.HideError();
          UpdateStichedBasemap(e);
        }
      }
    }

    private void BwProgressChanged(object sender, ProgressChangedEventArgs e)
    {
      // Do we know what what our progress completion percent is (instead of 50)?
      //TODO: App.ProgressHandler.Progress("Loading Basemap ...", e.ProgressPercentage, "Loading Basemap ...");
    }

    private void BwRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
    {
      if (e.Cancelled)
      {
        bw.RunWorkerAsync();
        return;
      }

      if (this.theMap != null)
      {
        this.theMap.IsBusy = false;
        this.busySet = false;
        this.theMap.MapFrame.Invalidate();
        //TODO: App.ProgressHandler.Progress(String.Empty, 0, String.Empty);
      }
    }

    /// <summary>
    /// Changes the opacity of the current basemap image and invalidates the map
    /// </summary>
    private void ChangeBasemapOpacity()
    {
      MapFrameExtentsChanged(this, new ExtentArgs(this.theMap.ViewExtents)); //this forces the map layer to refresh

    }

    private void DisableBasemapLayer()
    {
      //RemoveBasemapLayer(this.baseMapLayer);
      //RemoveBasemapLayer(this.featureSetLayer);

      ////_optionsAction.Enabled = false;
      //this.baseMapLayer = null;
      //this.featureSetLayer = null;

      //this.theMap.MapFrame.ReprojectMapFrame(this.originalProjection);
      //this.theMap.ZoomToMaxExtent();

      //this.theMap.MapFrame.ViewExtentsChanged -= MapFrameExtentsChanged;
      //this.theMap.MapFrame.Invalidate();
    }

    private void ForceMaxExtentZoom()
    {
      //special case when there are no other layers in the map. Set map projection to WebMercator and zoom to max ext.
      this.theMap.MapFrame.ReprojectMapFrame(WebMercProj);

      // modifying the view extents didn't get the job done, so we are creating a new featureset.
      var fs = new FeatureSet(FeatureType.Point);
      fs.Features.Add(new Coordinate(TileCalculator.MinWebMercX, TileCalculator.MinWebMercY));
      fs.Features.Add(new Coordinate(TileCalculator.MaxWebMercX, TileCalculator.MaxWebMercY));

      fs.Projection = this.theMap.Projection;
      this.featureSetLayer = this.theMap.Layers.Add(fs);

      // hide the points that we are adding.
      this.featureSetLayer.LegendItemVisible = false;

      this.theMap.ZoomToMaxExtent();
    }

    private void EnableBasemapFetching(ServiceProvider provider)
    {
      // Zoom out as much as possible if there are no other layers.
      // reproject any existing layer in non-webMercator projection.
      if (this.theMap.Layers.Count == 0)
      {
        ForceMaxExtentZoom();
      }
      else if (!this.theMap.Projection.Equals(WebMercProj))
      {
        //TODO: figure out what to do here

        //get original view extents
        //TODO: App.ProgressHandler.Progress(String.Empty, 0, "Reprojecting Map Layers...");
        double[] viewExtentXY = { this.theMap.ViewExtents.MinX, this.theMap.ViewExtents.MinY, this.theMap.ViewExtents.MaxX, this.theMap.ViewExtents.MaxY };
        double[] viewExtentZ = { 0.0, 0.0 };

        //reproject view extents
        Reproject.ReprojectPoints(viewExtentXY, viewExtentZ, this.theMap.Projection, WebMercProj, 0, 2);

        //set the new map extents
        this.theMap.ViewExtents = new Extent(viewExtentXY);

        //if projection is not WebMercator - reproject all layers:
        this.originalProjection = this.theMap.Projection;
        this.theMap.MapFrame.ReprojectMapFrame(WebMercProj);

        //TODO: App.ProgressHandler.Progress(String.Empty, 0, "Loading Basemap...");
      }


      EnableBasemapLayer();
      this.tileManager = new TileManager(provider);
      RunOrCancelBW();
    }

    private void EnableBasemapLayer()
    {
      if (this.baseMapLayer == null)
      {
        //Need to first initialize and add the basemap layer synchronously (it will fail if done in
        // another thread.

        //First create a temporary imageData with an Envelope (otherwise adding to the map will fail)
        var tempImageData = new InRamImageData(Resources.Resources.nodata, new Extent(1, 1, 2, 2));

        this.baseMapLayer = new MapImageLayer(tempImageData)
                        {
                          Projection = this.theMap.Projection,
                          LegendText = Resources.Resources.Legend_Title,
                          LegendItemVisible = false
                        };

        this.baseMapLayer.RemoveItem += BaseMapLayerRemoveItem;
        AddBasemapLayerToMap();
      }

      this.theMap.MapFrame.ViewExtentsChanged -= MapFrameExtentsChanged;
      this.theMap.MapFrame.ViewExtentsChanged += MapFrameExtentsChanged;
    }

    private void RunOrCancelBW()
    {
      if (bw.IsBusy != true)
        bw.RunWorkerAsync();
      else
        bw.CancelAsync();
    }

    private void MapFrameExtentsChanged(object sender, DotSpatial.Data.ExtentArgs e)
    {
      RunOrCancelBW();
    }

    //private void OpacitySelected(object sender, SelectedValueChangedEventArgs e)
    //{
    //  if (this.baseMapLayer == null)
    //    return;

    //  Int16 opacityInt;

    //  //Check to make sure the text in the box is an integer and we are in the range
    //  if (!Int16.TryParse(e.SelectedItem as string, out opacityInt) || opacityInt > 100 || opacityInt < 0)
    //  {
    //    opacityInt = 100;
    //    _opacityDropDown.SelectedItem = opacityInt;
    //  }
    //  _opacity = opacityInt;
    //  ChangeBasemapOpacity();
    //}

    /// <summary>
    /// Finds and removes the online basemap layer
    /// </summary>
    private void RemoveBasemapLayer(IMapLayer Layer)
    {
      //attempt to remove from the top-level
      if (this.theMap.Layers.Remove(Layer)) return;

      //Remove from other groups if the user has moved it
      foreach (var group in this.theMap.Layers.OfType<IMapGroup>())
      {
        group.Remove(Layer);
      }
    }

    private void ServiceSelected()
    {
      var p = CurrentProvider;
      if (p == null || p.Name == Resources.Resources.None)
      {
        //this.opacityMenuItem.OnButtonStatusChanged(this, new Window.ToolbarItemEvent(false));
        DisableBasemapLayer();
      }
      else
      {
        //this.opacityMenuItem.OnButtonStatusChanged(this, new Window.ToolbarItemEvent(true));
        if (p.NeedConfigure)
        {
          if (p.Configure != null)
          {
            p.Configure();
          }
        }
        EnableBasemapFetching(p);
      }
    }

    /// <summary>
    /// Main method of this plugin: gets the tiles from the TileManager, stitches them together, and adds the layer to the map.
    /// </summary>
    private void UpdateStichedBasemap(DoWorkEventArgs e)
    {
      var map = this.theMap as Map;
      if (map == null) return;

      var bwProgress = (Func<int, bool>)(delegate(int p)
      {
        bw.ReportProgress(p);
        if (bw.CancellationPending)
        {
          e.Cancel = true;
          return false;
        }
        return true;
      });

      var rectangle = map.Bounds;
      var webMercExtent = map.ViewExtents;

      //Clip the reported Web Merc Envelope to be within possible Web Merc extents
      //  This fixes an issue with Reproject returning bad results for very large (impossible) web merc extents reported from the Map
      var webMercTopLeftX = TileCalculator.Clip(webMercExtent.MinX, TileCalculator.MinWebMercX, TileCalculator.MaxWebMercX);
      var webMercTopLeftY = TileCalculator.Clip(webMercExtent.MaxY, TileCalculator.MinWebMercY, TileCalculator.MaxWebMercY);
      var webMercBtmRightX = TileCalculator.Clip(webMercExtent.MaxX, TileCalculator.MinWebMercX, TileCalculator.MaxWebMercX);
      var webMercBtmRightY = TileCalculator.Clip(webMercExtent.MinY, TileCalculator.MinWebMercY, TileCalculator.MaxWebMercY);

      if (!bwProgress(25)) return;

      //Get the web mercator vertices of the current map view
      var mapVertices = new[] { webMercTopLeftX, webMercTopLeftY, webMercBtmRightX, webMercBtmRightY };
      double[] z = { 0, 0 };

      //Reproject from web mercator to WGS1984 geographic
      Reproject.ReprojectPoints(mapVertices, z, WebMercProj, Wgs84Proj, 0, mapVertices.Length / 2);
      var geogEnv = new Envelope(mapVertices[0], mapVertices[2], mapVertices[1], mapVertices[3]);

      if (!bwProgress(40)) return;

      //Grab the tiles
      var tiles = this.tileManager.GetTiles(geogEnv, rectangle, bw);
      if (!bwProgress(50)) return;

      //Stitch them into a single image
      var stitchedBasemap = TileCalculator.StitchTiles(tiles.Bitmaps, _opacity);
      var tileImage = new InRamImageData(stitchedBasemap) { Projection = this.baseMapLayer.Projection };

      //report progress and check for cancel
      if (!bwProgress(70)) return;

      // Tiles will have often slightly different bounds from what we are displaying on screen
      // so we need to get the top left and bottom right tiles' bounds to get the proper extent
      // of the tiled image
      var tileVertices = new[]
            {
                tiles.TopLeftTile.TopLeft().X, tiles.TopLeftTile.TopLeft().Y,
                tiles.BottomRightTile.BottomRight().X, tiles.BottomRightTile.BottomRight().Y
            };

      //Reproject from WGS1984 geographic coordinates to web mercator so we can show on the map
      Reproject.ReprojectPoints(tileVertices, z, Wgs84Proj, WebMercProj, 0, tileVertices.Length / 2);

      tileImage.Bounds = new RasterBounds(stitchedBasemap.Height, stitchedBasemap.Width,
          new Extent(tileVertices[0], tileVertices[3], tileVertices[2], tileVertices[1]));

      //report progress and check for cancel
      if (!bwProgress(90)) return;

      try
      {
        this.baseMapLayer.Image = tileImage;
      }
      catch (Exception ex)
      {
        this.basemapWindow.ShowError();
      }

      // ReSharper disable RedundantJumpStatement
      //report progress and check for cancel
      if (!bwProgress(99)) return;
      // ReSharper restore RedundantJumpStatement
    }

    #endregion

    internal ProjectionInfo GetProjection()
    {
      return WebMercProj;
    }

    internal void AddBasemapWindow(IBasemapWindow basemapWindow)
    {
      this.basemapWindow = basemapWindow;
    }
  }
}
