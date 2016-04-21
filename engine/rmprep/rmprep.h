#ifndef __PREPROCESSING_H___________________20060427093838__
#define __PREPROCESSING_H___________________20060427093838__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>
#include "rmprep_impexp.h"
#include "rmprep_enums.h"
#include "rmprepdata.h"
#include "../rmmigration/rmmigrationdata.h"		//CORRECTION AUG2009
//#include "../rmbankerosion/rmbankerosion.h"		//CORRECTION AUG2009
#include "virtual_array.h"

// RMPrep creates splines from a 2-D double array.    It
// then provides a client with capability to interpolate on
// those splines, compute curvature from those splines, as well
// as other functionality.
class LIBPREP_DLL_IMP_EXP RMPrep
    : public RMLoggerImpl
{
public:
    RMPrep();
    virtual ~RMPrep();
    virtual bool Initialize(double** xy, int numPoints, bool reverseOrientation = false);
	virtual bool Initialize(const RMSeries& data, bool reverseOrientation = false);
	virtual bool InitializeVirtual(Virtual2ColArray* data, bool reverseOrientation = false);

    // Interpolation functions
    virtual bool GetXY(double at, double& x, double& y);
    virtual double GetX(double at);
    virtual double GetY(double at);
    virtual bool GetX(double at, double& x);
    virtual bool GetY(double at, double& y);

    // This gets the values of the spline interpolated at numPoints evenly-
    // spaced points.
    virtual bool GetXY(int numPoints, int& xySize, double**& xy, double min = 0.0, double max = 0.0);

    virtual void FreeXY(int numPoints, double**& xy);

    // This is similar to GetXY, but stores the values three times (this
    // is useful for migration as there are three arrays that get initialized
    // with the same values).
    virtual bool GetXY3(int numPoints, int& xySize, double**& xy, double**& xy2, double**& xy3, double min = 0.0, double max = 0.0);

    virtual double GetCurvatureAt(double at);
    virtual bool GetCurvatureAt(double at, double& curvature);

    // This gets the curvature interpolated at numPoints evenly-spaced points.
    virtual bool GetCurvature(int numPoints, int& curvSize, double**& curvature);

    // This gets the curvature and the values of the splines interpolated at
    // numPoints evenly-spaced points.
    virtual bool GetCurvature(int numPoints, int& curvSize, double**& curvature, double**& xy);
    virtual bool GetCurvaturePreCreated(Virtual2ColArray* curv, Virtual2ColArray* xy, int& curvSize);

    virtual void GetRange(double& minX, double& minY, double& minI, double& maxX, double& maxY, double& maxI);
    virtual void GetXRange(double& minX, double& maxX);
    virtual void GetYRange(double& minY, double& maxY);
    virtual void GetIRange(double& minI, double& maxI);

    // This returns the minimum distance between points in the original geometry.
    virtual double GetMinStep();

    // This returns the maximum distance between points in the original geometry.
    virtual double GetMaxStep();

    virtual double GetLength();

    // This returns the recommended number of points.    Right now it is just
    // 10 * GetMinStep().
    virtual int GetRecommendedNumPoints();

	virtual void ComputeCurvature(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);
	virtual void CurvatureCapping(int iter, const RMCurvatureMethod& method, RMMigrationData* migdata, RMPrepData* data);//DM-FEB2012
	virtual void ComputeCurvatureGradient(const RMSeries& channel, RMPrepData*& data);
	//virtual void ComputeIntervals(RMSeries& channel, double width, RMPrepData* data);		//CORRECTION AUG2009
	virtual void ComputeIntervals(RMMigrationData*& migdata, RMPrepData* data);		//CORRECTION AUG2009
	virtual void RegridMCA(RMMigrationData* migdata, RMPrepData* data);															//ADDITION FEB2010	//DM-FEB2012 Removed & from RMMigrationData*	
	virtual void RegridPBA(RMMigrationData* migdata, RMPrepData* data);															//ADDITION FEB2010  //DM-FEB2012 Removed & from RMMigrationData*
	virtual void RegridClean(RMMigrationData* migdata, RMPrepData* data, int oldnumberpoints);								//DM-FEB2012 Removed & from RMMigrationData*
	virtual void SplinesMCA(RMMigrationData* migData, int ARRAY_WIDTH);															//ADDITION FEB2010   //DM-FEB2012 Removed & from RMMigrationData*
	virtual void SplinesPBA(RMMigrationData* migData, RMPrepData* prepData, int ARRAY_WIDTH);											//ADDITION FEB2010  //DM-FEB2012 Removed & from RMMigrationData*
	virtual void StoreCurrentStreamwiseCoordinate(RMPrepData* data, int oldnumberpoints);											//ADDITION FEB2010
	virtual void ComputeWeightsForInterpolation(RMPrepData* data, int newnumberpoints, int oldnumberofpoints);						//ADDITION FEB2010
	virtual void ComputeTheta(const RMSeries& channel, RMPrepData* data, int);														//DM-FEB2011	

	virtual void CurvatureFilter(const RMSeries& channel, RMPrepData*,RMMigrationData*& migdata);//DM-FEB2011
	virtual bool ApplyCurvatureFilter(RMPrepData* data);

	virtual void ComputeDimensionedSN(int arrayLen, int arrayWidth, double channelWidth, RMPrepData* data);

protected:
    virtual bool InternalCalcCurvature(double at, double& curvature, double& x, double& y);

	virtual void ComputeCurvature_JohannessonParker(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);
	virtual void ComputeCurvature_DT_DS_Backward(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);
	virtual void ComputeCurvature_DT_DS_Central(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);
	virtual void ComputeCurvature_FittingCircle(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);
	virtual void ComputeCurvature_Splines(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data);

    virtual void Cleanup();

protected:
    gsl_spline* m_xSpline;
    gsl_spline* m_ySpline;
    double m_minX, m_minY, m_maxX, m_maxY, m_minI, m_maxI;
    double m_minStep, m_maxStep, m_length;
    int m_numPoints;
    bool m_initialized;
};


#endif//__PREPROCESSING_H___________________20060427093838__
