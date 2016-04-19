using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RVRMeander.Core.Project
{
  public interface IProjectManager
  {
    void SetProperty(string propertyName, string value);
    void SetProperty(string groupName, string propertyName, string value);

    /// <summary>
    /// Returns empty string if the property name doesn't exist.
    /// </summary>
    /// <param name="propertyName"></param>
    /// <returns></returns>
    string GetProperty(string propertyName);
    string GetProperty(string groupName, string propertyName);

    string[] GetProperties(string groupName);

    bool CreatePackage(string pathToPackageDir);
    
    /// <summary>
    /// Creates a unique directory within the project that starts with the given prefix
    /// and returns the path to it.
    /// </summary>
    /// <param name="dirNamePrefix"></param>
    /// <returns></returns>
    string CreateDirectory(string dirNamePrefix);
  }
}
