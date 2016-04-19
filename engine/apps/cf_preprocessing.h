#ifndef __CF_PREPROCESSING_H________________20101004083636__
#define __CF_PREPROCESSING_H________________20101004083636__

#include "config_file_obj.h"
#include "cf_migration.h"
#include "cf_smoothing.h"
#include "../rmio/rmlog.h"
#include "../rmprep/rmprep.h"


namespace ConfigFile
{
    struct Preprocessing
    {
        static Preprocessing* Create(GenericConfigObj& config, RMLogger* logger);
        void InitializePreprocessingObject(const Migration* mig, const Smoothing* smoothing, RMCurvatureMethod& method, RMPrepData*& prepData, RMPrep*& prep);

	    int num_points;
	    int curvature_method;
	    int curvature_upstream_method;
	    int curvature_downstream_method;
		bool curvature_capping;//DM-FEB2012
    };
}


#endif//__CF_PREPROCESSING_H________________20101004083636__
