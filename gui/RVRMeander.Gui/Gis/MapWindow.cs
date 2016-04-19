using DotSpatial.Controls;
using DotSpatial.Data;
using DotSpatial.Topology;
using RVRMeander.Core;
using RVRMeander.Core.Project;
using RVRMeander.Core.Window;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace RVRMeander.Gui.Gis
{
  [System.ComponentModel.Composition.Export(typeof(IInteractiveWindow))]
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItem))]
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItemArray))]
  [System.ComponentModel.Composition.Export(typeof(WebMap.IBasemapWindow))]
  [System.ComponentModel.Composition.Export(typeof(IMapWindow))]
  [System.ComponentModel.Composition.Export(typeof(MapWindow))]
  public partial class MapWindow : DockableWindow, IMapWindow, Core.Window.IToolbarItem, Core.Window.IToolbarItemArray, WebMap.IBasemapWindow,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private const string Section_Layers = "LAYERS";

    private List<IToolbarItem> toolItems;
    private MapTool selectMapTool;

    public string Title
    {
      get
      {
        return this.Text;
      }
      set
      {
        this.Text = value;
      }
    }

    public event MapMouseMove MapMouseMove = delegate { };
    private Core.Events.IEventManager eventMgr;
    private IProjectManager projectMgr;

    [System.ComponentModel.Composition.ImportingConstructor]
    public MapWindow([Import]Core.Events.IEventManager eventMgr, [Import]IProjectManager projectMgr)
    {
      InitializeComponent();

      this.eventMgr = eventMgr;
      this.projectMgr = projectMgr;

      this.map.Legend = this.legend;
      this.map.GeoMouseMove += map_GeoMouseMove;

      this.toolItems = new List<Core.Window.IToolbarItem>();
      SetupToolItems();

      this.eventMgr.AddListener(this);
    }

    void map_GeoMouseMove(object sender, DotSpatial.Controls.GeoMouseArgs e)
    {
      MapMouseMove(sender, e);
    }

    public void AddMapGroup(IRvrMapGroup group)
    {
      DotSpatialMapGroup g = group as DotSpatialMapGroup;
      if (g != null)
      {
        this.map.Layers.Add(g.MapGroup);
      }
    }

    public double[,] GetLayerPoints(string layerName)
    {
      IMapFeatureLayer layer;
      if (!TryGetMapLayer(layerName, out layer) || layer.DataSet.Features.Count == 0)
      {
        return new double[0, 0];
      }

      // Right now we only handle the first feature

      var feature = layer.DataSet.Features[0];

      var data = new double[feature.Coordinates.Count, 2];
      
      for (int i = 0; i < feature.Coordinates.Count; i++)
      {
        var point = feature.Coordinates[i];
        data[i, 0] = point.X;
        data[i, 1] = point.Y;
      }

      return data;
    }

    private bool TryGetMapLayer(string layerName, out IMapFeatureLayer mapFeatureLayer)
    {
      mapFeatureLayer = null;

      foreach (var layer in this.map.Layers)
      {
        if (layer.LegendText.ToLower() == layerName.ToLower())
        {
          IMapFeatureLayer temp = layer as IMapFeatureLayer;
          if (temp != null)
          {
            mapFeatureLayer = temp;
            return true;
          }
        }
      }

      return false;
    }
    
    #region IToolbarItem Members

    public string Caption
    {
      get { return "Map"; }
    }

    public string Group
    {
      get { return Const.Group; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar; }
    }

    public new void Click()
    {
      Show(Core.Window.DockingState.Document);
    }

    public bool IsChecked
    {
      get;
      set;
    }

    public bool IsMapInteractive { get { return true; } }

    public bool IsButtonList { get { return false; } }

    public bool CheckOnClick
    {
      get { return false; }
    }

    public System.Drawing.Image Image
    {
      get { return null; }
    }
    public System.Drawing.Image SmallImage
    {
      get { return null; }
    }

    public void ToolbarItemInitialized()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    #endregion
	
    #region IToolbarItemArray methods

    public IEnumerable<IToolbarItem> ToolbarItems
    {
      get { return this.toolItems; }
    }

    private void SetupToolItems()
    {

      this.toolItems.Add(
        new MapTool((bool isChecked) =>
        {
          BeginInvoke(new MethodInvoker(() =>
          {
            if (isChecked)
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.ZoomIn;
            else
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
          }));
        })
        {
          Caption = "Zoom In",
          Group = this.Group,
          IsMapInteractive = true,
          Toolbar = this.Toolbar,
          IsChecked = false,
          Image = Icons.zoom_32xLG,
          SmallImage = Icons.zoom_16xLG,
          GroupedTools = this.toolItems,
        });
      this.toolItems.Add(
        new MapTool((bool isChecked) =>
        {
          BeginInvoke(new MethodInvoker(() =>
          {
            if (isChecked)
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.ZoomOut;
            else
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
          }));
        })
        {
          Caption = "Zoom Out",
          IsMapInteractive = true,
          Group = this.Group,
          Toolbar = this.Toolbar,
          IsChecked = false,
          Image = Icons.ZoomOut_32xlG,
          SmallImage = Icons.ZoomOut_16xlG,
          GroupedTools = this.toolItems,
        });
      this.toolItems.Add(
        new MapTool((bool isChecked) =>
        {
          BeginInvoke(new MethodInvoker(() =>
          {
            if (isChecked)
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.Pan;
            else
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
          }));
        })
        {
          Caption = "Pan",
          IsMapInteractive = true,
          Group = this.Group,
          Toolbar = this.Toolbar,
          IsChecked = false,
          GroupedTools = this.toolItems,
          Image = Icons.pan_32,
          SmallImage = Icons.pan_16,
        });
      this.toolItems.Add(
        new MapTool((bool isChecked) =>
        {
          BeginInvoke(new MethodInvoker(() =>
          {
            this.map.ZoomToMaxExtent();
          }));
        })
        {
          Caption = "Full Extent",
          IsMapInteractive = false,
          Group = this.Group,
          Toolbar = this.Toolbar,
          IsChecked = false,
          GroupedTools = this.toolItems,
          Image = Icons.fullextent_32,
          SmallImage = Icons.fullextent_16,
        });
      this.toolItems.Add(
        new MapTool((bool isChecked) =>
        {
          BeginInvoke(new MethodInvoker(() =>
          {
            if (isChecked)
            {
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.Info;
              // Except for function mode "none" allow scrolling
              var scroll = this.map.MapFunctions.Find(f => f.GetType() == typeof(DotSpatial.Controls.MapFunctionZoom));
              this.map.ActivateMapFunction(scroll);
            }
            else
            {
              this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
            }
          }));
        })
        {
          Caption = "Info",
          IsMapInteractive = true,
          Group = this.Group,
          Toolbar = this.Toolbar,
          IsChecked = false,
          GroupedTools = this.toolItems,
          Image = Icons.info_32,
          SmallImage = Icons.info_16,
        });
      //this.toolItems.Add(
      //  new MapTool((bool isChecked) =>
      //  {
      //    BeginInvoke(new MethodInvoker(() =>
      //    {
      //      if (isChecked)
      //      {
      //        this.SetMapFunction(new CoordClickFunction(this.statusBar, this));
      //      }
      //      else
      //      {
      //        this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
      //      }
      //    }));
      //  })
      //  {
      //    Caption = "X, Y",
      //    IsMapInteractive = true,
      //    Group = this.Group,
      //    Toolbar = this.Toolbar,
      //    IsChecked = false,
      //    Image = Icons.coord_32_01,
      //    SmallImage = Icons.coord_16_01,
      //    GroupedTools = this.toolItems,
      //  });

      this.selectMapTool = new MapTool((bool isChecked) =>
      {
        BeginInvoke(new MethodInvoker(() =>
        {
          if (isChecked)
          {
            this.map.FunctionMode = DotSpatial.Controls.FunctionMode.Select;

            //this.map.ActivateMapFunction(this.selectFunction);

            //// Except for function mode "none" allow scrolling
            //var scroll = this.map.MapFunctions.Find(f => f.GetType() == typeof(MapFunctionZoom));
            //this.map.ActivateMapFunction(scroll);

            //this.eventMgr.Publish(new Selections.Events.MapSelectionEvent() { IsStart = true });
            ////this.map.SelectionChanged += map_SelectionChanged;
            ////this.mapLayerHelper.SelectionChanged += group_SelectionChanged;
          }
          else
          {
            this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
            //ClearSelection();
          }
        }));
      })
      {
        Caption = "Select",
        IsMapInteractive = true,
        Group = this.Group,
        Toolbar = this.Toolbar,
        IsChecked = false,
        GroupedTools = this.toolItems,
        Image = Icons.select_32,
        SmallImage = Icons.select_16,
      };

      this.toolItems.Add(this.selectMapTool);
    }

    #endregion

    public void HideError()
    {
      //throw new NotImplementedException();
    }

    public void ShowError()
    {
      //throw new NotImplementedException();
    }

    public DotSpatial.Controls.IMap GetMap()
    {
      return this.map;
    }

    public void SetMousePosition(double p1, double p2)
    {
      //throw new NotImplementedException();
    }

    public void SetExtents(DotSpatial.Data.Extent extent)
    {
      //throw new NotImplementedException();
    }

    public bool ImportShapefile(string filePath, string name, string p)
    {
      return ImportShapefile(filePath, name, p, false);
    }

    public bool ImportShapefile(string filePath, string name, string p, bool addedFromProject)
    {
      IFeatureSet fs = null;
      try
      {
        fs = FeatureSet.Open(filePath);
      }
      catch
      {
        fs = null;
        return false;
      }

      var mapLayer = GetMapLayerFromFeatureSet(fs);
      mapLayer.LegendText = name;
      mapLayer.Reproject(this.map.MapFrame.Projection);

      this.map.Layers.Add(mapLayer);

      if (!addedFromProject)
      {
        this.projectMgr.SetProperty(Section_Layers, mapLayer.LegendText, filePath);
      }

      return true;
    }

    public bool ImportSQLite(string filePath, string name)
    {
      Model.Geometry.SpatiaLite.Database db = new Model.Geometry.SpatiaLite.Database(filePath);
      List<IFeatureSet> dataset = null;

      bool ok = true;
      ok = db.TryLoadSpatialDataset(out dataset);

      if (!ok)
      {
        MessageBox.Show("Unable to load spatial dataset from file '" + filePath + "': " + db.ErrorMessage);
        return false;
      }
      
      DotSpatial.Controls.MapGroup mapGroup = new DotSpatial.Controls.MapGroup();
      mapGroup.LegendText = name;
      mapGroup.SelectionEnabled = true;

      foreach (var fs in dataset)
      {
        var mapLayer = GetMapLayerFromFeatureSet(fs);
        mapLayer.SelectionEnabled = true;
        //group.Layers.Add(layer);
        mapGroup.Layers.Add(mapLayer);
        mapLayer.SetParentItem(mapGroup);
        var id = Guid.NewGuid().ToString();
        //this.idMapLayer[id] = mapLayer;
        //this.idMapLayerInverse[mapLayer] = id;
        mapGroup.Layers[mapGroup.Layers.Count - 1].LegendText = fs.Name;
      }
      
      foreach (var layer in mapGroup.Layers)
      {
        //if (!layer.Projection.Equals(this.map.MapFrame.Projection))
        // If we don't reproject upon load for every layer, then the layers don't display properly.
        layer.Reproject(this.map.MapFrame.Projection);

        IMapFeatureLayer fl = layer as IMapFeatureLayer;
        if (fl != null)
        {
          //LayerAdded(fl);
        }
      }

      this.map.Layers.Add(mapGroup);
      //var id = Guid.NewGuid().ToString();
      //this.idMapGroupInverse[mapGroup] = id;
      //this.idMapGroup[id] = mapGroup;
      
      mapGroup.IsExpanded = false;
      //if (!this.mapSymbolHelper.LoadSymbology(mg, file, this))
      //{
      //  return false;
      //}

      return true;
    }

    private DotSpatial.Controls.IMapFeatureLayer GetMapLayerFromFeatureSet(IFeatureSet featureSet)
    {
      if (featureSet == null)
        return null;

      IMapFeatureLayer layer = null;
      if (featureSet.FeatureType == FeatureType.Point || featureSet.FeatureType == FeatureType.MultiPoint)
      {
        layer = new MapPointLayer(featureSet);
      }
      else if (featureSet.FeatureType == FeatureType.Line)
      {
        layer = new MapLineLayer(featureSet);
      }
      else if (featureSet.FeatureType == FeatureType.Polygon)
      {
        layer = new MapPolygonLayer(featureSet);
      }

      return layer;
    }

    public string GetLayerPath(string layerName)
    {
      return this.projectMgr.GetProperty(Section_Layers, layerName);
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.map.Layers.Clear();
      Show(DockingState.Document);

      bool isEnabled = theEvent != null && theEvent.PackagePath.Length > 0;
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, isEnabled));

      if (isEnabled)
      {
        string[] layerNames = this.projectMgr.GetProperties(Section_Layers);
        foreach (var layerName in layerNames)
        {
          string path = this.projectMgr.GetProperty(Section_Layers, layerName);
          if (File.Exists(path))
          {
            ImportShapefile(path, layerName, "", true);
          }
        }
      }
    }
  }

  public delegate void MapMouseMove(object sender, DotSpatial.Controls.GeoMouseArgs e);
}
