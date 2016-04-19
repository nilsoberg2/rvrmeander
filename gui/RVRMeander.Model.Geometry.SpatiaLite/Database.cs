using DotSpatial.Data;
using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Geometry.SpatiaLite
{
  public class Database
  {
    private static readonly Log.ILogger log = new Log.NetLogger(typeof(Database));

    public const string Database_File_Name = "geofabric.sqlite";

    private string connString;
    private string filePath;

    public string ErrorMessage { get; private set; }

    public Database(string filePath)
    {
      this.filePath = filePath;
      this.connString = Model.Geometry.SpatiaLite.Helper.SQLiteHelper.GetSQLiteConnectionString(this.filePath);

      Helper.SpatiaLiteHelper.SetEnvironmentVars();
      this.ErrorMessage = "";
    }

    private bool TryOpen(out SQLiteConnection conn)
    {
      bool initMetadata = !Helper.SQLiteHelper.DatabaseExists(this.filePath);
      conn = new SQLiteConnection(this.connString);

      try
      {
        conn.Open();
      }
      catch (Exception ex)
      {
        this.ErrorMessage = string.Format("Unable to open connection to database {0}: {1}", this.connString, ex.Message);
        log.Error(this.ErrorMessage);
        return false;
      }

      if (!LoadSpatiaLiteExtension(conn, initMetadata))
      {
        return false;
      }

      return true;
    }

    private bool LoadSpatiaLiteExtension(SQLiteConnection connection, bool initMetadata)
    {
      var cmdText = "SELECT load_extension('" + Path.Combine(Helper.SpatiaLiteHelper.GetPlatformBinaryDirectory(), "mod_spatialite.dll").Replace('\\', '/') + "')";
      //var cmdText = "SELECT load_extension('mod_spatialite.dll')";
      log.Debug("Loading extension \"{0}\"", cmdText);

      using (var cmd = new SQLiteCommand(cmdText, connection))
      {
        try
        {
          cmd.ExecuteNonQuery();
          log.Debug("OK");
        }
        catch (Exception ex)
        {
          this.ErrorMessage = string.Format("Unable to load SpatiaLite extension: {0}", ex.Message);
          log.Error(this.ErrorMessage);
          return false;
        }
      }

      if (initMetadata)
      {
        cmdText = "SELECT InitSpatialMetaData(1);";
        log.Debug("Initializing metadata \"{0}\"", cmdText);

        using (var cmd = new SQLiteCommand(cmdText, connection))
        {
          try
          {
            cmd.ExecuteNonQuery();
            log.Debug("OK");
          }
          catch (Exception ex)
          {
            this.ErrorMessage = string.Format("Unable to load SpatiaLite extension: {0}", ex.Message);
            log.Error(this.ErrorMessage);
            return false;
          }
        }
      }

      return true;
    }

    /// <summary>
    /// Saves the given feature class to a SpatiaLite database
    /// </summary>
    /// <param name="fs"></param>
    /// <returns></returns>
    public async Task<bool> TrySaveSpatialClass(DotSpatial.Data.IFeatureSet fs)
    {
      SQLiteConnection conn;
      if (!TryOpen(out conn))
      {
        return false;
      }

      using (conn)
      {
        Helper.SpatiaLiteHelper helper = new Helper.SpatiaLiteHelper();

        if (helper.CheckIfFeatureSetExists(conn, fs))
        {
          this.ErrorMessage = "Unable to save spatial class: the class already exists in the dataset.";
          return false;
        }

        bool ok = false;
        await Task.Run(() =>
          {
            ok = helper.TrySaveFeatureSet(conn, fs);
          });

        if (!ok)
        {
          this.ErrorMessage = "Unable to save spatial class: " + helper.ErrorMessage;
          return false;
        }

        return true;
      }
    }

    /// <summary>
    /// Loads a spatial dataset from the database.
    /// </summary>
    /// <param name="dataset"></param>
    /// <returns></returns>
    public bool TryLoadSpatialDataset(out List<IFeatureSet> dataset)
    {
      var theDataset = new List<IFeatureSet>();
      SQLiteConnection conn;
      if (!TryOpen(out conn))
      {
        dataset = null;
        return false;
      }

      using (conn)
      {
        Helper.SpatiaLiteHelper helper = new Helper.SpatiaLiteHelper();

        var classInfo = helper.ListFeatureSets(conn);

        bool ok = true;
        foreach (var info in classInfo)
        {
          DotSpatial.Data.IFeatureSet featureSet;
          if (!helper.TryReadFeatureSet(conn, info, out featureSet))
          {
            this.ErrorMessage = string.Format("Unable to read feature set {0} from database: {1}", info.TableName, helper.ErrorMessage);
            log.Error(this.ErrorMessage);
            ok = false;
            break;
          }
          featureSet.Name = info.TableName;
          theDataset.Add(featureSet);
        }

        if (!ok)
        {
          dataset = null;
          return false;
        }

        dataset = theDataset;
        return true;
      }
    }
  }
}
