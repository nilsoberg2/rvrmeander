#ifndef __RMPREPDATA_H______________________20090413132121__
#define __RMPREPDATA_H______________________20090413132121__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include "rmprep_impexp.h"
#include "rmprep_enums.h"

//Test
struct LIBPREP_DLL_IMP_EXP RMPrepData
{
	// Curvature
	RMSeries curvature;
	RMSeries curvaturecopy;
	// Curvature derivitave
	RMSeries dcurvatureds;
	// Length between intrinsic coordinate points
	RMSeries intervals;
	double MaxInterval;				//ADDITION FEB2010
	double MinInterval;//DM-FEB2011
	// Dimensioness s 
	RMSeries coords;
	RMSeries oldcoords;				//ADDITION FEB2010
	RMSeries wInterpolationDS;		//ADDITION FEB2010

	// Dimensioness n 
	RMSeries coordn;
	// s with dimensions 
	RMSeries S;
	// n with dimensions 
	RMSeries N;

	// For curvature
	RMSeries deltaX;
	RMSeries deltaY;
	RMSeries delta2X;
	RMSeries delta2Y;

	RMSeries theta;

	double C_0;
	double sSum;

	bool FilterCurvature;
	int inodeOldXsDS[10000];				//ADDITION FEB2010   //DM-FEB2012

	// Initialize the size of the arrays in this object to be the size of the input array.
	virtual void Init(int length, int width);
	virtual void InitBase(int length, int width);
	virtual ~RMPrepData();
};


#endif//__RMPREPDATA_H______________________20090413132121__
