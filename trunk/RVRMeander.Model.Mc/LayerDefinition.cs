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

namespace RVRMeander.Gui
{

  [System.ComponentModel.Composition.Export(typeof(Core.Window.IInteractiveWindow))]
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItem))]

  public partial class LayerDefinition : Core.Window.DockableWindow, Core.Window.IInteractiveWindow, Core.Window.IToolbarItem
  {
    public LayerDefinition()
    {
      InitializeComponent();
    }


    #region IToolbarItem Members

    public string Caption
    {
      get { return "Layer definition"; }
    }

    public string Group
    {
      get { return "RVR"; }
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
    }

    #endregion

  }
}
