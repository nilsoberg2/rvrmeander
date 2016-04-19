using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IMenuToolbarItem : IToolbarItem
  {
    IEnumerable<string> MenuItems { get; }

    void MenuItemClicked(string menuItemName);
  }
}
