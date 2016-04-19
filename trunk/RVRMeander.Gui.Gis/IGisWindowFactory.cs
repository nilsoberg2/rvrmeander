using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  /// <summary>
  /// This interface provides a way for clients to create a GIS window.
  /// </summary>
  public interface IGisWindowFactory
  {
    IMapWindow CreateMapWindow();
  }
}
