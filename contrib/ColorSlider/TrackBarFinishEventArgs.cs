using System;
using System.Collections.Generic;
using System.Text;

namespace MB.Controls
{
  public delegate void TrackBarFinishEvent(TrackBarFinishEventArgs e);

  public class TrackBarFinishEventArgs : EventArgs
  {
    public int Value;

    public TrackBarFinishEventArgs(int value)
    {
      this.Value = value;
    }
  }
}
