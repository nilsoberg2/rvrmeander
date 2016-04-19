#ifndef RMIO_H__20090106__
#define RMIO_H__20090106__

#include "../common.h"

#if defined(LIBRMIO_EXPORTS)
#define LIBRMIO_DLL_IMP_EXP DLL_EXPORT
#define LIBRMIO_DLL_EXTERN
#elif defined(LIBRMIO_STATIC)
#define LIBRMIO_DLL_IMP_EXP
#define LIBRMIO_DLL_EXTERN
#else
#define LIBRMIO_DLL_IMP_EXP DLL_IMPORT
#define LIBRMIO_DLL_EXTERN  extern
#endif


#define DIM_X                   1
#define DIM_XY                  2
#define DIM_XYZ                 3


#define FILE_DBF                1
#define FILE_NETCDF             2
#define FILE_BINARY             3
#define FILE_TEXT               4
#define FILE_TECPLOT			5


#endif//RMIO_H__20090106__
