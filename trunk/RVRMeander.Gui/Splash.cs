using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui
{
  //[System.ComponentModel.Composition.Export(typeof(Core.Window.IToolbarItem))]
  public partial class Splash : Form, Core.Window.IToolbarItem
  {
    private Timer timer;
    private bool closeForReal;

    public Splash()
    {
      InitializeComponent();
      this.timer = new Timer();
      this.timer.Interval = 2000;
      this.timer.Tick += timer_Tick;
    }

    void timer_Tick(object sender, EventArgs e)
    {
      //this.timer.Enabled = false;
      //Close();
    }


    #region IToolbarItem Members

    public string Caption
    {
      get { return "Splash"; }
    }

    public string Group
    {
      get { return "Debug"; }
    }

    public string Toolbar
    {
      get { return "Help"; }
    }

    public new void Click()
    {
      Show();
    }

    public bool IsChecked
    {
      get;
      set;
    }

    public bool IsMapInteractive { get { return false; } }

    public bool CheckOnClick
    {
      get { return false; }
    }
    public Image Image
    {
      get { return null; }
    }

    public Image SmallImage
    {
      get { return null; }
    }

    public void ToolbarItemInitialized()
    {
    }

    #endregion

    private void Splash_Load(object sender, EventArgs e)
    {
      //this.timer.Start();
    }

    internal void CloseForReal()
    {
      this.closeForReal = true;
      Close();
    }

    private void Splash_FormClosing(object sender, FormClosingEventArgs e)
    {
      e.Cancel = !this.closeForReal;
    }
  }
}
