using DotSpatial.Symbology;
using RVRMeander.Core;
using RVRMeander.Core.Project;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Layers
{
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IInteractiveWindow))]
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItem))]
  public partial class LayerDefinition : Core.Window.DockableWindow, Core.Window.IInteractiveWindow, Core.Window.IToolbarItem,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private Gis.MapWindow mapWindow;
    private IProjectManager projectMgr;
    private Core.Events.IEventManager eventMgr;
    private string riverName;
    private string valleyName;

    [ImportingConstructor]
    public LayerDefinition([Import]Gis.MapWindow mapWindow, [Import]IProjectManager projectMgr, [Import]Core.Events.IEventManager eventMgr)
    {
      InitializeComponent();

      this.eventMgr = eventMgr;
      this.projectMgr = projectMgr;
      this.mapWindow = mapWindow;

      var map = this.mapWindow.GetMap();
      map.MapFrame.LayerAdded += MapFrame_LayerAdded;
      map.MapFrame.LayerRemoved += MapFrame_LayerRemoved;

      this.valleyName = "";
      this.riverName = "";

      this.eventMgr.AddListener(this);
    }

    void MapFrame_LayerRemoved(object sender, DotSpatial.Symbology.LayerEventArgs e)
    {
      RemoveLayer(this.cmbSelectRiverCenterline, e.Layer);
      RemoveLayer(this.cmbSelectValleyCenterline, e.Layer);
    }

    private static void RemoveLayer(ComboBox comboBox, ILayer removedLayer)
    {
      object itemToRemove = null;
      foreach (var item in comboBox.Items)
      {
        var layer = item as string;
        if (layer == removedLayer.LegendText)
        {
          itemToRemove = item;
          break;
        }
      }
      if (itemToRemove == comboBox.SelectedItem)
      {
        comboBox.SelectedItem = null;
        comboBox.SelectedText = "";
        comboBox.SelectedIndex = -1;
      }
      if (itemToRemove != null)
      {
        comboBox.Items.Remove(itemToRemove);
      }
    }

    void MapFrame_LayerAdded(object sender, DotSpatial.Symbology.LayerEventArgs e)
    {
      this.valleyName = this.projectMgr.GetProperty("valley");
      this.riverName = this.projectMgr.GetProperty("river");

      var curItem = this.cmbSelectRiverCenterline.SelectedItem;
      this.cmbSelectRiverCenterline.Items.Add(e.Layer.LegendText);
      if (e.Layer.LegendText == this.riverName)
      {
        this.cmbSelectRiverCenterline.SelectedItem = this.riverName;
      }
      this.cmbSelectValleyCenterline.Items.Add(e.Layer.LegendText);
      if (e.Layer.LegendText == this.valleyName)
      {
        this.cmbSelectValleyCenterline.SelectedItem = this.valleyName;
      }
    }

    #region IToolbarItem Members

    public string Caption
    {
      get { return "Definition"; }
    }

    public string Group
    {
      get { return "Layer Properties"; }
    }

    public string Toolbar
    {
      get { return "Layer Definition"; }
    }

    public new void Click()
    {
      Show(Core.Window.DockingState.DockRight);
    }

    public bool IsChecked
    {
      get;
      set;
    }

    public bool IsMapInteractive { get { return false; } }

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

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      bool isEnabled = theEvent != null && theEvent.PackagePath.Length > 0;
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, isEnabled));

      //if (isEnabled)
      //{
      //  this.valleyName = this.projectMgr.GetProperty("valley");
      //  this.riverName = this.projectMgr.GetProperty("river");

      //  foreach (var item in this.cmbSelectRiverCenterline.Items)
      //  {
      //    if (item.ToString() == valleyName)
      //    {
      //      this.cmbSelectRiverCenterline.SelectedItem = item;
      //      break;
      //    }
      //  }
      //  foreach (var item in this.cmbSelectValleyCenterline.Items)
      //  {
      //    if (item.ToString() == valleyName)
      //    {
      //      this.cmbSelectValleyCenterline.SelectedItem = item;
      //      break;
      //    }
      //  }
      //}
    }

    #endregion

    private void btnApply_Click(object sender, EventArgs e)
    {
      if (this.cmbSelectValleyCenterline.SelectedItem == null && this.cmbSelectRiverCenterline.SelectedItem == null)
      {
        return;
      }

      if (this.cmbSelectRiverCenterline.SelectedItem != null)
      {
        var riverLayer = this.cmbSelectRiverCenterline.SelectedItem as string;
        if (riverLayer != null)
        {
          this.projectMgr.SetProperty("river", riverLayer);
        }
      }
      if (this.cmbSelectValleyCenterline.SelectedItem != null)
      {
        var valleyLayer = this.cmbSelectValleyCenterline.SelectedItem as string;
        if (valleyLayer != null)
        {
          this.projectMgr.SetProperty("valley", valleyLayer);
        }
      }
    }

  }
}
