using DotSpatial.Topology;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Gis.WebMap.Tiling
{
  internal class Tiles
  {
    public Tiles(Bitmap[,] bitmaps, Envelope topLeftTile, Envelope bottomRightTile)
    {
      BottomRightTile = bottomRightTile;
      TopLeftTile = topLeftTile;
      Bitmaps = bitmaps;
    }

    public Bitmap[,] Bitmaps { get; private set; }
    public Envelope TopLeftTile { get; private set; }
    public Envelope BottomRightTile { get; private set; }
  }
}
