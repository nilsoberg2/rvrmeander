
#include "config_file.h"
#include "cf_preprocessing.h"
#include "../rmio/rmlog.h"


namespace ConfigFile
{
    Preprocessing* Preprocessing::Create(GenericConfigObj& config, RMLogger* logger)
    {
        Preprocessing* obj = new Preprocessing();
	    if (!configIntLookup(config, obj->curvature_method, "preprocessing.curvature.method"))
            CONFIG_ERR("preprocessing.curvature.method");
	    if (!configIntLookup(config, obj->curvature_upstream_method, "preprocessing.curvature.upstream_method"))
            CONFIG_ERR("preprocessing.curvature.upstream_method");
	    if (!configIntLookup(config, obj->curvature_downstream_method, "preprocessing.curvature.downstream_method"))
            CONFIG_ERR("preprocessing.curvature.downstream_method");
	    if (!configBoolLookup(config, obj->curvature_capping, "preprocessing.curvature.curvature_capping"))//DM-FEB2012
            CONFIG_ERR("preprocessing.curvature.curvature_capping");  //DM-FEB2012
        return obj;
    }

    void Preprocessing::InitializePreprocessingObject(const Migration* mig, const Smoothing* smoothing, RMCurvatureMethod &method, RMPrepData*& prepData, RMPrep*& prep)
    {
        method.CurvatureMethod = curvature_method;
	    method.DownstreamCurvatureMethod = curvature_downstream_method;
	    method.UpstreamCurvatureMethod = curvature_upstream_method;
		method.CurvatureCapping = curvature_capping;//DM-FEB2012

	    prep = new RMPrep();
	    prepData = new RMPrepData();

	    prepData->C_0 = mig->init_curvature;
	    prepData->FilterCurvature = smoothing->curvature_filter;
    }
}
