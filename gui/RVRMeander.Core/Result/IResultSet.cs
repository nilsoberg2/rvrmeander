using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Result
{
  public interface IResultSet
  {
    string Name { get; }
    
    IEnumerable<string> LayerNames { get; }

    double[,] GetDataset(string layerName);
  }
}
