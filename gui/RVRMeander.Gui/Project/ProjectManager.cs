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
  class ProjectManager : RVRMeander.Core.Project.IProjectManager,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    private Core.Events.IEventManager eventMgr;
    private string curProjectPath;

    [ImportingConstructor]
    public ProjectManager([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;

      this.curProjectPath = "";

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
      Utils.IniFile config = null;
      try
      {
        config = new Utils.IniFile(ConfigFilePath);
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
      Utils.IniFile config = null;
      try
      {
        config = new Utils.IniFile(ConfigFilePath);
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
        Utils.IniFile config = new Utils.IniFile(sr);
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
        Utils.IniFile config = new Utils.IniFile(sr);
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
    }

    public bool CreatePackage(string targetDirPath)
    {
      StreamReader sr = null;
      try
      {
        sr = new StreamReader(ConfigFilePath);
        Utils.IniFile config = new Utils.IniFile(sr);
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

  }
}
