#ifndef RM_MEANDER_H__20090107__
#define RM_MEANDER_H__20090107__

#include "../common.h"
#include "../rmprep/rmprep.h"
#include "../rmfilter/rmfilter.h"
#include "../rmmigration/rmmigration.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"
#include "../rmbankerosion/rmbankerosion.h"
#include "../rmplotting/rmplotting.h"
#include "../rmio/rmlog.h"
#include "config_file.h"
#include "rundata.h"
#include <cstdio>


class RVRMeander
    : public RMLogger
{
public:
    RVRMeander();
    ~RVRMeander();

    virtual bool Load(std::string file, ConfigFileType type);
    virtual int Initialize();
    virtual int RunModel();
	virtual int ExecuteStep(int j);
	virtual int Finish();
	virtual int GetNumIters();

    // PRINTING ROUTINES
    virtual void PrintInfo(RMMigrationData* migData, RMFilter* filter,
			               RMPrepData* pData, RMHydrodynamicsData* hData, RMBankErosionData* beData);
    virtual void PrintInfo_Migration(RMMigrationData* migData, RMFilter* filter);
    virtual void PrintInfo_BankErosion(RMBankErosionData* beData);
    virtual void PrintInfo_Hydrodynamics(RMHydrodynamicsData* hData, RMMigrationData* migData);
    virtual void PrintInfo_Prep(RMMigrationData* migData);
    virtual void PrintInfo_Smoothing(RMFilter* filter);
    virtual void PrintInfo_Channel(RMChannel& channel);

    // SMOOTHING ROUTINES
    virtual void ApplySmoothing(int j, RMFilter* filter, RMPrepData* prepData, RMPrep*& prep, RMMigrationData* migData, int ARRAY_WIDTH, int MethodBankEvolution);

protected:
    ConfigFileContents m_configData;
    std::string m_configFile;
    RMLogger* m_logger;

	RunData rd;

protected:
    virtual bool loadConfigFile(GenericConfigObj& config, ConfigFileContents& configData);

    virtual void cleanup();
};


std::string DetermineMethod(const char* file, ConfigFileType type);


#endif//RM_MEANDER_H__20090107__
