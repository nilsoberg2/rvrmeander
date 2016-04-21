using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Result.Events
{
  public class ResultLayerSelected
  {
    public IResultSet ResultSet { get; set; }

    public List<string> SelectedLayers { get; set; }

    public ResultLayerSelected(IResultSet resultSet, List<string> selectedLayers)
    {
      this.ResultSet = resultSet;
      this.SelectedLayers = selectedLayers;
    }
  }
}
