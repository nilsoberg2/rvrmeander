#ifndef __RM_MEANDER_INTERFACE_H__20100329__
#define __RM_MEANDER_INTERFACE_H__20100329__

#include "../common.h"
#include "enums.h"
#include <cstddef>


#define EXTERN extern "C"


#if defined(RMMEANDERDLL_EXPORTS)
#define RMMEANDERDLL_DLL_API DLL_EXPORT
#else
#define RMMEANDERDLL_DLL_API DLL_IMPORT
#endif


// This version uses the INI file format as default.
EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_Run(const char* configFile, const char* logFile);

EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_Init(const char* configFile, const char* logFile);

EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_GetNumIters(int infoPtrAddr);

EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_Step(int infoPtrAddr, int stepIdx);

EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_Finish(int infoPtrAddr);


// Config file

EXTERN
RMMEANDERDLL_DLL_API
int RVRDLL_Run_libconfig(const char* configFile, const char* logFile);


int Run(const char* configFile, const char* logFile, ConfigFileType cft);


#endif//__RM_MEANDER_INTERFACE_H__20100329__
