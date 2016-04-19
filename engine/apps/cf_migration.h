#ifndef __CF_MIGRATION_H____________________20101003225353__
#define __CF_MIGRATION_H____________________20101003225353__

#include "config_file_obj.h"
#include "cf_channel.h"
#include "../rmio/rmlog.h"
#include "../rmmigration/rmmigration.h"
#include "../rmmigration/rmmigrationdata.h"


namespace ConfigFile
{
    struct Migration
    {
        static Migration* Create(GenericConfigObj& config, RMLogger* logger);
        void InitializeMigrationObject(RMMigrationData*& data_in, RMMigration*& mig_in);

        double duration;			// same as Duration			//CORRECTION AUG2009
        int num_iterations;		// same as Iterations
	    int filter_iterations;
	    bool splines_initially;			
	    int plot_iterations;
	    bool migrate_upstream_node;
	    bool migrate_downstream_node;
    	double threshold_for_cutoff;													//CORRECTION AUG2009
        bool apply_intermittency;					//DM-FEB2011
		double intermittency_factor;				//DM-FEB2011
		double init_curvature;

        double DeltaT;

        RMLogger* m_logger;
    };
}


#endif//__CF_MIGRATION_H____________________20101003225353__
