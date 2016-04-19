using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui
{
  [System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItem))]
  class ViewLog : Core.Window.IToolbarItem
  {
    #region IToolbarItem Members

    public string Caption
    {
      get { return "View Log"; }
    }

    public string Group
    {
      get { return "Diagnostics"; }
    }

    public string Toolbar
    {
      get { return "Help"; }
    }

    public void Click()
    {
      System.Diagnostics.Process.Start(Path.Combine(Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory), "log/log.txt"));
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

  }
}
