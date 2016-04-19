namespace RVRMeander.Gui
{
  partial class MainForm
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
      WeifenLuo.WinFormsUI.Docking.DockPanelSkin dockPanelSkin2 = new WeifenLuo.WinFormsUI.Docking.DockPanelSkin();
      WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin autoHideStripSkin2 = new WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin();
      WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient4 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient8 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin dockPaneStripSkin2 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin();
      WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient dockPaneStripGradient2 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient9 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient5 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient10 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient dockPaneStripToolWindowGradient2 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient11 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient12 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient6 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient13 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient14 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
      this.ribbon = new System.Windows.Forms.Ribbon();
      this.ribbonButton1 = new System.Windows.Forms.RibbonButton();
      this.vS2012LightTheme1 = new WeifenLuo.WinFormsUI.Docking.VS2012LightTheme();
      this.ribbonTab2 = new System.Windows.Forms.RibbonTab();
      this.statusStrip1 = new System.Windows.Forms.StatusStrip();
      this.dockPanel = new WeifenLuo.WinFormsUI.Docking.DockPanel();
      this.statusMessage2 = new System.Windows.Forms.ToolStripStatusLabel();
      this.statusProgress = new System.Windows.Forms.ToolStripProgressBar();
      this.statusStrip1.SuspendLayout();
      this.SuspendLayout();
      // 
      // ribbon
      // 
      this.ribbon.Font = new System.Drawing.Font("Segoe UI", 9F);
      this.ribbon.Location = new System.Drawing.Point(0, 0);
      this.ribbon.Minimized = false;
      this.ribbon.Name = "ribbon";
      // 
      // 
      // 
      this.ribbon.OrbDropDown.BorderRoundness = 8;
      this.ribbon.OrbDropDown.Location = new System.Drawing.Point(0, 0);
      this.ribbon.OrbDropDown.Name = "";
      this.ribbon.OrbDropDown.Size = new System.Drawing.Size(527, 447);
      this.ribbon.OrbDropDown.TabIndex = 0;
      this.ribbon.OrbDropDown.Visible = false;
      this.ribbon.OrbImage = null;
      this.ribbon.OrbStyle = System.Windows.Forms.RibbonOrbStyle.Office_2010;
      this.ribbon.OrbText = "MetroFlow";
      this.ribbon.OrbVisible = false;
      // 
      // 
      // 
      this.ribbon.QuickAcessToolbar.Enabled = false;
      this.ribbon.QuickAcessToolbar.Items.Add(this.ribbonButton1);
      this.ribbon.RibbonTabFont = new System.Drawing.Font("Trebuchet MS", 9F);
      this.ribbon.Size = new System.Drawing.Size(1097, 151);
      this.ribbon.TabIndex = 0;
      this.ribbon.TabsMargin = new System.Windows.Forms.Padding(12, 26, 20, 0);
      this.ribbon.Text = "Ribbon Header";
      this.ribbon.ThemeColor = System.Windows.Forms.RibbonTheme.Purple;
      // 
      // ribbonButton1
      // 
      this.ribbonButton1.Enabled = false;
      this.ribbonButton1.Image = ((System.Drawing.Image)(resources.GetObject("ribbonButton1.Image")));
      this.ribbonButton1.MaxSizeMode = System.Windows.Forms.RibbonElementSizeMode.Compact;
      this.ribbonButton1.SmallImage = ((System.Drawing.Image)(resources.GetObject("ribbonButton1.SmallImage")));
      this.ribbonButton1.Text = "ribbonButton1";
      // 
      // ribbonTab2
      // 
      this.ribbonTab2.Tag = "testing";
      this.ribbonTab2.Text = "ribbonTab2";
      // 
      // statusStrip1
      // 
      this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusMessage2,
            this.statusProgress});
      this.statusStrip1.Location = new System.Drawing.Point(0, 716);
      this.statusStrip1.Name = "statusStrip1";
      this.statusStrip1.Size = new System.Drawing.Size(1097, 22);
      this.statusStrip1.TabIndex = 2;
      this.statusStrip1.Text = "statusStrip1";
      // 
      // dockPanel
      // 
      this.dockPanel.Dock = System.Windows.Forms.DockStyle.Fill;
      this.dockPanel.DockBackColor = System.Drawing.SystemColors.ControlDark;
      this.dockPanel.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
      this.dockPanel.Location = new System.Drawing.Point(0, 151);
      this.dockPanel.Name = "dockPanel";
      this.dockPanel.Size = new System.Drawing.Size(1097, 565);
      dockPanelGradient4.EndColor = System.Drawing.SystemColors.ControlLight;
      dockPanelGradient4.StartColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(122)))), ((int)(((byte)(204)))));
      autoHideStripSkin2.DockStripGradient = dockPanelGradient4;
      tabGradient8.EndColor = System.Drawing.SystemColors.Control;
      tabGradient8.StartColor = System.Drawing.SystemColors.Control;
      tabGradient8.TextColor = System.Drawing.SystemColors.ControlDarkDark;
      autoHideStripSkin2.TabGradient = tabGradient8;
      autoHideStripSkin2.TextFont = new System.Drawing.Font("Segoe UI", 9F);
      dockPanelSkin2.AutoHideStripSkin = autoHideStripSkin2;
      tabGradient9.EndColor = System.Drawing.Color.FromArgb(((int)(((byte)(204)))), ((int)(((byte)(206)))), ((int)(((byte)(219)))));
      tabGradient9.StartColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(122)))), ((int)(((byte)(204)))));
      tabGradient9.TextColor = System.Drawing.Color.White;
      dockPaneStripGradient2.ActiveTabGradient = tabGradient9;
      dockPanelGradient5.EndColor = System.Drawing.SystemColors.Control;
      dockPanelGradient5.StartColor = System.Drawing.SystemColors.Control;
      dockPaneStripGradient2.DockStripGradient = dockPanelGradient5;
      tabGradient10.EndColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(151)))), ((int)(((byte)(234)))));
      tabGradient10.StartColor = System.Drawing.SystemColors.Control;
      tabGradient10.TextColor = System.Drawing.Color.Black;
      dockPaneStripGradient2.InactiveTabGradient = tabGradient10;
      dockPaneStripSkin2.DocumentGradient = dockPaneStripGradient2;
      dockPaneStripSkin2.TextFont = new System.Drawing.Font("Segoe UI", 9F);
      tabGradient11.EndColor = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(220)))));
      tabGradient11.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
      tabGradient11.StartColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(122)))), ((int)(((byte)(204)))));
      tabGradient11.TextColor = System.Drawing.Color.White;
      dockPaneStripToolWindowGradient2.ActiveCaptionGradient = tabGradient11;
      tabGradient12.EndColor = System.Drawing.SystemColors.ControlLightLight;
      tabGradient12.StartColor = System.Drawing.SystemColors.ControlLightLight;
      tabGradient12.TextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(122)))), ((int)(((byte)(204)))));
      dockPaneStripToolWindowGradient2.ActiveTabGradient = tabGradient12;
      dockPanelGradient6.EndColor = System.Drawing.SystemColors.Control;
      dockPanelGradient6.StartColor = System.Drawing.SystemColors.Control;
      dockPaneStripToolWindowGradient2.DockStripGradient = dockPanelGradient6;
      tabGradient13.EndColor = System.Drawing.SystemColors.ControlDark;
      tabGradient13.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
      tabGradient13.StartColor = System.Drawing.SystemColors.Control;
      tabGradient13.TextColor = System.Drawing.SystemColors.GrayText;
      dockPaneStripToolWindowGradient2.InactiveCaptionGradient = tabGradient13;
      tabGradient14.EndColor = System.Drawing.SystemColors.Control;
      tabGradient14.StartColor = System.Drawing.SystemColors.Control;
      tabGradient14.TextColor = System.Drawing.SystemColors.GrayText;
      dockPaneStripToolWindowGradient2.InactiveTabGradient = tabGradient14;
      dockPaneStripSkin2.ToolWindowGradient = dockPaneStripToolWindowGradient2;
      dockPanelSkin2.DockPaneStripSkin = dockPaneStripSkin2;
      this.dockPanel.Skin = dockPanelSkin2;
      this.dockPanel.TabIndex = 3;
      this.dockPanel.Theme = this.vS2012LightTheme1;
      // 
      // statusMessage2
      // 
      this.statusMessage2.Name = "statusMessage2";
      this.statusMessage2.Size = new System.Drawing.Size(949, 17);
      this.statusMessage2.Spring = true;
      this.statusMessage2.Text = "---";
      this.statusMessage2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
      // 
      // statusProgress
      // 
      this.statusProgress.Name = "statusProgress";
      this.statusProgress.Size = new System.Drawing.Size(100, 16);
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1097, 738);
      this.Controls.Add(this.dockPanel);
      this.Controls.Add(this.statusStrip1);
      this.Controls.Add(this.ribbon);
      this.Name = "MainForm";
      this.Text = "RVR Meander";
      this.Load += new System.EventHandler(this.MainForm_Load);
      this.statusStrip1.ResumeLayout(false);
      this.statusStrip1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Ribbon ribbon;
    private System.Windows.Forms.RibbonButton ribbonButton1;
    private WeifenLuo.WinFormsUI.Docking.VS2012LightTheme vS2012LightTheme1;
    private System.Windows.Forms.RibbonTab ribbonTab2;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private WeifenLuo.WinFormsUI.Docking.DockPanel dockPanel;
    private System.Windows.Forms.ToolStripStatusLabel statusMessage2;
    private System.Windows.Forms.ToolStripProgressBar statusProgress;


  }
}

