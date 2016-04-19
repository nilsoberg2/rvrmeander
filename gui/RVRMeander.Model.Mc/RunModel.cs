using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  [Export(typeof(Core.Window.IToolbarItem))]
  class RunModel : Core.Window.IToolbarItem, Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    [Import]
    private Core.Window.IStatusBar statusBar;

    [Import]
    private Core.Project.IProjectManager projectMgr;

    [Import]
    private ChannelProperties propsWindow;

    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public RunModel([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;

      IsChecked = false;
      IsMapInteractive = false;
      CheckOnClick = false;

      this.eventMgr.AddListener(this);
    }

    #region IToolbarItem Members

    public string Caption
    {
      get { return "Run Sim"; }
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
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));

      ThreadPool.QueueUserWorkItem(new WaitCallback(delegate
        {
          for (int i = 0; i < 100; i++)
          {
            Thread.Sleep(250);
            this.statusBar.SetProgress(i);
          }

          this.statusBar.Reset();
          this.statusBar.SetMessage("Simulation Completed");
          this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, true));
        }));
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
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    #endregion


    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, true));
    }
  }
}
