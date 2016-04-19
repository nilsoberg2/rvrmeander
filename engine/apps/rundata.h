#ifndef RUNDATA_H__
#define RUNDATA_H__

#include "../common.h"
#include "../rmprep/rmprep.h"
#include "../rmfilter/rmfilter.h"
#include "../rmmigration/rmmigration.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"
#include "../rmbankerosion/rmbankerosion.h"
#include "../rmplotting/rmplotting.h"
#include "../rmio/rmlog.h"
#include "../timer.h"


struct RunData
{
	RMMigration* migObj;
	RMMigrationData* migData;
	RMFilter* filter;
	RMPrep* prep;
	RMPrepData* prepData;
	RMHydrodynamics* hydrodynamics;
	RMHydrodynamicsData* hydrodynamicsData;
	RMBankErosion* bankErosion;
	RMBankErosionData* bankErosionData;
	RMPlotting* plotter;
	int ARRAY_LENGTH;
	int ARRAY_WIDTH;

	double streamwiseLength;

	CPrecisionTimer totalTimer;
	double ComputeIntervals_time;
	double ComputeTheta_time;
	double ComputeCurvature_time;
	double ComputeCurvatureGradient_time;
	double Preprocess_time;
	double ComputeDimensionlessPerturbationUVHDE_time;
	double ComputeDimensionedPerturbationUVHDE_time;
	double ComputeDimensionedFlowField_time;
	double ComputeDimensionlessBankErosion_time;
	double Migrate_time;
	double Smoothing_time;
	double TotalTime;
};


#endif//RUNDATA_H__
