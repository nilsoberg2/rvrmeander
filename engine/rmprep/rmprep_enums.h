#ifndef __RMPREP_ENUMS_H____________________20090406170303__
#define __RMPREP_ENUMS_H____________________20090406170303__


struct LIBPREP_DLL_IMP_EXP RMCurvatureMethod
{
	int CurvatureMethod;
	int UpstreamCurvatureMethod;
	int DownstreamCurvatureMethod;
	bool CurvatureCapping;//DM-FEB2012
};


const int Curvature_JohannessonParker = 1;
const int Curvature_DT_DS_Backward = 3;			//CORRECTION FEB2010
const int Curvature_DT_DS_Central = 4;			//CORRECTION FEB2010
const int Curvature_FittingCircle = 2;
const int Curvature_Splines = 5;				//CORRECTION FEB2010
const int Curvature_Upstream_C_0_First = 1;
const int Curvature_Upstream_C_0_Always = 2;
const int Curvature_Upstream_Constant_Zero = 3;
const int Curvature_Extrapolated = 1;  //DM-FEB2011
const int Curvature_Downstream_Constant_Zero = 2;
const int Curvature_Downstream_Zero_Gradient = 3; //DM-FEB2011


#endif//__RMPREP_ENUMS_H____________________20090406170303__
