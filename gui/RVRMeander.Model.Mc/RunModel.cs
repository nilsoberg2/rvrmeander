using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  [Export(typeof(Core.Window.IToolbarItem))]
  class RunModel : Core.Window.IToolbarItem, Core.Events.IListener<Core.Project.Events.PackageOpened>
  {
    [Import]
    private Core.Window.IStatusBar statusBar;

    [Import]
    private Core.Project.IProjectManager projectMgr;

    [Import]
    private ChannelProperties propsWindow;

    [Import]
    private Gui.Gis.IMapWindow mapWindow;

    private Core.Events.IEventManager eventMgr;

    [ImportingConstructor]
    public RunModel([Import]Core.Events.IEventManager eventMgr)
    {
      this.eventMgr = eventMgr;

      IsChecked = false;
      IsMapInteractive = false;
      CheckOnClick = false;

      this.eventMgr.AddListener(this);
    }

    #region IToolbarItem Members

    public string Caption
    {
      get { return "Run Sim"; }
    }

    public string Group
    {
      get { return Const.Group_Mc; }
    }

    public string Toolbar
    {
      get { return Const.Toolbar_Mc; }
    }

    public new void Click()
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));

      string dirPath = this.projectMgr.CreateDirectory("sim");
      string configFilePath = Path.Combine(dirPath, "sim.ini");
      string logFilePath = Path.Combine(dirPath, "sim.log");

      if (!PopulateSimulation(dirPath, configFilePath))
      {
        this.statusBar.SetMessage("Unable to setup the simulation");
        return;
      }

      RunSimulation(configFilePath, logFilePath);
    }

    public bool IsChecked
    {
      get;
      set;
    }

    public bool IsMapInteractive
    {
      get;
      private set;
    }

    public bool CheckOnClick
    {
      get;
      private set;
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
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, false));
    }

    #endregion

    private void RunSimulation(string configFilePath, string logFilePath)
    {
      var wrapUp = new Action<string>(delegate(string message)
      {
        this.statusBar.SetMessage(message);
        this.statusBar.Reset();
        this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, true));
      });

      ThreadPool.QueueUserWorkItem(new WaitCallback(delegate
      {
        PInvoke.SetEnvironmentVars();

        RvrDotNetAccess dll = new RvrDotNetAccess();
        if (!dll.Initialize(configFilePath, logFilePath))
        {
          wrapUp("Error in RVR Meander initialization");
          return;
        }

        int nC = dll.GetNumberOfIterations();

        if (nC < 0)
        {
          wrapUp("Error: unable to get number of iterations");
          return;
        }

        for (int i = 0; i < nC; i++)
        {
          if (!dll.ExecuteStep(i))
          {
            wrapUp("Error at step " + i);
            return;
          }
          this.statusBar.SetProgress(i * 100 / nC);
        }

        dll.Finish();

        wrapUp("Simulation completed successfully");
      }));
    }

    #region Export configuration

    private bool PopulateSimulation(string dirPath, string iniFilePath)
    {
      string valleyFile;
      string riverFile;
      if (!ExportShapeFiles(dirPath, out valleyFile, out riverFile))
      {
        return false;
      }

      Utils.IniFile config = null;
      try
      {
        config = new Utils.IniFile(iniFilePath);

        //Set header values regarding the units used by the user
        config.SetValue("RVRMEANDER", "UserInterfaceUnits", "ENGLISH");
        config.SetValue("RVRMEANDER", "MapUnits", "ENGLISH");

        //Call specific export routines passing the required variables/strings.
        ExportSection_Channel(config, valleyFile, riverFile);
        ExportSection_PrepCurv(config);
        ExportSection_Hydrodynamics(config);
        ExportSection_BankErosion(config, "", "");
        ExportSection_BankErosionProperties(config);
        ExportSection_Migration(config);
        ExportSection_Smoothing(config);
        ExportSection_Plotting(dirPath, config);
      }
      catch (Exception ex)
      {
        //TODO: log error
        return false;
      }
      finally
      {
        if (config != null)
        {
          config.Dispose();
        }
      }

      return true;
    }

    #region Export configuration values to project file

    private void ExportSection_Plotting(string dirPath, Utils.IniFile config)
    {
      string cat = "PLOTTING";

      config.SetValue(cat, "output_1D", Path.Combine(dirPath, "TecPlot" + "1D" + ".DAT"));
      config.SetValue(cat, "output_uv2D", Path.Combine(dirPath, "TecPlot" + "uv2D" + ".DAT"));
      config.SetValue(cat, "output_hde2D", Path.Combine(dirPath, "TecPlot" + "hde2D" + ".DAT"));
      config.SetValue(cat, "output_U1V12D", Path.Combine(dirPath, "TecPlot" + "U1V12D" + ".DAT"));
      config.SetValue(cat, "output_H1D1E12D", Path.Combine(dirPath, "TecPlot" + "H1D1E12D" + ".DAT"));
      config.SetValue(cat, "output_UUVV2D", Path.Combine(dirPath, "TecPlot" + "UUVV2D" + ".DAT"));
      config.SetValue(cat, "output_HHDDEE2D", Path.Combine(dirPath, "TecPlot" + "HHDDEE2D" + ".DAT"));
      config.SetValue(cat, "output_VELandTAU2D", Path.Combine(dirPath, "TecPlot" + "VELandTAU2D" + ".DAT"));
      config.SetValue(cat, "output_Sections2D", Path.Combine(dirPath, "TecPlot" + "Sections2D" + ".DAT"));
      config.SetValue(cat, "output_Hydro3D", Path.Combine(dirPath, "TecPlot" + "Hydro3D" + ".DAT"));
      config.SetValue(cat, "output_BedBanks3D", Path.Combine(dirPath, "TecPlot" + "BedBanks3D" + ".DAT"));
      config.SetValue(cat, "output_Floodplain", Path.Combine(dirPath, "TecPlot" + "Grid" + ".DAT"));
      config.SetValue(cat, "output_KarmorFile", Path.Combine(dirPath, "KarmorFile.DAT"));
      config.SetValue(cat, "output_IndicesFile", Path.Combine(dirPath, "IndicesFile.DAT"));
      config.SetValue(cat, "output_KarmorVisualizedFile", Path.Combine(dirPath, "KarmorVisualizedFile.DAT"));
    }

    private void ExportSection_Smoothing(Utils.IniFile config)
    {
      string cat = "SMOOTHING";

      //Verify with Nils that we need this method call
      //cf.SetValue(cat, "method", cboFilterOnCenterlineMethod.SelectedItem.ToString());
      config.SetValue(cat, "method", "Savitzky-Golay");
      config.SetValue(cat, "smoothing_order", 0); // No splines; no filter on centerline
      config.SetValue(cat, "curvature_filter", true);

      cat = "SMOOTHING.PARAMETERS";

      config.SetValue(cat, "iterations", 1);
      config.SetValue(cat, "window_nodes", 5);
      config.SetValue(cat, "polynomial_order", 2);
    }

    private void ExportSection_Migration(Utils.IniFile config)
    {
      string cat = "MIGRATION";

      config.SetValue(cat, "duration", this.propsWindow.PropDuration);
      config.SetValue(cat, "num_iterations", this.propsWindow.PropNumIterations);
      config.SetValue(cat, "filter_iterations", 10);
      config.SetValue(cat, "splines_initially", true);
      config.SetValue(cat, "plot_iterations", 5);
      config.SetValue(cat, "migrate_upstream_node", true);
      config.SetValue(cat, "migrate_downstream_node", true);
      config.SetValue(cat, "threshold_for_cutoff", 1.0);
      config.SetValue(cat, "init_curvature", 0.0);
    }

    private void ExportSection_BankErosionProperties(Utils.IniFile config)
    {
      string cat = "BANK_EROSION.BANK_EVOLUTION_PROPERTIES";

      config.SetValue(cat, "use_hydraulic_erosion", true);
      config.SetValue(cat, "type_law_hydraulic_erosion", 1); // Original
      config.SetValue(cat, "xs_division", 2);
      config.SetValue(cat, "gr_threshold", 0.016);
      config.SetValue(cat, "wp_threshold", 0.025);
      if (true)
        config.SetValue(cat, "max_erosion_dist", "");
      else
        config.SetValue(cat, "max_erosion_dist", 10.0);
      config.SetValue(cat, "min_distance", 0.05);
      config.SetValue(cat, "use_cantilever_failure", true);
      config.SetValue(cat, "undercut_threshold", 0.1);
      config.SetValue(cat, "use_planar_failure", false);
      config.SetValue(cat, "num_intersects", 3);
      config.SetValue(cat, "z_threshold", 0.1);
      config.SetValue(cat, "tension_crack_opt", 0.5);
      config.SetValue(cat, "slice_multip", 5);
      config.SetValue(cat, "positive_pore", true);
      config.SetValue(cat, "negative_pore", true);
      config.SetValue(cat, "hydrost_force", true);
      config.SetValue(cat, "accuracy", 0.005);
      config.SetValue(cat, "max_iter", 8);
      config.SetValue(cat, "analysis_method", 1); // Ordinary
      config.SetValue(cat, "lambda", 0.4);
      //cf.SetValue(cat, "tblock", txtCharacteristicPeriodOfExistenceForASlumpBlock.Text);
      //cf.SetValue(cat, "use_karmor", getTrueFalseValue(ref chkUseDimensionlessSlumpBlockArmoringFactor));
    }

    private void ExportSection_BankErosion(Utils.IniFile config, string initXsPropFile, string floodPlainFile)
    {
      string cat = "BANK_EROSION";

      if (true) // rbMigrationCoefficient.Checked
        config.SetValue(cat, "method", "Migration coefficient");
      else
        config.SetValue(cat, "method", "Physically-based");
      config.SetValue(cat, "floodplain_heterogeneity", false);
      config.SetValue(cat, "erosion_coeff", 0.00000030);
      config.SetValue(cat, "pb_method", 0); // Purely Erosional
      config.SetValue(cat, "bank_shear_stress_method", 1); // Stage based
      config.SetValue(cat, "node_to_monitor", 55);
      config.SetValue(cat, "update_width_method", 1); // WS-banks intersects; mean dist.
      config.SetValue(cat, "erosion_distance_method", 1); // WS-banks intersects
      config.SetValue(cat, "gap_elongation", 0.1);
      config.SetValue(cat, "regrid_centerline_nodes", true);
      config.SetValue(cat, "interpolate_after_splines", true);
      config.SetValue(cat, "interpolate_frequency", 10);
      config.SetValue(cat, "bank_interpolation_tolerance", 0.02);
      config.SetValue(cat, "initial_section_prop_file", initXsPropFile);
      config.SetValue(cat, "floodplain_file", floodPlainFile);
      config.SetValue(cat, "tblock", 0.0);
      config.SetValue(cat, "use_karmor", false);
    }

    private void ExportSection_Hydrodynamics(Utils.IniFile config)
    {
      string cat = "HYDRODYNAMICS";

      config.SetValue(cat, "method", "Ikeda Et. Al. 1981"); // Ikeda et al 1981
      config.SetValue(cat, "last_node_method", 1); // Extrapolation
      config.SetValue(cat, "init_perturbation_velocity", 0);
      config.SetValue(cat, "scour_factor", 5.0);
      config.SetValue(cat, "secondary_flow_correction", false);
    }

    private void ExportSection_PrepCurv(Utils.IniFile config)
    {
      string cat = "PREPROCESSING.CURVATURE";

      config.SetValue(cat, "method", 1); // Parametric
      config.SetValue(cat, "upstream_method", 2); // User-specified and constant
      config.SetValue(cat, "downstream_method", 1); // Extrapolated
      config.SetValue(cat, "curvature_capping", false);
    }

    private void ExportSection_Channel(Utils.IniFile config, string valleyFile, string riverFile)
    {
      string cat = "CHANNEL";

      config.SetValue(cat, "flow", this.propsWindow.PropFlow);
      config.SetValue(cat, "width", this.propsWindow.PropWidth);
      config.SetValue(cat, "sediment_size", this.propsWindow.PropSedimentSize);
      config.SetValue(cat, "water_density", 1000);
      config.SetValue(cat, "valley_slope", this.propsWindow.PropValleySlope);
      config.SetValue(cat, "upstream_bed_elevation", this.propsWindow.PropUpstreamBedElev);
      config.SetValue(cat, "num_transverse_nodes", 51);
      config.SetValue(cat, "centerline", riverFile);
      config.SetValue(cat, "valley_centerline", valleyFile);
      config.SetValue(cat, "manning_coefficient", this.propsWindow.PropManningsN);
      config.SetValue(cat, "mesh_generation_method", 1); //TODO: represent in GUI
      config.SetValue(cat, "use_valley_centerline", false);
      config.SetValue(cat, "threshold_regridding", 0.9); //TODO: represent in GUI
    }

    #endregion

    private bool ExportShapeFiles(string dirPath, out string valleyFile, out string riverFile)
    {
      valleyFile = "";
      riverFile = "";

      var valleyName = this.projectMgr.GetProperty("valley");
      //var valleyShpPath = this.mapWindow.GetLayerPath(valleyName);
      var riverName = this.projectMgr.GetProperty("river");
      //var riverShpPath = this.mapWindow.GetLayerPath(riverName);

      //if (!File.Exists(valleyShpPath))
      //{
      //  //TODO: error
      //  return false;
      //}

      //if (!File.Exists(riverShpPath))
      //{
      //  //TODO: error
      //  return false;
      //}

      valleyFile = Path.Combine(dirPath, "valley.txt");
      //if (!ExportShapeFileToText(valleyShpPath, valleyFile))
      if (!ExportLayerToText(valleyName, valleyFile))
      {
        //TODO: error
        return false;
      }

      riverFile = Path.Combine(dirPath, "river.txt");
      //if (!ExportShapeFileToText(riverShpPath, riverFile))
      if (!ExportLayerToText(riverName, riverFile))
      {
        //TODO: error
        return false;
      }

      return true;
    }

    private bool ExportLayerToText(string layerName, string outputFile)
    {
      double[,] xy = this.mapWindow.GetLayerPoints(layerName);

      int nRows = xy.GetUpperBound(0) + 1;
      if (xy.GetUpperBound(1) < 1) // number of cols (+1)
      {
        return false;
      }

      StreamWriter writer = new StreamWriter(outputFile);

      //Write coordinates to the file
      writer.WriteLine("RMTEXT\t" + nRows + "\t2");
      for (int i = 0; i < nRows; i++)
      {
        writer.WriteLine("{0} {1}", xy[i, 0], xy[i, 1]);
      }

      writer.Close();

      return true;
    }

    #endregion

    public void MessageReceived(Core.Project.Events.PackageOpened theEvent)
    {
      this.eventMgr.Publish(new Core.Window.Events.ToolbarItemStatusChanged(this, true));
    }
  }
}
