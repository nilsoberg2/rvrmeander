using RVRMeander.Core.Project;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Project
{
  [Export(typeof(Core.Window.IToolbarItem))]
  class NewProject : Core.Window.IToolbarItem
  {
    private Core.Events.IEventManager eventMgr;
    private IProjectManager projectMgr;

    [ImportingConstructor]
    public NewProject([Import]Core.Events.IEventManager eventMgr, [Import]IProjectManager projectMgr)
    {
      this.eventMgr = eventMgr;
      this.projectMgr = projectMgr;
    }

    public string Caption
    {
      get { return "New"; }
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
      NewProjectForm form = new NewProjectForm(this.projectMgr);
      if (form.ShowDialog() != DialogResult.OK)
      {
        return;
      }

      this.eventMgr.Publish(new Core.Project.Events.PackageOpened() { PackagePath = form.PackagePath });
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
    }
  }
}
