namespace RVRMeander.Gui.Gis
{
  partial class MapWindow
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
      this.toolStripContainer = new System.Windows.Forms.ToolStripContainer();
      this.splitContainer = new System.Windows.Forms.SplitContainer();
      this.legend = new DotSpatial.Controls.Legend();
      this.map = new DotSpatial.Controls.Map();
      this.toolStripContainer.ContentPanel.SuspendLayout();
      this.toolStripContainer.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).BeginInit();
      this.splitContainer.Panel1.SuspendLayout();
      this.splitContainer.Panel2.SuspendLayout();
      this.splitContainer.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStripContainer
      // 
      // 
      // toolStripContainer.ContentPanel
      // 
      this.toolStripContainer.ContentPanel.Controls.Add(this.splitContainer);
      this.toolStripContainer.ContentPanel.Size = new System.Drawing.Size(918, 677);
      this.toolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.toolStripContainer.Location = new System.Drawing.Point(0, 0);
      this.toolStripContainer.Name = "toolStripContainer";
      this.toolStripContainer.Size = new System.Drawing.Size(918, 702);
      this.toolStripContainer.TabIndex = 0;
      this.toolStripContainer.Text = "toolStripContainer1";
      // 
      // splitContainer
      // 
      this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer.Location = new System.Drawing.Point(0, 0);
      this.splitContainer.Name = "splitContainer";
      // 
      // splitContainer.Panel1
      // 
      this.splitContainer.Panel1.Controls.Add(this.legend);
      // 
      // splitContainer.Panel2
      // 
      this.splitContainer.Panel2.Controls.Add(this.map);
      this.splitContainer.Size = new System.Drawing.Size(918, 677);
      this.splitContainer.SplitterDistance = 212;
      this.splitContainer.TabIndex = 1;
      // 
      // legend
      // 
      this.legend.BackColor = System.Drawing.Color.White;
      this.legend.ControlRectangle = new System.Drawing.Rectangle(0, 0, 212, 677);
      this.legend.Dock = System.Windows.Forms.DockStyle.Fill;
      this.legend.DocumentRectangle = new System.Drawing.Rectangle(0, 0, 187, 428);
      this.legend.HorizontalScrollEnabled = true;
      this.legend.Indentation = 30;
      this.legend.IsInitialized = false;
      this.legend.Location = new System.Drawing.Point(0, 0);
      this.legend.MinimumSize = new System.Drawing.Size(5, 5);
      this.legend.Name = "legend";
      this.legend.ProgressHandler = null;
      this.legend.ResetOnResize = false;
      this.legend.SelectionFontColor = System.Drawing.Color.Black;
      this.legend.SelectionHighlight = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(238)))), ((int)(((byte)(252)))));
      this.legend.Size = new System.Drawing.Size(212, 677);
      this.legend.TabIndex = 0;
      this.legend.Text = "legend";
      this.legend.VerticalScrollEnabled = true;
      // 
      // map
      // 
      this.map.AllowDrop = true;
      this.map.BackColor = System.Drawing.Color.White;
      this.map.CollectAfterDraw = false;
      this.map.CollisionDetection = false;
      this.map.Dock = System.Windows.Forms.DockStyle.Fill;
      this.map.ExtendBuffer = false;
      this.map.FunctionMode = DotSpatial.Controls.FunctionMode.None;
      this.map.IsBusy = false;
      this.map.IsZoomedToMaxExtent = false;
      this.map.Legend = null;
      this.map.Location = new System.Drawing.Point(0, 0);
      this.map.Name = "map";
      this.map.ProgressHandler = null;
      this.map.ProjectionModeDefine = DotSpatial.Controls.ActionMode.Prompt;
      this.map.ProjectionModeReproject = DotSpatial.Controls.ActionMode.Prompt;
      this.map.RedrawLayersWhileResizing = false;
      this.map.SelectionEnabled = true;
      this.map.Size = new System.Drawing.Size(702, 677);
      this.map.TabIndex = 0;
      this.map.ZoomOutFartherThanMaxExtent = false;
      // 
      // MapWindow
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(918, 702);
      this.Controls.Add(this.toolStripContainer);
      this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.HideOnClose = true;
      this.Name = "MapWindow";
      this.Text = "Map Window";
      this.toolStripContainer.ContentPanel.ResumeLayout(false);
      this.toolStripContainer.ResumeLayout(false);
      this.toolStripContainer.PerformLayout();
      this.splitContainer.Panel1.ResumeLayout(false);
      this.splitContainer.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer)).EndInit();
      this.splitContainer.ResumeLayout(false);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ToolStripContainer toolStripContainer;
    private System.Windows.Forms.SplitContainer splitContainer;
    private global::DotSpatial.Controls.Map map;
    private DotSpatial.Controls.Legend legend;
  }
}