#ifndef __CF_CHANNEL_H______________________20101003225757__
#define __CF_CHANNEL_H______________________20101003225757__

#include "config_file_obj.h"
#include "../rmio/rmlog.h"
#include "../rmchannel/rmchannel.h"


namespace ConfigFile
{
    struct Channel
    {
        static Channel* Create(GenericConfigObj& config, RMLogger* logger);
        void InitializeChannelObject(RMChannel& channel);

	    double flow;
	    double width;
	    double sediment_size;
	    std::string centerline;
	    std::string valley_centerline;
        std::string output;
	    double water_density;
	    double valley_slope;
	    double upstream_bed_elevation;
	    int num_transverse_nodes;
		int mesh_generation_method;			//DM-FEB2011
		double threshold_regridding;		//DM-FEB2011
		bool use_valley_centerline;			//DM-FEB2011
        double manning_coefficient;
    };
}


#endif//__CF_CHANNEL_H______________________20101003225757__
