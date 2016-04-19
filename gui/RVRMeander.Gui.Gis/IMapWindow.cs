using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis
{
  public interface IMapWindow : Core.Window.IInteractiveWindow
  {
    string Title { get; set; }
    void AddMapGroup(IRvrMapGroup group);

    bool ImportShapefile(string filePath, string name, string p);

    bool ImportSQLite(string filePath, string name);
  }
}
