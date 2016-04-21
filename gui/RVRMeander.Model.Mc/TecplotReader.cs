using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  class TecplotReader : Core.Result.IResultSet
  {
    private string filePath;

    private Dictionary<string, double[,]> values;

    public TecplotReader(string resultName, string filePath)
    {
      this.filePath = filePath;
      this.values = new Dictionary<string, double[,]>();
      Name = resultName;
    }

    public bool LoadFile()
    {
      char[] splitChars = new char[] { '\t', ' ' };

      StreamReader sr = null;
      try
      {
        sr = new StreamReader(this.filePath);

        string currentZone = "";
        List<Tuple<double, double>> coords = new List<Tuple<double, double>>();

        string line = "";
        while ((line = sr.ReadLine()) != null)
        {
          line = line.Trim();
          string[] parts = line.Split(splitChars, StringSplitOptions.RemoveEmptyEntries);
          if (parts.Length < 2)
          {
            continue;
          }

          double x, y;
          if (!double.TryParse(parts[0], out x) || !double.TryParse(parts[1], out y))
          {
            if (parts[0].ToLower() == "zone" && parts[1].Length > 0)
            {
              parts = parts[1].Split('=');
              if (parts.Length == 2)
              {
                currentZone = parts[1].Trim('"');
              }
              if (coords.Count > 0)
              {
                SaveCoords(currentZone, coords);
                coords.Clear();
              }
            }
          }
          else
          {
            coords.Add(new Tuple<double, double>(x, y));
          }
        }

        if (coords.Count > 0)
        {
          SaveCoords(currentZone, coords);
          coords.Clear();
        }

        return true;
      }
      catch (Exception ex)
      {
        return false;
      }
      finally
      {
        if (sr != null)
          sr.Close();
      }
    }

    private void SaveCoords(string zoneName, List<Tuple<double, double>> coords)
    {
      double[,] data = new double[coords.Count, 2];
      for (int i = 0; i < coords.Count; i++)
      {
        data[i, 0] = coords[i].Item1;
        data[i, 1] = coords[i].Item2;
      }

      this.values[zoneName] = data;
    }

    public string Name { get; private set; }

    public IEnumerable<string> LayerNames { get { return this.values.Keys; } }

    public double[,] GetDataset(string layerName)
    {
      if (this.values.ContainsKey(layerName))
      {
        return this.values[layerName];
      }
      else
      {
        return new double[0, 0];
      }
    }
  }
}
