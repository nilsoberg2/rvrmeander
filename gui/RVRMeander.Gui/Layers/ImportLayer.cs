using DotSpatial.Data;
using DotSpatial.Topology;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Layers
{
  [Export(typeof(Core.Window.IToolbarItem))]
  class ImportLayer : Core.Window.IToolbarItem,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {

    [Import]
    private RVRMeander.Gui.Gis.MapWindow mapWindow;

    [Import]
    private Core.Project.IProjectManager projectMgr;

    private Core.Events.IEventManager eventMgr;
    private string pkgPath;

    [ImportingConstructor]
    public ImportLayer([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;
      this.eventMgr.AddListener(this);
    }

    private bool TryCreateFeatureSet(string pathToFile, DotSpatial.Topology.FeatureType type, out DotSpatial.Data.FeatureSet set)
    {
      char[] splitChars = new char[] { ' ', '\t' };
      set = new FeatureSet(type);

      if (!File.Exists(pathToFile))
      {
        return false;
      }

      StreamReader sr = null;
      try
      {
        sr = new StreamReader(pathToFile);

        List<Coordinate> coords = new List<Coordinate>();

        string line = "";
        while ((line = sr.ReadLine()) != null)
        {
          if (line.StartsWith("RMTEXT"))
          {
            continue;
          }

          string[] parts = line.Split(splitChars, StringSplitOptions.RemoveEmptyEntries);

          if (parts.Length < 2)
          {
            continue;
          }

          double x;
          if (!double.TryParse(parts[0], out x))
          {
            continue;
          }

          double y;
          if (!double.TryParse(parts[1], out y))
          {
            continue;
          }

          coords.Add(new Coordinate(x, y));
        }

        set.AddFeature(new LineString(coords));
        
        return true;
      }
      catch
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

    public void Click()
    {
      var dlg = new System.Windows.Forms.OpenFileDialog();
      dlg.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";

      if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
      {
        return;
      }

      string filePath = dlg.FileName;

      FeatureSet set;
      if (!TryCreateFeatureSet(filePath, FeatureType.Line, out set))
      {
        return;
      }

      string layerName = Path.GetFileNameWithoutExtension(filePath);

      // Save to the project directory
      string ext = ".shp";
      int c = 0;
      string targetPath = Path.Combine(this.pkgPath, layerName + ext);
      while (File.Exists(targetPath))
      {
        targetPath = Path.Combine(this.pkgPath, layerName + c.ToString() + ext);
        c++;
      }

      //if (!SetProjection(filePath, set))
      //{
      //  set.Projection = this.mapWindow.GetMap().Projection;
      //}

      try
      {
        set.SaveAs(targetPath, true);
      }
      catch (Exception ex)
      {
        //TODO: error
        return;
      }

      this.mapWindow.ImportShapefile(targetPath, layerName, "", false, true);
    }

    private bool SetProjection(string filePath, FeatureSet set)
    {
      string dir = Path.GetDirectoryName(filePath);
      string name = Path.GetFileNameWithoutExtension(filePath);

      try
      {
        DotSpatial.Projections.ProjectionInfo pi;
        string projString = "";

        string projPath = Path.Combine(dir, name + ".proj");
        if (File.Exists(projPath))
        {
          projString = SlurpFile(projPath).Trim();
          pi = DotSpatial.Projections.ProjectionInfo.FromEsriString(projString);
          set.Projection = pi;
          return true;
        }

        projPath = Path.Combine(dir, name + ".epsg");
        if (File.Exists(projPath))
        {
          projString = SlurpFile(projPath).Trim();
          if (projString.ToLower().StartsWith("epsg:"))
            projString = projString.Remove(0, 5);
          int epsgCode = 26771;
          int.TryParse(projString, out epsgCode);
          pi = DotSpatial.Projections.ProjectionInfo.FromEpsgCode(epsgCode);
          set.Projection = pi;
          return true;
        }

        projPath = Path.Combine(dir, name + ".proj4");
        if (File.Exists(projPath))
        {
          projString = SlurpFile(projPath).Trim();
          pi = DotSpatial.Projections.ProjectionInfo.FromProj4String(projString);
          set.Projection = pi;
          return true;
        }

        return false;
      }
      catch (Exception ex)
      {
        return false;
      }
    }

    private static string SlurpFile(string filePath)
    {
      StreamReader sr = null;
      try
      {
        sr = new StreamReader(filePath);
        StringBuilder sb = new StringBuilder();
        string line = "";
        while ((line = sr.ReadLine()) != null)
        {
          sb.Append(line);
          sb.Append(" ");
        }
        return sb.ToString();
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

    public string Caption
    {
      get { return "Import Text Layer"; }
    }

    public string Group
    {
      get { return "Editing"; }
    }

    public string Toolbar
    {
      get { return "Layer Definition"; }
    }

    public bool IsChecked { get; set; }

    public bool IsMapInteractive
    {
      get { return false; }
    }

    public bool CheckOnClick
    {
      get { return false; }
    }

    public System.Drawing.Image Image
    {
      get { return UiImages.new_32; }
    }

    public System.Drawing.Image SmallImage
    {
      get { return UiImages.new_32; }
    }

    public void ToolbarItemInitialized()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.pkgPath = theEvent.PackagePath;
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, theEvent != null && theEvent.PackagePath.Length > 0));
    }
  }
}
