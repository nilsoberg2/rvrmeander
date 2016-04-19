using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IUpDownToolbarItem : IToolbarItem
  {
    int InitialValue { get; set; }

    int Value { get; set; }

    int MaximumValue { get; set; }

    int MinimumValue { get; set; }

    int Interval { get; set; }
  }
}
