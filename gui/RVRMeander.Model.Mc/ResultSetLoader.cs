using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  [Export(typeof(Core.Result.IResultSetLoader))]
  class ResultSetLoader : Core.Result.IResultSetLoader
  {
    public bool CanLoad(string dirPath)
    {
      return File.Exists(Path.Combine(dirPath, "sim.ini")) && File.Exists(Path.Combine(dirPath, "TecPlot1D.DAT"));
    }

    public bool LoadResultSet(string dirPath, out Core.Result.IResultSet set)
    {
      TecplotReader reader = new TecplotReader(Path.GetFileNameWithoutExtension(dirPath), Path.Combine(dirPath, "TecPlot1D.DAT"));
      set = reader;
      return reader.LoadFile();
    }
  }
}
