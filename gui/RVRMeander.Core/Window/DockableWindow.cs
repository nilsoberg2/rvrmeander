using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WeifenLuo.WinFormsUI.Docking;

namespace RVRMeander.Core.Window
{
  [System.ComponentModel.Composition.Export(typeof(IInteractiveWindow))]
  public class DockableWindow : DockContent, IInteractiveWindow
  {
    protected IInteractiveWindowContainer mainWindow;

    public virtual void Show(DockingState state)
    {
      this.mainWindow.Show(this, state);
    }

    public virtual void Attach(IInteractiveWindowContainer windowContainer)
    {
      this.mainWindow = windowContainer;
    }
	
    private void InitializeComponent()
    {
      this.SuspendLayout();
      // 
      // DockableWindow
      // 
      this.ClientSize = new System.Drawing.Size(284, 262);
      this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.HideOnClose = true;
      this.Name = "DockableWindow";
      this.ResumeLayout(false);

    }
  }
}
