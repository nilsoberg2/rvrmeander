#ifndef __BANKEROSION_H_____________________20101001154747__
#define __BANKEROSION_H_____________________20101001154747__

#include "config_file_obj.h"
#include "../rmio/rmlog.h"
#include "config_file_util.h"
#include "../rmbankerosion/rmbankerosion.h"


namespace ConfigFile
{
    struct BankErosion
    {
        static BankErosion* Create(GenericConfigObj& config, RMLogger* logger);
        virtual void InitializeBankErosionObject(RMBankErosionData*& data, RMBankErosion*& be) = 0;

        std::string method;
    };


    struct BankErosion_MigrationCoefficient : public BankErosion
    {
        static BankErosion_MigrationCoefficient* Create(GenericConfigObj& config, RMLogger* logger);
        virtual void InitializeBankErosionObject(RMBankErosionData*& data, RMBankErosion*& be);

        double erosion_coeff;
		bool floodplain_heterogeneity; //DM-FEB2011

		std::string floodplain_file; //DM-FEB2011
    };


    struct BankEvolutionProps
    {
        int fluvialerosionswitch;
        int TypeLawFluvialErosion;
        int CrossSectionDivision;
        double grainroughnessthreshold;
        double wpthreshold;
        double MAX_EROSION_DISTANCE;
        double mindistance;
        int cantileverfailureswitch;
        double undercutthreshold;
        int planarfailureswitch;
        int numberofintersects;
        double zthreshold;
        double tensioncrackoption;
        int slicemultiplier;
        bool bankfailureanalysis[3];
        double accuracy;
        int maxiter;
        int ANALYSISMETHOD;
        double lambda;
    };


    struct BankErosion_PhysicallyBased : public BankErosion
    {
        static BankErosion_PhysicallyBased* Create(GenericConfigObj& config, RMLogger* logger);
        virtual void InitializeBankErosionObject(RMBankErosionData*& data, RMBankErosion*& be);

	    int pb_method;																//SEP2009
		bool reduce_stress_at_bank;													//DM-FEB2012
		double reduction_stress_at_bank;											//DM-FEB2012
	    int bank_shear_stress_method;											//CORRECTION AUG2009
	    int node_to_monitor;														//CORRECTION AUG2009
	    int update_width_method;													//CORRECTION AUG2009
	    int erosion_distance_method;												//CORRECTION AUG2009
	    double gap_elongation;														//CORRECTION AUG2009
	    bool regrid_centerline_nodes;												//ADDITION MAR2010
	    bool interpolate_after_splines;												//ADDITION FEB2010
	    int interpolate_frequency;													//ADDITION FEB2010
	    double bank_interpolation_tolerance;											//ADDITION MAR2010
	    bool deposition_switch;													//CORRECTION AUG2009
        bool floodplain_heterogeneity; //DM-FEB2011
        BankEvolutionProps evoProps;
        static void readBankEvoProps(GenericConfigObj &config, RMLogger* logger, BankErosion_PhysicallyBased* obj);

        std::string initial_section_prop_file;

		std::string floodplain_file; //DM-FEB2011

        bool use_karmor;												//DM-FEB2012
		double tblock;													//DM-FEB2012
		double c_factor_for_karmor;										//MMMMTEMP Additional parameter in "prototype.cfg"
		double d_factor_for_karmor;										//MMMMTEMP Additional parameter in "prototype.cfg"
    };
}


#endif//__BANKEROSION_H_____________________20101001154747__
