using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Events
{
  public interface IListener<T>
  {
    void MessageReceived(T theEvent);
  }
}
