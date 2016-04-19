using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  /// <summary>
  /// This interface represents a toolbar item in the user interface.
  /// </summary>
  public interface IToolbarItem
  {
    /// <summary>
    /// This is the text caption of the tool item.
    /// </summary>
    string Caption { get; }

    /// <summary>
    /// This is which toolbar (e.g. ribbon group) the tool item belongs.
    /// </summary>
    string Group { get; }

    /// <summary>
    /// This is which toolbar (e.g. ribbon tab) the tool item belongs.
    /// </summary>
    string Toolbar { get; }

    /// <summary>
    /// This is called when the UI element is clicked.
    /// </summary>
    void Click();

    /// <summary>
    /// Reflects the current check state of the tool.
    /// </summary>
    bool IsChecked { get; set; }

    /// <summary>
    /// This is true if the tool interacts with the map.  This means that only one map-interactive
    /// tool can be selected at a given time.
    /// </summary>
    bool IsMapInteractive { get; }

    /// <summary>
    /// This is true if the tool should be checked/unchecked upon clicking.  Note this is for non-
    /// map-interactive tools.
    /// </summary>
    bool CheckOnClick { get; }

    /// <summary>
    /// This should be a 32x32 icon.  May be null.
    /// </summary>
    System.Drawing.Image Image { get; }

    /// <summary>
    /// This should be a 16x16 icon.  May be null.
    /// </summary>
    System.Drawing.Image SmallImage { get; }

    /// <summary>
    /// Called after the toolbar item has been added to the header control.
    /// </summary>
    void ToolbarItemInitialized();
  }
}
