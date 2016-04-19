
#include <vector>

#define RESERVE_FACTOR 15

	
	class FlowVariables
	{
	public:
	double area, depth, discharge, stage, velocity, frictionslope, effectivefrictionfactor, hydraulicradius, wettedperimeter;
	};  

    class SizeDistributionDescriptor
	{
	public:
	int numberofsizefractions;
    std::vector<double> sizefraction; //NOO																										//   DIMENSION(numberofsizeclasses)																																//        REAL(REAL_KIND) :: d16, d50, d84, d90, dmean, dmeansg
    
    //NOO
    void SetNumberOfSizeFractions(int num)
    {
        numberofsizefractions = num;
        sizefraction.resize(num);
        sizefraction.reserve(num + RESERVE_FACTOR);
    }
	};

    class BankMaterialDescriptor
	{
	public:
    int bottomindex, topindex;																										//bottomindex      index in bank profile of intersect of bottom of soil layer and bank face
	double phib, reposeangle, cohesion;	
	//cohesion                 soil cohesion (N/m^2)
	//phib                     angle indicating the increase in shear strength for an increase in matric suction (rad)
	//reposeangle              soil friction angle (rad)
	
	
																				//topindex         index in bank profile of intersect of top of soil layer and bank face
    double criticalshearstress, erosionrate, unitweight, elevation;																	// elevation of top of the bank-material layer (m) 
	SizeDistributionDescriptor composition;
	};

	class Coordinates																												// coordinates of a node
	{
	public:
	double station, elevation, x, y, dimensionlessstation;
	};

	class FailureParameters
	{
	public:
    bool tensioncrack;
    double apparentcohesion, factorofsafety, hydrostaticforce, mobilizedshear;
    double normalforce, porewaterforce, waterweight, weight, weightslices, angle; //MMMMTEMP Added the last two
    Coordinates bottomvalleypoint, topvalleypoint;
	};

	class Bank																														// a bank
	{
	public:
	int numberoflayers;																												// Done differently in CONCEPTS, since material is a allocatable pointer
	int initialnumberoflayers;	//DM-FEB2011	
	int numberofnodes;																												// Done differently in CONCEPTS, since nodes is a allocatable pointer	
	int direction;																													// bank identifier: -1 = leftbank; +1 = rightbank                           
	int	undercut;																													// index of the point on the bank profile that sticks out the most
	int undercuttoe;
	double groundwatertable;
	double Karmor;				//DM-FEB2012
	double TotalAblock;			//DM-FEB2012
	double AblockCantilever;	//DM-FEB2012
	double AblockPlanar;		//DM-FEB2012
	double PlanarFailureAngle;		//DM-FEB2012
	Coordinates PlanarFailureToe;	//DM-FEB2012
	double FactorOfSafety;			//DM-FEB2012
	double FloodplainElevation; //MMMMTEMP Floodplain elevation

    //NOO added this so we can easily change vector size
    void SetNumberOfNodes(int num)
    {
        numberofnodes = num;
        nodes.resize(num); // + 1 because code uses numberofnodes as an index
        nodes.reserve(num + RESERVE_FACTOR);
    }

    //NOO
    void SetNumberOfLayers(int num)
    {
        numberoflayers = num;
        material.resize(num);
        material.reserve(num + RESERVE_FACTOR);
    }

    std::vector<Coordinates> nodes; //NOO																									// Max number of nodes in a bank (this dimension shouldn't be specified)
    std::vector<BankMaterialDescriptor> material;  //NOO																							// Max number of materials (this dimension shouldn't be specified)
																																	//CON		INTEGER :: direction, hasbuffer, undercut, undercuttoe
																																	//			REAL(REAL_KIND) :: frictionfactor, groundwatertable
																																	//			TYPE(FailureParameters) :: failureparameters
																																	//			TYPE(LookUpTableReal) :: conveyance, flowarea, hydraulicradius, topwidth, wettedperimeter
																																	//			TYPE(VegetationDescriptor) :: vegetationattributes
	//Coordinates nodesforTecPlot[100];
	FailureParameters failureparameters;
	};

	class SedimentHorizon
	{
	public:
	double criticalerosionshearstress, erosionrate;
	};

	class ChannelBed
	{
	public:
	SedimentHorizon	surfacelayer;
	double frictionfactor;
	};
	
	class CrossSection																												// a cross section
	{
	public:
	int id;
	char name;
	double rivermile, effectivefrictionfactor;
	double xcenter, ycenter;
	Bank leftbank, rightbank;
	FlowVariables flow;
	ChannelBed bed;
	};

	class GridPoint																													// a node on the grid
	{
	public:
	int id;
	CrossSection xsection;			
	//CrossSection Oldxsection;
	};

	class Grid																														// a grid
	{
	public:
	int id, numberofnodes;
    std::vector<GridPoint> nodes;																											// Max number of nodes in a grid (this dimension shouldn't be specified)			
    
    //NOO added this so we can easily resize vector
    void SetNumberOfNodes(int num)
    {
        numberofnodes = num;
        nodes.resize(num);
        nodes.reserve(num + RESERVE_FACTOR);
    }
	};	