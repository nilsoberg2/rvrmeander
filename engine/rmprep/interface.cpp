
#include "interface.h"
#include "rmprep.h"
#include "virtual_array.h"


EXTERN int RMPrep_Initialize(double* xp, double* yp, int numPoints)
{
    RMPrep* p = new RMPrep();
    TwoColDoubleArray* a = new TwoColDoubleArray();
    a->setobj(xp, yp, numPoints);
    p->InitializeVirtual(a, false);
    return (int)p;
}


EXTERN int RMPrep_ComputeXYC(int objPtr, double* xp, double* yp, double* sp, double* cp, int numPoints)
{
    RMPrep* p = (RMPrep*)objPtr;
    int curvSize = 0;
    TwoColDoubleArray* xy = new TwoColDoubleArray();
    xy->setobj(xp, yp, numPoints);
    TwoColDoubleArray* sc = new TwoColDoubleArray();
    sc->setobj(sp, cp, numPoints);
    p->GetCurvaturePreCreated(sc, xy, curvSize);
    return curvSize;
}

EXTERN int RMPrep_Cleanup(int objPtr)
{
    RMPrep* p = (RMPrep*)objPtr;
    delete p;
    return 1;
}

EXTERN int RMPrep_GetNumberOfInterpPoints(int objPtr)
{
    RMPrep* p = (RMPrep*)objPtr;
    return p->GetRecommendedNumPoints();
}
