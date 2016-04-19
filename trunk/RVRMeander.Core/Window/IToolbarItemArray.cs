using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IToolbarItemArray : IToolbarItem
  {
    IEnumerable<IToolbarItem> ToolbarItems { get; }

    bool IsButtonList { get; }
  }
}
