using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Result
{
  public interface IResultManager
  {
    /// <summary>
    /// 
    /// </summary>
    /// <param name="simDirName">directory name only (local to the project directory)</param>
    /// <param name="setName"></param>
    /// <param name="set"></param>
    void AddResultSet(string simDirName, string setName, IResultSetLoader set);

    IEnumerable<string> ResultSetIds { get; }

    string GetResultSetName(string id);

    bool LoadResultSet(string id, out IResultSet set);
  }
}
