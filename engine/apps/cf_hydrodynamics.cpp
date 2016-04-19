#pragma warning(disable : 4290)

#include "cf_hydrodynamics.h"
#include "config_file_util.h"
#include "error.h"
#include "util.h"
#include "../rmio/rmlog.h"
#include "../rmhydrodynamics/rmhydrodynamics_ikeda1981.h"


namespace ConfigFile
{
    Hydrodynamics* Hydrodynamics::Create(GenericConfigObj &config, RMLogger *logger)
    {
        std::string method;
        if (!configStringLookup(config, method, "hydrodynamics.method"))
            CONFIG_ERR("hydrodynamics.method");

        Hydrodynamics* obj = NULL;
        
        if (ToLower(method) == "ikeda et. al. 1981")
            obj = Hydrodynamics_Ikeda1981::Create(config, logger);
        else
		{
            CONFIG_ERR("hydrodynamics.method");
			return NULL;
		}

        if (obj != NULL)
            obj->method = method;

        return obj;
    }


    Hydrodynamics_Ikeda1981* Hydrodynamics_Ikeda1981::Create(GenericConfigObj &config, RMLogger *logger)
    {
        Hydrodynamics_Ikeda1981* obj = new Hydrodynamics_Ikeda1981();
	    if (!configIntLookup(config, obj->last_node_method, "hydrodynamics.last_node_method"))
            CONFIG_ERR("hydrodynamics.last_node_method");
        if (!configDoubleLookup(config, obj->init_perturbation_velocity, "hydrodynamics.init_perturbation_velocity"))
            CONFIG_ERR("hydrodynamics.init_perturbation_velocity");
		if (!configDoubleLookup(config, obj->scour_factor, "hydrodynamics.scour_factor"))					//DM-FEB2011	
            CONFIG_ERR("hydrodynamics.scour_factor");														//DM-FEB2011
		if (!configBoolLookup(config, obj->secondary_flow_correction, "hydrodynamics.secondary_flow_correction"))									//DM-FEB2012
            CONFIG_ERR("hydrodynamics.secondary_flow_correction");																					//DM-FEB2012
		if (!configBoolLookup(config, obj->use_manning_for_friction_coefficient, "hydrodynamics.use_manning_for_friction_coefficient"))				//DM-FEB2012	
            CONFIG_ERR("hydrodynamics.use_manning_for_friction_coefficient");																		//DM-FEB2012
		if (!configBoolLookup(config, obj->amplify_friction_coefficient, "hydrodynamics.amplify_friction_coefficient"))								//DM-FEB2012	
            CONFIG_ERR("hydrodynamics.amplify_friction_coefficient");																				//DM-FEB2012
        if (!configDoubleLookup(config, obj->amplification_friction_coefficient, "hydrodynamics.amplification_friction_coefficient"))				//DM-FEB2012
            CONFIG_ERR("hydrodynamics.amplification_friction_coefficient");																			//DM-FEB2012
        return obj;
    }

    void Hydrodynamics_Ikeda1981::InitializeHydrodynamicsObject(const RMChannel& channel, const Migration* mig, RMHydrodynamicsData*& data_in, RMHydrodynamics*& hydr_in)
    {
	    RMHydrodynamicsData_Ikeda1981* data = new RMHydrodynamicsData_Ikeda1981();
	    data_in = (RMHydrodynamicsData*)data;
	    RMHydrodynamics_Ikeda1981* hydr = new RMHydrodynamics_Ikeda1981();
	    hydr_in = (RMHydrodynamics*)hydr;

        data->UB_0 = init_perturbation_velocity;
		data->ALF = scour_factor;																			//DM-FEB2011
        data->C_0 = mig->init_curvature;
        data->U0 = channel.FLOW / (channel.D0 * channel.WIDTH);
		data->sec_flow_corr = secondary_flow_correction;													//DM-FEB2012
		data->use_mann_for_fric_coeff = use_manning_for_friction_coefficient;								//DM-FEB2012
		data->amplify_fric_coeff = amplify_friction_coefficient;											//DM-FEB2012
		data->ampl_fric_coeff = amplification_friction_coefficient;											//DM-FEB2012

	    if (last_node_method == HydrodynamicsLastNodeMethod_CalculatedWithCurvature)						//DM-FEB2012
		    data->HydrodynamicsLastNodeMethod = HydrodynamicsLastNodeMethod_CalculatedWithCurvature;		//DM-FEB2012
	    else if (last_node_method == HydrodynamicsLastNodeMethod_Extrapolation)								//DM-FEB2012
		    data->HydrodynamicsLastNodeMethod = HydrodynamicsLastNodeMethod_Extrapolation;					//DM-FEB2012
	    else if (last_node_method == HydrodynamicsLastNodeMethod_ZeroGradient)								//DM-FEB2012
		    data->HydrodynamicsLastNodeMethod = HydrodynamicsLastNodeMethod_ZeroGradient;					//DM-FEB2012	    
    }
}
