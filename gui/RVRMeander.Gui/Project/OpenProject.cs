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
  [Export(typeof(Core.Window.IToolbarItem))]
  class OpenProject : Core.Window.IToolbarItem
  {
    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public OpenProject([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;
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

      string projStr = "";
      using (Utils.IniFile config = new Utils.IniFile(Path.Combine(dlg.SelectedPath, Const.Config_File_Name)))
      {
        projStr = config.GetValue(Const.Config_Section_Main, Const.Config_Key_Projection, "");
      }

      if (projStr.Length == 0)
      {
        MessageBox.Show("The selected directory is not valid");
        return;
      }

      this.eventMgr.Publish(new Core.Project.Events.PackageOpened() { PackagePath = dlg.SelectedPath });
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
    }
  }
}
