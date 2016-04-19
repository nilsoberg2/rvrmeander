#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(once : 4290)

#include <cstdio>
#include <cstdarg>
#include <string>
#include <cmath> //DM-FEB2012 (in order to use fabs)
#include "rvrmeander.h"
#include "../rmdebug/debug_printing.h"
#include "../rmio/rmfile.h"
#include "../timer.h"
#include "../rmutil/utils.h"


RVRMeander::RVRMeander()
: RMLogger()
{
	m_logger = (RMLogger*)this;
}


RVRMeander::~RVRMeander()
{
	cleanup();
}


void RVRMeander::cleanup()
{
}


int RVRMeander::RunModel()
{
	Initialize();
	int numIter = GetNumIters();
	for (int j = 1; j <= numIter; j++)
	{
		ExecuteStep(j);
	}
	return Finish();
}


int RVRMeander::Initialize()
{
	using namespace std;

	int result;

	/* DECLARE POINTERS TO OBJECTS */
	rd.migObj = NULL;
	rd.migData = NULL;
	rd.filter = NULL;
	rd.prep = NULL;
	rd.prepData = NULL;
	rd.hydrodynamics = NULL;
	rd.hydrodynamicsData = NULL;
	rd.bankErosion = NULL;
	rd.bankErosionData = NULL;
	rd.plotter = NULL;

	// Resolve the path of the input and output files.
	string configPath = dirPath(m_configFile);

	/* CREATE THE VARIOUS OBJECTS */


	m_configData.Migration->InitializeMigrationObject(rd.migData, rd.migObj);
	rd.migObj->SetLogger(m_logger);
	m_configData.Channel->InitializeChannelObject(rd.migData->channel);

	// Migration object and data
	m_configData.Channel->centerline = resolvePath(configPath, m_configData.Channel->centerline);
	m_configData.Channel->valley_centerline = resolvePath(configPath, m_configData.Channel->valley_centerline);

	// Load channel centerline series from the input file
	result = LoadSeries(m_configData.Channel->centerline.c_str(), FILE_TEXT, rd.migData->channel.centerline);
	if (result < 0)
	{
		logmsg("[ERROR]  Loading input file '%s': err# %d\n", m_configData.Channel->centerline.c_str(), result);
		exit(1);
	}

	// Load valley "centerline" series from the input file
	result = LoadSeries(m_configData.Channel->valley_centerline.c_str(), FILE_TEXT, rd.migData->channel.valley_centerline);
	if (result < 0)
	{
		logmsg("[ERROR]  Loading input file '%s': err# %d\n", m_configData.Channel->valley_centerline.c_str(), result);
		exit(1);
	}		

	// Get the sizes of the arrays necessary
	rd.ARRAY_LENGTH = rd.migData->channel.centerline.length;
	rd.ARRAY_WIDTH = rd.migData->channel.nnodes;

	// Initialize the migration data object
	rd.migData->Init(rd.ARRAY_LENGTH, rd.ARRAY_WIDTH);

	// Create the filter object
	rd.filter = m_configData.Smoothing->CreateFilter(m_configData.Migration);

	// Preprocessor
	m_configData.Preprocessing->InitializePreprocessingObject(m_configData.Migration, m_configData.Smoothing, rd.migData->curvMethod, rd.prepData, rd.prep);
	rd.prepData->Init(rd.ARRAY_LENGTH, rd.ARRAY_WIDTH);
	rd.prep->SetLogger(m_logger);

	// Hydrodynamics
	m_configData.Hydrodynamics->InitializeHydrodynamicsObject(rd.migData->channel, m_configData.Migration, rd.hydrodynamicsData, rd.hydrodynamics);
	rd.hydrodynamicsData->Init(rd.ARRAY_LENGTH, rd.ARRAY_WIDTH);
	rd.hydrodynamics->SetLogger(m_logger);

	// Bank erosion object and data
	m_configData.BankErosion->InitializeBankErosionObject(rd.bankErosionData, rd.bankErosion);
	rd.bankErosionData->Init(rd.ARRAY_LENGTH, rd.ARRAY_WIDTH);
	rd.bankErosionData->InitBankProperties(rd.ARRAY_LENGTH);	
	rd.bankErosion->SetLogger(m_logger);
	rd.bankErosionData->floodplain_file = resolvePath(configPath, rd.bankErosionData->floodplain_file);

	m_configData.Plotting->InitializePlottingObject(m_configData.Migration, rd.plotter);
	rd.plotter->SetLogger(m_logger);

	//TODO: move to be initialized within the creation of bankErosionData
	// Initialize the station of the centerline at the monitor node
	if (rd.bankErosionData->MethodBankEvolution == 2)			
	{	
		if(rd.bankErosionData->MonitorNode > rd.migData->channel.centerline.length-1)								//ADDITION FEB2010
		{																								//ADDITION FEB2010
			rd.bankErosionData->MonitorNode = rd.migData->channel.centerline.length-1;						//ADDITION FEB2010
		}																								//ADDITION FEB2010
		//bankErosionData->StationCenterline[bankErosionData->MonitorNode] = 0.0;	//DM-FEB2011 COMMENTED OUT
		for(int i = 0; i <= rd.migData->channel.centerline.length-1; i++)				//DM-FEB2011 
		{																		//DM-FEB2011 
			rd.bankErosionData->StationCenterline[i] = 0.0;						//DM-FEB2011 
		}																		//DM-FEB2011
	}

	//TODO: move to be initialized within the creation of bankErosionData
	// Initialize bank reworked areas and volumes 
	if (rd.bankErosionData->MethodBankEvolution == 2)			
	{	
		for (int i = 0; i <= rd.ARRAY_LENGTH-1; i++)
		{
			rd.bankErosionData->ReworkedAreaLeft[i] = 0.0;
			rd.bankErosionData->ReworkedAreaRight[i] = 0.0;
		}
		rd.bankErosionData->TotalErodedVolume = 0.0;
	}

	//TODO: move to be initialized within the creation of bankErosionData
	// Read simulation properties and initial section properties
	if (rd.bankErosionData->MethodBankEvolution == 2)			
	{
		// Commented out 2/16/2011 by Nils because this is done in the configuration
		// file now instead of a separate file.
		//rd.bankErosion->AssignBankEvolutionProperties(configPath.c_str(), rd.bankErosionData);			
		rd.bankErosion->AssignInitialSectionProperties(
			resolvePath(configPath, ((ConfigFile::BankErosion_PhysicallyBased*)(m_configData.BankErosion))->initial_section_prop_file).c_str(),
			rd.bankErosionData);
	}


	//TODO: move to be initialized within the creation of bankErosionData
	if(rd.bankErosionData->FloodplainHeterogeneity)								//DM-FEB2011
		{																		//DM-FEB2011
			//rd.bankErosion->ReadHeterogeneityGrid(rd.bankErosionData);				//DM-FEB2011	
			//rd.plotter->TecplotHeterogeneityGrid(rd.bankErosionData);					//DM-FEB2011
			rd.bankErosion->ReadHeterogeneityGridWithLayers(rd.bankErosionData);	//DM-FEB2011
			rd.plotter->TecplotHeterogeneityGridWithLayers(rd.bankErosionData);		//DM-FEB2011
		}																		//DM-FEB2011

	//TODO: move to be initialized within the creation of bankErosionData
	// Calculate (optionally) the initial width
	if (rd.bankErosionData->MethodBankEvolution == 2 && rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 1 &&(rd.bankErosionData->MethodUpdateWidth == 3 || rd.bankErosionData->MethodUpdateWidth == 4)) //DM-FEB2011
	{
		rd.migObj->CalculateInitialWidth(rd.bankErosionData, rd.migData);
	}

	// The centerline coordinates are made dimensionless
	for (int i = 0; i<= rd.migData->channel.centerline.length-1; i++)
	{
		rd.migData->channel.centerline(i,0) = rd.migData->channel.centerline(i,0)/(rd.migData->channel.WIDTH/2);
		rd.migData->channel.centerline(i,1) = rd.migData->channel.centerline(i,1)/(rd.migData->channel.WIDTH/2);
	}

	/***** START OF DATA PROCESSING AND TIME LOOP ******************/

	PrintInfo(rd.migData, rd.filter, rd.prepData, rd.hydrodynamicsData, rd.bankErosionData);

	//DEBUG_PRINT3_ARRAY2(rd.migData->channel.centerline.length-1, xyTemp, rd.migData->channel.centerline);		//CORRECTION AUG2009

	CREATE_TIMER(timer);

	rd.ComputeIntervals_time = 0.0;
	rd.ComputeTheta_time = 0.0;
	rd.ComputeCurvature_time = 0.0;
	rd.ComputeCurvatureGradient_time = 0.0;
	rd.Preprocess_time = 0.0;
	rd.ComputeDimensionlessPerturbationUVHDE_time = 0.0;
	rd.ComputeDimensionedPerturbationUVHDE_time = 0.0;
	rd.ComputeDimensionedFlowField_time = 0.0;
	rd.ComputeDimensionlessBankErosion_time = 0.0;
	rd.Migrate_time = 0.0;
	rd.Smoothing_time = 0.0;
	rd.TotalTime = 0.0;

	START_TIMER(rd.totalTimer);

	//TODO: move to be initialized within the creation of bankErosionData
	// Apply the initial splines if requested				//CORRECTION FEB2010
	if (m_configData.Migration->splines_initially)
	{
		logmsg("Initial splines \n");
		rd.prep->SplinesMCA(rd.migData, rd.ARRAY_WIDTH);
	}

	//DEBUG_PRINT3_ARRAY2(rd.migData->channel.centerline.length-1, xyFilteredTemp, rd.migData->channel.centerline);		//CORRECTION AUG2009

	rd.streamwiseLength = 0.0;

	return 0;
}


int RVRMeander::GetNumIters()
{
	return m_configData.Migration->num_iterations;
}


int RVRMeander::ExecuteStep(int j)
{
	int result;

	CREATE_TIMER(timer);

	logmsg("*****************************************************************************\n");
	logmsg("Iteration %d of %d (t = %lf of %lf years) \n", j, m_configData.Migration->num_iterations, j*m_configData.Migration->DeltaT/365, m_configData.Migration->num_iterations*m_configData.Migration->DeltaT/365);
	logmsg("*****************************************************************************\n");
	printf("*********************************************************************\n");		//DM-FEB2012 
	printf("Iteration %d (%lf years)\n", j,j*m_configData.Migration->DeltaT/365);			//DM-FEB2012
	printf("No Nodes %d \n", rd.migData->channel.centerline.length);			//DM-FEB2012

	/***** VARYING DISCHARGE ***************************************/
	// Include this line in the code if you want to use stepwise hydrograph instead of constant discharge 
	//rd.hydrodynamics->InputHydrograph(j,rd.migData->channel.FLOW,rd.migData->interm_factor);//DM-FEB2012

	/***** OSCILLATING BOUNDARY CONDITION  ***************************************/
	// Include this line in the code if you want to use an oscillating boundary condition at the upstream end for dimensionless streamwise velocity perturbation 
	//rd.hydrodynamics->OscillatingUSBoundaryCondition(j, rd.hydrodynamicsData, rd.prepData); //DM-FEB2012

	/***** PREPROCESSING ***************************************/

	// Compute intervals, the sSum, and the coords (coord-s) array
	START_TIMER(timer);
	rd.prep->ComputeIntervals(rd.migData, rd.prepData);			//CORRECTION AUG2009
	rd.ComputeIntervals_time += STOP_TIMER(timer);

	//TODO: BE restructure
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////ADDITION FEB2010//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Regridding for "migration coefficient" approach
	if(rd.bankErosionData->MethodBankEvolution == 1)							
	{																
		if(rd.prepData->MaxInterval/(rd.migData->channel.WIDTH/2) > rd.migData->channel.threshold_regrid)			//DM-FEB2011																
		{	
			rd.prep->RegridMCA(rd.migData, rd.prepData); 
			if(rd.bankErosionData->FloodplainHeterogeneity)												//DM-FEB2011
			{																						//DM-FEB2011
				rd.bankErosionData->FACTRnu.init(rd.migData->channel.centerline.length);					//DM-FEB2011
				rd.bankErosionData->FACTRnu_left.init(rd.migData->channel.centerline.length);				//DM-FEB2011
				rd.bankErosionData->FACTRnu_right.init(rd.migData->channel.centerline.length);			//DM-FEB2011
				rd.bankErosionData->ECnu.init(rd.migData->channel.centerline.length);						//DM-FEB2011
				rd.bankErosionData->ECnu_left.init(rd.migData->channel.centerline.length);				//DM-FEB2011
				rd.bankErosionData->ECnu_right.init(rd.migData->channel.centerline.length);				//DM-FEB2011
			}																						//DM-FEB2011
		}		
	}	

	//TODO: BE restructure
	//Regridding and section interpolation for "physically-based" approach
	if(rd.bankErosionData->MethodBankEvolution == 2 && rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 2 && rd.bankErosionData->RegridCenterlineNodes)	//DM-FEB2011	  		
	{		
		if(rd.prepData->MaxInterval/(rd.migData->channel.WIDTH/2) > rd.migData->channel.threshold_regrid)			//DM-FEB2011		
		{		
			rd.prep->ComputeTheta(rd.migData->channel.centerline, rd.prepData, rd.migData->channel.mesh_gen_method);									//DM-FEB2011	
			rd.prep->ComputeCurvature(j, rd.migData->curvMethod, rd.migData->channel.centerline, rd.prepData);											//DM-FEB2011	
			if (rd.prep->ApplyCurvatureFilter(rd.prepData))																						//DM-FEB2011	
			{																															//DM-FEB2011	
				rd.prep->CurvatureFilter(rd.migData->channel.centerline, rd.prepData, rd.migData);													//DM-FEB2011	
			}																															//DM-FEB2011	
			else																															//DM-FEB2011	
			{																															//DM-FEB2011	
				logmsg("No Filter on Curvature \n");																					//DM-FEB2011	
			}																															//DM-FEB2011	
			rd.prep->ComputeCurvatureGradient(rd.migData->channel.centerline, rd.prepData);															//DM-FEB2011	
			rd.hydrodynamics->Preprocess(j, rd.prepData, rd.migData, rd.hydrodynamicsData);																//DM-FEB2011	
			rd.migObj->Preprocess(j, rd.prepData, rd.migData);																						//DM-FEB2011	
			rd.migObj->CalculateDepthStraightChannel(rd.migData);																					//DM-FEB2011						
			rd.hydrodynamicsData->Init(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);														//DM-FEB2011	
			rd.hydrodynamics->ComputeDimensionlessPerturbationUVHDE(rd.migData, rd.prepData, rd.hydrodynamicsData);										//DM-FEB2011	
			rd.hydrodynamics->ComputeDimensionedPerturbationUVHDE(rd.prepData, rd.migData, rd.hydrodynamicsData);										//DM-FEB2011	
			rd.hydrodynamics->ComputeDimensionedFlowField(rd.migData, rd.prepData, rd.hydrodynamicsData);												//DM-FEB2011		



			rd.hydrodynamicsData->InitOldHydroValues(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);														// ADDITION MAR2010
			rd.hydrodynamics->StoreHydrodynamics(rd.migData, rd.hydrodynamicsData);																				// ADDITION MAR2010
			rd.bankErosionData->MonitorNodeStreamwiseDistance = rd.prepData->coords(rd.bankErosionData->MonitorNode) * rd.migData->channel.WIDTH/2;					//DM-FEB2011
			rd.prep->RegridPBA(rd.migData, rd.prepData);
			rd.bankErosion->ReachInterpolation(rd.prepData, rd.migData, rd.bankErosionData, rd.bankErosion);
			rd.hydrodynamicsData->InitNewHydroValues(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);														// ADDITION MAR2010
			rd.hydrodynamics->InterpolateHydrodynamics(rd.prepData, rd.migData, rd.hydrodynamicsData, rd.bankErosionData->BankInterpolationTolerance);					// ADDITION MAR2010  //DM-FEB2011
			rd.bankErosion->IntersectCrossSectionAndStageAndToes(rd.bankErosionData, rd.hydrodynamicsData, rd.migData);												// ADDITION MAR2010
			rd.bankErosionData->MonitorNode = rd.bankErosion->UpdateMonitorNode(rd.prepData, rd.migData, rd.bankErosionData->MonitorNodeStreamwiseDistance,rd.bankErosionData->MonitorNode);			// ADDITION MAR2010    //MMMMTEMP Changes in UpdateMonitorNode routine
			//bankErosionData->StationCenterline[bankErosionData->MonitorNode] = 0.0;																	// ADDITION MAR2010  //DM-FEB2011 COMMENTED OUT  	
		}		
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Computation of the angle
	START_TIMER(timer);
	rd.prep->ComputeTheta(rd.migData->channel.centerline, rd.prepData, rd.migData->channel.mesh_gen_method);   //DM-FEB2011
	rd.ComputeTheta_time += STOP_TIMER(timer);

	// Computation of curvature
	START_TIMER(timer);
	rd.prep->ComputeCurvature(j, rd.migData->curvMethod, rd.migData->channel.centerline, rd.prepData);
	rd.ComputeCurvature_time += STOP_TIMER(timer);
	// Optional filter on curvature
	if (rd.prep->ApplyCurvatureFilter(rd.prepData))
	{
		rd.prep->CurvatureFilter(rd.migData->channel.centerline, rd.prepData, rd.migData);  //DM-FEB2011
	}
	else
	{	
		logmsg("No Filter on Curvature \n");
	}

	// Computation of curvature gradient
	START_TIMER(timer);
	rd.prep->ComputeCurvatureGradient(rd.migData->channel.centerline, rd.prepData);
	rd.ComputeCurvatureGradient_time += STOP_TIMER(timer);

	// Allow the migration object to preprocess if necessary
	START_TIMER(timer);
	rd.hydrodynamics->Preprocess(j, rd.prepData, rd.migData, rd.hydrodynamicsData);
	rd.migObj->Preprocess(j, rd.prepData, rd.migData);
	rd.Preprocess_time += STOP_TIMER(timer);

	// Calculate the depth straight channel and alpha
	rd.migObj->CalculateDepthStraightChannel(rd.migData);							// Calculate Depth Straight Channel
	//CalculateAlpha went to hydrodynamics->Preprocess


	/***** HYDRODYNAMICS ***************************************/
	rd.hydrodynamicsData->Init(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH); //CORRECTION AUG2009
		
	// Computation of dimensionless perturbation fields (u,v,h,d,e) 
	START_TIMER(timer);
	rd.hydrodynamics->ComputeDimensionlessPerturbationUVHDE(rd.migData, rd.prepData, rd.hydrodynamicsData);
	rd.ComputeDimensionlessPerturbationUVHDE_time += STOP_TIMER(timer);
	
	// Computation of dimensioned perturbation fields (U1,V1,H1,D1,E1)
	START_TIMER(timer);
	rd.hydrodynamics->ComputeDimensionedPerturbationUVHDE(rd.prepData, rd.migData, rd.hydrodynamicsData);
	rd.ComputeDimensionedPerturbationUVHDE_time += STOP_TIMER(timer);

	// Computation of "total" variables fields (UU, VV, HH, DD, EE)
	START_TIMER(timer);
	rd.hydrodynamics->ComputeDimensionedFlowField(rd.migData, rd.prepData, rd.hydrodynamicsData);
	rd.ComputeDimensionedFlowField_time += STOP_TIMER(timer);

	//TODO: BE restructure	
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	if(rd.bankErosionData->FloodplainHeterogeneity)																				//DM-FEB2011
	{																														//DM-FEB2011
		//HETEROGENEITY																										//DM-FEB2011
		if(rd.bankErosionData->MethodBankEvolution == 1)																		//DM-FEB2011
		{																												//DM-FEB2011
			//rd.bankErosion->UpdateMigrationCoefficient(rd.bankErosionData,rd.migData,rd.prepData);									//DM-FEB2011	
			rd.bankErosion->UpdateMigrationCoefficientWithLayers(rd.bankErosionData,rd.migData,rd.prepData);						//DM-FEB2011	
		}																												//DM-FEB2011
																														//DM-FEB2011
		else if (rd.bankErosionData->MethodBankEvolution == 2)																	//DM-FEB2011
		{																										//DM-FEB2011
			//rd.bankErosion->UpdateBankProperties(rd.bankErosionData,rd.migData,rd.prepData);										//DM-FEB2011		
			rd.bankErosion->UpdateBankPropertiesWithLayers(rd.bankErosionData,rd.migData,rd.prepData);							//DM-FEB2011	 
		}																												//DM-FEB2011
		//rd.plotter->HeterogeneityAtCenterline(j,m_configData.Migration->DeltaT, rd.bankErosionData, rd.prepData);					//DM-FEB2011
	}	

	/*
	double maxtauc, mintauc;																										//MMMMTEMP
	maxtauc = 0.0;																													//MMMMTEMP
	mintauc = 10000.0;																													//MMMMTEMP
	for(int k = 0; k<= rd.migData->channel.centerline.length-1; k++)																//MMMMTEMP
		{																															//MMMMTEMP
			//printf("%d	%lf\n",k,rd.bankErosionData->grids[0].nodes[k].xsection.leftbank.material[0].criticalshearstress);			//MMMMTEMP
			if(rd.bankErosionData->grids[0].nodes[k].xsection.leftbank.material[0].criticalshearstress>maxtauc)maxtauc=rd.bankErosionData->grids[0].nodes[k].xsection.leftbank.material[0].criticalshearstress;//MMMMTEMP
			if(rd.bankErosionData->grids[0].nodes[k].xsection.leftbank.material[0].criticalshearstress<mintauc)mintauc=rd.bankErosionData->grids[0].nodes[k].xsection.leftbank.material[0].criticalshearstress;//MMMMTEMP
		}																															//MMMMTEMP
	printf("%lf	%lf\n",maxtauc,mintauc);//MMMMTEMP
	*/

	//DM-FEB2011
	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////

	//TODO: BE restructure		
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////ADDITION SEP2010bis//////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	if(rd.bankErosionData->MethodBankEvolution == 1)
	{	
		//rd.migObj->CalculateDepthStraightChannel(rd.migData);							// Calculate Depth Straight Channel
		//rd.migData->channel.D0 =  1.113579;			//CORRECTION JUNE 2010, D0 imposed (calculated offline, not calulated with Manning's expression)
		//double u0 = rd.migData->channel.FLOW/rd.migData->channel.WIDTH/rd.migData->channel.D0;		//flow / width / depth;
		//bankErosionData->FACTR = 2.0 * rd.bankErosionData->EC * u0 * m_configData.migration_duration / rd.migData->channel.WIDTH / m_configData.migration_num_iterations;
		rd.bankErosionData->FACTR = 2.0 * rd.bankErosionData->EC * rd.hydrodynamicsData->U_ch * m_configData.Migration->duration / rd.migData->channel.WIDTH / m_configData.Migration->num_iterations;
		//2.0 * bank_erosion_coeff * u0 * num_years / width / num_iter;
		//if (rd.bankErosionData->FACTR > 1.0)//DM-FEB2012 Commented out
		//	rd.bankErosionData->FACTR = -1.0;//DM-FEB2012 Commented out
		// if (temp > 1.0)
		// return -1.0;
		// else
		// return (temp * 3600.0 * 24.0 * 365.0);
		if(rd.migData->apply_interm == true)																				//DM-FEB2012
			rd.bankErosionData->FACTR =  (rd.bankErosionData->FACTR * 3600.0 * 24.0 * 365.0 * rd.migData->interm_factor);	//DM-FEB2012
		else																												//DM-FEB2012
			rd.bankErosionData->FACTR =  (rd.bankErosionData->FACTR * 3600.0 * 24.0 * 365.0);								//DM-FEB2012
		//HETEROGENEITY 

		if(rd.bankErosionData->FloodplainHeterogeneity)																																									//DM-FEB2011
		{																																																			//DM-FEB2011
			for(int i = 0; i <= rd.migData->channel.centerline.length-1; i++)																																			//DM-FEB2011
			{																																																	//DM-FEB2011
				//bankErosionData->FACTRnu(i) = 2.0 * rd.bankErosionData->ECnu(i) * rd.hydrodynamicsData->U_ch * m_configData.Migration->duration / rd.migData->channel.WIDTH / m_configData.Migration->num_iterations;	//DM-FEB2011 Commented out
				rd.bankErosionData->FACTRnu_left(i) = 2.0 * rd.bankErosionData->ECnu_left(i) * rd.hydrodynamicsData->U_ch * m_configData.Migration->duration / rd.migData->channel.WIDTH / m_configData.Migration->num_iterations;	//DM-FEB2011
				rd.bankErosionData->FACTRnu_right(i) = 2.0 * rd.bankErosionData->ECnu_right(i) * rd.hydrodynamicsData->U_ch * m_configData.Migration->duration / rd.migData->channel.WIDTH / m_configData.Migration->num_iterations;	//DM-FEB2011

				//if (rd.bankErosionData->FACTRnu(i) > 1.0)	rd.bankErosionData->FACTRnu(i) = -1.0;										//DM-FEB2011 Commented out																																			
				//else rd.bankErosionData->FACTRnu(i) =  (rd.bankErosionData->FACTRnu(i) * 3600.0 * 24.0 * 365.0);						//DM-FEB2011 Commented out																																														//DM-FEB2011

				//if (rd.bankErosionData->FACTRnu_left(i) > 1.0)	rd.bankErosionData->FACTRnu_left(i) = -1.0;								//DM-FEB2011			//DM-FEB2012 Commented out																														
				//else rd.bankErosionData->FACTRnu_left(i) =  (rd.bankErosionData->FACTRnu_left(i) * 3600.0 * 24.0 * 365.0);				//DM-FEB2011			//DM-FEB2012 Commented out
				
				if(rd.migData->apply_interm == true)rd.bankErosionData->FACTRnu_left(i) = (rd.bankErosionData->FACTRnu_left(i) * 3600.0 * 24.0 * 365.0 * rd.migData->interm_factor);//DM-FEB2012
				else rd.bankErosionData->FACTRnu_left(i) = (rd.bankErosionData->FACTRnu_left(i) * 3600.0 * 24.0 * 365.0);//DM-FEB2012

				//if (rd.bankErosionData->FACTRnu_right(i) > 1.0)	rd.bankErosionData->FACTRnu_right(i) = -1.0;							//DM-FEB2011			//DM-FEB2012 Commented out																													
				//else rd.bankErosionData->FACTRnu_right(i) =  (rd.bankErosionData->FACTRnu_right(i) * 3600.0 * 24.0 * 365.0);				//DM-FEB2011		//DM-FEB2012 Commented out

				if(rd.migData->apply_interm == true)rd.bankErosionData->FACTRnu_right(i) =  (rd.bankErosionData->FACTRnu_right(i) * 3600.0 * 24.0 * 365.0 * rd.migData->interm_factor);//DM-FEB2012
				else rd.bankErosionData->FACTRnu_right(i) =  (rd.bankErosionData->FACTRnu_right(i) * 3600.0 * 24.0 * 365.0); //DM-FEB2012

			}																																																
		}																																															

	}
	
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	// Write TecPlot output hydrodynamic files (hydrodynmanics is plotted before migration)  
	if (rd.plotter->CanPlot(j))
	{
		logmsg("TecPlot2D \n");
		rd.plotter->TecPlot2D(j, m_configData.Migration->DeltaT, rd.migData, rd.hydrodynamicsData, rd.prepData);
	}

	// Initial centerline (before migration) for Tecplot
	if (j == 1)
	{
		logmsg("TecPlot1D (initial) \n");
		rd.plotter->TecPlot1D(j-1, m_configData.Migration->DeltaT, rd.migData);
	}

	/***** BANK EROSION ****************************************/

	// Computation bank erosion 
	START_TIMER(timer);
//printf("(Bef XSADJ) 167 TOE ELEVATION = %lf \n",rd.bankErosionData->grids[0].nodes[167].xsection.rightbank.nodes[0].elevation); //MMMMTEMP Printing

	//TODO: BE restructure
	// Routines run if data->MethodBankEvolution = 2
	if(rd.bankErosionData->MethodBankEvolution == 2)						
	{	
/*
		printf("BEFORE SectionGeometryAdjustment\n");																														//MMMMTEMP AdNo
		if(rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberoflayers > 1)														//MMMMTEMP AdNo
		{																																									//MMMMTEMP AdNo
		printf("TopElev(BottomLayer) = %lf\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].elevation);					//MMMMTEMP AdNo
		printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].bottomindex);//MMMMTEMP AdNo
		printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].bottomindex);//MMMMTEMP AdNo
		}//MMMMTEMP AdNo
		for(int k = 0; k<= rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].station,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].elevation);//MMMMTEMP AdNo
*/

		rd.bankErosion->SectionGeometryAdjustment(rd.bankErosionData, rd.hydrodynamicsData, rd.migData, rd.prepData);//DM-FEB2011

/*
		printf("AFTER SectionGeometryAdjustment\n");//MMMMTEMP AdNo
		if(rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberoflayers > 1)//MMMMTEMP AdNo
		{//MMMMTEMP AdNo
		printf("TopElev(BottomLayer) = %lf\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].elevation);//MMMMTEMP AdNo
		printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].bottomindex);//MMMMTEMP AdNo
		printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].bottomindex);//MMMMTEMP AdNo
		}//MMMMTEMP AdNo
		for(int k = 0; k<= rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].station,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].elevation);//MMMMTEMP AdNo
*/
		if (j == 1)										
		{	
			rd.bankErosion->IntersectCrossSectionAndStageAndToes(rd.bankErosionData, rd.hydrodynamicsData, rd.migData);
			rd.bankErosion->PrepareTecPlot3D(rd.bankErosionData, rd.migData, rd.prepData);						
			rd.plotter->TecPlotBedBank3D(j-1, m_configData.Migration->DeltaT, rd.bankErosionData, rd.migData, rd.hydrodynamicsData);				
			rd.plotter->TecPlotSections(j-1, m_configData.Migration->DeltaT, rd.bankErosionData, rd.migData, rd.hydrodynamicsData, rd.prepData);					
		}
		if (rd.bankErosionData->MethodShearStress == 1)
		{
			rd.bankErosion->ComputeFrictionSlope(rd.bankErosionData, rd.hydrodynamicsData, rd.migData, rd.prepData);	
		}
	}

/*
double MinToeElevation = 1000.0;//MMMMTEMP
int MinToeNode = -99;//MMMMTEMP
for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)//MMMMTEMP
	{//MMMMTEMP
		if(rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[0].elevation<MinToeElevation)//MMMMTEMP
			{//MMMMTEMP
				MinToeElevation=rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[0].elevation;//MMMMTEMP
				MinToeNode = i;//MMMMTEMP
			}//MMMMTEMP
	}//MMMMTEMP
*/
//printf(">>>>>>> %lf	%d <<<<<<<<\n",MinToeElevation,MinToeNode);//MMMMTEMP

//printf("(After XSADJ) 167 TOE ELEVATION = %lf \n",rd.bankErosionData->grids[0].nodes[167].xsection.rightbank.nodes[0].elevation); //MMMMTEMP Printing


//for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)//MMMMTEMP
//	{						//MMMMTEMP
//		logmsg("COMPARE	%d	%lf	%lf	%lf\n",i,rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[1].elevation,rd.hydrodynamicsData->HH(i,1),rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes-1].elevation);
//	}						//MMMMTEMP


	rd.bankErosionData->Init(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);//CORRECTION AUG2009
	rd.migData->Init(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);//CORRECTION AUG2009
	rd.prepData->Init(rd.migData->channel.centerline.length, rd.ARRAY_WIDTH);//CORRECTION AUG2009
	rd.bankErosion->ComputeDimensionlessBankErosion(rd.hydrodynamicsData, rd.bankErosionData, rd.migData, rd.prepData, m_configData.Migration->DeltaT,j);		//DM-FEB2012 

/*
	if (rd.bankErosionData->MethodBankEvolution == 2)//MMMMTEMP AdNo																											//DM-FEB2012 
	{
		printf("AFTER Erosion\n");//MMMMTEMP AdNo
		if(rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberoflayers > 1)//MMMMTEMP AdNo
		{//MMMMTEMP AdNo
		printf("TopElev(BottomLayer) = %lf\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].elevation);//MMMMTEMP AdNo
		printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].bottomindex);//MMMMTEMP AdNo
		printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].topindex,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[1].bottomindex);//MMMMTEMP AdNo
		}//MMMMTEMP AdNo
		for(int k = 0; k<= rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].station,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.nodes[k].elevation);//MMMMTEMP AdNo
	}
*/

	//TODO: BE restructure
	/***** PRINT KARMOR AND BANK INDICES VALUES **********************************************/
	//Comment out this if not needed																												
	if (rd.bankErosionData->MethodBankEvolution == 2)																											//DM-FEB2012 
		{																																						//DM-FEB2012 
			rd.plotter->WriteKarmorFile(j, m_configData.Migration->DeltaT, rd.bankErosionData, rd.hydrodynamicsData, rd.prepData, rd.migData);					//DM-FEB2012 
			rd.plotter->WriteIndicesFile(j, m_configData.Migration->DeltaT, rd.bankErosionData);																//DM-FEB2012
			rd.plotter->WriteKarmorVisualizedFile(j, m_configData.Migration->DeltaT, rd.migData,rd.bankErosionData);											//DM-FEB2013
		}																																						//DM-FEB2012 
	rd.ComputeDimensionlessBankErosion_time += STOP_TIMER(timer);

	//for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)logmsg("BEFOREXXX	%d	%lf\n",i,rd.bankErosionData->ErosionBank(i,1)); //MMMMTEMP

	/***** MIGRATE CENTERLINE **********************************/
	START_TIMER(timer);										
	rd.migObj->Migrate(j, rd.prepData, rd.bankErosionData, rd.migData);																																					
	rd.Migrate_time += STOP_TIMER(timer);

	//for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)logmsg("AFTERXXX	%d	%lf\n",i,rd.bankErosionData->ErosionBank(i,1));//MMMMTEMP

	/***** SMOOTHING **********************************************/
	START_TIMER(timer);
	ApplySmoothing(j, rd.filter, rd.prepData, rd.prep, rd.migData, rd.ARRAY_WIDTH, rd.bankErosionData->MethodBankEvolution);
	rd.Smoothing_time += STOP_TIMER(timer);



	//TODO: BE restructure
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////ADDITION FEB2010//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/***** SECTION INTERPOLATION FOR "PHYSICALLY-BASED" APPROACH **********************************************/
	//Section interpolation for "physically-based" approach
	if(rd.bankErosionData->MethodBankEvolution == 2 && rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 2 && rd.bankErosionData->InterpolateAfterSplines && j % rd.bankErosionData->InterpolateFrequency == 0)	 //DM-FEB2011					
	{						
		rd.hydrodynamics->StoreHydrodynamics(rd.migData, rd.hydrodynamicsData);																			// ADDITION MAR2010
		rd.prep->ComputeIntervals(rd.migData, rd.prepData);																								//DM-FEB2011
		rd.bankErosionData->MonitorNodeStreamwiseDistance = rd.prepData->coords(rd.bankErosionData->MonitorNode) * rd.migData->channel.WIDTH/2;				//DM-FEB2011
		rd.bankErosion->ReachInterpolation(rd.prepData, rd.migData, rd.bankErosionData, rd.bankErosion);	
		rd.hydrodynamics->InterpolateHydrodynamics(rd.prepData, rd.migData, rd.hydrodynamicsData, rd.bankErosionData->BankInterpolationTolerance);			// ADDITION MAR2010		//DM-FEB2011
		rd.bankErosion->IntersectCrossSectionAndStageAndToes(rd.bankErosionData, rd.hydrodynamicsData, rd.migData);											// ADDITION MAR2010
		rd.bankErosionData->MonitorNode = rd.bankErosion->UpdateMonitorNode(rd.prepData, rd.migData, rd.bankErosionData->MonitorNodeStreamwiseDistance,rd.bankErosionData->MonitorNode);		// ADDITION MAR2010  //MMMMTEMP Changes in UpdateMonitorNode routine
		//bankErosionData->StationCenterline[bankErosionData->MonitorNode] = 0.0;																	// ADDITION MAR2010   //DM-FEB2011  COMMENTED OUT  
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/***** UPDATE STATION VALUES AT THE CENTERLINE **********************************************/

	//TODO: BE restructure
	if(rd.bankErosionData->MethodBankEvolution == 2)																																//DM-FEB2011
	{																																										//DM-FEB2011
		//Update station values at the centerline																															//DM-FEB2011
		if(rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 1)																										//DM-FEB2011																												//DM-FEB2011	
		{																																								//DM-FEB2011					
			for(int i = 0; i <= rd.migData->channel.centerline.length-1; i++)																								//DM-FEB2011	
			{																																						//DM-FEB2011	
				rd.bankErosionData->StationCenterline[i] = rd.bankErosionData->StationCenterline[i] - rd.bankErosionData->ErosionBank(i,1)*rd.migData->channel.WIDTH/2;			//DM-FEB2011	
			}																																						//DM-FEB2011						
		}																																								//DM-FEB2011	
		else if(rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 2 && rd.bankErosionData->MethodErosionDistance == 2)													//DM-FEB2011																//DM-FEB2011																											//DM-FEB2011
		{																																								//DM-FEB2011					
			for(int i = 0; i <= rd.migData->channel.centerline.length-1; i++)																								//DM-FEB2011
			{																																						//DM-FEB2011
				rd.bankErosionData->StationCenterline[i] = rd.bankErosionData->StationCenterline[i] - rd.bankErosionData->ErosionBank(i,1)*rd.migData->channel.WIDTH/2;			//DM-FEB2011
			}																																						//DM-FEB2011					
		}																																								//DM-FEB2011
		else if (rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 2 && rd.bankErosionData->MethodErosionDistance == 1)													//DM-FEB2011																//DM-FEB2011
		{																																								//DM-FEB2011
			rd.bankErosion->IntersectCrossSectionAndStageAndToes(rd.bankErosionData, rd.hydrodynamicsData, rd.migData);																//DM-FEB2011
			for(int i = 0; i <= rd.migData->channel.centerline.length-1; i++)																								//DM-FEB2011
			{																																						//DM-FEB2011
				rd.bankErosionData->StationCenterline[i] = rd.bankErosionData->toeleft[i].station+rd.migData->channel.WIDTH/2;												//DM-FEB2011
			}																																						//DM-FEB2011
		}																																																																			//DM-FEB2011
	}																														
	

	/***** BANK EROSION CHECKS **********************************************/
	//Remove comments to activate checks

	//Check for a single bank		//DM-FEB2012
/*
	if(rd.bankErosionData->MethodBankEvolution == 2)
		{
			printf("\n*** MONITOR NODE *** = %d \n", rd.bankErosionData->MonitorNode);
			printf("*** M.N.(Layer 0) *** Erosion distance = %lf \n", rd.bankErosionData->ErosionBank(rd.bankErosionData->MonitorNode,1)*rd.migData->channel.WIDTH/2);
			printf("*** M.N.(Layer 0) *** ErRate left = %e ErRate right = %e \n", rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].erosionrate,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[0].erosionrate);   
			printf("*** M.N.(Layer 0) *** CritTau left = %lf CritTau right = %lf \n", rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[0].criticalshearstress,rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[0].criticalshearstress);    
			printf("*** M.N.(Layer 0) *** TauS left = %lf TauS right = %lf \n", rd.hydrodynamicsData->TauS(rd.bankErosionData->MonitorNode,0),rd.hydrodynamicsData->TauS(rd.bankErosionData->MonitorNode,rd.migData->channel.nnodes-1));	
			printf("*** M.N.(Layer 0) *** Depth left = %lf Depth right = %lf \n", rd.hydrodynamicsData->DD(rd.bankErosionData->MonitorNode,0),rd.hydrodynamicsData->DD(rd.bankErosionData->MonitorNode,rd.migData->channel.nnodes-1));	
		
			printf("\n*** M.N. left bank ***\n");
			for(int k = 0; k <= rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.numberoflayers - 1; k++)
				{
					printf("Layer = %d	Er_rate_coeff = %e	Crit_shear_stress = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].erosionrate, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].criticalshearstress);  
					printf("Layer = %d	gamma_sat = %lf	c = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].unitweight, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].cohesion);  
					printf("Layer = %d	phi = %lf	phi_b = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].reposeangle, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].phib);  
					printf("Layer = %d	Elevation = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.material[k].elevation);
				}
			printf("Karmor = %lf \n", rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.leftbank.Karmor);  
			printf("*** M.N. right bank ***\n");
			for(int k = 0; k <= rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.numberoflayers - 1; k++)
				{
					printf("Layer = %d	Er_rate_coeff = %e	Crit_shear_stress = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].erosionrate, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].criticalshearstress);  
					printf("Layer = %d	gamma_sat = %lf	c = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].unitweight, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].cohesion);  
					printf("Layer = %d	phi = %lf	phi_b = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].reposeangle, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].phib);  
					printf("Layer = %d	Elevation = %lf \n", k, rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.material[k].elevation);
				}
			printf("Karmor = %lf \n", rd.bankErosionData->grids[0].nodes[rd.bankErosionData->MonitorNode].xsection.rightbank.Karmor); 
		}
*/

	//Check for all banks			//DM-FEB2012
/*
	if(rd.bankErosionData->MethodBankEvolution == 2)
		{
			double* LEFTERO = NULL;												//Array of fluvial erosion distance at left bank
			LEFTERO  = new double[rd.migData->channel.centerline.length];  
			double* RIGHTERO = NULL;											//Array of fluvial erosion distance at right bank
			RIGHTERO  = new double[rd.migData->channel.centerline.length];		
			double* MAXERO = NULL;												//Array of maximum fluvial erosion distance between left and right bank
			MAXERO  = new double[rd.migData->channel.centerline.length];
			double SUMANOMALY;													//Sum of differences between fluvial erosion distance computed from bank routines and directly computed with the fluvial erosion formula
			SUMANOMALY = 0.0;
			
			for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)
			{
					if(rd.hydrodynamicsData->TauS(i,0) > rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress && rd.hydrodynamicsData->DD(i,0) > 0.0)
							if(rd.migData->apply_interm == true) LEFTERO[i]  = rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].erosionrate*(rd.hydrodynamicsData->TauS(i,0)-rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress)/rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress*m_configData.Migration->DeltaT*24*3600*rd.migData->interm_factor;									//DM-FEB2011
							else LEFTERO[i] = rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].erosionrate*(rd.hydrodynamicsData->TauS(i,0)-rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress)/rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress*m_configData.Migration->DeltaT*24*3600;  																								//DM-FEB2011
					else
							{LEFTERO[i]  = 0.0;}
					
					if(rd.hydrodynamicsData->TauS(i,rd.migData->channel.nnodes-1) > rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress && rd.hydrodynamicsData->DD(i,rd.migData->channel.nnodes-1) > 0.0)	
							if(rd.migData->apply_interm == true) RIGHTERO[i] = -rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].erosionrate*(rd.hydrodynamicsData->TauS(i,rd.migData->channel.nnodes-1)-rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress)/rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress*m_configData.Migration->DeltaT*24*3600*rd.migData->interm_factor;	//DM-FEB2011
							else RIGHTERO[i] = -rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].erosionrate*(rd.hydrodynamicsData->TauS(i,rd.migData->channel.nnodes-1)-rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress)/rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress*m_configData.Migration->DeltaT*24*3600; 																//DM-FEB2011
					else
							{RIGHTERO[i]  = 0.0;}
					
					if(fabs(RIGHTERO[i]) == LEFTERO[i])											//DM-FEB2012
						{MAXERO[i] = 0.0;}														//DM-FEB2012
					else																		//DM-FEB2012
						{																		//DM-FEB2012														
							if(fabs(RIGHTERO[i]) > LEFTERO[i]){MAXERO[i] = RIGHTERO[i];}			
							else {MAXERO[i] = LEFTERO[i];};										
						}																		//DM-FEB2012
					if(fabs(rd.bankErosionData->ErosionBank(i,1)*rd.migData->channel.WIDTH/2 - MAXERO[i])>0.000001)printf("ANOMALY %d	%lf	%lf	%lf	%lf\n",i, rd.bankErosionData->ErosionBank(i,1)*rd.migData->channel.WIDTH/2, MAXERO[i],LEFTERO[i],RIGHTERO[i]);			
					
					SUMANOMALY = SUMANOMALY + fabs(rd.bankErosionData->ErosionBank(i,1)*rd.migData->channel.WIDTH/2 - MAXERO[i]);
				}
			


			delete [] LEFTERO; 
			delete [] RIGHTERO; 
			delete [] MAXERO; 
			LEFTERO = NULL;
			RIGHTERO = NULL;
			MAXERO = NULL;
			printf("SUMANOMALY = %lf\n",SUMANOMALY);
		}
*/
	
	//Check for MC approach				//DM-FEB2012																																										
	/*
	if(rd.bankErosionData->MethodBankEvolution == 1)		
		{													
			double sumdifferences;																																											
			sumdifferences = 0.0;																																											
			if(rd.bankErosionData->FloodplainHeterogeneity == false)																																		
				{																																														
					for(int inode = 0; inode <=rd.migData->channel.centerline.length-1; inode++)																											
						{																																													
							sumdifferences = sumdifferences + rd.bankErosionData->ErosionBank(inode,1)-rd.bankErosionData->FACTR*rd.hydrodynamicsData->u(inode,0);											
						}																																													
				}																																															
			else if(rd.bankErosionData->FloodplainHeterogeneity == true)																																	
				{																																																	
					for(int inode = 0; inode <=rd.migData->channel.centerline.length-1; inode++)																											
						{																																													
							if(rd.hydrodynamicsData->u(inode,0) >= 0)																																																
										{																																																			
											sumdifferences = sumdifferences + rd.bankErosionData->FACTRnu_left(inode) * rd.hydrodynamicsData->u(inode,0) - rd.bankErosionData->ErosionBank(inode,1);		
										}																																									
							else																																											
										{																																									
											sumdifferences = sumdifferences + rd.bankErosionData->FACTRnu_right(inode) * rd.hydrodynamicsData->u(inode,0) - rd.bankErosionData->ErosionBank(inode,1);		
										}																																									
						}																																													
				}																																															
			printf("Time step = %d, Difference = %lf\n",j,sumdifferences);																																	
		}													
	*/


//printf("rd.bankErosionData->ErosionBank(230,1) = %lf\n",rd.bankErosionData->ErosionBank(230,1)*rd.migData->channel.WIDTH/2.0);//MMMMTEMP

	//TODO: restructure
	/***** CURVATURE CAPPING **********************************************/
	//Curvature capping (removes nodes with dimensioneless curvature greater than 1.0 or smaller than -1.0
	if(rd.migData->curvMethod.CurvatureCapping)																																																											//DM-FEB2012
		{																																																																				//DM-FEB2012
			int numberofnodesbefore = rd.migData->channel.centerline.length;														// Store initial number of centerline nodes before possible elimination of nodes																	//DM-FEB2012
			rd.prep->ComputeIntervals(rd.migData, rd.prepData);																		// Compute streamwise coordinates																													//DM-FEB2012
			rd.prep->StoreCurrentStreamwiseCoordinate(rd.prepData, numberofnodesbefore);											// Store streamwise coordinates (for section interpolation)	//DM-FEB2012
			rd.prep->ComputeCurvature(j, rd.migData->curvMethod, rd.migData->channel.centerline, rd.prepData);						// Compute curvatures																																//DM-FEB2012
			rd.prep->CurvatureCapping(j, rd.migData->curvMethod, rd.migData, rd.prepData);											// Call CurvatureCapping routine																													//DM-FEB2012
			if(numberofnodesbefore != rd.migData->channel.centerline.length)														// Regridding through splines if a centerline node or more has been erased																			//DM-FEB2012	
				{ 																																																																		//DM-FEB2012
					rd.prep->ComputeIntervals(rd.migData, rd.prepData);																// Compute intervals 																																//DM-FEB2012
					rd.prep->RegridClean(rd.migData,rd.prepData,numberofnodesbefore);												// Regrid centerline nodes through splines																											//DM-FEB2012
					if(rd.bankErosionData->MethodBankEvolution == 2)rd.bankErosion->ReachInterpolation(rd.prepData, rd.migData, rd.bankErosionData, rd.bankErosion);				// Bank interpolation																																//DM-FEB2012
				}																																																																		//DM-FEB2012
		}																																																																				//DM-FEB2012

	//TODO: BE restructure
	/***** CUTOFF **********************************************/
	//Cutoff for migration-coefficient approach										//DM-FEB2011
	if(rd.bankErosionData->MethodBankEvolution == 1 && rd.migData->CutoffThreshold > 0)   //DM-FEB2011
	{
		rd.migData->CutoffHappened = false;				//DM-FEB2011
		rd.migObj->Cutoff(rd.migData);												//DM-FEB2011
		if(rd.migData->CutoffHappened == true)
			rd.prep->SplinesMCA(rd.migData, rd.ARRAY_WIDTH);									//DM-FEB2011
	}
	//Cutoff for physically-based approach										//DM-FEB2011
	if(rd.bankErosionData->MethodBankEvolution == 2 && rd.migData->CutoffThreshold > 0)   //DM-FEB2011
	{																			//DM-FEB2011
		//rd.migObj->CutoffPBA(rd.migData, rd.bankErosionData);							//DM-FEB2011 
		rd.bankErosion->CutoffPBA(rd.migData, rd.bankErosionData);	//MMMMTEMP Cutoff with PB method
	}																			//DM-FEB2011

	/***** PLOT CENTERLINES **********************************************/
	// Plotting: Write TecPlot output migration file
	if (rd.plotter->CanPlot(j))
	{
		logmsg("TecPlot1D \n");
		rd.plotter->TecPlot1D(j, m_configData.Migration->DeltaT, rd.migData);
	}

	/***** PLOT CROSS SECTIONS **********************************************/
	// Routines run if data->MethodBankEvolution = 2	
	if (rd.plotter->CanPlot(j) && rd.bankErosionData->MethodBankEvolution == 2)			
	{
		{		
			rd.bankErosion->PrepareTecPlot3D(rd.bankErosionData, rd.migData, rd.prepData);		
			rd.plotter->TecPlotSections(j, m_configData.Migration->DeltaT, rd.bankErosionData, rd.migData, rd.hydrodynamicsData, rd.prepData);
			rd.plotter->TecPlotBedBank3D(j, m_configData.Migration->DeltaT, rd.bankErosionData, rd.migData, rd.hydrodynamicsData); 
			// plotter->ReworkedAreas(j, m_configData.Migration->DeltaT, rd.bankErosionData, rd.migData);			//CORRECTION MAR2010
		}
	}

	//TODO: BE restructure
	//Check charactersitics of cross sections			//DM-FEB2012
	//Remove comments to activate checks
	if(rd.bankErosionData->MethodBankEvolution == 2)
		{
			//for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)																																																												//MMMMTEMP Printing
			//	{																																																																											//MMMMTEMP Printing
			//		logmsg("Top bank profiles	%d	%lf	%lf	%lf\n",i,rd.hydrodynamicsData->HH(i,1),rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[1].elevation,rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes-2].elevation);			//MMMMTEMP Printing
			//	}																																																																											//MMMMTEMP Printing
			
			int MaxNumberOfNodesLeftBank, MaxNumberOfNodesRightBank, NodeWithMaxNumberOfNodesLeftBank,NodeWithMaxNumberOfNodesRightBank;
			MaxNumberOfNodesLeftBank = 0; 
			MaxNumberOfNodesRightBank = 0;
			NodeWithMaxNumberOfNodesLeftBank = 0;
			NodeWithMaxNumberOfNodesRightBank = 0;

			for(int i = 0; i <=rd.migData->channel.centerline.length-1; i++)																																																																																																																			//MMMMTEMP Printing
				{																																																																																																																																		//MMMMTEMP Printing
					for(int m = 0; m <= rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes - 2; m++)if(rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m].elevation<rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m+1].elevation)printf("Left bank: node elevation not monotonically incresing at centerline node = %d	(%lf, %lf)\n",i,rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m].elevation,rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m+1].elevation);																																											//MMMMTEMP Printing
					for(int m = 0; m <= rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes - 2; m++)if(rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m].elevation>rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m+1].elevation)printf("Right bank: node elevation not monotonically incresing at centerline node = %d	(%lf, %lf)\n",i,rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m].elevation,rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m+1].elevation);	//MMMMTEMP Printing
					if(rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes > MaxNumberOfNodesLeftBank)
						{
							MaxNumberOfNodesLeftBank = rd.bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes;
							NodeWithMaxNumberOfNodesLeftBank = i;
						}
					if(rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes > MaxNumberOfNodesRightBank)
						{
							MaxNumberOfNodesRightBank = rd.bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes;
							NodeWithMaxNumberOfNodesRightBank = i;
						}
				}
			printf("MaxNumberOfNodesLeftBank = %d (at node %d)\n",MaxNumberOfNodesLeftBank,NodeWithMaxNumberOfNodesLeftBank);
			printf("MaxNumberOfNodesRightBank = %d (at node %d)\n",MaxNumberOfNodesRightBank,NodeWithMaxNumberOfNodesRightBank);																																																																																																																													//MMMMTEMP Printing
		}

printf("No Nodes %d \n", rd.migData->channel.centerline.length);			//MMMMTEMP

//TODO: BE restructure
	/***** UPDATE WIDTH AND DIMENSIONLESS CENTERLINE **********************************************/
	// Update width and dimensionless centerline	
	if(rd.bankErosionData->MethodBankEvolution == 2)							//CORRECTION JAN2010	
	{																	//CORRECTION DEC2009
		if(rd.bankErosionData->MethodPhysicallyBasedBankEvolution == 1)    //CORRECTION JAN2010			
		{
			rd.bankErosion->UpdateWidthAndDimensionlessCenterline(rd.bankErosionData, rd.migData);
		}
	}																	//CORRECTION DEC2009

	return 0;
}





int RVRMeander::Finish()
{
	rd.TotalTime += STOP_TIMER(rd.totalTimer);

	logmsg("********************************************************\n");
	logmsg("ComputeIntervals time = %f\n", rd.ComputeIntervals_time);
	logmsg("ComputeTheta time = %f\n", rd.ComputeTheta_time);
	logmsg("ComputeCurvature time = %f\n", rd.ComputeCurvature_time);
	logmsg("ComputeCurvatureGradient time = %f\n", rd.ComputeCurvatureGradient_time);
	logmsg("Preprocess time = %f\n", rd.Preprocess_time);
	logmsg("ComputeDimensionlessPerturbationUVHDE time = %f\n", rd.ComputeDimensionlessPerturbationUVHDE_time);
	logmsg("ComputeDimensionedPerturbationUVHDE time = %f\n", rd.ComputeDimensionedPerturbationUVHDE_time);
	logmsg("ComputeDimensionedFlowField time = %f\n", rd.ComputeDimensionedFlowField_time);
	logmsg("ComputeDimensionlessBankErosion time = %f\n", rd.ComputeDimensionlessBankErosion_time);
	logmsg("Migrate time = %f\n", rd.Migrate_time);
	logmsg("Smoothing time = %f\n", rd.Smoothing_time);
	logmsg("Total time = %f\n", rd.TotalTime);

	delete rd.migObj;
	delete rd.migData;
	delete rd.filter;
	delete rd.prep;
	delete rd.prepData;
	delete rd.hydrodynamics;
	delete rd.hydrodynamicsData;
	delete rd.bankErosion;
	delete rd.bankErosionData;
	delete rd.plotter;

	logmsg("Exiting. \n");

	return 0;
}


void RVRMeander::ApplySmoothing(int j, RMFilter* filter, RMPrepData* prepData, RMPrep*& prep, RMMigrationData* migData, int ARRAY_WIDTH, int MethodBankEvolution)
{ 
	int oldnumberpoints, newnumberpoints; 

	if (m_configData.Smoothing->order == 0)
	{
		// No Splines, No Filter
		logmsg("No Splines, No Filter on Centerline \n");
		return;
	}

	else if (m_configData.Smoothing->order == 1)
	{
		// Filter Only
		logmsg("Filter on Centerline, No Splines \n");
		rd.prep->ComputeIntervals(rd.migData,prepData);  //DM-FEB2011 Added
		if (filter->CanApplyFilter(j))
		{
			logmsg("Filter applied in this iteration \n");			//DM-FEB2011
			filter->SetIntervals(rd.prepData->intervals);
			filter->ApplyFilter(rd.migData->channel.centerline, rd.filter->m_polynomial_order, rd.filter->m_window_nodes);
		}
		else															//DM-FEB2011
		{															//DM-FEB2011
			logmsg("Filter not applied in this iteration \n");		//DM-FEB2011
		}															//DM-FEB2011	
	}

	else if (m_configData.Smoothing->order == 2)
	{			
		// Splines Only
		logmsg("Splines, No Filter on Centerline \n");
		rd.prep->ComputeIntervals(rd.migData,prepData);

		//TODO: BE restructure
		if (MethodBankEvolution == 1)
		{	
			rd.prep->SplinesMCA(rd.migData, rd.ARRAY_WIDTH);	
		}
		else if (MethodBankEvolution == 2)
		{
			rd.prep->SplinesPBA(rd.migData, rd.prepData, rd.ARRAY_WIDTH);
		}

		/*
		RMPrep prep;
		rd.prep.Initialize(rd.migData->channel.centerline);

		int size = 0;
		double** xy = NULL;
		rd.prep.GetXY(rd.migData->channel.centerline.length, size, xy);				//CORRECTION AUG2009

		migData->Init(size, rd.ARRAY_WIDTH);			
		migData->channel.centerline.init(size, 2, xy);
		*/
	}

	else if (m_configData.Smoothing->order == 3)
	{
		// Filter then Splines
		logmsg("Filter on Centerline then Splines \n");
		rd.prep->ComputeIntervals(rd.migData,prepData);  //DM-FEB2011 Moved before filter
		if (filter->CanApplyFilter(j))
		{	
			logmsg("Filter applied in this iteration \n");			//DM-FEB2011
			filter->SetIntervals(rd.prepData->intervals);
			filter->ApplyFilter(rd.migData->channel.centerline, rd.filter->m_polynomial_order, rd.filter->m_window_nodes);
		}
		else															//DM-FEB2011
		{															//DM-FEB2011
			logmsg("Filter not applied in this iteration \n");		//DM-FEB2011
		}															//DM-FEB2011

		//TODO: BE restructure
		if (MethodBankEvolution == 1)
		{	
			rd.prep->SplinesMCA(rd.migData, rd.ARRAY_WIDTH);
		}
		else if (MethodBankEvolution == 2)
		{
			rd.prep->SplinesPBA(rd.migData, rd.prepData, rd.ARRAY_WIDTH); 
		}

		/*
		RMPrep prep;
		rd.prep.Initialize(rd.migData->channel.centerline);

		int size = 0;
		double** xy = NULL;
		rd.prep.GetXY(rd.migData->channel.centerline.length, size, xy);				//CORRECTION AUG2009

		migData->Init(size, rd.ARRAY_WIDTH);			
		migData->channel.centerline.init(size, 2, xy);
		*/
	}

	else if (m_configData.Smoothing->order == 4)
	{
		// Splines then Filter
		logmsg("Splines then Filter on Centerline \n");

		//TODO: BE restructure
		rd.prep->ComputeIntervals(rd.migData,prepData);
		if (MethodBankEvolution == 1)
		{	
			rd.prep->SplinesMCA(rd.migData, rd.ARRAY_WIDTH);
		}
		else if (MethodBankEvolution == 2)
		{
			rd.prep->SplinesPBA(rd.migData, rd.prepData, rd.ARRAY_WIDTH);
		}

		/*
		RMPrep prep;
		rd.prep.Initialize(rd.migData->channel.centerline);

		int size = 0;
		double** xy = NULL;
		rd.prep.GetXY(rd.migData->channel.centerline.length, size, xy);				//CORRECTION AUG2009

		migData->Init(size, rd.ARRAY_WIDTH);			
		migData->channel.centerline.init(size, 2, xy);
		*/

		if (filter->CanApplyFilter(j))
		{
			logmsg("Filter applied in this iteration \n");			//DM-FEB2011
			filter->SetIntervals(rd.prepData->intervals);
			filter->ApplyFilter(rd.migData->channel.centerline, rd.filter->m_polynomial_order, rd.filter->m_window_nodes);
		}
		else															//DM-FEB2011
		{															//DM-FEB2011
			logmsg("Filter not applied in this iteration \n");		//DM-FEB2011
		}															//DM-FEB2011

	}

}
