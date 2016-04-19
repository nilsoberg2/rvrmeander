using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IComboToolbarItem : IToolbarItem
  {
    IEnumerable<string> ComboItems { get; }

    void ComboItemClicked(string comboItemName);

    event Events.ComboItemSelectedTextChangedEventHandler ComboSelectedTextChanged;

    /// <summary>
    /// Called when the button status is changed.
    /// </summary>
    event Events.ComboItemsAddedEventHandler ComboItemsAdded;

    void OnComboSelectedTextChanged(object o, Events.ComboSelectedTextChangedEventArgs e);
  }
}
