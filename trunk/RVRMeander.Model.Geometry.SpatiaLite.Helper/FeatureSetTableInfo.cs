// Originally part of DotSpatial.Plugins.SpatiaLite, DotSpatial release 1.7.  Licensed under the LGPL.
using DotSpatial.Topology;
namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// Contains metadata about the geometry column in a SpatiaLite database.
  /// </summary>
  public class FeatureSetTableInfo
  {
    /// <summary>
    /// Name of the table.
    /// </summary>
    public string TableName { get; set; }

    /// <summary>
    /// Name of the geometry column.  Defaults to DefaultGeometryColumnName.
    /// </summary>
    public string GeometryColumnName { get; set; }

    /// <summary>
    /// Default geometry column name.
    /// </summary>
    public const string DefaultGeometryColumnName = "feature_geometry";

    /// <summary>
    /// Type of the geometry (e.g. Point, LineString).
    /// </summary>
    public DotSpatial.Topology.FeatureType GeometryType { get; set; }

    /// <summary>
    /// Dimension of the geometry (e.g. XY, XYZ).
    /// </summary>
    public CoordinateDimension CoordDimension { get; set; }

    /// <summary>
    /// Spatial reference ID
    /// </summary>
    public int SRID { get; set; }

    /// <summary>
    /// ?
    /// </summary>
    public bool SpatialIndexEnabled { get; set; }

    /// <summary>
    /// Create a GeometryColumnInfo object with the specified parameters.
    /// </summary>
    public FeatureSetTableInfo(string tableName, string geomColName, FeatureType geomType, CoordinateDimension coordDim, int srid, bool spatialIndexEnabled)
    {
      this.TableName = tableName;
      this.GeometryColumnName = geomColName;
      this.GeometryType = geomType;
      this.CoordDimension = coordDim;
      this.SRID = srid;
      this.SpatialIndexEnabled = spatialIndexEnabled;
    }

    /// <summary>
    /// Create a GeometryColumnInfo object with the specified parameters.
    /// </summary>
    public FeatureSetTableInfo(string tableName, FeatureType geomType, int srid)
      : this(tableName, DefaultGeometryColumnName, geomType, CoordinateDimension.XY, srid, false)
    {
    }

    /// <summary>
    /// Create a GeometryColumnInfo object with the specified parameters.
    /// </summary>
    public FeatureSetTableInfo(string tableName, FeatureType geomType)
      : this(tableName, DefaultGeometryColumnName, geomType, CoordinateDimension.XY, 0, false)
    {
    }

    /// <summary>
    /// Create an empty GeometryColumnInfo object.
    /// </summary>
    public FeatureSetTableInfo()
    {
    }

    /// <summary>
    /// Returns the SpatiaLite geometry type corresponding to the FeatureType in this.GeometryType.
    /// </summary>
    /// <returns></returns>
    public string GetGeometryType()
    {
      if (this.GeometryType == FeatureType.Line)
      {
        return "LINESTRING";
      }
      else
      {
        return this.GeometryType.ToString().ToUpper();
      }
    }
  }
}