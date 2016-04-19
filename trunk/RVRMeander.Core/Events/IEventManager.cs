using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Events
{
  public interface IEventManager
  {
    /// <summary>
    /// Attaches the listener to listen for events of its type.
    /// </summary>
    /// <param name="listener"></param>
    void AddListener(object listener);

    /// <summary>
    /// Publishes the event/message to all objects listening for events of type TEvent
    /// </summary>
    /// <param name="listener"></param>
    void Publish<TListener>(TListener listener);
  }
}
