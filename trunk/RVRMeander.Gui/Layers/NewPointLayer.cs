using DotSpatial.Plugins.ShapeEditor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Layers
{
  class NewPointLayer : AddShapeFunction
  {
    private DotSpatial.Controls.IMap map;
    private DotSpatial.Projections.ProjectionInfo projection;
    private string filePath;
    private ISnapSettings snapSettings;

    public NewPointLayer(DotSpatial.Controls.IMap map, DotSpatial.Projections.ProjectionInfo projection, string filePath, ISnapSettings snapSettings)
      : base(map)
    {
      this.map = map;
      this.projection = projection;
      this.filePath = filePath;
      this.snapSettings = snapSettings;
      DoSnapping = this.snapSettings.Snap;
    }

    //public bool Save()
    //{

    //}
  }
}
