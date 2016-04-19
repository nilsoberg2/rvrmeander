using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// Represents an SQLite non-geometry column.
  /// </summary>
  public struct Column
  {
    private string p;
    private Type type;

    /// <summary>
    /// Name of the column
    /// </summary>
    public string Name { get; set; }

    /// <summary>
    /// Data type of the column (e.g. INT)
    /// </summary>
    public string DataType { get; set; }

    /// <summary>
    /// Column options e.g. NOT NULL
    /// </summary>
    public string Options { get; set; }

    /// <summary>
    /// Creates the column with the specified name and data type (e.g. id, INT).
    /// Options default to NOT NULL.
    /// </summary>
    public Column(string name, string dataType)
      : this(name, dataType, "NOT NULL")
    {
    }

    /// <summary>
    /// Creates the column with the specified name and data type (e.g. id, INT).
    /// Options default to NOT NULL.
    /// </summary>
    public Column(string name, SQLiteDataType dataType)
      : this(name, dataType.ToString(), "NOT NULL")
    {
    }

    /// <summary>
    /// Creates the column with the specified name and data type (e.g. id, INT)
    /// and options (e.g. NOT NULL).
    /// </summary>
    public Column(string name, string dataType, string options)
      : this()
    {
      Name = name;
      DataType = dataType;
      Options = options;
    }

    /// <summary>
    /// Creates the column with the specified name and data type (e.g. id, INT)
    /// and options (e.g. NOT NULL).
    /// </summary>
    public Column(string name, SQLiteDataType dataType, string options)
      : this(name, dataType.ToString(), options)
    {
    }

    /// <summary>
    /// Creates the column with the specified name and data type (e.g. id, int)
    /// </summary>
    public Column(string name, Type type)
      : this(name, ConvertType(type))
    {
    }

    private static SQLiteDataType ConvertType(Type type)
    {
      if (type == typeof(int) || type == typeof(long))
      {
        return SQLiteDataType.INT;
      }
      else if (type == typeof(double) || type == typeof(float))
      {
        return SQLiteDataType.REAL;
      }
      else if (type == typeof(string))
      {
        return SQLiteDataType.TEXT;
      }
      else
      {
        return SQLiteDataType.NULL;
      }
    }
  }
}
