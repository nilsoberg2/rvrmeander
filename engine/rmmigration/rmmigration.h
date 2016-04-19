#ifndef __RMMIGRATION_H_____________________20090406175151__
#define __RMMIGRATION_H_____________________20090406175151__

#include "../common.h"
#include "../rmio/rmseries.h"
#include "../rmio/rmlog.h"
#include "../rmprep/rmprep.h"
#include "../rmbankerosion/rmbankerosion.h"
#include "rmmigrationdata.h"
#include "rmmigration_impexp.h"


class LIBRMMIGRATION_DLL_IMP_EXP RMMigration
    : public RMLoggerImpl
{
public:
	virtual bool Preprocess(int iter, const RMPrepData* prepData, RMMigrationData* data);
	virtual bool Migrate(int iter, const RMPrepData* prepData, const RMBankErosionData* beData, RMMigrationData* data);
	virtual ~RMMigration();
	
	virtual void CalculateDepthStraightChannel(RMMigrationData*);																		// Calculate Depth Straight Channel
	virtual void CalculateInitialWidth(RMBankErosionData* data, RMMigrationData* migdata);																				// CalculateInitialWidth
	
	virtual void Cutoff(RMMigrationData*);
	virtual void CutoffPBA(RMMigrationData*,RMBankErosionData*);

protected:
	virtual void ComputeNewCenterline(const RMPrepData* prepData, const RMBankErosionData* beData, RMMigrationData* data);

	virtual void ComputeXandY(const RMPrepData* prepData, RMMigrationData* data);

    // Compute "straight channel resistance coefficient"
    virtual double ComputeSCRCoeff(double depth, double sediment_size);
    // Compute "half the channel-width-to-depth ratio"
    virtual double ComputeHCWDRatio(double depth, double width);
    // Compute "froude number squared"
    virtual double ComputeFroudeSquared(double flow, double depth, double width);
    // Compute "parameter proportional to erosion rate"
    virtual double ComputePPERate(double bank_erosion_coeff, double flow, double depth,
		double width, double num_years, double num_iter);
};


#endif//__RMMIGRATION_H_____________________20090406175151__
