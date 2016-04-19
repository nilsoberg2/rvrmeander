#ifndef __DEBUG_PRINTING_H__________________20090413150404__
#define __DEBUG_PRINTING_H__________________20090413150404__

#ifdef _DEBUG
#define WANT_DEBUG_PRINTING
#endif

#ifdef WANT_DEBUG_PRINTING

#include "../common.h"
#include <cstdio>

#define DEBUG_PRINTING_FORMAT					"%d %.10f %.10f \n"
#define START_DEBUG_PRINTING					StartDebugPrinting();
#define STOP_DEBUG_PRINTING						StopDebugPrinting();
#define DEBUG_PRINT3(FH, x, y, z)				fprintf(FH, DEBUG_PRINTING_FORMAT, x, y, z);
#define DEBUG_PRINT3_ARRAY(size, FH, y, z)  \
	for (int i__ = 0; i__ <= ##size##; i__++) \
		fprintf(##FH##, DEBUG_PRINTING_FORMAT, i__+1, ##y##(i__), ##z##(i__));	
#define DEBUG_PRINT3_ARRAY2(size, FH, y)  \
	for (int i__ = 0; i__ <= ##size##; i__++) \
		fprintf(##FH##, DEBUG_PRINTING_FORMAT, i__+1, ##y##(i__,0), ##y##(i__,1));

#else

#define DEBUG_PRINTING_FORMAT
#define START_DEBUG_PRINTING
#define STOP_DEBUG_PRINTING
#define DEBUG_PRINT3(FH, x, y, z)				
#define DEBUG_PRINT3_ARRAY(size, FH, x, y)  
#define DEBUG_PRINT3_ARRAY2(size, FH, y)

#endif


#endif//__DEBUG_PRINTING_H__________________20090413150404__
