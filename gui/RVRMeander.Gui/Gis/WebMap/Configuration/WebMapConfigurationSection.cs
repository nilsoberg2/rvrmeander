using System.Configuration;

namespace RVRMeander.Gui.Gis.WebMap.Configuration
{
    public class WebMapConfigurationSection : ConfigurationSection
    {
        [ConfigurationProperty("Services", IsDefaultCollection = false)]
        [ConfigurationCollection(typeof(ServiceProviderElementCollection))]
        public ServiceProviderElementCollection Services
        {
            get
            {
                return (ServiceProviderElementCollection)base["Services"];
            }
        }
    }
}
