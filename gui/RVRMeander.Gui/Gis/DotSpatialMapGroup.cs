using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  public class DotSpatialMapGroup : IRvrMapGroup
  {
    public DotSpatial.Controls.IMapGroup MapGroup { get; set; }
  }
}
