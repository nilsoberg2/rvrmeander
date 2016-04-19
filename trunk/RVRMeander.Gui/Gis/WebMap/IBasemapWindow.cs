using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis.WebMap
{
  interface IBasemapWindow : Core.Window.IInteractiveWindow
  {
    void HideError();

    void ShowError();

    DotSpatial.Controls.IMap GetMap();

    void SetMousePosition(double p1, double p2);

    void SetExtents(DotSpatial.Data.Extent extent);

    void Hide();
  }
}
