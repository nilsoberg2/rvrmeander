using System.IO;
using DotSpatial.Topology;
using DotSpatial.Topology.Utilities;

namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// Helper class for reading binary data from the SpatiaLite database
  /// </summary>
  public class SpatiaLiteWkbWriter : WkbWriter
  {
    public override byte[] Write(IGeometry geometry)
    {
      byte[] bytes = GetBytes(geometry);
      using (BinaryWriter writer = new BinaryWriter(new MemoryStream(bytes)))
      {
        writer.Write((byte)0); // START
        writer.Write((byte)ByteOrder.LittleEndian); // ENDIAN
        writer.Write(geometry.Srid); // SRID
        writer.Write(geometry.Envelope.Minimum.X); // MBR_MIN_X
        writer.Write(geometry.Envelope.Minimum.Y); // MBR_MIN_Y
        writer.Write(geometry.Envelope.Maximum.X); // MBR_MAX_X
        writer.Write(geometry.Envelope.Maximum.Y); // MBR_MAX_Y
        writer.Write((byte)0x7C); // MBR_END
        Write(geometry, writer);
        writer.Write((byte)0xFE); // END
        return bytes;
      }
    }

    /// <summary>
    /// Get the allocated array of bytes corresponding to the given geometry.
    /// </summary>
    /// <param name="geometry"></param>
    /// <returns></returns>
    protected override byte[] GetBytes(IGeometry geometry)
    {
      return new byte[SetByteStream(geometry)];
    }

    /// <summary>
    /// Override the WriteByteOrder function to do nothing, since SpatialLite stores
    /// its byte order in the header.
    /// </summary>
    /// <param name="writer"></param>
    protected override void WriteByteOrder(BinaryWriter writer)
    {
      // Do nothing, since SpatialLite stores byte order in its header.
    }

    /// <summary>
    /// Gets the number of bytes necessary for writing the geometry.  Note that this is
    /// overridden in order to add the bytes that SpatiaLite needs for its header info.
    /// </summary>
    /// <param name="geometry"></param>
    /// <returns></returns>
    protected override int SetByteStream(IGeometry geometry)
    {
      // Compute the difference between the INIT_COUNT const in the parent class and
      // 4.  Since we can't override INIT_COUNT, we need to subtract the calculated
      // byte count in base.SetByteStream by this difference since we are not writing
      // the byte order.
      int initOffset = 4 - INIT_COUNT;
      // 39 is for the SpatiaLite header (e.g. MBR), (40 - 39) is for the END byte
      return 40 + base.SetByteStream(geometry) + initOffset;
    }
  }
}
