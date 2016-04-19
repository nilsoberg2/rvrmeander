#ifndef __CF_SMOOTHING______________________20101003231414__
#define __CF_SMOOTHING______________________20101003231414__

#include "config_file_obj.h"
#include "cf_migration.h"
#include "../rmio/rmlog.h"
#include "../rmio/rmseries.h"
#include "../rmfilter/rmfilter.h"


namespace ConfigFile
{
    struct Smoothing
    {
        static Smoothing* Create(GenericConfigObj& config, RMLogger* logger);
        virtual RMFilter* CreateFilter(const Migration* mig) = 0;

	    std::string method;
	    int order; // 0 nothing, 1 for splines only, 2 for splines then filter, 3 for filter then splines, and 4 for filter only
	    bool curvature_filter;								//CORRECTION AUG2009
    };


    struct Smoothing_SavitzkyGolay : public Smoothing
    {
        static Smoothing_SavitzkyGolay* Create(GenericConfigObj& config, RMLogger* logger);
        virtual RMFilter* CreateFilter(const Migration* mig);  
	
	    int parameters_iterations;
	    int parameters_polynomial_order;
	    int parameters_window_nodes;
        RMSeries parameters_coeffs;
    };
}


#endif//__CF_SMOOTHING______________________20101003231414__
