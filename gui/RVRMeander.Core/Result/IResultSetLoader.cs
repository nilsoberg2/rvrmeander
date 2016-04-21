using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Result
{
  public interface IResultSetLoader
  {
    bool CanLoad(string dirPath);

    bool LoadResultSet(string dirPath, out IResultSet set);
  }
}
