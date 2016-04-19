
#include <cmath>
#include "rmhydrodynamics.h"
#include "../rmmigration/rmmigrationdata.h"

RMHydrodynamics::~RMHydrodynamics()
{
}

void RMHydrodynamicsData::InitBase(int length, int width)
{
    u.init(length, width);
    v.init(length, width);
    h.init(length, width);
    d.init(length, width);
    e.init(length, width);
    U1.init(length, width);
    V1.init(length, width);
    H1.init(length, width);
    D1.init(length, width);
    E1.init(length, width);
    UU.init(length, width);
    VV.init(length, width);
    HH.init(length, width);
    DD.init(length, width);
    EE.init(length, width);
    TauS.init(length, width);
    TauN.init(length, width);
    VEL.init(length, width);
    Ux.init(length, width);
    Vy.init(length, width);
    TAU.init(length, width);
    TauX.init(length, width);
    TauY.init(length, width);
    CF.init(length, width);
    ub.init(length);
	OldUU.init(length, width);		//ADDITION MAR2010
	OldVV.init(length, width);		//ADDITION MAR2010
	OldDD.init(length, width);		//ADDITION MAR2010
	OldEE.init(length, width);		//ADDITION MAR2010	
	OldHH.init(length, width);		//ADDITION MAR2010
	OldVEL.init(length, width);		//ADDITION MAR2010
	OldUx.init(length, width);		//ADDITION MAR2010
	OldVy.init(length, width);		//ADDITION MAR2010
	OldCF.init(length, width);		//ADDITION MAR2010
	OldTAU.init(length, width);		//ADDITION MAR2010
	OldTauX.init(length, width);	//ADDITION MAR2010
	OldTauY.init(length, width);	//ADDITION MAR2010
}

void RMHydrodynamicsData::InitBaseNewHydroValues(int length, int width)  //ADDITION MAR2010
{
    u.init(length, width);
    v.init(length, width);
    h.init(length, width);
    d.init(length, width);
    e.init(length, width);
    U1.init(length, width);
    V1.init(length, width);
    H1.init(length, width);
    D1.init(length, width);
    E1.init(length, width);
    UU.init(length, width);
    VV.init(length, width);
    HH.init(length, width);
    DD.init(length, width);
    EE.init(length, width);
    TauS.init(length, width);
    TauN.init(length, width);
    VEL.init(length, width);
    Ux.init(length, width);
    Vy.init(length, width);
    TAU.init(length, width);
    TauX.init(length, width);
    TauY.init(length, width);
    CF.init(length, width);
    ub.init(length);
}

void RMHydrodynamicsData::InitBaseOldHydroValues(int length, int width) //ADDITION MAR2010
{
	OldUU.init(length, width);		//ADDITION MAR2010
	OldVV.init(length, width);		//ADDITION MAR2010
	OldDD.init(length, width);		//ADDITION MAR2010
	OldEE.init(length, width);		//ADDITION MAR2010	
	OldHH.init(length, width);		//ADDITION MAR2010
	OldVEL.init(length, width);		//ADDITION MAR2010
	OldUx.init(length, width);		//ADDITION MAR2010
	OldVy.init(length, width);		//ADDITION MAR2010
	OldCF.init(length, width);		//ADDITION MAR2010
	OldTAU.init(length, width);		//ADDITION MAR2010
	OldTauX.init(length, width);	//ADDITION MAR2010
	OldTauY.init(length, width);	//ADDITION MAR2010
}