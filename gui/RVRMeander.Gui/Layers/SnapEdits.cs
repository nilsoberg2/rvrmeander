using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Layers
{
  [Export(typeof(Core.Window.IToolbarItem))]
  class SnapEdits : Core.Window.IToolbarItem, ISnapSettings,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public SnapEdits([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;
      this.eventMgr.AddListener(this);
    }

    public string Caption
    {
      get { return "Snap Edits"; }
    }

    public string Group
    {
      get { return "Editing"; }
    }

    public string Toolbar
    {
      get { return "Layer Definition"; }
    }

    public void Click()
    {
      if (IsChecked)
      {

      }
      else
      {

      }
    }

    public bool IsChecked { get; set; }

    public bool IsMapInteractive
    {
      get { return false; }
    }

    public bool CheckOnClick
    {
      get { return true; }
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
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, theEvent != null && theEvent.PackagePath.Length > 0));
    }

    public bool Snap
    {
      get { return IsChecked; }
    }
  }
}
