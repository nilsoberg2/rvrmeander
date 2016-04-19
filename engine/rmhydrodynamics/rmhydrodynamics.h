#ifndef __RMHYDRODYNAMICS_H_________________20090410144141__
#define __RMHYDRODYNAMICS_H_________________20090410144141__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include "../rmprep/rmprep.h"
#include "../rmchannel/rmchannel.h"
#include "../rmmigration/rmmigrationdata.h"


#if defined(LIBRMHYDRODYNAMICS_EXPORTS)
#define LIBRMHYDRODYNAMICS_DLL_IMP_EXP DLL_EXPORT
#elif defined(LIBRMHYDRODYNAMICS_STATIC)
#define LIBRMHYDRODYNAMICS_DLL_IMP_EXP
#else
#define LIBRMHYDRODYNAMICS_DLL_IMP_EXP DLL_IMPORT
#endif


const int HydrodynamicsLastNodeMethod_Extrapolation = 1;
const int HydrodynamicsLastNodeMethod_CalculatedWithCurvature = 2;
const int HydrodynamicsLastNodeMethod_ZeroGradient = 3;  //DM-FEB2012


struct LIBRMHYDRODYNAMICS_DLL_IMP_EXP RMHydrodynamicsData
{ 
	// all RMSeries hydrodynamics generic
    // Dimensionless perturbations
    RMSeries u, v, h, d, e;
    // Perturbations with dimensions
    RMSeries U1, V1, H1, D1, E1;
    // "Total" values with dimensions
    RMSeries UU, VV, HH, DD, EE;
    // Bed shear stresses with dimensions (s,n)
    RMSeries TauS, TauN;
    // Velocity with dimensions (x,y)
    RMSeries VEL, Ux, Vy;
    // Bed shear stresses with dimensions (x,y)
    RMSeries TAU, TauX, TauY;

    // Dimensionless perturbation u at n = 1
    RMSeries ub;

	// Friction factor
	RMSeries CF;

	// Arrays for storing
	RMSeries OldUU, OldVV, OldHH, OldDD, OldEE, OldTAU, OldTauX, OldTauY, OldCF, OldVEL, OldUx, OldVy;				//ADDITION MAR2010
	int HydrodynamicsLastNodeMethod;
	double ManningCoefficient;
	double U_ch;			//ADDITION SEP2010bis 

	virtual void InitBase(int length, int width);
	virtual void InitBaseNewHydroValues(int length, int width);		//ADDITION MAR2010
	virtual void InitBaseOldHydroValues(int length, int width);		//ADDITION MAR2010
	virtual void Init(int length, int width) = 0;
	virtual void InitNewHydroValues(int length, int width) = 0;
	virtual void InitOldHydroValues(int length, int width) = 0;
};

class LIBRMHYDRODYNAMICS_DLL_IMP_EXP RMHydrodynamics
    : public RMLoggerImpl
{
public:
	virtual ~RMHydrodynamics();

	virtual void Preprocess(int iter, const RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data_in) = 0;  //DM-FEB2011

	// Implementation specific
	virtual void ComputeDimensionlessPerturbationUVHDE(const RMMigrationData* migdata_in,
		const RMPrepData* pdata,
		RMHydrodynamicsData* data) = 0;

	virtual void ComputeDimensionedPerturbationUVHDE(const RMPrepData* pdata,
        RMMigrationData* migdata_in,
		RMHydrodynamicsData* data) = 0;

	virtual void ComputeDimensionedFlowField(const RMMigrationData* migdata_in,
		const RMPrepData* pdata,
		RMHydrodynamicsData* data) = 0;

	virtual void StoreHydrodynamics (RMMigrationData* migdata, RMHydrodynamicsData* data) = 0;		//ADDITION MAR2010

	virtual void InputHydrograph(int,double &,double &) = 0;//DM-FEB2011
	virtual void OscillatingUSBoundaryCondition(int, RMHydrodynamicsData*, RMPrepData*) = 0; //MMMMTEMP OscillatingUSBoundaryCondition


	virtual void InterpolateHydrodynamics (RMPrepData* pdata, RMMigrationData* migdata, RMHydrodynamicsData* data, double) = 0;		//ADDITION MAR2010  //DM-FEB2011
};


#endif//__RMHYDRODYNAMICS_H_________________20090410144141__
