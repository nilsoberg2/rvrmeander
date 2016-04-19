
#include "rmfilter.h"

void RMFilter::Init()
{
	m_initially = false;
	m_freq = 0;
	m_numIter = 0;
}

bool RMFilter::ApplyFilter(RMSeries& data, int polynomial_order, int window_nodes)     //CORRECTION FEB2010
{
	return false;
}

bool RMFilter::ApplyFilterInitially()
{
	return m_initially;
}

bool RMFilter::CanApplyFilter(int iter)
{
	if (m_freq > 0 && (iter % m_freq) == 0)
		return true;
	else
		return false;
}

void RMFilter::SetFilterFrequency(int freq)
{
	m_freq = freq;
	//m_initially = (initially ? true : false);								
}

int RMFilter::GetFilterFrequency()
{
	return m_freq;
}

void RMFilter::SetNumIterations(int numIter)
{
	m_numIter = numIter;
}

int RMFilter::GetNumIterations()
{
	return m_numIter;
}

void RMFilter::SetIntervals(const RMSeries& intervals)
{
	m_intervals = intervals;
}
