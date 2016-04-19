using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Project
{
  [Export(typeof(Core.Window.IMenuToolbarItem))]
  class OpenProject : Core.Window.IMenuToolbarItem
  {
    private const string Section_Recent = "PACKAGE.RECENT";
    private const string Key_Recent = "Recent";

    private List<string> menuItems;

    private Core.Events.IEventManager eventMgr;
    private IApplicationConfig appConfig;

    [Import]
    Core.Window.IInteractiveWindowContainer mainWindow;

    [ImportingConstructor]
    public OpenProject([Import]Core.Events.IEventManager eventMgr, [Import]IApplicationConfig appConfig)
    {
      this.eventMgr = eventMgr;
      this.appConfig = appConfig;
      this.menuItems = new List<string>();
    }

    public string Caption
    {
      get { return "Open"; }
    }

    public string Group
    {
      get { return "Project"; }
    }

    public string Toolbar
    {
      get { return "Main"; }
    }

    public void Click()
    {
      Core.Window.FolderBrowserDialogEx dlg = new Core.Window.FolderBrowserDialogEx();
      if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
      {
        return;
      }

      if (!DoTheOpen(dlg.SelectedPath))
      {
        return;
      }

      AddRecentProject(dlg.SelectedPath);
    }

    private bool DoTheOpen(string dirPath)
    {
      string projStr = "";
      using (RVRMeander.Utils.IniFile config = new RVRMeander.Utils.IniFile(Path.Combine(dirPath, Const.Config_File_Name)))
      {
        projStr = config.GetValue(Const.Config_Section_Main, Const.Config_Key_Projection, "");
      }

      if (projStr.Length == 0)
      {
        MessageBox.Show("The selected directory is not valid");
        return false;
      }

      this.eventMgr.Publish(new Core.Project.Events.PackageOpened() { PackagePath = dirPath });

      return true;
    }

    private void AddRecentProject(string projectPath)
    {
      string[] values = this.appConfig.GetKeys(Section_Recent);
      string key = Key_Recent + (values.Length + 1).ToString();
      this.appConfig.SetValue(Section_Recent, key, projectPath);
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
      get { return UiImages.open_32; }
    }

    public System.Drawing.Image SmallImage
    {
      get { return UiImages.open_32; }
    }

    public void ToolbarItemInitialized()
    {
      string[] keys = this.appConfig.GetKeys(Section_Recent);
      if (keys != null)
      {
        foreach (var key in keys)
        {
          this.menuItems.Add(this.appConfig.GetValue(Section_Recent, key));
        }
      }
      this.mainWindow.UpdateToolItem(this);
    }

    public IEnumerable<string> MenuItems
    {
      get { return this.menuItems; }
    }

    public void MenuItemClicked(string menuItemName)
    {
      if (!Directory.Exists(menuItemName))
      {
        return;
      }
      if (!DoTheOpen(menuItemName))
      {
        return;
      }
    }
  }
}
