using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DotSpatial.Projections;

namespace MetroFlow.Gui.Gis.DotSpatialImpl
{
  public partial class ProjectionSelectionPanel : UserControl
  {
    #region Private Variables

    private ProjectionInfo _selectedCoordinateSystem;

    #endregion

    #region Public Properties

    /// <summary>
    /// Gets or sets the currently chosen coordinate system
    /// </summary>
    public ProjectionInfo SelectedCoordinateSystem
    {
      get { return _selectedCoordinateSystem; }
      set { _selectedCoordinateSystem = value; }
    }

    #endregion

    /// <summary>
    /// Constructor
    /// </summary>
    public ProjectionSelectionPanel()
    {
      InitializeComponent();
    }

    #region Methods

    /// <summary>
    /// Load the major categories (e.g. region) into combo box.
    /// </summary>
    public void LoadMajorCategories()
    {
      if (radProjected.Checked)
      {
        cmbMajorCategory.SuspendLayout();
        cmbMajorCategory.Items.Clear();
        string[] names = KnownCoordinateSystems.Projected.Names;
        foreach (string name in names)
        {
          cmbMajorCategory.Items.Add(name);
        }
        cmbMajorCategory.SelectedIndex = 0;
        cmbMajorCategory.ResumeLayout();
      }
      else
      {
        cmbMajorCategory.SuspendLayout();
        cmbMajorCategory.Items.Clear();
        string[] names = KnownCoordinateSystems.Geographic.Names;
        foreach (string name in names)
        {
          cmbMajorCategory.Items.Add(name);
        }
        cmbMajorCategory.SelectedIndex = 0;
        cmbMajorCategory.ResumeLayout();
      }
    }

    /// <summary>
    /// Load the minor categories (e.g. projections) into combo box.
    /// </summary>
    public void LoadMinorCategories()
    {
      if (radProjected.Checked)
      {
        CoordinateSystemCategory c = KnownCoordinateSystems.Projected.GetCategory((string)cmbMajorCategory.SelectedItem);
        if (c == null) return;
        cmbMinorCategory.SuspendLayout();
        cmbMinorCategory.Items.Clear();
        string[] names = c.Names;
        foreach (string name in names)
        {
          cmbMinorCategory.Items.Add(name);
        }
        cmbMinorCategory.SelectedIndex = 0;
        _selectedCoordinateSystem = c.GetProjection(names[0]);
        cmbMinorCategory.ResumeLayout();
      }
      else
      {
        CoordinateSystemCategory c = KnownCoordinateSystems.Geographic.GetCategory((string)cmbMajorCategory.SelectedItem);
        cmbMinorCategory.SuspendLayout();
        cmbMinorCategory.Items.Clear();
        string[] names = c.Names;
        foreach (string name in names)
        {
          cmbMinorCategory.Items.Add(name);
        }
        cmbMinorCategory.SelectedIndex = 0;
        _selectedCoordinateSystem = c.GetProjection(names[0]);
        cmbMinorCategory.ResumeLayout();
      }
    }

    /// <summary>
    /// Set the selected project from type and name.
    /// </summary>
    /// <param name="type">type is "Projected" or "Geographic"</param>
    /// <param name="name">name is "MajorCat/MinorCat" (e.g. "World/WGS1984")</param>
    public void SetProjectionFromName(string type, string name)
    {
      if (type == "Projected")
      {
        this.radProjected.Checked = true;
        this.radGeographic.Checked = false;
      }
      else
      {
        this.radProjected.Checked = false;
        this.radGeographic.Checked = true;
      }

      string[] np = name.Split(new char[] { '/' });
      if (np == null || np.Length != 2)
      {
        return;
      }

      int idx = this.cmbMajorCategory.Items.IndexOf(np[0]);
      if (idx < 0)
      {
        return;
      }

      this.cmbMajorCategory.SelectedIndex = idx;

      idx = this.cmbMinorCategory.Items.IndexOf(np[1]);
      if (idx < 0)
      {
        return;
      }

      this.cmbMinorCategory.SelectedIndex = idx;
    }

    #endregion

    #region Events

    /// <summary>
    /// Called when a projection is selected by the user from the combo boxes.
    /// </summary>
    public event EventHandler ProjSelected;

    /// <summary>
    /// Call event handler.
    /// </summary>
    protected void OnProjSelected()
    {
      if (ProjSelected != null)
        ProjSelected(this, new EventArgs());
    }

    #endregion

    #region UI Event Handlers

    private void radProjected_CheckedChanged(object sender, EventArgs e)
    {
      LoadMajorCategories();
    }

    private void cmbMajorCategory_SelectedIndexChanged(object sender, EventArgs e)
    {
      LoadMinorCategories();
    }

    private void cmbMinorCategory_SelectedIndexChanged(object sender, EventArgs e)
    {
      if (radProjected.Checked)
      {
        CoordinateSystemCategory c = KnownCoordinateSystems.Projected.GetCategory((string)cmbMajorCategory.SelectedItem);
        this.SelectedCoordinateSystem = c.GetProjection((string)cmbMinorCategory.SelectedItem);
        cmbMinorCategory.ResumeLayout();
      }
      else
      {
        CoordinateSystemCategory c = KnownCoordinateSystems.Geographic.GetCategory((string)cmbMajorCategory.SelectedItem);
        this.SelectedCoordinateSystem = c.GetProjection((string)cmbMinorCategory.SelectedItem);
        cmbMinorCategory.ResumeLayout();
      }
      OnProjSelected();
    }

    #endregion

  }
}
