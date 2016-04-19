#pragma warning(once : 4290)
#pragma warning (disable:4251)

#include "rvrmeander.h"
#include "interface.h"
#include "error.h"
#include <INIReader.h>
#include <string>


EXTERN int RVRDLL_Run(const char* configFile, const char* logFile)
{
    return Run(configFile, logFile, CFT_INI);
}


EXTERN int RVRDLL_Run_libconfig(const char* configFile, const char* logFile)
{
    return Run(configFile, logFile, CFT_libconfig);
}


EXTERN int RVRDLL_Init(const char* configFile, const char* logFile)
{
    RVRMeander* meander = new RVRMeander();

    if (logFile != NULL)
    {
        if (strcmp(logFile, "-stdout") == 0)
            meander->OpenLogSTDOUT();
        else if (strcmp(logFile, "-stderr") == 0)
            meander->OpenLogSTDERR();
        else if (! meander->OpenLog(logFile))
            return RVR_ERR_UNABLE_TO_CREATE_LOG;
    }

	if (! meander->Load(configFile, CFT_INI))
        return RVR_ERR_UNABLE_TO_LOAD_FILE;

	if (meander->Initialize() != 0)
		return 0;

	return (int)meander;
}

EXTERN int RVRDLL_GetNumIters(int infoPtrAddr)
{
	if (infoPtrAddr == 0)
		return -1;

	return ((RVRMeander*)infoPtrAddr)->GetNumIters();
}

EXTERN int RVRDLL_Step(int infoPtrAddr, int iterIdx)
{
	if (infoPtrAddr == 0)
		return -1;

	return ((RVRMeander*)infoPtrAddr)->ExecuteStep(iterIdx);
}

EXTERN int RVRDLL_Finish(int infoPtrAddr)
{
	if (infoPtrAddr == 0)
		return -1;

	((RVRMeander*)infoPtrAddr)->CloseLog();
	return ((RVRMeander*)infoPtrAddr)->Finish();
}


int Run(const char* configFile, const char* logFile, ConfigFileType cft)
{
    RVRMeander meander;

    if (logFile != NULL)
    {
        if (strcmp(logFile, "-stdout") == 0)
            meander.OpenLogSTDOUT();
        else if (strcmp(logFile, "-stderr") == 0)
            meander.OpenLogSTDERR();
        else if (! meander.OpenLog(logFile))
            return RVR_ERR_UNABLE_TO_CREATE_LOG;
    }

	if (! meander.Load(configFile, cft))
        return RVR_ERR_UNABLE_TO_LOAD_FILE;

    int result = meander.RunModel();

    meander.CloseLog();

    return result;
}

