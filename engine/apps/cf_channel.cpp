#pragma warning(disable : 4290)

#include "cf_channel.h"
#include "config_file_util.h"
#include "error.h"


namespace ConfigFile
{
    Channel* Channel::Create(GenericConfigObj &config, RMLogger *logger)
    {
        Channel* obj = new Channel();
        if (!configDoubleLookup(config, obj->flow, "channel.flow"))
            CONFIG_ERR("channel.flow");
        if (!configDoubleLookup(config, obj->width, "channel.width"))
            CONFIG_ERR("channel.width");
        if (!configDoubleLookup(config, obj->sediment_size, "channel.sediment_size"))
            CONFIG_ERR("channel.sediment_size");
	    if (!configDoubleLookup(config, obj->water_density, "channel.water_density"))
		    CONFIG_ERR("channel.water_density");
	    if (!configDoubleLookup(config, obj->valley_slope, "channel.valley_slope"))
		    CONFIG_ERR("channel.valley_slope");
	    if (!configDoubleLookup(config, obj->upstream_bed_elevation, "channel.upstream_bed_elevation"))
		    CONFIG_ERR("channel.upstream_bed_elevation");
	    if (!configIntLookup(config, obj->num_transverse_nodes, "channel.num_transverse_nodes"))
		    CONFIG_ERR("channel.num_transverse_nodes");
	    if (!configIntLookup(config, obj->mesh_generation_method, "channel.mesh_generation_method"))			//DM-FEB2011
		    CONFIG_ERR("channel.mesh_generation_method");														//DM-FEB2011
	    if (!configDoubleLookup(config, obj->threshold_regridding, "channel.threshold_regridding"))				//DM-FEB2011
		    CONFIG_ERR("channel.threshold_regridding");															//DM-FEB2011
        if (!configStringLookup(config, obj->centerline, "channel.centerline"))
            CONFIG_ERR("channel.centerline");
        if (!configStringLookup(config, obj->valley_centerline, "channel.valley_centerline"))
            CONFIG_ERR("channel.valley_centerline");
        if (!configBoolLookup(config, obj->use_valley_centerline, "channel.use_valley_centerline"))				//DM-FEB2011
            CONFIG_ERR("channel.use_valley_centerline");														//DM-FEB2011
	    if (!configDoubleLookup(config, obj->manning_coefficient, "channel.manning_coefficient"))				//CORRECTION AUG2009
            CONFIG_ERR("channel.manning_coefficient");															//CORRECTION
		return obj;
    }

    void Channel::InitializeChannelObject(RMChannel& channel)
    {
	    channel.FLOW = flow;
	    channel.WIDTH =  width;
	    channel.SEDSIZE =  sediment_size;
	    channel.ValleySlope = valley_slope;
	    channel.WaterDensity = water_density;
	    channel.USBed = upstream_bed_elevation;
	    channel.nnodes = num_transverse_nodes;
		channel.mesh_gen_method = mesh_generation_method;		//DM-FEB2011
        channel.threshold_regrid = threshold_regridding;		//DM-FEB2011
		channel.use_valley_centerl = use_valley_centerline;		//DM-FEB2011
	    channel.ManningCoefficient = manning_coefficient;
    }
}
