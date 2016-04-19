#pragma warning(disable : 4290)

#include "config_file_obj.h"
#include "config_file_util.h"
#include "cf_plotting.h"
#include "error.h"    //DM-FEB2011


namespace ConfigFile
{
    Plotting* Plotting::Create(GenericConfigObj &config, RMLogger *logger)		//DM-FEB2011												
    {
        Plotting* out = new Plotting();

	    //configStringLookup(config, out->output_1D, "plotting.output_1D");															//DM-FEB2011
	    //configStringLookup(config, out->output_uv2D, "plotting.output_uv2D");														//DM-FEB2011
	    //configStringLookup(config, out->output_hde2D, "plotting.output_hde2D");													//DM-FEB2011
	    //configStringLookup(config, out->output_U1V12D, "plotting.output_U1V12D");													//DM-FEB2011
	    //configStringLookup(config, out->output_H1D1E12D, "plotting.output_H1D1E12D");												//DM-FEB2011
	    //configStringLookup(config, out->output_UUVV2D, "plotting.output_UUVV2D");													//DM-FEB2011
	    //configStringLookup(config, out->output_HHDDEE2D, "plotting.output_HHDDEE2D");												//DM-FEB2011
	    //configStringLookup(config, out->output_VELandTAU2D, "plotting.output_VELandTAU2D");										//DM-FEB2011
	    //configStringLookup(config, out->output_Sections2D, "plotting.output_Sections2D");											//DM-FEB2011
	    //configStringLookup(config, out->output_Hydro3D, "plotting.output_Hydro3D");												//DM-FEB2011
	    //configStringLookup(config, out->output_BedBanks3D, "plotting.output_BedBanks3D");											//DM-FEB2011
	    //configStringLookup(config, out->output_ReworkedAreas, "plotting.output_reworkedareas");									//DM-FEB2011
	    //configStringLookup(config, out->output_HeterogeneityAtCenterline, "plotting.output_HeterogeneityAtCenterline");			//DM-FEB2011

	    if (!configStringLookup(config, out->output_1D, "plotting.output_1D"))														//DM-FEB2011
            CONFIG_ERR("plotting.output_1D");																						//DM-FEB2011
	    if (!configStringLookup(config, out->output_uv2D, "plotting.output_uv2D"))													//DM-FEB2011
            CONFIG_ERR("plotting.output_uv2D");																						//DM-FEB2011
	    if (!configStringLookup(config, out->output_hde2D, "plotting.output_hde2D"))												//DM-FEB2011
            CONFIG_ERR("plotting.output_hde2D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_U1V12D, "plotting.output_U1V12D"))												//DM-FEB2011
            CONFIG_ERR("plotting.output_U1V12D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_H1D1E12D, "plotting.output_H1D1E12D"))											//DM-FEB2011
            CONFIG_ERR("plotting.output_H1D1E12D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_UUVV2D, "plotting.output_UUVV2D"))												//DM-FEB2011
            CONFIG_ERR("plotting.output_UUVV2D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_HHDDEE2D, "plotting.output_HHDDEE2D"))											//DM-FEB2011
            CONFIG_ERR("plotting.output_HHDDEE2D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_VELandTAU2D, "plotting.output_VELandTAU2D"))									//DM-FEB2011
            CONFIG_ERR("plotting.output_VELandTAU2D");																				//DM-FEB2011
	    if (!configStringLookup(config, out->output_Sections2D, "plotting.output_Sections2D"))										//DM-FEB2011
            CONFIG_ERR("plotting.output_Sections2D");																				//DM-FEB2011
	    if (!configStringLookup(config, out->output_Hydro3D, "plotting.output_Hydro3D"))											//DM-FEB2011
            CONFIG_ERR("plotting.output_Hydro3D");																					//DM-FEB2011
	    if (!configStringLookup(config, out->output_BedBanks3D, "plotting.output_BedBanks3D"))										//DM-FEB2011
            CONFIG_ERR("plotting.output_BedBanks3D");																				//DM-FEB2011
	    if (!configStringLookup(config, out->output_Floodplain, "plotting.output_Floodplain"))										//DM-FEB2011
            CONFIG_ERR("plotting.output_Floodplain");																				//DM-FEB2011
	    //if (!configStringLookup(config, out->output_HeterogeneityAtCenterline, "plotting.output_HeterogeneityAtCenterline"))		//DM-FEB2011 Commented out
        //    CONFIG_ERR("plotting.output_HeterogeneityAtCenterline");	
		if (!configStringLookup(config, out->output_KarmorFile, "plotting.output_KarmorFile"))										//DM-FEB2011
			CONFIG_ERR("plotting.output_KarmorFile");//DM-FEB2011 Commented out		
		if (!configStringLookup(config, out->output_IndicesFile, "plotting.output_IndicesFile"))										//DM-FEB2011
			CONFIG_ERR("plotting.output_IndicesFile");//DM-FEB2011 Commented out	
		if (!configStringLookup(config, out->output_KarmorVisualizedFile, "plotting.output_KarmorVisualizedFile"))										//DM-FEB2011
			CONFIG_ERR("plotting.output_KarmorVisualizedFile");//DM-FEB2011 Commented out	

        return out;
    }

    void Plotting::InitializePlottingObject(const Migration* mig, RMPlotting*& plotter)
    {
	    plotter = new RMPlotting();
	    plotter->SetFrequency(mig->plot_iterations);
	    plotter->SetTecplotOutput_1D(output_1D.c_str());
	    plotter->SetTecplotOutput_uv2D(output_uv2D.c_str());
	    plotter->SetTecplotOutput_hde2D(output_hde2D.c_str());
	    plotter->SetTecplotOutput_U1V12D(output_U1V12D.c_str());
	    plotter->SetTecplotOutput_H1D1E12D(output_H1D1E12D.c_str());
	    plotter->SetTecplotOutput_UUVV2D(output_UUVV2D.c_str());
	    plotter->SetTecplotOutput_HHDDEE2D(output_HHDDEE2D.c_str());
	    plotter->SetTecplotOutput_VELandTAU2D(output_VELandTAU2D.c_str());
	    plotter->SetTecplotOutput_Sections2D(output_Sections2D.c_str());
	    plotter->SetTecplotOutput_Hydro3D(output_Hydro3D.c_str());
	    plotter->SetTecplotOutput_BedBanks3D(output_BedBanks3D.c_str());
	    //plotter->SetOutput_ReworkedAreas(output_ReworkedAreas.c_str());								//DM-FEB2011 Commented out
        //plotter->SetOutput_HeterogeneityAtCenterline(output_HeterogeneityAtCenterline.c_str());		//DM-FEB2011 Commented out
		plotter->SetTecplotOutput_Floodplain(output_Floodplain.c_str());//DM-FEB2011
		plotter->SetKarmorFile(output_KarmorFile.c_str()); //DM-FEB2012
		plotter->SetIndicesFile(output_IndicesFile.c_str()); //MMMMTEMP Indices file
		plotter->SetKarmorVisualizedFile(output_KarmorVisualizedFile.c_str()); //DM-FEB2013
    }
}
