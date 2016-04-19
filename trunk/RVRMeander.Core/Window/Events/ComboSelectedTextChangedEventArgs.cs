using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window.Events
{
  public delegate void ComboItemSelectedTextChangedEventHandler(object sender, ComboSelectedTextChangedEventArgs e);

  public class ComboSelectedTextChangedEventArgs : EventArgs
  {
    public string Text { get; set; }

    public ComboSelectedTextChangedEventArgs(string text)
    {
      Text = text;
    }
  }
}
