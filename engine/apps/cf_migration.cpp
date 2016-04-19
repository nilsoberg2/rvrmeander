#pragma warning(disable : 4290)

#include "cf_migration.h"
#include "error.h"
#include "config_file_util.h"


namespace ConfigFile
{
    Migration* Migration::Create(GenericConfigObj& config, RMLogger* logger)
    {
        Migration* obj = new Migration();
        if (!configDoubleLookup(config, obj->duration, "migration.duration"))
            CONFIG_ERR("migration.duration");
	    if (!configIntLookup(config, obj->num_iterations, "migration.num_iterations"))
            CONFIG_ERR("migration.num_iterations");
	    if (!configIntLookup(config, obj->filter_iterations, "migration.filter_iterations"))
            CONFIG_ERR("migration.filter_iterations");
	    if (!configBoolLookup(config, obj->splines_initially, "migration.splines_initially"))
            CONFIG_ERR("migration.splines_initially");
	    if (!configIntLookup(config, obj->plot_iterations, "migration.plot_iterations"))
		    CONFIG_ERR("migration.plot_iterations");
	    if (!configBoolLookup(config, obj->migrate_upstream_node, "migration.migrate_upstream_node"))
		    CONFIG_ERR("migration.migrate_upstream_node");
	    if (!configBoolLookup(config, obj->migrate_downstream_node, "migration.migrate_downstream_node"))
		    CONFIG_ERR("migration.migrate_downstream_node");
	    if (!configDoubleLookup(config, obj->threshold_for_cutoff, "migration.threshold_for_cutoff"))		//CORRECTION AUG2009
		    CONFIG_ERR("migration.ThresholdForCutoff");																		//CORRECTION AUG2009
		if (!configBoolLookup(config, obj->apply_intermittency, "migration.apply_intermittency"))					//DM-FEB2012
		    CONFIG_ERR("migration.apply_intermittency");															//DM-FEB2012
	    if (!configDoubleLookup(config, obj->intermittency_factor, "migration.intermittency_factor"))				//DM-FEB2011
		    CONFIG_ERR("migration.intermittency_factor");															//DM-FEB2011
        if (!configDoubleLookup(config, obj->init_curvature, "migration.init_curvature"))
            CONFIG_ERR("migration.init_curvature");
        return obj;
    }

    void Migration::InitializeMigrationObject(RMMigrationData*& data_in, RMMigration*& mig_in)
    {
        RMMigrationData* data = new RMMigrationData();
	    data_in = data;
	    RMMigration* mig = new RMMigration();
	    mig_in = mig;

	    data->MigrateUpstreamNode = migrate_upstream_node;
	    data->MigrateDownstreamNode = migrate_downstream_node;
	    data->CutoffThreshold = threshold_for_cutoff;
		data->apply_interm = apply_intermittency;				//DM-FEB2011
		data->interm_factor = intermittency_factor;				//DM-FEB2011
        this->DeltaT = duration / num_iterations * 365;
    }
}
