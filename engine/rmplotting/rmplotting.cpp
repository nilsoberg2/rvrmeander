#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES		//DM-FEB2011  to use M_PI
#include <math.h>				//DM-FEB2011  to use M_PI			
#include "rmplotting.h"
#include "../rmhydrodynamics/rmhydrodynamics.h"
#include <cstring>
#include <map>


RMPlotting::RMPlotting()
{
	m_Tecplot1D = NULL;
	m_Tecplotuv2D = NULL;
	m_Tecplothde2D = NULL;
	m_TecplotU1V12D = NULL;
	m_TecplotH1D1E12D = NULL;
	m_TecplotUUVV2D = NULL;
	m_TecplotHHDDEE2D = NULL;
	m_TecplotVELandTAU2D = NULL;
	m_TecPlotSections2D = NULL;
	m_TecPlotHydro3D = NULL;
	m_TecPlotBedBanks3D = NULL;
	m_TecplotGrid = NULL;  //DM-FEB2011
	KarmorFile = NULL; //DM-FEB2012
	IndicesFile = NULL;//MMMMTEMP Indices file

	//m_ReworkedAreas = NULL;										//DM-FEB2011 Commented out
	//m_PlanarFailure = NULL;										//DM-FEB2011 Commented out
    //m_HeterogeneityAtCenterline = NULL;   //HETEROGENEITY			//DM-FEB2011 Commented out
}

RMPlotting::~RMPlotting()
{
	if (m_Tecplot1D != NULL)
		fclose(m_Tecplot1D);
	if (m_Tecplotuv2D != NULL)
		fclose(m_Tecplotuv2D);
	if (m_Tecplothde2D != NULL)
		fclose(m_Tecplothde2D);
	if (m_TecplotU1V12D != NULL)
		fclose(m_TecplotU1V12D);
	if (m_TecplotH1D1E12D != NULL)
		fclose(m_TecplotH1D1E12D);
	if (m_TecplotUUVV2D != NULL)
		fclose(m_TecplotUUVV2D);
	if (m_TecplotHHDDEE2D != NULL)
		fclose(m_TecplotHHDDEE2D);
	if (m_TecplotVELandTAU2D != NULL)
		fclose(m_TecplotVELandTAU2D);
	if (m_TecPlotSections2D != NULL)
		fclose(m_TecPlotSections2D);
	if (m_TecPlotHydro3D != NULL)
		fclose(m_TecPlotHydro3D);
	if (m_TecPlotBedBanks3D != NULL)
		fclose(m_TecPlotBedBanks3D);
	if (m_TecplotGrid != NULL)			//DM-FEB2011
		fclose(m_TecplotGrid);			//DM-FEB2011
	//if (m_ReworkedAreas != NULL)									//DM-FEB2011 Commented out
		//fclose(m_ReworkedAreas);									//DM-FEB2011 Commented out
	//if (m_PlanarFailure != NULL)									//DM-FEB2011 Commented out
	//	fclose(m_PlanarFailure);									//DM-FEB2011 Commented out
	//if (m_HeterogeneityAtCenterline != NULL)//HETEROGENEITY		//DM-FEB2011 Commented out
	//	fclose(m_HeterogeneityAtCenterline);//HETEROGENEITY			//DM-FEB2011 Commented out
	if (KarmorFile != NULL)			//DM-FEB2012
		fclose(KarmorFile);			//DM-FEB2012
	if (IndicesFile != NULL)			//MMMMTEMP Indices file
		fclose(IndicesFile);			//MMMMTEMP Indices file
}

void RMPlotting::SetFrequency(int freq)
{
	m_frequency = freq;
}

bool RMPlotting::CanPlot(int iter)
{		
	if (m_frequency > 0 && (iter % m_frequency) == 0)
		return true;
	else
		return false;
}

void RMPlotting::SetTecplotOutput_1D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_Tecplot1D = NULL;
        return;
    }

	m_Tecplot1D = fopen(file, "w");
	fprintf(m_Tecplot1D,"TITLE = \"Meander migration\"\n");
	fprintf(m_Tecplot1D,"VARIABLES = \"x\" \"y\" \n");
}

void RMPlotting::SetTecplotOutput_uv2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_Tecplotuv2D = NULL;
        return;
    }

	m_Tecplotuv2D = fopen(file, "w");
	fprintf(m_Tecplotuv2D,"TITLE = \"2D dimensionless hydrodynamics (u, v, C, and theta)\"\n");			//DM-FEB2011
	fprintf(m_Tecplotuv2D,"VARIABLES = \"x\" \"y\" \"u\" \"v\" \"C\" \"theta\" ");						//DM-FEB2011
}

void RMPlotting::SetTecplotOutput_hde2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_Tecplothde2D = NULL;
        return;
    }

	m_Tecplothde2D = fopen(file, "w");									
	fprintf(m_Tecplothde2D,"TITLE = \"2D dimensionless hydrodynamics (h, d and e)\"\n");
	fprintf(m_Tecplothde2D,"VARIABLES = \"x\" \"y\" \"h\" \"d\" \"e\" ");
}

void RMPlotting::SetTecplotOutput_U1V12D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotU1V12D = NULL;
        return;
    }

	m_TecplotU1V12D = fopen(file, "w");
	fprintf(m_TecplotU1V12D,"TITLE = \"2D dimensioned hydrodynamics (U1 and V1)\"\n");
	fprintf(m_TecplotU1V12D,"VARIABLES = \"X\" \"Y\" \"U1\" \"V1\" ");
}

void RMPlotting::SetTecplotOutput_H1D1E12D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotH1D1E12D = NULL;
        return;
    }

	m_TecplotH1D1E12D = fopen(file, "w");									
	fprintf(m_TecplotH1D1E12D,"TITLE = \"2D dimensioned hydrodynamics (H1, D1 and E1)\"\n");
	fprintf(m_TecplotH1D1E12D,"VARIABLES = \"X\" \"Y\" \"H1\" \"D1\" \"E1\" ");
}

void RMPlotting::SetTecplotOutput_UUVV2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotUUVV2D = NULL;
        return;
    }

	m_TecplotUUVV2D = fopen(file, "w");
	fprintf(m_TecplotUUVV2D,"TITLE = \"2D dimensioned hydrodynamics (UU and VV)\"\n");
	fprintf(m_TecplotUUVV2D,"VARIABLES = \"X\" \"Y\" \"UU\" \"VV\" ");
}

void RMPlotting::SetTecplotOutput_HHDDEE2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotHHDDEE2D = NULL;
        return;
    }

	m_TecplotHHDDEE2D = fopen(file, "w");									
	fprintf(m_TecplotHHDDEE2D,"TITLE = \"2D dimensioned hydrodynamics (HH, DD and EE)\"\n");
	fprintf(m_TecplotHHDDEE2D,"VARIABLES = \"X\" \"Y\" \"HH\" \"DD\" \"EE\" ");
}

void RMPlotting::SetTecplotOutput_VELandTAU2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotVELandTAU2D = NULL;
        return;
    }

	m_TecplotVELandTAU2D = fopen(file, "w");									
	fprintf(m_TecplotVELandTAU2D,"TITLE = \"2D dimensioned hydrodynamics (Ux, Vy, VEL, CF, TauS, TauN and TAU)\"\n");		// CORRECTION MAY 2010
	fprintf(m_TecplotVELandTAU2D,"VARIABLES = \"X\" \"Y\" \"Ux\" \"Vy\" \"VEL\" \"CF\" \"TauS\" \"TauN\" \"TAU\" ");		// CORRECTION MAY 2010		
}

void RMPlotting::SetTecplotOutput_Sections2D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecPlotSections2D = NULL;
        return;
    }

	m_TecPlotSections2D=fopen(file, "w");				
	fprintf(m_TecPlotSections2D, "TITLE = \"Sections results \"\n");
	fprintf(m_TecPlotSections2D, "VARIABLES = \"X\" \"Y\" \n");
}

void RMPlotting::SetTecplotOutput_Hydro3D(const char* file)
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecPlotHydro3D = NULL;
        return;
    }

	m_TecPlotHydro3D=fopen(file, "w");						
	fprintf(m_TecPlotHydro3D, "TITLE = \"Hydrodynamics results \"\n");
	fprintf(m_TecPlotHydro3D, "VARIABLES = \"X\" \"Y\" \"Z\" \n");
}

void RMPlotting::SetTecplotOutput_BedBanks3D(const char* file)
{	
    if (file == NULL || strlen(file) == 0)
    {
        m_TecPlotBedBanks3D = NULL;
        return;
    }

	m_TecPlotBedBanks3D=fopen(file, "w");						
	fprintf(m_TecPlotBedBanks3D, "TITLE = \"Bed and banks \"\n");
	fprintf(m_TecPlotBedBanks3D, "VARIABLES = \"X\" \"Y\" \"Z\" \n");
}

void RMPlotting::SetOutput_ReworkedAreas(const char* file)
{	
	//m_ReworkedAreas=fopen(file, "w");																											//COMMENTED OUT IN FEB2010
	//fprintf(m_ReworkedAreas, "Cumulative reworked areas(erosion: positive contrinution, deposition: negative contribution) \n");				//COMMENTED OUT IN FEB2010
	//fprintf(m_ReworkedAreas, "Reworked areas \n");																							//COMMENTED OUT IN FEB2010
	//fprintf(m_ReworkedAreas, "Node	t (years)	ReworkedAreaLeft (m2)	ReworkedAreaRight (m2)	TotalErodedVolume (m3)\n");					//COMMENTED OUT IN FEB2010
}

/* //DM-FEB2011  Commented out
void RMPlotting::SetOutput_HeterogeneityAtCenterline(const char* file)	//HETEROGENEITY
{	
    if (file == NULL || strlen(file) == 0)
    {
        m_HeterogeneityAtCenterline = NULL;
        return;
    }

	m_HeterogeneityAtCenterline=fopen(file, "w");

	fprintf(m_HeterogeneityAtCenterline,"Time [years]	Node number	Longitudinal distance [m]	Critical shear stress [Pa] \n");
}
*/

void RMPlotting::SetOutput_PlanarFailure(const char* file)
{	
    if (file == NULL || strlen(file) == 0)
    {
        m_PlanarFailure = NULL;
        return;
    }

	m_PlanarFailure=fopen(file, "w");						
}

void RMPlotting::TecPlot1D(int j, double deltaT, const RMMigrationData* migdata)
{
    if (m_Tecplot1D == NULL)
		return;
		
	double PrintTime = j * deltaT/365.0;			// PrintTime in years
	fprintf(m_Tecplot1D, "\nZONE T=\"%lf\" I=%d,  DATAPACKING=POINT\n", PrintTime, migdata->channel.centerline.length);
	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)						
		fprintf(m_Tecplot1D, "%12.5lf %12.5lf \n", migdata->channel.centerline(i,0)*migdata->channel.WIDTH/2, migdata->channel.centerline(i,1)*migdata->channel.WIDTH/2);  
}

void RMPlotting::TecPlot2D(int j, double deltaT, const RMMigrationData* migData, const RMHydrodynamicsData* hData, RMPrepData* prepData)
{
	//if (m_Tecplotuv2D == NULL ||			//DM-FEB2011 Commented out
	//    m_Tecplothde2D == NULL ||			//DM-FEB2011 Commented out
	//    m_TecplotU1V12D == NULL ||		//DM-FEB2011 Commented out
	//    m_TecplotH1D1E12D == NULL ||		//DM-FEB2011 Commented out
	//    m_TecplotUUVV2D == NULL ||		//DM-FEB2011 Commented out
	//    m_TecplotHHDDEE2D == NULL ||		//DM-FEB2011 Commented out
	//    m_TecplotVELandTAU2D == NULL)		//DM-FEB2011 Commented out
    //{										//DM-FEB2011 Commented out
    //    return;							//DM-FEB2011 Commented out
    //}										//DM-FEB2011 Commented out

	int width = migData->channel.nnodes;
	int length = migData->channel.centerline.length;

	double PrintTime = j*deltaT/365;			// PrintTime in years

	if(m_Tecplotuv2D != NULL)fprintf(m_Tecplotuv2D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL) \n", PrintTime, length, width);											//DM-FEB2011
	if(m_Tecplothde2D != NULL)fprintf(m_Tecplothde2D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL) \n", PrintTime, length, width);													//DM-FEB2011
	if(m_TecplotU1V12D != NULL)fprintf(m_TecplotU1V12D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL) \n", PrintTime, length, width);														//DM-FEB2011
	if(m_TecplotH1D1E12D != NULL)fprintf(m_TecplotH1D1E12D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL) \n", PrintTime, length, width);											//DM-FEB2011
	if(m_TecplotUUVV2D != NULL)fprintf(m_TecplotUUVV2D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL) \n", PrintTime, length, width);														//DM-FEB2011 
	if(m_TecplotHHDDEE2D != NULL)fprintf(m_TecplotHHDDEE2D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL) \n", PrintTime, length, width);											//DM-FEB2011
	if(m_TecplotVELandTAU2D != NULL)fprintf(m_TecplotVELandTAU2D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL) \n", PrintTime, length, width);	//DM-FEB2011 
	for (int j = 0; j <= width-1; j++)
	{
		for (int i = 0; i <= length-1; i++)
		{ 	
			if(m_Tecplotuv2D != NULL)fprintf(m_Tecplotuv2D, "%lf	%lf	%lf	%lf	%lf	%lf \n", migData->XX(i,j), migData->YY(i,j), hData->u(i,j), hData->v(i,j), prepData->curvature(i), prepData->theta(i)*180/M_PI);				//DM-FEB2011	
			if(m_Tecplothde2D != NULL)fprintf(m_Tecplothde2D, "%lf	%lf	%lf	%lf %lf\n", migData->XX(i,j), migData->YY(i,j), hData->h(i,j), hData->d(i,j), hData->e(i,j));															//DM-FEB2011
			if(m_TecplotU1V12D != NULL)fprintf(m_TecplotU1V12D, "%lf	%lf	%lf	%lf\n", migData->XX(i,j), migData->YY(i,j), hData->U1(i,j), hData->V1(i,j));																		//DM-FEB2011
			if(m_TecplotH1D1E12D != NULL)fprintf(m_TecplotH1D1E12D, "%lf	%lf	%lf	%lf %lf\n", migData->XX(i,j), migData->YY(i,j), hData->H1(i,j), hData->D1(i,j), hData->E1(i,j));												//DM-FEB2011 
			if(m_TecplotUUVV2D != NULL)fprintf(m_TecplotUUVV2D, "%lf	%lf	%lf	%lf	\n", migData->XX(i,j), migData->YY(i,j), hData->UU(i,j), hData->VV(i,j));																		//DM-FEB2011	
			if(m_TecplotHHDDEE2D != NULL)fprintf(m_TecplotHHDDEE2D, "%lf	%lf	%lf	%lf %lf\n", migData->XX(i,j), migData->YY(i,j), hData->HH(i,j), hData->DD(i,j), hData->EE(i,j));												//DM-FEB2011 
			if(m_TecplotVELandTAU2D != NULL)fprintf(m_TecplotVELandTAU2D, "%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf\n", migData->XX(i,j), migData->YY(i,j), hData->Ux(i,j), hData->Vy(i,j), hData->VEL(i,j), hData->CF(i,j), hData->TauS(i,j), hData->TauN(i,j), hData->TAU(i,j));		// CORRECTION MAY 2010	//DM-FEB2011
		}
	}
}

void RMPlotting::TecPlotBedBank3D(int j, double deltaT, RMBankErosionData* data, RMMigrationData* migdata, RMHydrodynamicsData* hdata)			
{
    if (m_TecPlotBedBanks3D == NULL || m_TecPlotHydro3D == NULL)
        return;

    using namespace std; //NOO

	int firstnode;
    double PrintTime = j * deltaT/365.0; 			// PrintTime in years
	int maxnumbernodesonbank = 0;
	int sumnodes;
	int nodestoaddleft, nodestoaddright, remainingnodestoaddleft, remainingnodestoaddright;

	if (j == 0)
		{
			logmsg("TecPlotBedBank3D (initial) \n");
		}
	else
		{
			logmsg("TecPlotBedBank3D \n");
		}

	// Calculating maximum number of nodes on bank
	firstnode = 0;
	
	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{  
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
					{
						if(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes > maxnumbernodesonbank)
							{maxnumbernodesonbank = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes;}
						if(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes > maxnumbernodesonbank)
							{maxnumbernodesonbank = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes;}
					}
			firstnode = firstnode+data->grids[igrid].numberofnodes;
		}
	
	sumnodes = 2 * maxnumbernodesonbank + migdata->channel.nnodes;

	// Prepare nodes to print in the TecPlot file
	firstnode = 0;
	nodestoaddleft = 0;
	nodestoaddright = 0;
	remainingnodestoaddleft = 0;
	remainingnodestoaddright = 0;

    //NOO start
    typedef map<int, Coordinates> bank_node_map_t;
    typedef map<int, bank_node_map_t> node_map_t;
    typedef map<int, node_map_t> grid_map_t;

    grid_map_t leftBankGridMap;
    grid_map_t rightBankGridMap;
    //NOO end

	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{  		
            node_map_t leftNodeMap; //NOO
            node_map_t rightNodeMap; //NOO
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)		
				{	
                    bank_node_map_t leftBank; //NOO

					// Left bank
					nodestoaddleft = maxnumbernodesonbank - data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes;
					remainingnodestoaddleft = nodestoaddleft;
					for(int ivertex = 0; ivertex <= maxnumbernodesonbank - 1; ivertex++)
						{
							if(ivertex == 0)
								{
									leftBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex]));
								}
							else if(remainingnodestoaddleft > 0)
								{
									leftBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.leftbank.nodes[1]));
									remainingnodestoaddleft = remainingnodestoaddleft - 1;
								}
							else
								{
									leftBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex-nodestoaddleft]));								
								}
						}
					
                    bank_node_map_t rightBank; //NOO
					// Right bank
					nodestoaddright = maxnumbernodesonbank - data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes;
					remainingnodestoaddright = nodestoaddright;
					for(int ivertex = 0; ivertex <= maxnumbernodesonbank - 1; ivertex++)
						{
							
							if(ivertex <= data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 2)
								{
									rightBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex])); //NOO
								}
							else if(remainingnodestoaddright > 0)
								{
									rightBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.rightbank.nodes[data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 2])); //NOO									
									remainingnodestoaddright = remainingnodestoaddright - 1;
								}
							else
								{
									rightBank.insert(make_pair(ivertex, data->grids[igrid].nodes[inode].xsection.rightbank.nodes[data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 1])); //NOO								
								}
							
						}					
					nodestoaddleft = 0;
					nodestoaddright = 0;

                    leftNodeMap.insert(make_pair(inode, leftBank));
                    rightNodeMap.insert(make_pair(inode, rightBank));
				}
			firstnode = firstnode+data->grids[igrid].numberofnodes;

            leftBankGridMap.insert(make_pair(igrid, leftNodeMap)); //NOO
            rightBankGridMap.insert(make_pair(igrid, rightNodeMap)); //NOO
		}

	// Tecplot bed and banks
	firstnode = 0;
	
	fprintf(m_TecPlotBedBanks3D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL) \n", PrintTime, migdata->channel.centerline.length, sumnodes);

	for(int ivertex = 0; ivertex <= sumnodes - 1; ivertex++)
		{
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  		
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)		
						{		//logmsg("PRINT = %d	%d  \n", inode, data->grids[igrid].numberofnodes);				
							if (ivertex <= maxnumbernodesonbank - 1)   
								{		
                                    Coordinates c = leftBankGridMap[igrid][inode][ivertex];
									fprintf(m_TecPlotBedBanks3D, "%lf %lf %lf \n", c.x, c.y, c.elevation);	
if(inode == data->MonitorNode)
{
//if(data->grids[igrid].nodes[inode].xsection.leftbank.nodesforTecPlot[ivertex].x<-10000)logmsg("AAAAAAAAAAAAAAAAAAAAAAAAa = %d\n", inode);
}								
								}
							
							else if (ivertex>= maxnumbernodesonbank + migdata->channel.nnodes)		// Nodes might need to be added for TecPlot mesh visualization		
								{
                                    Coordinates c = rightBankGridMap[igrid][inode][ivertex];
									fprintf(m_TecPlotBedBanks3D, "%lf %lf %lf \n", c.x, c.y, c.elevation);
if(inode == data->MonitorNode)
{
			//if(data->grids[igrid].nodes[inode].xsection.rightbank.nodesforTecPlot[ivertex-maxnumbernodesonbank-migdata->channel.nnodes].x<-10000)logmsg("BBBBBBBBBBBBBBBBBBBB = %d\n", inode);
}				
								}
							
							else
								{	
									fprintf(m_TecPlotBedBanks3D, "%lf %lf %lf \n", migdata->XX(inode+firstnode,ivertex-maxnumbernodesonbank), migdata->YY(inode+firstnode, ivertex-maxnumbernodesonbank), hdata->EE(inode+firstnode,ivertex-maxnumbernodesonbank));
if(inode == data->MonitorNode)
{
//if(migdata->XX(inode+firstnode,ivertex-maxnumbernodesonbank)<-10000)logmsg("CCCCCCCCCCCCCCCCCCCC = %d\n", inode);
}	
								}

						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			firstnode = 0;
		}

	// Tecplot water surface
	if (j > 0)
		{
			fprintf(m_TecPlotHydro3D, "\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL) \n", PrintTime, migdata->channel.centerline.length, 2);
			firstnode = 0;
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  		
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)		
						{
							fprintf(m_TecPlotHydro3D, "%lf %lf %lf \n", data->intersectleft[firstnode+inode].x, data->intersectleft[firstnode+inode].y, data->intersectleft[firstnode+inode].elevation);
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}		
			firstnode = 0;
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  		
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)		
						{
							fprintf(m_TecPlotHydro3D, "%lf %lf %lf \n", data->intersectright[firstnode+inode].x, data->intersectright[firstnode+inode].y, data->intersectright[firstnode+inode].elevation);
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}

		}

}
			
void RMPlotting::TecPlotSections(int j, double deltaT, RMBankErosionData* data, RMMigrationData* migdata, RMHydrodynamicsData* hdata, RMPrepData* prepdata)			
{
    if (m_TecPlotSections2D == NULL)
        return;

	int sumnodesleft, sumnodesright;
    double PrintTime = j * deltaT/365.0; 			// PrintTime in years

	// Define the monitore node for which to print the bank evolution
	int igrid = 0;
	int inode = data->MonitorNode;
	
	int firstnode = 0;


	if (j == 0)
		{
			logmsg("TecPlotSections (initial) \n");
		}
	else
		{
			logmsg("TecPlotSections \n");
		}
	
	//Left bank
	sumnodesleft = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes; 
	fprintf(m_TecPlotSections2D, "\nZONE T=\"%lf Left Bank\" I=%d,  DATAPACKING=POINT\n", PrintTime, sumnodesleft); //DM-FEB2011
	for(int ivertex = 0; ivertex<=data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1; ivertex++)
		{
			fprintf(m_TecPlotSections2D, "%lf %lf \n", data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station, data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].elevation);	
		}
	sumnodesright = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes; 
	
	//Right bank
	fprintf(m_TecPlotSections2D, "\nZONE T=\"%lf Right Bank\" I=%d,  DATAPACKING=POINT\n", PrintTime, sumnodesright);//DM-FEB2011
	for(int ivertex = 0; ivertex<=data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1; ivertex++)
		{
			fprintf(m_TecPlotSections2D, "%lf %lf \n", data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station, data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].elevation);	
	    }

	//Water surface
	fprintf(m_TecPlotSections2D, "\nZONE T=\"%lf Water Surface\" I=%d,  DATAPACKING=POINT\n", PrintTime, migdata->channel.nnodes);//DM-FEB2011
	for(int i = 0; i<=migdata->channel.nnodes-1; i++)
		{
			//fprintf(m_TecPlotSections2D, "%lf %lf \n", (prepdata->coordn(i)*migdata->channel.WIDTH/2)+data->StationCenterline[inode], hdata->HH(inode,i));		//CORRECTION AUG2009
			fprintf(m_TecPlotSections2D, "%lf %lf \n", (-prepdata->coordn(i)*migdata->channel.WIDTH/2)+data->StationCenterline[inode], hdata->HH(inode,i));			//CORRECTION AUG2009
	    }

	//Bed
	fprintf(m_TecPlotSections2D, "\nZONE T=\"%lf Bed\" I=%d,  DATAPACKING=POINT\n", PrintTime, migdata->channel.nnodes);//DM-FEB2011
	for(int i = 0; i<=migdata->channel.nnodes-1; i++)
		{
			//fprintf(m_TecPlotSections2D, "%lf %lf \n", (prepdata->coordn(i)*migdata->channel.WIDTH/2)+data->StationCenterline[inode], hdata->EE(inode,i));		//CORRECTION AUG2009	
			fprintf(m_TecPlotSections2D, "%lf %lf \n", (-prepdata->coordn(i)*migdata->channel.WIDTH/2)+data->StationCenterline[inode], hdata->EE(inode,i));			//CORRECTION AUG2009
	    }
}

void RMPlotting::ReworkedAreas(int j, double deltaT, RMBankErosionData* data, RMMigrationData* migdata)			
{
	//logmsg("ReworkedAreas \n");																																				//COMMENTED OUT IN FEB2010
	//double PrintTime = j * deltaT/365.0; 			// PrintTime in years																										//COMMENTED OUT IN FEB2010
	//int inode = data->MonitorNode;																																			//COMMENTED OUT IN FEB2010
	//for(int inode = 0; inode <= migdata->channel.centerline.length-1; inode++)																								//COMMENTED OUT IN FEB2010
	//	{																																										//COMMENTED OUT IN FEB2010
			//fprintf(m_ReworkedAreas, "%d	%lf	%lf	%lf	%lf\n", inode, PrintTime, data->ReworkedAreaLeft[inode], data->ReworkedAreaRight[inode], data->TotalErodedVolume);		//COMMENTED OUT IN FEB2010
	//	}																																										//COMMENTED OUT IN FEB2010
}

/*  //DM-FEB2011  Commented out
void RMPlotting::HeterogeneityAtCenterline(int j, double deltaT, RMBankErosionData* bankErosionData, RMPrepData* prepData)	//HETEROGENEITY
{
    if (m_HeterogeneityAtCenterline == NULL)
        return;

	logmsg("HeterogeneityAtCenterline \n");

	double PrintTime = j * deltaT/365.0; 			// PrintTime in years

	for(int i = 0; i <= bankErosionData->grids[0].numberofnodes-1; i++)
		{
			fprintf(m_HeterogeneityAtCenterline,"%lf	%d	%lf	%lf	\n", PrintTime, i, prepData->coords(i), bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress);
		}
}
*/

void RMPlotting::TecplotHeterogeneityGrid(RMBankErosionData* bankErosionData)	//HETEROGENEITY    //DM-FEB2011
{
	logmsg("TecplotHeterogeneityGrid\n");
	
	double Erodibility;

	fprintf(m_TecplotGrid,"TITLE = \"Heterogeneity Grid\"\n");																																																			//DM-FEB2011
	fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Eo\" \"Erosion rate coefficient [m/s]\" \"Critical shear stress [Pa]\" \"Erodibility [m3/(Ns)]\" \"Unit weight [N]\" \"Cohesion [Pa]\" \"Angle of repose [o]\" \"Angle Phib [o]\" ");								//DM-FEB2011
	fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);										//DM-FEB2011

	for(int i = 0; i <= bankErosionData->Ncolumns - 1; i++)							//Loop over columns
		{
			for(int j = 0; j <= bankErosionData->Nrows - 1; j++)					//Loop over rows
				{		
					Erodibility = bankErosionData->ErosionRate_grid(i, j)/bankErosionData->CriticalTau_grid(i, j); 
					fprintf(m_TecplotGrid,"%lf	%lf	%e	%e	%lf	%e	%lf	%lf	%lf	%lf \n",//DM-FEB2011
                        bankErosionData->X_grid(i, j),
                        bankErosionData->Y_grid(i, j),
                        bankErosionData->EC_grid(i, j),
                        bankErosionData->ErosionRate_grid(i, j),
                        bankErosionData->CriticalTau_grid(i, j),
                        Erodibility,
                        bankErosionData->Unitweight_grid(i, j),
                        bankErosionData->Cohesion_grid(i, j),
                        bankErosionData->AngleRepose_grid(i, j),
                        bankErosionData->AnglePhib_grid(i, j));
				}
		}
}

void RMPlotting::TecplotHeterogeneityGridWithLayers(RMBankErosionData* bankErosionData)	//HETEROGENEITY    //DM-FEB2011
{
	logmsg("TecplotHeterogeneityGridWithLayers\n");
	
	double Erodibility;

	fprintf(m_TecplotGrid,"TITLE = \"Heterogeneity Grid\"\n"); //DM-FEB2011

	//Different header according to MaxNumberOfLayers
	if(bankErosionData->MaxNumberOfLayers == 1)
		{
			//fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Floodplain elevation [m a.s.l.]\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"	");			//MMMMTEMP Floodplain elevation
			fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"	");			//DM-FEB2011
			fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL, 11=NODAL, 12=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);																							//DM-FEB2011
		}
	else if(bankErosionData->MaxNumberOfLayers == 2)
		{
			//fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Floodplain elevation [m a.s.l.]\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"		\			//MMMMTEMP Floodplain elevation
			//																  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\" ");					//MMMMTEMP Floodplain elevation
			fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"		\
																			  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\" ");					//DM-FEB2011 
			fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL, 11=NODAL, 12=NODAL, 13=NODAL, 14=NODAL, 15=NODAL, 16=NODAL, 17=NODAL, 18=NODAL, 19=NODAL, 20=NODAL, 21=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);		//DM-FEB2011 
		}
	else if(bankErosionData->MaxNumberOfLayers == 3)
		{
			//fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Floodplain elevation [m a.s.l.]\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"	");			//MMMMTEMP Floodplain elevation
			//																  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\
			//																  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\" ");			//MMMMTEMP Floodplain elevation
			fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"		\
																			  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\
																			  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\" ");			//DM-FEB2011
			fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL, 11=NODAL, 12=NODAL, 13=NODAL, 14=NODAL, 15=NODAL, 16=NODAL, 17=NODAL, 18=NODAL, 19=NODAL, 20=NODAL, 21=NODAL, 22=NODAL, 23=NODAL, 24=NODAL, 25=NODAL, 26=NODAL,     \
				27=NODAL, 28=NODAL, 29=NODAL, 30=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);																																																								//DM-FEB2011						
		}
	else if(bankErosionData->MaxNumberOfLayers == 4)
		{
			//fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Floodplain elevation [m a.s.l.]\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"	");			//MMMMTEMP Floodplain elevation
			//																  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\		
			//																  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\"		\		
			//																  \"Eo (L4)\" \"Erosion rate coefficient [m/s] (L4)\" \"Critical shear stress [Pa] (L4)\" \"Erodibility [m3/(Ns)] (L4)\" \"Unit weight [N] (L4)\" \"Cohesion [Pa] (L4)\" \"Angle of repose [o] (L4)\" \"Angle Phib [o] (L4)\" \"Layer elevation [m] (L4)\" ");			//MMMMTEMP Floodplain elevation
			fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"		\
																			  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\
																			  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\"		\
																			  \"Eo (L4)\" \"Erosion rate coefficient [m/s] (L4)\" \"Critical shear stress [Pa] (L4)\" \"Erodibility [m3/(Ns)] (L4)\" \"Unit weight [N] (L4)\" \"Cohesion [Pa] (L4)\" \"Angle of repose [o] (L4)\" \"Angle Phib [o] (L4)\" \"Layer elevation [m] (L4)\" ");			//DM-FEB2011
			fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL, 11=NODAL, 12=NODAL, 13=NODAL, 14=NODAL, 15=NODAL, 16=NODAL, 17=NODAL, 18=NODAL, 19=NODAL, 20=NODAL, 21=NODAL, 22=NODAL, 23=NODAL, 24=NODAL, 25=NODAL, 26=NODAL,     \
				27=NODAL, 28=NODAL, 29=NODAL, 30=NODAL, 31=NODAL, 32=NODAL, 33=NODAL, 34=NODAL, 35=NODAL, 36=NODAL, 37=NODAL, 38=NODAL, 39=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);																																	//DM-FEB2011

		}
	else if(bankErosionData->MaxNumberOfLayers == 5)
		{
			//fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Floodplain elevation [m a.s.l.]\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"	");			//MMMMTEMP Floodplain elevation
			//																  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\
			//																  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\"		\
			//																  \"Eo (L4)\" \"Erosion rate coefficient [m/s] (L4)\" \"Critical shear stress [Pa] (L4)\" \"Erodibility [m3/(Ns)] (L4)\" \"Unit weight [N] (L4)\" \"Cohesion [Pa] (L4)\" \"Angle of repose [o] (L4)\" \"Angle Phib [o] (L4)\" \"Layer elevation [m] (L4)\"      \
			//																  \"Eo (L5)\" \"Erosion rate coefficient [m/s] (L5)\" \"Critical shear stress [Pa] (L5)\" \"Erodibility [m3/(Ns)] (L5)\" \"Unit weight [N] (L5)\" \"Cohesion [Pa] (L5)\" \"Angle of repose [o] (L5)\" \"Angle Phib [o] (L5)\" \"Layer elevation [m] (L5)\" ");			//MMMMTEMP Floodplain elevation
			fprintf(m_TecplotGrid,"VARIABLES = \"X\" \"Y\" \"Number of layers\" \"Eo (L1)\" \"Erosion rate coefficient [m/s] (L1)\" \"Critical shear stress [Pa] (L1)\" \"Erodibility [m3/(Ns)] (L1)\" \"Unit weight [N] (L1)\" \"Cohesion [Pa] (L1)\" \"Angle of repose [o] (L1)\" \"Angle Phib [o] (L1)\" \"Layer elevation [m] (L1)\"		\
																			  \"Eo (L2)\" \"Erosion rate coefficient [m/s] (L2)\" \"Critical shear stress [Pa] (L2)\" \"Erodibility [m3/(Ns)] (L2)\" \"Unit weight [N] (L2)\" \"Cohesion [Pa] (L2)\" \"Angle of repose [o] (L2)\" \"Angle Phib [o] (L2)\" \"Layer elevation [m] (L2)\"		\
																			  \"Eo (L3)\" \"Erosion rate coefficient [m/s] (L3)\" \"Critical shear stress [Pa] (L3)\" \"Erodibility [m3/(Ns)] (L3)\" \"Unit weight [N] (L3)\" \"Cohesion [Pa] (L3)\" \"Angle of repose [o] (L3)\" \"Angle Phib [o] (L3)\" \"Layer elevation [m] (L3)\"		\
																			  \"Eo (L4)\" \"Erosion rate coefficient [m/s] (L4)\" \"Critical shear stress [Pa] (L4)\" \"Erodibility [m3/(Ns)] (L4)\" \"Unit weight [N] (L4)\" \"Cohesion [Pa] (L4)\" \"Angle of repose [o] (L4)\" \"Angle Phib [o] (L4)\" \"Layer elevation [m] (L4)\"      \
																			  \"Eo (L5)\" \"Erosion rate coefficient [m/s] (L5)\" \"Critical shear stress [Pa] (L5)\" \"Erodibility [m3/(Ns)] (L5)\" \"Unit weight [N] (L5)\" \"Cohesion [Pa] (L5)\" \"Angle of repose [o] (L5)\" \"Angle Phib [o] (L5)\" \"Layer elevation [m] (L5)\" ");			//DM-FEB2011
			fprintf(m_TecplotGrid,"\nZONE T=\"%lf\" I=%d, J=%d,  DATAPACKING=POINT, VARLOCATION=(3=NODAL, 4=NODAL, 5=NODAL, 6=NODAL, 7=NODAL, 8=NODAL, 9=NODAL, 10=NODAL, 11=NODAL, 12=NODAL, 13=NODAL, 14=NODAL, 15=NODAL, 16=NODAL, 17=NODAL, 18=NODAL, 19=NODAL, 20=NODAL, 21=NODAL, 22=NODAL, 23=NODAL, 24=NODAL, 25=NODAL, 26=NODAL,     \
				27=NODAL, 28=NODAL, 29=NODAL, 30=NODAL, 31=NODAL, 32=NODAL, 33=NODAL, 34=NODAL, 35=NODAL, 36=NODAL, 37=NODAL, 38=NODAL, 39=NODAL, 40=NODAL, 41=NODAL, 42=NODAL, 43=NODAL, 44=NODAL, 45=NODAL, 46=NODAL, 47=NODAL, 48=NODAL) \n", 0.000, bankErosionData->Nrows, bankErosionData->Ncolumns);											//DM-FEB2011

		}

	//Print values
	for(int i = 0; i <= bankErosionData->Ncolumns - 1; i++)							//Loop over columns
		{
			for(int j = 0; j <= bankErosionData->Nrows - 1; j++)					//Loop over rows
				{		
					fprintf(m_TecplotGrid,"%lf	%lf	%d	",							//DM-FEB2011
					bankErosionData->X_grid(i, j),
                    bankErosionData->Y_grid(i, j),
					bankErosionData->NoOfLayers_gridWL[i][j]);
					
					//fprintf(m_TecplotGrid,"%lf	",bankErosionData->FloodplainElevation_gridWL(i, j));  //MMMMTEMP Floodplain elevation
					
					for(int ilayer = 0; ilayer <= bankErosionData->MaxNumberOfLayers - 1; ilayer++)
						{
							Erodibility = bankErosionData->ErosionRate_gridWL[ilayer](i, j)/bankErosionData->CriticalTau_gridWL[ilayer](i, j);
							fprintf(m_TecplotGrid,"%e	%e	%lf	%e	%lf	%lf	%lf	%lf	%lf ",		//DM-FEB2011
							bankErosionData->EC_gridWL[ilayer](i, j),
							bankErosionData->ErosionRate_gridWL[ilayer](i, j),
							bankErosionData->CriticalTau_gridWL[ilayer](i, j),
							Erodibility,
							bankErosionData->Unitweight_gridWL[ilayer](i, j),
							bankErosionData->Cohesion_gridWL[ilayer](i, j),
							bankErosionData->AngleRepose_gridWL[ilayer](i, j),
							//bankErosionData->AnglePhib_gridWL[ilayer](i, j),  //MMMMTEMP Commented ot for now and replaced with floodplain elevation
							bankErosionData->FloodplainElevation_gridWL(i, j),  //MMMMTEMP 
							bankErosionData->LayerElevation_gridWL[ilayer](i, j));
							}
					fprintf(m_TecplotGrid,"\n");					//DM-FEB2011
				}	
		}
}



void RMPlotting::SetTecplotOutput_Floodplain(const char* file) //DM-FEB2011
{
    if (file == NULL || strlen(file) == 0)
    {
        m_TecplotGrid = NULL;
        return;
    }

	m_TecplotGrid = fopen(file, "w");
}

// The code below is creating problems when running from the interface. Davide wrote this code which tries to output the Karmor.Dat and Indices.DAT files
// to the same location where the RVRMeander.dll is running from. This, in many cases means C://ProgramFiles and this folder might be protected. 
// The code below is Roberto's attempt to fix the issue. 
/*
void RMPlotting::SetKarmorFile() //DM-FEB2012
{
	KarmorFile = fopen("Karmor.DAT", "w");
}

void RMPlotting::SetIndicesFile() //MMMMTEMP Indices file
{
	IndicesFile = fopen("Indices.DAT", "w");
}

void RMPlotting::SetKarmorVisualizedFile() //DM-FEB2013
{
	KarmorVisualizedFile = fopen("KarmorVisualized.DAT", "w");
	fprintf(KarmorVisualizedFile,"TITLE = \"Karmor Visualized\"\n");
	fprintf(KarmorVisualizedFile,"VARIABLES = \"x[m]\" \"y[m]\" \"Ablock,cantilever[m2]\" \"Ablock,planar[m2]\" \"TotalAblock[m2]\" \"Karmor[-]\" \"ErosionBank[m]\" \n"); 
}
*/
void RMPlotting::SetKarmorFile(const char* file)
{
	if (file == NULL || strlen(file) == 0)
	{
		KarmorFile = NULL;
		return;
	}

	KarmorFile = fopen(file, "w");
}
void RMPlotting::SetIndicesFile(const char* file)
{
	if (file == NULL || strlen(file) == 0)
	{
		IndicesFile = NULL;
		return;
	}

	IndicesFile = fopen(file, "w");
}
void RMPlotting::SetKarmorVisualizedFile(const char* file) 
{
	if (file == NULL || strlen(file) == 0)
	{
		KarmorVisualizedFile = NULL;
		return;
	}

	KarmorVisualizedFile = fopen(file, "w");
	fprintf(KarmorVisualizedFile, "TITLE = \"Karmor Visualized\"\n");
	fprintf(KarmorVisualizedFile, "VARIABLES = \"x[m]\" \"y[m]\" \"Ablock,cantilever[m2]\" \"Ablock,planar[m2]\" \"TotalAblock[m2]\" \"Karmor[-]\" \"ErosionBank[m]\" \n");
}

void RMPlotting::WriteKarmorFile(int j, double deltaT, const RMBankErosionData* bdata, const RMHydrodynamicsData* hdata, const RMPrepData* pdata, const RMMigrationData* mdata) //DM-FEB2012
{
	// This routine writes to a text file quantities related to cantilever and planar failure volumes and values of armor factor karmor
	double sdim,s,C,x,y,DL,ACL,APL,FAL,FOSL,PWL,PWSL,PWWL,TSL,TEL,ATL,KAL,DR,ACR,APR,FAR,FOSR,PWR,PWSR,PWWR,TSR,TER,ATR,KAR,BE;   
	int monitornode;  
	
	if (KarmorFile  == NULL)
		return;

	double PrintTime = j * deltaT/365.0;			// PrintTime in years
	fprintf(KarmorFile , "\nZONE T=\"%lf\" I=%d,  DATAPACKING=POINT\n", PrintTime, bdata->grids[0].numberofnodes);
	fprintf(KarmorFile , 
		"Time(yrs)	#nodes	node	s_dimensionless(-)	s_scaled(m)	C_dimensioneless(-)	x_centerline(m)	y_centerline(m)\
		Depth(m)(LeftBank)	CantileverArea(m2)(LeftBank)	PlanarArea(m2)(LeftBank)	FailureAngle(degrees)(LeftBank)	FactorOfSafety(-)(LeftBank) PWL	PWSL	PWWL	StationToeFailureBlock(ma.s.l.)(LeftBank)	ElevationToeFailureBlock(ma.s.l.)(LeftBank) TotalBlockArea(m2)(LeftBank)	Karmor(-)(LeftBank)	\
		Depth(m)(RightBank)	CantileverArea(m2)(RightBank)	PlanarArea(m2)(RightBank)	FailureAngle(degrees)(RightBank)	FactorOfSafety(-)(RightBank) PWR	PWSR PWWR	StationToeFailureBlock(ma.s.l.)(RightBank)	ElevationToeFailureBlock(ma.s.l.)(RightBank) TotalBlockArea(m2)(RightBank)	Karmor(-)(RightBank)		monitornode	BankErosion(m)\n");
	for (int i = 0; i <= bdata->grids[0].numberofnodes-1; i++)
		{
			//Computation of variables
			sdim = pdata->coords(i);														//s_dimensionless(-)	
			s = pdata->coords(i)/pdata->coords(mdata->channel.centerline.length-1);			//s_scaled(m)
			C = pdata->curvature(i);														//C_dimensioneless(-)	
			x = mdata->channel.centerline(i,0)*mdata->channel.WIDTH/2.0;					//x coordinate of the centerline node(m)
			y = mdata->channel.centerline(i,1)*mdata->channel.WIDTH/2.0;					//y coordinate of the centerline node(m)
			DL = hdata->DD(i,0);															//Depth(m)(LeftBank)	
			ACL = bdata->grids[0].nodes[i].xsection.leftbank.AblockCantilever;				//CantileverArea(m2)(LeftBank)	
			APL = bdata->grids[0].nodes[i].xsection.leftbank.AblockPlanar;					//PlanarArea(m2)(LeftBank)		
			FAL = bdata->grids[0].nodes[i].xsection.leftbank.PlanarFailureAngle;			//FailureAngle(degrees)(LeftBank)					
			FOSL = bdata->grids[0].nodes[i].xsection.leftbank.FactorOfSafety;				//FactorOfSafety(-)(LeftBank)	
			PWL = bdata->grids[0].nodes[i].xsection.leftbank.failureparameters.weight;		//This is the total weight of slices for minimum factor of safety (includes water weight above slices, if any, and slices located upvalley with respect to tension crack, if any). It is printed to file for check purposes.
			PWSL = bdata->grids[0].nodes[i].xsection.leftbank.failureparameters.weightslices; //This is the total weight of slices for minimum factor of safety (does not include water weight above slices, if any, and does not include slices located upvalley with respect to tension crack, if any). It is printed to file for check purposes.
			PWWL = bdata->grids[0].nodes[i].xsection.leftbank.failureparameters.waterweight;//This is the total weight of water above slices, if any. It is printed to file for check purposes.
			TSL = bdata->grids[0].nodes[i].xsection.leftbank.PlanarFailureToe.station;		//StationToeFailureBlock(ma.s.l.)(LeftBank)			
			TEL = bdata->grids[0].nodes[i].xsection.leftbank.PlanarFailureToe.elevation;	//ElevationToeFailureBlock(ma.s.l.)(LeftBank)		
			ATL = bdata->grids[0].nodes[i].xsection.leftbank.TotalAblock;					//TotalBlockArea(m2)(LeftBank)
			KAL = bdata->grids[0].nodes[i].xsection.leftbank.Karmor;						//Karmor(-)(LeftBank)	
			DR = hdata->DD(i,mdata->channel.nnodes-1);										//Depth(m)(RightBank)
			ACR = bdata->grids[0].nodes[i].xsection.rightbank.AblockCantilever;				//CantileverArea(m2)(RightBank)	
			APR = bdata->grids[0].nodes[i].xsection.rightbank.AblockPlanar;					//PlanarArea(m2)(RightBank)	
			FAR = bdata->grids[0].nodes[i].xsection.rightbank.PlanarFailureAngle;			//FailureAngle(degrees)(RightBank) 
			FOSR = bdata->grids[0].nodes[i].xsection.rightbank.FactorOfSafety;				//FactorOfSafety(-)(RightBank)    
			PWR = bdata->grids[0].nodes[i].xsection.rightbank.failureparameters.weight;		//This is the total weight of slices for minimum factor of safety (includes water weight above slices, if any, and slices located upvalley with respect to tension crack, if any). It is printed to file for check purposes.
			PWSR = bdata->grids[0].nodes[i].xsection.rightbank.failureparameters.weightslices; //This is the total weight of slices for minimum factor of safety (does not include water weight above slices, if any, and does not include slices located upvalley with respect to tension crack, if any). It is printed to file for check purposes.	
			PWWR = bdata->grids[0].nodes[i].xsection.rightbank.failureparameters.waterweight;//This is the total weight of water above slices, if any. It is printed to file for check purposes.
			TSR = bdata->grids[0].nodes[i].xsection.rightbank.PlanarFailureToe.station;		//StationToeFailureBlock(ma.s.l.)(RightBank) 
			TER = bdata->grids[0].nodes[i].xsection.rightbank.PlanarFailureToe.elevation;	//ElevationToeFailureBlock(ma.s.l.)(RightBank) 
			ATR = bdata->grids[0].nodes[i].xsection.rightbank.TotalAblock;					//TotalBlockArea(m2)(RightBank)
			KAR = bdata->grids[0].nodes[i].xsection.rightbank.Karmor;						//Karmor(-)(RightBank)
			BE = bdata->ErosionBank(i,1)*mdata->channel.WIDTH/2.0;							//Bank erosion
			if(i == bdata->MonitorNode)monitornode = 1;										//Value equal to 1 is the node is the Monitor node
			else monitornode = 0;															//Otherwise it is zero
			//Printing in file
			fprintf(KarmorFile, "%lf	%d	%d	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf %lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%d	%lf\n", PrintTime,bdata->grids[0].numberofnodes,i,sdim,s,C,x,y,DL,ACL,APL,FAL,FOSL,PWL,PWSL,PWWL,TSL,TEL,ATL,KAL,DR,ACR,APR,FAR,FOSR,PWR,PWSR,PWWR,TSR,TER,ATR,KAR,monitornode,BE);
		}
}

void RMPlotting::WriteIndicesFile(int j, double deltaT, const RMBankErosionData* bdata) //DM-FEB2012
{
	// This routine writes to a text file bottom and top index values of left and right banks for the case of one or two layers
	// It is meant for check purposes
	int LBI_one, LTI_one, LBI_two, LTI_two, RBI_one, RTI_one, RBI_two, RTI_two;
	double LE_one, LE_two, RE_one, RE_two;

	if (IndicesFile  == NULL)
		return;

	double PrintTime = j * deltaT/365.0;			// PrintTime in years
	fprintf(IndicesFile , "\nZONE T=\"%lf\" I=%d,  DATAPACKING=POINT\n", PrintTime, bdata->grids[0].numberofnodes);
	fprintf(IndicesFile , 
		"Time(yrs)	#nodes	node		\
		LBI_one	LTI_one	LE_one	LBI_two	LTI_two	LE_two	\
		RBI_one	RTI_one	RE_one	RBI_two	RTI_two	RE_two	 \n");
	for (int i = 0; i <= bdata->grids[0].numberofnodes-1; i++)
		{
			//Computation of variables
			LBI_one = bdata->grids[0].nodes[i].xsection.leftbank.material[0].bottomindex;				//Left bank, bottom index of layer 1		
			LTI_one = bdata->grids[0].nodes[i].xsection.leftbank.material[0].topindex;					//Left bank, top index of layer 1	
			LE_one = bdata->grids[0].nodes[i].xsection.leftbank.material[0].elevation;					//Left bank, elevation of layer 1	
			if(bdata->grids[0].nodes[i].xsection.leftbank.numberoflayers>1)
				{
					LBI_two = bdata->grids[0].nodes[i].xsection.leftbank.material[1].bottomindex;		//Left bank, bottom index of layer 2			
					LTI_two = bdata->grids[0].nodes[i].xsection.leftbank.material[1].topindex;			//Left bank, top index of layer 2
					LE_two = bdata->grids[0].nodes[i].xsection.leftbank.material[1].elevation;			//Left bank, elevation of layer 2	
				}		
			else
				{
					LBI_two = 0;																		//Left bank, bottom index of layer 2
					LTI_two = 0;																		//Left bank, top index of layer 2
					LE_two = 0.0;																		//Left bank, elevation of layer 2	
				}	
			RBI_one = bdata->grids[0].nodes[i].xsection.rightbank.material[0].bottomindex;				//Right bank, bottom index of layer 1	
			RTI_one = bdata->grids[0].nodes[i].xsection.rightbank.material[0].topindex;					//Right bank, top index of layer 1
			RE_one = bdata->grids[0].nodes[i].xsection.rightbank.material[0].elevation;					//Right bank, elevation of layer 1
			if(bdata->grids[0].nodes[i].xsection.rightbank.numberoflayers>1)
				{
					RBI_two = bdata->grids[0].nodes[i].xsection.rightbank.material[1].bottomindex;		//Right bank, bottom index of layer 2		
					RTI_two = bdata->grids[0].nodes[i].xsection.rightbank.material[1].topindex;			//Right bank, top index of layer 2
					RE_two = bdata->grids[0].nodes[i].xsection.rightbank.material[1].elevation;			//Right bank, elevation of layer 2
				}
			else
				{
					RBI_two = 0;																		//Right bank, bottom index of layer 2		
					RTI_two = 0;																		//Right bank, top index of layer 2	
					RE_two = 0.0;																		//Right bank, elevation of layer 2
				}
			//Printing in file
			fprintf(IndicesFile, "%lf	%d	%d	%d	%d	%lf	%d	%d	%lf	%d	%d	%lf	%d	%d	%lf \n", PrintTime,bdata->grids[0].numberofnodes,i,LBI_one,LTI_one,LE_one,LBI_two,LTI_two,LE_two,RBI_one,RTI_one,RE_one,RBI_two,RTI_two,RE_two);
		}
}


void RMPlotting::WriteKarmorVisualizedFile(int j, double deltaT, const RMMigrationData* migdata, const RMBankErosionData* data) //DM-FEB2013
{
	if (KarmorVisualizedFile == NULL)
	return;
		
	double PrintTime = j * deltaT/365.0;			// PrintTime in years
	fprintf(KarmorVisualizedFile, "\nZONE T=\"%lf\" I=%d,  DATAPACKING=POINT\n", PrintTime, 3*migdata->channel.centerline.length);
	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)						
		{ 
			fprintf(KarmorVisualizedFile, "%lf %lf	%lf	%lf	%lf	%lf %e\n", migdata->channel.centerline(i,0)*migdata->channel.WIDTH/2.0,migdata->channel.centerline(i,1)*migdata->channel.WIDTH/2.0, 0.0, 0.0, 0.0, 0.0, 0.0);		//Centerline
			fprintf(KarmorVisualizedFile, "%lf %lf	%lf	%lf	%lf	%lf %e\n", migdata->XX(i,0), migdata->YY(i,0), data->grids[0].nodes[i].xsection.leftbank.AblockCantilever, data->grids[0].nodes[i].xsection.leftbank.AblockPlanar, data->grids[0].nodes[i].xsection.leftbank.TotalAblock, data->grids[0].nodes[i].xsection.leftbank.Karmor, data->ErosionBank(i,1)*migdata->channel.WIDTH/2.0);  //Left bank
			fprintf(KarmorVisualizedFile, "%lf %lf	%lf	%lf	%lf	%lf %e\n", migdata->XX(i,migdata->channel.nnodes-1), migdata->YY(i,migdata->channel.nnodes-1), data->grids[0].nodes[i].xsection.rightbank.AblockCantilever, data->grids[0].nodes[i].xsection.rightbank.AblockPlanar, data->grids[0].nodes[i].xsection.rightbank.TotalAblock, data->grids[0].nodes[i].xsection.rightbank.Karmor, data->ErosionBank(i,1)*migdata->channel.WIDTH/2.0);  //Right bank
		}	
}