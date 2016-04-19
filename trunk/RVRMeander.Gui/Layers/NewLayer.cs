using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Layers
{
  [Export(typeof(Core.Window.IMenuToolbarItem))]
  class NewLayer : Core.Window.IMenuToolbarItem,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private const string Layer_Point = "Point";
    private const string Layer_Line = "Line";

    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public NewLayer([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;
      this.eventMgr.AddListener(this);
    }

    public string Caption
    {
      get { return "New Layer"; }
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
      get { return false; }
    }

    public System.Drawing.Image Image
    {
      get { return UiImages.new_32; }
    }

    public System.Drawing.Image SmallImage
    {
      get { return UiImages.new_32; }
    }

    public void ToolbarItemInitialized()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, theEvent != null && theEvent.PackagePath.Length > 0));
    }

    public IEnumerable<string> MenuItems
    {
      get { return new string[] { Layer_Line, Layer_Point }; }
    }

    public void MenuItemClicked(string menuItemName)
    {
      if (menuItemName == Layer_Point)
      {
        //TODO: point
      }
      else if (menuItemName == Layer_Line)
      {
        //TODO: layer
      }
    }
  }
}
