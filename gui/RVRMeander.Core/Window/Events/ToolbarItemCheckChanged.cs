using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window.Events
{
  public class ToolbarItemCheckChanged
  {
    public bool Checked { get; set; }

    public IToolbarItem Item { get; set; }

    public ToolbarItemCheckChanged(IToolbarItem item, bool isChecked)
    {
      Item = item;
      Checked = isChecked;
    }
  }
}
