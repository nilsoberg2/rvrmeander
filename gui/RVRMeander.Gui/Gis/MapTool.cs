using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  class MapTool : Core.Window.IToolbarItem
  {
    private Action<bool> clickFunction;

    public MapTool(Action<bool> clickFunction)
    {
      this.clickFunction = clickFunction;
    }

    public string Caption { get; set; }

    public string Group { get; set; }

    public string Toolbar { get; set; }

    public void Click()
    {
      this.clickFunction(IsChecked);
    }

    public bool IsChecked { get; set; }

    public bool CheckOnClick { get; set; }


    public System.Drawing.Image Image { get; set; }

    public System.Drawing.Image SmallImage { get; set; }

    //public 

    public bool IsMapInteractive { get; set; }

    public List<Core.Window.IToolbarItem> GroupedTools { get; set; }

    public void ToolbarItemInitialized()
    {
    }
  }
}
