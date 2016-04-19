using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui
{
  [Export(typeof(IApplicationConfig))]
  class ApplicationConfig : IApplicationConfig
  {
    private const string Config_File = "config.ini";

    private string configFile;

    public ApplicationConfig()
    {
      this.configFile = Path.Combine(Util.AppData.GetApplicationDirectory(), Config_File);
    }
    
    public string GetValue(string section, string key)
    {
      string defaultValue = "";

      if (!File.Exists(this.configFile))
      {
        return defaultValue;
      }

      RVRMeander.Utils.IniFile ini = null;
      try
      {
        ini = new RVRMeander.Utils.IniFile(configFile);
        return ini.GetValue(section, key, defaultValue);
      }
      catch (Exception ex)
      {
        //ErrorMessage = Resources.Strings.UnableToOpenConfigurationFile;
        //log.Error("{0}: {1}: {2}", ErrorMessage, configFile, ex.Message);
        return defaultValue;
      }
      finally
      {
        if (ini != null)
        {
          ini.Dispose();
        }
      }
    }
    
    public string[] GetKeys(string section)
    {
      if (!File.Exists(this.configFile))
      {
        return new string[0];
      }

      RVRMeander.Utils.IniFile ini = null;
      try
      {
        ini = new RVRMeander.Utils.IniFile(configFile);
        return ini.GetEntryNames(section);
      }
      catch (Exception ex)
      {
        //ErrorMessage = Resources.Strings.UnableToOpenConfigurationFile;
        //log.Error("{0}: {1}: {2}", ErrorMessage, configFile, ex.Message);
        return new string[0];
      }
      finally
      {
        if (ini != null)
        {
          ini.Dispose();
        }
      }
    }

    public void SetValue(string section, string key, string value)
    {
      RVRMeander.Utils.IniFile ini = null;
      try
      {
        ini = new RVRMeander.Utils.IniFile(configFile);
        ini.SetValue(section, key, value);
      }
      catch (Exception ex)
      {
        //ErrorMessage = Resources.Strings.UnableToOpenConfigurationFile;
        //log.Error("{0}: {1}: {2}", ErrorMessage, configFile, ex.Message);
      }
      finally
      {
        if (ini != null)
        {
          ini.Dispose();
        }
      }
    }
  }
}
