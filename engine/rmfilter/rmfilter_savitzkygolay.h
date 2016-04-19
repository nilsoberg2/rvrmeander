#ifndef __RMFILTER_SAVITZKYGOLAY_H__________20090406144040__
#define __RMFILTER_SAVITZKYGOLAY_H__________20090406144040__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "rmfilter.h"


class LIBRMFILTER_DLL_IMP_EXP RMFilter_SavitzkyGolay
	: public RMFilter
{
public:
	virtual bool ApplyFilter(RMSeries& series, int polynomial_order, int window_nodes);//CORRECTION FEB2010

	virtual bool SavitzkyGolay_p4m7(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p4m9(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p4m11(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p4m13(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p4m15(RMSeries& series);  //ADDITION FEB2010

	virtual bool SavitzkyGolay_p2m5(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p2m7(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p2m9(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p2m11(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p2m13(RMSeries& series);  //ADDITION FEB2010
	virtual bool SavitzkyGolay_p2m15(RMSeries& series);  //ADDITION FEB2010

	virtual void SetWindowSize(int leftWindowSize, int rightWindowSize);
	virtual void SetCoefficients(const RMSeries& coeffs);
	virtual RMSeries GetCoefficients();
	virtual void GetWindowSize(int& leftWindowSize, int& rightWindowSize);

protected:
	int m_leftWindowSize;
	int m_rightWindowSize;
	RMSeries m_coeffs;
};


#endif//__RMFILTER_SAVITZKYGOLAY_H__________20090406144040__
