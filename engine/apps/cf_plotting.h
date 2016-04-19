#ifndef __CF_PLOTTING_H_____________________20101004091313__
#define __CF_PLOTTING_H_____________________20101004091313__

#include "config_file_obj.h"
#include "cf_migration.h"
#include "../rmplotting/rmplotting.h"
#include <string>


namespace ConfigFile
{
    struct Plotting
    {
        static Plotting* Create(GenericConfigObj& config, RMLogger* logger);		//DM-FEB2011	
        void InitializePlottingObject(const Migration* mig, RMPlotting*& plotter);

	    std::string output_1D;
	    std::string output_uv2D;
	    std::string output_hde2D;
	    std::string output_U1V12D;
	    std::string output_H1D1E12D;
	    std::string output_UUVV2D;
	    std::string output_HHDDEE2D;
	    std::string output_VELandTAU2D;
	    std::string output_Sections2D;
	    std::string output_Hydro3D;
	    std::string output_BedBanks3D;
	    std::string output_Floodplain; //DM-FEB2011
	    //std::string output_ReworkedAreas;						//DM-FEB2011 Commented out
	    //std::string output_PlanarFailure;						//DM-FEB2011 Commented out
        //std::string output_HeterogeneityAtCenterline;			//DM-FEB2011 Commented out
		std::string output_KarmorFile;
		std::string output_IndicesFile;
		std::string output_KarmorVisualizedFile;
    };
}


#endif//__CF_PLOTTING_H_____________________20101004091313__
