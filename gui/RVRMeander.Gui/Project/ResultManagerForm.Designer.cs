namespace RVRMeander.Gui.Project
{
  partial class ResultManagerForm
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
      this.toolStrip1 = new System.Windows.Forms.ToolStrip();
      this.listResults = new System.Windows.Forms.ListBox();
      this.comboSim = new System.Windows.Forms.ComboBox();
      this.label1 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.tsbClear = new System.Windows.Forms.ToolStripButton();
      this.toolStrip1.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStrip1
      // 
      this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbClear});
      this.toolStrip1.Location = new System.Drawing.Point(0, 0);
      this.toolStrip1.Name = "toolStrip1";
      this.toolStrip1.Size = new System.Drawing.Size(232, 25);
      this.toolStrip1.TabIndex = 0;
      this.toolStrip1.Text = "toolStrip1";
      // 
      // listResults
      // 
      this.listResults.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.listResults.FormattingEnabled = true;
      this.listResults.IntegralHeight = false;
      this.listResults.Location = new System.Drawing.Point(0, 98);
      this.listResults.Name = "listResults";
      this.listResults.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
      this.listResults.Size = new System.Drawing.Size(232, 328);
      this.listResults.TabIndex = 1;
      this.listResults.SelectedIndexChanged += new System.EventHandler(this.listResults_SelectedIndexChanged);
      this.listResults.DoubleClick += new System.EventHandler(this.listResults_DoubleClick);
      // 
      // comboSim
      // 
      this.comboSim.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.comboSim.FormattingEnabled = true;
      this.comboSim.Location = new System.Drawing.Point(0, 48);
      this.comboSim.Name = "comboSim";
      this.comboSim.Size = new System.Drawing.Size(232, 21);
      this.comboSim.TabIndex = 2;
      this.comboSim.SelectedIndexChanged += new System.EventHandler(this.comboSim_SelectedIndexChanged);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 32);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(58, 13);
      this.label1.TabIndex = 3;
      this.label1.Text = "Simulation:";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 79);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(41, 13);
      this.label2.TabIndex = 4;
      this.label2.Text = "Layers:";
      // 
      // tsbClear
      // 
      this.tsbClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.tsbClear.Image = global::RVRMeander.Gui.UiImages.clear;
      this.tsbClear.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.tsbClear.Name = "tsbClear";
      this.tsbClear.Size = new System.Drawing.Size(23, 22);
      this.tsbClear.Text = "Clear";
      this.tsbClear.Click += new System.EventHandler(this.tsbClear_Click);
      // 
      // ResultManagerForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(232, 426);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.comboSim);
      this.Controls.Add(this.listResults);
      this.Controls.Add(this.toolStrip1);
      this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Name = "ResultManagerForm";
      this.Text = "Result Manager";
      this.toolStrip1.ResumeLayout(false);
      this.toolStrip1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ToolStrip toolStrip1;
    private System.Windows.Forms.ListBox listResults;
    private System.Windows.Forms.ComboBox comboSim;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.ToolStripButton tsbClear;
  }
}