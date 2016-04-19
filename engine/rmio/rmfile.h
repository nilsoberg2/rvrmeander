#ifndef RMFILE_H__20090106__
#define RMFILE_H__20090106__

#include "rmio.h"
#include "rmseries.h"


LIBRMIO_DLL_IMP_EXP int LoadSeries(const char* file, int type, RMSeries& series);
LIBRMIO_DLL_IMP_EXP int SaveSeries(const char* file, int type, RMSeries& series);


#endif//RMFILE_H__20090106__
