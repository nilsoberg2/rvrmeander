#pragma warning(disable : 4290)

#include "config_file.h"
#include "cf_bankerosion.h"
#include <string>
#include "util.h"

namespace ConfigFile
{
    BankErosion* BankErosion::Create(GenericConfigObj &config, RMLogger* logger)
    {
        std::string method;
        if (!configStringLookup(config, method, "bank_erosion.method"))
            CONFIG_ERR("bank_erosion.method");

        BankErosion* obj = NULL;
        
        if (ToLower(method) == "migration coefficient")
            obj = BankErosion_MigrationCoefficient::Create(config, logger);
        else if (ToLower(method) == "physically-based")
            obj = BankErosion_PhysicallyBased::Create(config, logger);
        else
            return NULL;

        if (obj != NULL)
            obj->method = method;

        return obj;
    }

    BankErosion_MigrationCoefficient* BankErosion_MigrationCoefficient::Create(GenericConfigObj& config, RMLogger* logger)
    {
        BankErosion_MigrationCoefficient* obj = new BankErosion_MigrationCoefficient();
        if (!configDoubleLookup(config, obj->erosion_coeff, "bank_erosion.erosion_coeff"))
            CONFIG_ERR("bank_erosion.erosion_coeff");

		if (!configBoolLookup(config, obj->floodplain_heterogeneity, "bank_erosion.floodplain_heterogeneity"))	//DM-FEB2011
            CONFIG_ERR("bank_erosion.floodplain_heterogeneity");												//DM-FEB2011

		if (!configStringLookup(config, obj->floodplain_file, "bank_erosion.floodplain_file"))				//DM-FEB2011
            CONFIG_ERR("bank_erosion.floodplain_file");														//DM-FEB2011

        return obj;
    }

    void BankErosion_MigrationCoefficient::InitializeBankErosionObject(RMBankErosionData *&data, RMBankErosion *&be)
    {
	    data = new RMBankErosionData();
	    be = new RMBankErosion();

	    // bank erosion
	    data->MethodBankEvolution = 1;
	    data->EC = erosion_coeff;
		data->FloodplainHeterogeneity = floodplain_heterogeneity;//DM-FEB2011
		if(data->FloodplainHeterogeneity == true)data->floodplain_file = floodplain_file; //DM-FEB2011
    }

    BankErosion_PhysicallyBased* BankErosion_PhysicallyBased::Create(GenericConfigObj &config, RMLogger* logger)
    {
        BankErosion_PhysicallyBased* obj = new BankErosion_PhysicallyBased();
	    if (!configIntLookup(config, obj->pb_method, "bank_erosion.pb_method"))											//SEP2009
            CONFIG_ERR("bank_erosion.pb_method");																							//SEP2009
		if (!configBoolLookup(config, obj->reduce_stress_at_bank, "bank_erosion.reduce_stress_at_bank"))				//DM-FEB2012
            CONFIG_ERR("bank_erosion.reduce_stress_at_bank");															//DM-FEB2012
        if (!configDoubleLookup(config, obj->reduction_stress_at_bank, "bank_erosion.reduction_stress_at_bank"))		//DM-FEB2012
            CONFIG_ERR("bank_erosion.reduction_stress_at_bank");														//DM-FEB2012
	    if (!configIntLookup(config, obj->bank_shear_stress_method, "bank_erosion.bank_shear_stress_method"))			//CORRECTION AUG2009
            CONFIG_ERR("bank_erosion.bank_shear_stress_method");																				//CORRECTION AUG2009
	    if (!configIntLookup(config, obj->node_to_monitor, "bank_erosion.node_to_monitor"))								//CORRECTION AUG2009
            CONFIG_ERR("bank_erosion.node_to_monitor");																						//CORRECTION AUG2009
	    if (!configIntLookup(config, obj->update_width_method, "bank_erosion.update_width_method"))						//CORRECTION AUG2009
            CONFIG_ERR("bank_erosion.update_width_method");																					//CORRECTION AUG2009
	    if (!configIntLookup(config, obj->erosion_distance_method, "bank_erosion.erosion_distance_method"))				//CORRECTION AUG2009
            CONFIG_ERR("bank_erosion.erosion_distance_method");																				//CORRECTION AUG2009
	    if (!configDoubleLookup(config, obj->gap_elongation, "bank_erosion.gap_elongation"))								//CORRECTION AUG2009
            CONFIG_ERR("bank_erosion.gap_elongation");																						//CORRECTION AUG2009
	    if (!configBoolLookup(config, obj->regrid_centerline_nodes, "bank_erosion.regrid_centerline_nodes"))					//ADDITION MAR2010
            CONFIG_ERR("bank_erosion.regrid_centerline_nodes");																				//ADDITION MAR2010
	    if (!configBoolLookup(config, obj->interpolate_after_splines, "bank_erosion.interpolate_after_splines"))				//ADDITION FEB2010
            CONFIG_ERR("bank_erosion.interpolate_after_splines");																				//ADDITION FEB2010
	    if (!configIntLookup(config, obj->interpolate_frequency, "bank_erosion.interpolate_frequency"))					//ADDITION FEB2010
            CONFIG_ERR("bank_erosion.interpolate_frequency");																					//ADDITION FEB2010
	    if (!configDoubleLookup(config, obj->bank_interpolation_tolerance, "bank_erosion.bank_interpolation_tolerance"))		//ADDITION MAR2010
            CONFIG_ERR("bank_erosion.bank_interpolation_tolerance");																			//ADDITION MAR2010
        if (!configStringLookup(config, obj->initial_section_prop_file, "bank_erosion.initial_section_prop_file"))
            CONFIG_ERR("bank_erosion.initial_section_prop_file");
		if (!configBoolLookup(config, obj->floodplain_heterogeneity, "bank_erosion.floodplain_heterogeneity"))	//DM-FEB2011
            CONFIG_ERR("bank_erosion.floodplain_heterogeneity");												//DM-FEB2011
        if (!configStringLookup(config, obj->floodplain_file, "bank_erosion.floodplain_file"))				//DM-FEB2011
            CONFIG_ERR("bank_erosion.floodplain_file");														//DM-FEB2011
        if (!configBoolLookup(config, obj->use_karmor, "bank_erosion.use_karmor"))				//DM-FEB2012
            CONFIG_ERR("bank_erosion.use_karmor");												//DM-FEB2012
        if (!configDoubleLookup(config, obj->tblock, "bank_erosion.tblock"))					//DM-FEB2012
            CONFIG_ERR("bank_erosion.tblock");													//DM-FEB2012	
		if (!configDoubleLookup(config, obj->c_factor_for_karmor, "bank_erosion.c_factor_for_karmor"))		//MMMMTEMP Additional parameter in "prototype.cfg"
            CONFIG_ERR("bank_erosion.c_factor_for_karmor");													//MMMMTEMP Additional parameter in "prototype.cfg"	
		if (!configDoubleLookup(config, obj->d_factor_for_karmor, "bank_erosion.d_factor_for_karmor"))		//MMMMTEMP Additional parameter in "prototype.cfg"
            CONFIG_ERR("bank_erosion.d_factor_for_karmor");													//MMMMTEMP Additional parameter in "prototype.cfg"	
        readBankEvoProps(config, logger, obj);

        return obj;
    }

    void BankErosion_PhysicallyBased::readBankEvoProps(GenericConfigObj &config, RMLogger* logger, BankErosion_PhysicallyBased* obj)
    {
        bool temp = false;
        if (!configBoolLookup(config, temp, "bank_erosion.bank_evolution_properties.use_hydraulic_erosion"))
            CONFIG_ERR("bank_evolution_props.use_hydraulic_erosion");
        obj->evoProps.fluvialerosionswitch = (temp ? 1 : 0);
        if (!configIntLookup(config, obj->evoProps.TypeLawFluvialErosion, "bank_erosion.bank_evolution_properties.type_law_hydraulic_erosion"))
            CONFIG_ERR("bank_evolution_props.type_law_hydraulic_erosion");
        if (!configIntLookup(config, obj->evoProps.CrossSectionDivision, "bank_erosion.bank_evolution_properties.xs_division"))
            CONFIG_ERR("bank_evolution_props.xs_division");
        if (!configDoubleLookup(config, obj->evoProps.grainroughnessthreshold, "bank_erosion.bank_evolution_properties.gr_threshold"))
            CONFIG_ERR("bank_evolution_props.gr_threshold");
        if (!configDoubleLookup(config, obj->evoProps.wpthreshold, "bank_erosion.bank_evolution_properties.wp_threshold"))
            CONFIG_ERR("bank_evolution_props.wp_threshold");
        if (!configDoubleLookup(config, obj->evoProps.MAX_EROSION_DISTANCE, "bank_erosion.bank_evolution_properties.max_erosion_dist"))
            CONFIG_ERR("bank_evolution_props.max_erosion_dist");
        if (!configDoubleLookup(config, obj->evoProps.mindistance, "bank_erosion.bank_evolution_properties.min_distance"))
            CONFIG_ERR("bank_evolution_props.min_distance");
        if (!configBoolLookup(config, temp, "bank_erosion.bank_evolution_properties.use_cantilever_failure"))
            CONFIG_ERR("bank_evolution_props.use_cantilever_failure");
        obj->evoProps.cantileverfailureswitch = (temp ? 1 : 0);
        if (!configDoubleLookup(config, obj->evoProps.undercutthreshold, "bank_erosion.bank_evolution_properties.undercut_threshold"))
            CONFIG_ERR("bank_evolution_props.undercut_threshold");
        if (!configBoolLookup(config, temp, "bank_erosion.bank_evolution_properties.use_planar_failure"))
            CONFIG_ERR("bank_evolution_props.use_planar_failure");
        obj->evoProps.planarfailureswitch = (temp ? 1 : 0);
        if (!configIntLookup(config, obj->evoProps.numberofintersects, "bank_erosion.bank_evolution_properties.num_intersects"))
            CONFIG_ERR("bank_evolution_props.num_intersects");
        if (!configDoubleLookup(config, obj->evoProps.zthreshold, "bank_erosion.bank_evolution_properties.z_threshold"))
            CONFIG_ERR("bank_evolution_props.z_threshold");
        if (!configDoubleLookup(config, obj->evoProps.tensioncrackoption, "bank_erosion.bank_evolution_properties.tension_crack_opt"))
            CONFIG_ERR("bank_evolution_props.tension_crack_opt");
        if (!configIntLookup(config, obj->evoProps.slicemultiplier, "bank_erosion.bank_evolution_properties.slice_multip"))
            CONFIG_ERR("bank_evolution_props.slice_multip");

        if (!configBoolLookup(config, obj->evoProps.bankfailureanalysis[0], "bank_erosion.bank_evolution_properties.positive_pore"))
            CONFIG_ERR("bank_evolution_props.positive_pore");
        if (!configBoolLookup(config, obj->evoProps.bankfailureanalysis[1], "bank_erosion.bank_evolution_properties.negative_pore"))
            CONFIG_ERR("bank_evolution_props.negative_pore");
        if (!configBoolLookup(config, obj->evoProps.bankfailureanalysis[2], "bank_erosion.bank_evolution_properties.hydrost_force"))
            CONFIG_ERR("bank_evolution_props.hydrost_force");

        if (!configDoubleLookup(config, obj->evoProps.accuracy, "bank_erosion.bank_evolution_properties.accuracy"))
            CONFIG_ERR("bank_evolution_props.accuracy");
        if (!configIntLookup(config, obj->evoProps.maxiter, "bank_erosion.bank_evolution_properties.max_iter"))
            CONFIG_ERR("bank_evolution_props.max_iter");
        if (!configIntLookup(config, obj->evoProps.ANALYSISMETHOD, "bank_erosion.bank_evolution_properties.analysis_method"))
            CONFIG_ERR("bank_evolution_props.analysis_method");
        if (!configDoubleLookup(config, obj->evoProps.lambda, "bank_erosion.bank_evolution_properties.lambda"))
            CONFIG_ERR("bank_evolution_props.lambda");
    }

    void BankErosion_PhysicallyBased::InitializeBankErosionObject(RMBankErosionData *&data, RMBankErosion *&be)
    {
	    data = new RMBankErosionData();
	    be = new RMBankErosion();

	    data->MethodBankEvolution = 2;
	    data->MethodPhysicallyBasedBankEvolution = pb_method;				//SEP2009
	    data->MethodShearStress = bank_shear_stress_method;
		data->reduce_stress_bank = reduce_stress_at_bank;//DM-FEB2012
		data->reduction_stress_bank = reduction_stress_at_bank;//DM-FEB2012
	    data->MonitorNode = node_to_monitor;
	    data->MethodUpdateWidth = update_width_method;
	    data->MethodErosionDistance = erosion_distance_method;
	    data->ElongationGap = gap_elongation;
	    data->RegridCenterlineNodes = regrid_centerline_nodes;				// ADDITION MAR2010
	    data->InterpolateAfterSplines = interpolate_after_splines;			// ADDITION FEB2010
	    data->InterpolateFrequency = interpolate_frequency;					// ADDITION FEB2010
	    data->BankInterpolationTolerance = bank_interpolation_tolerance;		// ADDITION MAR2010
	    data->DepositionSwitch = 0;				// Correction MAR2010

        data->fluvialerosionswitch = evoProps.fluvialerosionswitch;
        data->TypeLawFluvialErosion = evoProps.TypeLawFluvialErosion;
        data->CrossSectionDivision = evoProps.CrossSectionDivision;
        data->grainroughnessthreshold = evoProps.grainroughnessthreshold;
        data->wpthreshold = evoProps.wpthreshold;
        data->MAX_EROSION_DISTANCE = evoProps.MAX_EROSION_DISTANCE;
        data->mindistance = evoProps.mindistance;
        data->cantileverfailureswitch = evoProps.cantileverfailureswitch;
        data->undercutthreshold = evoProps.undercutthreshold;
        data->planarfailureswitch = evoProps.planarfailureswitch;
        data->numberofintersects = evoProps.numberofintersects;
        data->zthreshold = evoProps.zthreshold;
        data->tensioncrackoption = evoProps.tensioncrackoption;
        data->slicemultiplier = evoProps.slicemultiplier;
        data->bankfailureanalysis[0] = evoProps.bankfailureanalysis[0];
        data->bankfailureanalysis[1] = evoProps.bankfailureanalysis[1];
        data->bankfailureanalysis[2] = evoProps.bankfailureanalysis[2];
        data->accuracy = evoProps.accuracy;
        data->maxiter = evoProps.maxiter;
        data->ANALYSISMETHOD = evoProps.ANALYSISMETHOD;
        data->lambda = evoProps.lambda;
		data->FloodplainHeterogeneity = floodplain_heterogeneity;//DM-FEB2011
		if(data->FloodplainHeterogeneity == true)data->floodplain_file = floodplain_file; //DM-FEB2011
		data->tblock = tblock;				//DM-FEB2012
		data->use_karmor = use_karmor;		//DM-FEB2012
		data->c_factor_for_karmor = c_factor_for_karmor;		//MMMMTEMP Additional parameter in "prototype.cfg"
		data->d_factor_for_karmor = d_factor_for_karmor;		//MMMMTEMP Additional parameter in "prototype.cfg"
    }
}

