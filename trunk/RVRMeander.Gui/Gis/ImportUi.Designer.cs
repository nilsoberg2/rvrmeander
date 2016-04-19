namespace RVRMeander.Gui.Import
{
  partial class ImportUi
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
      this.btnStart = new System.Windows.Forms.Button();
      this.textFilePath = new System.Windows.Forms.TextBox();
      this.progLoading = new System.Windows.Forms.ProgressBar();
      this.btnBrowse = new System.Windows.Forms.Button();
      this.label1 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.btnSaveDataset = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // btnStart
      // 
      this.btnStart.Location = new System.Drawing.Point(12, 51);
      this.btnStart.Name = "btnStart";
      this.btnStart.Size = new System.Drawing.Size(75, 23);
      this.btnStart.TabIndex = 4;
      this.btnStart.Text = "Import";
      this.btnStart.UseVisualStyleBackColor = true;
      this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
      // 
      // textFilePath
      // 
      this.textFilePath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.textFilePath.Location = new System.Drawing.Point(12, 25);
      this.textFilePath.Name = "textFilePath";
      this.textFilePath.Size = new System.Drawing.Size(150, 20);
      this.textFilePath.TabIndex = 3;
      // 
      // progLoading
      // 
      this.progLoading.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.progLoading.Location = new System.Drawing.Point(12, 339);
      this.progLoading.Name = "progLoading";
      this.progLoading.Size = new System.Drawing.Size(213, 23);
      this.progLoading.TabIndex = 5;
      // 
      // btnBrowse
      // 
      this.btnBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.btnBrowse.Location = new System.Drawing.Point(168, 23);
      this.btnBrowse.Name = "btnBrowse";
      this.btnBrowse.Size = new System.Drawing.Size(57, 23);
      this.btnBrowse.TabIndex = 6;
      this.btnBrowse.Text = "Browse";
      this.btnBrowse.UseVisualStyleBackColor = true;
      this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 9);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(79, 13);
      this.label1.TabIndex = 7;
      this.label1.Text = "Import file path:";
      // 
      // label2
      // 
      this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 323);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(82, 13);
      this.label2.TabIndex = 8;
      this.label2.Text = "Import progress:";
      // 
      // btnSaveDataset
      // 
      this.btnSaveDataset.Location = new System.Drawing.Point(12, 124);
      this.btnSaveDataset.Name = "btnSaveDataset";
      this.btnSaveDataset.Size = new System.Drawing.Size(95, 23);
      this.btnSaveDataset.TabIndex = 9;
      this.btnSaveDataset.Text = "Save dataset";
      this.btnSaveDataset.UseVisualStyleBackColor = true;
      this.btnSaveDataset.Click += new System.EventHandler(this.btnSaveDataset_Click);
      // 
      // ImportUi
      // 
      this.AllowDrop = true;
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(237, 374);
      this.Controls.Add(this.btnSaveDataset);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.btnBrowse);
      this.Controls.Add(this.progLoading);
      this.Controls.Add(this.btnStart);
      this.Controls.Add(this.textFilePath);
      this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Name = "ImportUi";
      this.Text = "Geometry Import";
      this.DragDrop += new System.Windows.Forms.DragEventHandler(this.Import_DragDrop);
      this.DragEnter += new System.Windows.Forms.DragEventHandler(this.Import_DragEnter);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button btnStart;
    private System.Windows.Forms.TextBox textFilePath;
    private System.Windows.Forms.ProgressBar progLoading;
    private System.Windows.Forms.Button btnBrowse;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Button btnSaveDataset;
  }
}

