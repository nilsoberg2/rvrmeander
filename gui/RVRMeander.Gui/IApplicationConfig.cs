using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui
{
  interface IApplicationConfig
  {
    string GetValue(string section, string key);
    void SetValue(string section, string key, string value);
    string[] GetKeys(string section);
  }
}
