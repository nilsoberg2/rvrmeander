using RVRMeander.Core;
using RVRMeander.Core.Project;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Project
{
  public partial class NewProjectForm : Form
  {
    private IProjectManager projectMgr;

    public NewProjectForm(IProjectManager projectMgr)
    {
      InitializeComponent();

      this.projectMgr = projectMgr;

      Projection = "";
      PackagePath = "";
    }

    private void projSelPanel_ProjSelected(object sender, EventArgs e)
    {
      if (this.projSelPanel.SelectedCoordinateSystem != null)
      {
        //this.textProjStringEsri.Text = this.projSelPanel.SelectedCoordinateSystem.ToEsriString();
        //this.textProjStringProj4.Text = this.projSelPanel.SelectedCoordinateSystem.ToProj4String();
        //this.projection = new ProjectionInfo();
        //this.projection.CopyProperties(this.projSelPanel.SelectedCoordinateSystem);
        if (this.projSelPanel.SelectedCoordinateSystem.Authority != null && this.projSelPanel.SelectedCoordinateSystem.Authority.Length > 0 &&
          this.projSelPanel.SelectedCoordinateSystem.AuthorityCode > 0)
        {
          Projection = "EPSG:" + this.projSelPanel.SelectedCoordinateSystem.AuthorityCode.ToString();
        }
        else
        {
          Projection = this.projSelPanel.SelectedCoordinateSystem.ToProj4String();
        }
      }
    }

    private void btnOk_Click(object sender, EventArgs e)
    {
      if (Projection.Length == 0)
      {
        DialogResult = System.Windows.Forms.DialogResult.None;
        return;
      }

      Core.Window.FolderBrowserDialogEx dlg = new Core.Window.FolderBrowserDialogEx();
      if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
      {
        DialogResult = System.Windows.Forms.DialogResult.None;
        return;
      }

      this.projectMgr.CreatePackage(dlg.SelectedPath);
      this.projectMgr.SetProperty(Const.Config_Key_Projection, Projection);

      PackagePath = dlg.SelectedPath;

      DialogResult = System.Windows.Forms.DialogResult.OK;
    }

    public string Projection { get; set; }

    public string PackagePath { get; set; }
  }
}
