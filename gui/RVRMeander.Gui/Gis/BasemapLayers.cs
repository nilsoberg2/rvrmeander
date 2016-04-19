using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  [Export(typeof(Core.Window.IComboToolbarItem))]
  class BasemapLayers : IMapComboToolbarItem
  {

    public event Core.Window.Events.ComboItemsAddedEventHandler ComboItemsAdded = delegate { };
    public event Core.Window.Events.ComboItemSelectedTextChangedEventHandler ComboSelectedTextChanged = delegate { };


    private List<string> comboItems;
    private const string None_String = "--NONE--";
    private IMapWindow mapWindow;
    private WebMap.Util webMap;
    private WebMap.IBasemapWindow basemapWin;
    private DotSpatial.Controls.IMap mainMap;
    private Core.Window.IInteractiveWindowContainer mainWindow;

    [ImportingConstructor]
    public BasemapLayers([Import]IMapWindow mapWindow, [Import]WebMap.IBasemapWindow basemapWin, [Import]Core.Window.IInteractiveWindowContainer mainWindow)
    {
      this.mapWindow = mapWindow;
      this.mainWindow = mainWindow;

      this.comboItems = new List<string>();

      IsChecked = false;

      this.basemapWin = basemapWin;
      
      MapWindow mw = mapWindow as MapWindow;
      if (mw != null)
      {
        this.mainMap = mw.GetMap();
        this.mainMap.MapFrame.ViewExtentsChanged += MapFrame_ViewExtentsChanged;

        mw.MapMouseMove += mainMapWin_MapMouseMove;

        this.webMap = new WebMap.Util(this.basemapWin.GetMap());
        this.webMap.AddBasemapWindow(this.basemapWin);
        this.webMap.AddServiceProvidersToComboItem(this);
      }
      else
      {
        this.webMap = null;
        this.mainMap = null;
      }
    }

    void mainMapWin_MapMouseMove(object sender, DotSpatial.Controls.GeoMouseArgs e)
    {
      double[] xy = new double[] { e.GeographicLocation.X, e.GeographicLocation.Y };
      double[] z = new double[] { 0 };
      DotSpatial.Projections.Reproject.ReprojectPoints(xy, z, this.mainMap.MapFrame.Projection, this.webMap.GetProjection(), 0, 1);
      this.basemapWin.SetMousePosition(xy[0], xy[1]);
    }

    void MapFrame_ViewExtentsChanged(object sender, DotSpatial.Data.ExtentArgs e)
    {
      Debug.WriteLine(e.Extent.ToString());
      if (this.mainMap != null && this.webMap != null)
      {
        double[] xy = new double[] { e.Extent.MinX, e.Extent.MinY, e.Extent.MaxX, e.Extent.MaxY };
        double[] z = new double[] { 0, 0 };
        DotSpatial.Projections.Reproject.ReprojectPoints(xy, z, this.mainMap.MapFrame.Projection, this.webMap.GetProjection(), 0, 2);
        this.basemapWin.SetExtents(new DotSpatial.Data.Extent(xy[0], xy[1], xy[2], xy[3]));
      }
    }

    public IEnumerable<string> ComboItems
    {
      get { return this.comboItems; }
    }

    public void ComboItemClicked(string comboItemName)
    {
      if (this.webMap != null)
      {
        this.webMap.SetCurrentProvider(comboItemName);
        if (comboItemName == None_String)
        {
          this.basemapWin.Hide();
        }
        else
        {
          //this.mainWindow.Show(this.basemapWin, Core.Window.DockingState.Float);
        }
      }
    }

    public void OnComboSelectedTextChanged(object o, Core.Window.Events.ComboSelectedTextChangedEventArgs e)
    {
      ComboSelectedTextChanged(o, e);
    }

    public string Caption
    {
      get { return ""; }
    }

    public string Group
    {
      get { return Const.Basemap_Group; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar; }
    }

    public void Click()
    {
    }

    public bool IsChecked { get; set; }

    public bool IsMapInteractive
    {
      get { return false; }
    }

    public bool CheckOnClick { get { return false; } }

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
    }

    public void AddComboItem(string serviceProvider)
    {
      this.comboItems.Add(serviceProvider);
      ComboItemsAdded(this, new Core.Window.Events.ComboItemsAddedEvent());
    }

  }
}
