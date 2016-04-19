using RVRMeander.Core.Events;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RVRMeander.Gui
{
  [Export(typeof(RVRMeander.Core.Events.IEventManager))]
  class EventManager : RVRMeander.Core.Events.IEventManager
  {
    private Dictionary<Type, List<WeakReference>> eventListeners;

    public EventManager()
    {
      this.eventListeners = new Dictionary<Type, List<WeakReference>>();
    }

    public void AddListener(object listener)
    {
      lock (this.eventListeners)
      {
        var subscribesTypes = listener.GetType().GetInterfaces().
               Where(i => i.IsGenericType && i.GetGenericTypeDefinition()
                        == typeof(RVRMeander.Core.Events.IListener<>));
        WeakReference weakReference = new WeakReference(listener);
        foreach (Type subscribesType in subscribesTypes)
        {
          var subscribers = GetListeners(subscribesType);
          subscribers.Add(weakReference);
        }
      }
    }
    
    private List<WeakReference> GetListeners(Type listenerType)
    {
      List<WeakReference> listeners;
      var found = this.eventListeners.TryGetValue(listenerType, out listeners);
      if (!found)
      {
        listeners = new List<WeakReference>();
        this.eventListeners.Add(listenerType, listeners);
      }
      return listeners;
    }

    public void Publish<TListener>(TListener eventToPublish)
    {
      List<WeakReference> listeners = null;
      Type listenerType = typeof(IListener<>).MakeGenericType(typeof(TListener));
      List<WeakReference> listenersToRemove = new List<WeakReference>();
      lock (this.eventListeners)
      {
        listeners = GetListeners(listenerType);
      }

      // This is not thread-safe since the list could be modified between GetListeners and here (or during foreach)
      foreach (WeakReference listenerRef in listeners)
      {
        if (listenerRef.IsAlive)
        {
          IListener<TListener> listener = (IListener<TListener>)listenerRef.Target;
          var syncContext = SynchronizationContext.Current;
          if (syncContext == null)
            syncContext = new SynchronizationContext();
          syncContext.Post(s => listener.MessageReceived(eventToPublish), null);
        }
        else
          listenersToRemove.Add(listenerRef);
      }

      // Remove any orphaned listeners
      if (listenersToRemove.Any())
      {
        foreach (WeakReference weakReferenceToRemove in listenersToRemove)
        {
          listeners.Remove(weakReferenceToRemove);
        }
      }
    }
  }
}
