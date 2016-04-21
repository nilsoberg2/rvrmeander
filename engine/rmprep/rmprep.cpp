#define  _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
#define SPLINE_TYPE gsl_interp_cspline
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>
#include <math.h>
#include "rmprep.h"
#include "../rmdebug/debug_printing.h"


void RMPrepData::InitBase(int length, int width)
{
	curvature.init(length);
	curvaturecopy.init(length);
	dcurvatureds.init(length);
	intervals.init(length);
	coords.init(length);
	coordn.init(width);
	S.init(length);
	N.init(width);
	theta.init(length);

  	// Defining the n-coordinate
	for (int i=0; i <= width-1; i++)
		//coordn(i) = -1.0 + (i) * (2.0 / (width-1));	
		coordn(i) = 1.0 - (i) * (2.0 / (width-1));		//CORRECTION AUG2009

	deltaX.init(length);
	deltaY.init(length);
	delta2X.init(length);
	delta2Y.init(length);
}


void RMPrepData::Init(int length, int width)
{
	InitBase(length, width);
}


RMPrepData::~RMPrepData()
{
}


RMPrep::RMPrep()
{
    m_xSpline = m_ySpline = NULL;
}


bool RMPrep::Initialize(const RMSeries& data, bool reverseOrientation)
{
    TwoDDoubleArray* a = new TwoDDoubleArray();
    a->setobj(data.get_pointer(), data.length);
	bool retval = InitializeVirtual(a, reverseOrientation);
    delete a;
    return retval;
}


bool RMPrep::Initialize(double** xy, int numPoints, bool reverseOrientation)
{
    TwoDDoubleArray* a = new TwoDDoubleArray();
    a->setobj(xy, numPoints);
	bool retval = InitializeVirtual(a, reverseOrientation);
    delete a;
    return retval;
}


bool RMPrep::InitializeVirtual(Virtual2ColArray* data, bool reverseOrientation)
{
    if (data->size() < 4)
        return false;

    Cleanup();

    int numPoints = data->size();

    m_numPoints = numPoints;
    m_length = 0.0;
    
    m_minX = 1e200;
    m_minY = 1e200;
    m_minI = 1e200;
    m_maxX = 1e-200;
    m_maxY = 1e-200;
    m_maxI = 1e-200;

    m_minStep = 1e200;
    m_maxStep = 1e-200;
    double lastX, lastY;
    bool firstIter = true;

    // Now we create our arrays that we'll give to the spline routines.
    double* indexes = new double[numPoints];
    double* xvalues = new double[numPoints];
    double* yvalues = new double[numPoints];
    int startIndex = (reverseOrientation ? numPoints-1 : 0);
    int endIndex = (reverseOrientation ? -1 : numPoints);
    int incVal = (reverseOrientation ? -1 : 1);
    for (int i = startIndex; i != endIndex; i += incVal)
    {
        double x = 0;
        double y = 0;
        data->get(i, x, y);

        int index = (reverseOrientation ? numPoints-i-1 : i);
        xvalues[index] = x;
        yvalues[index] = y;

        if (x > m_maxX) m_maxX = x;
        if (y > m_maxY) m_maxY = y;
        if (x < m_minX) m_minX = x;
        if (y < m_minY) m_minY = y;

        if (firstIter)
        {
            indexes[index] = 0.0;
            m_minI = 0.0;
            m_maxI = 0.0;
            lastX = x;
            lastY = y;
            firstIter = false;
        }
        else
        {
            double len = sqrt( (x-lastX)*(x-lastX) + (y-lastY)*(y-lastY) );
            indexes[index] = indexes[index-1] + len;
            m_maxI += len;
            m_length += len;
            if (len > m_maxStep) m_maxStep = len;
            if (len < m_minStep) m_minStep = len;
            lastX = x;
            lastY = y;
        }
    }

    // Next, we create the spline (first deleting it if it already exists).
	//if (m_xSpline != NULL)
	//	gsl_spline_free(m_xSpline);
    m_xSpline = gsl_spline_alloc(SPLINE_TYPE, numPoints);
    int retval = gsl_spline_init(m_xSpline, indexes, xvalues, numPoints);
    if (retval || m_xSpline == NULL)
    {
        delete[] xvalues;
        delete[] yvalues;
        delete[] indexes;
        return false;
    }

    m_ySpline = gsl_spline_alloc(SPLINE_TYPE, numPoints);
	//if (m_ySpline != NULL)
	//	gsl_spline_free(m_ySpline);
	retval = gsl_spline_init(m_ySpline, indexes, yvalues, numPoints);
    if (retval || m_ySpline == NULL)
    {
        delete[] xvalues;
        delete[] yvalues;
        delete[] indexes;
        return false;
    }

    delete[] indexes;
    delete[] xvalues;
    delete[] yvalues;

    return true;
}



RMPrep::~RMPrep()
{
    Cleanup();
}


void RMPrep::Cleanup()
{
    if (m_xSpline != NULL)
        gsl_spline_free(m_xSpline);
    if (m_ySpline != NULL)
        gsl_spline_free(m_ySpline);
}


bool RMPrep::GetXY(double at, double& x, double& y)
{
    //if (x < m_minX || x > m_maxX || y < m_minY || y > m_maxY)
    if (at < m_minI || at > m_maxI)
        return false;

    int r;
    if (r = gsl_spline_eval_e(m_xSpline, at, 0, &x))
        return false;
    if (r = gsl_spline_eval_e(m_ySpline, at, 0, &y))
        return false;

    return true;
}

double RMPrep::GetX(double at)
{
    if (at < m_minI || at > m_maxI)
        return 0.0;

    int r;
    double x;
    if (r = gsl_spline_eval_e(m_xSpline, at, 0, &x))
        return 0.0;
    else
        return x;
}

double RMPrep::GetY(double at)
{
    if (at < m_minI || at > m_maxI)
        return 0.0;

    int r;
    double y;
    if (r = gsl_spline_eval_e(m_ySpline, at, 0, &y))
        return 0.0;
    else
        return y;
}

bool RMPrep::GetX(double at, double& x)
{
    if (at < m_minI || at > m_maxI)
        return false;

    int r;
    if (r = gsl_spline_eval_e(m_xSpline, at, 0, &x))
        return false;

    return true;
}

bool RMPrep::GetY(double at, double& y)
{
    if (at < m_minI || at > m_maxI)
        return false;

    int r;
    if (r = gsl_spline_eval_e(m_ySpline, at, 0, &y))
        return false;

    return true;
}

bool RMPrep::GetXY(int numPoints, int& xySize, double**& xy, double min, double max)
{
    if (min == 0.0 && max == 0.0)
    {
        min = m_minI;
        max = m_maxI;
    }
																			// -DM- min is 0.0, max is the s coordinate of the downstream node. The code already knows these values.		
    if (numPoints < 2)
        return false;														// -DM- Need at least two nodes.	

    xy = new double*[numPoints];											

    bool ok = true;
    double at = min;														// -DM- moving s coordinate
    double inc = (max - min) / ((double)numPoints - 1.0);					// -DM- computation of the uniform increment

    int curIndex = 0;														// -DM- integer node counter
    int sr = 0;																
    //while (at <= max)						//CORRECTION AUG2009			// -DM- the orinal while allowed for modifying the number of nodes 
    //{										//CORRECTION AUG2009
	for(int i = 0; i <=numPoints-1; i++)	//CORRECTION AUG2009			// -DM- this modification ensures that the number of nodes is maintained
	{										//CORRECTION AUG2009
        double x, y;
        if (sr = gsl_spline_eval_e(m_xSpline, at, 0, &x))					// -DM- get x coordinate of the node
        {
            ok = false;
            break;
        }
        if (sr = gsl_spline_eval_e(m_ySpline, at, 0, &y))					// -DM- get y coordinate of the node
        {
            ok = false;
            break;
        }

        xy[curIndex] = new double[3];										
        xy[curIndex][0] = x;												// -DM- assign x to xy matrix
        xy[curIndex][1] = y;												// -DM- assign y to xy matrix
        xy[curIndex][2] = at;												// -DM- assign s coordinate to xy matrix

        curIndex++;															// -DM- increment integer node counter
        at += inc;															// -DM- increment moving s coordinate
		if (at > max) {at = max;}			//CORRECTION AUG2009 
	}										//CORRECTION AUG2009
    //}										//CORRECTION AUG2009

    if (! ok)
    {
        for (int i = 0; i < curIndex; i++)
            delete[] xy[i];
        delete[] xy;
        xy = NULL;
        return false;
    }

    xySize = curIndex;			// -DM- Return the number of nodes

	return true;
}

void RMPrep::FreeXY(int numPoints, double**& xy)
{
    for (int i = 0; i < numPoints; i++)
        delete[] xy[i];
    delete[] xy;
    xy = NULL;
}


bool RMPrep::GetXY3(int numPoints, int& xySize, double**& xy, double**& xy2, double**& xy3, double min, double max)
{
    if (min == 0.0 && max == 0.0)
    {
        min = m_minI;
        max = m_maxI;
    }

    if (numPoints < 2)
        return false;

    xy = new double*[numPoints];
    xy2 = new double*[numPoints];
    xy3 = new double*[numPoints];

    bool ok = true;
    double at = min;
    double inc = (max - min) / ((double)numPoints - 1);
    int curIndex = 0;
    int sr = 0;
    while (at <= max)
    {
        double x, y;
        if (sr = gsl_spline_eval_e(m_xSpline, at, 0, &x))
        {
            ok = false;
            break;
        }
        if (sr = gsl_spline_eval_e(m_ySpline, at, 0, &y))
        {
            ok = false;
            break;
        }

        xy[curIndex] = new double[2];
        xy[curIndex][0] = x;
        xy[curIndex][1] = y;
        xy2[curIndex] = new double[2];
        xy2[curIndex][0] = x;
        xy2[curIndex][1] = y;
        xy3[curIndex] = new double[2];
        xy3[curIndex][0] = x;
        xy3[curIndex][1] = y;

        curIndex++;
        at += inc;
    }

    if (! ok)
    {
        for (int i = 0; i < curIndex; i++)
            delete[] xy[i];
        delete[] xy;
        xy = NULL;
        return false;
    }

    xySize = curIndex;

    return true;
}


bool RMPrep::InternalCalcCurvature(double at, double& curvature, double& x, double& y)
{
    double xp, xpp, yp, ypp;
    int r;
    if (r = gsl_spline_eval_e(m_xSpline, at, 0, &x))
        return false;
    if (r = gsl_spline_eval_deriv_e(m_xSpline, at, 0, &xp))
        return false;
    if (r = gsl_spline_eval_deriv2_e(m_xSpline, at, 0, &xpp))
        return false;
    if (r = gsl_spline_eval_e(m_ySpline, at, 0, &y))
        return false;
    if (r = gsl_spline_eval_deriv_e(m_ySpline, at, 0, &yp))
        return false;
    if (r = gsl_spline_eval_deriv2_e(m_ySpline, at, 0, &ypp))
        return false;

	
    curvature = -(xp*ypp - yp*xpp) / pow(xp*xp + yp*yp, 1.5);
	//logmsg("xp= %lf	ypp= %lf	yp= %lf	xpp= %lf \n", xp,ypp,yp,xpp);
	//logmsg("%lf \n", curvature);
    
    return true;
}

double RMPrep::GetCurvatureAt(double at)
{
    if (at < m_minI || at > m_maxI)
        return 0.0;
  
    double curvature, x, y;
    if (InternalCalcCurvature(at, curvature, x, y))
        return curvature;
    else
        return 0.0;
}

bool RMPrep::GetCurvatureAt(double at, double& curvature)
{
    if (at < m_minI || at > m_maxI)
        return false;

    double x, y;
    return InternalCalcCurvature(at, curvature, x, y);
}


bool RMPrep::GetCurvature(int numPoints, int& curvSize, double**& curvature)
{
    double** xy; // temp
    bool result = GetCurvature(numPoints, curvSize, curvature, xy);
	delete[] xy;
	return result;
}

bool RMPrep::GetCurvature(int numPoints, int& curvSize, double**& curvature, double**& xy)
{
    curvature = new double*[numPoints];
    xy = new double*[numPoints];

    for (int i = 0; i < numPoints; i++)
    {
        curvature[i] = new double[2];
        xy[i] = new double[2];
    }

    TwoDDoubleArray* a_xy = new TwoDDoubleArray();
    a_xy->setobj(xy, numPoints);

    TwoDDoubleArray* a_c = new TwoDDoubleArray();
    a_c->setobj(curvature, numPoints);

    bool status = GetCurvaturePreCreated(a_c, a_xy, curvSize);
    
    if (!status)
    {
        for (int i = 0; i < numPoints; i++)
        {
            delete[] curvature;
            delete[] xy;
        }

        delete[] curvature;
        delete[] xy;
    }

    return status;
}

bool RMPrep::GetCurvaturePreCreated(Virtual2ColArray* curvature, Virtual2ColArray* xy, int& curvSize)
{
    int numPoints = curvature->size();

    if (numPoints < 2)
        return false;

    bool ok = true;
    double at = m_minI;
    double inc = (m_maxI - m_minI) / ((double)numPoints - 1);
    int curIndex = 0;
    while (at <= m_maxI)
    {
        double curv, x, y;
        if ((ok = InternalCalcCurvature(at, curv, x, y)) == false)
            break;
        curvature->set(curIndex, at, curv);
        xy->set(curIndex, x, y);
        curIndex++;
        at += inc;
    }

    if (! ok)
        return false;

    curvSize = curIndex;

    return true;
}


void RMPrep::GetRange(double& minX, double& minY, double& maxX, double& maxY, double& minI, double& maxI)
{
    minX = m_minX;
    minY = m_minY;
    maxX = m_maxX;
    maxY = m_maxY;
    minI = m_minI;
    maxI = m_maxI;
}

void RMPrep::GetXRange(double& minX, double& maxX)
{
    minX = m_minX;
    maxX = m_maxX;
}

void RMPrep::GetYRange(double& minY, double& maxY)
{
    minY = m_minY;
    maxY = m_maxY;
}

void RMPrep::GetIRange(double& minI, double& maxI)
{
    minI = m_minI;
    maxI = m_maxI;
}

double RMPrep::GetMinStep()
{
    return m_minStep;
}

double RMPrep::GetMaxStep()
{
    return m_maxStep;
}

double RMPrep::GetLength()
{
    return m_length;
}

int RMPrep::GetRecommendedNumPoints()
{
    //return m_numPoints * 10;
    if (m_minStep > 0.0)
        return ( (int)(2.0*m_length/m_minStep) + 1 );
    else
        return 0;
}

void RMPrep::ComputeCurvature_JohannessonParker(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{
	double dssSum;
	double dssSumPow;

	// All nodes except first and last ones
	for (int i = 1; i <= channel.length-2; i++)
	{
		dssSum = data->intervals(i) + data->intervals(i+1);
		dssSumPow = pow(0.5 * dssSum, 2.0);
		data->deltaX(i)  = (channel(i+1,0)-channel(i-1,0)) / dssSum;
		data->deltaY(i)  = (channel(i+1,1)-channel(i-1,1)) / dssSum;
		data->delta2X(i) = (channel(i+1,0)+channel(i-1,0) - 2.0*channel(i,0)) / dssSumPow;
		data->delta2Y(i) = (channel(i+1,1)+channel(i-1,1) - 2.0*channel(i,1)) / dssSumPow;
		data->curvature(i) = -(data->deltaX(i)*data->delta2Y(i)-data->deltaY(i)*data->delta2X(i));
	}

	// First node
	if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_First)
	{	
		if (iter == 1)
		{
			data->curvature(0)=data->C_0;
		}
		else
		{
			double mco = (data->curvature(2)-data->curvature(1)) / data->intervals(2);
			data->curvature(0) = data->curvature(1) - mco * data->intervals(1);
			data->C_0 = data->curvature(0);//DM-FEB2011
		}
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_Always)
	{
		data->curvature(0)=data->C_0;
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_Constant_Zero)
	{
		data->curvature(0)=0.0;	
		data->C_0 = 0.0;//DM-FEB2011
	}



	// Last node
	if (method.DownstreamCurvatureMethod == Curvature_Extrapolated) //DM-FEB2011
	{
		//data->curvature(channel.length-1) = -(data->theta(channel.length-1)-data->theta(channel.length-2))/data->intervals(channel.length-1);			//DM-FEB2011    Commented out
		double mcurvature = (data->curvature(channel.length-2)-data->curvature(channel.length-3)) / data->intervals(channel.length-2);					//DM-FEB2011 
		data->curvature(channel.length-1) = data->curvature(channel.length-2) + mcurvature * data->intervals(channel.length-1);							//DM-FEB2011 
	}
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Constant_Zero)
	{
		data->curvature(channel.length-1) = 0.0;
	}	
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Zero_Gradient)			//DM-FEB2011
	{																							//DM-FEB2011	
		data->curvature(channel.length-1) = data->curvature(channel.length-2);					//DM-FEB2011
	}																							//DM-FEB2011

}

void RMPrep::ComputeCurvature_DT_DS_Backward(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{
	// All nodes except first and last ones
	for (int i = 1; i <= channel.length-2; i++)
	{
		data->curvature(i) = -(data->theta(i)-data->theta(i-1))/data->intervals(i);
	}

	// First node
	if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_First)
	{	
		if (iter == 1)
		{
			data->curvature(0)=data->C_0;
		}
		else
		{
			double mco = (data->curvature(2)-data->curvature(1)) / data->intervals(2);
			data->curvature(0) = data->curvature(1) - mco * data->intervals(1);
			data->C_0 = data->curvature(0);//DM-FEB2011
		}
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_Always)
	{
		data->curvature(0)=data->C_0;
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_Constant_Zero)
	{
		data->curvature(0)=0.0;
		data->C_0 = 0.0;//DM-FEB2011
	}

	// Last node
	if (method.DownstreamCurvatureMethod == Curvature_Extrapolated) //DM-FEB2011
	{
		//data->curvature(channel.length-1) = -(data->theta(channel.length-1)-data->theta(channel.length-2))/data->intervals(channel.length-1);			//DM-FEB2011    Commented out
		double mcurvature = (data->curvature(channel.length-2)-data->curvature(channel.length-3)) / data->intervals(channel.length-2);					//DM-FEB2011 
		data->curvature(channel.length-1) = data->curvature(channel.length-2) + mcurvature * data->intervals(channel.length-1);							//DM-FEB2011 
	}
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Constant_Zero)
	{
		data->curvature(channel.length-1) = 0.0;					
	}	
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Zero_Gradient)			//DM-FEB2011
	{																							//DM-FEB2011
		data->curvature(channel.length-1) = data->curvature(channel.length-2);					//DM-FEB2011
	}																							//DM-FEB2011
}

void RMPrep::ComputeCurvature_DT_DS_Central(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{
	// All nodes except first and last ones
	for (int i = 1; i <= channel.length-2; i++)
		{
			data->curvature(i) = -(data->theta(i+1)-data->theta(i-1))/(data->intervals(i+1)+data->intervals(i));
		}

	// First node
	if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_First)
	{	
		if (iter == 1)
		{
			data->curvature(0)=data->C_0;
		}
		else
		{
			double mco = (data->curvature(2)-data->curvature(1)) / data->intervals(2);
			data->curvature(0) = data->curvature(1) - mco * data->intervals(1);
			data->C_0 = data->curvature(0);//DM-FEB2011
		}
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_Always)
	{
		data->curvature(0)=data->C_0;
	}
	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_Constant_Zero)
	{
		data->curvature(0)=0.0;		
		data->C_0 = 0.0;//DM-FEB2011
	}

	// Last node
	if (method.DownstreamCurvatureMethod == Curvature_Extrapolated) //DM-FEB2011
	{	
		//data->curvature(channel.length-1) = -(data->theta(channel.length-1)-data->theta(channel.length-2))/data->intervals(channel.length-1);			//DM-FEB2011    Commented out
		double mcurvature = (data->curvature(channel.length-2)-data->curvature(channel.length-3)) / data->intervals(channel.length-2);					//DM-FEB2011 
		data->curvature(channel.length-1) = data->curvature(channel.length-2) + mcurvature * data->intervals(channel.length-1);							//DM-FEB2011 
	}
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Constant_Zero)
	{
		data->curvature(channel.length-1) = 0.0;					
	}	
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Zero_Gradient)			//DM-FEB2011
	{																							//DM-FEB2011
		data->curvature(channel.length-1) = data->curvature(channel.length-2);					//DM-FEB2011
	}																							//DM-FEB2011
}

void RMPrep::ComputeCurvature_FittingCircle(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{
	//DM-FEB2011 The Johannesson and Parker method is here used just to get the sign of the curvature
	// The module of the curvature is found with the "fitting circle" method 
	double* curvaturesign = NULL;									//DM-FEB2011	
	curvaturesign  = new double[channel.length];					//DM-FEB2011
	ComputeCurvature_JohannessonParker(iter,method,channel,data);	//DM-FEB2011
	for (int i = 0; i <= channel.length-1; i++)						//DM-FEB2011				
		{															//DM-FEB2011
			if(data->curvature(i) >= 0.0)							//DM-FEB2011
				{													//DM-FEB2011
					curvaturesign[i] = 1.0;							//DM-FEB2011
				}													//DM-FEB2011
			else													//DM-FEB2011
				{													//DM-FEB2011
					curvaturesign[i] = -1.0;						//DM-FEB2011
				}													//DM-FEB2011
		}															//DM-FEB2011		

	// All nodes except first and last ones
	for (int i = 1; i <= channel.length-2; i++)
		{
			double xcenter;
			double ycenter;
			double ma;
			double mb;
			double R;
			ma = (channel(i,1)-channel(i-1,1))/(channel(i,0)-channel(i-1,0));
			mb = (channel(i+1,1)-channel(i,1))/(channel(i+1,0)-channel(i,0));
			xcenter = (ma*mb*(channel(i-1,1)-channel(i+1,1))+mb*(channel(i-1,0)+channel(i,0))-ma*(channel(i,0)+channel(i+1,0)))/2/(mb-ma);                         
			ycenter = -1/ma*(xcenter-(channel(i-1,0)+channel(i,0))/2)+(channel(i-1,1)+channel(i,1))/2;
			R = sqrt((channel(i,0)-xcenter)*(channel(i,0)-xcenter)+(channel(i,1)-ycenter)*(channel(i,1)-ycenter));   
			//if (data->theta(i)>data->theta(i-1))					//DM-FEB2011		Commented out
			//	{													//DM-FEB2011		Commented out
			//		data->curvature(i)=-1/R;						//DM-FEB2011		Commented out
			//	}													//DM-FEB2011		Commented out
			//else													//DM-FEB2011		Commented out
			//	{													//DM-FEB2011		Commented out
			//		data->curvature(i)=1/R;							//DM-FEB2011		Commented out
			//	}													//DM-FEB2011		Commented out
			data->curvature(i)=curvaturesign[i]*1/R;				//DM-FEB2011	
		}

	delete [] curvaturesign; 										//DM-FEB2011	
	curvaturesign = NULL;											//DM-FEB2011	

	// First node
	if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_First)
		{	
			if (iter == 1)
				{
					data->curvature(0)=data->C_0;
				}
			else
				{
					double mco = (data->curvature(2)-data->curvature(1)) / data->intervals(2);
					data->curvature(0) = data->curvature(1) - mco * data->intervals(1);
					data->C_0 = data->curvature(0);//DM-FEB2011
				}
		}

	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_C_0_Always)
		{
			data->curvature(0)=data->C_0;
		}

	else if (method.UpstreamCurvatureMethod == Curvature_Upstream_Constant_Zero)
		{
			data->curvature(0)=0.0;	
			data->C_0 = 0.0;//DM-FEB2011
		}

    // Last node
	if (method.DownstreamCurvatureMethod == Curvature_Extrapolated) //DM-FEB2011
		{
		//data->curvature(channel.length-1) = -(data->theta(channel.length-1)-data->theta(channel.length-2))/data->intervals(channel.length-1);			//DM-FEB2011    Commented out
		double mcurvature = (data->curvature(channel.length-2)-data->curvature(channel.length-3)) / data->intervals(channel.length-2);					//DM-FEB2011 
		data->curvature(channel.length-1) = data->curvature(channel.length-2) + mcurvature * data->intervals(channel.length-1);							//DM-FEB2011 
		}	

	if (method.DownstreamCurvatureMethod == Curvature_Downstream_Constant_Zero)
		{
			data->curvature(channel.length-1) = 0.0;					
		}
	else if (method.DownstreamCurvatureMethod == Curvature_Downstream_Zero_Gradient)			//DM-FEB2011
		{																						//DM-FEB2011
		data->curvature(channel.length-1) = data->curvature(channel.length-2);					//DM-FEB2011
		}																						//DM-FEB2011
}

void RMPrep::ComputeCurvature_Splines(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{	
	RMPrep prep;
	prep.Initialize(channel);
     
	for (int i = 0; i <= channel.length-1; i++)
		{
			data->curvature(i) = prep.GetCurvatureAt(data->coords(i));
		}
}

void RMPrep::ComputeCurvature(int iter, const RMCurvatureMethod& method, const RMSeries& channel, RMPrepData* data)
{
	
	// Curvature method 1
	if (method.CurvatureMethod == Curvature_JohannessonParker)
		ComputeCurvature_JohannessonParker(iter, method, channel, data);

	// Curvature method 2	
	else if (method.CurvatureMethod == Curvature_DT_DS_Backward)				
		ComputeCurvature_DT_DS_Backward(iter, method, channel, data);

	// Curvature method 3
	else if (method.CurvatureMethod == Curvature_DT_DS_Central)
		ComputeCurvature_DT_DS_Central(iter, method, channel, data);

	// Curvature method 4
	else if (method.CurvatureMethod == Curvature_FittingCircle)
		ComputeCurvature_FittingCircle(iter, method, channel, data);

	// Curvature method 5
	else if (method.CurvatureMethod == Curvature_Splines)
		ComputeCurvature_Splines(iter, method, channel, data);
    
	//DEBUG_PRINT3_ARRAY(channel.length-1, sCurvatureTemp, data->coords, data->curvature);		//CORRECTION AUG2009
}

void RMPrep::CurvatureFilter(const RMSeries& channel, RMPrepData* data, RMMigrationData*& migdata)//DM-FEB2011
{
	logmsg("Filter on Curvature\n");

	for (int i = 0; i <= channel.length-1; i++)
		{	
			data->curvaturecopy(i) = data->curvature(i);
		}

	if(migdata->curvMethod.UpstreamCurvatureMethod == 1)data->curvature(0) = (data->curvaturecopy(0) + 2 * data->curvaturecopy(0) + data->curvaturecopy(1))/4;//DM-FEB2011

	data->curvature(channel.length-1) = (data->curvaturecopy(channel.length-2) + 2 * data->curvaturecopy(channel.length-1) + data->curvaturecopy(channel.length-1))/4;

	for (int i = 1; i <= channel.length-2; i++)
		{	
			data->curvature(i) = (data->curvaturecopy(i-1) + 2 * data->curvaturecopy(i) + data->curvaturecopy(i+1))/4;
		}
}

bool RMPrep::ApplyCurvatureFilter(RMPrepData* data)
{
	return data->FilterCurvature;
}

void RMPrep::ComputeCurvatureGradient(const RMSeries& channel, RMPrepData*& data)
{	
	/*		//DM-FEB2011	Commented out
	data->dcurvatureds(0)=(data->curvature(1)-data->curvature(0))/data->intervals(1);

	for (int i = 1; i <= channel.length-1; i++)
	{
		data->dcurvatureds(i)=(data->curvature(i)-data->curvature(i-1))/data->intervals(i);
		//DEBUG_PRINT3(sCurvatureGradientTemp, i+1, data->coords(i), data->dcurvatureds(i));		//CORRECTION AUG2009 
	}
	*/		//DM-FEB2011	Commented out

	//DM-FEB2011
	data->dcurvatureds(0)=(data->curvature(1)-data->curvature(0))/data->intervals(1);

	for (int i = 1; i <= channel.length-2; i++)
		{
			data->dcurvatureds(i)=(data->curvature(i+1)-data->curvature(i-1))/(data->intervals(i)+data->intervals(i+1));
		}

	data->dcurvatureds(channel.length-1)=(data->curvature(channel.length-1)-data->curvature(channel.length-2))/data->intervals(channel.length-1);
}

void RMPrep::ComputeIntervals(RMMigrationData*& migdata, RMPrepData* data)			//CORRECTION AUG2009
{
	RMSeries channel = migdata->channel.centerline;
	//int newnumberpoints, oldnumberpoints;										//ADDITION FEB2010		

	// Defining intervals and total streamwise length
	data->intervals(0) = 0.0;				
	data->MaxInterval = data->intervals(0);														//CORRECTION FEB2009
	data->MinInterval = 9999.0;//DM-FEB2011
	data->sSum = 0.0;
	data->coords(0) = 0.0;
	//DEBUG_PRINT3(sIntervalTemp, 1, data->coords(0), data->intervals(0));					//CORRECTION AUG2009
	for (int i = 1; i <= channel.length-1; i++)
	{
		data->intervals(i) = sqrt(pow((channel(i,0)-channel(i-1,0)),2.0)+pow((channel(i,1)-channel(i-1,1)),2.0));
		if (data->intervals(i) > data->MaxInterval) {data->MaxInterval = data->intervals(i);}			//CORRECTION AUG2009
		if (data->intervals(i) < data->MinInterval) {data->MinInterval = data->intervals(i);}			//DM-FEB2011
		data->sSum = data->sSum + data->intervals(i);
		data->coords(i) = data->sSum;
		//logmsg("************ %d	%lf	%lf \n", i, channel(i,0), channel(i,1));
		//DEBUG_PRINT3(sIntervalTemp, i+1, data->coords(i), data->intervals(i));			//CORRECTION AUG2009
	}

	data->MaxInterval = data->MaxInterval * migdata->channel.WIDTH/2;													//CORRECTION AUG2009
	data->MinInterval = data->MinInterval * migdata->channel.WIDTH/2;	//DM-FEB2011												
	//logmsg("	Max ds (m)= %lf, ds/B = %lf \n", data->MaxInterval, data->MaxInterval/(migdata->channel.WIDTH/2.0));			//CORRECTION AUG2009   
	//logmsg("	Number of nodes in the streamwise direction = %d \n", channel.length);																	
}

void RMPrep::RegridMCA(RMMigrationData* migdata, RMPrepData* data)		//ADDITION FEB2010    //DM-FEB2012 Removed & from RMMigrationData*
{
	auto channel = &(migdata->channel.centerline);
	int newnumberpoints;
	
	logmsg("Regridding\n");

	newnumberpoints = data->sSum / migdata->channel.threshold_regrid;	//DM-FEB2011

	// Interpolate on splines here.
	RMPrep prep;
	prep.Initialize(*channel);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(newnumberpoints, size, xy);				
					
    // Nils changed this 2/16/2011 from channel.width (width of array) to migdata->channel.nnodes
	migdata->Init(size, migdata->channel.nnodes);			
	channel->init(size, 2, xy);

    FreeXY(size, xy);

	//auto channel = &(migdata->channel.centerline);
	//int channelLength = migdata->channel.centerline.length;
	int channelLength = channel->length;
	
	data->intervals.init(channelLength);
	data->coords.init(channelLength);
	data->theta.init(channelLength);
	data->dcurvatureds.init(channelLength);
	data->curvature.init(channelLength);
	data->curvaturecopy.init(channelLength);
	data->S.init(channelLength);
	data->deltaX.init(channelLength);
	data->deltaY.init(channelLength);
	data->delta2X.init(channelLength);
	data->delta2Y.init(channelLength);
				
	data->sSum = 0.0;
	data->coords(0) = 0.0;	
	data->intervals(0) = 0.0;	
	data->MaxInterval = data->intervals(0);
	data->MinInterval = 9999.0;//DM-FEB2011
	for (int i = 1; i <= channelLength - 1; i++)
		{	
			data->intervals(i) = sqrt(pow(((*channel)(i, 0) - (*channel)(i - 1, 0)), 2.0) + pow(((*channel)(i, 1) - (*channel)(i - 1, 1)), 2.0));
			data->sSum = data->sSum + data->intervals(i);
			data->coords(i) = data->sSum;	
			if (data->intervals(i) > data->MaxInterval) {data->MaxInterval = data->intervals(i);}	
			if (data->intervals(i) < data->MinInterval) {data->MinInterval = data->intervals(i);}//DM-FEB2011
		}
	data->MaxInterval = data->MaxInterval * migdata->channel.WIDTH/2;	
	data->MinInterval = data->MinInterval * migdata->channel.WIDTH/2;	//DM-FEB2011
	//logmsg("	Updated Max ds (m)= %lf, ds/B = %lf \n", data->MaxInterval, data->MaxInterval/(migdata->channel.WIDTH/2));
	//logmsg("	Updated Number of nodes in the streamwise direction = %d \n", channel.length);												
	//migdata->channel.centerline = channel;															

}

void RMPrep::RegridPBA(RMMigrationData* migdata, RMPrepData* data)		//ADDITION FEB2010   //DM-FEB2012 Removed & from RMMigrationData*
{
	RMSeries channel = migdata->channel.centerline;
	int newnumberpoints, oldnumberpoints;

	logmsg("Regridding\n");


	oldnumberpoints = channel.length;										
	StoreCurrentStreamwiseCoordinate(data, oldnumberpoints);	

	
	newnumberpoints = data->sSum / migdata->channel.threshold_regrid;  //DM-FEB2011					

	RMPrep prep;
	prep.Initialize(channel);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(newnumberpoints, size, xy);				
					
    // Nils changed this 2/16/2011 from channel.width (width of array) to
    // migdata->channel.nnodes.
	migdata->Init(size, migdata->channel.nnodes);			
	channel.init(size, 2, xy);

    FreeXY(size, xy);
					
	data->intervals.init(channel.length);
	data->coords.init(channel.length);
	data->theta.init(channel.length);
	data->dcurvatureds.init(channel.length);
	data->curvature.init(channel.length);
	data->curvaturecopy.init(channel.length);
	data->S.init(channel.length);
	data->deltaX.init(channel.length);
	data->deltaY.init(channel.length);
	data->delta2X.init(channel.length);
	data->delta2Y.init(channel.length);
				
	data->sSum = 0.0;
	data->coords(0) = 0.0;	
	data->intervals(0) = 0.0;	
	data->MaxInterval = data->intervals(0);
data->MinInterval = 9999.0;//DM-FEB2011


	for (int i = 1; i <= channel.length-1; i++)
		{	
			data->intervals(i) = sqrt(pow((channel(i,0)-channel(i-1,0)),2.0)+pow((channel(i,1)-channel(i-1,1)),2.0));
			data->sSum = data->sSum + data->intervals(i);
			data->coords(i) = data->sSum;	
			if (data->intervals(i) > data->MaxInterval) {data->MaxInterval = data->intervals(i);}
			if (data->intervals(i) < data->MinInterval) {data->MinInterval = data->intervals(i);}//DM-FEB2011
		}

	data->MaxInterval = data->MaxInterval * migdata->channel.WIDTH/2;
	data->MinInterval = data->MinInterval * migdata->channel.WIDTH/2;//DM-FEB2011
	//logmsg("	Updated Max ds (m)= %lf, ds/B = %lf \n", data->MaxInterval, data->MaxInterval/(migdata->channel.WIDTH/2));
	//logmsg("	Updated Number of nodes in the streamwise direction = %d \n", channel.length);			
	migdata->channel.centerline = channel;	
	ComputeWeightsForInterpolation(data, newnumberpoints, oldnumberpoints);	
}

void RMPrep::SplinesMCA(RMMigrationData* migData, int ARRAY_WIDTH)		//ADDITION FEB2010   //DM-FEB2012 Removed & from RMMigrationData*
{

/*	
	RMPrep prep;
	prep.Initialize(migData->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migData->channel.centerline.length, size, xy);				

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
*/
	/*
	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			logmsg("A %d	%lf	%lf \n", i, migData->channel.centerline(i,0), migData->channel.centerline(i,1));
		}
	*/

	RMPrep prep;
	prep.Initialize(migData->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migData->channel.centerline.length+1, size, xy);			// Splines with one more node are generated		

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);

	/*
	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			logmsg("B %d	%lf	%lf \n", i, migData->channel.centerline(i,0), migData->channel.centerline(i,1));
		}
	*/


	prep.Initialize(migData->channel.centerline);

	size = 0;
	xy = NULL;
	prep.GetXY(migData->channel.centerline.length-1, size, xy);			// Splines with one less node are generated, so overall the 
																		// number of nodes before this routine is called is maintained

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);

	/*
	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			logmsg("C %d	%lf	%lf \n", i, migData->channel.centerline(i,0), migData->channel.centerline(i,1));
		}
		*/
}

void RMPrep::SplinesPBA(RMMigrationData* migData, RMPrepData* prepData, int ARRAY_WIDTH)		//ADDITION FEB2010   //DM-FEB2012 Removed & from RMMigrationData*
{
	int newnumberpoints, oldnumberpoints;



/////////////////////////////////////////////////////////////////////
	/*	for (int i = 1; i <= migData->channel.centerline.length-1; i++)
		{
logmsg("x	%lf	y	%lf	\n", migData->channel.centerline(i,0),	migData->channel.centerline(i,1));
		}*/
////////////////////////////////////////////////////////////////////

	oldnumberpoints = migData->channel.centerline.length;										
	StoreCurrentStreamwiseCoordinate(prepData, oldnumberpoints);	

/*
	RMPrep prep;
	prep.Initialize(migData->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migData->channel.centerline.length, size, xy);	

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
*/

/////////////////////////////////////////////////////////////////////
/*logmsg("BEFOREEEEEE\n");
		for (int i = 1; i <= migData->channel.centerline.length-1; i++)
		{
logmsg("i	%d	x	%lf	y	%lf	\n", i, migData->channel.centerline(i,0),	migData->channel.centerline(i,1));
		}*/
////////////////////////////////////////////////////////////////////

	

	RMPrep prep;
	prep.Initialize(migData->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migData->channel.centerline.length+1, size, xy);			// Splines with one more node are generated		

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);


	prep.Initialize(migData->channel.centerline);

	size = 0;
	xy = NULL;
	prep.GetXY(migData->channel.centerline.length-1, size, xy);			// Splines with one less node are generated, so overall the 
																		// number of nodes before this routine is called is maintained

	migData->Init(size, ARRAY_WIDTH);			
	migData->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);

	newnumberpoints = migData->channel.centerline.length;	




	///////////////////////
	prepData->sSum = 0.0;
	prepData->coords(0) = 0.0;	
	prepData->intervals(0) = 0.0;	
	prepData->MaxInterval = prepData->intervals(0);
	prepData->MinInterval = 9999.0;//DM-FEB2011
	for (int i = 1; i <= migData->channel.centerline.length-1; i++)
		{	
			prepData->intervals(i) = sqrt(pow((migData->channel.centerline(i,0)-migData->channel.centerline(i-1,0)),2.0)+pow((migData->channel.centerline(i,1)-migData->channel.centerline(i-1,1)),2.0));
			prepData->sSum = prepData->sSum + prepData->intervals(i);
			prepData->coords(i) = prepData->sSum;	
			if (prepData->intervals(i) > prepData->MaxInterval) {prepData->MaxInterval = prepData->intervals(i);}	
			if (prepData->intervals(i) < prepData->MinInterval) {prepData->MinInterval = prepData->intervals(i);}//DM-FEB2011
		}
					
	prepData->MaxInterval = prepData->MaxInterval * migData->channel.WIDTH/2;
	prepData->MinInterval = prepData->MinInterval * migData->channel.WIDTH/2;//DM-FEB2011

	//logmsg("	Updated Max ds (m)= %lf, ds/B = %lf \n", prepData->MaxInterval, prepData->MaxInterval/(migData->channel.WIDTH/2));	
	//logmsg("	Updated Number of nodes in the streamwise direction = %d \n", migData->channel.centerline.length);	   

		for (int i = 1; i <= migData->channel.centerline.length-1; i++)
		{	
			//logmsg("SPL i = %d	data->coords(i) = %lf	data->oldcoords(i) = %lf \n", i, prepData->coords(i), prepData->oldcoords(i));		
		}
	///////////////////////////////




	ComputeWeightsForInterpolation(prepData, newnumberpoints, oldnumberpoints);	





}

void RMPrep::StoreCurrentStreamwiseCoordinate(RMPrepData* data, int oldnumberpoints)																//ADDITION FEB2010
{
	data->oldcoords.init(oldnumberpoints);									
    for (int i = 0; i <= oldnumberpoints - 1; i++)						
		{																	
			data->oldcoords(i)= data->coords(i);
		}	
}

void RMPrep::ComputeWeightsForInterpolation(RMPrepData* data, int newnumberpoints, int oldnumberpoints)																//ADDITION FEB2010
{



	data->wInterpolationDS.init(newnumberpoints);	




	for(int i=0; i <= newnumberpoints-1; i++)																			
		{ 		
			if(i == 0)
				{
					data->inodeOldXsDS[i] = 1;
				}
			else
				{
					data->inodeOldXsDS[i] = data->inodeOldXsDS[i-1];
				}		
																																		
			while((data->coords(i) > data->oldcoords(data->inodeOldXsDS[i])) && (data->inodeOldXsDS[i] < oldnumberpoints - 1))		
				{																											
					data->inodeOldXsDS[i] = data->inodeOldXsDS[i] + 1;																
				}																										
					
			data->wInterpolationDS(i) = (data->coords(i)-data->oldcoords(data->inodeOldXsDS[i]-1))/(data->oldcoords(data->inodeOldXsDS[i])-data->oldcoords(data->inodeOldXsDS[i]-1));			
	
			if (data->wInterpolationDS(i)>1)																			
				{																										
					data->wInterpolationDS(i)=1.0;																	
				}		
			//logmsg("WEIGHTS i = %d, data->inodeOldXsDS[i] = %d, data->wInterpolationDS(i) = %lf \n", i, data->inodeOldXsDS[i], data->wInterpolationDS(i));
		}


}

void RMPrep::ComputeTheta(const RMSeries& channel, RMPrepData* data, int mesh_gen_method)
{
	for (int i = 0; i <= channel.length-2; i++)
	{
		double tanx = (channel(i+1,1)-channel(i,1))/(channel(i+1,0)-channel(i,0));
		
		//DM-FEB2011			I commented out this portion that calculates theta between -PI and PI	
		/*if ((channel(i+1,0)-channel(i,0))>=0.0)
		{
			data->theta(i) = atan(tanx);
		}
		else
		{
			if ((channel(i+1,1)-channel(i,1))>=0.0)
			{
				data->theta(i) = M_PI + atan(tanx);
			}
			else
			{
				data->theta(i) = -M_PI + atan(tanx);
			}
		}*/
		//DM-FEB2011
		

		//DM-FEB2011		    This portion calculates theta between 0 and 2PI
		if ((channel(i+1,0)-channel(i,0))>=0.0 && (channel(i+1,1)-channel(i,1))>=0.0)		// Quadrant I
		{
			data->theta(i) = atan(tanx);
		}
		else if ((channel(i+1,0)-channel(i,0))>=0.0 && (channel(i+1,1)-channel(i,1))<0.0)	// Quadrant IV
		{
			data->theta(i) = atan(tanx) + 2*M_PI;
		}
		else																				// Quadrants III and II
		{
			data->theta(i) = M_PI + atan(tanx);
		}

		//DM-FEB2011

		//DEBUG_PRINT3(sThetaTemp, i+1, data->coords(i), data->theta(i));			//CORRECTION AUG2009
	}

	
	//DM-FEB2011  Here theta is obtained by considering the bisecting angles of formerly calculated values at nodes i and i-1	
	if(mesh_gen_method == 2)
	{
		RMSeries T(data->theta);//DM-FEB2011

		for (int i = 1; i <= channel.length-2; i++)
		{
			if((T(i-1) > 0 && T(i-1) < M_PI/2 && T(i) > 3*M_PI/2 && T(i) < 2*M_PI) || (T(i-1) > 3*M_PI/2  && T(i-1) < 2*M_PI && T(i) > 0 && T(i) < M_PI/2))				//From Quadrant I to IV or viceversa
				{
					data->theta(i) = (T(i)+ T(i-1))/2 + M_PI;
				}
			else if((T(i-1) > M_PI/2 && T(i-1) < M_PI && T(i) > 3*M_PI/2 && T(i) < 2*M_PI) || (T(i-1) > 3*M_PI/2  && T(i-1) < 2*M_PI && T(i) > M_PI/2 && T(i) < M_PI))	//From Quadrant II to IV or viceversa
				{
					if(T(i-1) > M_PI/2 && T(i-1) < M_PI && T(i) > 3*M_PI/2 && T(i) < 2*M_PI)				//From Quadrant II to IV
						{
							if(T(i)>T(i-1)+M_PI)data->theta(i) = (T(i)+ T(i-1))/2 + M_PI;
							else data->theta(i) = (T(i)+ T(i-1))/2;
						}
					else if(T(i-1) > 3*M_PI/2  && T(i-1) < 2*M_PI && T(i) > M_PI/2 && T(i) < M_PI)			//From Quadrant IV to II
						{
							if(T(i-1)>T(i)+M_PI)data->theta(i) = (T(i)+ T(i-1))/2 + M_PI;
							else data->theta(i) = (T(i)+ T(i-1))/2;
						}
				}
			else if((T(i-1) > 0 && T(i-1) < M_PI/2 && T(i) > M_PI && T(i) < 3*M_PI/2) || (T(i-1) > M_PI  && T(i-1) < 3*M_PI/2 && T(i) > 0 && T(i) < M_PI/2))			//From Quadrant I to III or viceversa
				{
					if(T(i-1) > 0 && T(i-1) < M_PI/2 && T(i) > M_PI && T(i) < 3*M_PI/2)						//From Quadrant I to III
						{
							if(T(i)>T(i-1)+M_PI)data->theta(i) = (T(i)+ T(i-1))/2 + M_PI;
							else data->theta(i) = (T(i)+ T(i-1))/2;
						}
					else if(T(i-1) > M_PI  && T(i-1) < 3*M_PI/2 && T(i) > 0 && T(i) < M_PI/2)				//From Quadrant III to I
						{
							if(T(i-1)>T(i)+M_PI)data->theta(i) = (T(i)+ T(i-1))/2 + M_PI;
							else data->theta(i) = (T(i)+ T(i-1))/2;
						}
				}
			else
				{
					data->theta(i) = (T(i)+ T(i-1))/2;
				}
		}		
	}
	//DM-FEB2011

	double mtheta = (data->theta(channel.length-2)-data->theta(channel.length-3)) / data->intervals(channel.length-2);
	data->theta(channel.length-1) = data->theta(channel.length-2) + mtheta * data->intervals(channel.length-1);

	//DEBUG_PRINT3(sThetaTemp, channel.length, data->coords(channel.length-1), data->theta(channel.length-1));    //CORRECTION AUG2009
}

void RMPrep::ComputeDimensionedSN(int arrayLen, int arrayWidth, double channelWidth, RMPrepData* data)
{
	for (int i = 0; i <= arrayLen-1; i++)
		data->S(i) = data->coords(i) * channelWidth/2.0;			// s with dimensions

	for (int j = 0; j <= arrayWidth-1; j++)
		data->N(j) = data->coordn(j) * channelWidth/2.0;           // n with dimensions
}


void RMPrep::RegridClean(RMMigrationData* migdata, RMPrepData* data, int oldnumberpoints)	//DM-FEB2012 Removed & from  RMMigrationData*
{
	int newnumberpoints;

	logmsg("RegridClean\n");

	

/////////////////////
/*
	RMPrep prep;
	prep.Initialize(migdata->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migdata->channel.centerline.length+1, size, xy);			// Splines with one more node are generated		

	migdata->Init(size, migdata->channel.nnodes);			
	migdata->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);


	prep.Initialize(migdata->channel.centerline);

	size = 0;
	xy = NULL;
	prep.GetXY(migdata->channel.centerline.length-1, size, xy);			// Splines with one less node are generated, so overall the 
																		// number of nodes before this routine is called is maintained

	migdata->Init(size, migdata->channel.nnodes);			
	migdata->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);
*/
//////////////////////

	RMPrep prep;
	prep.Initialize(migdata->channel.centerline);

	int size = 0;
	double** xy = NULL;
	prep.GetXY(migdata->channel.centerline.length, size, xy);				

	migdata->Init(size, migdata->channel.nnodes);			
	migdata->channel.centerline.init(size, 2, xy);
    FreeXY(size, xy);

/////////////////////////

	newnumberpoints = migdata->channel.centerline.length;	

	data->sSum = 0.0;
	data->coords(0) = 0.0;	
	data->intervals(0) = 0.0;	
	data->MaxInterval = data->intervals(0);
data->MinInterval = 9999.0;//DM-FEB2011
	for (int i = 1; i <= migdata->channel.centerline.length-1; i++)
		{	
			data->intervals(i) = sqrt(pow((migdata->channel.centerline(i,0)-migdata->channel.centerline(i-1,0)),2.0)+pow((migdata->channel.centerline(i,1)-migdata->channel.centerline(i-1,1)),2.0));
			data->sSum = data->sSum + data->intervals(i);
			data->coords(i) = data->sSum;	
			if (data->intervals(i) > data->MaxInterval) {data->MaxInterval = data->intervals(i);}
			if (data->intervals(i) < data->MinInterval) {data->MinInterval = data->intervals(i);}//DM-FEB2011
		}
	
	data->MaxInterval = data->MaxInterval * migdata->channel.WIDTH/2;
	data->MinInterval = data->MinInterval * migdata->channel.WIDTH/2;//DM-FEB2011
	//logmsg("	Updated Max ds (m)= %lf, ds/B = %lf \n", data->MaxInterval, data->MaxInterval/(migdata->channel.WIDTH/2));
	//logmsg("	Updated Number of nodes in the streamwise direction = %d \n", channel.length);			
	
	ComputeWeightsForInterpolation(data, newnumberpoints, oldnumberpoints);	

	//Set all weights all wInterpolationDS equal to 1
	for(int r = 0; r <= migdata->channel.centerline.length-1; r++)data->wInterpolationDS(r) = 1.0;//DM-FEB2012
}


void RMPrep::CurvatureCapping(int iter, const RMCurvatureMethod& method, RMMigrationData* migdata, RMPrepData* data)					//DM-FEB2012		//Removed & from RMMigrationData* 
{
	for(int i = 0; i<= migdata->channel.centerline.length-1; i++)														// Loop on centerline nodes
		{	
			//logmsg("COUNTER %d\n",i);
			double* COPYx = NULL;																	// Store centerline nodes
			double* COPYy = NULL; 
			COPYx  = new double[migdata->channel.centerline.length];
			COPYy  = new double[migdata->channel.centerline.length];
			for(int j = 0; j <=migdata->channel.centerline.length-1; j++)
				{
					COPYx[j] = migdata->channel.centerline(j,0);
					COPYy[j] = migdata->channel.centerline(j,1);
				}
			if(data->curvature(i) > 1.0)															// Case dimensionless curvature greater than 1.0
				{	
					//printf("RIGHT HIGH NODE = %d\n",i);
					migdata->channel.centerline.length = migdata->channel.centerline.length - 1;	// Reduce number of centerline nodes by one
					for(int k = i; k<= migdata->channel.centerline.length-1; k++)					// Remove centerline node
						{
							migdata->channel.centerline(k,0) = COPYx[k+1];
							migdata->channel.centerline(k,1) = COPYy[k+1];
						}
					ComputeIntervals(migdata, data);												// Re-compute intervals
					ComputeCurvature(iter, migdata->curvMethod, migdata->channel.centerline, data);	// Re-compute curvatures
					//logmsg("\n");
					//logmsg("RIGHTMOD	i	x	y	s	C\n");
					//for(int r = 0; r<= migdata->channel.centerline.length - 1; r++)logmsg("RIGHTMOD	%d	%lf	%lf	%lf	%lf\n",r,migdata->channel.centerline(r,0),migdata->channel.centerline(r,1),data->coords(r),data->curvature(r));
					i = i-1;																		// Reduce by one the counter
				}
			else if(data->curvature(i) < -1.0)														// Case dimensionless curvature lower than -1.0
				{	
					//printf("LEFT HIGH = %d\n",i);
					migdata->channel.centerline.length = migdata->channel.centerline.length - 1;	// Reduce number of centerline nodes by one
					for(int k = i; k<= migdata->channel.centerline.length-1; k++)					// Remove centerline node
						{
							migdata->channel.centerline(k,0) = COPYx[k+1];
							migdata->channel.centerline(k,1) = COPYy[k+1];
						}
					ComputeIntervals(migdata, data);												// Re-compute intervals
					ComputeCurvature(iter, migdata->curvMethod, migdata->channel.centerline, data);	// Re-compute curvatures 
					//logmsg("/n");
					//logmsg("LEFTMOD	i	x	y	s	C\n");
					//for(int r = 0; r<= migdata->channel.centerline.length - 1; r++)logmsg("LEFTMOD	%d	%lf	%lf	%lf	%lf\n",r,migdata->channel.centerline(r,0),migdata->channel.centerline(r,1),data->coords(r),data->curvature(r));
					i = i-1;																		// Reduce by one the counter
				}
			delete [] COPYx; 
			delete [] COPYy; 
			COPYx = NULL;
			COPYy = NULL;
		}

}	
