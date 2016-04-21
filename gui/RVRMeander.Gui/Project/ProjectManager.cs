using RVRMeander.Core.Result;
using RVRMeander.Utils;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Project
{
  [Export(typeof(RVRMeander.Core.Project.IProjectManager))]
  [Export(typeof(RVRMeander.Core.Result.IResultManager))]
  class ProjectManager : RVRMeander.Core.Project.IProjectManager, Core.Result.IResultManager,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {

    [ImportMany]
    private IEnumerable<Core.Result.IResultSetLoader> resultSetLoaders;

    private Core.Events.IEventManager eventMgr;
    private string curProjectPath;
    private Dictionary<string, Tuple<string, IResultSetLoader>> resultSets;

    [ImportingConstructor]
    public ProjectManager([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;

      this.curProjectPath = "";
      this.resultSets = new Dictionary<string, Tuple<string, IResultSetLoader>>();

      this.eventMgr.AddListener(this);
    }

    private string ConfigFilePath
    {
      get
      {
        return Path.Combine(this.curProjectPath, Const.Config_File_Name);
      }
    }

    public void SetProperty(string propertyName, string value)
    {
      if (!File.Exists(ConfigFilePath))
      {
        return;
      }
      IniFile config = null;
      try
      {
        config = new IniFile(ConfigFilePath);
        config.SetValue(Const.Config_Section_Main, propertyName, value);
      }
      catch (Exception ex)
      {
      }
      finally
      {
        if (config != null)
        {
          config.Dispose();
        }
      }
    }

    public void SetProperty(string groupName, string propertyName, string value)
    {
      if (!File.Exists(ConfigFilePath))
      {
        return;
      }
      IniFile config = null;
      try
      {
        config = new IniFile(ConfigFilePath);
        config.SetValue(groupName, propertyName, value);
      }
      catch (Exception ex)
      {
      }
      finally
      {
        if (config != null)
        {
          config.Dispose();
        }
      }
    }

    public string GetProperty(string propertyName)
    {
      return GetProperty(Const.Config_Section_Main, propertyName);
    }


    public string GetProperty(string groupName, string propertyName)
    {
      if (!File.Exists(ConfigFilePath))
      {
        return "";
      }
      StreamReader sr = null;
      try
      {
        sr = new StreamReader(ConfigFilePath);
        IniFile config = new IniFile(sr);
        return config.GetValue(groupName, propertyName, "");
      }
      catch (Exception ex)
      {
        return "";
      }
      finally
      {
        if (sr != null)
        {
          sr.Close();
        }
      }
    }

    public string[] GetProperties(string groupName)
    {
      if (!File.Exists(ConfigFilePath))
      {
        return new string[0];
      }
      StreamReader sr = null;
      try
      {
        sr = new StreamReader(ConfigFilePath);
        IniFile config = new IniFile(sr);
        return config.GetEntryNames(groupName);
      }
      catch (Exception ex)
      {
        return new string[0];
      }
      finally
      {
        if (sr != null)
        {
          sr.Close();
        }
      }
    }

    /// <summary>
    /// Creates a unique directory within the project that starts with the given prefix
    /// and returns the path to it.
    /// </summary>
    /// <param name="dirNamePrefix"></param>
    /// <returns></returns>
    public string CreateDirectory(string dirNamePrefix)
    {
      string dirPath = Path.Combine(this.curProjectPath, dirNamePrefix + ".0");
      int inc = 1;
      while (Directory.Exists(dirPath))
      {
        dirPath = Path.Combine(this.curProjectPath, dirNamePrefix + "." + inc.ToString());
        inc++;
      }

      Directory.CreateDirectory(dirPath);

      return dirPath;
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.curProjectPath = theEvent.PackagePath;

      string[] simDirs = GetProperties(Const.Config_Section_Results);
      foreach (var simDir in simDirs)
      {
        foreach (var loader in this.resultSetLoaders)
        {
          if (loader.CanLoad(Path.Combine(this.curProjectPath, simDir)))
          {
            this.resultSets[simDir] = new Tuple<string, IResultSetLoader>(GetProperty(Const.Config_Section_Results, simDir), loader);
            break;
          }
        }
      }
    }

    public bool CreatePackage(string targetDirPath)
    {
      StreamReader sr = null;
      try
      {
        sr = new StreamReader(ConfigFilePath);
        IniFile config = new IniFile(sr);
        config.SetValue(Const.Config_Section_Main, "CreationDate", DateTime.Now.ToString("o"));
        return true;
      }
      catch (Exception ex)
      {
        return false;
      }
      finally
      {
        if (sr != null)
        {
          sr.Close();
        }
      }
    }

    public void AddResultSet(string simDirName, string setName, Core.Result.IResultSetLoader loader)
    {
      //this.resultSets[simDirName] = new Tuple<string, Core.Result.IResultSet>(setName, set);
      this.resultSets[simDirName] = new Tuple<string, IResultSetLoader>(setName, loader);
      SetProperty(Const.Config_Section_Results, simDirName, setName);
    }

    public IEnumerable<string> ResultSetIds
    {
      get { return this.resultSets.Keys; }
    }

    public string GetResultSetName(string id)
    {
      return this.resultSets[id].Item1;
    }

    public bool LoadResultSet(string id, out IResultSet set)
    {
      if (!this.resultSets.ContainsKey(id))
      {
        set = null;
        return false;
      }
      var loader = this.resultSets[id].Item2;
      if (!loader.LoadResultSet(Path.Combine(this.curProjectPath, id), out set))
      {
        return false;
      }
      return true;
    }
  }
}
