using System.Configuration;

namespace RVRMeander.Gui.Gis.WebMap.Configuration
{
    public class ServiceProviderElementCollection : ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new ServiceProviderElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((ServiceProviderElement)element).Key;
        }
    }
}