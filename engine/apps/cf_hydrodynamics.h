#ifndef __CF_HYDRODYNAMICS_H________________20101004084141__
#define __CF_HYDRODYNAMICS_H________________20101004084141__

#include "config_file_obj.h"
#include "cf_migration.h"
#include "cf_channel.h"
#include "../rmio/rmlog.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"


namespace ConfigFile
{
    struct Hydrodynamics
    {
        static Hydrodynamics* Create(GenericConfigObj& config, RMLogger* logger);
        virtual void InitializeHydrodynamicsObject(const RMChannel& channel, const Migration* mig, RMHydrodynamicsData*& data_in, RMHydrodynamics*& hydr_in) = 0;

        std::string method;
    };

    struct Hydrodynamics_Ikeda1981 : public Hydrodynamics
    {
        static Hydrodynamics_Ikeda1981* Create(GenericConfigObj& config, RMLogger* logger);
        virtual void InitializeHydrodynamicsObject(const RMChannel& channel, const Migration* mig, RMHydrodynamicsData*& data_in, RMHydrodynamics*& hydr_in);
        
        int last_node_method;
        double init_perturbation_velocity;
		double scour_factor;							//DM-FEB2011
		bool secondary_flow_correction;					//DM-FEB2012
		bool use_manning_for_friction_coefficient;		//DM-FEB2012
		bool amplify_friction_coefficient;				//DM-FEB2012
		double amplification_friction_coefficient;		//DM-FEB2012
    };
}


#endif//__CF_HYDRODYNAMICS_H________________20101004084141__
