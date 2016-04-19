using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
  /// <summary>
  /// This class stores information about a table in SQLite.
  /// </summary>
  public class SQLiteTableMetadata
  {
    /// <summary>
    /// The  name of the table.
    /// </summary>
    public string TableName { get; set; }

    /// <summary>
    /// The name of the primary key column.
    /// </summary>
    public string PrimaryKeyColumnName { get; set; }

    /// <summary>
    /// Default primary key column name.
    /// </summary>
    public const string DefaultPrimaryKeyColumnName = "id";

    /// <summary>
    /// Default primary key column settings
    /// </summary>
    protected virtual string PrimaryKeyColumnSettings { get { return "NOT NULL PRIMARY KEY AUTOINCREMENT"; } }

    /// <summary>
    /// List of the columns in the table.
    /// </summary>
    public List<Column> Columns { get; protected set; }

    /// <summary>
    /// Create an object that describes a table with only the default primary key and
    /// associated settings.
    /// </summary>
    /// <param name="tableName">name of the table</param>
    public SQLiteTableMetadata(string tableName)
      : this(tableName, DefaultPrimaryKeyColumnName, new List<Column>())
    {
    }

    /// <summary>
    /// Create object with the given parameters.  Throws exception if the primary key column
    /// exists in the input column list and is not compatible with the needed column settings.
    /// </summary>
    /// <param name="tableName">name of the table</param>
    /// <param name="primaryKeyColumnName">name of the primary key column</param>
    /// <param name="columns">list of the columns (may be empty)</param>
    public SQLiteTableMetadata(string tableName, string primaryKeyColumnName, List<Column> columns)
    {
      this.TableName = tableName;
      this.PrimaryKeyColumnName = primaryKeyColumnName;
      this.Columns = new List<Column>();
      if (columns != null)
      {
        this.Columns.AddRange(columns);
      }
      int index = this.Columns.FindIndex(x => x.Name == this.PrimaryKeyColumnName);
      if (index >= 0 && (this.Columns[index].DataType != "INT" || this.Columns[index].Options != this.PrimaryKeyColumnSettings))
      {
        throw new Exception("Column list already contains a column that is not compatible with the primary key column settings.");
      }
      else
      {
        this.Columns.Add(new Column(this.PrimaryKeyColumnName, "INTEGER", ""));
      }
    }

    /// <summary>
    /// Generate SQL code for creating the table.  Checks the column names to make sure they
    /// are valid (contain only alphanumeric and underscore characters; throws an exception if
    /// not valid).
    /// </summary>
    /// <returns></returns>
    public string GetCreateSqlCode()
    {
      string cols = "";
      for (int i = 0; i < this.Columns.Count; i++)
      {
        if (!System.Text.RegularExpressions.Regex.IsMatch(this.Columns[i].Name, @"^[a-zA-Z0-9_]+$"))
          throw new Exception("Column name " + this.Columns[i].Name + " is not valid.");
        if (i > 0)
          cols += ",\n";
        cols += this.Columns[i].Name + " " + this.Columns[i].DataType + " " + this.Columns[i].Options;
        if (this.Columns[i].Name == this.PrimaryKeyColumnName)
          cols += " " + this.PrimaryKeyColumnSettings;
      }

      string sql = String.Format("CREATE TABLE IF NOT EXISTS {0} ({1});", this.TableName, cols);

      return sql;
    }
  }
}
