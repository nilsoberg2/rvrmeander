// Portions of this come from DotSpatial.Plugins.SpatiaLite, DotSpatial release 1.7.  Licensed under the LGPL.
using DotSpatial.Data;
using DotSpatial.Projections;
using DotSpatial.Topology;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// This class contains helper methods.  Some of the methods are not static.  The only reason
  /// they are not static is so that they are able to save error messages to a shared variable
  /// for consumption by clients of the class.  No other variables are stored in the class.
  /// </summary>
  public class SpatiaLiteHelper
  {
    private static readonly Log.ILogger log = new Log.NetLogger(typeof(SpatiaLiteHelper));

    public string ErrorMessage { get; set; }

    /// <summary>
    /// Check if the process is a 64-bit or 32-bit process.  Does not check if the system OS is 64-bit or 32-bit, just the process.
    /// </summary>
    /// <returns></returns>
    public static bool CheckIs64bitProcess()
    {
      if (IntPtr.Size == 8)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Sets the environment variables so that SpatiaLite can find the dll's
    /// </summary>
    /// <returns>true if successful</returns>
    public static string GetPlatformBinaryDirectory()
    {
      string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

      if (CheckIs64bitProcess())
      {
        appPath = Path.Combine(appPath, "x64");
      }
      else
      {
        appPath = Path.Combine(appPath, "x86");
      }

      return appPath;
    }

    /// <summary>
    /// Sets the environment variables so that SpatiaLite can find the dll's
    /// </summary>
    /// <returns>true if successful</returns>
    public static bool SetEnvironmentVars()
    {
      string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
      string binFilesDir = "";

      try
      {
        binFilesDir = GetPlatformBinaryDirectory();

        //var userPath = Environment.GetEnvironmentVariable("path", EnvironmentVariableTarget.User);
        //var hasPathSet = false;
        //if (!String.IsNullOrEmpty(userPath))
        //{
        //  if (userPath.ToLower().Contains(binFilesDir.ToLower() + ";"))
        //  {
        //    hasPathSet = true;
        //  }
        //}
        //else
        //{
        //  userPath = "";
        //}
        //if (!hasPathSet)
        //{
        //  string sep = ";";
        //  if (userPath.Length == 0 || userPath.EndsWith(";"))
        //  {
        //    sep = "";
        //  }
        //  userPath = userPath + sep + binFilesDir + ";";
        //  //Environment.SetEnvironmentVariable("path", userPath, EnvironmentVariableTarget.User);
        //}

        StringBuilder sb = new StringBuilder(1024);
        GetDllDirectory(1024, sb);
        log.Debug("GetDllDirectory={0}", sb.ToString());

        if (!sb.ToString().Contains(binFilesDir))
        {
          SetDllDirectory(binFilesDir);
          log.Debug("SetDllDirectory={0}", binFilesDir);
        }
      }
      catch (Exception ex)
      {
        log.Error("Unable to set application path for unmanaged DLL's: {0}", ex.Message);
        return false;
      }

      //if (binFilesDir.Length > 0)
      //{
      //  try
      //  {
      //    Utils.IO.FileUtils.CopyAll(binFilesDir, appPath);
      //  }
      //  catch (Exception ex)
      //  {
      //    log.Error("Unable to copy platform-specific binary files to application dir: {0}", ex.Message);
      //    return false;
      //  }
      //}

      return true;
    }


    /// <summary>
    /// Save a feature set to a SQLite database. The FeatureSet is saved to a table
    /// with the same name as FeatureSet.Name.  Existing tables will get dropped.
    /// </summary>
    /// <param name="connstring">database connection string</param>
    /// <param name="fs">the feature set to save</param>
    public bool TrySaveFeatureSet(SQLiteConnection conn, DotSpatial.Data.IFeatureSet fs)
    {
      var meta = new SQLiteTableMetadata(fs.Name);

      if (meta.TableName == null || meta.TableName.Length == 0)
      {
        this.ErrorMessage = "Invalid table name provided";
        log.Error(this.ErrorMessage);
        return false;
      }

      int srid = 0;
      if (fs.Projection == null)
      {
        srid = -1; // unknown
      }
      else
      {
        srid = fs.Projection.AuthorityCode;
      }

      using (SQLiteCommand cmd = new SQLiteCommand("DROP TABLE IF EXISTS " + meta.TableName, conn))
      {
        log.Debug("TrySaveFeatureSet: dropped table {0} if it exists", meta.TableName);
        try
        {
          cmd.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
          this.ErrorMessage = string.Format("SQL execution issue 1: {0}", ex.Message);
          log.Error(this.ErrorMessage);
          return false;
        }
      }

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

      var geomMeta = new FeatureSetTableInfo(meta.TableName, fs.FeatureType, srid);

      colNames += "[" + geomMeta.GeometryColumnName + "]";
      vals += "ST_GeomFromWKB(?, " + geomMeta.SRID + ")";
      parms.Add(new SQLiteParameter(DbType.Object));

      if (!TryCreateFeatureSetTable(conn, meta, geomMeta))
      {
        return false;
      }
      log.Debug("Created feature set table");

      var trans = conn.BeginTransaction();

      try
      {
        SQLiteCommand insCmd = new SQLiteCommand(conn);
        insCmd.CommandText = string.Format("INSERT INTO [{0}] ({1}) VALUES ({2});", meta.TableName, colNames, vals);
        insCmd.Parameters.AddRange(parms.ToArray());

        for (int i = 0; i < fs.Features.Count; i++)
        {
          try
          {
            UpdateCommandParameters(fs.Features[i], parms, colNamesInFS, srid);
            //log.Debug("Starting insert of feature {0}: sql=\"{1}\" parameters=\"{2}\"", i, insCmd.CommandText, GetParametersAsString(insCmd.Parameters));
            //log.Debug(string.Join("|", Array.ConvertAll(fs.Features[i].BasicGeometry.Coordinates.ToArray(), x => x.X.ToString() + "," + x.Y.ToString())));
            insCmd.ExecuteNonQuery();
          }
          catch (Exception ex)
          {
            this.ErrorMessage = string.Format("SQL execution issue 2: {0}", ex.Message);
            log.Error(this.ErrorMessage);
            return false;
          }
        }
        log.Debug("Inserted {0} features", fs.Features.Count);
      }
      catch (KeyNotFoundException kex)
      {
        log.Error("Unable to save feature set into table \"{0}\": {1}", fs.Name, kex.Message);
        trans.Rollback();
        return false;
      }
      catch (Exception ex)
      {
        log.Error("Unable to save feature set into table \"{0}\": {1}", fs.Name, ex.Message);
        trans.Rollback();
        return false;
      }

      trans.Commit();

      log.Debug("Saved feature set into the table");

      return true;
    }

    /// <summary>
    /// Used for debugging only.
    /// </summary>
    /// <param name="parms"></param>
    /// <returns></returns>
    private string GetParametersAsString(SQLiteParameterCollection parms)
    {
      StringBuilder sb = new StringBuilder();
      for (int i = 0; i < parms.Count; i++)
      {
        if (i > 0)
        {
          sb.Append(", ");
        }
        if (parms[i].Value.GetType() == typeof(System.Byte[]))
        {
          System.Byte[] arr = parms[i].Value as System.Byte[];
          if (arr == null)
          {
            sb.Append("null-geom;;;");
          }
          else
          {
            sb.Append("geom.len=" + arr.Length.ToString() + ";;;");
          }
        }
        else
        {
          sb.Append(parms[i].Value);
        }
      }
      return sb.ToString();
    }

    /// <summary>
    /// Updates the parameters in the SQL statement with the values from the feature, including
    /// the geometry.
    /// </summary>
    /// <param name="feature">feature to save</param>
    /// <param name="parms">SQL parameters to update</param>
    /// <param name="cols">column names</param>
    /// <param name="srid">spatial reference</param>
    private void UpdateCommandParameters(DotSpatial.Data.IFeature feature, List<SQLiteParameter> parms, List<string> cols, int srid)
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
      //Helper.SpatiaLiteWkbWriter writer = new Helper.SpatiaLiteWkbWriter();
      //var g = DotSpatial.Topology.Geometry.FromBasicGeometry(feature.BasicGeometry);
      //g.Srid = srid;
      //byte[] blob = writer.Write(g);
      parms[parms.Count - 1].Value = feature.BasicGeometry.ToBinary();
    }

    private static Dictionary<DbType, Type> DbTypeMapping = new Dictionary<DbType, Type>()
      {
        { DbType.Int16, typeof(Int16) },
        { DbType.Int32, typeof(Int32) },
        { DbType.Int64, typeof(Int64) },
        { DbType.String, typeof(string) },
        { DbType.Double, typeof(double) },
        { DbType.Single, typeof(float) },
      };

    /// <summary>
    /// Creates a feature set table using the given SQLite database connection.  Catches exceptions and returns creation status.
    /// </summary>
    /// <param name="conn"></param>
    /// <param name="metadata"></param>
    /// <param name="geomMetadata"></param>
    /// <returns></returns>
    private bool TryCreateFeatureSetTable(SQLiteConnection conn, SQLiteTableMetadata metadata, FeatureSetTableInfo geomMetadata)
    {
      try
      {
        string sql = metadata.GetCreateSqlCode();
        log.Debug("TryCreateFeatureSetTable: sql=\"{0}\"", sql);

        var transaction = conn.BeginTransaction();

        var cmd = new SQLiteCommand(sql, conn);
        int nRows = cmd.ExecuteNonQuery();

        log.Debug("TryCreateFeatureSetTable: nRows={0}", nRows);

        sql = String.Format("SELECT AddGeometryColumn('{0}', '{1}', {2}, '{3}', '{4}', 1);", // last parameter means that only NOT NULL geometries will be accepted
          metadata.TableName, geomMetadata.GeometryColumnName, geomMetadata.SRID,
          geomMetadata.GetGeometryType(), geomMetadata.CoordDimension.ToString().ToUpper());

        log.Debug("TryCreateFeatureSetTable: sql=\"{0}\"", sql);

        cmd = new SQLiteCommand(sql, conn);

        nRows = cmd.ExecuteNonQuery();
        log.Debug("TryCreateFeatureSetTable: nRows={0}", nRows);

        transaction.Commit();
      }
      catch (Exception ex)
      {
        this.ErrorMessage = string.Format("Unable to create feature set table \"{0}\": {1}", metadata.TableName, ex.Message);
        log.Error(this.ErrorMessage);
        return false;
      }

      return true;
    }

    /// <summary>
    /// Returns true if the specified feature set already exists in the sqlite database.
    /// </summary>
    /// <param name="conn"></param>
    /// <param name="fs"></param>
    /// <returns></returns>
    public bool CheckIfFeatureSetExists(SQLiteConnection conn, DotSpatial.Data.IFeatureSet fs)
    {
      string sql = string.Format("SELECT name FROM sqlite_master WHERE type='table' AND name='{0}';", fs.Name);

      var cmd = new SQLiteCommand(sql, conn);
      int nRows = cmd.ExecuteNonQuery();

      if (nRows > 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }


    #region Reading from Database Functions
    
    /// <summary>
    /// Checks the database for a valid SpatiaLite version.  If it's not valid then
    /// returns empty string.
    /// </summary>
    /// <param name="conn">SQLite database connection</param>
    /// <returns>string version (e.g. "4.2.0") if it's a valid database, empty string otherwise</returns>
    /// <remarks>
    /// Checks for existence of spatialite_history table and queries table_name and ver_splite
    /// </remarks>
    public string GetSpatiaLiteDbVersion(SQLiteConnection conn)
    {
      string tableSql = "SELECT name FROM sqlite_master WHERE type='table'";

      bool validDb = false;

      SQLiteCommand cmd = null;
      try
      {
        cmd = new SQLiteCommand(tableSql, conn);

        var r = cmd.ExecuteReader();
        while (r.Read())
        {
          string name = Convert.ToString(r["name"]);
          if (name == "spatialite_history")
          {
            validDb = true;
            break;
          }
        }
      }
      catch (Exception ex)
      {
        log.Error("Failed to check database for existence of spatialite_history table: {0}", ex.Message);
        return "";
      }
      finally
      {
        if (cmd != null)
        {
          cmd.Dispose();
        }
      }

      if (!validDb)
      {
        return "";
      }

      string version = "";
      string verSql = "SELECT table_name, ver_splite FROM spatialite_history";
      try
      {
        cmd = new SQLiteCommand(verSql, conn);

        var r = cmd.ExecuteReader();
        while (r.Read())
        {
          string name = Convert.ToString(r["table_name"]);
          if (name == "geometry_columns")
          {
            version = Convert.ToString(r["ver_splite"]);
          }
        }
      }
      catch (Exception ex)
      {
        log.Error("Failed to check spatialite_history table for version: {0}", ex.Message);
        return "";
      }
      finally
      {
        if (cmd != null)
        {
          cmd.Dispose();
        }
      }

      return version;
    }

    /// <summary>
    /// Finds a list of all valid geometry columns in the database
    /// </summary>
    /// <param name="connString">connection string</param>
    /// <returns>the list of geometry columns</returns>
    public IEnumerable<FeatureSetTableInfo> ListFeatureSets(SQLiteConnection conn)
    {
      string version = GetSpatiaLiteDbVersion(conn);

      if (version.Length == 0)
      {
        this.ErrorMessage = "The database doesn't appear to be valid.";
        log.Error(this.ErrorMessage);
        return new List<FeatureSetTableInfo>();
      }

      if (!version.StartsWith("4"))
      {
        this.ErrorMessage = "The database software requires version 4 of the SpatiaLite framework.";
        log.Error(this.ErrorMessage);
        return new List<FeatureSetTableInfo>();
      }

      var sql = "SELECT f_table_name, f_geometry_column, geometry_type, coord_dimension, srid, spatial_index_enabled FROM geometry_columns";
      SQLiteCommand cmd = null;
      try
      {
        var columns = new List<FeatureSetTableInfo>();
        cmd = new SQLiteCommand(sql, conn);

        var r = cmd.ExecuteReader();
        while (r.Read())
        {
          var info = new FeatureSetTableInfo();
          info.TableName = Convert.ToString(r["f_table_name"]);
          info.GeometryColumnName = Convert.ToString(r["f_geometry_column"]);
          info.GeometryType = GetGeometryType(Convert.ToString(r["geometry_type"]));
          CoordinateDimension coordDim;
          if (!Enum.TryParse(Convert.ToString(r["coord_dimension"]), out coordDim))
          {
            info.CoordDimension = CoordinateDimension.XY;
          }
          info.SRID = Convert.ToInt32(r["srid"]);
          info.SpatialIndexEnabled = false;
          columns.Add(info);
        }

        return columns;
      }
      catch (Exception ex)
      {
        log.Error("Failed to retrieve feature set listing from database: {0}", ex.Message);
        return new List<FeatureSetTableInfo>();
      }
      finally
      {
        if (cmd != null)
        {
          cmd.Dispose();
        }
      }
    }

    /// <summary>
    /// Returns the feature type given a specific type of geometry string.
    /// </summary>
    /// <param name="geometryTypeStr"></param>
    /// <returns></returns>
    private DotSpatial.Topology.FeatureType GetGeometryType(string geometryTypeStr)
    {
      switch (geometryTypeStr.ToLower())
      {
        case "point":
        case "multipoint":
        case "1":
        case "4":
          return FeatureType.Point;
        case "linestring":
        case "multilinestring":
        case "2":
        case "5":
          return FeatureType.Line;
        case "polygon":
        case "multipolygon":
        case "3":
        case "6":
          return FeatureType.Polygon;
        default:
          return FeatureType.Unspecified;
      }
    }

    /// <summary>
    /// Reads the complete feature set from the database
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="featureSetInfo">information about the table</param>
    /// <returns>the resulting feature set</returns>
    public bool TryReadFeatureSet(SQLiteConnection conn, FeatureSetTableInfo featureSetInfo, out IFeatureSet featureSet)
    {
      featureSet = null;
      if (featureSetInfo == null)
      {
        return false;
      }
      var sql = string.Format("SELECT * FROM {0}", featureSetInfo.TableName);
      return TryReadFeatureSet(conn, featureSetInfo, sql, out featureSet);
    }

    /// <summary>
    /// Reads the feature set from the database, satisfying the given SQL query
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="featureSetInfo">information about the table</param>
    /// <param name="sqlFilter">the sql query; to select all: "SELECT * FROM TableName"</param>
    /// <returns>the resulting feature set</returns>
    public bool TryReadFeatureSet(SQLiteConnection conn, FeatureSetTableInfo featureSetInfo, string sqlFilter, out IFeatureSet featureSet)
    {
      featureSet = null;
      if (featureSetInfo == null)
      {
        return false;
      }

      var fs = new SpatiaLiteFeatureSet(featureSetInfo.GeometryType);
      fs.IndexMode = false; //setting the initial index mode..

      SQLiteCommand cmd = null;
      try
      {
        cmd = new SQLiteCommand(sqlFilter, conn);

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

        fs.Name = featureSetInfo.TableName;

        //TODO: look into this...
        //HACK required for selection to work properly
        fs.IndexMode = true;

        // assign projection
        ProjectionInfo proj;
        if (featureSetInfo.SRID >= 0)
        {
          try
          {
            proj = ProjectionInfo.FromEpsgCode(featureSetInfo.SRID);
          }
          catch (Exception ex)
          {
            log.Warning("Unable to find projection info from EPSG code {0}, using default: {1}", featureSetInfo.SRID, ex.Message);
            proj = new ProjectionInfo();
          }
        }
        else
        {
          proj = new ProjectionInfo();
        }
        fs.Projection = proj;

        featureSet = fs;
        return true;
      }
      catch (Exception ex)
      {
        log.Error("Unable to read feature set {0} using SQL \"{1}\": {2}", featureSetInfo.TableName, sqlFilter, ex.Message);
        return false;
      }
      finally
      {
        if (cmd != null)
        {
          cmd.Dispose();
        }
      }
    }

    /// <summary>
    /// Reads the feature set from the database, satisfying the given SQL query (filter)
    /// </summary>
    /// <param name="connString">sqlite db connection string</param>
    /// <param name="sqlFilter">the SQL Query string; to select all: "SELECT * FROM TableName"</param>
    /// <returns>the resulting feature set</returns>
    public bool TryReadFeatureSet(SQLiteConnection conn, string sqlFilter, out IFeatureSet featureSet)
    {
      //Find the geometry type and geometry column
      var geomInfo = FindGeometryColumnInfo(conn, sqlFilter);
      if (geomInfo == null)
      {
        featureSet = null;
        return false;
      }
      else
      {
        return TryReadFeatureSet(conn, geomInfo, sqlFilter, out featureSet);
      }
    }

    /// <summary>
    /// Finds out information about the geometry column 
    /// </summary>
    /// <param name="conn"></param>
    /// <param name="sqlQuery"></param>
    /// <returns></returns>
    private FeatureSetTableInfo FindGeometryColumnInfo(SQLiteConnection conn, string sqlQuery)
    {
      FeatureSetTableInfo result = null;

      using (var cmd = new SQLiteCommand(sqlQuery, conn))
      {
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
            result = new FeatureSetTableInfo();
            result.GeometryColumnName = colName;
            break;
          }
        }

        if (result != null && rdr.HasRows)
        {
          rdr.Read();
          var blob = rdr[result.GeometryColumnName] as byte[];
          var geom = wkbr.Read(blob);
          result.GeometryType = geom.FeatureType;
        }

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

    #endregion


    #region P/Invoke

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static extern bool SetDllDirectory(string lpPathName);

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern int GetDllDirectory(int nBufferLength, StringBuilder lpPathName);

    #endregion
  }
}
