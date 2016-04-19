#ifndef _PRECISIONTIMER_H_
#define _PRECISIONTIMER_H_

// By Andreas Masur

#ifdef _MSC_VER // Visual C++
#include <windows.h>
#define LINT	LARGE_INTEGER
#else
#define LINT long
#endif

class CPrecisionTimer
{
	LINT lFreq, lStart;
public:
	CPrecisionTimer()
	{
#ifdef _MSC_VER
		QueryPerformanceFrequency(&lFreq);
#else
		lFreq = 0;
#endif
	}

	inline void Start()
	{
#ifdef _MSC_VER
		QueryPerformanceCounter(&lStart);
#else
		lStart = 0;
#endif
	}

	inline double Stop()
	{
		// Return duration in seconds...
		LINT lEnd;

#ifdef _MSC_VER
		QueryPerformanceCounter(&lEnd);
		return (double(lEnd.QuadPart - lStart.QuadPart) / lFreq.QuadPart);
#else
		return 0.0;
#endif
	}
};


#define CREATE_TIMER(X)		CPrecisionTimer X;
#define START_TIMER(X)		X.Start();
#define STOP_TIMER(X)		X.Stop()
#define STOP_TIMER_RV(X)	X.Stop();


#endif // _PRECISIONTIMER_H_