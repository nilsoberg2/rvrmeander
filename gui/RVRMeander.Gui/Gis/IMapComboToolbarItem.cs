using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  interface IMapComboToolbarItem : Core.Window.IComboToolbarItem
  {
    void AddComboItem(string serviceProvider);
  }
}
