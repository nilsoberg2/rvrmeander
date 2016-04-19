#ifndef __RMMIGRATIONDATA_H_________________20090410150202__
#define __RMMIGRATIONDATA_H_________________20090410150202__

// Place here
#include "../rmchannel/rmchannel.h"
#include "../rmprep/rmprep.h"
#include "rmmigration_impexp.h"


struct LIBRMMIGRATION_DLL_IMP_EXP RMMigrationData
{
	// Channel description
	RMChannel channel;

	// Curvature parameters
	RMCurvatureMethod curvMethod;

	// Spatial coordinates for 2D plot (dimensionless)
	RMSeries x, y;
	// Spatial coordinates for 2D plot (with dimensions)
	RMSeries XX, YY;

	bool MigrateUpstreamNode;
	bool MigrateDownstreamNode;

	double CutoffThreshold;													// Threshold for cutoff
	bool CutoffHappened; //DM-FEB2011

	bool apply_interm;//DM-FEB2012
	double interm_factor;//DM-FEB2012

	virtual void Init(int length, int width);
};


#endif//__RMMIGRATIONDATA_H_________________20090410150202__
