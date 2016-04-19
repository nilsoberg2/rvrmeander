using DotSpatial.Data;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RVRMeander.Gui.Import
{
  [System.ComponentModel.Composition.Export(typeof(Window.IInteractiveWindow))]
  [System.ComponentModel.Composition.Export(typeof(Window.IToolbarItem))]
  public partial class ImportUi : Window.DockableWindow, Window.IInteractiveWindow, Window.IToolbarItem
  {
    private static readonly Log.ILogger log = Log.Util.GetDefaultLogger(typeof(ImportUi));
    private CompositionContainer compContainer;

    [System.ComponentModel.Composition.Import]
    RVRMeander.Gui.Window.IInteractiveWindowContainer container;

    [System.ComponentModel.Composition.Import]
    RVRMeander.Gui.Gis.IGisWindowFactory mapFactory;

    private List<IFeatureSet> dataset;

    public ImportUi()
    {
      InitializeComponent();

      this.compContainer = new CompositionContainer(RVRMeander.Utils.Mef.GetMefCatalogs());

      this.container = null;
      this.mapFactory = null;

      try
      {
        this.compContainer.ComposeParts(this);
      }
      catch (CompositionException ex)
      {
        log.Fatal(ex.ToString());
        Close();
      }

      if (this.container == null)
      {
        log.Error("Unable to find an IInteractiveWindowContainer");
      }

      if (this.mapFactory == null)
      {
        log.Error("Unable to find a IGisWindowFactory");
      }

      this.dataset = null;

      this.btnSaveDataset.Enabled = false;
    }


    #region IToolbarItem Members

    public string Caption
    {
      get { return "C"; }
    }

    public string Group
    {
      get { return "Import"; }
    }

    public string Toolbar
    {
      get { return "Data"; }
    }

    public new void Click()
    {
      Show(Window.DockingState.DockRight);
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

    public System.Drawing.Image Image
    {
      get { return null; }
    }
    public System.Drawing.Image SmallImage
    {
      get { return null; }
    }

    public void ToolbarItemInitialized()
    {
    }

    #endregion

    private void Import_DragDrop(object sender, DragEventArgs e)
    {
      if (this.container == null || this.mapFactory == null)
      {
        return;
      }

      string[] files = e.Data.GetData(DataFormats.FileDrop) as string[];
      if (files == null || files.Length == 0)
      {
        return;
      }
      else
      {
        string file = files[0];

        this.textFilePath.Text = file;
      }
    }

    private DotSpatial.Controls.IMapGroup CreateMapGroup(List<IFeatureSet> outputDataset)
    {
      DotSpatial.Controls.IMapGroup group = new DotSpatial.Controls.MapGroup();
      
      foreach (var spatialClass in outputDataset)
      {
        group.Layers.Add(spatialClass);
        group.Layers[group.Layers.Count - 1].LegendText = spatialClass.Name;
      }

      return group;
    }

    private void SetProgress(int percent)
    {
      this.Invoke((MethodInvoker)delegate
      {
        this.progLoading.Value = percent;
      });
    }

    private void Import_DragEnter(object sender, DragEventArgs e)
    {
      if (e.Data.GetDataPresent(DataFormats.FileDrop))
      {
        string[] files = e.Data.GetData(DataFormats.FileDrop) as string[];
        if (files == null || files.Length == 0)
        {
          e.Effect = DragDropEffects.None;
        }
        else
        {
          string ext = Path.GetExtension(files[0]).ToLower();
          if (ext == ".sqlite")
          {
            e.Effect = DragDropEffects.Link;
          }
          else
          {
            e.Effect = DragDropEffects.None;
          }
        }
      }
      else
      {
        e.Effect = DragDropEffects.None;
      }
    }

    private async void btnStart_Click(object sender, EventArgs e)
    {
      await Import(this.textFilePath.Text);
    }

    private async Task Import(string file)
    {
      if (Path.GetExtension(file).ToLower() == ".sqlite")
      {
        await ImportSQLite(file);
      }
    }

    private async Task ImportSQLite(string file)
    {
      Gis.IMapWindow window = null;

      Model.Geometry.SpatiaLite.Database db = new Model.Geometry.SpatiaLite.Database(file);
      List<IFeatureSet> dataset = null;

      bool ok = true;
      await Task.Run(() =>
        {
          ok = db.TryLoadSpatialDataset(out dataset);
        });

      if (!ok)
      {
        MessageBox.Show("Unable to load spatial dataset from file '" + file + "': " + db.ErrorMessage);
        return;
      }

      SetProgress(50);

      var mapGroup = new Gis.DotSpatialMapGroup();

      mapGroup.MapGroup = CreateMapGroup(dataset);
      mapGroup.MapGroup.LegendText = Path.GetFileNameWithoutExtension(file);

      window = mapFactory.CreateMapWindow();
      window.Attach(this.container);
      window.AddMapGroup(mapGroup);

      SetProgress(100);

      window.Show(Window.DockingState.Document);

      this.dataset = dataset;
    }

    private void btnBrowse_Click(object sender, EventArgs e)
    {
      Window.FolderBrowserDialogEx dlg = new Window.FolderBrowserDialogEx();
      if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
      {
        return;
      }

      this.textFilePath.Text = dlg.SelectedPath;
    }

    private async void btnSaveDataset_Click(object sender, EventArgs e)
    {
      if (this.dataset == null)
      {
        return;
      }

      SaveFileDialog dlg = new SaveFileDialog();
      dlg.Filter = "SpatiaLite Database (*.sqlite)|*.sqlite|All Files (*.*)|*.*";

      if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
      {
        return;
      }

      string outputFile = dlg.FileName;

      Model.Geometry.SpatiaLite.Database db = new Model.Geometry.SpatiaLite.Database(outputFile);

      SetProgress(0);

      int numClasses = this.dataset.Count;
      int i = 1;
      foreach (var sc in this.dataset)
      {
        await db.TrySaveSpatialClass(sc);
        SetProgress(i++ * 100 / numClasses);
      }
    }
  }
}
