namespace MetroFlow.Gui.Gis.DotSpatialImpl
{
  partial class ProjectionSelectionPanel
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

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.grpType = new System.Windows.Forms.GroupBox();
      this.radGeographic = new System.Windows.Forms.RadioButton();
      this.radProjected = new System.Windows.Forms.RadioButton();
      this.cmbMinorCategory = new System.Windows.Forms.ComboBox();
      this.cmbMajorCategory = new System.Windows.Forms.ComboBox();
      this.grpType.SuspendLayout();
      this.SuspendLayout();
      // 
      // grpType
      // 
      this.grpType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.grpType.Controls.Add(this.radGeographic);
      this.grpType.Controls.Add(this.radProjected);
      this.grpType.Location = new System.Drawing.Point(3, 3);
      this.grpType.Name = "grpType";
      this.grpType.Size = new System.Drawing.Size(305, 74);
      this.grpType.TabIndex = 13;
      this.grpType.TabStop = false;
      this.grpType.Text = "Projection Type";
      // 
      // radGeographic
      // 
      this.radGeographic.AutoSize = true;
      this.radGeographic.ImeMode = System.Windows.Forms.ImeMode.NoControl;
      this.radGeographic.Location = new System.Drawing.Point(24, 44);
      this.radGeographic.Name = "radGeographic";
      this.radGeographic.Size = new System.Drawing.Size(80, 17);
      this.radGeographic.TabIndex = 1;
      this.radGeographic.Text = "&Geographic";
      this.radGeographic.UseVisualStyleBackColor = true;
      this.radGeographic.CheckedChanged += new System.EventHandler(this.radProjected_CheckedChanged);
      // 
      // radProjected
      // 
      this.radProjected.AutoSize = true;
      this.radProjected.Checked = true;
      this.radProjected.ImeMode = System.Windows.Forms.ImeMode.NoControl;
      this.radProjected.Location = new System.Drawing.Point(24, 21);
      this.radProjected.Name = "radProjected";
      this.radProjected.Size = new System.Drawing.Size(70, 17);
      this.radProjected.TabIndex = 0;
      this.radProjected.TabStop = true;
      this.radProjected.Text = "&Projected";
      this.radProjected.UseVisualStyleBackColor = true;
      this.radProjected.CheckedChanged += new System.EventHandler(this.radProjected_CheckedChanged);
      // 
      // cmbMinorCategory
      // 
      this.cmbMinorCategory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.cmbMinorCategory.FormattingEnabled = true;
      this.cmbMinorCategory.Location = new System.Drawing.Point(3, 123);
      this.cmbMinorCategory.Name = "cmbMinorCategory";
      this.cmbMinorCategory.Size = new System.Drawing.Size(305, 21);
      this.cmbMinorCategory.TabIndex = 15;
      this.cmbMinorCategory.SelectedIndexChanged += new System.EventHandler(this.cmbMinorCategory_SelectedIndexChanged);
      // 
      // cmbMajorCategory
      // 
      this.cmbMajorCategory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                  | System.Windows.Forms.AnchorStyles.Right)));
      this.cmbMajorCategory.FormattingEnabled = true;
      this.cmbMajorCategory.Location = new System.Drawing.Point(3, 96);
      this.cmbMajorCategory.Name = "cmbMajorCategory";
      this.cmbMajorCategory.Size = new System.Drawing.Size(305, 21);
      this.cmbMajorCategory.TabIndex = 14;
      this.cmbMajorCategory.SelectedIndexChanged += new System.EventHandler(this.cmbMajorCategory_SelectedIndexChanged);
      // 
      // ProjectionSelectionPanel
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.grpType);
      this.Controls.Add(this.cmbMinorCategory);
      this.Controls.Add(this.cmbMajorCategory);
      this.Name = "ProjectionSelectionPanel";
      this.Size = new System.Drawing.Size(314, 148);
      this.grpType.ResumeLayout(false);
      this.grpType.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.GroupBox grpType;
    private System.Windows.Forms.RadioButton radGeographic;
    private System.Windows.Forms.RadioButton radProjected;
    private System.Windows.Forms.ComboBox cmbMinorCategory;
    private System.Windows.Forms.ComboBox cmbMajorCategory;


  }
}
