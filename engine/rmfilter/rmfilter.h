#ifndef __FILTER_H__________________________20090406143939__
#define __FILTER_H__________________________20090406143939__

#include "../common.h"
#include "../rmio/rmseries.h"

#if defined(LIBRMFILTER_EXPORTS)
#define LIBRMFILTER_DLL_IMP_EXP DLL_EXPORT
#elif defined(LIBRMFILTER_STATIC)
#define LIBRMFILTER_DLL_IMP_EXP
#else
#define LIBRMFILTER_DLL_IMP_EXP DLL_IMPORT
#endif


class LIBRMFILTER_DLL_IMP_EXP RMFilter
{
public:
	virtual bool ApplyFilter(RMSeries& data, int polynomial_order, int window_nodes);			//CORRECTION FEB2010
	virtual bool ApplyFilterInitially();
	virtual bool CanApplyFilter(int iter);
	virtual void SetFilterFrequency(int freq);
	virtual int GetFilterFrequency();
	virtual void SetNumIterations(int numIter);
	virtual int GetNumIterations();
	virtual void SetIntervals(const RMSeries& intervals);
	virtual void Init();
	int m_polynomial_order;			//ADDITION FEB2010
	int m_window_nodes;				//ADDITION FEB2010
	int m_numIter;				//DM-FEB2011 Moved from Protected to Public
	int m_freq;					//DM-FEB2011 Moved from Protected to Public

protected:
	RMSeries m_intervals;
	bool m_initially;

};


#endif//__FILTER_H__________________________20090406143939__
