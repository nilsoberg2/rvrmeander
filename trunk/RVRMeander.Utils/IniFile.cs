using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace RVRMeander.Utils
{
  /// <summary>
  /// Encapsulates editing and access routines for reading Windows INI-style files.
  /// This is pure C# code, no P/Invoke required.
  /// </summary>
  /// <example>
  /// An example of the data that is parsed:
  /// 
  ///   [Test]
  ///   TestKey=99
  ///   [Test2]
  ///   TestKey=Test String
  ///   ;commentedline=notread
  ///   comments=cancomeafterwards ;comment
  ///   [Test3]
  ///   ;multiple keys result in an array
  ///   ;GetValue returns the first, GetValues returns an IList
  ///   TestKey=1
  ///   TestKey=2
  ///   TestKey=3
  /// </example>
  public class IniFile : IDisposable
  {
    #region Nested Classes
    /// <summary>
    /// Ini value status.
    /// </summary>
    protected enum IniValueStatus
    {
      /// <summary>
      /// Value has been updated by the SetValue function.
      /// </summary>
      Updated,

      /// <summary>
      /// Value has been removed by the RemoveEntry function.
      /// </summary>
      Deleted,

      /// <summary>
      /// Value with no change.
      /// </summary>
      None,
    }

    /// <summary>
    /// Each ini value has a status which is used to determine if a value has been
    /// updated or deleted.
    /// </summary>
    protected class IniValue
    {
      private List<string> values;
      public string Value
      {
        get
        {
          return (values == null || values.Count == 0 ? "" : values[0]);
        }
        set
        {
          values.Add(value);
        }
      }

      public string[] Values { get { return this.values.ToArray(); } }

      public int ValueCount { get { return this.values.Count; } }

      public IniValueStatus Status;

      public IniValue(string value, IniValueStatus status)
      {
        this.Status = status;
        this.values = new List<string>();
        if (value.Length > 0)
        {
          this.values.Add(value);
        }
      }
    }

    #endregion

    #region Properties

    /// <summary>
    /// True if the data source is a Stream and not a data file.
    /// </summary>
    public bool IsReadOnly { get; protected set; }

    //protected bool WriteInRealTime { get; set; }

    /// <summary>
    /// Path to the data source file.  If no file, then "".
    /// </summary>
    public string FilePath { get; protected set; }

    #endregion

    #region Fields

    //protected List<IniLine> data;
    //protected List<string> sections;
    protected Dictionary<string, Dictionary<string, IniValue>> valueMap;
    protected bool preserveCase = true;
    protected bool isDirty = false;

    #endregion

    #region Initialization

    /// <summary>
    /// Read in the specified ini file.  If the file does not exist, it will
    /// create an empty ini object which can be populated.
    /// </summary>
    /// <param name="filePath">path to file</param>
    public IniFile(string filePath)
      : this(filePath, true)
    {
    }

    /// <summary>
    /// Read in the specified ini file.  If the file does not exist, it will
    /// create an empty ini object which can be populated.
    /// </summary>
    /// <param name="filePath">path to file</param>
    /// <param name="preserveCase">true to ensure that section names and keys are case-sensitive</param>
    public IniFile(string filePath, bool preserveCase)
    {
      if (File.Exists(filePath))
      {
        using (StreamReader reader = new StreamReader(filePath))
        {
          Initialize(reader, false, true, preserveCase);
        }
      }
      else
      {
        Initialize(null, false, true, preserveCase);
      }

      this.FilePath = filePath;
    }

    /// <summary>
    /// Read data from the specified StreamReader.
    /// </summary>
    /// <param name="reader"></param>
    public IniFile(StreamReader reader)
      : this(reader, true)
    {
    }

    /// <summary>
    /// Read data from the specified StreamReader.
    /// </summary>
    /// <param name="reader"></param>
    /// <param name="preserveCase">true to ensure that section names and keys are case-sensitive</param>
    public IniFile(StreamReader reader, bool preserveCase)
    {
      Initialize(reader, false, false, preserveCase);
    }

    /// <summary>
    /// Create an empty ini object that can be written to a file or stream.
    /// </summary>
    public IniFile()
    {
      Initialize(null, false, false, true);
    }

    /// <summary>
    /// Perform initialization tasks.  If the data file exists, or the stream is not null, data
    /// is read in and parsed.
    /// </summary>
    /// <param name="reader"></param>
    /// <param name="readOnly">is the ini object read-only (changes are not allowed)</param>
    /// <param name="writeInRealTime"></param>
    /// <param name="preserveCase"></param>
    protected void Initialize(StreamReader reader, bool readOnly, bool writeInRealTime, bool preserveCase)
    {
      this.preserveCase = preserveCase;
      this.FilePath = "";
      this.IsReadOnly = readOnly;
      //if (this.IsReadOnly)
      //{
      //  this.WriteInRealTime = false;
      //}
      //else
      //{
      //  this.WriteInRealTime = writeInRealTime;
      //}

      //this.sections = new List<string>();
      //this.data = new List<IniLine>();
      this.valueMap = new Dictionary<string, Dictionary<string, IniValue>>(this.preserveCase ? StringComparer.Ordinal : StringComparer.OrdinalIgnoreCase);

      if (reader != null)
      {
        ReadData(reader);
      }
    }

    #endregion

    #region Accessor Functions

    /// <summary>
    /// Returns true if the specified section exists, false otherwise.
    /// </summary>
    /// <param name="section"></param>
    /// <returns></returns>
    public bool HasSection(string section)
    {
      return this.valueMap.ContainsKey(section);
    }

    /// <summary>
    /// Returns true if the specified section/key exists in the data structure, false otherwise.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <returns></returns>
    public bool HasEntry(string section, string key)
    {
      return this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted;
    }

    /// <summary>
    /// Set the specified value for the specified section/key.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    public void SetValue(string section, string key, object value)
    {
      if (this.IsReadOnly)
      {
        return;
      }

      if (!this.valueMap.ContainsKey(section))
      {
        this.valueMap.Add(section, CreateSectionDictionary());
      }

      string theValue = value == null ? "" : value.ToString();

      this.isDirty = true;

      if (!this.valueMap[section].ContainsKey(key))
      {
        this.valueMap[section].Add(key, new IniValue(theValue, IniValueStatus.None));
      }
      else
      {
        this.valueMap[section][key] = new IniValue(theValue, IniValueStatus.Updated);
      }

      //if (this.WriteInRealTime && this.FilePath.Length > 0)
      //{
      //  SetValueInRealTime(section, key, value);
      //}
    }

    /// <summary>
    /// Removes the specified entry from the ini-object.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    public void RemoveEntry(string section, string key)
    {
      if (!this.valueMap.ContainsKey(section))
      {
        return;
      }

      if (this.IsReadOnly)
      {
        return;
      }

      if (this.valueMap[section].ContainsKey(key))
      {
        this.isDirty = true;
        this.valueMap[section][key].Status = IniValueStatus.Deleted;
      }
    }

    /// <summary>
    /// Removes the specified section from the ini-object.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    public void RemoveSection(string section)
    {
      if (!this.valueMap.ContainsKey(section))
      {
        return;
      }

      if (this.IsReadOnly)
      {
        return;
      }

      this.isDirty = true;
      this.valueMap.Remove(section);
    }

    /// <summary>
    /// Gets a value from the ini object, with defaultValue being returned if the value
    /// does not exist.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    public string GetValue(string section, string key, string defaultValue)
    {
      if (this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted)
      {
        return this.valueMap[section][key].Value;
      }
      else
      {
        return defaultValue;
      }
    }

    /// <summary>
    /// Gets a list of values from the ini file.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    public string[] GetValues(string section, string key)
    {
      if (this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted)
      {
        return this.valueMap[section][key].Values;
      }
      else
      {
        return new string[0];
      }
    }

    /// <summary>
    /// Gets a value from the ini object, with defaultValue being returned if the value
    /// does not exist.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    public int GetValue(string section, string key, int defaultValue)
    {
      if (this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted)
      {
        int result = 0;
        if (int.TryParse(this.valueMap[section][key].Value, out result))
        {
          return result;
        }
        else
        {
          return defaultValue;
        }
      }
      else
      {
        return defaultValue;
      }
    }

    /// <summary>
    /// Gets a value from the ini object, with defaultValue being returned if the value
    /// does not exist.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    public bool GetValue(string section, string key, bool defaultValue)
    {
      if (this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted)
      {
        bool result = defaultValue;
        if (bool.TryParse(this.valueMap[section][key].Value, out result))
        {
          return result;
        }
        else
        {
          return defaultValue;
        }
      }
      else
      {
        return defaultValue;
      }
    }

    /// <summary>
    /// Gets a value from the ini object, with defaultValue being returned if the value
    /// does not exist.
    /// </summary>
    /// <param name="section"></param>
    /// <param name="key"></param>
    /// <param name="defaultValue"></param>
    /// <returns></returns>
    public double GetValue(string section, string key, double defaultValue)
    {
      if (this.valueMap.ContainsKey(section) && this.valueMap[section].ContainsKey(key) && this.valueMap[section][key].Status != IniValueStatus.Deleted)
      {
        double result = 0;
        if (double.TryParse(this.valueMap[section][key].Value, out result))
        {
          return result;
        }
        else
        {
          return defaultValue;
        }
      }
      else
      {
        return defaultValue;
      }
    }

    /// <summary>
    /// Gets a list of all of the section names in the ini object.
    /// </summary>
    /// <returns></returns>
    public string[] GetSectionNames()
    {
      string[] sectionList = new string[this.valueMap.Keys.Count];
      this.valueMap.Keys.CopyTo(sectionList, 0);
      return sectionList;
    }

    /// <summary>
    /// Gets a list of all of the entry names in the given section.
    /// </summary>
    /// <param name="section"></param>
    /// <returns></returns>
    public string[] GetEntryNames(string section)
    {
      if (!this.valueMap.ContainsKey(section))
      {
        return new string[0];
      }
      else
      {
        string[] entryNames = new string[this.valueMap[section].Keys.Count];
        this.valueMap[section].Keys.CopyTo(entryNames, 0);
        return entryNames;
      }
    }

    /// <summary>
    /// Returns a dictionary that maps keys to values for the given section.
    /// </summary>
    /// <param name="section"></param>
    /// <returns>Dictionary<string, string> if the section exists, empty Dictionary<string, string> otherwise</returns>
    public Dictionary<string, string> GetSectionData(string section)
    {
      if (this.valueMap.ContainsKey(section))
      {
        Dictionary<string, string> data = new Dictionary<string, string>();
        foreach (var kv in this.valueMap[section])
        {
          if (kv.Value.Status != IniValueStatus.Deleted)
          {
            data.Add(kv.Key, kv.Value.Value);
          }
        }
        return data;
      }
      else
      {
        return new Dictionary<string, string>();
      }
    }

    #endregion

    #region Input/Output Functions

    /// <summary>
    /// Read and parse ini data from the specified StreamReader.
    /// </summary>
    /// <param name="reader"></param>
    private void ReadData(StreamReader reader)
    {
      char[] splitChars = new char[] { '=' };

      string line = "";
      string curSection = "";
      while ((line = reader.ReadLine()) != null)
      {
        line = line.Trim();
        if (line.Length == 0 || line[0] == ';')
        {
          continue;
        }

        if (line[0] == '[' && line[line.Length - 1] == ']')
        {
          curSection = line.Substring(1, line.Length - 2);
          //this.sections.Add(curSection);
        }
        else
        {
          // If there is no section, we put the key/value pairs into a default section.
          if (!this.valueMap.ContainsKey(curSection))
          {
            this.valueMap.Add(curSection, CreateSectionDictionary());
          }

          string[] parts = line.Split(splitChars, 2, StringSplitOptions.RemoveEmptyEntries);

          string value = "";
          if (parts.Length >= 2)
          {
            value = parts[1];
          }

          if (this.valueMap[curSection].ContainsKey(parts[0]))
          {
            if (value.Length > 0)
            {
              this.valueMap[curSection][parts[0]].Value = value;
            }
          }
          else
          {
            this.valueMap[curSection].Add(parts[0], new IniValue(value, IniValueStatus.None));
          }
        }
      }
    }

    //private void SetValueInRealTime(string section, string key, object value)
    //{
    //  char[] splitChars = new char[] { '=' };

    //  if (!this.preserveCase)
    //  {
    //    key = key.ToLower();
    //  }

    //  string tempFile = Path.GetTempFileName();

    //  StreamWriter writer = new StreamWriter(tempFile);
    //  StreamReader reader = new StreamReader(this.FilePath);

    //  string rawline = "";
    //  string curSection = "";
    //  while ((rawline = reader.ReadLine()) != null)
    //  {
    //    string line = rawline.Trim();
    //    if (line[0] == '[' && line[line.Length - 1] == ']')
    //    {
    //      curSection = line.Substring(1, line.Length - 2);
    //      writer.WriteLine(rawline);
    //    }
    //    else if (curSection == section)
    //    {
    //      string[] parts = line.Split(splitChars, 2, StringSplitOptions.RemoveEmptyEntries);

    //      if (parts.Length >= 1 && (this.preserveCase && parts[0] == key) || (!this.preserveCase && parts[0].ToLower() == key))
    //      {
    //        writer.WriteLine(parts[0] + "=" + value.ToString());
    //      }
    //      else
    //      {
    //        writer.WriteLine(rawline);
    //      }
    //    }
    //    else
    //    {
    //      writer.WriteLine(rawline);
    //    }
    //  }

    //  writer.Close();
    //  reader.Close();

    //  File.Copy(tempFile, this.FilePath, true);
    //  File.Delete(tempFile);
    //}

    /// <summary>
    /// Save to the original data file, if one exists.
    /// </summary>
    /// <returns></returns>
    public bool Save()
    {
      if (!this.IsReadOnly && /*!this.WriteInRealTime &&*/ this.FilePath.Length != 0 && this.isDirty)
      {
        return Save(this.FilePath);
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Save the ini data to the given file.  If the file does not exist, it is
    /// created.  If the file exists, it will be merged with any values that
    /// already exist in the file.
    /// </summary>
    /// <param name="filePath"></param>
    /// <returns></returns>
    public bool Save(string filePath)
    {
      if (File.Exists(filePath))
      {
        return MergeWithExistingFile(filePath);
      }
      else
      {
        StreamWriter writer = new StreamWriter(filePath);
        SaveToStream(writer);
        writer.Close();
        return true;
      }
    }

    /// <summary>
    /// Save the ini data to the given stream.
    /// </summary>
    /// <param name="writer"></param>
    /// <returns></returns>
    public void SaveToStream(StreamWriter writer)
    {
      foreach (var kv1 in this.valueMap)
      {
        if (kv1.Value.Count > 0)
        {
          writer.WriteLine("[" + kv1.Key + "]");
          foreach (var kv in this.valueMap[kv1.Key])
          {
            if (kv.Value.Status != IniValueStatus.Deleted)
            {
              writer.WriteLine(kv.Key + "=" + kv.Value.Value);
            }
          }
        }
      }
      this.isDirty = false;
    }

    /// <summary>
    /// Merge any changes that we have made to the internal datastructure to the specified file.
    /// Will keep any data that is present in the file but is not present in the data structure.
    /// </summary>
    /// <param name="filePath"></param>
    /// <returns></returns>
    protected bool MergeWithExistingFile(string filePath)
    {
      char[] splitChars = new char[] { '=' };

      string tempFile = Path.GetTempFileName();

      StreamWriter writer = new StreamWriter(tempFile);
      StreamReader reader = new StreamReader(filePath);

      var dataCopy = CopyData();

      string rawline = "";
      string curSection = "";
      while ((rawline = reader.ReadLine()) != null)
      {
        string line = rawline.Trim();
        if (line.Length == 0)
        {
          continue;
        }

        if (line[0] == '[' && line[line.Length - 1] == ']')
        {
          string newSection = line.Substring(1, line.Length - 2);

          // Save any remaining items.
          if (dataCopy.ContainsKey(curSection))
          {
            string[] keys = new string[dataCopy[curSection].Count];
            dataCopy[curSection].Keys.CopyTo(keys, 0);
            foreach (string key in keys)
            {
              if (dataCopy[curSection][key].Status != IniValueStatus.Deleted)
              {
                writer.WriteLine(key + "=" + dataCopy[curSection][key].Value);
              }
              dataCopy[curSection].Remove(key);
            }
          }

          // If the new section doesn't exist, create an empty section so that we don't have
          // to check if the section exists later.
          if (!dataCopy.ContainsKey(newSection))
          {
            dataCopy.Add(newSection, CreateSectionDictionary());
          }

          curSection = newSection;

          writer.WriteLine(rawline);
        }
        else
        {
          string[] parts = line.Split(splitChars, 2, StringSplitOptions.RemoveEmptyEntries);

          if (parts.Length >= 1 && dataCopy[curSection].ContainsKey(parts[0]))
          {
            if (dataCopy[curSection][parts[0]].Status != IniValueStatus.Deleted)
            {
              writer.WriteLine(parts[0] + "=" + dataCopy[curSection][parts[0]].Value);
            }
            dataCopy[curSection].Remove(parts[0]);
          }
          else
          {
            writer.WriteLine(rawline);
          }
        }
      }

      foreach (var kv1 in dataCopy)
      {
        if (kv1.Value.Count > 0)
        {
          if (kv1.Key != curSection)
          {
            writer.WriteLine("[" + kv1.Key + "]");
          }

          foreach (var kv in dataCopy[kv1.Key])
          {
            writer.WriteLine(kv.Key + "=" + kv.Value.Value);
          }
        }
      }

      writer.Close();
      reader.Close();

      File.Copy(tempFile, filePath, true);
      File.Delete(tempFile);

      this.isDirty = false;

      return true;
    }

    /// <summary>
    /// Copy the internal data structure that holds the ini data.  Used for saving.
    /// </summary>
    /// <returns></returns>
    private Dictionary<string, Dictionary<string, IniValue>> CopyData()
    {
      Dictionary<string, Dictionary<string, IniValue>> dataCopy = new Dictionary<string, Dictionary<string, IniValue>>(this.preserveCase ? StringComparer.Ordinal : StringComparer.OrdinalIgnoreCase);
      foreach (var kv1 in this.valueMap)
      {
        dataCopy.Add(kv1.Key, CreateSectionDictionary(kv1.Value));
      }
      if (!dataCopy.ContainsKey(""))
      {
        dataCopy.Add("", CreateSectionDictionary());
      }
      return dataCopy;
    }

    /// <summary>
    /// Create an empty section dictionary with the correct case sensitivity settings.
    /// </summary>
    /// <returns></returns>
    private Dictionary<string, IniValue> CreateSectionDictionary()
    {
      return new Dictionary<string, IniValue>(this.preserveCase ? StringComparer.Ordinal : StringComparer.OrdinalIgnoreCase);
    }

    /// <summary>
    /// Create a copy of the given section dictionary with the correct case sensitivity settings.
    /// </summary>
    /// <returns></returns>
    private Dictionary<string, IniValue> CreateSectionDictionary(IDictionary<string, IniValue> dictionary)
    {
      return new Dictionary<string, IniValue>(dictionary, this.preserveCase ? StringComparer.Ordinal : StringComparer.OrdinalIgnoreCase);
    }

    #endregion

    #region Cleanup Stuff

    ~IniFile()
    {
      Dispose(false);
    }

    #region IDisposable Members

    /// <summary>
    /// Saves the file if there have been changes made.
    /// </summary>
    public void Dispose()
    {
      Dispose(true);
      GC.SuppressFinalize(this);
    }

    #endregion

    /// <summary>
    /// Saves the file if there have been changes made.
    /// </summary>
    /// <param name="disposingManagedResources"></param>
    protected virtual void Dispose(bool disposingManagedResources)
    {
      if (disposingManagedResources)
      {
      }

      Save();
    }

    #endregion
  }
}
