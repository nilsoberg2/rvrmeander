namespace RVRMeander.Gui.Layers
{
    partial class LayerDefinition
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
      this.cmbSelectRiverCenterline = new System.Windows.Forms.ComboBox();
      this.cmbSelectValleyCenterline = new System.Windows.Forms.ComboBox();
      this.lblRiver = new System.Windows.Forms.Label();
      this.lblValley = new System.Windows.Forms.Label();
      this.btnApply = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // cmbSelectRiverCenterline
      // 
      this.cmbSelectRiverCenterline.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.cmbSelectRiverCenterline.FormattingEnabled = true;
      this.cmbSelectRiverCenterline.Location = new System.Drawing.Point(12, 25);
      this.cmbSelectRiverCenterline.Name = "cmbSelectRiverCenterline";
      this.cmbSelectRiverCenterline.Size = new System.Drawing.Size(260, 21);
      this.cmbSelectRiverCenterline.TabIndex = 2;
      // 
      // cmbSelectValleyCenterline
      // 
      this.cmbSelectValleyCenterline.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.cmbSelectValleyCenterline.FormattingEnabled = true;
      this.cmbSelectValleyCenterline.Location = new System.Drawing.Point(12, 76);
      this.cmbSelectValleyCenterline.Name = "cmbSelectValleyCenterline";
      this.cmbSelectValleyCenterline.Size = new System.Drawing.Size(260, 21);
      this.cmbSelectValleyCenterline.TabIndex = 3;
      // 
      // lblRiver
      // 
      this.lblRiver.AutoSize = true;
      this.lblRiver.Location = new System.Drawing.Point(9, 9);
      this.lblRiver.Name = "lblRiver";
      this.lblRiver.Size = new System.Drawing.Size(114, 13);
      this.lblRiver.TabIndex = 5;
      this.lblRiver.Text = "River Centerline Layer:";
      // 
      // lblValley
      // 
      this.lblValley.AutoSize = true;
      this.lblValley.Location = new System.Drawing.Point(9, 60);
      this.lblValley.Name = "lblValley";
      this.lblValley.Size = new System.Drawing.Size(117, 13);
      this.lblValley.TabIndex = 6;
      this.lblValley.Text = "Valley Centerline Layer:";
      // 
      // btnApply
      // 
      this.btnApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.btnApply.Location = new System.Drawing.Point(197, 103);
      this.btnApply.Name = "btnApply";
      this.btnApply.Size = new System.Drawing.Size(75, 23);
      this.btnApply.TabIndex = 7;
      this.btnApply.Text = "Apply";
      this.btnApply.UseVisualStyleBackColor = true;
      this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
      // 
      // LayerDefinition
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(284, 262);
      this.Controls.Add(this.btnApply);
      this.Controls.Add(this.lblValley);
      this.Controls.Add(this.lblRiver);
      this.Controls.Add(this.cmbSelectValleyCenterline);
      this.Controls.Add(this.cmbSelectRiverCenterline);
      this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Name = "LayerDefinition";
      this.ShowIcon = false;
      this.Text = "Layer Definition";
      this.ResumeLayout(false);
      this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmbSelectRiverCenterline;
        private System.Windows.Forms.ComboBox cmbSelectValleyCenterline;
        private System.Windows.Forms.Label lblRiver;
        private System.Windows.Forms.Label lblValley;
        private System.Windows.Forms.Button btnApply;
    }
}