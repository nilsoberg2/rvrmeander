// Originally part of DotSpatial.Plugins.SpatiaLite, DotSpatial release 1.7.  Licensed under the LGPL.
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using DotSpatial.Data;
using DotSpatial.Projections;
using DotSpatial.Topology;
using DotSpatial.Topology.Utilities;
using ByteOrder = DotSpatial.Topology.Utilities.ByteOrder;
using System.Diagnostics.Contracts;

namespace MetroFlow.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// Helper class with SpatiaLite specific operations
  /// </summary>
  public class SpatiaLiteHelper
  {
    protected string dbConnString;


    /// <summary>
    /// Sets the environment variables so that SpatiaLite can find the dll's
    /// </summary>
    /// <returns>true if successful</returns>
    public static bool SetEnvironmentVars()
    {
      try
      {
        var pathVar = Environment.GetEnvironmentVariable("path", EnvironmentVariableTarget.User);
        var sqLitePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        var pathVariableExists = false;
        if (!String.IsNullOrEmpty(pathVar))
        {
          if (pathVar.ToLower().Contains(sqLitePath.ToLower() + ";"))
            pathVariableExists = true;
        }
        if (!pathVariableExists)
        {
          pathVar = pathVar + ";" + sqLitePath;
          Environment.SetEnvironmentVariable("path", pathVar, EnvironmentVariableTarget.User);
          return true;
        }
      }
      catch (Exception ex)
      {
        Trace.WriteLine(ex.Message);
        return false;
      }
      return false;
    }

    /// <summary>
    /// Returns true if the schema is a valid schema (has a geometry_columns table)
    /// </summary>
    public static bool CheckSpatiaLiteSchema(string connString)
    {
      var qry = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'geometry_columns'";

      using (var cmd = CreateCommand(connString, qry))
      {
        var result = false;
        cmd.Connection.Open();
        var obj = cmd.ExecuteScalar();
        if (obj != null)
          result = true;
        cmd.Connection.Close();

        return result;
      }
    }

    /// <summary>
    /// Finds all table names in the SpatiaLite database
    /// </summary>
    /// <param name="connString">connection string</param>
    /// <returns>a list of all table names</returns>
    public List<string> GetTableNames(string connString)
    {
      var tableNameList = new List<string>();

      using (var cnn = new SQLiteConnection(connString))
      {
        var qry = "SELECT name FROM sqlite_master WHERE type = 'table'";
        using (var cmd = new SQLiteCommand(qry, cnn))
        {
          cmd.Connection.Open();
          var r = cmd.ExecuteReader();
          while (r.Read())
          {
            tableNameList.Add(r[0].ToString());
          }
          cmd.Connection.Close();
        }
      }
      return tableNameList;
    }

    /// <summary>
    /// Finds all column names in the database table
    /// </summary>
    /// <param name="connString">connection string</param>
    /// <param name="tableName">table name</param>
    /// <returns>list of all column names</returns>
    public List<string> GetColumnNames(string connString, string tableName)
    {
      var columnNameList = new List<string>();

      var qry = String.Format("PRAGMA table_info({0})", tableName);
      using (var cmd = CreateCommand(connString, qry))
      {
        cmd.Connection.Open();
        var r = cmd.ExecuteReader();
        while (r.Read())
        {
          columnNameList.Add(r["name"].ToString());
        }
        cmd.Connection.Close();
      }
      return columnNameList;
    }

    /// <summary>
    /// Finds a list of all valid geometry columns in the database
    /// </summary>
    /// <param name="connString">connection string</param>
    /// <returns>the list of geometry columns</returns>
    public List<GeometryColumnInfo> GetGeometryColumns(string connString)
    {
      var lst = new List<GeometryColumnInfo>();
      var sql =
      "SELECT f_table_name, f_geometry_column, type, coord_dimension, srid, spatial_index_enabled FROM geometry_columns";
      using (var cmd = CreateCommand(connString, sql))
      {
        cmd.Connection.Open();

        var r = cmd.ExecuteReader();
        while (r.Read())
        {
          var gci = new GeometryColumnInfo();
          gci.TableName = Convert.ToString(r["f_table_name"]);
          gci.GeometryColumnName = Convert.ToString(r["f_geometry_column"]);
          gci.GeometryType = GetGeometryType(Convert.ToString(r["type"]));
          gci.CoordDimension = ConvertToCoordDimension(Convert.ToString(r["coord_dimension"]));
          gci.SRID = Convert.ToInt32(r["srid"]);
          gci.SpatialIndexEnabled = false;
          lst.Add(gci);
        }

        cmd.Connection.Close();
      }
      return lst;
    }

    private static CoordinateDimension ConvertToCoordDimension(string coord)
    {
      return (CoordinateDimension)Enum.Parse(typeof(CoordinateDimension), coord);
    }

    /// <summary>
    /// Reads the complete feature set from the database
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="featureSetInfo">information about the table</param>
    /// <returns>the resulting feature set</returns>
    public IFeatureSet ReadFeatureSet(string connString, GeometryColumnInfo featureSetInfo)
    {
      var sql = String.Format("SELECT * FROM {0}", featureSetInfo.TableName);
      return ReadFeatureSet(connString, featureSetInfo, sql);
    }

    /// <summary>
    /// Reads the complete feature set from the database
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="featureSetInfo">information about the table</param>
    /// <param name="sql">the sql query</param>
    /// <returns>the resulting feature set</returns>
    public IFeatureSet ReadFeatureSet(string connString, GeometryColumnInfo featureSetInfo, string sql)
    {
      FeatureSet fs = new SpatiaLiteFeatureSet(featureSetInfo.GeometryType);
      fs.IndexMode = false; //setting the initial index mode..

      using (var cmd = CreateCommand(connString, sql))
      {
        cmd.Connection.Open();

        RunInitialCommands(cmd.Connection);

        //DotSpatial.Topology.Utilities.WkbReader wkbr = new DotSpatial.Topology.Utilities.WkbReader();
        var wkbr = new SpatiaLiteWkbReader();

        var rdr = cmd.ExecuteReader();

        var columnNames = PopulateTableSchema(fs, featureSetInfo.GeometryColumnName, rdr);
        while (rdr.Read())
        {
          var wkb = rdr[featureSetInfo.GeometryColumnName] as byte[];
          var geom = wkbr.Read(wkb);

          var newFeature = fs.AddFeature(geom);

          //populate the attributes
          foreach (var colName in columnNames)
          {
            newFeature.DataRow[colName] = rdr[colName];
          }
        }
        cmd.Connection.Close();
        fs.Name = featureSetInfo.TableName;

        //TODO: look into this more
        //HACK required for selection to work properly
        fs.IndexMode = true;

        //assign projection
        var proj = ProjectionInfo.FromEpsgCode(featureSetInfo.SRID);
        fs.Projection = proj;

        return fs;
      }
    }

    /// <summary>
    /// Reads the complete feature set from the database
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="sqlQuery">the SQL Query string</param>
    /// <returns>the resulting feature set</returns>
    public IFeatureSet ReadFeatureSet(string connString, string sqlQuery)
    {
      //RunInitialCommands(connString);

      //Find the geometry type and geometry column
      var geomInfo = FindGeometryColumnInfo(connString, sqlQuery);
      return ReadFeatureSet(connString, geomInfo, sqlQuery);
    }

    private void RunInitialCommands(SQLiteConnection connection)
    {
      //(new SQLiteConnection("vole"))

      //string cmdText = "SELECT load_extension('E:/dev/DotSpatial/Debug/x86/Plugins/SpatiaLite/libspatialite-2.dll');";

      //TODO: set path
      var cmdText = "SELECT load_extension('mod_spatialite.dll')";

      using (var cmd = new SQLiteCommand(cmdText, connection))
      {
        try
        {
          cmd.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
          Trace.WriteLine(ex.Message);
          Debug.WriteLine("Warning: error loading spatiaLite extensions. Spatial queries won't be enabled.");
        }
      }
    }

    //finds out the geometry column information..
    private GeometryColumnInfo FindGeometryColumnInfo(string connString, string sqlQuery)
    {
      GeometryColumnInfo result = null;

      using (var cmd = CreateCommand(connString, sqlQuery))
      {
        cmd.Connection.Open();

        RunInitialCommands(cmd.Connection);

        var wkbr = new SpatiaLiteWkbReader();

        var rdr = cmd.ExecuteReader(CommandBehavior.SingleRow);

        var schemaTable = rdr.GetSchemaTable();
        foreach (DataRow r in schemaTable.Rows)
        {
          var colName = Convert.ToString(r["ColumnName"]);
          var colDataType = Convert.ToString(r["DataType"]);
          //if BLOB, then assume geometry column
          if (Type.GetType(colDataType) == typeof(byte[]))
          {
            result = new GeometryColumnInfo();
            result.GeometryColumnName = colName;
            break;
          }
        }

        if (result != null && rdr.HasRows)
        {
          rdr.Read();
          var blob = rdr[result.GeometryColumnName] as byte[];
          var geom = wkbr.Read(blob);
          result.GeometryType = GetGeometryType(Convert.ToString(rdr["type"]));
        }

        cmd.Connection.Close();
        return result;
      }
    }

    private string[] PopulateTableSchema(IFeatureSet fs, string geomColumnName, SQLiteDataReader rdr)
    {
      var schemaTable = rdr.GetSchemaTable();
      var columnNameList = new List<string>();
      foreach (DataRow r in schemaTable.Rows)
      {
        if (r["ColumnName"].ToString() != geomColumnName)
        {
          var colName = Convert.ToString(r["ColumnName"]);
          var colDataType = Convert.ToString(r["DataType"]);
          fs.DataTable.Columns.Add(colName, Type.GetType(colDataType));
          columnNameList.Add(colName);
        }
      }
      return columnNameList.ToArray();
    }


    private Point ReadPoint(byte[] blob)
    {
      using (Stream stream = new MemoryStream(blob))
      {
        //read first byte
        BinaryReader reader = null;
        var startByte = stream.ReadByte(); //must be "0"
        var byteOrder = (ByteOrder)stream.ReadByte();
        try
        {
          reader = (byteOrder == ByteOrder.BigEndian) ? new BeBinaryReader(stream) : new BinaryReader(stream);
          var srid = reader.ReadInt32();
          var mbrMinX = reader.ReadDouble();
          var mbrMinY = reader.ReadDouble();
          var mbrMaxX = reader.ReadDouble();
          var mbrMaxY = reader.ReadDouble();
          var mbrEnd = reader.ReadByte();
          var geomType = reader.ReadInt32();
          var ptX = reader.ReadDouble();
          var ptY = reader.ReadDouble();
          var geomEnd = reader.ReadByte();
          return new Point(ptX, ptY);
        }
        finally
        {
          if (reader != null)
            reader.Close();
        }
      }
    }

    private FeatureType GetGeometryType(string geometryTypeStr)
    {
      switch (geometryTypeStr.ToLower())
      {
        case "point":
        case "multipoint":
          return FeatureType.Point;
        case "linestring":
        case "multilinestring":
          return FeatureType.Line;
        case "polygon":
        case "multipolygon":
          return FeatureType.Polygon;
        default:
          return FeatureType.Unspecified;
      }
    }

    /// <summary>
    /// Creates a SQLite command
    /// </summary>
    /// <param name="conString">connection string</param>
    /// <param name="cmdText">command text</param>
    /// <returns>the SpatiaLite command object</returns>
    private static SQLiteCommand CreateCommand(string conString, string cmdText)
    {
      var con = new SQLiteConnection(conString);
      return new SQLiteCommand(cmdText, con);
    }

    /// <summary>
    /// Creates a SQLite connection and opens the database.
    /// </summary>
    /// <returns></returns>
    protected SQLiteConnection OpenDatabase()
    {
      var conn = new SQLiteConnection(this.dbConnString).OpenAndReturn();
      RunInitialCommands(conn);
      return conn;
    }

    /// <summary>
    /// Save a feature set to a SQLite database. The FeatureSet is saved to a table
    /// with the same name as FeatureSet.Name.  Existing tables will get dropped.
    /// </summary>
    /// <param name="connstring">database connection string</param>
    /// <param name="fs">the feature set to save</param>
    public bool SaveFeatureSet(IFeatureSet fs)
    {
      using (SQLiteConnection conn = OpenDatabase())
      {
        var meta = new SQLiteTableMetadata(fs.Name);

        SQLiteCommand cmd = new SQLiteCommand("DROP TABLE IF EXISTS " + meta.TableName, conn);
        cmd.ExecuteNonQuery();

        string colNames = "";
        string vals = "";

        List<SQLiteParameter> parms = new List<SQLiteParameter>();
        List<string> colNamesInFS = new List<string>();
        foreach (DataColumn c in fs.DataTable.Columns)
        {
          meta.Columns.Add(new Column(c.ColumnName, c.DataType));
          colNames += "[" + c.ColumnName + "], ";
          vals += "?, ";
          parms.Add(new SQLiteParameter());
          colNamesInFS.Add(c.ColumnName);
        }

        //TODO: check if EpsgCode is correct == SRID
        var geomMeta = new GeometryColumnInfo(meta.TableName, fs.FeatureType, fs.Projection.EpsgCode);

        colNames += "[" + geomMeta.GeometryColumnName + "]";
        vals += "ST_GeomFromWKB(?, " + geomMeta.SRID + ")";
        parms.Add(new SQLiteParameter(DbType.Object));

        try
        {
          CreateFeatureSetTable(conn, meta, geomMeta);
        }
        catch (Exception ex)
        {
          conn.Close();
          return false;
        }

        var trans = conn.BeginTransaction();

        try
        {
          SQLiteCommand insCmd = new SQLiteCommand(conn);
          insCmd.CommandText = string.Format("INSERT INTO [{0}] ({1}) VALUES ({2});", meta.TableName, colNames, vals);
          insCmd.Parameters.AddRange(parms.ToArray());

          for (int i = 0; i < fs.Features.Count; i++)
          {
            UpdateCommandParameters(fs.Features[i], parms, colNamesInFS, fs.Projection.EpsgCode);
            insCmd.ExecuteNonQuery();
          }
        }
        catch (KeyNotFoundException kex)
        {
          //TODO: log this
          trans.Rollback();
          conn.Close();
          return false;
        }
        catch (Exception ex)
        {
          //TODO: log this
          trans.Rollback();
          conn.Close();
          return false;
        }

        trans.Commit();
        conn.Close();
      }
      return true;
    }

    protected void UpdateCommandParameters(IFeature feature, List<SQLiteParameter> parms, List<string> cols, int srid)
    {
      for (int i = 0; i < cols.Count; i++)
      {
        if (System.DBNull.Value.Equals(feature.DataRow[cols[i]]))
        {
          if (parms[i].DbType == DbType.String)
          {
            parms[i].Value = "";
          }
          else
          {
            parms[i].Value = Activator.CreateInstance(DbTypeMapping[parms[i].DbType]);
          }
        }
        else
        {
          parms[i].Value = feature.DataRow[cols[i]];
        }
      }

      // Save geometry
      //SpatiaLiteWkbWriter writer = new SpatiaLiteWkbWriter();
      //var g = Geometry.FromBasicGeometry(feature.BasicGeometry);
      //g.Srid = srid;
      //byte[] blob = writer.Write(g);
      parms[parms.Count - 1].Value = feature.BasicGeometry.ToBinary();
    }


    protected static Dictionary<DbType, Type> DbTypeMapping = new Dictionary<DbType, Type>()
      {
        { DbType.Int16, typeof(Int16) },
        { DbType.Int32, typeof(Int32) },
        { DbType.Int64, typeof(Int64) },
        { DbType.String, typeof(string) },
        { DbType.Double, typeof(double) },
        { DbType.Single, typeof(float) },
      };



    /// <summary>
    /// Creates a geometry-enabled (feature set) table.
    /// </summary>
    /// <param name="metadata">metadata describing the standard attributes</param>
    /// <param name="geomMetadata">metadata describing the geometry column</param>
    /// <returns></returns>
    public bool CreateFeatureSetTable(SQLiteTableMetadata metadata, GeometryColumnInfo geomMetadata)
    {
      Contract.Requires(metadata != null && geomMetadata != null);

      using (var conn = OpenDatabase())
      {
        CreateFeatureSetTable(conn, metadata, geomMetadata);

        conn.Close();
      }

      return true;
    }

    protected void CreateFeatureSetTable(SQLiteConnection conn, SQLiteTableMetadata metadata, GeometryColumnInfo geomMetadata)
    {
      string sql = metadata.GetCreateSqlCode();

      var transaction = conn.BeginTransaction();

      var cmd = new SQLiteCommand(sql, conn);
      int nRows = cmd.ExecuteNonQuery();

      sql = String.Format("SELECT AddGeometryColumn('{0}', '{1}', {2}, '{3}', '{4}');",
        metadata.TableName, geomMetadata.GeometryColumnName, geomMetadata.SRID,
        geomMetadata.GetGeometryType(), geomMetadata.CoordDimension.ToString().ToUpper());

      cmd = new SQLiteCommand(sql, conn);
      nRows = cmd.ExecuteNonQuery();

      transaction.Commit();
    }

    /// <summary>
    /// Creates a new SpatiaLite database
    /// </summary>
    public bool CreateNewDatabase()
    {
      var dbConn = new SQLiteConnection(this.dbConnString);
      dbConn.Open();
      dbConn.Close();

      return true;
    }

  }
}