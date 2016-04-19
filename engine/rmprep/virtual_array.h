#ifndef VIRTUAL_ARRAY_H__
#define VIRTUAL_ARRAY_H__


// Virtual Arrays allow us to encapsulate various arrays and provide
// a uniform interface to accessing them for some of the RMPrep functions.

class Virtual2ColArray
{
protected:
    int _count;
public:
    int size() { return _count; }
    virtual void get(int i, double& x, double& y) { x = 0.0; y = 0.0; }
    virtual void set(int i, double x, double y) {}
};


class TwoDDoubleArray : public Virtual2ColArray
{
protected:
    double** _xy;
public:
    virtual void get(int i, double& x, double& y) { x = _xy[i][0]; y = _xy[i][1]; }
    void setobj(double** xy, int size) { _xy = xy; _count = size; }
    virtual void set(int i, double x, double y) { _xy[i][0] = x; _xy[i][1] = y; }
};


class TwoColDoubleArray : public Virtual2ColArray
{
protected:
    double* _x;
    double* _y;
public:
    virtual void get(int i, double& x, double& y) { x = _x[i]; y = _y[i]; }
    void setobj(double* x, double* y, int size) { _x = x; _y = y; _count = size; }
    virtual void set(int i, double x, double y) { _x[i] = x; _y[i] = y; }
};


#endif//VIRTUAL_ARRAY_H__
