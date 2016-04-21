using RVRMeander.Core.Result;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Project
{
  [Export(typeof(Core.Window.IInteractiveWindow))]
  [Export(typeof(Core.Window.IToolbarItem))]
  public partial class ResultManagerForm : Core.Window.DockableWindow, Core.Window.IToolbarItem,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {

    [Import]
    private Core.Result.IResultManager resultMgr;

    private Core.Events.IEventManager eventMgr;
    private IResultSet currentResultSet;

    [ImportingConstructor]
    public ResultManagerForm([Import]Core.Events.IEventManager eventMgr)
    {
      InitializeComponent();

      this.eventMgr = eventMgr;

      this.currentResultSet = null;

      this.eventMgr.AddListener(this);
    }

    #region IToolbarItem members

    public string Caption
    {
      get { return "Results"; }
    }

    public string Group
    {
      get { return Const.Group_Name; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar_Name; }
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

    public bool IsMapInteractive
    {
      get { return false; }
    }

    public bool CheckOnClick
    {
      get { return false; }
    }

    public Image Image
    {
      get { return null; }
    }

    public Image SmallImage
    {
      get { return null; }
    }

    public void ToolbarItemInitialized()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    #endregion

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, true));

      this.comboSim.Items.Clear();
      this.comboSim.Items.AddRange((from x in this.resultMgr.ResultSetIds select new ComboItem(x, this.resultMgr.GetResultSetName(x))).ToArray());
    }

    class ComboItem
    {
      public ComboItem(string id, string name)
      {
        Id = id;
        Name = name;
      }

      public string Id { get; set; }
      public string Name { get; set; }
      public override string ToString()
      {
        return Name;
      }
    }

    private void listResults_SelectedIndexChanged(object sender, EventArgs e)
    {
      List<string> selectedLayers = new List<string>();
      foreach (string layerName in this.listResults.SelectedItems)
      {
        selectedLayers.Add(layerName);
      }

      this.eventMgr.Publish(new Core.Result.Events.ResultLayerSelected(this.currentResultSet, selectedLayers));
    }

    private void listResults_DoubleClick(object sender, EventArgs e)
    {
    }

    private void comboSim_SelectedIndexChanged(object sender, EventArgs e)
    {
      var item = this.comboSim.SelectedItem as ComboItem;
      if (item == null)
        return;

      IResultSet set;
      if (!this.resultMgr.LoadResultSet(item.Id, out set))
      {
        MessageBox.Show("Unable to load result set with ID " + item.Id + " and name " + item.Name);
        return;
      }

      this.currentResultSet = set;

      this.listResults.Items.Clear();
      this.listResults.Items.AddRange(set.LayerNames.ToArray());

      this.eventMgr.Publish(new Core.Result.Events.ResultSetSelected(set));
    }

    private void tsbClear_Click(object sender, EventArgs e)
    {
      this.listResults.ClearSelected();
    }
  }
}
