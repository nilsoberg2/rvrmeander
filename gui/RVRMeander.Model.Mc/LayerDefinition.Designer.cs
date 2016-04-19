namespace RVRMeander.Gui
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
            this.chkUseValleyCenterline = new System.Windows.Forms.CheckBox();
            this.btnLoadLayers = new System.Windows.Forms.Button();
            this.cmbSelectRiverCenterline = new System.Windows.Forms.ComboBox();
            this.cmbSelectValleyCenterline = new System.Windows.Forms.ComboBox();
            this.lblLayerDefinition = new System.Windows.Forms.Label();
            this.lblRiver = new System.Windows.Forms.Label();
            this.lblValley = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // chkUseValleyCenterline
            // 
            this.chkUseValleyCenterline.AutoSize = true;
            this.chkUseValleyCenterline.Location = new System.Drawing.Point(19, 163);
            this.chkUseValleyCenterline.Name = "chkUseValleyCenterline";
            this.chkUseValleyCenterline.Size = new System.Drawing.Size(124, 17);
            this.chkUseValleyCenterline.TabIndex = 0;
            this.chkUseValleyCenterline.Text = "Use valley centerline";
            this.chkUseValleyCenterline.UseVisualStyleBackColor = true;
            // 
            // btnLoadLayers
            // 
            this.btnLoadLayers.Location = new System.Drawing.Point(19, 12);
            this.btnLoadLayers.Name = "btnLoadLayers";
            this.btnLoadLayers.Size = new System.Drawing.Size(103, 31);
            this.btnLoadLayers.TabIndex = 1;
            this.btnLoadLayers.Text = "Load layers";
            this.btnLoadLayers.UseMnemonic = false;
            this.btnLoadLayers.UseVisualStyleBackColor = true;
            // 
            // cmbSelectRiverCenterline
            // 
            this.cmbSelectRiverCenterline.FormattingEnabled = true;
            this.cmbSelectRiverCenterline.Location = new System.Drawing.Point(19, 136);
            this.cmbSelectRiverCenterline.Name = "cmbSelectRiverCenterline";
            this.cmbSelectRiverCenterline.Size = new System.Drawing.Size(243, 21);
            this.cmbSelectRiverCenterline.TabIndex = 2;
            // 
            // cmbSelectValleyCenterline
            // 
            this.cmbSelectValleyCenterline.FormattingEnabled = true;
            this.cmbSelectValleyCenterline.Location = new System.Drawing.Point(19, 212);
            this.cmbSelectValleyCenterline.Name = "cmbSelectValleyCenterline";
            this.cmbSelectValleyCenterline.Size = new System.Drawing.Size(243, 21);
            this.cmbSelectValleyCenterline.TabIndex = 3;
            // 
            // lblLayerDefinition
            // 
            this.lblLayerDefinition.AutoSize = true;
            this.lblLayerDefinition.Location = new System.Drawing.Point(25, 75);
            this.lblLayerDefinition.Name = "lblLayerDefinition";
            this.lblLayerDefinition.Size = new System.Drawing.Size(183, 13);
            this.lblLayerDefinition.TabIndex = 4;
            this.lblLayerDefinition.Text = "Select the river and valley centerlines";
            // 
            // lblRiver
            // 
            this.lblRiver.AutoSize = true;
            this.lblRiver.Location = new System.Drawing.Point(25, 116);
            this.lblRiver.Name = "lblRiver";
            this.lblRiver.Size = new System.Drawing.Size(32, 13);
            this.lblRiver.TabIndex = 5;
            this.lblRiver.Text = "River";
            // 
            // lblValley
            // 
            this.lblValley.AutoSize = true;
            this.lblValley.Location = new System.Drawing.Point(25, 196);
            this.lblValley.Name = "lblValley";
            this.lblValley.Size = new System.Drawing.Size(35, 13);
            this.lblValley.TabIndex = 6;
            this.lblValley.Text = "Valley";
            // 
            // LayerDefinition
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.lblValley);
            this.Controls.Add(this.lblRiver);
            this.Controls.Add(this.lblLayerDefinition);
            this.Controls.Add(this.cmbSelectValleyCenterline);
            this.Controls.Add(this.cmbSelectRiverCenterline);
            this.Controls.Add(this.btnLoadLayers);
            this.Controls.Add(this.chkUseValleyCenterline);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "LayerDefinition";
            this.ShowIcon = false;
            this.Text = "Layer Definition";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox chkUseValleyCenterline;
        private System.Windows.Forms.Button btnLoadLayers;
        private System.Windows.Forms.ComboBox cmbSelectRiverCenterline;
        private System.Windows.Forms.ComboBox cmbSelectValleyCenterline;
        private System.Windows.Forms.Label lblLayerDefinition;
        private System.Windows.Forms.Label lblRiver;
        private System.Windows.Forms.Label lblValley;
    }
}