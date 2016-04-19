#ifndef RM_PREP_INTERFACE_
#define RM_PREP_INTERFACE_

#include "../common.h"
#include <cstddef>


#define EXTERN extern "C"


#if defined(LIBPREP_EXPORTS)
#define RMPREPDLL_DLL_API DLL_EXPORT
#else
#define RMPREPDLL_DLL_API DLL_IMPORT
#endif


// This version uses the INI file format as default.
EXTERN
RMPREPDLL_DLL_API
int RMPrep_Initialize(double* xp, double* yp, int numPoints);

EXTERN
RMPREPDLL_DLL_API
int RMPrep_ComputeXYC(int objPtr, double* xp, double* yp, double* sp, double* cp, int numPoints);

EXTERN
RMPREPDLL_DLL_API
int RMPrep_GetNumberOfInterpPoints(int objPtr);

EXTERN
RMPREPDLL_DLL_API
int RMPrep_Cleanup(int objPtr);


#endif//RM_PREP_INTERFACE_
