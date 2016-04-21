using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Result.Events
{
  public class ResultSetSelected
  {
    public IResultSet Set { get; set; }

    public ResultSetSelected(IResultSet set)
    {
      Set = set;
    }
  }
}
