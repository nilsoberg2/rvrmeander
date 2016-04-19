#ifndef __RMCHANNEL_H_______________________20090410094141__
#define __RMCHANNEL_H_______________________20090410094141__

#include "../common.h"
#include "../rmio/rmseries.h"

#if defined(LIBRMCHANNEL_EXPORTS)
#define LIBRMCHANNEL_DLL_IMP_EXP DLL_EXPORT
#else
#define LIBRMCHANNEL_DLL_IMP_EXP DLL_IMPORT
#endif


struct RMChannel
{
	double FLOW;
	double D0;
	double WIDTH;
	double SEDSIZE;
	RMSeries centerline;
	RMSeries valley_centerline;			//VALLEY
	int nnodes;
	int mesh_gen_method;			//DM-FEB2011
	double threshold_regrid;		//DM-FEB2011
	bool use_valley_centerl;		//DM-FEB2011

    double WaterDensity;        // Mass water density in kg/m3, for the calculation of the shear stress
    double ValleySlope;         // Slope of the valley, used to calculate the slope of the channel
    double USBed;               // UpstreamBedElevation

    double ManningCoefficient;												// For routine CalculateDepthStraightChannel()	
};


#endif//__RMCHANNEL_H_______________________20090410094141__
