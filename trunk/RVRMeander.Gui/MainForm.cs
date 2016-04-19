using RVRMeander.Core.Window;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.ComponentModel.Composition.Hosting;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace RVRMeander.Gui
{
  [System.ComponentModel.Composition.Export(typeof(IInteractiveWindowContainer))]
  [System.ComponentModel.Composition.Export(typeof(IStatusBar))]
  public partial class MainForm : RibbonForm, IInteractiveWindowContainer, IStatusBar, RVRMeander.Progress.IProgressBar,
    Core.Events.IListener<Core.Window.Events.ToolbarItemStatusChanged>,
    Core.Events.IListener<Core.Window.Events.ToolbarItemCheckChanged>
  {
    private static readonly Log.ILogger log = Log.Util.GetDefaultLogger(typeof(MainForm));

    #region MEF Variables

    private CompositionContainer compContainer;

    [Import]
    Core.Events.IEventManager eventManager;

    [ImportMany]
    IEnumerable<RVRMeander.Core.Window.IInteractiveWindow> windows;

    //[Import]
    RVRMeander.Core.Window.IInteractiveWindowContainer container;

    [ImportMany]
    IEnumerable<RVRMeander.Core.Window.IToolbarItem> toolItems;

    [ImportMany]
    IEnumerable<RVRMeander.Core.Window.IToolbarItemArray> groupedToolItems;

    [ImportMany]
    protected IEnumerable<Lazy<RVRMeander.Core.Window.IMenuToolbarItem>> menuButtonToolItems;

    [ImportMany]
    protected IEnumerable<RVRMeander.Core.Window.IComboToolbarItem> comboToolItems;

    [ImportMany]
    protected IEnumerable<RVRMeander.Core.Window.IUpDownToolbarItem> upDownToolItems;

    [ImportMany]
    protected IEnumerable<RVRMeander.Core.Window.IHostedControlToolbarItem> hostedItems;

    #endregion

    private MRG.Controls.UI.LoadingCircleToolStripMenuItem circleProgressItem;

    /// <summary>
    /// This is a list of tools that are map-interactive, e.g. only one should be selected at a given time.
    /// </summary>
    private List<IToolbarItem> interactiveTools;

    private Dictionary<IToolbarItem, RibbonItem> toolToUiMap;

    public MainForm()
    {
      log.Info("Starting main form");
      InitializeComponent();

      this.circleProgressItem = new MRG.Controls.UI.LoadingCircleToolStripMenuItem();
      this.circleProgressItem.LoadingCircleControl.SpokeThickness = 2;
      this.circleProgressItem.LoadingCircleControl.NumberSpoke = 7;
      this.circleProgressItem.LoadingCircleControl.OuterCircleRadius = 6;
      this.circleProgressItem.LoadingCircleControl.InnerCircleRadius = 4;
      this.circleProgressItem.LoadingCircleControl.RotationSpeed = 80;
      this.circleProgressItem.LoadingCircleControl.Color = Color.DarkGray;
      this.statusStrip1.Items.Insert(this.statusStrip1.Items.Count - 1, circleProgressItem);

      this.interactiveTools = new List<IToolbarItem>();
      this.toolToUiMap = new Dictionary<IToolbarItem, RibbonItem>();
    }

    public void Show(IInteractiveWindow theWindow, DockingState state)
    {
      DockContent dockableWindow = theWindow as DockContent;
      if (dockableWindow != null)
      {
        DockState dockState = DockState.Document;
        if (state == DockingState.DockRight)
        {
          dockState = DockState.DockRight;
        }
        else if (state == DockingState.Float)
        {
          dockState = DockState.Float;
        }

        this.dockPanel.Invoke((MethodInvoker)delegate
        {
          dockableWindow.Show(this.dockPanel, dockState);
        });
      }
    }

    private void MainForm_Load(object sender, EventArgs e)
    {
      bool done = false;
      var ss = new Splash();
      System.Threading.ThreadPool.QueueUserWorkItem((x) =>
        {
          ss.Show();
          while (!done)
          {
            Application.DoEvents();
          }
          ss.CloseForReal();
        });

      ComposeMefParts();

      //if (this.container == null)
      //{
      //  //TODO: error
      //  throw new Exception("Unable to find window container");
      //}

      if (this.windows == null)
      {
        log.Fatal("Unable to compose windows");
        Close();
        return;
      }

      this.eventManager.AddListener(this);

      foreach (var win in this.windows)
      {
        win.Attach(this.container);
      }

      if (this.toolItems == null)
      {
        log.Fatal("Unable to compose toolbar items");
        Close();
      }

      if (this.groupedToolItems == null)
      {
        log.Fatal("Unable to compose grouped toolbar items");
        Close();
      }


      AddRibbonItems();


      // Hack to enable splash screen to display
      //System.Threading.Thread.Sleep(2000);
      done = true;
    }

    /// <summary>
    /// Perform MEF initialization
    /// </summary>
    private void ComposeMefParts()
    {
      var catalog = Utils.Mef.GetMefCatalogs();
      try
      {
        catalog.Catalogs.Add(new AssemblyCatalog("RVRMeander.Model.Mc.dll"));
      }
      catch (Exception ex)
      {
        log.Fatal("Unable to find one of the assemblies that MainFormCore adds to the assembly catalog: {0}", ex.Message);
      }
      //catalog.Catalogs.Add(new DirectoryCatalog(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory), "*.dll"));
      //catalog.Catalogs.Add(new DirectoryCatalog(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory), "*.exe"));
      this.compContainer = new CompositionContainer(catalog);

      try
      {
        this.compContainer.ComposeParts(this);
      }
      catch (System.Reflection.ReflectionTypeLoadException ex)
      {
        foreach (Exception inner in ex.LoaderExceptions)
        {
          log.Warning("Reflection error: {0}", inner.Message);
        }
      }
      catch (CompositionException ex)
      {
        log.Fatal(ex.ToString());
        //Close();
      }

      this.container = this;
    }


    /// <summary>
    /// Add windows and tools to the control ribbon.
    /// </summary>
    private void AddRibbonItems()
    {
      foreach (var aTool in this.toolItems)
      {
        var tool = aTool;
        RibbonPanel group = GetRibbonPanel(tool);
        RibbonButton toolItem = new RibbonButton(tool.Caption);
        SetupToolbarItem(tool, toolItem);
        group.Items.Add(toolItem);
        tool.ToolbarItemInitialized();
      }

      foreach (var aGroupedTool in this.groupedToolItems)
      {
        var groupedTool = aGroupedTool;
        RibbonPanel group = GetRibbonPanel(groupedTool);

        RibbonButtonList hostList = null;
        if (groupedTool.IsButtonList)
        {
          hostList = new RibbonButtonList();
          hostList.ButtonsSizeMode = RibbonElementSizeMode.DropDown;
          //hostList.MaxSizeMode = RibbonElementSizeMode.Compact;
          //hostList.MinSizeMode = RibbonElementSizeMode.Compact;
          //hostList.Text = "Hello";
          group.Items.Add(hostList);
        }

        foreach (var tool in groupedTool.ToolbarItems)
        {
          RibbonButton toolItem = new RibbonButton(tool.Caption);

          if (groupedTool.IsButtonList)
          {
            hostList.Buttons.Add(toolItem);
          }
          else
          {
            group.Items.Add(toolItem);
          }
          SetupToolbarItem(tool, toolItem);
        }
        groupedTool.ToolbarItemInitialized();
      }

      foreach (var aMenuTool in this.menuButtonToolItems)
      {
        var menuTool = aMenuTool.Value;
        RibbonPanel group = GetRibbonPanel(menuTool);

        RibbonButton theButton = new RibbonButton(menuTool.Caption);
        theButton.Style = RibbonButtonStyle.SplitDropDown;
        theButton.DropDownItemClicked +=
          delegate(object sender, RibbonItemEventArgs e)
          {
            menuTool.MenuItemClicked(e.Item.Text);
          };

        SetupToolbarItem(menuTool, theButton);

        group.Items.Add(theButton);

        foreach (var menuItem in menuTool.MenuItems)
        {
          RibbonItem item = new RibbonButton(menuItem);
          theButton.DropDownItems.Add(item);
        }
        if (theButton.DropDownItems.Count == 0)
        {
          theButton.Style = RibbonButtonStyle.Normal;
        }
        menuTool.ToolbarItemInitialized();
      }

      foreach (var aComboTool in this.comboToolItems)
      {
        var comboTool = aComboTool;
        RibbonPanel group = GetRibbonPanel(comboTool);

        RibbonComboBox theButton = new RibbonComboBox();
        theButton.Text = ""; // comboTool.Caption;
        theButton.AllowTextEdit = false;
        //if (theButton.Text.Length == 0)
        //{
        //  theButton.LabelWidth = 1;
        //}
        //else
        if (comboTool.Image == null)
        {
          RibbonLabel label = new RibbonLabel();
          label.Text = comboTool.Caption;
          group.Items.Add(label);
        }
        theButton.Image = comboTool.Image;
        //theButton.SmallImage = comboTool.SmallImage;
        //theButton.CheckOnClick = comboTool.CheckOnClick;
        //theButton.Style = RibbonButtonStyle.SplitDropDown;
        theButton.DropDownItemClicked +=
          delegate(object sender, RibbonItemEventArgs e)
          {
            comboTool.ComboItemClicked(e.Item.Text);
          };
        comboTool.ComboSelectedTextChanged +=
          delegate(object o, Core.Window.Events.ComboSelectedTextChangedEventArgs e)
          {
            //theButton.SelectedValue = e.Text;
            theButton.TextBoxText = e.Text;
          };

        this.toolToUiMap[comboTool] = theButton;

        group.Items.Add(theButton);

        //comboTool.ComboItemsChanged;
        foreach (var comboItem in comboTool.ComboItems)
        {
          RibbonItem item = new RibbonButton(comboItem);
          theButton.DropDownItems.Add(item);
        }

        comboTool.ComboItemsAdded +=
          delegate(object sender, Core.Window.Events.ComboItemsAddedEvent e)
          {
            theButton.DropDownItems.Clear();
            foreach (var item in comboTool.ComboItems)
            {
              theButton.DropDownItems.Add(new RibbonButton(item));
            }
          };

        comboTool.ToolbarItemInitialized();
      }

      foreach (var aUpDownTool in this.upDownToolItems)
      {
        var upDownTool = aUpDownTool;
        RibbonPanel group = GetRibbonPanel(upDownTool);

        RibbonUpDown theButton = new RibbonUpDown();
        theButton.Text = upDownTool.Caption;
        theButton.Image = upDownTool.Image;

        this.toolToUiMap[upDownTool] = theButton;

        theButton.Value = upDownTool.InitialValue.ToString();
        theButton.UpButtonClicked +=
          delegate(object sender, MouseEventArgs e)
          {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
              int val = 0;
              if (int.TryParse(theButton.Value, out val))
              {
                val += upDownTool.Interval;
                if (val <= upDownTool.MaximumValue)
                {
                  upDownTool.Value = val;
                  theButton.Value = val.ToString();
                  theButton.TextBoxText = theButton.Value;
                  upDownTool.Click();
                }
              }
            }
          };
        theButton.DownButtonClicked +=
          delegate(object sender, MouseEventArgs e)
          {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
              int val = 0;
              if (int.TryParse(theButton.Value, out val))
              {
                val -= upDownTool.Interval;
                if (val >= upDownTool.MinimumValue)
                {
                  upDownTool.Value = val;
                  theButton.Value = val.ToString();
                  theButton.TextBoxText = theButton.Value;
                  upDownTool.Click();
                }
              }
            }
          };

        ////theButton.
        ////theButton.SmallImage = comboTool.SmallImage;
        ////theButton.CheckOnClick = comboTool.CheckOnClick;
        ////theButton.Style = RibbonButtonStyle.SplitDropDown;
        //theButton.DropDownItemClicked +=
        //  delegate(object sender, RibbonItemEventArgs e)
        //  {
        //    upDownTool.ComboItemClicked(e.Item.Text);
        //  };
        //group.Items.Add(theButton);

        //upDownTool.ButtonStatusChanged +=
        //  delegate(object o, Window.ToolbarItemEvent e)
        //  {
        //    theButton.Enabled = e.IsEnabled;
        //  };

        ////comboTool.ComboItemsChanged;
        //foreach (var comboItem in upDownTool.ComboItems)
        //{
        //  RibbonItem item = new RibbonButton(comboItem);
        //  theButton.DropDownItems.Add(item);
        //}

        //upDownTool.ComboItemsAdded +=
        //  delegate(object sender, ComboItemsAddedEvent e)
        //  {
        //    theButton.DropDownItems.Clear();
        //    foreach (var item in upDownTool.ComboItems)
        //    {
        //      theButton.DropDownItems.Add(new RibbonButton(item));
        //    }
        //  };

        upDownTool.ToolbarItemInitialized();
      }

      foreach (var hostItem in this.hostedItems)
      {
        RibbonPanel group = GetRibbonPanel(hostItem);

        RibbonButton theButton = new RibbonButton();
        theButton.Text = hostItem.Caption;
        theButton.Image = hostItem.Image;
        theButton.SmallImage = hostItem.SmallImage;
        theButton.Style = RibbonButtonStyle.DropDown;

        this.toolToUiMap[hostItem] = theButton;

        var uiHost = new RibbonHost();
        uiHost.HostedControl = hostItem.UserControl;
        theButton.DropDownItems.Add(uiHost);

        hostItem.UserControlFinished +=
          delegate
          {
            uiHost.HostCompleted();
          };

        group.Items.Add(theButton);
        hostItem.ToolbarItemInitialized();
      }

      this.ribbon.Tabs.Sort(TabSorter);
    }

    private int TabSorter(RibbonTab x, RibbonTab y)
    {
      if (x.Text == "Main")
      {
        return -1;
      }
      else if (y.Text == "Main")
      {
        return 1;
      }
      else if (x.Text == "Help")
      {
        return 1;
      }
      else if (y.Text == "Help")
      {
        return -1;
      }
      else
      {
        return x.Text.CompareTo(y.Text);
      }
    }

    private void SetupToolbarItem(IToolbarItem tool, RibbonButton toolItem)
    {
      toolItem.Image = tool.Image;
      toolItem.SmallImage = tool.SmallImage;
      toolItem.CheckOnClick = tool.IsMapInteractive || tool.CheckOnClick;

      if (tool.IsMapInteractive)
      {
        this.interactiveTools.Add(tool);
      }

      this.toolToUiMap[tool] = toolItem;

      toolItem.Click +=
        delegate
        {
          if (tool.CheckOnClick || tool.IsMapInteractive)
          {
            toolItem.Checked = !tool.IsChecked;
            tool.IsChecked = !tool.IsChecked;
          }
          if (tool.IsMapInteractive)
          {
            foreach (var t in this.interactiveTools)
            {
              var ribbonItem = this.toolToUiMap[t];
              if (ribbonItem != toolItem)
              {
                ribbonItem.Checked = false;
                t.IsChecked = false;
              }
            }
          }
          tool.Click();
        };
    }

    protected RibbonPanel GetRibbonPanel(IToolbarItem menuTool)
    {
      string targetToolStripName = "toolStrip_" + menuTool.Toolbar;
      var toolStrip = this.ribbon.Tabs.Find(x => x.Tag == null ? false : x.Tag.ToString() == targetToolStripName);
      if (toolStrip == null)
      {
        toolStrip = new RibbonTab(menuTool.Toolbar);
        toolStrip.Tag = targetToolStripName;
        if (menuTool.Toolbar == "Main")
        {
          this.ribbon.Tabs.Insert(0, toolStrip);
          this.ribbon.ActiveTab = toolStrip;
        }
        else if (menuTool.Toolbar == "Help")
        {
          int index = this.ribbon.Tabs.Count - 1;
          if (index < 0)
          {
            index = 0;
          }
          this.ribbon.Tabs.Insert(index, toolStrip);
        }
        else
        {
          int index = 1;
          if (this.ribbon.Tabs.Count == 0)
          {
            index = 0;
          }
          this.ribbon.Tabs.Insert(index, toolStrip);
          //this.ribbon.Tabs.Add(toolStrip);
        }
      }

      RibbonPanel group = toolStrip.Panels.Find(x => x.Text == menuTool.Group);
      if (group == null)
      {
        group = new RibbonPanel(menuTool.Group);
        toolStrip.Panels.Add(group);
      }
      return group;
    }


    public void SetMessage(string message)
    {
      this.statusMessage2.Text = message;
    }

    public WindowOperation StartOperation()
    {
      this.BeginInvoke(new MethodInvoker(
        delegate
        {
          this.circleProgressItem.LoadingCircleControl.Active = true;
          circleProgressItem.LoadingCircleControl.Color = Color.DarkGreen;
          this.Enabled = false;
        }));
      return new WindowOperation(this);
    }

    public void StopOperation()
    {
      this.BeginInvoke(new MethodInvoker(
        delegate
        {
          this.statusProgress.Value = this.statusProgress.Minimum;
          this.circleProgressItem.LoadingCircleControl.Active = false;
          circleProgressItem.LoadingCircleControl.Color = Color.DarkGray;
          this.Enabled = true;
        }));
    }

    public void SetProgress(int percentage)
    {
      this.BeginInvoke(new MethodInvoker(
        delegate
        {
          this.circleProgressItem.LoadingCircleControl.Active = true;
          circleProgressItem.LoadingCircleControl.Color = Color.DarkGreen;
          this.statusProgress.Value = Math.Min(percentage, this.statusProgress.Maximum);
        }));
    }

    public void ResetProgress()
    {
      Reset();
    }

    public void Reset()
    {
      this.BeginInvoke(new MethodInvoker(
        delegate
        {
          this.circleProgressItem.LoadingCircleControl.Active = false;
          circleProgressItem.LoadingCircleControl.Color = Color.DarkGray;
          this.statusProgress.Value = this.statusProgress.Minimum;
        }));
    }

    public void MessageReceived(Core.Window.Events.ToolbarItemStatusChanged theEvent)
    {
      if (this.toolToUiMap.ContainsKey(theEvent.Item))
      {
        this.toolToUiMap[theEvent.Item].Enabled = theEvent.Enabled;
        var groupedItem = theEvent.Item as IToolbarItemArray;
        if (groupedItem != null)
        {
          foreach (var item in groupedItem.ToolbarItems)
          {
            this.toolToUiMap[item].Enabled = theEvent.Enabled;
          }
        }
      }
    }

    public void MessageReceived(Core.Window.Events.ToolbarItemCheckChanged theEvent)
    {
      if (this.toolToUiMap.ContainsKey(theEvent.Item))
      {
        this.toolToUiMap[theEvent.Item].Checked = theEvent.Checked;
      }
    }
  }
}
