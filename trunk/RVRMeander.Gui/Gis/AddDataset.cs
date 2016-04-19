using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Gis
{
  [Export(typeof(Core.Window.IToolbarItem))]
  public class AddDataset : Core.Window.IToolbarItem,
    Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    [Import]
    private IMapWindow mapWindow;
    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public AddDataset([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;
      this.eventMgr.AddListener(this);
      IsChecked = false;
    }

    public string Caption
    {
      get { return "+Layer"; }
    }

    public string Group
    {
      get { return Const.Group; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar; }
    }

    public void Click()
    {
      OpenFileDialog dlg = new OpenFileDialog();
      dlg.Filter = "Shapefiles (*.shp)|*.shp|SpatiaLite DB (*.sqlite)|*.sqlite|All Files (*.*)|*.*";

      if (dlg.ShowDialog() != DialogResult.OK)
      {
        return;
      }

      foreach (string filePath in dlg.FileNames)
      {
        string ext = Path.GetExtension(filePath.ToLower());
        string name = Path.GetFileNameWithoutExtension(filePath);
        if (ext == ".shp")
        {
          this.mapWindow.ImportShapefile(filePath, name, "");
        }
        else if (ext == ".sqlite")
        {
          this.mapWindow.ImportSQLite(filePath, name);
        }
      }
    }

    public bool IsChecked { get; set; }

    public bool IsMapInteractive
    {
      get { return false; }
    }

    public bool CheckOnClick { get { return false; } }

    public System.Drawing.Image Image
    {
      get { return Icons.adddata_32; }
    }

    public System.Drawing.Image SmallImage
    {
      get { return Icons.adddata_16; }
    }

    public void ToolbarItemInitialized()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      bool isEnabled = theEvent != null && theEvent.PackagePath.Length > 0;
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, isEnabled));
    }
  }
}
