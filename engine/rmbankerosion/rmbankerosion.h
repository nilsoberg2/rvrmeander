#ifndef __RMBANKEROSION_H___________________20090410142929__
#define __RMBANKEROSION_H___________________20090410142929__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"
#include "../rmmigration/rmmigrationdata.h"
#include "bankerosion.h"				
#include <vector>

#if defined(LIBRMBANKEROSION_EXPORTS)
#define LIBRMBANKEROSION_DLL_IMP_EXP DLL_EXPORT
#elif defined(LIBRMBANKEROSION_STATIC)
#define LIBRMBANKEROSION_DLL_IMP_EXP
#else
#define LIBRMBANKEROSION_DLL_IMP_EXP DLL_IMPORT
#endif


struct LIBRMBANKEROSION_DLL_IMP_EXP RMBankErosionData
{
    double FACTR;
	double EC;

	RMSeries ErosionBank;
		
	int MethodBankEvolution;
	bool FloodplainHeterogeneity; //DM-FEB2011
	std::string floodplain_file;  //DM-FEB2011
	int MethodPhysicallyBasedBankEvolution;
	bool reduce_stress_bank;//DM-FEB2012
	double reduction_stress_bank;//DM-FEB2012
	int MethodShearStress;
	int MethodUpdateWidth;
	int MethodErosionDistance;
	int numberofgrids;	
	int MonitorNode;
	double MonitorNodeStreamwiseDistance;		// ADDITION MAR2010
	double ElongationGap;
    std::vector<Grid> grids;
	FlowVariables *flowworkspace;
	Coordinates *intersectleft, *intersectright, *oldintersectleft, *oldintersectright;
	Coordinates *toeleft, *toeright, *oldtoeleft, *oldtoeright;
    std::vector<double> StationCenterline;
	double *ReworkedAreaLeft, *ReworkedAreaRight;
	double TotalErodedVolume;

	double tblock;			//DM-FEB2012
	bool use_karmor;		//DM-FEB2012
	double c_factor_for_karmor;										//MMMMTEMP Additional parameter in "prototype.cfg"
	double d_factor_for_karmor;										//MMMMTEMP Additional parameter in "prototype.cfg"

	FailureParameters parameters;										

	int fluvialerosionswitch;			int CrossSectionDivision;				
	int TypeLawFluvialErosion;		// ADDITION MAR 2010
	double grainroughnessthreshold;		double wpthreshold;
	double MAX_EROSION_DISTANCE;		double mindistance;
	int cantileverfailureswitch;		double undercutthreshold;
	int planarfailureswitch;			int numberofintersects;
	double zthreshold;					double tensioncrackoption; 
	int slicemultiplier; 
	bool bankfailureanalysis[3];//SEP2009		
	double accuracy; 
	int maxiter;						int ANALYSISMETHOD;
	double lambda;						
	//int sedimentroutingswitch;			//CORRECTION AUG2009
    //double MAX_COHESIVE_EROSION_DISTANCE;	//CORRECTION AUG2009
	bool DepositionSwitch;

	bool RegridCenterlineNodes;			//ADDITION MAR2010
	bool InterpolateAfterSplines;		//ADDITION FEB2010
	int InterpolateFrequency;			//ADDITION FEB2010
	double BankInterpolationTolerance;	//ADDITION MAR2010

	double X_origin, Y_origin, DeltaX, DeltaY;								//HETEROGENEITY //DM-FEB2011
	int Nrows, Ncolumns;													//HETEROGENEITY //DM-FEB2011
	int MaxNumberOfLayers;													//HETEROGENEITY //DM-FEB2011
	char Comment[200];														//HETEROGENEITY //DM-FEB2011
	char Header[200];														//HETEROGENEITY //DM-FEB2011

    // Heter. grid stuff
	RMSeries X_grid;
    RMSeries Y_grid;
    RMSeries ErosionRate_grid;
    RMSeries CriticalTau_grid;
    RMSeries Unitweight_grid;
    RMSeries Cohesion_grid;
    RMSeries AngleRepose_grid;
    RMSeries AnglePhib_grid;
	RMSeries EC_grid;

	std::vector<std::vector<int> > NoOfLayers_gridWL;
	RMSeries ErosionRate_gridWL[5];			//DM-FEB2011
    RMSeries CriticalTau_gridWL[5];			//DM-FEB2011
    RMSeries Unitweight_gridWL[5];			//DM-FEB2011
    RMSeries Cohesion_gridWL[5];			//DM-FEB2011
    RMSeries AngleRepose_gridWL[5];			//DM-FEB2011
    RMSeries AnglePhib_gridWL[5];			//DM-FEB2011
	RMSeries EC_gridWL[5];					//DM-FEB2011
	RMSeries LayerElevation_gridWL[5];		//DM-FEB2011
	RMSeries FloodplainElevation_gridWL;    //MMMMTEMP Floodplain elevation

    // 1-D stuff
	RMSeries FACTRnu;
	RMSeries FACTRnu_left;					//DM-FEB2011
	RMSeries FACTRnu_right;					//DM-FEB2011
	RMSeries ECnu;
	RMSeries ECnu_left;						//DM-FEB2011
	RMSeries ECnu_right;					//DM-FEB2011

	virtual void Init(int length, int width);
	virtual void InitBase(int length, int width);
	virtual void InitBankProperties(int length);
	virtual void ReInitBankProperties(int length);//OCT2009

	RMBankErosionData();
	~RMBankErosionData();
	void Cleanup();
};


class LIBRMBANKEROSION_DLL_IMP_EXP RMBankErosion
    : public RMLoggerImpl
{
public:
    RMBankErosion();
    virtual ~RMBankErosion();

	virtual void ComputeDimensionlessBankErosion(RMHydrodynamicsData*, RMBankErosionData*, RMMigrationData*, RMPrepData*, double, int);  //DM-FEB2012
	virtual void UpdateCrossSectionStations(RMBankErosionData*, RMMigrationData*);																			
	virtual void PhysicalBankEvolution(RMBankErosionData*, RMHydrodynamicsData*, RMMigrationData*, RMPrepData*, double,int);  //DM-FEB2012																				
	virtual void AssignInitialSectionProperties(const char* dirPath, RMBankErosionData*);	
	virtual Bank BankInterpolated(Bank*,Bank*, double, int, double, int);  //DM-FEB2011
	virtual Bank BankInterpolatedVARIANT(Bank*,Bank*, double, int, double, int);  //DM-FEB2012 
	virtual Bank BankGeometryInterpolated(Bank*,Bank*,int,double,double); //DM-FEB2012 
	virtual void SetBottomAndTopIndex(Bank &, int); //DM-FEB2012
	virtual void ReachInterpolation(RMPrepData* prepData, RMMigrationData* migData, RMBankErosionData* bankErosionData, RMBankErosion* bankErosion);		//ADDITION FEB2010
	virtual void CutoffPBA(RMMigrationData* migdata,RMBankErosionData* data);//MMMMTEMP Cutoff with PB method
	virtual void AssignBankEvolutionProperties(const char* dirPath, RMBankErosionData*);																		
	virtual void PrepareTecPlot3D(RMBankErosionData*, RMMigrationData*, RMPrepData*);																					
	virtual int SetMargin(const std::vector<Coordinates>&, double, int, int, int);	
	virtual void FluvialErosion(Bank *, int&, int&, CrossSection *, double, double, int, int, int, double, double&, const RMHydrodynamicsData*, RMBankErosionData*, RMMigrationData*, double, double&); 	
	virtual void CantileverFailure(Bank *, double&, RMBankErosionData*, double&, int);//DM-FEB2012																	
	virtual double InverseSlope(Coordinates, Coordinates);	
	virtual int UpdateMonitorNode(RMPrepData* , RMMigrationData* , double&, int);					//ADDITION MAR2010  //MMMMTEMP Changes in UpdateMonitorNode routine
	virtual void BankGeometryQualityControl(Bank &, int);  //DM-FEB2011
	virtual double PolygonArea(const std::vector<Coordinates>&, int);																	
	virtual double DistanceNode(Coordinates, Coordinates);																
    virtual double DistanceNodeArray(const std::vector<Coordinates>&, int);	
	virtual void SetUndercut(Bank *, double&, int); //MMMMTEMP Added last argument																			
	virtual	void UpdateBankNodes(Bank *,int&,int&,int,int&,Coordinates,double,CrossSection *, int, int, RMBankErosionData*);				
	virtual void AddNodeElevation(std::vector<Coordinates>&, int&, Coordinates); 	
	virtual	void RemoveNode(std::vector<Coordinates>&, int&, int);																	
	virtual	double Slope(const Coordinates&, const Coordinates&);																		
	virtual void AddLayerIntersects(Bank *);																			
	virtual Coordinates IntersectXSandStage(Bank *, double);															
	virtual void ComputeFrictionSlope(RMBankErosionData*, RMHydrodynamicsData*, RMMigrationData*, RMPrepData*);																				
	virtual void SectionGeometryAdjustment(RMBankErosionData*, RMHydrodynamicsData*, RMMigrationData*, RMPrepData*);//DM-FEB2011																		
	virtual void UpdateWidthAndDimensionlessCenterline(RMBankErosionData*, RMMigrationData*);																
	virtual void IntersectCrossSectionAndStageAndToes(RMBankErosionData*, RMHydrodynamicsData*, RMMigrationData*);																
	virtual void PlanarFailure(Bank *, RMBankErosionData*, RMHydrodynamicsData*, int, int, RMMigrationData*, double&);		
	virtual void ComputeKarmor(Bank *, RMBankErosionData*, int,int,double,double);																//DM-FEB2012
	virtual void DecayBlock(Bank *, RMBankErosionData*, double,double,int);																							//DM-FEB2012
	virtual Coordinates NodeAtZposition(const Coordinates&, const Coordinates&, double);												
	virtual void MinimumFactorOfSafety(Bank *, int, Coordinates&, int, double, double&, double&, FailureParameters&, RMBankErosionData*, RMMigrationData*, int);	
	virtual void TrimAtBack(std::vector<Coordinates>&, int&, int);																	
	virtual void TrimAtFront(std::vector<Coordinates>&, int&, int);																
	virtual void AddAtBack(std::vector<Coordinates>&, int&, int);																	
	virtual void AddAtFront(std::vector<Coordinates>&, int&, int);																	
	virtual double FactorOfSafety(Bank *, Coordinates, int, double, double, FailureParameters&, RMBankErosionData*, RMMigrationData*, int);												
    virtual Coordinates NodeAtFraction(const Coordinates&, const Coordinates&, double);												
	virtual int SetMarginY(const std::vector<Coordinates>&, double, int, int, int);														
	virtual Coordinates NodeAtYposition(const Coordinates&, const Coordinates&, double);												
	virtual void AddWaterWeight(Bank*, int, int, Coordinates[], Coordinates, int, double[], RMMigrationData*, int, int);			//MMMMTEMP Added last two int's						
	virtual void SliceWeight(Bank *, Coordinates [], Coordinates [], int, int, double [],int, RMMigrationData*, int, int);			//MMMMTEMP Added last one							
	virtual int CorrectP4Left(double, double, double, double);
	virtual int CorrectP4Right(double, double, double, double);
	virtual void UpdateBankProperties(RMBankErosionData*,RMMigrationData*,RMPrepData*);									//HETEROGENEITY		//DM-FEB2011
	virtual void UpdateBankPropertiesWithLayers(RMBankErosionData*,RMMigrationData*,RMPrepData*);						//HETEROGENEITY		//DM-FEB2011
	virtual void UpdateMigrationCoefficient(RMBankErosionData*,RMMigrationData*,RMPrepData*);							//HETEROGENEITY		//DM-FEB2011
	virtual void UpdateMigrationCoefficientWithLayers(RMBankErosionData*,RMMigrationData*,RMPrepData*);					//HETEROGENEITY		//DM-FEB2011
	virtual void ReadHeterogeneityGrid(RMBankErosionData*);																//HETEROGENEITY		//DM-FEB2011
	virtual void ReadHeterogeneityGridWithLayers(RMBankErosionData*);													//HETEROGENEITY		//DM-FEB2011	
	virtual double InterpolatedValueFromGrid(RMBankErosionData*, double, double, int);					//HETEROGENEITY		//DM-FEB2011
	virtual double InterpolatedValueFromGridWithLayers(RMBankErosionData*, double, double, int);			//HETEROGENEITY		//DM-FEB2011
	virtual void AddAnotherBankNode(Bank *, Coordinates, int&, int&, int, int, int&, int, int);//MMMMTEMP BottomCantileverBlock
	virtual double RoundNumber(double value, int digits);//MMMMTEMP Round value
	

};


#endif//__RMBANKEROSION_H___________________20090410142929__
