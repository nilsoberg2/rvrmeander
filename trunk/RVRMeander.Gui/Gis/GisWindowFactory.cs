using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  [System.ComponentModel.Composition.Export(typeof(IGisWindowFactory))]
  public class GisWindowFactory : IGisWindowFactory
  {
    public IMapWindow CreateMapWindow()
    {
      var win = new MapWindow();
      return win;
    }
  }
}
