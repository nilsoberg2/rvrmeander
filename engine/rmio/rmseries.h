#ifndef RMSERIES_H__20090106__
#define RMSERIES_H__20090106__

#include "rmio.h"

#define NAME_LENGTH 200

class LIBRMIO_DLL_IMP_EXP RMSeries
{
public:
    int length, width;
    RMSeries();
    RMSeries(const RMSeries& s);
    RMSeries& operator=(const RMSeries& s);
    ~RMSeries();
    void init(int length_, int width_);
    void init(int length_, int width_, double** xy);
    void init(int length_);
    void init(int length_, double** x);
    void init(int length_, double* x);
    double* nextp();
    void clear();
    void resetp();
	//double& vals(int idx);
	//double& vals(int idx1, int idx2);
	//double vals(int idx) const;
	//double vals(int idx1, int idx2) const;
	double**& get_pointer(); // dangerous!  VERY DANGEROUS!  ONLY USE THIS IF YOU ARE 100% ABSOLUTELY COMPLETELY FULLY CONFIDENTLY SURE THAT YOU KNOW WHAT YOU ARE DOING!!!!!!!
	inline double& operator()(int idx) { return vals_raw[idx][0]; }
	inline double operator()(int idx) const { return vals_raw[idx][0]; }
	inline double& operator()(int idx1, int idx2) { return vals_raw[idx1][idx2]; }
	inline double operator()(int idx1, int idx2) const { return vals_raw[idx1][idx2]; }
protected:
    double** vals_raw;
    int iterPos;
	char name[NAME_LENGTH];
    void initCopy(int length_, int width_, double** input);
    void initCopy(int length_, double* input);
};


#ifndef NULL
#define NULL 0
#endif

#endif//RMSERIES_H__20090106__
