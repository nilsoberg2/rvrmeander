#ifndef __RMPLOTTING_H______________________20090406150808__
#define __RMPLOTTING_H______________________20090406150808__

#include "../common.h"
#include <stdio.h>
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include "../rmmigration/rmmigration.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"

#if defined(LIBRMPLOTTING_EXPORTS)
#define LIBRMPLOTTING_DLL_IMP_EXP DLL_EXPORT
#elif defined(LIBRMPLOTTING_STATIC)
#define LIBRMPLOTTING_DLL_IMP_EXP
#else
#define LIBRMPLOTTING_DLL_IMP_EXP DLL_IMPORT
#endif


class LIBRMPLOTTING_DLL_IMP_EXP RMPlotting
    : public RMLoggerImpl
{
public:
    RMPlotting();
    virtual ~RMPlotting();

	virtual void SetTecplotOutput_1D(const char* file);
	virtual void SetTecplotOutput_uv2D(const char* file);
	virtual void SetTecplotOutput_hde2D(const char* file);
	virtual void SetTecplotOutput_U1V12D(const char* file);
	virtual void SetTecplotOutput_H1D1E12D(const char* file);
	virtual void SetTecplotOutput_UUVV2D(const char* file);
	virtual void SetTecplotOutput_HHDDEE2D(const char* file);
	virtual void SetTecplotOutput_VELandTAU2D(const char* file);
	virtual void SetTecplotOutput_Sections2D(const char* file);
	virtual void SetTecplotOutput_Hydro3D(const char* file);
	virtual void SetTecplotOutput_BedBanks3D(const char* file);
	virtual void SetOutput_ReworkedAreas(const char* file);
	virtual void SetOutput_PlanarFailure(const char* file);
	//virtual void SetOutput_HeterogeneityAtCenterline(const char* file);   //HETEROGENEITY  //DM-FEB2011 Commented out
	virtual void SetTecplotOutput_Floodplain(const char* file);//DM-FEB2011 
	virtual void SetKarmorFile(const char* file);//DM-FEB2012
	virtual void SetIndicesFile(const char* file);//MMMMTEMP Indices file
	virtual void SetKarmorVisualizedFile(const char* file);//DM-FEB2013

	virtual void TecPlot1D(int j, double deltaT, const RMMigrationData*);
	virtual void TecPlot2D(int j, double deltaT, const RMMigrationData* migData, const RMHydrodynamicsData* hData, RMPrepData* prepData);
	
	virtual void TecPlotSections(int, double deltaT, RMBankErosionData*, RMMigrationData*, RMHydrodynamicsData*, RMPrepData*);																					
	virtual void TecPlotBedBank3D(int, double deltaT, RMBankErosionData*, RMMigrationData*, RMHydrodynamicsData*);																					
	
	virtual void ReworkedAreas(int, double deltaT, RMBankErosionData*, RMMigrationData*);
	//virtual void HeterogeneityAtCenterline(int, double, RMBankErosionData*, RMPrepData*);   //HETEROGENEITY    //DM-FEB2011 Commented out
    virtual void TecplotHeterogeneityGrid(RMBankErosionData*);							    //HETEROGENEITY    //DM-FEB2011
    virtual void TecplotHeterogeneityGridWithLayers(RMBankErosionData*);					//HETEROGENEITY    //DM-FEB2011 


	virtual void RMPlotting::WriteKarmorFile(int, double, const RMBankErosionData*, const RMHydrodynamicsData*, const RMPrepData*, const RMMigrationData*); //DM-FEB2012
	virtual void RMPlotting::WriteIndicesFile(int, double, const RMBankErosionData*);																		//DM-FEB2012
	virtual void RMPlotting::WriteKarmorVisualizedFile(int, double, const RMMigrationData*, const RMBankErosionData*); //DM-FEB2013

	virtual void SetFrequency(int freq);
	virtual bool CanPlot(int iter);

protected:
	FILE* m_Tecplot1D;
	FILE* m_Tecplotuv2D;
	FILE* m_Tecplothde2D;
	FILE* m_TecplotU1V12D;
	FILE* m_TecplotH1D1E12D;
	FILE* m_TecplotUUVV2D;
	FILE* m_TecplotHHDDEE2D;
	FILE* m_TecplotVELandTAU2D;
	FILE* m_TecPlotSections2D;
	FILE* m_TecPlotHydro3D;
	FILE* m_TecPlotBedBanks3D;
	FILE* m_ReworkedAreas;
	FILE* m_PlanarFailure;
	//FILE* m_HeterogeneityAtCenterline;   //HETEROGENEITY  //DM-FEB2011  Commented out
	FILE* m_TecplotGrid; //DM-FEB2011
	FILE* KarmorFile;//DM-FEB2012
	FILE* IndicesFile;	//MMMMTEMP Indices file
	FILE* KarmorVisualizedFile; //DM-FEB2013

	int m_frequency;
};


#endif//__RMPLOTTING_H______________________20090406150808__
