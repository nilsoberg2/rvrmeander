#pragma warning(disable : 4290)

#include "cf_smoothing.h"
#include "config_file_util.h"
#include "util.h"
#include "error.h"
#include "../rmfilter/rmfilter.h"
#include "../rmfilter/rmfilter_savitzkygolay.h"


namespace ConfigFile
{
    Smoothing* Smoothing::Create(GenericConfigObj &config, RMLogger *logger)
    {
        std::string method;
        if (!configStringLookup(config, method, "smoothing.method"))
            CONFIG_ERR("smoothing.method");

        Smoothing* obj = NULL;

        if (ToLower(method) == "savitzky-golay")
            obj = Smoothing_SavitzkyGolay::Create(config, logger);

        if (obj != NULL)
        {
	        if (!configIntLookup(config, obj->order, "smoothing.smoothing_order"))
		        CONFIG_ERR("smoothing.smoothing_order");
	        if (!configBoolLookup(config, obj->curvature_filter, "smoothing.curvature_filter"))
		        CONFIG_ERR("smoothing.curvature_filter");
            obj->method = method;
        }

        return obj;
    }


    Smoothing_SavitzkyGolay* Smoothing_SavitzkyGolay::Create(GenericConfigObj& config, RMLogger* logger)
    {
        Smoothing_SavitzkyGolay* obj = new Smoothing_SavitzkyGolay();

	    if (!configIntLookup(config, obj->parameters_iterations, "smoothing.parameters.iterations"))
		    CONFIG_ERR("smoothing.parameters.iterations");
	    if (!configIntLookup(config, obj->parameters_polynomial_order, "smoothing.parameters.polynomial_order"))	//CORRECTION FEB2010
		    CONFIG_ERR("smoothing.parameters.polynomial_order");																			//CORRECTION FEB2010
	    if (!configIntLookup(config, obj->parameters_window_nodes, "smoothing.parameters.window_nodes"))			//CORRECTION FEB2010
		    CONFIG_ERR("smoothing.parameters.window_nodes");	

        int size = 11;
	    double* temp = new double[size];
	    for (int i = 0; i < size; i++)
	    {
		    temp[i] = 0.0;																													//CORRECTION FEB2010
	    }
	    obj->parameters_coeffs.init(size, temp);
	    delete[] temp;

        return obj;
    }


    RMFilter* Smoothing_SavitzkyGolay::CreateFilter(const Migration* mig)
    {
		RMFilter_SavitzkyGolay* filter = new RMFilter_SavitzkyGolay();

		filter->SetCoefficients(this->parameters_coeffs);									
		filter->SetNumIterations(this->parameters_iterations);
		filter->SetWindowSize((this->parameters_window_nodes-1)/2, (this->parameters_window_nodes-1)/2);	//CORRECTION FEB2010
		filter->SetFilterFrequency(mig->filter_iterations); 
		filter->m_polynomial_order = this->parameters_polynomial_order;															//ADDITION FEB2010
		filter->m_window_nodes = this->parameters_window_nodes; 																	//ADDITION FEB2010

        return filter;
	/*}																						//CORRECTION AUG2009
	else																					//CORRECTION AUG2009
		return new RMFilter();*/															//CORRECTION AUG2009
    }
}
