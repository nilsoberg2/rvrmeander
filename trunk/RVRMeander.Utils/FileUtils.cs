using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Utils.IO
{
  /// <summary>
  /// FileUtils provides some static functions that can be used for file and directory
  /// manipulation.
  /// </summary>
  public static class FileUtils
  {
    /// <summary>
    /// Returns the path to a uniquely-named temporary file in the specified directory and returns the path to that file.
    /// </summary>
    /// <param name="dirPath"></param>
    /// <returns></returns>
    public static string GetTempFile(string dirPath)
    {
      int c = 0;
      string filePath = Path.Combine(dirPath, "mftmp.tmp");
      while (File.Exists(filePath))
      {
        filePath = Path.Combine(dirPath, "mftmp_" + (c++).ToString() + ".tmp");
      }
      return filePath;
    }

    /// <summary>
    /// Returns a file-system safe version of the given file name (not a path).
    /// </summary>
    /// <param name="fileName">file name, not path</param>
    /// <returns></returns>
    public static string MakeSafeFileName(string fileName)
    {
      //string pattern = "(?:[^A-Za-z0-9\\-_\\.])";
      return System.Text.RegularExpressions.Regex.Replace(fileName, @"[^A-Za-z._\-0-9]", "_");
    }

    /// <summary>
    /// Copies a directory from one location to another, recursively.
    /// </summary>
    /// <param name="source">path to source directory</param>
    /// <param name="target">path to target directory</param>
    public static void CopyAll(string source, string target)
    {
      Contract.Requires(Directory.Exists(source), "Source directory is invalid");
      //Contract.Requires(Directory.Exists(Path.GetDirectoryName(target + "\\..")), "Target directory is invalid");

      CopyAll(new DirectoryInfo(source), new DirectoryInfo(target));
    }

    /// <summary>
    /// Copies a directory from one location to another, recursively.
    /// </summary>
    /// <param name="source">DirectoryInfo object relating to source path</param>
    /// <param name="target">DirectoryInfo object relating to target path</param>
    public static void CopyAll(DirectoryInfo source, DirectoryInfo target)
    {
      Contract.Requires(source.Exists, "Source directory is invalid");
      Contract.Ensures(target.Exists, "Failed to copy directory to target");

      if (!target.Exists)
      {
        target.Create();
      }

      foreach (FileInfo fi in source.GetFiles())
      {
        fi.CopyTo(Path.Combine(target.ToString(), fi.Name), true);
      }

      // Copy each subdirectory using recursion.
      foreach (DirectoryInfo subDir in source.GetDirectories())
      {
        DirectoryInfo nextTargetSubDir = target.CreateSubdirectory(subDir.Name);
        CopyAll(subDir, nextTargetSubDir);
      }
    }


  }
}
