#ifndef __RMHYDRODYNAMICS_IKEDA1981_H_______20090413083838__
#define __RMHYDRODYNAMICS_IKEDA1981_H_______20090413083838__


#include "rmhydrodynamics.h"

struct LIBRMHYDRODYNAMICS_DLL_IMP_EXP RMHydrodynamicsData_Ikeda1981
	: public RMHydrodynamicsData
{ 
	// all contants ikeda
    double a1;
    double a2;
    double a3;
    double a4;
    double a5;
    double a6;
    double s1;
    double s2;
	double a4_SC1; //MMMMTEMP Secondary flow correction
	double a4_SC2; //MMMMTEMP Secondary flow correction

	// ikeda

    double UB_0;
    double C_0;
    double CF_0;
	double CF_ch;  //ADDITION DEC2009
    double BE;
    double F_02;
    double ALF;
	double U0;

	double U_ch;    //ADDITION SEP2010
	double D_ch;    //ADDITION SEP2010
    double F_02_ch; //ADDITION SEP2010
    double BE_ch;   //ADDITION SEP2010

    double xi;

	bool sec_flow_corr;					//DM-FEB2012
	bool use_mann_for_fric_coeff;		//DM-FEB2012
	bool amplify_fric_coeff;			//DM-FEB2012
	double ampl_fric_coeff;				//DM-FEB2012


	virtual void Init(int length, int width);
	virtual void InitNewHydroValues(int length, int width);
	virtual void InitOldHydroValues(int length, int width);
};


class LIBRMHYDRODYNAMICS_DLL_IMP_EXP RMHydrodynamics_Ikeda1981
	: public RMHydrodynamics
{
public:
    RMHydrodynamics_Ikeda1981();
    virtual ~RMHydrodynamics_Ikeda1981();

	virtual void Preprocess(int iter, const RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data_in); //DM-FEB2011

    virtual void CalculateAlpha(const RMMigrationData* migData, RMHydrodynamicsData*);																						// Calculate Alpha
	virtual void ComputeXi(const RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data_in);  //DM-FEB2011

	// Implementation specific
	virtual void ComputeDimensionlessPerturbationUVHDE(const RMMigrationData* migdata_in,
		const RMPrepData* pdata,
		RMHydrodynamicsData* data);

	virtual void ComputeDimensionedPerturbationUVHDE(const RMPrepData* pdata,
        RMMigrationData* migdata_in,
		RMHydrodynamicsData* data);

	virtual void ComputeDimensionedFlowField(const RMMigrationData* migdata_in,
		const RMPrepData* pdata,
		RMHydrodynamicsData* data);

	virtual void StoreHydrodynamics (RMMigrationData* migdata, RMHydrodynamicsData* data);		//ADDITION MAR2010
	
	virtual void InputHydrograph(int,double &,double &);//DM-FEB2012
	virtual void OscillatingUSBoundaryCondition(int, RMHydrodynamicsData* data, RMPrepData* prepData); //DM-FEB2012

	virtual void InterpolateHydrodynamics (RMPrepData* pdata, RMMigrationData* migdata, RMHydrodynamicsData* data, double);		//ADDITION MAR2010   //DM-FEB2011
};



#endif//__RMHYDRODYNAMICS_IKEDA1981_H_______20090413083838__
