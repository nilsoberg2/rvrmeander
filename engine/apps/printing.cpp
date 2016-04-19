#pragma warning(once : 4290)

#include "rvrmeander.h"
#include "../rmfilter/rmfilter_savitzkygolay.h"
#include "../rmhydrodynamics/rmhydrodynamics_ikeda1981.h"//DM-FEB2012

void RVRMeander::PrintInfo_Migration(RMMigrationData* data, RMFilter* filter)
{
	logmsg("****************************************************************** \n");
	logmsg("MIGRATION \n");


	//logmsg("	Transverse slope coeff = %lf \n", data->ALF);							// Commented out since it is internally calculated in the code for each time step
	//logmsg("	Velocity, straight channel (m/s) = %lf \n", data->U0);					// Commented out since it is internally calculated in the code for each time step		
	//logmsg("	Ratio half width to depth = %lf \n", data->BE);							// Commented out since it is internally calculated in the code for each time step    
	//logmsg("	Friction coefficient, straight channel = %lf \n", data->CF_0);			// Commented out since it is internally calculated in the code for each time step
	//logmsg("	Froude number squared, straight channel = %lf \n", data->F_02);			// Commented out since it is internally calculated in the code for each time step

	logmsg("Duration (years) = %lf \n", m_configData.Migration->duration);
	logmsg("Number of iterations = %d \n", m_configData.Migration->num_iterations);
	logmsg("Time step (days) = %lf (calculated) \n", m_configData.Migration->DeltaT);

	logmsg("Upstream dimensionless curvature = %lf\n",m_configData.Migration->init_curvature);//DM-FEB2012

	logmsg("Number of iterations to filter = %d \n", filter->GetFilterFrequency());

	logmsg("Switch for initial splines application = %d \n",m_configData.Migration->splines_initially);//DM-FEB2012

	logmsg("Number of iterations to plot = %d \n",m_configData.Migration->plot_iterations);	

	logmsg("Switch for the migration at upstream end = %d \n", data->MigrateUpstreamNode);
    logmsg("Switch for the migration at downstream end = %d \n", data->MigrateDownstreamNode ? 1 : 0);
	
	logmsg("Threshold for cutoff (number of half widths) = %lf \n", data->CutoffThreshold);	

	logmsg("Switch for using intermittency factor = %d \n", data->apply_interm);	//DM-FEB2012
	logmsg("Intermittency factor = %lf \n", data->interm_factor);					//DM-FEB2012

	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo_Channel(RMChannel& channel)
{
	logmsg("****************************************************************** \n");
	logmsg("CHANNEL PARAMETERS \n");
    logmsg("Flow rate (m3/s) = %lf \n", channel.FLOW);
    //logmsg("Depth, straight channel (m) = %lf \n", channel.D0);		//CORRECTION AUG2009			
	logmsg("(Initial) width (m) = %lf \n", channel.WIDTH);
	logmsg("Sediment size (m) = %lf \n", channel.SEDSIZE);	
	logmsg("Use valley centerline = %d \n", channel.use_valley_centerl);								//DM-FEB2012
    logmsg("Water density (kg/m3) = %lf \n", channel.WaterDensity);
	logmsg("Valley slope = %lf \n", channel.ValleySlope);
	logmsg("Mean bed elevation at the upstream end (m) = %lf \n", channel.USBed);
	logmsg("Number of nodes (streamwise) = %d \n",channel.centerline.length);
	logmsg("Number of nodes (transverse) = %d \n",channel.nnodes);									//DM-FEB2012
	logmsg("Mesh generation method = %d \n",channel.mesh_gen_method);								//DM-FEB2012
	logmsg("Threshold for centerline-node regridding = %lf\n", channel.threshold_regrid);			//DM-FEB2012
	logmsg("Manning's coefficient = %lf\n", channel.ManningCoefficient);							//DM-FEB2012				
	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo_Smoothing(RMFilter* filter)
{	
	logmsg("****************************************************************** \n");
	logmsg("SMOOTHING \n");

	logmsg("Smoothing order = %d\n", m_configData.Smoothing->order);
	
	logmsg("Switch for curvature filtering = %d \n", m_configData.Smoothing->curvature_filter);

	logmsg("Savitzky-Golay filter, parameters: \n");
	//RMFilter_SavitzkyGolay* sgf = (RMFilter_SavitzkyGolay*)filter;
	//int nL, nR;													//CORRECTION FEB2010
	//sgf->GetWindowSize(nL, nR);									//CORRECTION FEB2010
	logmsg("	Order of the polynomial = %d \n", filter->m_polynomial_order);
	logmsg("	Number of nodes included in the filter window = %d \n", filter->m_window_nodes);
    logmsg("	Number of filter applications (each time) = %d \n", filter->GetNumIterations());//DM-FEB2012
	//RMSeries coeffs = sgf->GetCoefficients();						//CORRECTION FEB2010
	//logmsg("	Coefficients = ");									//CORRECTION FEB2010
	//for (int i = 0; i < coeffs.length; i++)						//CORRECTION FEB2010
	//	logmsg(" %lf", coeffs(i));									//CORRECTION FEB2010
	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo_Prep(RMMigrationData* migData)
{
	logmsg("****************************************************************** \n");
	logmsg("PREPROCESSING \n");
	logmsg("Method for computing curvature = %d \n", migData->curvMethod.CurvatureMethod);
    logmsg("Method for computing curvature at upstream end = %d \n", migData->curvMethod.UpstreamCurvatureMethod);
	logmsg("Method for computing curvature at downstream end = %d \n", migData->curvMethod.DownstreamCurvatureMethod);
	logmsg("Switch for curvature capping = %d \n", migData->curvMethod.CurvatureCapping);//DM-FEB2012
	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo_Hydrodynamics(RMHydrodynamicsData* hData, RMMigrationData* migData)
{
	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)hData;//DM-FEB2012
	logmsg("****************************************************************** \n");
	logmsg("HYDRODYNAMICS \n");	
	logmsg("Method for computing hydrodynamics at downstream end = %d \n", hData->HydrodynamicsLastNodeMethod );
	logmsg("Manning's roughness coefficient = %lf \n", migData->channel.ManningCoefficient );
	logmsg("Dimensionless perturbation velocity at the upstream end = %lf \n",data->UB_0);//DM-FEB2012
	logmsg("Scour factor = %lf \n",data->ALF);//DM-FEB2012
	logmsg("Switch for secondary flow correction = %d \n",data->sec_flow_corr);														//DM-FEB2012
	logmsg("Switch for using Manning's coefficient for computing friction coefficient = %d \n",data->use_mann_for_fric_coeff);		//DM-FEB2012
	logmsg("Switch for amplification of the friction coefficient = %d \n",data->amplify_fric_coeff);								//DM-FEB2012
	logmsg("Amplification factor for friction coefficient = %lf \n",data->ampl_fric_coeff);											//DM-FEB2012
	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo_BankErosion(RMBankErosionData* beData)
{
	logmsg("****************************************************************** \n");
	logmsg("BANK EROSION \n");
	if(beData->MethodBankEvolution == 1)
		{
			logmsg("Method for computing bank evolution = migration coefficient \n");						
		}
	else if(beData->MethodBankEvolution == 2)
		{
			logmsg("Method for computing bank evolution = physically-based \n");	
		}
	logmsg("Switch for floodplain heterogeneity = %d \n", beData->FloodplainHeterogeneity);			//DM-FEB2012
	if(beData->MethodBankEvolution == 1)																					
		{
			logmsg("Migration coefficient, parameters: \n");
			logmsg("	Bank erosion coefficient = %e \n", beData->EC);
		}
	else if (beData->MethodBankEvolution == 2)																			
		{
			logmsg("Physically-based, parameters: \n");
			logmsg("	Type of physically-based migration = %d \n", beData->MethodPhysicallyBasedBankEvolution);			//SEP2009
			logmsg("	Switch for reduction of shear stress on bank = %d \n", beData->reduce_stress_bank);//DM-FEB2012
			logmsg("	Reduction factor for the shear stress on bank = %lf \n", beData->reduction_stress_bank);//DM-FEB2012
			logmsg("	Method for computing shear stress on bank = %d \n", beData->MethodShearStress);	
			logmsg("	Node to monitor for bank evolution = %d \n", beData->MonitorNode);		
			logmsg("	Method to update the width = %d \n", beData->MethodUpdateWidth);		
			logmsg("	Erosion distance method = %d \n", beData->MethodErosionDistance);		
			logmsg("	Tolerance for bank elongation = %lf \n", beData->ElongationGap);
			logmsg("	Switch for regridding centerline nodes = %d \n", beData->RegridCenterlineNodes);//DM-FEB2012
			logmsg("	Switch for bank interpolating after splines application = %d \n", beData->InterpolateAfterSplines);
			logmsg("	Iterations interval for interpolating after splines application = %d \n", beData->InterpolateFrequency);	
			logmsg("	Bank interpolation tolerance (m) = %lf \n", beData->BankInterpolationTolerance); //DM-FEB2012
			logmsg("	Switch for fluvial erosion = %d \n", beData->fluvialerosionswitch);	
			logmsg("		Type of fluvial erosion law = %d \n", beData->TypeLawFluvialErosion);																//DM-FEB2012
			logmsg("		Cross section division method = %d \n", beData->CrossSectionDivision);																//DM-FEB2012
			logmsg("		Grain roughness threshold = %lf \n", beData->grainroughnessthreshold);																//DM-FEB2012
			logmsg("		Wetted perimeter threshold (m) = %lf \n", beData->wpthreshold);																		//DM-FEB2012
			logmsg("		Maximum erosion distance (m) = %lf \n", beData->MAX_EROSION_DISTANCE);																//DM-FEB2012
			logmsg("		Minimum distance to add node (m) = %lf \n", beData->mindistance);																	//DM-FEB2012
			logmsg("	Switch for cantilever failure = %d \n", beData->cantileverfailureswitch);
			logmsg("		Undercut threshold (m) = %lf \n", beData->undercutthreshold);																		//DM-FEB2012
			logmsg("	Switch for planar failure = %d \n", beData->planarfailureswitch);
			logmsg("		Number of intersects = %d \n", beData->numberofintersects);																			//DM-FEB2012
			logmsg("		Minimum distance from failing block toe node to an existing bank node (m) = %lf \n", beData->zthreshold);							//DM-FEB2012
			logmsg("		Tension crack option (m) = %lf \n", beData->tensioncrackoption);																	//DM-FEB2012
			logmsg("		Slice multiplier = %d \n", beData->slicemultiplier);																				//DM-FEB2012
			logmsg("		Bank failure analysis, switch 1 (positive pore-water pressure) = %d \n", beData->bankfailureanalysis[0]);							//DM-FEB2012
			logmsg("		Bank failure analysis, switch 2 (negative pore-water pressure) = %d \n", beData->bankfailureanalysis[1]);							//DM-FEB2012
			logmsg("		Bank failure analysis, switch 3 (hydrostatic force) = %d \n", beData->bankfailureanalysis[2]);										//DM-FEB2012
			logmsg("		Accuracy = %lf \n", beData->accuracy);																								//DM-FEB2012
			logmsg("		Maximum number of iterations in recursive research of the minimum FS = %d \n", beData->maxiter);									//DM-FEB2012
			logmsg("		Analysis method for the computation of the factor of safety = %d \n", beData->ANALYSISMETHOD);										//DM-FEB2012
			logmsg("		Lambda parameter = %lf \n", beData->lambda);																						//DM-FEB2012
			logmsg("	Switch for using dimensionless slump block armoring factor = %d \n", beData->use_karmor);												//DM-FEB2012
			logmsg("	Characteristic period of existence for a slump block = %lf \n", beData->tblock);														//DM-FEB2012
			logmsg("	c factor for dimensionless slump block armoring factor = %lf \n", beData->c_factor_for_karmor);											//MMMMTEMP Additional parameter in "prototype.cfg"
			logmsg("	d factor for dimensionless slump block armoring factor = %lf \n", beData->d_factor_for_karmor);											//MMMMTEMP Additional parameter in "prototype.cfg"
		}
	logmsg("****************************************************************** \n\n");
}


void RVRMeander::PrintInfo(RMMigrationData* migData,
			               RMFilter* filter,
			               RMPrepData* pData,
			               RMHydrodynamicsData* hData,
			               RMBankErosionData* beData)
{
	logmsg("\n****************************************************************** \n");
	logmsg("RVR Meander 2011 \n");
	logmsg("****************************************************************** \n\n");
	
	PrintInfo_Channel(migData->channel);									//CORRECTION AUG2009

	PrintInfo_Prep(migData);												//CORRECTION AUG2009

	PrintInfo_Hydrodynamics(hData, migData);					//CORRECTION AUG2009

	PrintInfo_BankErosion(beData);											//CORRECTION AUG2009

	PrintInfo_Migration(migData, filter);						//CORRECTION AUG2009

	PrintInfo_Smoothing(filter);								//CORRECTION AUG2009
}
