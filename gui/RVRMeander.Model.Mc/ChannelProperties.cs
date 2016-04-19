using RVRMeander.Core.Project;
using RVRMeander.Core.Window;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Model.Mc
{
  [Export(typeof(IToolbarItem))]
  [Export(typeof(IInteractiveWindow))]
  public partial class ChannelProperties : DockableWindow, IToolbarItem, IInteractiveWindow, Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private const string File_Section = "MC";
    private const string File_Width = "Width";
    private const string File_Flow = "Flow";
    private const string File_SedimentSize = "SedimentSize";
    private const string File_ValleySlope = "ValleySlope";
    private const string File_ManningsN = "ManningsN";
    private const string File_UpstreamBedElev = "UpstreamBedElev";

    [Import]
    IProjectManager projectMgr;
    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public ChannelProperties([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;

      InitializeComponent();

      IsChecked = false;
      IsMapInteractive = false;
      CheckOnClick = false;

      this.eventMgr.AddListener(this);
    }

    #region IToolbarItem Members

    public string Caption
    {
      get { return "Properties"; }
    }

    public string Group
    {
      get { return Const.Group_Mc; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar_Mc; }
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
      get;
      private set;
    }

    public bool CheckOnClick
    {
      get;
      private set;
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
    }

    #endregion

    private void btnSave_Click(object sender, EventArgs e)
    {
      this.projectMgr.SetProperty(File_Section, File_Width, this.txtWidth.Text);
      this.projectMgr.SetProperty(File_Section, File_Flow, this.txtFlow.Text);
      this.projectMgr.SetProperty(File_Section, File_SedimentSize, this.txtSedimentSize.Text);
      this.projectMgr.SetProperty(File_Section, File_ValleySlope, this.txtValleySlope.Text);
      this.projectMgr.SetProperty(File_Section, File_ManningsN, this.txtManningsN.Text);
      this.projectMgr.SetProperty(File_Section, File_UpstreamBedElev, this.txtUSBedElevation.Text);
      Hide();
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.txtWidth.Text = this.projectMgr.GetProperty(File_Section, File_Width);
      this.txtFlow.Text = this.projectMgr.GetProperty(File_Section, File_Flow);
      this.txtSedimentSize.Text = this.projectMgr.GetProperty(File_Section, File_SedimentSize);
      this.txtValleySlope.Text = this.projectMgr.GetProperty(File_Section, File_ValleySlope);
      this.txtManningsN.Text = this.projectMgr.GetProperty(File_Section, File_ManningsN);
      this.txtUSBedElevation.Text = this.projectMgr.GetProperty(File_Section, File_UpstreamBedElev);
    }
  }
}
