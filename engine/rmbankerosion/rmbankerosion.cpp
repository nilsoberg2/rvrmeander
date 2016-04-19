#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES		// For definition of M_PI

#include <vector>
#include <algorithm>			// I Need this to be able to use max and min functions 
#include <cmath>
#include "rmbankerosion.h"
#include "../rmutil/utils.h"

using namespace std;			// I Need this to be able to use max and min functions


RMBankErosionData::RMBankErosionData()
{
	//TODO: Initialize all the pointers to NULL
	//grids = NULL;
	/*
	Grid *grids;
	FlowVariables *flowworkspace;
	Coordinates *intersectleft, *intersectright, *oldintersectleft, *oldintersectright;
	Coordinates *toeleft, *toeright, *oldtoeleft, *oldtoeright;
	double *StationCenterline;
	double *ReworkedAreaLeft, *ReworkedAreaRight;
	double TotalErodedVolume;
	*/

	flowworkspace = NULL;			//DM-FEB2011
	intersectleft = NULL;			//DM-FEB2011
	intersectright = NULL;			//DM-FEB2011
	oldintersectleft = NULL;		//DM-FEB2011
	oldintersectright = NULL;;		//DM-FEB2011
	toeleft = NULL;					//DM-FEB2011
 	toeright = NULL;				//DM-FEB2011
	oldtoeleft = NULL;				//DM-FEB2011
	oldtoeright = NULL;				//DM-FEB2011
	//StationCenterline = NULL;		//DM-FEB2011
	ReworkedAreaLeft = NULL;		//DM-FEB2011
	ReworkedAreaRight = NULL;		//DM-FEB2011
}

RMBankErosionData::~RMBankErosionData()
{
	Cleanup();
}

void RMBankErosionData::Cleanup()
{
    // DO NOT TOUCH grids!

	//TODO: fill this in with all the pointers

    if (flowworkspace != NULL)			//DM-FEB2011
		delete[] flowworkspace;			//DM-FEB2011
	if (intersectleft != NULL)			//DM-FEB2011
		delete[] intersectleft;			//DM-FEB2011
	if (intersectright != NULL)			//DM-FEB2011
		delete[] intersectright;		//DM-FEB2011
	if (oldintersectleft != NULL)		//DM-FEB2011
		delete[] oldintersectleft;		//DM-FEB2011
	if (oldintersectright != NULL)		//DM-FEB2011
		delete[] oldintersectright;		//DM-FEB2011
	if (toeleft != NULL)				//DM-FEB2011
		delete[] toeleft;				//DM-FEB2011
	if (toeright != NULL)				//DM-FEB2011
		delete[] toeright;				//DM-FEB2011
	if (oldtoeleft != NULL)				//DM-FEB2011
		delete[] oldtoeleft;			//DM-FEB2011
	if (oldtoeright != NULL)			//DM-FEB2011
		delete[] oldtoeright;			//DM-FEB2011
	//if (StationCenterline != NULL)		//DM-FEB2011
	//	delete[] StationCenterline;		//DM-FEB2011
    StationCenterline.clear();
	if (ReworkedAreaLeft != NULL)		//DM-FEB2011
		delete[] ReworkedAreaLeft;		//DM-FEB2011
	if (ReworkedAreaRight != NULL)		//DM-FEB2011
		delete[] ReworkedAreaRight;		//DM-FEB2011
}


void RMBankErosionData::Init(int length, int width)
{
	InitBase(length, width);
	
	if(FloodplainHeterogeneity)
		{
			FACTRnu.init(length);			//DM-FEB2011
			FACTRnu_left.init(length);		//DM-FEB2011
			FACTRnu_right.init(length);		//DM-FEB2011
			ECnu.init(length);				//DM-FEB2011
			ECnu_left.init(length);			//DM-FEB2011
			ECnu_right.init(length);		//DM-FEB2011
		}
}


void RMBankErosionData::InitBase(int length, int width)
{
	ErosionBank.init(length, width);
}

void RMBankErosionData::InitBankProperties(int length)
{
	Cleanup();

	flowworkspace	= new FlowVariables [length];
	intersectleft = new Coordinates[length];	
	intersectright = new Coordinates[length];
	oldintersectleft = new Coordinates[length];	
	oldintersectright = new Coordinates[length];
	toeleft = new Coordinates[length];	
	toeright = new Coordinates[length];
	oldtoeleft = new Coordinates[length];	
	oldtoeright = new Coordinates[length];  
	//StationCenterline = new double[length];
    StationCenterline.resize(length);
	ReworkedAreaLeft = new double[length];
	ReworkedAreaRight = new double[length];
}

void RMBankErosionData::ReInitBankProperties(int length)
{			
	Cleanup();

	flowworkspace	= new FlowVariables [length];
	intersectleft = new Coordinates[length];	
	intersectright = new Coordinates[length];
	oldintersectleft = new Coordinates[length];	
	oldintersectright = new Coordinates[length];
	toeleft = new Coordinates[length];	
	toeright = new Coordinates[length];
	oldtoeleft = new Coordinates[length];	
	oldtoeright = new Coordinates[length];  
	StationCenterline.resize(length);
	ReworkedAreaLeft = new double[length];
	ReworkedAreaRight = new double[length];
}


RMBankErosion::RMBankErosion()
{
}


RMBankErosion::~RMBankErosion()
{
}


void RMBankErosion::ComputeDimensionlessBankErosion(RMHydrodynamicsData* hdata, RMBankErosionData* data, RMMigrationData* migdata, RMPrepData* prepdata, double deltaT, int iteration)  //DM-FEB2012
{

	bool ConditionRightBank1;		//DM-FEB2011
	bool ConditionRightBank2;		//DM-FEB2011
	bool ConditionRightDepth;		//DM-FEB2011
	bool ConditionLeftBank1;		//DM-FEB2011
	bool ConditionLeftBank2;		//DM-FEB2011
	bool ConditionLeftDepth;		//DM-FEB2011

	// Computes dimensionless ErosionBank
    // Only the near bank nodes have values, everywhere else is 0
    // We can put also eqn-> 2->27 on HES-043 

	if (data->MethodBankEvolution == 1)					
		{											
			for (int i = 0; i <= hdata->u.length-1; i++)
				{
					//data->ErosionBank(i,1) = data->FACTR * hdata->u(i,hdata->u.width-1);			//CORRECTION FEB2010
					data->ErosionBank(i,1) = data->FACTR * hdata->u(i,0);							//CORRECTION FEB2010
					
					//HETEROGENEITY
					if(data->FloodplainHeterogeneity)												//DM-FEB2011
						{																			//DM-FEB2011
							//data->ErosionBank(i,1) = data->FACTRnu(i) * hdata->u(i,0);			//DM-FEB2011  //DM-FEB2011 Commented out
							
							if(hdata->u(i,0) >= 0)														//DM-FEB2011
								{																		//DM-FEB2011
									data->ErosionBank(i,1) = data->FACTRnu_left(i) * hdata->u(i,0);		//DM-FEB2011
								}																		//DM-FEB2011
							else																		//DM-FEB2011
								{																		//DM-FEB2011
									data->ErosionBank(i,1) = data->FACTRnu_right(i) * hdata->u(i,0);	//DM-FEB2011
								}																		//DM-FEB2011
							
						}																			//DM-FEB2011

				}	

		}
	
	
	if (data->MethodBankEvolution == 2)	//TO ERASE								
		{		
			PhysicalBankEvolution(data, hdata, migdata, prepdata, deltaT, iteration); //DM-FEB2012

			if(data->MethodPhysicallyBasedBankEvolution == 1)							//SEP2009
				{																		//SEP2009
					// Type of physically-based migration: purely erosional

					// Erosion distance: method 1
					if (data->MethodErosionDistance == 1) 
						{
							int firstnode = 0;
							for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
								{  
									for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
										{ 
											ConditionRightDepth = hdata->DD(inode+firstnode,migdata->channel.nnodes-1) > 0;	//DM-FEB2011																																			//DM-FEB2011
											ConditionLeftDepth = hdata->DD(inode+firstnode,0) > 0;							//DM-FEB2011

											if(ConditionRightDepth && ConditionLeftDepth)									//DM-FEB2011
												{																			//DM-FEB2011
													data->ErosionBank(inode+firstnode,1)=(-(data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station)+(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station))/2/(migdata->channel.WIDTH/2);	//CORRECTION FEB2010, CHANGE OF SIGN	
												}																			//DM-FEB2011
											else if(!ConditionRightDepth)													//DM-FEB2011
												{																			//DM-FEB2011
													data->ErosionBank(inode+firstnode,1)=((data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station))/(migdata->channel.WIDTH/2);//DM-FEB2011
												}																			//DM-FEB2011
											else if(!ConditionLeftDepth)													//DM-FEB2011
												{																			//DM-FEB2011
													data->ErosionBank(inode+firstnode,1)=(-(data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station))/(migdata->channel.WIDTH/2);//DM-FEB2011
												}																			//DM-FEB2011
										}
									firstnode = firstnode+data->grids[igrid].numberofnodes;
								}
						}

					// Erosion distance: method 2
					else if (data->MethodErosionDistance == 2)  
						{
							int firstnode = 0;
							int toeleft, toeright;
							for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
								{  
									for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
										{   
											ConditionRightDepth = hdata->DD(inode+firstnode,migdata->channel.nnodes-1) > 0;	//DM-FEB2011																																				//DM-FEB2011
											ConditionLeftDepth = hdata->DD(inode+firstnode,0) > 0;							//DM-FEB2011

											if(ConditionRightDepth && ConditionLeftDepth)									//DM-FEB2011
												{																			//DM-FEB2011										 
													data->ErosionBank(inode+firstnode,1)=(-(data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station)+(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station))/2/(migdata->channel.WIDTH/2);	//CORRECTION FEB2010, CHANGE OF SIGN
												}																			//DM-FEB2011
											else if(!ConditionRightDepth)													//DM-FEB2011
												{																			//DM-FEB2011
													data->ErosionBank(inode+firstnode,1)=((data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station))/(migdata->channel.WIDTH/2);	//DM-FEB2011
												}																			//DM-FEB2011
											else if(!ConditionLeftDepth)													//DM-FEB2011
												{																			//DM-FEB2011
													data->ErosionBank(inode+firstnode,1)=(-(data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station))/(migdata->channel.WIDTH/2);	//DM-FEB2011
												}																			//DM-FEB2011
										}
									
									firstnode = firstnode+data->grids[igrid].numberofnodes;
								}
						}
				}																			//SEP2009
			
			else if(data->MethodPhysicallyBasedBankEvolution == 2)							//SEP2009
				{
					// Type of physically-based migration: physically-based migration coefficient 
					
					// Erosion distance: method 1
					if (data->MethodErosionDistance == 1) 
						{
							int firstnode = 0;
							for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
								{  
									for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
										{   
											//ConditionRightBank1 = (data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station)>(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station);		//DM-FEB2012   No tolerance, Commented out
											ConditionRightBank1 = (data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station)>(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station)+0.000001;	//DM-FEB2012   Added tolerance
											ConditionRightBank2 = data->intersectright[inode+firstnode].station > data->oldintersectright[inode+firstnode].station ;																										//DM-FEB2011
											ConditionRightDepth = hdata->DD(inode+firstnode,migdata->channel.nnodes-1) > 0;																																					//DM-FEB2011
											//ConditionLeftBank1 =(data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station)<(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station);				//DM-FEB2012	No tolerance, Commented out
											ConditionLeftBank1 =(data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station) + 0.000001<(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station);		//DM-FEB2012	Added tolerance
											ConditionLeftBank2 =data->oldintersectleft[inode+firstnode].station > data->intersectleft[inode+firstnode].station;																												//DM-FEB2011
											ConditionLeftDepth = hdata->DD(inode+firstnode,0) > 0;																																											//DM-FEB2011
											
											if(ConditionRightBank1 && ConditionRightBank2 && ConditionRightDepth  || ConditionRightBank2 && !ConditionLeftDepth)  //DM-FEB2011
												{
													data->ErosionBank(inode+firstnode,1)=(-(data->intersectright[inode+firstnode].station - data->oldintersectright[inode+firstnode].station))/(migdata->channel.WIDTH/2);//CORRECTION FEB2010, CHANGE OF SIGN
													
													for(int j = 0; j <= data->grids[0].nodes[inode+firstnode].xsection.leftbank.numberofnodes-1 ; j++)																															//DM-FEB2011
														{																																																						//DM-FEB2011
															data->grids[0].nodes[inode+firstnode].xsection.leftbank.nodes[j].station = data->grids[0].nodes[inode+firstnode].xsection.leftbank.nodes[j].station-data->ErosionBank(inode+firstnode,1)*migdata->channel.WIDTH/2;	//DM-FEB2011
														}																																																						//DM-FEB2011
												}

											else if(ConditionLeftBank1 && ConditionLeftBank2 && ConditionLeftDepth  || ConditionLeftBank2 && !ConditionRightDepth)//DM-FEB2011
												{
													data->ErosionBank(inode+firstnode,1)=(+(data->oldintersectleft[inode+firstnode].station - data->intersectleft[inode+firstnode].station))/(migdata->channel.WIDTH/2);//CORRECTION FEB2010, CHANGE OF SIGN 
													
													for(int j = 0; j <= data->grids[0].nodes[inode+firstnode].xsection.rightbank.numberofnodes-1 ; j++)																															//DM-FEB2011
														{																																																						//DM-FEB2011
															data->grids[0].nodes[inode+firstnode].xsection.rightbank.nodes[j].station = data->grids[0].nodes[inode+firstnode].xsection.rightbank.nodes[j].station-data->ErosionBank(inode+firstnode,1)*migdata->channel.WIDTH/2;//DM-FEB2011
														}																																																						//DM-FEB2011
												}													
											else//ADDITION NOV2010quatris
												{//ADDITION NOV2010quatris
													data->ErosionBank(inode+firstnode,1)=0.0;//ADDITION NOV2010quatris
												}//ADDITION NOV2010quatris
										}
									firstnode = firstnode+data->grids[igrid].numberofnodes;
								}
						}

					// Erosion distance: method 2
					else if (data->MethodErosionDistance == 2)  
						{
							int firstnode = 0;
							int toeleft, toeright;
							for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
								{  
									for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
									{	
											//ConditionRightBank1 = (data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station)>(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station);				//DM-FEB2012	No tolerance, Commented out
											ConditionRightBank1 = (data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station)>(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station)+0.000001;			//DM-FEB2012	Added tolerance
											ConditionRightBank2 = data->toeright[inode+firstnode].station > data->oldtoeright[inode+firstnode].station;																							//DM-FEB2011
											ConditionRightDepth = hdata->DD(inode+firstnode,migdata->channel.nnodes-1) > 0;																												//DM-FEB2011
											//ConditionLeftBank1 =(data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station)<(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station);					//DM-FEB2012	No tolerance, Commented out
											ConditionLeftBank1 =(data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station)+0.000001<(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station);			//DM-FEB2012	Added tolerance
											ConditionLeftBank2 =data->oldtoeleft[inode+firstnode].station > data->toeleft[inode+firstnode].station;																								//DM-FEB2011
											ConditionLeftDepth = hdata->DD(inode+firstnode,0) > 0;																																				//DM-FEB2011
											
											if(ConditionRightBank1 && ConditionRightBank2 && ConditionRightDepth  || ConditionRightBank2 && !ConditionLeftDepth)  //DM-FEB2011
												{
													data->ErosionBank(inode+firstnode,1)=(-(data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station))/(migdata->channel.WIDTH/2);//CORRECTION FEB2010, CHANGE OF SIGN
											
													for(int j = 0; j <= data->grids[0].nodes[inode+firstnode].xsection.leftbank.numberofnodes-1 ; j++)																															//DM-FEB2011
														{																																																						//DM-FEB2011
															data->grids[0].nodes[inode+firstnode].xsection.leftbank.nodes[j].station = data->grids[0].nodes[inode+firstnode].xsection.leftbank.nodes[j].station-data->ErosionBank(inode+firstnode,1)*migdata->channel.WIDTH/2;	//DM-FEB2011
														}																																																						//DM-FEB2011	
												}
											else if(ConditionLeftBank1 && ConditionLeftBank2 && ConditionLeftDepth  || ConditionLeftBank2 && !ConditionRightDepth)//DM-FEB2011
												{
													data->ErosionBank(inode+firstnode,1)=(+(data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station))/(migdata->channel.WIDTH/2);//CORRECTION FEB2010, CHANGE OF SIGN

													for(int j = 0; j <= data->grids[0].nodes[inode+firstnode].xsection.rightbank.numberofnodes-1 ; j++)																															//DM-FEB2011
														{																																																						//DM-FEB2011
															data->grids[0].nodes[inode+firstnode].xsection.rightbank.nodes[j].station = data->grids[0].nodes[inode+firstnode].xsection.rightbank.nodes[j].station-data->ErosionBank(inode+firstnode,1)*migdata->channel.WIDTH/2;//DM-FEB2011
														}																																																						//DM-FEB2011
												}
											else//ADDITION NOV2010quatris
												{//ADDITION NOV2010quatris
													data->ErosionBank(inode+firstnode,1)=0.0;//ADDITION NOV2010quatris
												}//ADDITION NOV2010quatris

										}
									firstnode = firstnode+data->grids[igrid].numberofnodes;
								}
						}

				}
			/*
			for(int inode = 0; inode<=data->grids[0].numberofnodes-1; inode++)																																							//MMMMTEMP Reset cantilever volumes in non leading bank	
				{																																																						//MMMMTEMP Reset cantilever volumes in non leading bank	
					if(data->use_karmor == true)																																														//MMMMTEMP Reset cantilever volumes in non leading bank	
						{																																																				//MMMMTEMP Reset cantilever volumes in non leading bank	
							if(data->ErosionBank(inode,1) > 0.0)																																										//MMMMTEMP Reset cantilever volumes in non leading bank	
								{																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
									//logmsg("%d	Erosion Left	%lf\n",inode, data->ErosionBank(inode,1)*migdata->channel.WIDTH/2.0);																								//MMMMTEMP Reset cantilever volumes in non leading bank	
									data->grids[0].nodes[inode].xsection.rightbank.TotalAblock = data->grids[0].nodes[inode].xsection.rightbank.TotalAblock - data->grids[0].nodes[inode].xsection.rightbank.AblockCantilever;			//MMMMTEMP Reset cantilever volumes in non leading bank	
									data->grids[0].nodes[inode].xsection.rightbank.AblockCantilever = 0.0;																																//MMMMTEMP Reset cantilever volumes in non leading bank	
								}																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
							else if(data->ErosionBank(inode,1) < 0.0)																																									//MMMMTEMP Reset cantilever volumes in non leading bank	
								{																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
									//logmsg("%d	Erosion Right	%lf\n",inode, data->ErosionBank(inode,1)*migdata->channel.WIDTH/2.0);																								//MMMMTEMP Reset cantilever volumes in non leading bank	
									data->grids[0].nodes[inode].xsection.leftbank.TotalAblock = data->grids[0].nodes[inode].xsection.leftbank.TotalAblock - data->grids[0].nodes[inode].xsection.leftbank.AblockCantilever;				//MMMMTEMP Reset cantilever volumes in non leading bank	
									data->grids[0].nodes[inode].xsection.leftbank.AblockCantilever = 0.0;																																//MMMMTEMP Reset cantilever volumes in non leading bank	
								}																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
							else if(data->ErosionBank(inode,1) == 0.0)																																									//MMMMTEMP Reset cantilever volumes in non leading bank	
								{																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
									//logmsg("%d	No Erosion	%lf\n",inode, data->ErosionBank(inode,1)*migdata->channel.WIDTH/2.0);																									//MMMMTEMP Reset cantilever volumes in non leading bank	
								}																																																		//MMMMTEMP Reset cantilever volumes in non leading bank	
						}																																																				//MMMMTEMP Reset cantilever volumes in non leading bank	
				}																																																						//MMMMTEMP Reset cantilever volumes in non leading bank	
			*/
			
		}


}



void RMBankErosion::PhysicalBankEvolution(RMBankErosionData* data, RMHydrodynamicsData* hdata, RMMigrationData* migdata, RMPrepData* prepdata, double deltaT, int iteration)	//DM-FEB2012	
{
	int firstnode, top, toe, point; 
	double stage, reachlength, roughnessratio, rslope, maxundercut, bedtransverseslope;
	double erodedarealeft, erodedarearight;
	Bank *Abank, TempStoredLeftBank, TempStoredRightBank;
	CrossSection *xsection;
	Bank StoreBank;//DM-FEB2012
	Coordinates StoreBankToe;  //MMMMTEMP For first iteration computations 
	double distance; 
	double StoreKarmor;//DM-FEB2012
    double StoreTotalAblock;//DM-FEB2012
	
	// Loop over grids				
	firstnode = 0;
	for (int igrid = 0; igrid <= data->numberofgrids-1; igrid++)
		{
			int numberofnodes = data->grids[igrid].numberofnodes;
			// Loop over nodes
			for (int inode = 0; inode <= numberofnodes-1; inode++)
			{  
					TempStoredLeftBank = data->grids[igrid].nodes[inode].xsection.leftbank;
					TempStoredRightBank = data->grids[igrid].nodes[inode].xsection.rightbank;

                    //TODO: rivermile is accessed here but is never set.
					xsection = &data->grids[igrid].nodes[inode].xsection;

					/* //DM-FEB2011 Commented out
					if (inode == 0)
						{
							reachlength = 0.5*(data->grids[igrid].nodes[inode+1].xsection.rivermile - xsection->rivermile);
						}
					else if (inode == numberofnodes-1)
						{
							reachlength = 0.5*(xsection->rivermile - data->grids[igrid].nodes[inode-1].xsection.rivermile);
						}
					else
						{
							reachlength = 0.5 * (data->grids[igrid].nodes[inode+1].xsection.rivermile - data->grids[igrid].nodes[inode-1].xsection.rivermile);
						}
					*/
			
					//Calculate effective friction slope for streambank material
					ComputeFrictionSlope(data, hdata, migdata, prepdata);//DM-FEB2011
					roughnessratio = max(min(data->grainroughnessthreshold/xsection->effectivefrictionfactor, 1.), 0.3);
					rslope=	fabs(data->flowworkspace[inode+firstnode].frictionslope) * roughnessratio * sqrt(roughnessratio);		

					double bedtransverseslope = (xsection->rightbank.nodes[0].elevation-xsection->leftbank.nodes[xsection->leftbank.numberofnodes-1].elevation)/(xsection->rightbank.nodes[0].station-xsection->leftbank.nodes[xsection->leftbank.numberofnodes-1].station);
					if(inode == data->MonitorNode)printf("RIGHT TOE	%lf	%lf\n",xsection->rightbank.nodes[0].station,xsection->rightbank.nodes[0].elevation);//MMMMTEMP
					if(inode == data->MonitorNode)printf("LEFT TOE	%lf	%lf\n",xsection->leftbank.nodes[xsection->leftbank.numberofnodes-1].station,xsection->leftbank.nodes[xsection->leftbank.numberofnodes-1].elevation);//MMMMTEMP

					erodedarealeft = 0.0;			// Initialize eroded area on the left for this time step and this cross section
					erodedarearight = 0.0;			// Initialize eroded area on the right for this time step and this cross section

					//Compute fluvial erosion for left bank
					Abank = &data->grids[igrid].nodes[inode].xsection.leftbank;																						// in CONCEPTS Abank points directly to the pointer xsection
					if(iteration == 1 && data->use_karmor == true)							//DM-FEB2012
						{																	//DM-FEB2012
							StoreBank = *Abank;												//DM-FEB2012
							StoreBankToe = Abank->nodes[Abank->numberofnodes-1];			//MMMMTEMP For first iteration computations 
						}																	//DM-FEB2012
					toe = Abank->numberofnodes -1;																												// concepts uses SIZE(Abank%nodes) since the dimensionality of nodes is allocated
					top = 0;
					stage =hdata->HH(inode+firstnode,0);
					maxundercut = 0.0;  


					//if (data->fluvialerosionswitch)
					if(data->fluvialerosionswitch && hdata->DD(inode+firstnode,0)>0.0)//MMMMMMTEMP No fluvial erosion for negative depth at bank
						{
							FluvialErosion(Abank,toe,top,xsection,stage,rslope,igrid,inode,firstnode,bedtransverseslope,maxundercut,hdata, data, migdata, deltaT, erodedarealeft); 
						}

					Abank->AblockPlanar = 0.0;														//Reset of AblockPlanar			//DM-FEB2012 Moved here
					Abank->AblockCantilever = 0.0;													//Reset of AblockCantilever		//DM-FEB2012 Moved here

/*
if(inode == data->MonitorNode)printf("AFTER FluvialErosion\n");//MMMMTEMP AdNo
if(inode == data->MonitorNode)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP AdNo
*/

					//Compute cantilever failure for left bank
					while(maxundercut > data->undercutthreshold && data->cantileverfailureswitch)	
						{	
							CantileverFailure(Abank, maxundercut, data, erodedarealeft, inode);//DM-FEB2012
						}

/*
if(inode == data->MonitorNode)printf("AFTER CantileverFailure\n");//MMMMTEMP AdNo
if(inode == data->MonitorNode)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP AdNo
*/

					//Compute planar failure for left bank
					//if(data->planarfailureswitch)
					if(data->planarfailureswitch && hdata->DD(inode+firstnode,0)>0.0)//MMMMMMTEMP No planar for negative depth at bank
						{
							PlanarFailure(Abank, data, hdata, inode, firstnode, migdata, maxundercut);
						}
					else																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
						{																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->AblockPlanar = 0.0;															//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->FactorOfSafety = 9999.9999;													//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureAngle = 9999.9999;												//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureToe.station = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureToe.elevation = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.weight = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.weightslices = 9999.9999;									//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.waterweight = 9999.9999;									//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
						}																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"	



					if(data->use_karmor == true)																//DM-FEB2012
						{																						//DM-FEB2012
							DecayBlock(Abank, data, deltaT, migdata->interm_factor,iteration);					//DM-FEB2012
							ComputeKarmor(Abank, data, inode,iteration,migdata->interm_factor,deltaT);					//DM-FEB2012
							
							if(iteration == 1)																	//DM-FEB2012
								{																				//DM-FEB2012
									StoreKarmor = Abank->Karmor;												//DM-FEB2012
									StoreTotalAblock = Abank->TotalAblock;										//DM-FEB2012																			
									//*Abank = StoreBank;															//DM-FEB2012	//MMMMTEMP Commented out
									distance = Abank->nodes[Abank->numberofnodes-1].station - StoreBankToe.station;		//MMMMTEMP For first iteration computations 
									for(int t = 0; t <= Abank->numberofnodes-1; t++)Abank->nodes[t].station = Abank->nodes[t].station - distance; //MMMMTEMP For first iteration computations  
									Abank->Karmor = StoreKarmor;												//DM-FEB2012
									Abank->TotalAblock = StoreTotalAblock;										//DM-FEB2012
								}																				//DM-FEB2012
					
							
						}																						//DM-FEB2012

			
					// Compute fluvial erosion for right bank
					Abank = &data->grids[igrid].nodes[inode].xsection.rightbank;	
					if(iteration == 1 && data->use_karmor == true)		//DM-FEB2012
						{												//DM-FEB2012
							StoreBank = *Abank;							//DM-FEB2012
							StoreBankToe = Abank->nodes[0];				//MMMMTEMP For first iteration computations 
						}												//DM-FEB2012
					top = Abank->numberofnodes -1;																												//concepts uses SIZE(Abank%nodes) since the dimensionality of nodes is allocated
					toe = 0;
					stage =hdata->HH(inode+firstnode,migdata->channel.nnodes-1);
					maxundercut = 0.0;   




					//if (data->fluvialerosionswitch)
					if(data->fluvialerosionswitch && hdata->DD(inode+firstnode,migdata->channel.nnodes-1)>0.0)//MMMMMMTEMP No fluvial erosion for negative depth at bank
						{
							FluvialErosion(Abank,toe,top,xsection,stage,rslope,igrid,inode,firstnode,bedtransverseslope,maxundercut,hdata, data, migdata, deltaT, erodedarearight); 
						}
	
					Abank->AblockPlanar = 0.0;														//Reset of AblockPlanar			//DM-FEB2012 Moved here
					Abank->AblockCantilever = 0.0;													//Reset of AblockCantilever		//DM-FEB2012 Moved here




					//Compute cantilever failure for right bank
					while(maxundercut > data->undercutthreshold && data->cantileverfailureswitch)
						{
							CantileverFailure(Abank, maxundercut, data, erodedarearight,inode);//DM-FEB2012
						}

					//Compute planar failure for right bank
					//if(data->planarfailureswitch)
					if(data->planarfailureswitch && hdata->DD(inode+firstnode,migdata->channel.nnodes-1)>0.0)//MMMMMMTEMP No planar for negative depth at bank
						{
							PlanarFailure(Abank, data, hdata, inode, firstnode, migdata, maxundercut);
						}
					else																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
						{																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->AblockPlanar = 0.0;															//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->FactorOfSafety = 9999.9999;													//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureAngle = 9999.9999;												//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureToe.station = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->PlanarFailureToe.elevation = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.weight = 9999.9999;										//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.weightslices = 9999.9999;									//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
							Abank->failureparameters.waterweight = 9999.9999;									//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"
						}																						//DM-FEB2013 Quantities to be printed in file "Karmor.DAT"		
					

					if(data->use_karmor == true)																//DM-FEB2012
						{																						//DM-FEB2012
							DecayBlock(Abank, data, deltaT, migdata->interm_factor,iteration);													//DM-FEB2012
							ComputeKarmor(Abank, data, inode,iteration,migdata->interm_factor,deltaT);				//DM-FEB2012
							
							if(iteration == 1)																	//DM-FEB2012
								{																				//DM-FEB2012
									StoreKarmor = Abank->Karmor;												//DM-FEB2012
									StoreTotalAblock = Abank->TotalAblock;										//DM-FEB2012
									//*Abank = StoreBank;															//DM-FEB2012			//MMMMTEMP Commented out
									distance = Abank->nodes[0].station - StoreBankToe.station;		//MMMMTEMP For first iteration computations 
									for(int t = 0; t <= Abank->numberofnodes-1; t++)Abank->nodes[t].station = Abank->nodes[t].station - distance; //MMMMTEMP For first iteration computations  
									Abank->Karmor = StoreKarmor;												//DM-FEB2012
									Abank->TotalAblock = StoreTotalAblock;										//DM-FEB2012
								}																				//DM-FEB2012
							
						}																						//DM-FEB2012

					data->oldintersectleft[inode+firstnode] = data->intersectleft[inode+firstnode];
				    data->oldintersectright[inode+firstnode] = data->intersectright[inode+firstnode];
					data->oldtoeleft[inode+firstnode] = data->toeleft[inode+firstnode];
				    data->oldtoeright[inode+firstnode] = data->toeright[inode+firstnode];

					
					//IntersectCrossSectionAndStageAndToes(data, hdata, migdata);

/*
if(inode == 0)
{
	logmsg("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	logmsg("%lf	%lf \n", data->oldintersectleft[inode+firstnode].station, data->oldintersectleft[inode+firstnode].elevation);
	logmsg("%lf	%lf \n", data->oldintersectright[inode+firstnode].station,data->oldintersectright[inode+firstnode].elevation);
	logmsg("%lf	%lf \n", data->oldtoeleft[inode+firstnode].station,data->oldtoeleft[inode+firstnode].elevation);
	logmsg("%lf	%lf \n", data->oldtoeright[inode+firstnode].station,data->oldtoeright[inode+firstnode].elevation);
	logmsg("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}
*/

					//Deposition (optional) for the inner bank
					/*
					if(data->DepositionSwitch)
						{						
							if (data->oldtoeleft[inode+firstnode].station-data->toeleft[inode+firstnode].station > data->toeright[inode+firstnode].station-data->oldtoeright[inode+firstnode].station)
								{
									// Erosion on the left bank, deposition on the right bank 
									
									data->grids[igrid].nodes[inode].xsection.rightbank = TempStoredRightBank;		// Reset erosion on the right bank
									erodedarearight = 0.0;															// Reset eroded area on the right bank
									IntersectCrossSectionAndStageAndToes(data, hdata, migdata);						// Recalculate intersect and toe at the right bank
									
									double erosionleft = data->oldtoeleft[inode+firstnode].station - data->toeleft[inode+firstnode].station;		// Erosion on the left bank
									
									Abank = &data->grids[igrid].nodes[inode].xsection.rightbank;
						
									int ilayer = 0;																									// Find the bank layer where the intersection between water stage and bank is
									while(hdata->HH(inode+firstnode,migdata->channel.nnodes-1) < Abank->material[ilayer+1].elevation)
										{ilayer = ilayer + 1;}
																														
									AddNodeElevation(Abank->nodes, Abank->numberofnodes, data->intersectright[inode+firstnode]);					// Add node at the intersection with the water stage (it will move)
									Abank->SetNumberOfNodes(Abank->numberofnodes + 1);
									for (int counter = 0; counter <= ilayer-1; counter++)
										{
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									
									for (int counter = 0; counter <= ilayer; counter++)
										{
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
									
									Coordinates intersectrightplus;																					// Add another node at the intersection with the water stage (it will not move)
									intersectrightplus.station = data->intersectright[inode+firstnode].station;
									intersectrightplus.elevation = data->intersectright[inode+firstnode].elevation+0.000000001;
									
									AddNodeElevation(Abank->nodes, Abank->numberofnodes, intersectrightplus);
									Abank->SetNumberOfNodes(Abank->numberofnodes + 1);
									

									for (int counter = 0; counter <= ilayer-1; counter++)
										{
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}							
									for (int counter = 0; counter <= ilayer; counter++)
										{
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}			

									
									int counter = 0;																								// Deposition for all nodes whose elevation is non greater than the water stage
									while (Abank->nodes[counter].elevation <= hdata->HH(inode+firstnode,migdata->channel.nnodes-1))
										{
											Abank->nodes[counter].station=Abank->nodes[counter].station-erosionleft;
											counter = counter + 1;
										}

									erodedarearight = -(erosionleft*(hdata->HH(inode+firstnode,migdata->channel.nnodes-1)-Abank->nodes[0].elevation));    //Minus sign because it is deposition
								}
							else
								{
									// Erosion on the right bank, deposition on the left bank 
							
									data->grids[igrid].nodes[inode].xsection.leftbank = TempStoredLeftBank;								// Reset erosion on the left bank
									erodedarealeft = 0.0;																				// Reset eroded area on the left bank
									IntersectCrossSectionAndStageAndToes(data, hdata, migdata);											// Recalculate intersect and toe at the left bank
									
									double erosionright = data->toeright[inode+firstnode].station - data->oldtoeright[inode+firstnode].station;			// Erosion on the right bank
									Abank = &data->grids[igrid].nodes[inode].xsection.leftbank;
								
									int ilayer = 0;																										// Find the bank layer where the intersection between water stage and bank is
									while(hdata->HH(inode+firstnode,0) < Abank->material[ilayer+1].elevation)
										{ilayer = ilayer + 1;}
									
									AddNodeElevation(Abank->nodes, Abank->numberofnodes, data->intersectleft[inode+firstnode]);							// Add node at the intersection with the water stage (it will move)
									Abank->SetNumberOfNodes(Abank->numberofnodes + 1);
									
									for (int counter = ilayer; counter <= Abank->numberoflayers-1; counter++)
										{
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = ilayer+1; counter <= Abank->numberoflayers-1; counter++)
										{
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
									
									Coordinates intersectleftplus;																						// Add another node at the intersection with the water stage (it will not move)
									intersectleftplus.station = data->intersectleft[inode+firstnode].station;
									intersectleftplus.elevation = data->intersectleft[inode+firstnode].elevation+0.000000001;
									
									AddNodeElevation(Abank->nodes, Abank->numberofnodes, intersectleftplus);
									Abank->SetNumberOfNodes(Abank->numberofnodes + 1);
									
									for (int counter = ilayer; counter <= Abank->numberoflayers-1; counter++)
										{
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = ilayer+1; counter <= Abank->numberoflayers-1; counter++)
										{
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}									

									int counter = Abank->numberofnodes-1;																				// Deposition for all nodes whose elevation is non greater than the water stage
									while (Abank->nodes[counter].elevation <= hdata->HH(inode+firstnode,0))
										{	
											Abank->nodes[counter].station=Abank->nodes[counter].station+erosionright;
											counter = counter - 1;
										}

									erodedarealeft = -(erosionright*(hdata->HH(inode+firstnode,0)-Abank->nodes[Abank->numberofnodes-1].elevation));    //Minus sign because it is deposition

								}
							
							IntersectCrossSectionAndStageAndToes(data, hdata, migdata);									// Recalculate intersect and toe at both banks
						}
					*/
					
					/*
					data->ReworkedAreaLeft[inode+firstnode] = data->ReworkedAreaLeft[inode+firstnode] + erodedarealeft;
					data->ReworkedAreaRight[inode+firstnode] = data->ReworkedAreaRight[inode+firstnode] + erodedarearight;
					
					if(inode != 0 && inode != migdata->channel.centerline.length-1)
						{
							data->TotalErodedVolume = data->TotalErodedVolume + (erodedarealeft + erodedarearight) * 0.5 * (prepdata->intervals(inode+firstnode+1)+prepdata->intervals(inode+firstnode));
						}
					else if (inode == 0)
						{
							data->TotalErodedVolume = data->TotalErodedVolume + (erodedarealeft + erodedarearight) * 0.5 * (prepdata->intervals(inode+firstnode+1));
						}
					else if (inode == migdata->channel.centerline.length-1)
						{
							data->TotalErodedVolume = data->TotalErodedVolume + (erodedarealeft + erodedarearight) * 0.5 * (prepdata->intervals(inode+firstnode));
						}
					*/

				}			//End loop over nodes
			
			firstnode = firstnode+numberofnodes;

			IntersectCrossSectionAndStageAndToes(data, hdata, migdata);

		}					//End loop over grids



}

void RMBankErosion::AssignInitialSectionProperties(const char* filePath, RMBankErosionData* data)			
{
	char CommentNumberOfGrids[200];													//SEP2009
	char CommentNumberOfNodes[200];													//SEP2009
	char CommentSection[200];														//SEP2009
	char CommentXY[200];															//SEP2009
	char CommentLayersAndGroundwater[200];											//SEP2009
	char CommentLayerLeftBank[200];													//SEP2009
	char CommentLayerRightBank[200];												//SEP2009
	char CommentLayerLeftBankNumberSizeFractions[200];								//SEP2009
	char CommentLayerRightBankNumberSizeFractions[200];								//SEP2009

	FILE   *InitialSectionProperties;	
	InitialSectionProperties=fopen(filePath, "r");
	
	fscanf(InitialSectionProperties,"%d", &data->numberofgrids); 
	data->grids.resize(data->numberofgrids);

    fscanf(InitialSectionProperties,"%s", &CommentNumberOfGrids);					//SEP2009

	for(int igrid=0; igrid<=data->numberofgrids-1; igrid++)
		{

			fscanf(InitialSectionProperties,"%d", &data->grids[igrid].numberofnodes);
			data->grids[igrid].SetNumberOfNodes(data->grids[igrid].numberofnodes);

			fscanf(InitialSectionProperties,"%s", &CommentNumberOfNodes);					//SEP2009

			for(int inode=0; inode <= data->grids[igrid].numberofnodes-1; inode++)    
				{	
					fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.id);
					fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes); 
                    data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes);
					fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes);
                    data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes);
					//fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.bed.frictionfactor);//TOERASE								
					fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.effectivefrictionfactor);						
					//fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.bed.surfacelayer.erosionrate);				//TOERASE		
					//fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.bed.surfacelayer.criticalerosionshearstress);	//TOERASE	
					fscanf(InitialSectionProperties,"%s", &CommentSection);					//SEP2009

					//logmsg("%lf,%d, %d, %lf, %lf, %lf, %lf \n", data->grids[igrid].nodes[inode].xsection.rivermile, data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes, data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes, data->grids[igrid].nodes[inode].xsection.bed.frictionfactor, data->grids[igrid].nodes[inode].xsection.effectivefrictionfactor, data->grids[igrid].nodes[inode].xsection.bed.surfacelayer.erosionrate,data->grids[igrid].nodes[inode].xsection.bed.surfacelayer.criticalerosionshearstress );					

					for(int ivertex=0; ivertex <= data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1; ivertex++)
						{
								fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station);
								fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].elevation);
								fscanf(InitialSectionProperties,"%s", &CommentXY);					//SEP2009
								//logmsg("%lf, %lf \n", data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station, data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].elevation); 
						}

					data->grids[igrid].nodes[inode].xsection.leftbank.direction = -1;
					
					for(int ivertex=0; ivertex <= data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1; ivertex++)
						{
								fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station);
								fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].elevation);
								fscanf(InitialSectionProperties,"%s", &CommentXY);					//SEP2009
								//logmsg("%lf, %lf \n", data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station, data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].elevation); 
						}
					
					data->grids[igrid].nodes[inode].xsection.rightbank.direction = 1;
	
					fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers);
                    data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfLayers(data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers);
					data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers;		//DM-FEB2011
					fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers); 
                    data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfLayers(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers);
					data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers;	//DM-FEB2011
					fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.groundwatertable); 
					fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.groundwatertable);
					fscanf(InitialSectionProperties,"%s", &CommentLayersAndGroundwater);					//SEP2009
					//logmsg("%d,%d, %lf, %lf \n", data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers, data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers, data->grids[igrid].nodes[inode].xsection.leftbank.groundwatertable, data->grids[igrid].nodes[inode].xsection.rightbank.groundwatertable);
					
					data->grids[igrid].nodes[inode].xsection.leftbank.Karmor = 1.0;								//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.TotalAblock = 0.0;						//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.AblockPlanar = 0.0;						//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.AblockCantilever = 0.0;					//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.PlanarFailureAngle = 0.0;					//DM-FEB2012										//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.PlanarFailureToe.station = 0.0;			//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.leftbank.PlanarFailureToe.elevation = 0.0;			//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.Karmor = 1.0;							//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.TotalAblock = 0.0;						//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.AblockPlanar = 0.0;						//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.AblockCantilever = 0.0;					//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.PlanarFailureAngle = 0.0;				//DM-FEB2012										//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.PlanarFailureToe.station = 0.0;			//DM-FEB2012
					data->grids[igrid].nodes[inode].xsection.rightbank.PlanarFailureToe.elevation = 0.0;		//DM-FEB2012

					for(int ilayer=0; ilayer<=data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers-1; ilayer++)
						{
							//logmsg("layer LEFT \n");
							fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].bottomindex);
							fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].topindex); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].elevation); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].unitweight); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].erosionrate); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].criticalshearstress); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].cohesion);
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].reposeangle);
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].phib);
							fscanf(InitialSectionProperties,"%s", &CommentLayerLeftBank);					//SEP2009

							//fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[ilayer].xsection.leftbank.material[ilayer].composition.numberofsizefractions);   //DM-FEB2011			Commented out
                            //data->grids[igrid].nodes[ilayer].xsection.leftbank.material[ilayer].composition.SetNumberOfSizeFractions(data->grids[igrid].nodes[ilayer].xsection.leftbank.material[ilayer].composition.numberofsizefractions); //NOO
							//fscanf(InitialSectionProperties,"%s", &CommentLayerLeftBankNumberSizeFractions);					//SEP2009                                      //DM-FEB2011			Commented out
							//logmsg("%d %d   %lf %d\n", data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].bottomindex, data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].topindex, data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].phib, data->grids[igrid].nodes[ilayer].xsection.leftbank.material[ilayer].composition.numberofsizefractions);
							/*
							for(int isize=0; isize <= data->grids[igrid].nodes[ilayer].xsection.leftbank.material[ilayer].composition.numberofsizefractions-1; isize++)
								{
									fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].composition.sizefraction[isize]);
								}
							*/			
						}

					for(int ilayer=0; ilayer<=data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers-1; ilayer++)
						{
							//logmsg("layer RIGHT \n");
							fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex);
							fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].elevation); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].unitweight); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].erosionrate); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].criticalshearstress); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].cohesion); 
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].reposeangle);
							fscanf(InitialSectionProperties,"%lf", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].phib);
							fscanf(InitialSectionProperties,"%s", &CommentLayerRightBank);					//SEP2009

							//fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[ilayer].xsection.rightbank.material[ilayer].composition.numberofsizefractions);	 //DM-FEB2011			Commented out
                            //data->grids[igrid].nodes[ilayer].xsection.rightbank.material[ilayer].composition.SetNumberOfSizeFractions(data->grids[igrid].nodes[ilayer].xsection.rightbank.material[ilayer].composition.numberofsizefractions);
							//fscanf(InitialSectionProperties,"%s", &CommentLayerRightBankNumberSizeFractions);					//SEP2009										 //DM-FEB2011			Commented out 
							//logmsg("%d %d   %lf %d\n", data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex, data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex, data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].phib, data->grids[igrid].nodes[ilayer].xsection.rightbank.material[ilayer].composition.numberofsizefractions);
							/*
							for(int isize=0; isize <= data->grids[igrid].nodes[ilayer].xsection.rightbank.material[ilayer].composition.numberofsizefractions-1; isize++)
								{
									fscanf(InitialSectionProperties,"%d", &data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].composition.sizefraction[isize]);
								}	
							*/
						}				
				}
		}  

	fclose(InitialSectionProperties); 

}

void RMBankErosion::AssignBankEvolutionProperties(const char* dirPath, RMBankErosionData* data)			
{	
    // Commented out by Nils 2/16/2011 to move this into the main RVRMeander
    // class so it can be read by the ini/config file code.

    //char CommentFluvialErosion[200];					//SEP2009
    //char CommentCantileverFailure[200];					//SEP2009
    //char CommentPlanarFailure[200];						//SEP2009

    //FILE   *BankEvolutionProperties;		

    //BankEvolutionProperties=fopen(resolvePath(dirPath, "BankEvolutionProperties.dat").c_str(), "r");


    //fscanf(BankEvolutionProperties,"%d", &data->fluvialerosionswitch);
    //fscanf(BankEvolutionProperties,"%d", &data->TypeLawFluvialErosion);				//ADDITION MAR 2010
    //fscanf(BankEvolutionProperties,"%d", &data->CrossSectionDivision);
    //fscanf(BankEvolutionProperties,"%lf", &data->grainroughnessthreshold);
    //fscanf(BankEvolutionProperties,"%lf", &data->wpthreshold);
    //fscanf(BankEvolutionProperties,"%lf", &data->MAX_EROSION_DISTANCE);
    //fscanf(BankEvolutionProperties,"%lf", &data->mindistance);
    //fscanf(BankEvolutionProperties,"%s", &CommentFluvialErosion);					//SEP2009

    //fscanf(BankEvolutionProperties,"%d", &data->cantileverfailureswitch);
    //fscanf(BankEvolutionProperties,"%lf", &data->undercutthreshold);
    //fscanf(BankEvolutionProperties,"%s", &CommentCantileverFailure);				//SEP2009

    //fscanf(BankEvolutionProperties,"%d", &data->planarfailureswitch); 
    //fscanf(BankEvolutionProperties,"%d", &data->numberofintersects);
    //fscanf(BankEvolutionProperties,"%lf", &data->zthreshold);
    //fscanf(BankEvolutionProperties,"%lf", &data->tensioncrackoption);
    ////fscanf(BankEvolutionProperties,"%u", &data->chktensioncrack); 
    //fscanf(BankEvolutionProperties,"%d", &data->slicemultiplier); 
    //fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[0]); 
    //fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[1]); 
    //fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[2]);		
    ////fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[3]);		//SEP2009 
    ////fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[4]);		//SEP2009 
    ////fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[5]);		//SEP2009 
    ////fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[6]);		//SEP2009 
    ////fscanf(BankEvolutionProperties,"%d", &data->bankfailureanalysis[7]);		//SEP2009 
    //fscanf(BankEvolutionProperties,"%lf", &data->accuracy); 
    //fscanf(BankEvolutionProperties,"%d", &data->maxiter);
    //fscanf(BankEvolutionProperties,"%d", &data->ANALYSISMETHOD);
    //fscanf(BankEvolutionProperties,"%lf", &data->lambda);
    //fscanf(BankEvolutionProperties,"%s", &CommentPlanarFailure);					//SEP2009
    ////fscanf(BankEvolutionProperties,"%d", &data->sedimentroutingswitch);					//CORRECTION AUG2009			
    ////fscanf(BankEvolutionProperties,"%lf", &data->MAX_COHESIVE_EROSION_DISTANCE);			//CORRECTION AUG2009

    //fclose(BankEvolutionProperties); 
}

void RMBankErosion::PrepareTecPlot3D(RMBankErosionData* data, RMMigrationData* migdata, RMPrepData* prepData)			
{
	int firstnode, Mid;
	double xreltocenter, yreltocenter, xrot, yrot, tanx, mtheta;

//DM-FEB2011 Commented out since I do not need to recompute these quantities
/*
	// Computation of intervals									//ADDITION MAR2010
	//Defining intervals and total streamwise length
	prepData->intervals(0) = 0.0;																
	prepData->sSum = 0.0;
	prepData->coords(0) = 0.0;

	for (int i = 1; i <= migdata->channel.centerline.length-1; i++)
		{
			prepData->intervals(i) = sqrt(pow((migdata->channel.centerline(i,0)-migdata->channel.centerline(i-1,0)),2.0)+pow((migdata->channel.centerline(i,1)-migdata->channel.centerline(i-1,1)),2.0));			
			prepData->sSum = prepData->sSum + prepData->intervals(i);
			prepData->coords(i) = prepData->sSum;
		}

	// Computation of angular amplitude theta					//ADDITION MAR2010

	for (int i = 0; i <= migdata->channel.centerline.length-2; i++)
	{
		tanx = (migdata->channel.centerline(i+1,1)-migdata->channel.centerline(i,1))/(migdata->channel.centerline(i+1,0)-migdata->channel.centerline(i,0));
		if ((migdata->channel.centerline(i+1,0)-migdata->channel.centerline(i,0))>=0.0)
		{
			prepData->theta(i) = atan(tanx);
		}
		else
		{
			if ((migdata->channel.centerline(i+1,1)-migdata->channel.centerline(i,1))>=0.0)
			{
				prepData->theta(i) = M_PI + atan(tanx);
			}
			else
			{
				prepData->theta(i) = -M_PI + atan(tanx);
			}
		}
	}

	mtheta = (prepData->theta(migdata->channel.centerline.length-2)-prepData->theta(migdata->channel.centerline.length-3)) / prepData->intervals(migdata->channel.centerline.length-2);
	prepData->theta(migdata->channel.centerline.length-1) = prepData->theta(migdata->channel.centerline.length-2) + mtheta * prepData->intervals(migdata->channel.centerline.length-1);
 */  //Commented out 

	// Dimensioned centerline
	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)			
		{
			data->grids[0].nodes[i].xsection.xcenter = migdata->channel.centerline(i,0)*migdata->channel.WIDTH/2; 
			data->grids[0].nodes[i].xsection.ycenter = migdata->channel.centerline(i,1)*migdata->channel.WIDTH/2; 
		}	

	// Mounting the cross sections on the centerline
	firstnode = 0;

	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{  
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
				{
					for(int ivertex = 0; ivertex<=data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1; ivertex++)
							{
								data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].x = data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].y = data->grids[0].nodes[inode+firstnode].xsection.ycenter + data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station-data->StationCenterline[inode+firstnode];//DM-FEB2011
								xreltocenter = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].x - data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								yreltocenter = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].y - data->grids[0].nodes[inode+firstnode].xsection.ycenter;
								xrot = xreltocenter * cos(prepData->theta(inode+firstnode)) - yreltocenter * sin(prepData->theta(inode+firstnode));
								yrot = xreltocenter * sin(prepData->theta(inode+firstnode)) + yreltocenter * cos(prepData->theta(inode+firstnode));
								data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].x = -xrot + data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].y = -yrot + data->grids[0].nodes[inode+firstnode].xsection.ycenter;
							}
					for(int ivertex = 0; ivertex<=data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1; ivertex++)
							{
								data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].x = data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].y = data->grids[0].nodes[inode+firstnode].xsection.ycenter + data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station-data->StationCenterline[inode+firstnode];//DM-FEB2011
								xreltocenter = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].x - data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								yreltocenter = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].y - data->grids[0].nodes[inode+firstnode].xsection.ycenter;
								xrot = xreltocenter * cos(prepData->theta(inode+firstnode)) - yreltocenter * sin(prepData->theta(inode+firstnode));
								yrot = xreltocenter * sin(prepData->theta(inode+firstnode)) + yreltocenter * cos(prepData->theta(inode+firstnode));
								data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].x = -xrot + data->grids[0].nodes[inode+firstnode].xsection.xcenter;
								data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].y = -yrot + data->grids[0].nodes[inode+firstnode].xsection.ycenter;
							}
					
				}
			firstnode = firstnode+data->grids[igrid].numberofnodes;
		}

	// Mounting the intersects of banks and water surface on the centerline
	firstnode = 0;
	
	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{  
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
				{
					data->intersectleft[firstnode+inode].x = data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					data->intersectleft[firstnode+inode].y = data->grids[0].nodes[inode+firstnode].xsection.ycenter + data->intersectleft[firstnode+inode].station-data->StationCenterline[inode+firstnode];//DM-FEB2011
					xreltocenter = data->intersectleft[firstnode+inode].x - data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					yreltocenter = data->intersectleft[firstnode+inode].y - data->grids[0].nodes[inode+firstnode].xsection.ycenter;
					xrot = xreltocenter * cos(prepData->theta(inode+firstnode)) - yreltocenter * sin(prepData->theta(inode+firstnode));
					yrot = xreltocenter * sin(prepData->theta(inode+firstnode)) + yreltocenter * cos(prepData->theta(inode+firstnode));
					data->intersectleft[firstnode+inode].x = -xrot + data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					data->intersectleft[firstnode+inode].y = -yrot + data->grids[0].nodes[inode+firstnode].xsection.ycenter;

					data->intersectright[firstnode+inode].x = data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					data->intersectright[firstnode+inode].y = data->grids[0].nodes[inode+firstnode].xsection.ycenter+ data->intersectright[firstnode+inode].station-data->StationCenterline[inode+firstnode];//DM-FEB2011
					xreltocenter = data->intersectright[firstnode+inode].x - data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					yreltocenter = data->intersectright[firstnode+inode].y - data->grids[0].nodes[inode+firstnode].xsection.ycenter;
					xrot = xreltocenter * cos(prepData->theta(inode+firstnode)) - yreltocenter * sin(prepData->theta(inode+firstnode));
					yrot = xreltocenter * sin(prepData->theta(inode+firstnode)) + yreltocenter * cos(prepData->theta(inode+firstnode));
					data->intersectright[firstnode+inode].x = -xrot + data->grids[0].nodes[inode+firstnode].xsection.xcenter;
					data->intersectright[firstnode+inode].y = -yrot + data->grids[0].nodes[inode+firstnode].xsection.ycenter;
				}
			firstnode = firstnode+data->grids[igrid].numberofnodes;
		}

	// Bed region									//ADDITION MAR2010		

	// Computation of the coordinates XX and YY
	Mid = int(migdata->channel.nnodes/2.0 + 1.0);

	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)
		{
			migdata->x(i,Mid-1) = migdata->channel.centerline(i,0); 
			migdata->y(i,Mid-1) = migdata->channel.centerline(i,1);  
			migdata->XX(i,Mid-1) = migdata->x(i,Mid-1)*migdata->channel.WIDTH/2;
			migdata->YY(i,Mid-1) = migdata->y(i,Mid-1)*migdata->channel.WIDTH/2;	
		}

	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)
		{
			for(int j = 0; j <= Mid - 2; j++)
				{
					migdata->x(i,j) = migdata->x(i,Mid-1) - prepData->coordn(j)*sin(prepData->theta(i));		
					migdata->y(i,j) = migdata->y(i,Mid-1) + prepData->coordn(j)*cos(prepData->theta(i));
					migdata->XX(i,j) = migdata->x(i,j)*migdata->channel.WIDTH/2;
					migdata->YY(i,j) = migdata->y(i,j)*migdata->channel.WIDTH/2;	
				}
			for(int j = Mid; j <= migdata->channel.nnodes-1; j++)
				{
					migdata->x(i,j) = migdata->x(i,Mid-1) - prepData->coordn(j)*sin(prepData->theta(i));		
					migdata->y(i,j) = migdata->y(i,Mid-1) + prepData->coordn(j)*cos(prepData->theta(i));
					migdata->XX(i,j) = migdata->x(i,j)*migdata->channel.WIDTH/2;
					migdata->YY(i,j) = migdata->y(i,j)*migdata->channel.WIDTH/2;
				}
		}

}
				

void RMBankErosion::CantileverFailure(Bank *Abank, double& maxundercut, RMBankErosionData* data, double& erodedarea, int inode)      //DM-FEB2012
{	
	int numberofpoints, numberoffppoints, toe, toefp, top, topfp, intersectpoint, layer, toplayer, vertex, point, numberoflayers, counter;
	Coordinates toenode, bottomintersect, intersect, topintersect;
	double area, weight;
	bool intersectsfloodplain;
	vector<Coordinates> nodeworkspace;	//NOO
    nodeworkspace.resize(max(Abank->numberofnodes, 2000)); //NOO  //DM-FEB2011 I changed 50 to 2000
	double failurearea; //DM-FEB2012

	numberoflayers=Abank->numberoflayers;

	Abank->failureparameters.weight = 0.0;
	//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<=Abank->numberofnodes-1; k++)printf("BANK	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP AdNo
	// Initialize toe and top indices.
	numberofpoints = Abank->numberofnodes;																	  //SIZE(Abank%nodes)
   
    if (Abank->direction == 1)        // right bank
		{
			toe = 0; 
			top = numberofpoints-1;
		}
	else if (Abank->direction == -1)  // left bank
		{
			toe = numberofpoints-1; 
			top = 0;
		}

	// Check if the bank is undercut more than allowed.
    toenode = Abank->nodes[Abank->undercuttoe];

	if(fabs(toenode.station-Abank->nodes[Abank->undercut].station) > data->undercutthreshold)   // Cantilever failure: determine volume of slumped material and update cross section.
		{
			// Determine point where failure block intersects the bank or floodplain
			intersectpoint = Abank->undercut + Abank->direction;

			while(Abank->direction*Abank->nodes[intersectpoint].station <= Abank->direction*toenode.station && Abank->direction*intersectpoint < Abank->direction*top)
				{intersectpoint = intersectpoint + Abank->direction;}
	
			// Check if failure block intersects floodplain
			if (Abank->direction*Abank->nodes[intersectpoint].station < Abank->direction*toenode.station)
				{// Failure block intersects floodplain
					intersectsfloodplain = true;
					//	intersect.station = toenode.station;
					//	intersect.elevation = Abank->nodes[top].elevation;
				}
			else
				{// Failure block intersects bank
					intersectsfloodplain = false;
					intersect.station = toenode.station;// Done with function NodeAtYposition in CONCEPTS
					intersect.elevation = Abank->nodes[intersectpoint-Abank->direction].elevation + Slope(Abank->nodes[intersectpoint-Abank->direction],Abank->nodes[intersectpoint])*(toenode.station - Abank->nodes[intersectpoint-Abank->direction].station);
				}
    
			// Go to the top soil layer in the slumping mass
			layer = 0;
			toplayer = 0;
			for(int layer = 0; layer<=Abank->numberoflayers-1; layer++)											  //DO layer=1,SIZE(Abank%material)
				{
					if(Abank->direction*Abank->material[layer].bottomindex < Abank->direction*toe){break;}
					if(Abank->nodes[Abank->material[layer].bottomindex].elevation < intersect.elevation){break;}
					toplayer = toplayer +1;
				}
			

			// Determine the number of soil layers in the slumping mass
			counter = toplayer;
			for(int layer = toplayer; layer<=Abank->numberoflayers-1; layer++)											  //DO layer=toplayer,SIZE(Abank%material)
				{
					if(Abank->direction*Abank->material[layer].bottomindex < Abank->direction*toe){break;}
					if(Abank->nodes[Abank->material[layer].bottomindex].elevation <= toenode.elevation){break;}
					counter = counter +1;
				}
		
			numberoflayers = counter+1;

			//if(inode == data->MonitorNode && Abank->direction == -1)printf("numberoflayers in CANT BLOCK= %d\n",numberoflayers);//MMMMTEMP Printing

			/*  //DM-FEB2012 Commented out
			nodeworkspace[0].station=0.0; nodeworkspace[0].elevation=0.0;
			nodeworkspace[1].station=0.0; nodeworkspace[1].elevation=0.0;
			nodeworkspace[2].station=0.0; nodeworkspace[2].elevation=0.0;
			nodeworkspace[3].station=0.0; nodeworkspace[3].elevation=0.0;
			nodeworkspace[4].station=0.0; nodeworkspace[4].elevation=0.0;
			nodeworkspace[5].station=0.0; nodeworkspace[5].elevation=0.0;
			nodeworkspace[6].station=0.0; nodeworkspace[6].elevation=0.0;
			nodeworkspace[7].station=0.0; nodeworkspace[7].elevation=0.0;
			nodeworkspace[8].station=0.0; nodeworkspace[8].elevation=0.0;
			nodeworkspace[9].station=0.0; nodeworkspace[9].elevation=0.0;
			nodeworkspace[10].station=0.0; nodeworkspace[10].elevation=0.0;
			*/
//////
//DM-FEB2012 Added computation of failed area
			// Compute weight of failure block by soil layer
			//lateralflux = 0.0;
			failurearea = 0.0; //DM-FEB2012
			bottomintersect = intersect;
			
			for(int layer = toplayer; layer<=numberoflayers-1; layer++)			//   DO layer=toplayer,numberoflayers
				{
					// Top valley-side intersect of soil layer and slip surface is the bottom valley-side intersect of the soil layer above
					topintersect = bottomintersect;
					// Assemble vertices of polygon of the area of the soil layer within the failure block
					//Bottom side
					vertex = -1;
					if(layer == numberoflayers-1)
						{// One vertex only: intersect of slip surface and bank face
							nodeworkspace[vertex+1] = toenode;
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Top valley-side vertex(unique)	%d	%lf	%lf\n",vertex + 1, nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							vertex = vertex + 1; 
						}
					else
						{// Two vertices, stream side:
							nodeworkspace[vertex+1] = Abank->nodes[Abank->material[layer].bottomindex];
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Top valley-side vertex(1st)	%d	%lf	%lf\n",vertex+1,nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							// Valley side: intersect of bottom of soil layer and slip surface
							bottomintersect.elevation = Abank->material[layer+1].elevation;
							bottomintersect.station = toenode.station;
							nodeworkspace[vertex+2] = bottomintersect;
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Top valley-side vertex(2nd)	%d	%lf	%lf\n",vertex+1,nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							vertex = vertex + 2; 
						}
					
					// Top valley-side vertex
					nodeworkspace[vertex+1] = topintersect;
					//if(inode == data->MonitorNode && Abank->direction == -1)printf("Top valley-side vertex	%d	%lf	%lf\n",vertex+1,nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
					vertex = vertex + 1;   
					
					// Top stream-side vertex if layer is not the top layer
					if (layer != toplayer)
						{
							nodeworkspace[vertex+1] = Abank->nodes[Abank->material[layer].topindex];
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Top stream-side vertex	%d	%lf	%lf\n",vertex+1,nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							vertex = vertex + 1;
						}

					// Find first node on the bank face that may possibly be added to the polygon
					//point = top;//DM-FEB2012 Commented out
					point = top -Abank->direction;//DM-FEB2012

					while (nodeworkspace[vertex].elevation < Abank->nodes[point].elevation)
						{point = point - Abank->direction;}
		   
					// Add bank nodes to polygon until a node's elevation equals or drops below the elevation of the intersect of bank profile with the bottom of soil layer.
					while (Abank->nodes[point].elevation >= nodeworkspace[0].elevation && Abank->direction*point > Abank->direction*toe)
						{
							nodeworkspace[vertex+1] = Abank->nodes[point];
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Added node	%d	%lf	%lf\n",vertex+1,nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							vertex = vertex + 1 ; 
							point = point - Abank->direction;
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->nodes[point].elevation = %lf, nodeworkspace[0].elevation = %lf\n",Abank->nodes[point].elevation , nodeworkspace[0].elevation);//MMMMTEMP Printing
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->nodes[point].elevation - nodeworkspace[0].elevation = %lf\n",Abank->nodes[point].elevation - nodeworkspace[0].elevation);//MMMMTEMP Printing
						}

					// Compute weight of soil layer and area of failure block
					area = PolygonArea(nodeworkspace,vertex+1); 
//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<=vertex; k++)printf("CANTAREA(layer %d)	%d	%lf	%lf\n",toplayer,k,nodeworkspace[k].station,nodeworkspace[k].elevation);//MMMMTEMP AdNo

			//		erodedarea = erodedarea + area;
					failurearea = failurearea + area;

			//		weight = Abank->material[layer].unitweight * area;
			//		Abank->failureparameters.weight = Abank->failureparameters.weight + weight;

					/*   //DM-FEB2012 Commented out
					nodeworkspace[0].station=0.0; nodeworkspace[0].elevation=0.0;
					nodeworkspace[1].station=0.0; nodeworkspace[1].elevation=0.0;
					nodeworkspace[2].station=0.0; nodeworkspace[2].elevation=0.0;
					nodeworkspace[3].station=0.0; nodeworkspace[3].elevation=0.0;
					nodeworkspace[4].station=0.0; nodeworkspace[4].elevation=0.0;
					nodeworkspace[5].station=0.0; nodeworkspace[5].elevation=0.0;
					nodeworkspace[6].station=0.0; nodeworkspace[6].elevation=0.0;
					nodeworkspace[7].station=0.0; nodeworkspace[7].elevation=0.0;
					nodeworkspace[8].station=0.0; nodeworkspace[8].elevation=0.0;
					nodeworkspace[9].station=0.0; nodeworkspace[9].elevation=0.0;
					nodeworkspace[10].station=0.0; nodeworkspace[10].elevation=0.0;
					*/

			//		// Add slumped material to lateral sediment flux. To convert failed volume of
			//		// bank material to that being in transport, multiply it by the ratio of dry bulk
			//		// weight of bank material and unit weight of sediment (= 26 KN/M3).
			//		//lateralflux = lateralflux + (weight * Abank->material[layer].composition.sizefraction/( gravity * 2650 )) / timestep;
   //  
				}			// End for
			Abank->AblockCantilever = failurearea;  //DM-FEB2012
if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->AblockCantilever	%lf\n",Abank->AblockCantilever);//MMMMTEMP AdNo	
			if(Abank->FloodplainElevation-Abank->material[1].elevation > 0.45)								//MMMMTEMP For GRL paper
				{//MMMMTEMP For GRL paper
					//Abank->TotalAblock = Abank->TotalAblock + Abank->AblockCantilever;//DM-FEB2012
				}//MMMMTEMP For GRL paper
//////
//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("CantRoutine(befUpdateNodes)	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP AdNo
			// Update the nodes at the bank
			if (intersectsfloodplain)
				{// Remove all nodes from the toenode up from the bank profile
					// Set number of points to trim from the bank profile that were a part of the sliding soil mass
					numberofpoints = Abank->direction * ( top - Abank->undercuttoe ) - 1;

					// Trim the points from the profile
					if(Abank->direction == 1)         // right bank
						{
							//TrimAtBack(Abank->nodes,numberofpoints);
							top = Abank->numberofnodes;													//SIZE(Abank%nodes);
						}
					else if (Abank->direction == -1)  // left bank
						{
							//TrimAtFront(Abank->nodes,numberofpoints);
							top = 1;
						}
	
					// Add the top of the failure surface to the bank profile

					Abank->nodes[top] = intersect;
				}
			else
				{// Remove only part of the nodes on the bank
 	
					if (Abank->direction == 1)         // right bank
						{
							for(int point = intersectpoint-2; point>=Abank->undercuttoe+1; point--)			//DO point=intersectpoint-2,Abank%undercuttoe+1,-1
								{
									RemoveNode(Abank->nodes, Abank->numberofnodes, point);
								}
						}
					else if(Abank->direction == -1)          // left bank
						{
							for(int point = Abank->undercuttoe-1; point>=intersectpoint+2; point--)    //DO point=Abank%undercuttoe-1,intersectpoint+2,-1
							{
								{RemoveNode(Abank->nodes,Abank->numberofnodes, point);}
								Abank->undercuttoe = Abank->undercuttoe - 1;
							}
						}

					Abank->nodes[Abank->undercuttoe+Abank->direction] = intersect;

				}
//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("CantRoutine(aftUpdateNodes)	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP AdNo  
			// Determine the intersects of soil layers and new part of the bank profile and update indices.
			AddLayerIntersects(Abank);

			// Reset undercut to the index of the point that is sticking out the most.
			maxundercut = 0.0;

			SetUndercut(Abank, maxundercut, inode);
			//printf("FE routine Abank->direction = %d,inode = %d\n",Abank->direction,inode);//MMMMTEMP
		}																					// End Cantilever failure: determine volume of slumped material and update cross section.	 

	if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("AFTER CANTILEVER	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP
}


int RMBankErosion::SetMargin(const vector<Coordinates>& nodes, double stage, int first, int last, int direction)    
{
	int SetMargin;

	// Compute node number of margin
    SetMargin = first;
    while((nodes[SetMargin].elevation >= stage) && (direction * SetMargin < direction * last))
		{SetMargin = SetMargin + direction;}       
	
	return SetMargin;    
}
 
void RMBankErosion::FluvialErosion(Bank *Abank, int& toe, int& top, CrossSection *xsection, double stage, double rslope, int igrid, int inode, int firstnode, double bedtransverseslope, double& maxundercut, const RMHydrodynamicsData* hdata, RMBankErosionData* data, RMMigrationData* migdata, double deltaT, double& erodedarea) 	
{
	int ilayer, numberoflayers, bottompoint, toppoint, intersectpoint, numberofsizefractions, isize, vertex, point, counter;
	double bottomelevation, topelevation, flowarea, flowareaabovelayer, flowareaonbank, fraction, station;
	double shearstress, layerarea, wettedperimeter, shearstressratio, erosiondistance, erodedvolume;
	double shearstressmethod1, shearstresslayerbelow, scalefactor,referenceshearstress; //DM-FEB2012
	double volumebyclass[10];
	Coordinates bottomintersect, topintersect, surfacepoint, intersect;
	vector<Coordinates> nodeworkspace;	//NOO
    nodeworkspace.resize(max(Abank->numberofnodes, 2000)); //NOO  //DM-FEB2011 I changed 50 to 2000
	vector<Coordinates> nodeworkspace2; //NOO
    nodeworkspace2.resize(max(Abank->numberofnodes, 2000)); //NOO		//DM-FEB2011 I changed 50 to 2000
	double slopebank,anglebank,anglebed,bisectangle,bisectslope; //DM-FEB2011
	double reductionfactor;//DM-FEB2012

	//if(inode == data->MonitorNode && Abank->direction == -1)printf("Depth = %lf\n",hdata->DD(inode+firstnode,0));//MMMMTEMP
	if(inode == data->MonitorNode && Abank->direction == -1)printf("Stage = %lf\n",stage);//MMMMTEMP
	//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP

	numberoflayers = Abank->numberoflayers;															

	if (stage >= Abank->nodes[toe].elevation)		
		{
			// Initialize bottom elevation, flow area, and intersect of water surface and bank
			bottomelevation = Abank->nodes[toe].elevation;
			if (stage > Abank->nodes[top].elevation) 
				{
				intersectpoint = top;
				intersect.station = Abank->nodes[top].station;														//Why is this elevation set to stage and not to Abank.nodes[top].elevation
				intersect.elevation = stage;
				}
			else
				{
				intersectpoint = top;

				intersectpoint = SetMargin(Abank->nodes,stage,top,toe,-Abank->direction);
				
				if (intersectpoint < 0)
				{
					logmsg("Water surface and bank intersect error");
					exit(1);//DM-FEB2011
				}

				intersect.station = Abank->nodes[intersectpoint].station + InverseSlope(Abank->nodes[intersectpoint],Abank->nodes[intersectpoint+Abank->direction]) * (stage-Abank->nodes[intersectpoint].elevation);									// this is done in the function NodeAtZposition in CONCEPTS
				intersect.elevation = stage;
			}
			
			//if(inode == data->MonitorNode && Abank->direction == -1)printf("intersect = %lf	%lf\n",intersect.station,intersect.elevation);//MMMMTEMP


			//Calculation of "surface point" area on bank, if CrossSectionDivision == 2
			if(data->CrossSectionDivision == 2)		//DM_FEB2011
				{
					if(Abank->direction == 1)		slopebank =	(Abank->nodes[toe+1].elevation-Abank->nodes[toe].elevation)/(Abank->nodes[toe+1].station-Abank->nodes[toe].station);
					else if(Abank->direction == -1)	slopebank =	(Abank->nodes[toe-1].elevation-Abank->nodes[toe].elevation)/(Abank->nodes[toe-1].station-Abank->nodes[toe].station);
					anglebank = atan(slopebank);											
					if(anglebank < 0)anglebank = anglebank + M_PI;							
					anglebed = atan(bedtransverseslope);									
					if(Abank->direction == 1)anglebed = anglebed + M_PI;					
					bisectangle = (anglebank+anglebed)/2;
					bisectslope = tan(bisectangle);											
					surfacepoint.station = Abank->nodes[toe].station+(stage-Abank->nodes[toe].elevation)/bisectslope;
					surfacepoint.elevation = stage;
					// In CONCEPTS: GetLookupTableValue(xsection%rightsurfacepoint, & stage-xsection%bed%nodes(xsection%bed%thalweg)%elevation)
					//  rightsurfacepoint      intersect of water surface and line bisecting the right bank and bed flow regions
					// In CONCEPTS: GetLookupTableValue(xsection%leftsurfacepoint, & stage-xsection%bed%nodes(xsection%bed%thalweg)%elevation)
					//   leftsurfacepoint       intersect of water surface and line bisecting the left bank and bed flow regions	
				}

			//Calculation of flow area on bank (done according to the CrossSectionDivision mode selected)
			nodeworkspace2[0] = intersect;
			point = intersectpoint+Abank->direction;
			vertex = 0;
			while(Abank->nodes[point].elevation > Abank->nodes[toe].elevation)
				{
					nodeworkspace2[vertex+1] = Abank->nodes[point- Abank->direction];
					vertex = vertex + 1 ; 
					point = point - Abank->direction;	
				}

			if(data->CrossSectionDivision == 1)
				{
					nodeworkspace2[vertex+1].station = Abank->nodes[toe].station;
					nodeworkspace2[vertex+1].elevation = stage;
					vertex = vertex +1;
				}
			else if(data->CrossSectionDivision == 2)
				{
					nodeworkspace2[vertex+1].station = surfacepoint.station;
					nodeworkspace2[vertex+1].elevation = surfacepoint.elevation;
					vertex = vertex +1;
				}

			flowarea = PolygonArea(nodeworkspace2,vertex+1);			//CONCEPTS uses the INTERFACE GetLookupTableValue(Abank%flowarea,flowworkspace(firstnode+inode-1)%depth);
if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= vertex; k++)printf("FLOWAREA	%d	%lf	%lf\n",k,nodeworkspace2[k].station,nodeworkspace2[k].elevation);//MMMMTEMP
			flowareaonbank = flowarea;	

			// Loop over soil layers composing the bank material (from lowest to highest layer)
			for (int ilayer = numberoflayers-1; ilayer >=0; ilayer--)    // As it is in CONCEPTS, it makes a difference in the calculation of the areas of the polygon above the layers	
				{
					// Check if layer is above bank toe and (partially) submerged
					topelevation = Abank->material[ilayer].elevation;																// topelevation of a layer is its upper boundary elavatio
					if (ilayer == 0) {topelevation = Abank->nodes[top].elevation;}													// Layer 1 is the one at the top
					if (topelevation <= bottomelevation) 
						{
							// layer topelevation below toe  //This case, possible in CONCEPTS, is not possible here
						}	
					else		
						{		
							// Set bottom elevation to top elevation
							//            bottomelevation = topelevation																		// I don't understand what happens here

							// Set bottom intersect of soil layer and bank face
							bottompoint = Abank->material[ilayer].bottomindex;
							if (Abank->direction*bottompoint < Abank->direction*toe) {bottompoint = toe;}
							bottomintersect = Abank->nodes[bottompoint];

							if(stage > bottomintersect.elevation)	//Layer is not dry (i.e., it is not above the water surface)						//DM-FEB2011
								{																														//DM-FEB2011
									// Set top intersect and calculate fraction of flow area above the layer
									if (topelevation >= stage)					//! Partially submerged
										{	
											//if(inode == data->MonitorNode && Abank->direction == -1)printf(">>>>>>>>>>>>>>> Partially submerged (topelevation = %lf)\n",topelevation);//MMMMTEMP AdNo
											toppoint = intersectpoint;
											topintersect = intersect;
											layerarea = flowarea;
										}
									else										//Fully submerged
										{		
											//if(inode == data->MonitorNode && Abank->direction == -1)printf(">>>>>>>>>>>>>>> Fully submerged (topelevation = %lf)\n",topelevation);//MMMMTEMP AdNo
											toppoint = Abank->material[ilayer].topindex; 
											topintersect = Abank->nodes[toppoint];
											//if(inode == data->MonitorNode && Abank->direction == -1)printf(">>>>>>>>>>>>>>> Fully submerged (toppoint = %d)\n",toppoint);//MMMMTEMP AdNo
											// Assemble polygon of water above the soil layer
											nodeworkspace2[0] = topintersect;	
																												
											if(data->CrossSectionDivision == 1)
												{// Vertical dividers between bank and bed regions
													nodeworkspace2[1].station = topintersect.station;
													nodeworkspace2[1].elevation = stage;
												}
											else if(data->CrossSectionDivision == 2)
												{
													fraction = (topintersect.elevation-Abank->nodes[toe].elevation)/(stage-Abank->nodes[toe].elevation );
													if (ilayer == 0 && stage > topintersect.elevation) {fraction = 1.0;}					// This is not a relevant case because if the layer has index 0 and it is not dry it must be partially submerged, because the only flow admitted in RVR Meander is an in-channel flow (differently from CONCEPTS).
													station = surfacepoint.station + fraction*(intersect.station-surfacepoint.station);		// Valid for both left and right bank 
													nodeworkspace2[1].station = station;
													nodeworkspace2[1].elevation = stage;
												}
																															
											// Add intersect to polygon
											nodeworkspace2[2] = intersect;
											vertex = 2;
																	
											// Add possible points on the bank to polygon
											point = intersectpoint;

											while(Abank->nodes[point].elevation > nodeworkspace2[0].elevation)
												{
													nodeworkspace2[vertex+1] = Abank->nodes[point];

													vertex = vertex + 1 ; 
													point = point - Abank->direction;				// Increment vertex and decrement bank profile index               
												}
																																	
											//Compute flow area
											flowareaabovelayer = PolygonArea(nodeworkspace2,vertex+1);
											if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= vertex; k++)printf("FLOWAREAABOVELAYER	%d	%lf	%lf\n",k,nodeworkspace2[k].station,nodeworkspace2[k].elevation);//MMMMTEMP
											if(data->CrossSectionDivision == 2)
											{flowareaabovelayer = min(flowareaabovelayer,(1.0-fraction)*flowareaonbank);}      // fraction is the fraction of flow area impacted by a bank above a specified soil layer (M^2)
											layerarea = flowarea - flowareaabovelayer;
											flowarea = flowareaabovelayer;	
										}				
								}																									//DM-FEB2011
							else				//Layer is dry (i.e., it is above the water surface)								//DM-FEB2011
								{																									//DM-FEB2011
									layerarea = 0.0;																				//DM-FEB2011
								}																									//DM-FEB2011

							// Calculate wetted perimeter
							if(stage > bottomintersect.elevation)																	//DM-FEB2011
								{																									//DM-FEB2011
									vertex = Abank->direction * (toppoint-bottompoint) + 1;

									for(counter=0;counter<=vertex-1;counter++)    //FORISSUE
										{//nodeworkspace(1:vertex) = Abank%nodes(bottompoint:toppoint:Abank%direction)
											nodeworkspace2[counter]=Abank->nodes[bottompoint+counter*Abank->direction];
										} 
				    
									if(topelevation >= stage)
										{
											nodeworkspace2[vertex] = intersect;						// Warning: in CONCEPTS there is topintersect but I changed with intersect
											vertex = vertex+1;
										}

									wettedperimeter = DistanceNodeArray(nodeworkspace2,vertex);				// Vertex is OK, not vertex+1
								}																																				//DM-FEB2011
							else																																				//DM-FEB2011
								{																																				//DM-FEB2011
									wettedperimeter = 1.0; //This is just a non-zero number, so that the hydraulic radius is not indefinite (it has to be zero for dry layer)	//DM-FEB2011
								}																																				//DM-FEB2011
							
							// Calculate average shear stress across the layer
							if (data->MethodShearStress == 1)
								{	//tau = gamma * flowarea(on layer)* frictionslope / wetted perimeter (of layer)
									//Check if wetted perimeter exceeds the threshold.  If not use the cross-sectional average boundary shear stress 
									if (wettedperimeter < data->wpthreshold)
										{
											//shearstress = migdata->channel.WaterDensity * 9.80665 * rslope * layerarea / data->flowworkspace[firstnode+inode].wettedperimeter;	//DM-FEB2012 Commented out
											if(ilayer == numberoflayers-1) shearstress = 0.0;																						//DM-FEB2012
											else shearstress = shearstresslayerbelow;																								//DM-FEB2012
										}							
									else
										{
											shearstress = migdata->channel.WaterDensity * 9.80665 * rslope * layerarea / wettedperimeter;
										}
									shearstresslayerbelow = shearstress;																											//DM-FEB2012
								}
							else if (data->MethodShearStress == 2)
								{
									if(Abank->direction ==-1)//Left bank
										{
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,0) * data->reduction_stress_bank;							//DM-FEB2012
											else shearstress = hdata->TauS(inode+firstnode,0);																						//DM-FEB2012
										}
									if(Abank->direction ==1)//Right bank
										{
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1) * data->reduction_stress_bank;	//DM-FEB2012
											else shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1);																//DM-FEB2012
										}
								}
							else if (data->MethodShearStress == 3)
							{		
									if(inode == data->MonitorNode && Abank->direction == -1)printf("--------------------\n");//MMMMTEMP
									if(inode == data->MonitorNode && Abank->direction == -1)printf("ilayer = %d\n",ilayer);//MMMMTEMP
									if (wettedperimeter < data->wpthreshold)
										{
											//shearstressmethod1 = migdata->channel.WaterDensity * 9.80665 * rslope * layerarea / data->flowworkspace[firstnode+inode].wettedperimeter;		//DM-FEB2012 Commented out
											if(ilayer == numberoflayers-1) shearstressmethod1 = 0.0;																						//DM-FEB2012
											else shearstressmethod1 = shearstresslayerbelow;																								//DM-FEB2012
										}							
									else
										{
											shearstressmethod1 = migdata->channel.WaterDensity * 9.80665 * rslope * layerarea / wettedperimeter;
											if(inode == data->MonitorNode && Abank->direction == -1)printf("layerarea = %lf wettedperimeter = %lf rslope = %lf\n",layerarea,wettedperimeter,rslope);//MMMMTEMP
										}

									if (ilayer == numberoflayers-1)
										{
											scalefactor = 1.0;
										}
									else
										{
											//scalefactor = shearstressmethod1/shearstresslayerbelow;		//DM-FEB2012 //MMMMTEMP Commented out
											//scalefactor = shearstressmethod1/referenceshearstress;			//DM-FEB2012  //MMMMTEMP Commented out
											if(referenceshearstress != 0.0)scalefactor = shearstressmethod1/referenceshearstress;			//MMMMTEMP Shear stress distribution 3
											//else scalefactor = 0.0;																		//MMMMTEMP Shear stress distribution 3
											else																							//MMMMTEMP Shear stress distribution 3
												{																							//MMMMTEMP Shear stress distribution 3
													scalefactor = 1.0;																		//MMMMTEMP Shear stress distribution 3
													referenceshearstress = shearstressmethod1;												//MMMMTEMP Shear stress distribution 3
												}																							//MMMMTEMP Shear stress distribution 3
											
											if(inode == data->MonitorNode && Abank->direction == -1)printf("shearstressmethod1 = %lf, referenceshearstress = %lf\n",shearstressmethod1,referenceshearstress);//MMMMTEMP
										}
												
									if(Abank->direction ==-1)
										{
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,0)*scalefactor * data->reduction_stress_bank;									//DM-FEB2012	
											else shearstress = hdata->TauS(inode+firstnode,0)*scalefactor;																								//DM-FEB2012
											if(inode == data->MonitorNode && Abank->direction == -1)printf("hdata->TauS(inode+firstnode,0) = %lf\n",hdata->TauS(inode+firstnode,0));//MMMMTEMP
										}
									if(Abank->direction ==1)
										{
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1)*scalefactor * data->reduction_stress_bank;			//DM-FEB2012
											else shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1)*scalefactor;																		//DM-FEB2012
										}
									if(ilayer == numberoflayers-1)
										{
											referenceshearstress = shearstressmethod1;			//DM-FEB2012
										}
									shearstresslayerbelow = shearstressmethod1;					//DM-FEB2012

									if(inode == data->MonitorNode && Abank->direction == -1)printf("shearstress = %lf\n",shearstress);//MMMMTEMP
								}
							else if (data->MethodShearStress == 4)																																									//MMMMTEMP Additional stress distribution method
								{																																															//MMMMTEMP Additional stress distribution method
									if(ilayer == numberoflayers-1)																																									//MMMMTEMP Additional stress distribution method
										{																																															//MMMMTEMP Additional stress distribution method
											scalefactor = 1.0;																																										//MMMMTEMP Additional stress distribution method
										}																																															//MMMMTEMP Additional stress distribution method
									else																																															//MMMMTEMP Additional stress distribution method
										{																																															//MMMMTEMP Additional stress distribution method
											if(Abank->direction == -1)scalefactor = (intersect.elevation - Abank->material[ilayer+1].elevation)/(intersect.elevation - Abank->nodes[Abank->numberofnodes-1].elevation);			//MMMMTEMP Additional stress distribution method
											else if(Abank->direction == 1)scalefactor = (intersect.elevation - Abank->material[ilayer+1].elevation)/(intersect.elevation - Abank->nodes[0].elevation);								//MMMMTEMP Additional stress distribution method
										}																																															//MMMMTEMP Additional stress distribution method
									if(Abank->direction == -1)																																										//MMMMTEMP Additional stress distribution method
										{																																															//MMMMTEMP Additional stress distribution method
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,0)*scalefactor * data->reduction_stress_bank;																//MMMMTEMP Additional stress distribution method	
											else shearstress = hdata->TauS(inode+firstnode,0)*scalefactor;																															//MMMMTEMP Additional stress distribution method
										}																																															//MMMMTEMP Additional stress distribution method
									if(Abank->direction == 1)																																										//MMMMTEMP Additional stress distribution method
										{																																															//MMMMTEMP Additional stress distribution method
											if(data->reduce_stress_bank == true)shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1)*scalefactor * data->reduction_stress_bank;										//MMMMTEMP Additional stress distribution method
											else shearstress = hdata->TauS(inode+firstnode,migdata->channel.nnodes-1)*scalefactor;																									//MMMMTEMP Additional stress distribution method
										}																																															//MMMMTEMP Additional stress distribution method
								}
//if(inode == data->MonitorNode && Abank->direction == -1)printf(">>>>>>>shearstress layer = %d	%lf\n",ilayer,shearstress);//MMMMTEMP
							// If the average shear stress is less than the critical shear stress skip to the next layer.
							shearstressratio = shearstress / Abank->material[ilayer].criticalshearstress;


							//if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->direction = %d, ilayer = %d, shearstressratio = %lf\n",Abank->direction,ilayer,shearstressratio);//MMMMTEMP
							//if(inode == data->MonitorNode && Abank->direction == -1 && ilayer == 0)printf("Stage	%lf\n",stage);//MMMMTEMP
							if (shearstressratio <= 1.) 
								{
									erosiondistance = 0.0;  //ADDITION NOV2010quatris
//if(inode == data->MonitorNode && Abank->direction == -1)printf("EROSION layer = %d	%lf\n",ilayer,erosiondistance);//MMMMTEMP
								}	 
							else
								{
									// Do not scour sediments from the bank in case there is less than 0.05 m of water above the bottom of the soil layer, 
									// even if shear stress is large enough.
									/*if ((stage - bottomintersect.elevation) < 0.05) 
										{
											logmsg("Less than 0.05 m of water above the bottom of the soil layer");
											goto stop;
										}*/
				
									// Calculate lateral erosion distance and limit it

									//This is the original expression for fluvial erosion rate 
									if(data->TypeLawFluvialErosion == 1)		// ADDITION MAR 2010
										{										// ADDITION MAR 2010
											// Classic expression for fluvial erosion
											reductionfactor = 1.0;																																		//DM-FEB2012
											if(migdata->apply_interm == true) reductionfactor = reductionfactor * migdata->interm_factor;																//DM-FEB2012
											if(data->use_karmor == true)reductionfactor = reductionfactor * Abank->Karmor;																				//DM-FEB2012		//MMMMTEMP Computation of reduction factor (method 1, does not consider U1 sign)
				/*
											if(data->use_karmor == true)																																					//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)																									
												{																																											//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
													if(Abank->direction == 1)																																				//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
														{																																									//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
															if(hdata->u(inode+firstnode,migdata->channel.nnodes-1)>0.0)reductionfactor = reductionfactor * Abank->Karmor;													//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
															else reductionfactor = 0.0;																																		//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
														}																																									//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
													else if(Abank->direction == -1)																																			//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
														{																																									//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
															if(hdata->u(inode+firstnode,0)>0.0)reductionfactor = reductionfactor * Abank->Karmor;																			//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
															else reductionfactor = 0.0;																																		//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
														}																																									//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
												}																																											//MMMMTEMP Computation of reduction factor (method 2, considers U1 sign)
				*/
											erosiondistance = min(Abank->material[ilayer].erosionrate*( shearstressratio - 1.)* (deltaT*24*3600) * reductionfactor, data->MAX_EROSION_DISTANCE);		//DM-FEB2012
											//if(inode == data->MonitorNode && Abank->direction ==-1)printf("erosiondistance = %lf\n",erosiondistance);//MMMMTEMP AdNo
											//if(inode == data->MonitorNode && Abank->direction ==-1)printf("TAUS = %lf, Abank->Karmor = %lf\n",hdata->TauS(inode+firstnode,0),Abank->Karmor);//MMMMTEMP AdNo
										}										// ADDITION MAR 2010
									else if(data->TypeLawFluvialErosion == 2)	// ADDITION MAR 2010
										{										// ADDITION MAR 2010
											//This is the expression for fluvial erosion rate with continuous derivative  
											reductionfactor = 1.0;																																		//DM-FEB2012
											if(migdata->apply_interm == true) reductionfactor = reductionfactor * migdata->interm_factor;																//DM-FEB2012
											if(data->use_karmor == true)reductionfactor = reductionfactor * Abank->Karmor;																				//DM-FEB2012
											erosiondistance = min(Abank->material[ilayer].erosionrate/Abank->material[ilayer].criticalshearstress * pow(shearstress,2.0) / (shearstress+Abank->material[ilayer].criticalshearstress)* (deltaT*24*3600) * reductionfactor, data->MAX_EROSION_DISTANCE);   // ADDITION MAR 2010     //DM-FEB2012
										}										// ADDITION MAR 2010

									// Calculate eroded area
									erodedarea = erosiondistance * (topintersect.elevation-bottomintersect.elevation);
									/*	 Not done in the current version of the code
									// Convert eroded volume of bank material to that being transported by the flow:
									// multiply area by the ratio of dry bulk weight of bank material and dry bulk
									// weight of transported material (=26 KN/M^3).
									erodedvolume = erodedvolume * Abank->material[ilayer].unitweight/(gravity*2650);
									numberofsizefractions = Abank->material[ilayer].composition.numberofsizefractions;
									for (int isize = 0; isize <= numberofsizefractions-1; isize++)
										{
											volumebyclass[isize] = erodedvolume * Abank->material[ilayer].composition.sizefraction[isize];
											channelsedimentsupply[firstnode+inode][isize] = channelsedimentsupply[firstnode+inode][isize] + reachlength * volumebyclass[isize];
										}
									*/ 				

//if(inode == data->MonitorNode && Abank->direction == -1)printf("------------------------------------------------------------\n");//MMMMTEMP
if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("BEFORE UPDATING BANK NODES	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP
//if(inode == data->MonitorNode && Abank->direction == -1)printf("CALL UpdateBankNodes for layer = %d\n",ilayer);//MMMMTEMP
if(inode == data->MonitorNode && Abank->direction == -1)printf("EROSION layer = %d	%lf\n",ilayer,erosiondistance);//MMMMTEMP

									UpdateBankNodes(Abank,toe,top,ilayer,intersectpoint,topintersect,erosiondistance,xsection,igrid,inode, data);
//if(inode == data->MonitorNode && Abank->direction == -1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("AFTER UPDATING BANK NODES	%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP
									SetUndercut(Abank, maxundercut, inode);
									
									//if(Abank->direction == -1)printf("Comparison (Left Bank)	%d	%d	%lf	%lf\n",inode,ilayer,intersect.elevation - Abank->nodes[Abank->numberofnodes-1].elevation,hdata->DD(inode+firstnode,0));//MMMMMMTEMP
									//if(Abank->direction == 1)printf("Comparison (Right Bank)	%d	%d	%lf	%lf\n",inode,ilayer,intersect.elevation - Abank->nodes[0].elevation,hdata->DD(inode+firstnode,migdata->channel.nnodes-1));//MMMMMMTEMP
								}
						}
				}	// end cycle on ilayer
		}	// endif	
}

double RMBankErosion::InverseSlope(Coordinates node1, Coordinates node2)
{
	double denominator, rsign, InverseSlope;

	denominator = node2.elevation - node1.elevation;															
	if (denominator >= 0) {rsign = fabs(1.0);}  		
	else {rsign = -fabs(1.0);}  
	InverseSlope = (node2.station - node1.station)  / (rsign*max(fabs(denominator),0.000001));						
	return InverseSlope;
}

double RMBankErosion::PolygonArea(const vector<Coordinates>& polygon, int numberofvertices)
{
    int vertex, nextvertex;
	double PolygonArea = 0.0;

	/*for(int i =0; i <= numberofvertices - 1; i ++)
		{
			logmsg("AREA i = %d, x = %lf, y = %lf \n", i, polygon[i].station, polygon[i].elevation);
		}*/

    for(vertex=0; vertex<= numberofvertices-1; vertex++)  //FORISSUE
		{
			nextvertex = vertex+1;
			if (nextvertex > numberofvertices-1) {nextvertex = nextvertex - numberofvertices;}
			PolygonArea = PolygonArea + polygon[vertex].station * polygon[nextvertex].elevation - polygon[vertex].elevation * polygon[nextvertex].station;
		}

	PolygonArea = 0.5 * PolygonArea;
	if(PolygonArea < 0.) {PolygonArea = - PolygonArea;}

	//logmsg("AREA = %lf \n", PolygonArea);	

	return PolygonArea;
}

double RMBankErosion::DistanceNodeArray(const vector<Coordinates>& nodes, int numberofpoints)
{   
	int last;
	double DistanceNodeArray;											
    last = numberofpoints;										
	DistanceNodeArray = 0.0;

	for (int point=0; point<=last-2; point++)
		{DistanceNodeArray = DistanceNodeArray + DistanceNode(nodes[point],nodes[point+1]);}
	
	return DistanceNodeArray;
}

double RMBankErosion::DistanceNode(Coordinates node1, Coordinates node2)
{
	double DistanceNode;
	DistanceNode = sqrt(pow((node1.station-node2.station),2) + pow((node1.elevation-node2.elevation),2));
    return DistanceNode;
}
/*
void RMBankErosion::SetUndercut(Bank *Abank, double& maxundercut, int inode)
{
	//if(Abank->direction && inode == 289)for(int r = 0; r <= Abank->numberofnodes-1; r++)printf("SETUNDERCUT	%d	%lf	%lf\n",r,Abank->nodes[r].station,Abank->nodes[r].elevation);//MMMMTEMP Printing

	int numberofpoints, toe, top, toepoint, idirection, point;
	double maximum, minimum, rdistance;

	numberofpoints = Abank->numberofnodes;

	if (Abank->direction == 1) 
		{
			toe = 0;
			top = numberofpoints-1;
		}
	else if (Abank->direction == -1)
		{
			toe = numberofpoints-1;
			top = 0;
		}

	toepoint = toe;
	idirection = Abank->direction;
	Abank->undercut = toe; 
	Abank->undercuttoe = toe;
	maxundercut = 0.;
	minimum = Abank->nodes[toe].station;
	maximum = minimum;

	// Find the point on the bank with the largest overhang
		for (point = toe+Abank->direction ; Abank->direction*point <= top; point = point + Abank->direction)
			{// DO point=toe+Abank%direction,top,Abank%direction
				if(Abank->direction*Abank->nodes[point].station >= Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == Abank->direction)
															//IF (Abank%direction*Abank%nodes(point)%station >= Abank%direction*Abank%nodes(point-Abank%direction)%station &.AND. idirection == Abank%direction) THEN
					{// Channel bank continues to be laid backward.  Increment the toepoint.
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 1)\n",point);//MMMMTEMP Printing
						toepoint = point;
					}
				else if (Abank->direction*Abank->nodes[point].station < Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == Abank->direction)       
															//ELSEIF (Abank%direction*Abank%nodes(point)%station < Abank%direction*Abank%nodes(point-Abank%direction)%station & .AND. idirection == Abank%direction) THEN
					{// Channel bank face changes from being laid backward to overhanging the channel. Set the valleyside extent of the overhang.
						
						maximum = Abank->nodes[point-Abank->direction].station;
						minimum = Abank->nodes[point].station;
						idirection = - Abank->direction;
						if(point==top)												// Part added by Davide Motta, to detect overhanging point and calculate the undercut when it is at the top of the bank
							{
								rdistance = fabs(maximum - minimum);
								if (rdistance > maxundercut)
									{
										maxundercut = rdistance;
										Abank->undercut = point;
										Abank->undercuttoe = toepoint;
									}			
							}
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 2) maximum = %lf	minimum = %lf \n",point,maximum,minimum);//MMMMTEMP Printing
					}
				else if (Abank->direction*Abank->nodes[point].station <= Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == -Abank->direction)
															//ELSEIF (Abank%direction*Abank%nodes(point)%station <= Abank%direction*Abank%nodes(point-Abank%direction)%station &  .AND. idirection == -Abank%direction) THEN
					{// Channel bank overhang is increasing, update the streamside extent of the overhang.
						
						minimum = Abank->nodes[point].station;
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 3), minimum = %lf\n",point,minimum);//MMMMTEMP Printing
					}
		
				else if (Abank->direction*Abank->nodes[point].station > Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == -Abank->direction)
															//ELSEIF (Abank%direction*Abank%nodes(point)%station > Abank%direction*Abank%nodes(point-Abank%direction)%station & .AND. idirection == -Abank%direction) THEN
					{// Channel bank face changes from being overhanging the channel to laid backward. Check how much the bank segment is undercut, and if this is the max overhang.
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 4)\n",point);//MMMMTEMP Printing
						rdistance = fabs(maximum - minimum);
						if (rdistance > maxundercut)
							{
								maxundercut = rdistance;
								Abank->undercut = point - Abank->direction;
								Abank->undercuttoe = toepoint;
							}

						idirection = Abank->direction;
						toepoint = point;
					}//END IF

			}//ENDDO
	
	if(Abank->direction && inode == 289)printf("SETUNDERCUT	maxundercut = %lf\n",maxundercut);//MMMMTEMP Printing
}
*/

void RMBankErosion::SetUndercut(Bank *Abank, double& maxundercut, int inode)		//MMMMTEMP Alternative version of SetUndercut
{
	//if(Abank->direction && inode == 289)for(int r = 0; r <= Abank->numberofnodes-1; r++)printf("SETUNDERCUT	%d	%lf	%lf\n",r,Abank->nodes[r].station,Abank->nodes[r].elevation);//MMMMTEMP

	int numberofpoints, toe, top, toepoint, idirection, point;
	double maximum, minimum, rdistance;
	bool foundmaximum = false; //MMMMTEMP

	numberofpoints = Abank->numberofnodes;

	if (Abank->direction == 1) 
		{
			toe = 0;
			top = numberofpoints-1;
		}
	else if (Abank->direction == -1)
		{
			toe = numberofpoints-1;
			top = 0;
		}

	toepoint = toe;
	idirection = Abank->direction;
	Abank->undercut = toe; 
	Abank->undercuttoe = toe;
	maxundercut = 0.;
	minimum = Abank->nodes[toe].station;
	maximum = minimum;

	// Find the point on the bank with the largest overhang
		for (point = toe+Abank->direction ; Abank->direction*point <= top; point = point + Abank->direction)
			{// DO point=toe+Abank%direction,top,Abank%direction
				if(Abank->direction*Abank->nodes[point].station >= Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == Abank->direction && foundmaximum == false)
															//IF (Abank%direction*Abank%nodes(point)%station >= Abank%direction*Abank%nodes(point-Abank%direction)%station &.AND. idirection == Abank%direction) THEN
					{// Channel bank continues to be laid backward.  Increment the toepoint.
						//if(Abank->direction  && inode == 289)printf("SETUNDERCUT	point = %d (CASE 1)\n",point);//MMMMTEMP Printing
						toepoint = point;
					}
				else if (Abank->direction*Abank->nodes[point].station < Abank->direction*Abank->nodes[point-Abank->direction].station && idirection == Abank->direction && foundmaximum == false)       
															//ELSEIF (Abank%direction*Abank%nodes(point)%station < Abank%direction*Abank%nodes(point-Abank%direction)%station & .AND. idirection == Abank%direction) THEN
					{// Channel bank face changes from being laid backward to overhanging the channel. Set the valleyside extent of the overhang.
						foundmaximum = true; //MMMMTEMP
						maximum = Abank->nodes[point-Abank->direction].station;
						minimum = Abank->nodes[point].station;
						//idirection = - Abank->direction;   //MMMMTEMP Commented out
						if(point==top)												// Part added by Davide Motta, to detect overhanging point and calculate the undercut when it is at the top of the bank
							{
								rdistance = fabs(maximum - minimum);
								if (rdistance > maxundercut)
									{
										maxundercut = rdistance;
										Abank->undercut = point;
										Abank->undercuttoe = toepoint;
									}			
							}
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 2) maximum = %lf	minimum = %lf \n",point,maximum,minimum);//MMMMTEMP Printing
					}
				else if (Abank->direction*Abank->nodes[point].station <= Abank->direction*Abank->nodes[point-Abank->direction].station && foundmaximum == false || foundmaximum == true && Abank->direction*Abank->nodes[point].station < Abank->direction*minimum)
															//ELSEIF (Abank%direction*Abank%nodes(point)%station <= Abank%direction*Abank%nodes(point-Abank%direction)%station &  .AND. idirection == -Abank%direction) THEN
					{// Channel bank overhang is increasing, update the streamside extent of the overhang.
						
						minimum = Abank->nodes[point].station;
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 3), minimum = %lf\n",point,minimum);//MMMMTEMP Printing
					}
		
				else if (Abank->direction*Abank->nodes[point].station > Abank->direction*Abank->nodes[point-Abank->direction].station && foundmaximum == true)
															//ELSEIF (Abank%direction*Abank%nodes(point)%station > Abank%direction*Abank%nodes(point-Abank%direction)%station & .AND. idirection == -Abank%direction) THEN
					{// Channel bank face changes from being overhanging the channel to laid backward. Check how much the bank segment is undercut, and if this is the max overhang.
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 4)\n",point);//MMMMTEMP Printing
						rdistance = fabs(maximum - minimum);
						if (rdistance > maxundercut)
							{
								maxundercut = rdistance;
								Abank->undercut = point - Abank->direction;
								Abank->undercuttoe = toepoint;
							}
						//idirection = Abank->direction; //MMMMTEMP Commented out
						//toepoint = point;  //MMMMTEMP  Commented out
					}//END IF
				else if (foundmaximum == true  && Abank->direction*Abank->nodes[point].station >= Abank->direction*Abank->nodes[point-Abank->direction].station)
					{
						//if(Abank->direction && inode == 289)printf("SETUNDERCUT	point = %d (CASE 5)\n",point);//MMMMTEMP Printing
					}

			}//ENDDO
	
	if(Abank->direction && inode == 289)printf("SETUNDERCUT	maxundercut = %lf\n",maxundercut);//MMMMTEMP Printing
	//if(Abank->direction && inode == 289)printf("toepoint = %d\n",toepoint);//MMMMTEMP Printing
}


void RMBankErosion::UpdateBankNodes(Bank *Abank, int& toe, int& top, int ilayer, int& intersectpoint, Coordinates topintersect,double erosiondistance,CrossSection *xsection, int igrid, int inode, RMBankErosionData* data)
{
	int numberoflayers, point, topindex, counter, bottomindex; 

	numberoflayers = Abank->numberoflayers;

	if(ilayer < Abank->numberoflayers - 1 && topintersect.elevation<Abank->material[ilayer+1].elevation && topintersect.elevation<Abank->nodes[top].elevation)			//MMMMTEMP DRY
		{																																								//MMMMTEMP DRY
			//Layer is above water surface, no update of bank nodes is necessary																						//MMMMTEMP DRY
			//if(inode == data->MonitorNode && Abank->direction == -1)printf("DRY\n");																					//MMMMTEMP
		}																																								//MMMMTEMP DRY
	else																																								//MMMMTEMP DRY
		{																																								//MMMMTEMP DRY

			// Update top of layer
			if(topintersect.elevation<Abank->material[ilayer].elevation && topintersect.elevation<Abank->nodes[top].elevation)  
			//if(fabs(topintersect.elevation-Abank->material[ilayer].elevation)>0.000001 && topintersect.elevation<Abank->nodes[top].elevation) //MMMMTEMP AdNo  Using a tolerance
				{	//if(inode == data->MonitorNode && Abank->direction == -1)printf("PARTIALLY SUBMERGED\n"); //MMMMTEMP
					//if(inode == data->MonitorNode && Abank->direction == -1)printf("topintersect.elevation = %lf\n",topintersect.elevation); //MMMMTEMP
					//if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->material[ilayer].elevation = %lf\n",Abank->material[ilayer].elevation); //MMMMTEMP
					//if(inode == data->MonitorNode && Abank->direction == -1)printf("Abank->nodes[top].elevation = %lf\n",Abank->nodes[top].elevation); //MMMMTEMP
					
					// Layer is partially submerged.  Add flowmargin to bank profile, however, only if there are no bank nodes within mindistance meters.
					point = Abank->material[ilayer].topindex;			// original in concepts, but topindex is not updated
					//		point = Abank->nodes[top].elevation;

					while(Abank->nodes[point].elevation > topintersect.elevation && Abank->direction*point > Abank->direction*Abank->material[ilayer].bottomindex)
						{
							point = point - Abank->direction;
						}

					//if(inode == data->MonitorNode && Abank->direction == -1)printf("BEFore (TopLayer)bottomindex = %d, (TopLayer)topindex = %d\n",Abank->material[0].bottomindex,Abank->material[0].topindex);//MMMMTEMP
					//if(inode == data->MonitorNode && Abank->direction == -1)if(Abank->numberoflayers>1)printf("BEFore (BottomLayer)bottomindex = %d, (BottomLayer)topindex = %d\n",Abank->material[1].bottomindex,Abank->material[1].topindex);//MMMMTEMP

					if(fabs(Abank->nodes[point+Abank->direction].elevation-topintersect.elevation) < data->mindistance)
						{			// Intersect is close to the banknode with index "point+Abank.direction"
							topindex = point + Abank->direction + Abank->direction;		// Warning
						}
					else if (fabs(Abank->nodes[point].elevation-topintersect.elevation) < data->mindistance)
						{           // Intersect is close to the banknode with index "point"
							topindex = point + Abank->direction;    // Warning
						}
					else 
						{	// Add flowmargin

							AddNodeElevation(Abank->nodes, Abank->numberofnodes, topintersect);	

							//Set this variable in AddNodeElevation, rather than here.  Abank->numberofnodes = Abank->numberofnodes +1;				// Added by DM, since nodes is not an allocatable file as in CONCEPTS
							
							//  Reset toe of leftbank and top of rightbank
							if (Abank->direction == -1) {toe = toe + 1;}			// Left bank
							if (Abank->direction == 1) {top = top + 1;}			// Right bank
			
							//  Reset indices of soil layers
							if (Abank->direction == 1)							// Right bank
								{
									//if (ilayer-1>=0)
									for (int counter = 0; counter <= ilayer-1; counter++)
										{//Abank%material(1:ilayer-1)%bottomindex = Abank%material(1:ilayer-1)%bottomindex + 1
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = 0; counter <= ilayer; counter++)
										{//Abank%material(1:ilayer)%topindex = Abank%material(1:ilayer)%topindex + 1
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
								}
							else if (Abank->direction == -1)						// Left bank
								{
									for (int counter = ilayer; counter <= numberoflayers-1; counter++)
										{//Abank%material(ilayer:numberoflayers)%bottomindex = Abank%material(ilayer:numberoflayers)%bottomindex + 1
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = ilayer+1; counter <= numberoflayers-1; counter++)
										{//Abank%material(ilayer+1:numberoflayers)%topindex = Abank%material(ilayer+1:numberoflayers)%topindex + 1
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
								}
				
							// Set top index
							if (Abank->direction == 1) {topindex = point + Abank->direction+1;}			// Right bank				//WARNING1
							else if (Abank->direction == -1) {topindex = point + Abank->direction;}		// Left bank				//WARNING2

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////DM-FEB2011//////////////////////////////////////////////////////////////////
		/////This part is commented out. If in the code, the overhanging block in the partially submerged layer has horizontal bottom side//////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							AddAnotherBankNode(Abank, topintersect, toe, top, ilayer, numberoflayers, topindex, data->MonitorNode, inode);//MMMMTEMP BottomCantileverBlock
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////DM-FEB2011//////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						}
/*
if(inode == data->MonitorNode && Abank->direction == -1)//MMMMTEMP 
{
	if(Abank->numberoflayers > 1)//MMMMTEMP
	{//MMMMTEMP 
		printf("TopElev(BottomLayer) = %lf\n",Abank->material[1].elevation);//MMMMTEMP 
		printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",Abank->material[0].topindex,Abank->material[0].bottomindex);//MMMMTEMP 
		printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",Abank->material[1].topindex,Abank->material[1].bottomindex);//MMMMTEMP 
	}//MMMMTEMP 
	for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("IN UpdateBankNodes (AFTER ADDITION) %d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP
	printf("-----------------------------------------------------------------\n");
}
*/

				}
			else
				{//if(inode == data->MonitorNode && Abank->direction == -1)printf("FULLY SUBMERGED\n");//MMMMTEMP 
					//Layer is fully submerged. Check if point needs to be added.
				if (ilayer == 0) 
					{// Submerged top layer					// Not considered here since no floodplain is considered																																																																	
					}

				else if (Abank->material[ilayer].topindex == Abank->material[ilayer-1].bottomindex) 
					{// Top point of layer must be added to bank profile
						
						AddNodeElevation(Abank->nodes, Abank->numberofnodes, Abank->nodes[Abank->material[ilayer].topindex]);
						//Set this variable in AddNodeElevation, rather than here.  Abank->numberofnodes = Abank->numberofnodes + 1;

						// Reset toe of leftbank and top of rightbank.  Further reset intersectpoint at right bank.
						if (Abank->direction == -1) {toe = toe + 1;}
						if (Abank->direction == 1) 
							{
								top = top + 1;
								intersectpoint = intersectpoint + 1;
							}
						// Reset indices of soil layers
						if (Abank->direction == 1)					// right bank
							{
								//if (ilayer-1>=0)
								for (int counter = 0; counter <= ilayer-1; counter++)
									{//Abank->material(1:ilayer-1)%bottomindex = Abank%material(1:ilayer-1)%bottomindex + 1
										Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
									}
								for (int counter = 0; counter <= ilayer-1; counter++)
									{//Abank%material(1:ilayer-1)%topindex = Abank%material(1:ilayer-1)%topindex + 1
										Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
									}
							}			
						else if (Abank->direction == -1)			// left bank			
							{
								for (int counter = ilayer; counter <= numberoflayers-1; counter++)
									{//Abank%material(ilayer:numberoflayers)%bottomindex = Abank%material(ilayer:numberoflayers)%bottomindex + 1
										Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
									}
								for (int counter = ilayer; counter <= numberoflayers-1; counter++)
									{//Abank%material(ilayer:numberoflayers)%topindex = Abank%material(ilayer:numberoflayers)%topindex + 1
										Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
									}
							}
						
						// Adjust coordinates of top of layer
						Abank->nodes[Abank->material[ilayer].topindex].station = Abank->nodes[Abank->material[ilayer].topindex].station + Abank->direction * erosiondistance;
						
						//! Set top index
						topindex = Abank->material[ilayer].topindex;
					}
				else
					{// Adjust coordinates of top of layer
						
						Abank->nodes[Abank->material[ilayer].topindex].station = Abank->nodes[Abank->material[ilayer].topindex].station + Abank->direction * erosiondistance;
						
						// Set top index
						topindex = Abank->material[ilayer].topindex;
					}
				}

		//******************************************************************************************************************************


			//! Update bottom of layer
			if (Abank->direction*Abank->material[ilayer].bottomindex <= Abank->direction*toe)
				{//Bottom of layer coincides with bank toe	
					// Adjust coordinates of bank toe
					Abank->nodes[toe].station = Abank->nodes[toe].station + Abank->direction * erosiondistance;
					
					// Set bottom index
					bottomindex = toe;
				}
			else
				{// Check if point needs to be added
					if (Abank->material[ilayer].bottomindex == Abank->material[ilayer+1].topindex) 
						{// Bottom point of layer must be added to bank profile
							AddNodeElevation(Abank->nodes,Abank->numberofnodes, Abank->nodes[Abank->material[ilayer].bottomindex]);
							//Set this variable in AddNodeElevation, rather than here.  Abank->numberofnodes = Abank->numberofnodes + 1;

							// Reset toe of leftbank and top of rightbank
							if (Abank->direction == -1) {toe = toe + 1;}
							if (Abank->direction == 1) 
								{
									top = top + 1;
									intersectpoint = intersectpoint + 1;
									topindex = topindex+1;
								}					
							// Reset indices of soil layers
							if (Abank->direction == 1)					// right bank
								{
									//if (ilayer-1>=0)
									for (int counter = 0; counter <= ilayer; counter++)
										{//Abank%material(1:ilayer)%bottomindex = Abank%material(1:ilayer)%bottomindex + 1
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = 0; counter <= ilayer; counter++)
										{//Abank%material(1:ilayer)%topindex = Abank%material(1:ilayer)%topindex + 1
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
								}			
							else if (Abank->direction == -1)			// left bank			
								{
									for (int counter = ilayer+1; counter <= numberoflayers-1; counter++)
										{//Abank%material(ilayer+1:numberoflayers)%bottomindex = Abank%material(ilayer+1:numberoflayers)%bottomindex + 1
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
										}
									for (int counter = ilayer+1; counter <= numberoflayers-1; counter++)
										{//Abank%material(ilayer+1:numberoflayers)%topindex = Abank%material(ilayer+1:numberoflayers)%topindex + 1
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
								}			
					
							// Adjust coordinates of bottom of layer
							Abank->nodes[Abank->material[ilayer].bottomindex].station = Abank->nodes[Abank->material[ilayer].bottomindex].station + Abank->direction * erosiondistance;
							
						}

					else
						{// Adjust coordinates of bottom of layer
							Abank->nodes[Abank->material[ilayer].bottomindex].station = Abank->nodes[Abank->material[ilayer].bottomindex].station + Abank->direction * erosiondistance;
							
						}

					// Set bottom index
					bottomindex = Abank->material[ilayer].bottomindex;
				}

			// Adjust coordinates of intermediate nodes
			
			if(Abank->direction == -1)
				{
					for (int counter = 0; counter <= -topindex+bottomindex-2; counter++)
						{		
							Abank->nodes[bottomindex-1-counter].station = Abank->nodes[bottomindex-1-counter].station + Abank->direction * erosiondistance;
						}
				}
			else if(Abank->direction == 1)
				{
					for (int counter = 0; counter <= topindex-bottomindex-2; counter++)		
						{
							Abank->nodes[bottomindex+1+counter].station = Abank->nodes[bottomindex+1+counter].station + Abank->direction * erosiondistance;
						}
				}

		}//MMMMTEMP DRY

/*
if(inode == data->MonitorNode && Abank->direction == -1)//MMMMTEMP 
{
if(Abank->numberoflayers > 1)//MMMMTEMP
{//MMMMTEMP 
printf("TopElev(BottomLayer) = %lf\n",Abank->material[1].elevation);//MMMMTEMP 
printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",Abank->material[0].topindex,Abank->material[0].bottomindex);//MMMMTEMP 
printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",Abank->material[1].topindex,Abank->material[1].bottomindex);//MMMMTEMP 
}//MMMMTEMP 
for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("IN UpdateBankNodes (AFTER UPDATE NODES) %d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP
printf("-----------------------------------------------------------------\n");
}
*/

}


void RMBankErosion::AddNodeElevation(vector<Coordinates>& nodes, int& numberofnodes, Coordinates newnode)
{
	int direction, first, last;

	// Determine how nodes are sorted in the array
	if (nodes[0].elevation > nodes[numberofnodes-1].elevation) 
		{// Elevation is decreasing
			first = numberofnodes-1; 
			last = 0; 
			direction = -1;	
		}
	else
		{// Elevation is increasing
			first = 0;
			last = numberofnodes-1; 
			direction = 1;
		}

    //TODO: check if this is ok to remove.
	//// Check if elevation of new node is within the range of nodes
    //if(newnode.elevation > nodes[last].elevation || newnode.elevation < nodes[first].elevation)
	//	{
	//		logmsg("Elevation of node to be inserted is outside range of nodes array!!!! \n");
	//		return;
	//	}        

	// Determine index in nodes array at which to insert new node
	int point = SetMargin(nodes,newnode.elevation,last,first,-direction);

	//Add + 1 for right bank
	if(direction == 1)point = point+1;		//DM-FEB2011

    // NO: Replaced existing code with this vector line.
    nodes.insert(nodes.begin() + point, newnode);

    numberofnodes += 1;
}
    
void RMBankErosion::RemoveNode(vector<Coordinates>& nodes, int& numberofnodes, int index)				// Numberofnodes is not present in the original CONCEPTS function
{
    nodes.erase(nodes.begin() + index);
	numberofnodes = numberofnodes-1;
}

double RMBankErosion::Slope(const Coordinates& node1, const Coordinates& node2)
{
	double denominator, rsign, slope;

	denominator = node2.station-node1.station;																	// Function Ydistance is called in CONCEPTS  
	if (denominator >= 0) {rsign = fabs(1.0);}  		
	else {rsign = -fabs(1.0);} 
	slope = (node2.elevation-node1.elevation) /( rsign*max(fabs(denominator),0.000001));							// Function Zdistance is called in CONCEPTS
	return slope;
}

void RMBankErosion::AddLayerIntersects(Bank *Abank)
{
	int ilayer, layer, numberoflayers, point, toe, top;
	Coordinates intersect;

	// Set bank indices
	if (Abank->direction == 1)         // right bank
		{
			toe = 0; 
			top = Abank->numberofnodes-1;									
		}
	else if (Abank->direction == -1)   // left bank
		{
			toe = Abank->numberofnodes-1;									
			top = 0;
		}
	numberoflayers = Abank->numberoflayers;						
	 
	// Set top layer
	Abank->material[0].topindex = top;
    layer = 1;
    if(numberoflayers > 1) 
		{
			while(Abank->material[layer].elevation > Abank->nodes[top].elevation)				// Control not use for now, since the elevation of the top is not changed
				{
					Abank->material[layer].topindex = top;
					Abank->material[layer-1].bottomindex = top;
					layer = layer + 1;
					if(layer > numberoflayers-1) {break;}
				}
		}
    ilayer = layer;

	// Add intersects and set indices of remaining soil layers
    for(layer=ilayer; layer<=numberoflayers-1; layer++)       //FORISSUE
		{
			// Check whether layer elevation is above the toe of the bank					// I am not updating the elevation of the toe for now, so this control is not used
			if (Abank->material[layer].elevation < Abank->nodes[toe].elevation)
				{
					Abank->material[layer].topindex = toe - Abank->direction;
					Abank->material[layer-1].bottomindex = toe - Abank->direction;
					//goto there;
				}
			else//
				{//
					// Get intersect of layer and bank profile
					point = SetMargin(Abank->nodes,Abank->material[layer].elevation,top,toe,-Abank->direction);
					if(point < 0) 
						{
							logmsg("Bank layer intersect error");
							return;
						}

					intersect.station = Abank->nodes[point].station + InverseSlope(Abank->nodes[point],Abank->nodes[point+Abank->direction]) * (Abank->material[layer].elevation-Abank->nodes[point].elevation);									// this is done in the function NodeAtZposition in CONCEPTS
					intersect.elevation = Abank->material[layer].elevation;
		    
					// Check if intersect coincides with points in the bank profile
					if (fabs(Abank->nodes[point+Abank->direction].elevation-intersect.elevation)  < 0.001)
						{// intersect coincides with point in bank profile
							Abank->material[layer].topindex = point + Abank->direction;
							Abank->material[layer-1].bottomindex = point + Abank->direction;
							if ((point + Abank->direction) != top) 
							if (fabs(Abank->nodes[point+2*Abank->direction].elevation-intersect.elevation) < 0.001) 
								{Abank->material[layer-1].bottomindex = point + 2 * Abank->direction;}
							//goto there;
						}
					else if (fabs(Abank->nodes[point].elevation-intersect.elevation) < 0.01) 
						{// intersect coincides with point in bank profile
							Abank->material[layer].topindex = point;
							Abank->material[layer-1].bottomindex = point;
							//goto there;
						}
					else
						{
							// Add intersect
							AddNodeElevation(Abank->nodes, Abank->numberofnodes, intersect);
					
							// Set and reset indices
							if (Abank->direction == 1)        // right bank
								{
									top = top + 1;
									for(int counter=0; counter<=layer-1;counter++)
										{
											Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
											Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
										}
									Abank->material[layer-1].bottomindex = point + 1;
									Abank->material[layer].topindex = point + 1;
								}
							else if (Abank->direction == -1)   //left bank
								{
									toe = toe + 1;
									Abank->material[layer-1].bottomindex = point;
									Abank->material[layer].topindex = point;
								}
						}
				}//
			there:;
		}//End for cycle
	
	// Set bottom index of last layer
	if(Abank->direction == -1)
	{Abank->material[numberoflayers-1].bottomindex = toe;}		// I put an if and got rid of - Abank->direction
}

Coordinates RMBankErosion::IntersectXSandStage(Bank *Abank, double stage)
{
	Coordinates Intersect;
	int toe, top, point;
	
	if(Abank->direction == -1)
		{
			top = 0;																															
			toe = Abank->numberofnodes -1;
		}
	if(Abank->direction == 1)
		{
			top = Abank->numberofnodes -1;																															
			toe = 0;
		}

    point = SetMargin(Abank->nodes,stage,top,toe,-Abank->direction);
	Intersect.station = Abank->nodes[point].station + InverseSlope(Abank->nodes[point],Abank->nodes[point+Abank->direction]) * (stage-Abank->nodes[point].elevation);
	Intersect.elevation = stage;

	return Intersect; 
}

void RMBankErosion::ComputeFrictionSlope(RMBankErosionData* data, RMHydrodynamicsData* hdata, RMMigrationData* migdata, RMPrepData* prepdata)//DM-FEB2011
{
	int Mid = migdata->channel.nnodes/2;
	double TotalHead1,TotalHead2;

	//Upstream centerline node
	TotalHead1 = hdata->HH(0,Mid) + pow(hdata->UU(0,Mid),2.0)/(2.0 * 9.80665);
	TotalHead2 = hdata->HH(1,Mid) + pow(hdata->UU(1,Mid),2.0)/(2.0 * 9.80665);
    data->flowworkspace[0].frictionslope = (TotalHead2 - TotalHead1)/prepdata->intervals(1)/(migdata->channel.WIDTH/2);

	//All nodes except upstream and downstream centerline nodes
	for (int i = 1; i <= migdata->channel.centerline.length-2; i++)
		{
			TotalHead1 = hdata->HH(i-1,Mid) + pow(hdata->UU(i-1,Mid),2.0)/(2.0 * 9.80665);
			TotalHead2 = hdata->HH(i+1,Mid) + pow(hdata->UU(i+1,Mid),2.0)/(2.0 * 9.80665);
			data->flowworkspace[i].frictionslope = (TotalHead2 - TotalHead1)/(prepdata->intervals(i+1) + prepdata->intervals(i))/(migdata->channel.WIDTH/2);
		}

	//Downstream centerline node
	TotalHead1 = hdata->HH(migdata->channel.centerline.length-2,Mid) + pow(hdata->UU(migdata->channel.centerline.length-2,Mid),2.0)/(2.0 * 9.80665);
	TotalHead2 = hdata->HH(migdata->channel.centerline.length-1,Mid) + pow(hdata->UU(migdata->channel.centerline.length-1,Mid),2.0)/(2.0 * 9.80665);
    data->flowworkspace[migdata->channel.centerline.length-1].frictionslope = (TotalHead2 - TotalHead1)/prepdata->intervals(migdata->channel.centerline.length-1)/(migdata->channel.WIDTH/2);  
}

void RMBankErosion::SectionGeometryAdjustment(RMBankErosionData* data, RMHydrodynamicsData* hdata, RMMigrationData* migdata, RMPrepData* prepdata)//DM-FEB2011
{
	int toe;
	int firstnode = 0;
	Coordinates node1, node2;
	double z;
	Coordinates Anode;					//DM-FEB2011
	Bank Abank;							//DM-FEB2011
	double HorizontalGap;				//DM-FEB2011

	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{ 
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
				{   
					// Left bank
					toe = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;
					if(data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe].elevation < hdata->EE(inode+firstnode,0))
						{	// Cut
							/*			//DM-FEB2011 Commented out
							node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe];
							node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe-1];
							z = hdata->EE(inode+firstnode,0);
							data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe] = NodeAtZposition(node1, node2, z)
							*/

							//DM-FEB2011
							// Cut

							//Bed elevation at bank									
							z = hdata->EE(inode+firstnode,0);

							//Initialize counter									
							int i = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1;

							//Look for location where the bed intersects the bank   
							while(i >=0 && data->grids[igrid].nodes[inode].xsection.leftbank.nodes[i].elevation < z) //NOO: swapped condition because i should be checked first.
								{
									i = i-1;
								}

							//Add node at intersection bed/bank and update top and bottom indices			
							if(i!=-1)
								{
									Abank = data->grids[igrid].nodes[inode].xsection.leftbank;

									//Node at intersection bed/bank				
									node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[i+1];
									node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[i];
									Anode = NodeAtZposition(node1, node2, z);

									//Update the number of bank nodes				
									data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(i+2);

									//Re-order bank nodes							
									for(int j = 0; j <=i; j++)
										{
											data->grids[igrid].nodes[inode].xsection.leftbank.nodes[j] = Abank.nodes[j];
										}

									//Add node (new toe)						
									data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1] = Anode;

									//Update top and bottom indices				
									//Case of just one material layer    
									if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers == 1)	
										{
											data->grids[igrid].nodes[inode].xsection.leftbank.material[0].topindex = 0;																	//DM-FEB2011						
											data->grids[igrid].nodes[inode].xsection.leftbank.material[0].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;	//DM-FEB2011		
										}
									//Case of more than one material layer    
									else if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers > 1)  
										{	
											//Case when the number of layers needs to be updated  
											if(z > data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers - 1].elevation)
												{
													//Compute number of layers	
													int counter = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers - 1;
													while(counter >= 0 && z > data->grids[igrid].nodes[inode].xsection.leftbank.material[counter].elevation) 
														{
															counter = counter - 1;
														}
													data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers = counter+1;

													//Case when the current number of layers is 1  
													if(data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers == 1)
														{
															//Update top and bottom index of the only layer present 
															data->grids[igrid].nodes[inode].xsection.leftbank.material[0].topindex = 0;	
															data->grids[igrid].nodes[inode].xsection.leftbank.material[0].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;	
														}
													//Case when the current number of layers is greater than 1  
													else if(data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers > 1)
														{	
															//Update bottom index (just the lowest layer needs to be modified) 
															data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers-1].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;
														}
												}	
											//Case when the number of layers does not need to be updated
											else																																							
												{		
													if(data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers > 1)
														{
															//Update bottom index (just the lowest layer needs to be modified) 
															data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers-1].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;	
														}
													else if(data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers == 1)
														{
															//Update top and bottom index of the only layer present 
															data->grids[igrid].nodes[inode].xsection.leftbank.material[0].topindex = 0;				
															data->grids[igrid].nodes[inode].xsection.leftbank.material[0].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1;
														}
												}																																							
										}
								}
							//DM-FEB2011
		
							//DM-FEB2011
							//Eliminate gap between cut bank and bed
							HorizontalGap = ((data->StationCenterline[inode]-prepdata->coordn(0)*migdata->channel.WIDTH/2)) - data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1].station; //This should be a positive quantity 
							for(int i = 0; i <= data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1; i++)
								{
									data->grids[igrid].nodes[inode].xsection.leftbank.nodes[i].station = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[i].station + HorizontalGap;
								}
							//DM-FEB2011
						}
					else if(data->ElongationGap > 0 && data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe].elevation > hdata->EE(inode+firstnode,0)+data->ElongationGap)//DM-FEB2011
						{	
							//DM-FEB2011
							// Elongate
							
							//Increase by one the number of bank nodes		
							data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes + 1);

							//Add node (new toe)							
							toe = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;
							data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe].elevation = hdata->EE(inode+firstnode,0);
							data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe].station = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe-1].station;
						
							//Update top and bottom indices					
							//Case of just one material layer      
							if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers == 1)	
								{
									data->grids[igrid].nodes[inode].xsection.leftbank.material[0].topindex = 0;																			//DM-FEB2011										
									data->grids[igrid].nodes[inode].xsection.leftbank.material[0].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;			//DM-FEB2011			
								}
							//Case of more than one material layer     
							else if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers > 1)	
								{																			
									//Case when the number of layers needs to be updated 
									if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers > data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers  && hdata->EE(inode+firstnode,0) < data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers].elevation)
										{
											//Compute number of layers
											int counter = data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers - 1;
											while(counter >= 0 && hdata->EE(inode+firstnode,0) > data->grids[igrid].nodes[inode].xsection.leftbank.material[counter].elevation) 
												{
													counter = counter - 1;
												}

											int oldnumberoflayers = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers;
											data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers = counter+1;

											//Compute number of intersections 	
											int numberofintersections = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers - oldnumberoflayers;

											//Add nodes 
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1];							
													node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].elevation);
													data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes + 1);
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1] = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2] = Anode;	
													node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1];							
													node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].elevation);
													data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes + 1);
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1] = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2] = Anode;	
												}

											//Update top and bottom indices 
											data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers - 1].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers - 1].bottomindex + 1;		
								
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].topindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers -1 + m].bottomindex + 1;
													data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].topindex + 1;
												}
										
										}	
									//Case when the number of layers does not need to be updated
									else																																							
										{		
											//Update bottom index of the lowest layer 
											data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers-1].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers-1].bottomindex + 1;
										}	
								}
						}
																																									
					else if(data->ElongationGap <= 0)																															//DM-FEB2011
						{   
							// Elongate																																			//DM-FEB2011
							data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toe].elevation = hdata->EE(inode+firstnode,0);												//DM-FEB2011

							//Update top and bottom indices					
							//Case of just one material layer      
							if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers == 1)	
								{
									data->grids[igrid].nodes[inode].xsection.leftbank.material[0].topindex = 0;																			//DM-FEB2011										
									data->grids[igrid].nodes[inode].xsection.leftbank.material[0].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes-1;			//DM-FEB2011			
								}
							//Case of more than one material layer     
							else if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers > 1)	
								{																			
									//Case when the number of layers needs to be updated 
									if(data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers > data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers  && hdata->EE(inode+firstnode,0) < data->grids[igrid].nodes[inode].xsection.leftbank.material[data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers].elevation)
										{
											//Compute number of layers
											int counter = data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers - 1;
											while(counter >= 0 && hdata->EE(inode+firstnode,0) > data->grids[igrid].nodes[inode].xsection.leftbank.material[counter].elevation) 
												{
													counter = counter - 1;
												}

											int oldnumberoflayers = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers;
											data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers = counter+1;

											//Compute number of intersections 	
											int numberofintersections = data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers - oldnumberoflayers;

											//Add nodes 
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1];							
													node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].elevation);
													data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes + 1);
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1] = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2] = Anode;	
													node1 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1];							
													node2 = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].elevation);
													data->grids[igrid].nodes[inode].xsection.leftbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes + 1);
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1] = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2];
													data->grids[igrid].nodes[inode].xsection.leftbank.nodes[data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 2] = Anode;	
												}

											//Update top and bottom indices 
											//No update of data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers - 1].bottomindex is needed. //Different from the other type of elongation performed for data->ElongationGap > 0		
								
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].topindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers -1 + m].bottomindex + 1;
													data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].bottomindex = data->grids[igrid].nodes[inode].xsection.leftbank.material[oldnumberoflayers + m].topindex + 1;
												}
										
										}	
									//Case when the number of layers does not need to be updated
									else																																							
										{		
											//No update is necessary. //Different from the other type of elongation performed for data->ElongationGap > 0
										}	
								}
						}																																						//DM-FEB2011
				
					// Right bank
					toe = 0;
					if(data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe].elevation < hdata->EE(inode+firstnode,migdata->channel.nnodes-1))
						{	// Cut
							/*		//DM-FEB2011 Commented out
							node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe];
							node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe+1];
							z = hdata->EE(inode+firstnode,migdata->channel.nnodes-1);
							data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe] = NodeAtZposition(node1, node2, z);
							*/

							//DM-FEB2011
							// Cut

							//Bed elevation at bank							
							z = hdata->EE(inode+firstnode,migdata->channel.nnodes-1);

							//Initialize counter 
							int i = 0;

							//Look for location where the bed intersects the bank   
							while(i <=data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1 && data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i].elevation < z) //NOO swapped condition
								{
									i = i+1;
								}

							//Add node at intersection bed/bank and update top and bottom indices			
							if(i!=data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes)
								{	
									Abank = data->grids[igrid].nodes[inode].xsection.rightbank;

									//Node at intersection bed/bank				
									node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
									node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i];
									Anode = NodeAtZposition(node1, node2, z);

									//Update the number of bank nodes				
									data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-i+1);

									//Re-order bank nodes							
									for(int j = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1; j >=1; j--)
										{
											data->grids[igrid].nodes[inode].xsection.rightbank.nodes[j] = Abank.nodes[j+i-1];
										}

									//Add node (new toe)							
									data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0] = Anode;

									//Update top and bottom indices					
									//Case of just one material layer     
									if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers == 1)	
										{
											data->grids[igrid].nodes[inode].xsection.rightbank.material[0].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 1;	//DM-FEB2011		
											data->grids[igrid].nodes[inode].xsection.rightbank.material[0].bottomindex = 0;																	//DM-FEB2011		
										}
									//Case of more than one material layer    
									if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers > 1)	
										{
											//Case when the number of layers needs to be updated  
											if(z > data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].elevation)
												{
													//Compute number of layers	
													int counter = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1;
													while(counter >= 0 && z > data->grids[igrid].nodes[inode].xsection.rightbank.material[counter].elevation) 
														{
															counter = counter - 1;
														}
													data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers = counter+1; 
			
													//Update top and bottom indices	 
													//Case when the current number of layers is 1  
													if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers == 1)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.material[0].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1;	
															data->grids[igrid].nodes[inode].xsection.rightbank.material[0].bottomindex = 0; 	
														}
													//Case when the current number of layers is greater than 1  
													else if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers > 1)
														{
															//Top and bottom indices of all layers except the lowest one
															for(int ilayer = 0; ilayer <= data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 2; ilayer++)
																{
																	data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex - i + 1;																								
																	data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex - i + 1;
																}
															//Top and bottom indices of the lowest layer
															data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex - i + 1;
															data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].bottomindex = 0;
														}
												}	
											//Case when the number of layers does not need to be updated
											else																																							
												{		
													if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers > 1)
														{
															//Top and bottom indices of all layers except the lowest one 
															for(int ilayer = 0; ilayer <= data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 2; ilayer++)
																{
																	data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex - i + 1;																								
																	data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex - i + 1;
																}
															//Top and bottom indices of the lowest layer																						
															data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex - i + 1;
															data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].bottomindex = 0;	
														}
													else if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers == 1)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.material[0].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1;																								
															data->grids[igrid].nodes[inode].xsection.rightbank.material[0].bottomindex = 0;		
														}
												}																																							
										}
								}
							//DM-FEB2011

							//DM-FEB2011
							//Eliminate gap between cut bank and bed
							HorizontalGap = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0].station - (data->StationCenterline[inode]-prepdata->coordn(migdata->channel.nnodes-1)*migdata->channel.WIDTH/2); //This should be a positive quantity
							for(int i = 0; i <= data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 1; i++)
								{
									data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i].station = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i].station - HorizontalGap;
								}
							//DM-FEB2011
						}
					else if (data->ElongationGap > 0 && data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe].elevation > hdata->EE(inode+firstnode,migdata->channel.nnodes-1)+data->ElongationGap)  //DM-FEB2011
						{	
							//DM-FEB2011
							// Elongate

							//Increase by one the number of bank nodes		
							data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes + 1); //NOO
							
							//Re-order bank nodes							
							for(int i = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes -1; i>=1; i--)
								{
									data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i] = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
								}

							//Add node (new toe)							
							data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0].station = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1].station; 	
							data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0].elevation = hdata->EE(inode+firstnode,migdata->channel.nnodes-1);

							//Update top and bottom indices					
							//Case of just one material layer      
							if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers == 1)	
								{
									data->grids[igrid].nodes[inode].xsection.rightbank.material[0].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1;							
									data->grids[igrid].nodes[inode].xsection.rightbank.material[0].bottomindex = 0;			
								}
							//Case of more than one material layer     
							else if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers > 1)	
								{																				
									//Case when the number of layers needs to be updated  
									if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers > data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers && hdata->EE(inode+firstnode,migdata->channel.nnodes-1) < data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers].elevation)
										{
											//Compute number of layers	
											int counter = data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers - 1;
											while(counter >= 0 && hdata->EE(inode+firstnode,migdata->channel.nnodes-1) > data->grids[igrid].nodes[inode].xsection.rightbank.material[counter].elevation) 
												{
													counter = counter - 1;
												}

											int oldnumberoflayers = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers;
											data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers = counter+1;

											//Compute number of intersections	
											int numberofintersections = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - oldnumberoflayers;

											//Add nodes 
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0];							
													node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.rightbank.material[oldnumberoflayers + m].elevation);	
													data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes + 1);
													for(int i = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes -1; i>=2; i--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i] = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
														}
													data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1] = Anode;
													node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0];							
													node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.rightbank.material[oldnumberoflayers + m].elevation);	
													data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes + 1);
													for(int i = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes -1; i>=2; i--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i] = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
														}
													data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1] = Anode;
												}

											//Update top and bottom indices 
											for(int m = 0; m <= numberofintersections-1; m++)
												{
													data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - m].topindex = 2*m+1;																							
													data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - m].bottomindex = 2*m;			
												}
																						
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - numberofintersections].topindex + 1;
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].topindex + 1 + 2*numberofintersections;	
											
											if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections > 0)
												{
													for(int ilayer = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 2 - numberofintersections; ilayer >= 0; ilayer--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex + 1 + 2*numberofintersections;																								
															data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex + 1 + 2*numberofintersections;
														}
												}
										}	
									//Case when the number of layers does not need to be updated
									else																																							
										{		
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex + 1;
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].bottomindex = 0;

											for(int ilayer = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 2; ilayer >= 0; ilayer--)
												{
													data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex + 1;																								
													data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex + 1;
												}
										}	
								}
						}
					else if (data->ElongationGap <= 0)																													//DM-FEB2011
						{	
							// Elongate																																	//DM-FEB2011
							data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toe].elevation = hdata->EE(inode+firstnode,migdata->channel.nnodes-1);				//DM-FEB2011

							//Update top and bottom indices					
							//Case of just one material layer      
							if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers == 1)	
								{
									data->grids[igrid].nodes[inode].xsection.rightbank.material[0].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes-1;							
									data->grids[igrid].nodes[inode].xsection.rightbank.material[0].bottomindex = 0;			
								}
							//Case of more than one material layer     
							else if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers > 1)	
								{																				
									//Case when the number of layers needs to be updated  
									if(data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers > data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers && hdata->EE(inode+firstnode,migdata->channel.nnodes-1) < data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers].elevation)
										{
											//Compute number of layers	
											int counter = data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers - 1;
											while(counter >= 0 && hdata->EE(inode+firstnode,migdata->channel.nnodes-1) > data->grids[igrid].nodes[inode].xsection.rightbank.material[counter].elevation) 
												{
													counter = counter - 1;
												}

											int oldnumberoflayers = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers;
											data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers = counter+1; 

											//Compute number of intersections	
											int numberofintersections = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - oldnumberoflayers;

											//Add nodes 
											for(int m = 0; m <= numberofintersections - 1; m++)
												{
													node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0];							
													node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.rightbank.material[oldnumberoflayers + m].elevation);	
													data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes + 1);
													for(int i = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes -1; i>=2; i--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i] = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
														}
													data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1] = Anode;
													node1 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[0];							
													node2 = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1];							
													Anode = NodeAtZposition(node1, node2, data->grids[igrid].nodes[inode].xsection.rightbank.material[oldnumberoflayers + m].elevation);	
													data->grids[igrid].nodes[inode].xsection.rightbank.SetNumberOfNodes(data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes + 1);
													for(int i = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes -1; i>=2; i--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i] = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[i-1];
														}
													data->grids[igrid].nodes[inode].xsection.rightbank.nodes[1] = Anode;
												}

											//Update top and bottom indices 
											for(int m = 0; m <= numberofintersections-1; m++)
												{
													data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - m].topindex = 2*m+1;																							
													data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - m].bottomindex = 2*m;			
												}
																						
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - numberofintersections].topindex + 1;
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections].topindex + 2*numberofintersections;//Different from the other type of elongation performed for data->ElongationGap > 0	
											
											if(data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1 - numberofintersections > 0)
												{
													for(int ilayer = data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 2 - numberofintersections; ilayer >= 0; ilayer--)
														{
															data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex + 2*numberofintersections;	//Different from the other type of elongation performed for data->ElongationGap > 0																							
															data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex = data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex + 2*numberofintersections;//Different from the other type of elongation performed for data->ElongationGap > 0
														}
												}
										}	
									//Case when the number of layers does not need to be updated
									else																																							
										{		
											//No update of data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].topindex is needed. //Different from the other type of elongation performed for data->ElongationGap > 0	
											data->grids[igrid].nodes[inode].xsection.rightbank.material[data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1].bottomindex = 0;

											//No update of the indices of the overlying layers is needed. //Different from the other type of elongation performed for data->ElongationGap > 0
										}

									

								}

						}	//DM-FEB2011

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////						
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*
						if(inode == data->MonitorNode)
							{
			
								printf("LEFT \n");
								printf("Initial number of layers = %d\n", data->grids[igrid].nodes[inode].xsection.leftbank.initialnumberoflayers);
								printf("Current number of layers = %d\n", data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers);
								for(int k = 0; k<=data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1; k++) printf("S	%lf	E	%lf \n",data->grids[igrid].nodes[inode].xsection.leftbank.nodes[k].station,data->grids[igrid].nodes[inode].xsection.leftbank.nodes[k].elevation);
								for(int ilayer = 0; ilayer <= data->grids[igrid].nodes[inode].xsection.leftbank.numberoflayers - 1; ilayer++)
									{
										printf("Layer	%d	TI	%d	BI	%d \n",ilayer,data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].topindex, data->grids[igrid].nodes[inode].xsection.leftbank.material[ilayer].bottomindex);
									}
	
								printf("RIGHT \n");
								printf("Initial number of layers = %d\n", data->grids[igrid].nodes[inode].xsection.rightbank.initialnumberoflayers);
								printf("Current number of layers = %d\n", data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers);
								for(int k = 0; k<=data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes - 1; k++) printf("S	%lf	E	%lf \n",data->grids[igrid].nodes[inode].xsection.rightbank.nodes[k].station,data->grids[igrid].nodes[inode].xsection.rightbank.nodes[k].elevation);
								for(int ilayer = 0; ilayer <= data->grids[igrid].nodes[inode].xsection.rightbank.numberoflayers - 1; ilayer++)
									{
										printf("Layer	%d	TI	%d	BI	%d \n",ilayer,data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].topindex, data->grids[igrid].nodes[inode].xsection.rightbank.material[ilayer].bottomindex);
									}
							}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////						
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					


				}
			firstnode = firstnode+data->grids[igrid].numberofnodes;
		}

IntersectCrossSectionAndStageAndToes(data, hdata, migdata);//DM-FEB2011  Moved here
}


void RMBankErosion::UpdateWidthAndDimensionlessCenterline(RMBankErosionData* data, RMMigrationData* migdata)
{
	double newwidth, oldwidth, localwidth;
	int firstnode, toeleft, toeright;
	
	oldwidth = migdata->channel.WIDTH;
	firstnode = 0;

	// Update width: method 1
	if(data->MethodUpdateWidth == 1)
		{
			newwidth = 0;
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{   
							newwidth = newwidth + (data->intersectright[inode+firstnode].station - data->intersectleft[inode+firstnode].station);
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = newwidth/migdata->channel.centerline.length;
		}

	// Update width: method 2	
	else if(data->MethodUpdateWidth == 2)
		{
			newwidth = 1.7E+308;		// Max value in C++, we are looking for a minimum 
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{   
							localwidth = data->intersectright[inode+firstnode].station - data->intersectleft[inode+firstnode].station;
							if( localwidth < newwidth)
								{
									newwidth = localwidth;
								}						
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = newwidth;
		}
	
	// Update width: method 3
	else if(data->MethodUpdateWidth == 3)
		{
			newwidth = 0;
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{
							newwidth = newwidth + (data->toeright[inode+firstnode].station - data->toeleft[inode+firstnode].station);
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = newwidth/migdata->channel.centerline.length;
		}
	
	// Update width: method 4
	else if(data->MethodUpdateWidth == 4)
		{	
			newwidth = 1.7E+308;		// Max value in C++, we are looking for a minimum
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{
							localwidth = data->toeright[inode+firstnode].station - data->toeleft[inode+firstnode].station;
							//logmsg("SUB %d, %lf, %lf, %lf \n", inode, data->toeright[inode+firstnode].station, data->toeleft[inode+firstnode].station, localwidth);
							if( localwidth < newwidth)
								{
									newwidth = localwidth;
								}
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = newwidth;
		}

	// Update dimensionless centerline
	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)
		{
			migdata->channel.centerline(i,0) = migdata->channel.centerline(i,0)*oldwidth/migdata->channel.WIDTH;
			migdata->channel.centerline(i,1) = migdata->channel.centerline(i,1)*oldwidth/migdata->channel.WIDTH;
		}
}

void RMBankErosion::IntersectCrossSectionAndStageAndToes(RMBankErosionData* data, RMHydrodynamicsData* hdata, RMMigrationData* migdata)
{		
	for (int i = 0; i <= migdata->channel.centerline.length-1; i++)			
		{
			Bank *InitialBank;
			//printf("i (L) = %d\n",i);//MMMMTEMP
			if(i == 338)printf("stage (left) = %lf, top (left) = %lf \n",hdata->HH(i,0),InitialBank->nodes[1].elevation);//MMMMTEMP
			//if(i == 21)printf("(x,y) = %lf	%lf \n",migdata->channel.centerline(i,0)*migdata->channel.WIDTH/2.0,migdata->channel.centerline(i,1)*migdata->channel.WIDTH/2.0);//MMMMTEMP
			// Left bank
			InitialBank = &data->grids[0].nodes[i].xsection.leftbank;
			data->intersectleft[i] = IntersectXSandStage(InitialBank, hdata->HH(i,0));
			data->toeleft[i] = InitialBank->nodes[InitialBank->numberofnodes-1];
			//printf("i (R) = %d\n",i);//MMMMTEMP
			// Right bank
			InitialBank = &data->grids[0].nodes[i].xsection.rightbank;
			data->intersectright[i] = IntersectXSandStage(InitialBank, hdata->HH(i,migdata->channel.nnodes-1));
			data->toeright[i] = InitialBank->nodes[0];
		}
}

void RMBankErosion::UpdateCrossSectionStations(RMBankErosionData* data, RMMigrationData* migdata)
{
	int firstnode;
	int numbernodesleft, numbernodesright; 
	
	firstnode = 0;

	for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
		{  
			for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
				{   
					numbernodesleft = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes;
					numbernodesright = data->grids[igrid].nodes[inode].xsection.rightbank.numberofnodes;
					for(int ivertex = 0; ivertex<=numbernodesleft - 1; ivertex++)
						{
							data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station = data->grids[igrid].nodes[inode].xsection.leftbank.nodes[ivertex].station - data->ErosionBank(inode+firstnode,1)*(migdata->channel.WIDTH/2);
						}
					for(int ivertex = 0; ivertex<=numbernodesright - 1; ivertex++)
						{
							data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[ivertex].station - data->ErosionBank(inode+firstnode,1)*(migdata->channel.WIDTH/2);
						}
				}
			firstnode = firstnode+data->grids[igrid].numberofnodes;
		}

	//
}

void RMBankErosion::PlanarFailure(Bank *Abank, RMBankErosionData* data, RMHydrodynamicsData* hdata, int inode, int firstnode, RMMigrationData* migdata, double& maxundercut)		
{
	int numberofpoints, toe, top, point, toepoint, layer, pointatfailure, numberoflayers, vertex;
	double temp, ratio, elevationdifference;
	double angle, failureangle, fos;
	double failurearea, area;
	double weight[100];
	Coordinates toenode, toeatfailure, bottomintersect, topintersect;
	bool snaptolayer, failuresnaptolayer;
	FailureParameters parameters;
	Coordinates FloodplainNodeLeft, FloodplainNodeRight;	
    vector<Coordinates> nodeworkspace; //NOO
    nodeworkspace.resize(max(Abank->numberofnodes, 2000)); //NOO				//DM-FEB2011 I changed 50 to 2000
	double stage;
	int OtherNodesToAddLeft, OtherNodesToAddRight;						// ADDITION MAR2010
	vector<Coordinates> NodeToAddLeft, NodeToAddRight;				//NOO // ADDITION MAR2010
    NodeToAddLeft.resize(max(Abank->numberofnodes, 2000)); //NOO //DM-FEB2011 I changed 50 to 2000
    NodeToAddRight.resize(max(Abank->numberofnodes, 2000)); //NOO //DM-FEB2011 I changed 50 to 2000

	Abank->failureparameters.factorofsafety = 9999.0;//MMMMTEMP Modification to get minimum factor of safety
	// Initialize toe and top indices.
	numberofpoints = Abank->numberofnodes;																	  																														
	if (Abank->direction == 1)        // right bank
		{
			toe = 0; 
			top = numberofpoints-1;
		}
	else if (Abank->direction == -1)  // left bank
		{
			toe = numberofpoints-1; 
			top = 0;
		}

	//if(inode == 172 && Abank->direction == 1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation); //MMMMTEMP Printing
	//if(inode == 168 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 168 && Abank->direction == 1)printf("OOOOOOO Minimum elevation of bank at node (%d) = %lf\n",inode,Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 168 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 169 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 169 && Abank->direction == 1)printf("OOOOOOO Minimum elevation of bank at node (%d) = %lf\n",inode,Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 169 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 170 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 170 && Abank->direction == 1)printf("OOOOOOO Minimum elevation of bank at node (%d) = %lf\n",inode,Abank->nodes[0].elevation); //MMMMTEMP Printing
	//if(inode == 170 && Abank->direction == 1)printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",Abank->nodes[0].elevation); //MMMMTEMP Printing


	// Sweep over possible intersects of bank and slip surface
	temp = 999999999.0;
	for(point = 0; point <= data->numberofintersects-1; point++)     //FORISSUE
		{ 
			// Set intersect of slip surface and bank surface
			ratio = double(point)/double(data->numberofintersects);
			
			toenode.elevation = Abank->nodes[toe].elevation + ratio * (Abank->nodes[top].elevation-Abank->nodes[toe].elevation);   

			// Check whether this elevation is near the top of a soil layer
			toepoint = SetMargin(Abank->nodes,toenode.elevation,top,toe,-Abank->direction) + Abank->direction;

			if(toepoint < 0)
				{
					logmsg(" Slip surface and bank intersect error, at node = %d \n", inode);
					return;
				}

			toenode = NodeAtZposition(Abank->nodes[toepoint-Abank->direction],Abank->nodes[toepoint],toenode.elevation);

			snaptolayer = false;

			

			for(layer=Abank->numberoflayers-1;layer >= 1;layer--)	//FORISSUE
				{
					elevationdifference = Abank->material[layer].elevation - toenode.elevation;
					
					//elevationdifference < 0. is the case when layer top boundary below toenode elevation (in the original CONCEPTS code, the sign is <=. I think it is wrong because the case when toenode is also a node of the bank is not handled correctly )					
					if (elevationdifference <= data->zthreshold && elevationdifference > 0.)					// Layer top boundary above toenode elevation but close 
						{
							// Snap toe to the top of the bank material layer. Check if it is undercut, receded, or none of the above.
							snaptolayer = true;
							if (Abank->material[layer].topindex == Abank->material[layer-1].bottomindex) 
								{
									// Single node.  Set toe of the bank at the interface of the bank material layers.
									toenode = Abank->nodes[Abank->material[layer].topindex];
									toepoint = Abank->material[layer].topindex + Abank->direction;
								}
							else
								{
									if(Abank->direction*Abank->nodes[Abank->material[layer-1].bottomindex].station < Abank->direction*Abank->nodes[Abank->material[layer].topindex].station) 
										{
											// Undercut, set toenode to the top of the bank material layer with index "layer"
											toenode = Abank->nodes[Abank->material[layer].topindex];
											toepoint = Abank->material[layer-1].bottomindex;
										}
									else
										{
											// Receded, set toenode to the bottom of the bank material layer with index "layer-1"
											toenode = Abank->nodes[Abank->material[layer-1].bottomindex];
											toepoint = Abank->material[layer-1].bottomindex + Abank->direction;
										}

								}
						}// End If
				}    // End For cycle on layers

			// Determine angle of failure plane and parameters of failing mass.
			if(Abank->direction == -1)							// Left bank
				{
					stage = hdata->HH(inode+firstnode,0);
				}
			else if(Abank->direction == 1)						// Right bank
				{
					stage = hdata->HH(inode+firstnode,migdata->channel.nnodes-1);
				}
			
			// Calculation of the minimum factor of safety
			//if(inode == data->MonitorNode)printf(">>>>>>> Call MinimumFactorOfSafety <<<<<<<<\n");//MMMMTEMP Printing
//if(inode == 172 && Abank->direction == 1)printf("toenode %lf	%lf\n",toenode.station,toenode.elevation); //MMMMTEMP Printing				
			MinimumFactorOfSafety(Abank,top,toenode,toepoint,stage,angle,fos,parameters, data, migdata, inode);
			//if(inode == data->MonitorNode)printf(">>>>>>> After minimization fos = %lf, angle = %lf, Abank->failureparameters.factorofsafety = %lf \n",fos,angle,Abank->failureparameters.factorofsafety);//MMMMTEMP Printing
			//if(inode == data->MonitorNode){logmsg("TEMP = %lf, fos = %lf\n", temp, fos);}
			if (fos <= 0.) {fos = 9999999999.;}
			if (fos < temp) 
				{
					temp = fos; 
					failureangle = angle;
					failuresnaptolayer = snaptolayer;				//Logical variable for snapping toe to the top of the bank material layer		
					toeatfailure = toenode;							
					pointatfailure = toepoint;						
					//Abank->failureparameters = parameters;	//MMMMTEMP Commented out   MMMMTEMP Modification to get minimum factor of safety		
				}
		} // End For on bank-planar failure plane intersects

	// Get lowest factor of safety
	fos = temp;
	
	//MMMMTEMP Printing

	if(inode == data->MonitorNode) 
	{
		printf("\nBACK IN PLANAR FAILURE \n");

		printf("fos = %lf \n", fos); 
		printf("failureangle = %lf \n", failureangle);							
		printf("failuresnaptolayer = %d \n", failuresnaptolayer);						
		printf("toeatfailure = %lf %lf \n", toeatfailure.station, toeatfailure.elevation);							
		printf("pointatfailure = %d \n", pointatfailure);						
		printf("Abank->failureparameters.apparentcohesion = %lf \n", Abank->failureparameters.apparentcohesion);	
		printf("Abank->failureparameters.bottomvalleypoint = %lf %lf \n", Abank->failureparameters.bottomvalleypoint.station, Abank->failureparameters.bottomvalleypoint.elevation);
		printf("Abank->failureparameters.factorofsafety = %lf \n", Abank->failureparameters.factorofsafety);
		printf("Abank->failureparameters.hydrostaticforce = %lf \n", Abank->failureparameters.hydrostaticforce);
		printf("Abank->failureparameters.mobilizedshear = %lf \n", Abank->failureparameters.mobilizedshear);
		printf("Abank->failureparameters.normalforce = %lf \n", Abank->failureparameters.normalforce);
		printf("Abank->failureparameters.porewaterforce = %lf \n", Abank->failureparameters.porewaterforce);
		printf("Abank->failureparameters.tensioncrack = %d \n", Abank->failureparameters.tensioncrack);
		printf("Abank->failureparameters.topvalleypoint = %lf %lf \n", Abank->failureparameters.topvalleypoint.station, Abank->failureparameters.topvalleypoint.elevation);
		printf("Abank->failureparameters.waterweight = %lf \n", Abank->failureparameters.waterweight);
		printf("Abank->failureparameters.weight = %lf \n", Abank->failureparameters.weight);
		printf("Abank->failureparameters.weightslices = %lf \n", Abank->failureparameters.weightslices);
		printf("Abank->failureparameters.angle = %lf \n", Abank->failureparameters.angle);
	}

	// If bank fails, determine volume of slumped material and update cross section.
	if (fos < 1.) 
		{
			// Determine the number of soil layers in the sliding mass
			for(layer=0; layer<=Abank->numberoflayers-1; layer++)  //FORISSUE
				{
					if(Abank->direction*Abank->material[layer].bottomindex < Abank->direction*toe) {break;}	// This does not actualy happen, since the bottom index of the lowest layer is coincident with the toe index
					if(Abank->nodes[Abank->material[layer].bottomindex].elevation <= toeatfailure.elevation) {break;}
				}
			numberoflayers = layer+1;																		// Recall layers are counted from zero

			// Compute weight of failure block by soil layer
			bottomintersect = Abank->failureparameters.topvalleypoint;

			failurearea = 0.0;																				// Addition MAR2010

			for (layer=0; layer<=numberoflayers-1; layer++)
				{
					// Top valley-side intersect of soil layer and slip surface is the bottom valley-side intersect of the soil layer above
					topintersect = bottomintersect;

					// Assemble vertices of polygon of the area of the soil layer within the failure block

					// Bottom side
					vertex = -1;
					if (layer == numberoflayers-1) 
						{
							// One vertex only: intersect of slip surface and bank face
							nodeworkspace[vertex+1] = toeatfailure;
							bottomintersect = toeatfailure;		
							vertex = vertex + 1;		// Update of vertex
						}
					else
						{
							// Two vertices, stream side:
							nodeworkspace[vertex+1] = Abank->nodes[Abank->material[layer].bottomindex];
	
							// Valley side: intersect of bottom of soil layer and slip surface
							bottomintersect.elevation = Abank->material[layer+1].elevation;
							bottomintersect.station = toeatfailure.station + Abank->direction * (bottomintersect.elevation - toeatfailure.elevation) / tan(failureangle*3.14159/180);

							// Check for tensioncrack in this layer and possibly reset the bottomintersect
							if (Abank->direction*topintersect.station < Abank->direction*bottomintersect.station) {bottomintersect.station = topintersect.station;}
							nodeworkspace[vertex+2] = bottomintersect;
							vertex = vertex + 2;					// Update of vertex
						}

					
					// Check if tension crack ends in this layer.  If so, add node to polygon.
					if(Abank->failureparameters.bottomvalleypoint.elevation > bottomintersect.elevation && Abank->failureparameters.bottomvalleypoint.elevation < topintersect.elevation) 
						{
							nodeworkspace[vertex+1] = Abank->failureparameters.bottomvalleypoint;
							vertex = vertex + 1;
						}
//if(inode == data->MonitorNode)for(int s = 0; s <= vertex; s++)printf("BOTTOMANDTENSCRACK %d = %lf	%lf\n",s,nodeworkspace[s].station,nodeworkspace[s].elevation);//MMMMTEMP Printing
					// Top valley-side vertex
					nodeworkspace[vertex+1] = topintersect;
					vertex = vertex + 1;
//if(inode == data->MonitorNode)for(int s = 0; s <= vertex; s++)printf("TOPVALLEYSIDE %d = %lf	%lf\n",s,nodeworkspace[s].station,nodeworkspace[s].elevation);//MMMMTEMP Printing
					// Top stream-side vertex
					if(layer != 0)				// Correction MAR2010	
						{						// Correction MAR2010		
							nodeworkspace[vertex+1] = Abank->nodes[Abank->material[layer].topindex];
							vertex = vertex + 1;
						}						// Correction MAR2010		
					//if(layer == 0)																						//DM-FEB2012			//MMMMTEMP Commented out    //MMMMTEMP Modification to PlanarFailure routine
					//	{																									//DM-FEB2012			//MMMMTEMP Commented out    //MMMMTEMP Modification to PlanarFailure routine		
					//		nodeworkspace[vertex+1] = Abank->nodes[Abank->material[layer].topindex-Abank->direction];		//DM-FEB2012			//MMMMTEMP Commented out	//MMMMTEMP Modification to PlanarFailure routine
					//		vertex = vertex + 1;																			//DM-FEB2012			//MMMMTEMP Commented out	//MMMMTEMP Modification to PlanarFailure routine
					//	}																									//DM-FEB2012			//MMMMTEMP Commented out	//MMMMTEMP Modification to PlanarFailure routine	

//if(inode == data->MonitorNode)for(int s = 0; s <= vertex; s++)printf("TOPSTREAMSIDE %d = %lf	%lf\n",s,nodeworkspace[s].station,nodeworkspace[s].elevation);//MMMMTEMP Printing
					// Find first node on the bank face that may possibly be added to the polygon
					point = top;
//if(inode == data->MonitorNode)printf("point %d	%lf	%lf \n",point, nodeworkspace[vertex].station, nodeworkspace[vertex].elevation);//MMMMTEMP Printing
					//while(nodeworkspace[vertex].elevation <= Abank->nodes[point].elevation)  //MMMMTEMP Commented out   //MMMMTEMP Modification to PlanarFailure routine
					while(nodeworkspace[vertex].elevation < Abank->nodes[point].elevation || (nodeworkspace[vertex].station-Abank->nodes[point].station)*Abank->direction < 0.0) //MMMMTEMP Added second condition	//MMMMTEMP Modification to PlanarFailure routine
						{
							point = point - Abank->direction;
						}
//if(inode == data->MonitorNode)printf("point %d \n",point);//MMMMTEMP Printing
					// Add bank nodes to polygon until a node's elevation equals or drops below the elevation of the intersect of bank profile with the bottom of soil layer.
					while (Abank->nodes[point].elevation > nodeworkspace[0].elevation)
						{
							nodeworkspace[vertex+1] = Abank->nodes[point];
							//if(inode == data->MonitorNode && Abank->direction == -1)printf("nodeworkspace[vertex+1] = %lf	%lf\n",nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation);//MMMMTEMP Printing
							// Increment vertex and decrement bank profile index
							vertex = vertex + 1; 
							point = point - Abank->direction;
						}
//if(inode == data->MonitorNode)for(int s = 0; s <= vertex; s++)printf("ADDED ON BANK %d = %lf	%lf\n",s,nodeworkspace[s].station,nodeworkspace[s].elevation);//MMMMTEMP Printing
					// Compute weight of soil layer and area of failure block
					area = PolygonArea(nodeworkspace,vertex+1) ;		// Recall that the number of vertices is vertex + 1
 					failurearea = failurearea + area;
					//if(inode == data->MonitorNode)																										//MMMMTEMP Printing
					//	{																																	//MMMMTEMP Printing
							//for(int p = 0; p<=vertex; p++)printf("BLOCK		%d	%lf	%lf\n",p,nodeworkspace[p].station,nodeworkspace[p].elevation);		//MMMMTEMP Printing
							//printf("Area	= %lf\n",area);																									//MMMMTEMP Printing
					//	}																																	//MMMMTEMP Printing
					weight[layer] = Abank->material[layer].unitweight * area;
				}		// End For on layers
	
																														// Set flux of slumped material. To convert failed volume of bank material to
																														// that being trnsported, multiply it by the ratio of dry bulk weight of
																														// bank material and unit weight of sediment.
																														//-DM- This part is for now not included in the code

																														//lateralflux = 0.
																														//DO layer=1,numberoflayers
																														//lateralflux = lateralflux + ( weight(layer) * Abank%material(layer)%composition%sizefraction / ( gravity * 2650 ) ) &
																														//                               / timestep
																														//ENDDO

			// Update the nodes at the bank
			
			// Store node in the floodplain (this part is not present in the original CONCEPTS, which makes a distiction between bank and floodplain)
			if(Abank->direction == -1)
				{
					FloodplainNodeLeft = Abank->nodes[0]; 
				}
			if(Abank->direction == 1)
				{
					FloodplainNodeRight = Abank->nodes[Abank->numberofnodes-1]; 
				}

/////////////////////////////////////////////////////////////////////////////////////////////////
			// ADDITION MAR2010
			// Store other possible existing bank nodes which are at a higher location than topvalleypoint


			OtherNodesToAddLeft = 0;
			OtherNodesToAddRight = 0;

			if(Abank->direction == -1)			// Left bank
				{
					for (int i = 1; i <= Abank->numberofnodes - 1; i++)		// Loop from floodplain to the bank toe
						{
							if(Abank->nodes[i].elevation > Abank->failureparameters.topvalleypoint.elevation)
								{
									OtherNodesToAddLeft = OtherNodesToAddLeft + 1;			// Counter of nodes to be added on the left bank
									NodeToAddLeft[i-1] = Abank->nodes[i];					// Nodes stored in the direction towards the toe
								}
						} 
				}
			else if(Abank->direction == 1)		// Right bank
				{
					for (int i = top - 1; i >= 0; i--)						// Loop from floodplain to the bank toe
						{
							if(Abank->nodes[i].elevation > Abank->failureparameters.topvalleypoint.elevation)
								{
									OtherNodesToAddRight = OtherNodesToAddRight + 1;		// Counter of nodes to be added on the right bank
									NodeToAddRight[(top-1)-i] = Abank->nodes[i];			// Nodes stored in the direction towards the toe
								}
						} 
					 
				}
/////////////////////////////////////////////////////////////////////////////////////////////////

			// Set number of points to trim from the bank profile that were a part of the sliding soil mass
			numberofpoints = Abank->direction * ( top - pointatfailure )+1;
					
			// Trim the points from the profile
			if (Abank->direction == 1)				//right bank
				{        
					TrimAtBack(Abank->nodes,Abank->numberofnodes, numberofpoints);
				}
			else if (Abank->direction == -1)		//left bank
				{   
					TrimAtFront(Abank->nodes,Abank->numberofnodes, numberofpoints);
				}
			
			// Add the base and top of the slip surface to the bank profile
			if (Abank->direction == 1)						//right bank
				{     
					// Check if base intersects with top node in the profile. If it does do not add it to the profile, otherwise at the base of the slip surface to the profile
					//if (toeatfailure.station != Abank->nodes[Abank->numberofnodes-1].station && toeatfailure.elevation != Abank->nodes[Abank->numberofnodes-1].elevation) //DM-FEB2012 Commented out
					if (toeatfailure.station != Abank->nodes[Abank->numberofnodes-1].station || toeatfailure.elevation != Abank->nodes[Abank->numberofnodes-1].elevation) //DM-FEB2012
						{		
							AddAtBack(Abank->nodes,Abank->numberofnodes,1);
							Abank->nodes[Abank->numberofnodes-1] = toeatfailure;
						}
					
					// Add the base of the tension crack if present
				    if (Abank->failureparameters.tensioncrack) 
						{	
							AddAtBack(Abank->nodes,Abank->numberofnodes,1);
							Abank->nodes[Abank->numberofnodes-1]  = Abank->failureparameters.bottomvalleypoint;
						}

					// Add new bank top
					AddAtBack(Abank->nodes,Abank->numberofnodes,1);
					Abank->nodes[Abank->numberofnodes-1] = Abank->failureparameters.topvalleypoint;

/////////////////////////////////////////////////////////////////////////////////////////////////
					// ADDITION MAR2010
					// Add other possible existing bank nodes which are at a higher location than topvalleypoint
					if (OtherNodesToAddRight != 0)
						{
							for(int i = OtherNodesToAddRight - 1; i >= 0; i--)				// Nodes added in the direction towards the floodplain
								{
									AddAtBack(Abank->nodes,Abank->numberofnodes,1);
									Abank->nodes[Abank->numberofnodes-1] = NodeToAddRight[i];
								}
						}
/////////////////////////////////////////////////////////////////////////////////////////////////

					// Add floodplain node
					AddAtBack(Abank->nodes,Abank->numberofnodes,1);
					Abank->nodes[Abank->numberofnodes-1] = FloodplainNodeRight;

				}
			else if (Abank->direction == -1)				//left bank
				{
					// Check if base intersects with top node in the profile. If it does do not add it to the profile, otherwise at the base of the slip surface to the profile
					//if (toeatfailure.station != Abank->nodes[0].station && toeatfailure.elevation != Abank->nodes[0].elevation) //DM-FEB2012 Commented out
					if (toeatfailure.station != Abank->nodes[0].station || toeatfailure.elevation != Abank->nodes[0].elevation) //DM-FEB2012
						{
							AddAtFront(Abank->nodes,Abank->numberofnodes,1);
							Abank->nodes[0] = toeatfailure;
						}

					// Add the base of the tension crack if present
				    if (Abank->failureparameters.tensioncrack) 
						{	
							AddAtFront(Abank->nodes,Abank->numberofnodes,1);
							Abank->nodes[0]  = Abank->failureparameters.bottomvalleypoint;
						}

					// Add new bank top
					AddAtFront(Abank->nodes,Abank->numberofnodes,1);
					Abank->nodes[0] = Abank->failureparameters.topvalleypoint;

/////////////////////////////////////////////////////////////////////////////////////////////////
					// ADDITION MAR2010
					// Add other possible existing bank nodes which are at a higher location than topvalleypoint
					if (OtherNodesToAddLeft != 0)
						{
							for(int i = OtherNodesToAddLeft - 1; i >= 0; i--)			// Nodes added in the direction towards the floodplain
								{
									AddAtFront(Abank->nodes,Abank->numberofnodes,1);
									Abank->nodes[0] = NodeToAddLeft[i];
								}
						}
/////////////////////////////////////////////////////////////////////////////////////////////////


					// Add floodplain node
					AddAtFront(Abank->nodes,Abank->numberofnodes,1);
					Abank->nodes[0] = FloodplainNodeLeft;
				}

			
			// Determine the intersects of soil layers and new part of the bank profile and update indices.
			AddLayerIntersects(Abank);


			// Check if bank is undercut. If so, set undercut to the index of the point that is sticking out the most.
			//SetUndercut(Abank, maxundercut);    //I commented out this part since there cannot be undercut after the planar failure occurs

																														// Update the nodes at the floodplain
																														// This part, originally present in CONCEPTS, is not included here, since there is no Floodplain (all nodes belong to a Bank)

																														// Set number of points to trim from the floodplain profile that were a part of the sliding soil mass
																														//        intersectpoint = toefp + Abank%direction
																														//        DO WHILE (Abank%direction*Afloodplain%nodes(intersectpoint)%station &
																														//                        < Abank%direction*Abank%failureparameters%topvalleypoint%station)
																														//          intersectpoint = intersectpoint + Abank%direction
																														//        ENDDO
																														//        numberoffppoints = Abank%direction * ( intersectpoint - toefp )

																														// Trim the points from the profile
																														//        IF (Abank%direction == 1) THEN        !right floodplain
																														//          CALL TrimAtFront(Afloodplain%nodes,numberoffppoints)
																														//        ELSEIF (Abank%direction == -1) THEN   !left floodplain
																														//          CALL TrimAtBack(Afloodplain%nodes,numberoffppoints)
																														//        END IF

																														// Add the top of the slip surface to the floodplain profile
																														//IF (Abank%direction == 1) THEN        !right floodplain

																														// Check if top intersects with toe node in the profile. If it does do not add
																														//it to the profile, otherwise at the top of the slip surface to the profile.
																														//          IF (Abank%failureparameters%topvalleypoint /= Afloodplain%nodes(1)) THEN
																														//            CALL AddAtFront(Afloodplain%nodes,1)
																														//            Afloodplain%nodes(1) = Abank%failureparameters%topvalleypoint
																														//          END IF

																														//        ELSEIF (Abank%direction == -1) THEN   !left floodplain

																														// Check if top intersects with toe node in the profile. If it does do not add
																														//it to the profile, otherwise at the top of the slip surface to the profile.

																														//          IF (Abank%failureparameters%topvalleypoint /= Afloodplain%nodes(SIZE(Afloodplain%nodes))) THEN
																														//            CALL AddAtBack(Afloodplain%nodes,1)
																														//            Afloodplain%nodes(SIZE(Afloodplain%nodes)) = Abank%failureparameters%topvalleypoint
																														//          END IF

																														//END IF

		}  // End If bank fails

	if(fos < 1.0)																				//DM-FEB2012
		{																						//DM-FEB2012
			Abank->AblockPlanar = failurearea;													//DM-FEB2012
			Abank->FactorOfSafety = fos;														//DM-FEB2012
			Abank->PlanarFailureAngle = failureangle;											//DM-FEB2012
			Abank->PlanarFailureToe = toeatfailure;												//DM-FEB2012
		}																						//DM-FEB2012
	else																						//DM-FEB2012
		{																						//DM-FEB2012
			Abank->AblockPlanar = 0.0;															//DM-FEB2012
			Abank->FactorOfSafety = fos;														//DM-FEB2012
			Abank->PlanarFailureAngle = 9999.9999;											//DM-FEB2012
			Abank->PlanarFailureToe.station = 9999.9999;										//DM-FEB2012
			Abank->PlanarFailureToe.elevation = 9999.9999;									//DM-FEB2012
		}																						//DM-FEB2012
//	if(Abank->FloodplainElevation-Abank->material[1].elevation > 0.45)								//MMMMTEMP For GRL paper
//		{//MMMMTEMP For GRL paper
			Abank->TotalAblock = Abank->TotalAblock + Abank->AblockPlanar;								//DM-FEB2012
//		}//MMMMTEMP For GRL paper
}

void RMBankErosion::DecayBlock(Bank *Abank, RMBankErosionData* data, double deltaT, double intermittency, int iteration)	//DM-FEB2012
{
	//Method 1
	//The slump block volume per unit streamwise distance is decayed between floods by a constant factor specified in the file "prototype.cfg"
	//Abank->TotalAblock = Abank->TotalAblock*data->tblock;

	//Method 2
	//The slump block volume per unit streamwise distance is decayed as in Parker et al. (2011)
	//Decay of block is made every frequencydecay time steps (frequencydecay is the number of time steps in a single flood event)
	int frequencydecay;
	double deltaTdry;
	frequencydecay = int(intermittency/(deltaT/365.0));	
	if ((iteration % frequencydecay) == 0)
		{
			deltaTdry = deltaT*(1-intermittency)/intermittency;
			deltaTdry = (1-intermittency)*365.0;
			Abank->TotalAblock = Abank->TotalAblock*exp(-(deltaTdry/data->tblock));
		}
/*
	else
		{
			if(Abank->FloodplainElevation-Abank->material[1].elevation > 0.45)								//MMMMTEMP For GRL paper
				{
					Abank->TotalAblock = Abank->AblockCantilever;
				}
		}
*/
}

void RMBankErosion::ComputeKarmor(Bank *Abank, RMBankErosionData* data, int inode, int iteration, double intermittency, double deltaT)			//DM-FEB2012	
{
	int frequencydecay;

	frequencydecay = int(intermittency/(deltaT/365.0));	

	//printf("%d	%lf\n",inode,Abank->FloodplainElevation-Abank->material[1].elevation);
	//if(Abank->FloodplainElevation-Abank->material[1].elevation > 0.65)								//MMMMTEMP For GRL paper
	//{																								//MMMMTEMP For GRL paper
		if ((iteration % frequencydecay) == 0)		//Armor factor before new flood
			{
				//Abank->Karmor = exp(-(Abank->TotalAblock)*c);
				Abank->Karmor = exp(-(Abank->TotalAblock)*data->c_factor_for_karmor);				//MMMMTEMP Additional parameter in "prototype.cfg"
				//Abank->Karmor = 1-c*Abank->TotalAblock;
			}
		else										//Armor factor during a flood
			{
				//Abank->Karmor = exp(-(Abank->TotalAblock)*d);
				Abank->Karmor = exp(-(Abank->TotalAblock)*data->d_factor_for_karmor);				//MMMMTEMP Additional parameter in "prototype.cfg"  
				//Abank->Karmor = 1-d*Abank->TotalAblock;
			}
	//}																								//MMMMTEMP For GRL paper
	//else																							//MMMMTEMP For GRL paper
	//	{																							//MMMMTEMP For GRL paper
	//		Abank->Karmor = 1.0;																	//MMMMTEMP For GRL paper
	//	}																							//MMMMTEMP For GRL paper
}

Coordinates RMBankErosion::NodeAtZposition(const Coordinates& node1, const Coordinates& node2, double z)
{
	Coordinates NodeAtZposition;

    NodeAtZposition.station = node1.station + InverseSlope(node1,node2) * ( z - node1.elevation );
    NodeAtZposition.elevation = z;

	return NodeAtZposition;
} 

void RMBankErosion::MinimumFactorOfSafety(Bank *Abank, int top, Coordinates& basepoint, int baseindex, double stage, double& angle, double& fos, FailureParameters& parameters, RMBankErosionData* data, RMMigrationData* migdata, int inode)		//Afloodplain is also an argument in CONCEPTS
{	
	//if (inode == data->MonitorNode) {logmsg("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n");}  
	//if (inode == data->MonitorNode) {logmsg("basepoint.station = %lf, basepoint.elevation = %lf \n", basepoint.station, basepoint.elevation);}  
	const double balanceadjustment = 0.9;
	const double maxbalans = 10.0;
	double minangle, maxangle;
	int point, searchlim, status;
	double minspacing, quarttolerance, slopetolerance, intvltolerance;
    int additionallim, initiallim, iterationleft, iterationright, numberofsearches;
	double angle1, angle2, angle3, angle4;
    double value1, value2, value3, value4;
	double slope1, slope2;
	double rightfactor, leftfactor;

	// Initialize minimum angle, which is that of the line connecting 'basepoint' and
	// the valley-most point on the floodplain, maximum angle, and the angle
    if(Abank->direction == 1)									//Right bank
		{minangle = atan(fabs(Slope(basepoint,Abank->nodes[Abank->numberofnodes-1])))*180/3.14159;}
	else if (Abank->direction == -1)							//Left bank
		{minangle = atan(fabs(Slope(basepoint,Abank->nodes[0])))*180/3.14159;}
		//if (inode == data->MonitorNode) {logmsg("minangle = %lf \n", minangle);}   
	minangle = max(10.0,minangle);								//Set the minimum angle to 10 degrees
	//if (inode == data->MonitorNode) {logmsg("minangle = %lf \n", minangle);}   

	maxangle = 65.0;											//Set the maximum angle to 65 degrees
	//if (inode == data->MonitorNode) {logmsg("maxangle = %lf \n", maxangle);}  

	angle = 0.25 * 180 + 0.5 * Abank->material[0].reposeangle;
	//if (inode == data->MonitorNode) {logmsg("angle = %lf \n", angle);}  

	//if (inode == 0) {logmsg("minangle = %lf 	angle = %lf 	maxangle = %lf \n", minangle,angle,maxangle);}  	
	// Set machine and precision dependent constants
    // minspacing = EPSILON(angle) / RADIX(angle)
    slopetolerance = 0.0001;				//10. * minspacing
    intvltolerance = 0.001;				//100. * minspacing
    quarttolerance = 0.25 * intvltolerance;

	// The initial angle can be increased (or decreased) by a factor of
	// 2.0**((initiallim-1)*initiallim-2)/2) (assuming we take initiallim >= 3, as
	// we should).  We take initiallim = 6 since a factor of 1024 seems sufficient.
    initiallim = 6;

	// additionallim = 6 seems to be sufficient since this number of iterations past
	// the initialization seems to only rarely be exceeded.
    additionallim = 6;
    numberofsearches = 0 ; 
	iterationleft = 0; 
	iterationright = 0;

	// We now try to compute values at points: angle1 = 0.5*reposeangle, point2
	// = angle, and point3 = bankangle - 3 degrees.  However if point2 > point3, then
	// point2 = 0.5 * ( angle1 + point3).
	angle3 = 0.5 * 180;
	if(Abank->direction == 1)
		{
			point = baseindex;													//CORRECTION FEB2010

			while(Abank->nodes[point].station-basepoint.station < 0.0)			//CORRECTION FEB2010
				{																//CORRECTION FEB2010
					point = point + 1;											//CORRECTION FEB2010
				}																//CORRECTION FEB2010	
			//if (inode == 0)logmsg("point %d	baseindex %d	Abank->nodes[point].station %lf	basepoint.station %lf \n", point, baseindex, Abank->nodes[point].station,basepoint.station);
			angle3 = min(angle3 , atan(fabs(Slope(basepoint,Abank->nodes[point])))*180/3.14159);
		}
	else if(Abank->direction == -1)
		{
			point = baseindex;													//CORRECTION FEB2010
			while(Abank->nodes[point].station-basepoint.station > 0.0)			//CORRECTION FEB2010
				{																//CORRECTION FEB2010
					point = point - 1;											//CORRECTION FEB2010
				}																//CORRECTION FEB2010
			angle3 = min(angle3 , atan(fabs(Slope(basepoint,Abank->nodes[point])))*180/3.14159);

		}
	angle3 = angle3 - 3;
	if (angle3 > maxangle) {angle3 = maxangle;}
    if (angle3 < minangle) 
		{
			angle = minangle ; 
			fos = 9999999.;
			return;                
		}
   
	angle1 = max(0.5 * Abank->material[0].reposeangle, minangle);

	if(angle1 >= angle3-intvltolerance)
		{
			// Bank geometry is such that a factor of safety evaluation is going to break
			// down return angle3 and a factor of safety arbitrarily set to a large number
			angle = angle3; 
			fos = 9999999.;
		}
	
	angle2 = angle;
	if (angle2 > angle3 || angle2 < angle1) {angle2 = 0.5*(angle1+angle3);}

	//if (inode == 0) {logmsg("angle1 = %lf 	angle2 = %lf 	angle3 = %lf \n", angle1,angle2,angle3);}  
	//if (inode == data->MonitorNode) {logmsg("	angle2 = %lf \n", angle2);}  
	//if (inode == data->MonitorNode) {logmsg("	angle3 = %lf \n", angle3);}  

	// Compute factor of safety for each angle.
	value1 = FactorOfSafety(Abank,basepoint,baseindex,angle1,stage,parameters,data,migdata,inode);
	//if(inode == data->MonitorNode)printf("-- Computed value 1 of FS = %lf--\n",value1,Abank->failureparameters.factorofsafety);//MMMMTEMP Printing
	value2 = FactorOfSafety(Abank,basepoint,baseindex,angle2,stage,parameters,data,migdata,inode);
	//if(inode == data->MonitorNode)printf("-- Computed value 2 of FS = %lf--\n",value2,Abank->failureparameters.factorofsafety);//MMMMTEMP Printing
	value3 = FactorOfSafety(Abank,basepoint,baseindex,angle3,stage,parameters,data,migdata,inode);
	//if(inode == data->MonitorNode)printf("-- Computed value 3 of FS = %lf--\n",value3,Abank->failureparameters.factorofsafety);//MMMMTEMP Printing
	//if (inode == 0) {logmsg("	value1 = %lf \n", value1);}  
	//if (inode == 0) {logmsg("	value2 = %lf \n", value2);}  
	//if (inode == 0) {logmsg("	value3 = %lf \n", value3);}  
	numberofsearches = 3;

	// We now have found angle1, angle2, and angle3 with corresponding values
	// value1, value2, and value3.  We expand the interval if necessary to try to
	// find new values with value2 <= MIN(value1,value3).

	if(value2 > value1)
		{
			if(value1 <= value3)
				{
					// Here we will expand the interval to the left, providing that numberofsearches
					// is smaller than initiallim and angle1-angle1/2.0**(numberofsearches-1) is
					// greater than or equal to quarttolerance.
					double exponent = numberofsearches-1;
					while (value2 > value1)
						{
							if(numberofsearches > initiallim || angle1-angle1 / pow(2,exponent) < quarttolerance)
								{
									//Here we cannot expand left and we return with the best values found so far.
									angle = angle1; 
									fos = value1;
									return;
								}

							// Expand left.
							angle3 = angle2; value3 = value2;
							angle2 = angle1; value2 = value1;
							double exponent = numberofsearches-1;
							angle1 = angle1/pow(2,exponent);
							
							if(angle1 < minangle)
								{
									angle1 = minangle + intvltolerance;
									numberofsearches = initiallim;
								}

							// Compute F(angle1).
							value1 = FactorOfSafety(Abank,basepoint,baseindex,angle1,stage,parameters,data,migdata,inode);
							numberofsearches = numberofsearches + 1;
		
						}
		
				}
			else
				{
					// Here we cannot expand right and we return with the best values found so far.
					angle = angle3; 
					fos = value3;
					return;
				}
		}
	else if(value2 > value3)
		{
			// Here we cannot expand right and we return with the best values found so far.
			angle = angle3; 
			fos = value3;
			return;     
			
		}
	// End of initialization.
//if (inode == 0) {logmsg("AFTERINIT angle1 = %lf	angle2 = %lf	angle3 = %lf \n", angle1,angle2,angle3);} 
//if (inode == 0) {logmsg("AFTERINIT value1 = %lf	value2 = %lf	value3 = %lf \n", value1,value2,value3);} 
	// Assuming that angle3-angle1 >= intvltolerance, we now have points angle1,
	// angle2, angle3 with angle1 <= angle2-quarttolerance,
	// angle2 <= angle3-quarttolerance, value1=F(angle1) >= value2=F(angle2), and
	// value3=F(angle3) >= value2.  These conditions will be maintained throughout
	// the program.

	// Set searchlim so that at most additionallim more computations of F will be
	// done.

    searchlim = numberofsearches + additionallim;

	// If we have computed F searchlim times, we put angle2 in angle, put value2 in
	// fos, and return.  Otherwise continue search for minimum.
    while(numberofsearches < searchlim)
		{
			// If the search interval length is less than intvltolerance we put angle2 in
			// angle, put value2 in fos, and return.
			if (angle3-angle1 < intvltolerance) 
				{
					break;
				}

			// Compute slope1 = the absolute value of the slope of the line through
			// (angle1,value1) and (angle2,value2), and slope2 = the (absolute value of the)
			// slope of the line through (angle2,value2) and (angle3,value3).

			slope1 = ( value1 - value2 ) / ( angle2 - angle1);
			slope2 = ( value3 - value2 ) / ( angle3 - angle2);

			// If slope1+slope2 is very small we return with the best values found so far.

			if(slope1+slope2 < slopetolerance)
				{
					break;
				}

			// The minimum of the quadratic polynomial passing through (angle1,value1),
			// (angle2,value2), and (angle3,value3) will occur at (leftfactor*angle1+
			// rightfactor*angle3+angle2)/2.0.  Note that the three points cannot be
			// collinear, else we would have terminated above.  Since the minimum occurs
			// at the average of angle2 and a convex combination of angle1 and angle3, it
			// will be at least as close to angle2 as to the endpoint on the same side.

			leftfactor = slope2 / ( slope1 + slope2 );
			rightfactor = 1.0 - leftfactor;
			
			if(iterationleft > 1) 
				{
					// Here the left endpoint was dropped at the last iterationleft > 1 iterations,
					// so to prevent a long string of such occurrences with little reduction of
					// angle3-angle1 we will shift the new point to the right by decreasing
					// leftfactor relative to rightfactor.
					double exponent = iterationleft-1;
					leftfactor = leftfactor / pow(2,exponent);
					rightfactor = 1. - leftfactor;
				}
			else 
				{
					if(iterationright > 1)
						{
							// Here the right endpoint was dropped at the last iterationright > 1
							// iterations, and we will shift the new point to the left.
							double exponent = iterationright-1;
							rightfactor = rightfactor / pow(2,exponent);
							leftfactor = 1. - rightfactor;
						}
					else
						{
							
							//Here we have not just had a string of two or more moves in the same direction,
							//but if the subintervals are out of balance by more than a factor of maxbalans,
							//we shift the new point slightly in the direction of the longer interval.  The
							//idea here is that the two close points are probably near the solution, and if
							//we can bracket the solution we may be able to cut off the major portion of the
							//longer subinterval.
							if(angle2-angle1 > maxbalans*(angle3-angle2)) 
								{
									// Here the left subinterval is more than maxbalans times longer than the right
									// subinterval, so we decrease rightfactor relative to leftfactor.
									rightfactor = balanceadjustment * rightfactor;
									leftfactor = 1. - rightfactor;
								}
							else if (angle3-angle2 > maxbalans*(angle2-angle1))
								{
									// Here the right subinterval is more than maxbalans times longer than the left
									// subinterval, so we decrease leftfactor relative to rightfactor.
									leftfactor = balanceadjustment * leftfactor;
									rightfactor = 1. - leftfactor;
								}
						}
				}

			// Compute the (possibly modified) minimum of the quadratic fit.
			angle4 = 0.5 * ( leftfactor * angle1 + rightfactor * angle3 + angle2 );

			// The next section modifies angle4 if necessary to get angle1+quarttolerance
			// <= angle2, angle4 <= angle3-quarttolerance and fabs(angle4-angle2)
			// >= quarttolerance.  In the unlikely event this is not possible we set
			// angle=angle2, fos=value2 and return.

			if(fabs(angle4-angle2) < quarttolerance)
				{
					//If fabs(angle4-angle2) < quarttolerance we redefine angle4 by moving
					// quarttolerance from angle2 into the longer subinterval.  Note that the length
					//of this subinterval must be at least intvltolerance/2.0 = 2.0*quarttolerance,
					// else we would have terminated earlier.
					if(angle3-angle2 > angle2-angle1)
						{
							status = CorrectP4Right(angle4,angle2,angle3,quarttolerance);	
							if(status == 0) 
								{
									break;
								}
						}
					else
						{
							status = CorrectP4Left(angle4,angle1,angle2,quarttolerance);	
							if(status == 0) 
								{
									break;
								}
						}
				}
			else
				{	
					// Here we had ABS(angle4-angle2) >= quarttolerance and we make sure that
					// angle1+quarttolerance <= angle4 <= angle3-quarttolerance.

					if(angle4 > angle3-quarttolerance) 
						{
							// Here angle4 > angle3-quarttolerance and we set angle4=angle3-quarttolerance
							// if angle3-angle2 >= intvltolerance/2.0, and otherwise we set
							// angle4=angle2-quarttolerance.
							if(angle3-angle2 >= 0.5*intvltolerance)
								{
									angle4 = angle3 - quarttolerance;
								}
							else
								{
									status = CorrectP4Left(angle4,angle1,angle2,quarttolerance);	
									if(status == 0) 
										{
											break;
										}
								}
						}
					else
						{
							if(angle4 < angle1+quarttolerance)
								{
									// Here angle4 < angle1+quarttolerance and we set angle4=angle1+quarttolerance
									// if angle2-angle1 >= intvltolerance/2.0, and otherwise we set
									// angle4=angle2+quarttolerance.
									if (angle2-angle1 >= 0.5*intvltolerance) 
										{
											angle4 = angle1 + quarttolerance;
										}
									else
										{
											status = CorrectP4Right(angle4,angle2,angle3,quarttolerance);  
											if(status == 0) 
												{
													break;
												}
										}
								}									
						}
				}

			// Compute F(angle4)
			numberofsearches = numberofsearches + 1;
			//if(inode == 164)printf("toepoint in MINIMUM FACTOR OF SAFET = %d\n",baseindex);//MMMMTEMP Printing
			value4 = FactorOfSafety(Abank,basepoint,baseindex,angle4,stage,parameters,data,migdata,inode);
			//if(inode == data->MonitorNode)printf("-- Computed value 4 of FS = %lf angle - %lf--\n",value4,angle4,Abank->failureparameters.factorofsafety);//MMMMTEMP Printing

			// Now we drop either angle1 or angle3 and relabel the remaining points so that
			// F(angle2) <= F(angle1) AND F(angle2) <= F(angle3).

			// If now the leftmost of the two middle points is lower than the rightmost of
			//the two middle points we drop angle3, and set iterationleft=0 and increment
			// iterationright to indicate the right endpoint has been dropped. Otherwise we
			//drop angle1, set iterationright=0 and increment iterationleft.  In all cases
			// we then reshuffle the values into angle1, angle2, angle3, value1, value2,
			//value3 and try to do another iteration.

			if(angle4 < angle2)
				{
					//Here angle4 is smaller than angle2.
					if(value4 < value2) 
						{
							angle3 = angle2; value3 = value2;
							angle2 = angle4; value2 = value4;
							iterationleft = 0 ; iterationright = iterationright + 1;
						}
					else
						{
							angle1 = angle4; value1 = value4;
							iterationleft = iterationleft + 1; iterationright = 0;
						}
				}
			else
				{
					// Here angle4 is greater than angle2.
					if (value2 < value4) 
						{
							angle3 = angle4; value3 = value4;
							iterationleft = 0; iterationright = iterationright + 1;
						}
					else
						{
							angle1 = angle2 ; value1 = value2;
							angle2 = angle4 ; value2 = value4;
							iterationleft = iterationleft + 1 ; iterationright = 0;
						}
				}
		
		}		// End While cycle

	angle = angle2; 
	fos = value2;
}

void RMBankErosion::TrimAtBack(vector<Coordinates>& nodes, int& numberofnodes, int number)			// Added numberofnodes as argument, and it is passed by reference
{
    nodes.erase(nodes.end() - number, nodes.end());
    numberofnodes -= number;
}



void RMBankErosion::TrimAtFront(vector<Coordinates>& nodes, int& numberofnodes, int number)			// Added numberofnodes as argument, and it is passed by reference
{
    nodes.erase(nodes.begin(), nodes.begin() + number);
    numberofnodes -= number;
}

void RMBankErosion::AddAtBack(vector<Coordinates>& nodes, int& numberofnodes, int number)					// Added numberofnodes as argument, and it is passed by reference
{
    nodes.insert(nodes.end(), number, nodes[0]);

    for (int i = numberofnodes; i < nodes.size(); i++)
		{
			nodes[i].station = 0.0;
			nodes[i].elevation = 0.0;
		}

	numberofnodes += number;
}


void RMBankErosion::AddAtFront(vector<Coordinates>& nodes, int& numberofnodes, int number)					// Added numberofnodes as argument, and it is passed by reference
{
    nodes.insert(nodes.begin(), number, nodes[0]);
    for (int i = 0; i < number; i++)
	{
		nodes[i].station = 0.0;
		nodes[i].elevation = 0.0;
	}
	numberofnodes += number;
}


double RMBankErosion::FactorOfSafety(Bank *Abank, Coordinates basepoint, int toepoint, double angle, double stage, FailureParameters& parameters, RMBankErosionData* data, RMMigrationData* migdata, int inode)		// CONCEPTS has also Afloodplain
{
	double maxtensioncrackdepth, anchorforce, FactorOfSafety, fpslope, fraction, hydro, totalnormal, watertable[100], weight[100], normal[100], weightstored[100]; //MMMMTEMP Added weightstored for computation of weightslices
	double fosnumerator, fosdenominator, sumcohesion, sumpore, sumweight, rdistance, sliplength, error, guess;
	int firstslice, toe, top, layer, numberoflayers, multiplier, numberofslices, intersectpoint, slice, topindex, bottomindex, index, iteration;
	bool chktensioncrack, tensioncrack, slicechange; 
	Coordinates failuresliceintersect[100], banksliceintersect[100], watertablemargin, flowmargin;
	double length[100], cohesion[100], pore[100], internormal[100], intershear[100], forcef[100];
	int ORDINARY = 1;								
	int JANBUSIMPLE = 2;							
	int MORGENSTERNPRICE = 3;
	double sumlength, totalweight;
	int storingslice;

	// Reset parameters of failing block
	parameters.tensioncrack = false;	
	parameters.apparentcohesion = 0.0;
	parameters.factorofsafety = 9999.0;  //MMMMTEMP Modification to get minimum factor of safety
	parameters.hydrostaticforce = 0.0;
	parameters.mobilizedshear = 0.0;
	parameters.normalforce = 0.0;
	parameters.normalforce = 0.0;
	parameters.porewaterforce = 0.0;
	parameters.waterweight = 0.0;
	parameters.weight = 0.0;
	parameters.bottomvalleypoint.station = -999999.0;
	parameters.bottomvalleypoint.elevation = -999999.0;
	parameters.topvalleypoint.station = -999999.0;
	parameters.topvalleypoint.elevation = -999999.0;

	// Initialize indices and other parameters
	if (data->tensioncrackoption < 0.) 
		{
			maxtensioncrackdepth = 2*(Abank->material[0].cohesion/Abank->material[0].unitweight)*tan(0.25*3.14159+Abank->material[0].reposeangle*3.14159/180);
		}
	else
		{
			maxtensioncrackdepth = data->tensioncrackoption;
		}
	
//if(inode==0){logmsg("maxtensioncrackdepth = %lf \n", maxtensioncrackdepth);}
	firstslice = 0;
	chktensioncrack = false; 
	tensioncrack = false; 
	FactorOfSafety = 999999.;
    anchorforce = 0.;

	if (Abank->direction == 1)         // right bank
		{
			toe = 0;
			top = Abank->numberofnodes-1; 
		}
	else if (Abank->direction == -1)		// left bank
		{
			toe = Abank->numberofnodes -1; 
			top = 0; 
		}

	// Set number of slices and soil layers
	for (layer=0; layer<= Abank->numberoflayers-1; layer++)
		{
			if (Abank->direction*Abank->material[layer].bottomindex < Abank->direction*toe) {break;}
			if (Abank->nodes[Abank->material[layer].bottomindex].elevation <= basepoint.elevation) {break;}
		}

	numberoflayers = layer+1;

	multiplier = data->slicemultiplier;
    numberofslices = multiplier * numberoflayers;

	// Set intersects of slip surface, slices, and bank profile

	// a. Intersects of slices and slip surface.  First compute the intersects of slip surface and soil layers
	//if(inode == 164)printf("toepoint = %d\n",toepoint);//MMMMTEMP Printing
	//    failuresliceintersect(0) is upper intersect of slip surface and bank
	//if(inode==0){logmsg("toepoint	%d	\n", toepoint);}	
	/*
	if(basepoint.station == Abank->nodes[toepoint].station && basepoint.elevation == Abank->nodes[toepoint].elevation)				//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
		{																															//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
			intersectpoint = toepoint+Abank->direction;																				//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
		}																															//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
	else																															//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
		{																															//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
			intersectpoint = toepoint;			// This is the only line present in the original CONCEPTS							//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
												// I put if, else since, if basepoint happens to be coincident						//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
												// with Abank->nodes[toepoint], intersectpoint is not calculated					//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
												// correctly																		//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
		}																															//MMMMTEMP Commented out   //MMMMTEMP Modification to FactorOfSafety routine
	*/
	intersectpoint = toepoint;//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
	//if(inode == 9)printf("intersectpoint = %d\n",toepoint);//MMMMTEMP Printing
	//if(inode == 9)printf("COMPARE %lf	%lf	%lf	%lf	\n",basepoint.station,basepoint.elevation,Abank->nodes[intersectpoint].station,Abank->nodes[intersectpoint].elevation);//MMMMTEMP Printing
	//if(inode == 9)printf("COMPARE %e	%e 	\n",basepoint.station-Abank->nodes[intersectpoint].station,basepoint.elevation-Abank->nodes[intersectpoint].elevation);//MMMMTEMP Printing
	while(abs(basepoint.station - Abank->nodes[intersectpoint].station)<0.0001 && abs(basepoint.elevation - Abank->nodes[intersectpoint].elevation)<0.0001)		//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
	{																																							//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
		//if(inode == 9)printf("ADD + 1\n");																														//MMMMTEMP Printing	
		intersectpoint = intersectpoint+Abank->direction;																										//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine	
	}																																							//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
	//if(inode == 9)printf("intersectpoint = %d\n",toepoint);//MMMMTEMP Printing

	//if(inode == 172 && Abank->direction == 1)for(int k = 0; k<= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation); //MMMMTEMP Printing
	//if(inode == 172 && Abank->direction == 1)printf("Basepoint	%lf	%lf\n",basepoint.station,basepoint.elevation); //MMMMTEMP Printing
	//if(inode == 172 && Abank->direction == 1)printf("intersectpoint	%d\n",intersectpoint); //MMMMTEMP Printing
	//if(inode == 172 && Abank->direction == 1)printf("Slope = %lf\n",Slope(basepoint,Abank->nodes[intersectpoint])); //MMMMTEMP Printing
	//if(inode==0){logmsg("Basepoint	%lf	%lf\n", basepoint.station, basepoint.elevation);}
	//if(inode==0){logmsg("Abank->nodes[intersectpoint]	%lf	%lf\n", Abank->nodes[intersectpoint].station, Abank->nodes[intersectpoint].elevation);}
	//if(inode == 9)printf("BASEPOINT = %lf	%lf\n",basepoint.station, basepoint.elevation);//MMMMTEMP Printing
	//if(inode == 9)printf("BEFORE FINDING INTERSECTPOINT intersectpoint = %d slope = %lf, angle = %lf\n",intersectpoint, Slope(basepoint,Abank->nodes[intersectpoint]),angle);//MMMMTEMP Printing	
	if(Abank->direction == 1)	//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
		{						//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
			while (fabs(Slope(basepoint,Abank->nodes[intersectpoint])) > tan(angle*3.14159/180) || Slope(basepoint,Abank->nodes[intersectpoint]) < 0.0)			//MMMTEMP Added second condition   //MMMMTEMP Modification to FactorOfSafety routine
				{   	
					//if(inode == 172 && Abank->direction == 1)printf("ENTER LOOP\n");//MMMMTEMP Printing
					//if(inode == 172 && Abank->direction == 1)printf("basepoint.station = %lf, basepoint.elevation = %lf, Slope = %lf, intersectpoint = %d\n",basepoint.station, basepoint.elevation,Slope(basepoint,Abank->nodes[intersectpoint]),intersectpoint); //MMMMTEMP Printing
					//if(inode == 172 && Abank->direction == 1)printf("Abank->nodes[intersectpoint].station = %lf,Abank->nodes[intersectpoint].elevation = %lf\n",Abank->nodes[intersectpoint].station,Abank->nodes[intersectpoint].elevation); //MMMMTEMP Printing
					intersectpoint = intersectpoint + Abank->direction;
					//if(inode == 172 && Abank->direction == 1)printf("HERE\n");//MMMMTEMP Printing
					//if(inode == 172 && Abank->direction == 1)printf("%lf	%lf	%lf	%lf\n",fabs(Slope(basepoint,Abank->nodes[intersectpoint])),tan(angle*3.14159/180),angle,Slope(basepoint,Abank->nodes[intersectpoint]));//MMMMTEMP Printing
				}
		}//MMMMTEMP		   //MMMMTEMP Modification to FactorOfSafety routine
	else if(Abank->direction == -1)	//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
		{						//MMMMTEMP   //MMMMTEMP Modification to FactorOfSafety routine
			while (fabs(Slope(basepoint,Abank->nodes[intersectpoint])) > tan(angle*3.14159/180)  || Slope(basepoint,Abank->nodes[intersectpoint]) > 0.0)        //MMMTEMP Added second condition   //MMMMTEMP Modification to FactorOfSafety routine
				{
					intersectpoint = intersectpoint + Abank->direction;
				}
		}//MMMMTEMP		   //MMMMTEMP Modification to FactorOfSafety routine

//if(inode==0){logmsg("intersectpoint	%d	\n", intersectpoint);}	
	//if(inode == data->MonitorNode){logmsg("intersectpoint = %d\n", intersectpoint);}

	// slope of bank where slip surface intersects bank
    fpslope = Slope(Abank->nodes[intersectpoint-Abank->direction],Abank->nodes[intersectpoint]);
	//if(inode==0){logmsg("fpslope	%lf	", fpslope);}
	//if(inode == 9)printf("INTERCS SLICE %lf	%lf	%lf	%lf --> slope = %lf\n",Abank->nodes[intersectpoint-Abank->direction].station,Abank->nodes[intersectpoint-Abank->direction].elevation,Abank->nodes[intersectpoint].station, Abank->nodes[intersectpoint].elevation,fpslope);//MMMMTEMP Printing	
	//intersect
	failuresliceintersect[0].station=((Abank->nodes[intersectpoint-Abank->direction].elevation - basepoint.elevation)+ Abank->direction * tan(angle*3.14159/180) * basepoint.station - fpslope * Abank->nodes[intersectpoint-Abank->direction].station) / ( Abank->direction * tan(angle*3.14159/180) - fpslope );
    failuresliceintersect[0].elevation = Abank->nodes[intersectpoint-Abank->direction].elevation + Slope(Abank->nodes[intersectpoint-Abank->direction],Abank->nodes[intersectpoint]) * ( failuresliceintersect[0].station - Abank->nodes[intersectpoint-Abank->direction].station );
	//if(inode == 9)printf("INTERCS SLICE %lf	%lf, slope = %lf\n",failuresliceintersect[0].station,failuresliceintersect[0].elevation,fpslope);//MMMMTEMP Printing

	//    failuresliceintersect(multiplier:numberofslices-1:multiplier) are the
	//    intersects of the slip surface and the top elevation of soil layer "layer".
	for (slice=multiplier; slice<=numberofslices-1;slice = slice + multiplier)			//Changed from the original CONCEPTS
		{
			layer = ( slice + multiplier ) / multiplier-1;				//Changed from the original CONCEPTS
			failuresliceintersect[slice].elevation = Abank->material[layer].elevation;
			failuresliceintersect[slice].station =	basepoint.station + Abank->direction * ( failuresliceintersect[slice].elevation - basepoint.elevation ) / tan(angle*3.14159/180);
		}

	//    failuresliceintersect(numberofslices) is the basepoint
	failuresliceintersect[numberofslices] = basepoint;
	
	//    here we compute intersects of the slip surface with intermediate slices whose sides are not aligned with soil layers
	for(layer=0;layer<=numberoflayers-1;layer++)			//Changed from the original CONCEPTS    //FORISSUE
		{        
			topindex = ( layer ) * multiplier;				//Changed from the original CONCEPTS
			bottomindex = (layer+1) * multiplier;			//Changed from the original CONCEPTS

			for(slice=1;slice<=multiplier-1;slice++)   //FORISSUE
				{          
					index = topindex + slice;
					fraction = double(slice) / double(multiplier);
					failuresliceintersect[index] = NodeAtFraction(failuresliceintersect[topindex],failuresliceintersect[bottomindex],fraction);
				}        
		}

/*if(inode == 0) 
	{
		for(int index = 0; index <= numberofslices; index++)
			{
					logmsg("FSI %lf	%lf	", failuresliceintersect[index].station, failuresliceintersect[index].elevation);
			}
	}
if(inode == 0) logmsg("\n################## \n");*/


	//  b. Intersects of slices and bank profile banksliceintersect(0) is failuresliceintersect(0)
	banksliceintersect[0] = failuresliceintersect[0];

	// banksliceintersect(1:numberofslices-1) are the intersects of the bank profile and the valley side of slices 1 through numberofslices-1
	for (slice=1; slice<=numberofslices-1; slice++)
		{
			//    if (Abank.direction*failuresliceintersect[slice].station > Abank.direction*Abank.nodes[top].station) 
			//	{//    Intersect with floodplain
			//   intersectpoint = SetMarginY(Afloodplain.nodes,failuresliceintersect[slice].station,toefp,topfp,Abank.direction);
			//   banksliceintersect[slice] = NodeAtYposition(Afloodplain.nodes[intersectpoint], Afloodplain.nodes[intersectpoint-Abank.direction],failuresliceintersect[slice].station);
			//	}
			//	else
			//	{// Intersect with streambank
			intersectpoint = SetMarginY(Abank->nodes,failuresliceintersect[slice].station,toe,top,Abank->direction);
			while (Abank->nodes[intersectpoint].elevation < basepoint.elevation)
				{
					intersectpoint = SetMarginY(Abank->nodes,failuresliceintersect[slice].station,intersectpoint+Abank->direction, top,Abank->direction);
				}
			//printf(">>> inode = %d, Monitor node = %d \n ",inode, data->MonitorNode);//MMMMTEMP Printing
			//for(int r = 0; r <= Abank->numberofnodes-1; r++)printf("%d	%lf	%lf\n",r,Abank->nodes[r].station,Abank->nodes[r].elevation);//MMMMTEMP Printing
			//printf("failuresliceintersect[%d].station = %lf (no. slices = %d)\n", slice,failuresliceintersect[slice].station, numberofslices);//MMMMTEMP Printing
			//printf("VVVVVVVVVV inode = %d\n",inode);//MMMMTEMP Printing
			//if(inode = data->MonitorNode)printf("START\n");//MMMMTEMP Printing
			banksliceintersect[slice] = NodeAtYposition(Abank->nodes[intersectpoint], Abank->nodes[intersectpoint-Abank->direction],failuresliceintersect[slice].station); 
			//if(inode = data->MonitorNode)printf("END\n");//MMMMTEMP Printing
		}

	//    banksliceintersect(numberofslices) is failuresliceintersect(numberofslices)
	banksliceintersect[numberofslices] = failuresliceintersect[numberofslices];

	//if(inode == 9)for (int p = 0; p<=numberofslices; p++)printf("INTERCS SLICE %d	%lf	%lf	%lf	%lf\n",p,banksliceintersect[p].station,banksliceintersect[p].elevation,failuresliceintersect[p].station,failuresliceintersect[p].elevation);//MMMMTEMP Printing
/*	if(inode == 0) 
		{
			for(int index = 0; index <= numberofslices; index++)
				{
					logmsg("BSI %lf %lf ", banksliceintersect[index].station, banksliceintersect[index].elevation);
				}
		}*/

	// Compute baselength of slices
	for(slice=0;slice<=numberofslices-1;slice++)   //FORISSUE
		{
			double DeltaStation = failuresliceintersect[slice].station-failuresliceintersect[slice+1].station;
			double DeltaElevation = failuresliceintersect[slice].elevation-failuresliceintersect[slice+1].elevation;
			length[slice] = pow(DeltaStation, 2)+pow(DeltaElevation, 2);
			length[slice] = pow(length[slice], 0.5);
		}

	// Compute cohesive force on slice base
	for(slice=0;slice<=numberofslices-1;slice++)      //FORISSUE
		{
			layer = ( slice + multiplier ) / multiplier - 1;		//Changed from the original CONCEPTS
			cohesion[slice] = length[slice] * Abank->material[layer].cohesion;
			//if(inode == data->MonitorNode){logmsg("cohesion[%d] (IFIRTS ITER) = %lf\n", slice, cohesion[slice]);}
		}

/*if(inode == 0) 
	{
		for(int index = 0; index <= numberofslices-1; index++)
			{
				logmsg("C %lf ", cohesion[index]);
			}
	}*/
/*	//if(inode == data->MonitorNode){logmsg("++++++++++++++++++++++++++++++\n");}

	/*if(inode == data->MonitorNode)
		{
			for(int i = 0; i <= Abank->numberofnodes-1; i++)
			{logmsg("%lf	%lf	BANKSt,El\n", Abank->nodes[i].station, Abank->nodes[i].elevation);}
			for(int i = 0; i <= numberofslices; i++)
			{logmsg("%lf	%lf	BANKSLICESt,El\n", banksliceintersect[i].station, banksliceintersect[i].elevation);}
			for(int i = 0; i <= numberofslices; i++)
			{logmsg("%lf	%lf	FAILURESLICESt,El\n", failuresliceintersect[i].station, failuresliceintersect[i].elevation);}
		}*/
	// Compute weight of each slice
	//if(inode == data->MonitorNode)printf("SLICEWEIGHT Call routine (inode = %d, angle = %lf)\n",inode,angle);//MMMMTEMP Printing
	SliceWeight(Abank, banksliceintersect, failuresliceintersect, numberofslices, multiplier, weight, 0, migdata, inode, data->MonitorNode);  //MMMMTEMP added monitornode as argument
	
	parameters.weight = 0;
	for (int counter = 0; counter<= numberofslices-1; counter++)
		{
			parameters.weight = parameters.weight + weight[counter];
			weightstored[counter] = weight[counter]; //MMMMTEMP For computation of weightslices
			//if(inode == data->MonitorNode){logmsg("^^^^^^^^^^^^^^^cOUNTER = %d  Weight = %lf\n", counter, weight[counter]);}
		}

	
//if(inode == data->MonitorNode){logmsg("Weight %lf ", parameters.weight);}

/*	if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
		{
			for(int index = 0; index <= numberofslices-1; index++)
				{
					logmsg("%lf\n", weight[index]);
				}
			//logmsg("parameters.weight = %lf\n",parameters.weight);
		}*/

	// Set the groundwater table at each slice

	// Check if groundwater table is above the base of the slip surface or there is more than one slice
//	if (Abank->groundwatertable > basepoint.elevation && numberofslices > 1)
	// I removed this line otherwise the groundwater level for unsaturated blocks is not defined


//		{// First set the intersect of groundwater table and bank profile
			intersectpoint = SetMargin(Abank->nodes,Abank->groundwatertable,top,toe,-Abank->direction) + Abank->direction;

			if (intersectpoint < 0) 
				{
				logmsg("Groundwater table and bank intersect error");
				exit(EXIT_FAILURE);
				}

			watertablemargin = NodeAtZposition(Abank->nodes[intersectpoint-Abank->direction],Abank->nodes[intersectpoint], Abank->groundwatertable);

			for(slice=0; slice<=numberofslices-1; slice++)   //FORISSUE
				{
					//if (Abank.direction*watertablemargin.station > Abank.direction*banksliceintersect[slice+1].station)
					//	{
					watertable[slice] = 0.5*(banksliceintersect[slice].elevation+ banksliceintersect[slice+1].elevation);
					/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
					{logmsg("WT = %lf\n", watertable[slice]);}	*/				
					watertable[slice] = min(watertable[slice],Abank->groundwatertable);
					/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
					{logmsg("WT = %lf\n", watertable[slice]);}*/
					//	}// End If
				}// End For
//		}
	
	/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79) 
		{
			for(int index = 0; index <= numberofslices-1; index++)
				{
					logmsg("WT = %lf\n", watertable[index]);
				}
		}*/

	// Compute pore-water force on slice base
	// Calculate from specified water table																														// {
    for (slice=0; slice<=numberofslices-1; slice++)
		{
			layer = ( slice + multiplier ) / multiplier - 1;		//Changed from the original CONCEPTS
			if (failuresliceintersect[slice+1].elevation >= watertable[slice]) 
				{// Slice base is surrounded with entirely unsaturated soil
					if (data->bankfailureanalysis[2] == true) //BTEST(bankfailureanalysis,2)
						{
							pore[slice]=9.80665*migdata->channel.WaterDensity*(watertable[slice]-0.5*(failuresliceintersect[slice].elevation+failuresliceintersect[slice+1].elevation))*length[slice]*tan(Abank->material[layer].phib*3.14159/180);
						}
				}
			else if (failuresliceintersect[slice].elevation <= watertable[slice])
				{// Slice base is surrounded with entirely saturated soil
					if (data->bankfailureanalysis[0] == true) //BTEST(bankfailureanalysis,0)
						{
							pore[slice]=9.80665*migdata->channel.WaterDensity*(watertable[slice]-0.5*(failuresliceintersect[slice].elevation+failuresliceintersect[slice+1].elevation))*length[slice]*tan(Abank->material[layer].reposeangle*3.14159/180);
						}
				}
			else
				{// Slice base is surrounded with partly saturated soil
					if(data->bankfailureanalysis[0] == true) //BTEST(bankfailureanalysis,0)
						{
							pore[slice] = 0.5*9.80665*migdata->channel.WaterDensity*pow((watertable[slice]-failuresliceintersect[slice+1].elevation),2)*tan(Abank->material[layer].reposeangle*3.14159/180)/sin(angle*3.14159/180);
						}
					if(data->bankfailureanalysis[2] == true) //BTEST(bankfailureanalysis,2)
						{
							pore[slice] = pore[slice]-0.5*9.80665*migdata->channel.WaterDensity*pow((failuresliceintersect[slice].elevation - watertable[slice]),2)*tan(Abank->material[layer].phib*3.14159/180)/sin(angle*3.14159/180);
						}
				}
		}// End For
																														
/*if(inode == 0)
{
	for(int index = 0; index <= numberofslices-1; index++)
		{
			logmsg("pore %lf ", pore[index]);
		}
}*/


	// Compute hydrostatic force and add weight of water to slice weights
	//if(inode == data->MonitorNode)printf("ADD WATER WEIGHT stage = %lf\n",stage);//MMMMTEMP Printing
	hydro = 0.;
	if (data->bankfailureanalysis[1] == true) //BTEST(bankfailureanalysis,1)
		{
			if (stage > basepoint.elevation && stage < Abank->nodes[top].elevation) 
				{
					hydro = 0.5*9.80665*migdata->channel.WaterDensity*pow((stage-basepoint.elevation),2);
					//if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79){logmsg("hydro = %lf\n",hydro);} 
					intersectpoint = SetMargin(Abank->nodes,stage,top,toe,-Abank->direction) + Abank->direction;
					
					if (intersectpoint < 0) 
						{
							logmsg("Water surface and bank intersect error");
							exit(EXIT_FAILURE);
						}
		
					flowmargin = NodeAtZposition(Abank->nodes[intersectpoint-Abank->direction],Abank->nodes[intersectpoint],stage);

					//if(inode == data->MonitorNode)for (int counter = 0; counter<= numberofslices-1; counter++)printf("weight (before) slice %d = %lf\n",counter,weight[counter]); //MMMMTEMP Printing
					//if(inode == data->MonitorNode)printf("ADD WATER WEIGHT Call routine (inode = %d)\n",inode);//MMMMTEMP Printing
					AddWaterWeight(Abank,toe,top,banksliceintersect,flowmargin,numberofslices,weight,migdata,inode,data->MonitorNode); //MMMMTEMP Added last two int's
					//if(inode == data->MonitorNode)for (int counter = 0; counter<= numberofslices-1; counter++)printf("weight (after) slice %d = %lf\n",counter,weight[counter]); //MMMMTEMP Printing
				}
		}

/*if(inode == 0)
{
			logmsg("hydro %lf ", hydro);
}*/
	parameters.waterweight = 0.0;		//ADDITION MAR 2010

	for (int counter = 0; counter<= numberofslices-1; counter++)
		{
			parameters.waterweight = parameters.waterweight + weight[counter];
		}

	parameters.waterweight = parameters.waterweight - parameters.weight;

	/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
	{logmsg("parameters.waterweight = %lf\n",parameters.waterweight);}*/

	// Check if hydrostatic force is larger than the weight of the failure block. If so, return, the streambank is stable.
	if (hydro > 0.9 * tan(angle*3.14159/180) * (parameters.weight + parameters.waterweight)) 
		{
			return 999999.;
		}

	else
		{
			// Check if pore-water pressures are larger than the weight.  This is not possible.
			for (int counter = 0; counter<= numberofslices-1; counter++)
				{
					if (pore[counter] > 0.5*weight[counter])
						{
							pore[counter] = 0.5*weight[counter];
							//logmsg("Pore-water pressure larger than the weight for slice \n");
						}
				}

			// This portion of the code, present in the original CONCEPTS code, is commented out, since in this code we assume that the bank is never submerged (remember there is no floodplain, all is bank
			// so it is not possible that stage >= Abank.nodes[top].elevation) 
			// Override factor of safety calculation if the bank is submerged   
			// if (stage >= Abank.nodes[top].elevation) 
			//		{
			//			SliceWeight(Abank,banksliceintersect,failuresliceintersect,numberofslices,multiplier,weight,1);
			//			parameters.weight = sum(weight);
			//			parameters.waterweight = 0.0;
			//			pore = 0.0; 
			//			hydro = 0.0;
			//		}

			// Iterate to compute factor of safety

			// First iteration, neglect interslice forces to compute normal force for each slice
			totalnormal = 0.;
			for (slice=0;slice<=numberofslices-1;slice++)
				{    
					layer = ( slice + multiplier ) / multiplier - 1;		//Changed from the original CONCEPTS
					normal[slice] = weight[slice] * cos(angle*3.14159/180);
					totalnormal = totalnormal + normal[slice] * tan(Abank->material[layer].reposeangle*3.14159/180);
					//if(inode == data->MonitorNode){logmsg("normal[%d] (FIRST ITER) = %lf\n", slice, normal[slice]);}
				}
			
			//if(inode == data->MonitorNode){logmsg("Abank->material[layer].reposeangle = %lf, angle = %lf, totalnormal (FIRST ITER) = %lf\n", Abank->material[layer].reposeangle, angle, totalnormal);}
			//if(inode == data->MonitorNode){logmsg("totalnormal (FIRST ITER) = %lf\n", totalnormal);}
			
			//normal[numberofslices-1] = normal[numberofslices-1] + hydro * sin(angle*3.14159/180);  //Not used    //DM-FEB2011 Commented out
			layer = ( numberofslices - 1 + multiplier ) / multiplier - 1;		//Changed from the original CONCEPTS
			totalnormal = totalnormal + hydro * sin(angle*3.14159/180) * tan(Abank->material[layer].reposeangle*3.14159/180);

			
			/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
			{logmsg("totalnormal = %lf\n",totalnormal);}*/

			sumcohesion = 0.0;
			sumpore = 0.0;
			sumweight = 0.0;
			for (slice=0;slice<=numberofslices-1;slice++)
				{    
					sumcohesion = sumcohesion + cohesion[slice];
					sumpore = sumpore + pore[slice];
					sumweight = sumweight + weight[slice];
				}
			
			/*if(inode == data->MonitorNode&&basepoint.elevation <= 0.81&&basepoint.elevation >= 0.79)
			{logmsg("sumcohesion = %lf	sumpore = %lf	sumweight = %lf\n",sumcohesion,sumpore,sumweight);} */

			//if(inode == data->MonitorNode) {logmsg("sumweight = %lf\n", sumweight);}
			fosnumerator = max( ( sumcohesion-sumpore + totalnormal ) / cos(angle*3.14159/180) , 0.001 );
			fosdenominator = max( sumweight * tan(angle*3.14159/180) - hydro , 1. );
			FactorOfSafety = fosnumerator / fosdenominator;
			//if(inode == 0){logmsg("FOS FIRST ITER = %lf \n", FactorOfSafety);}

			//if(inode == data->MonitorNode){logmsg("angle = %lf, fosnumerator = %lf, fosdenominator = %lf, FOS (FIRST ITER) = %lf\n", angle, fosnumerator, fosdenominator, FactorOfSafety);}

			// Initialize parameters that remain constant within the iterative process
			//internormal[0]= 0.0;						// Observe that in CONCEPTS, there is internormal[-1] = 0.0 and intershear[-1] = 0.0, instead here 
			//intershear[0]= 0.0;						// I decided to modify the expression of internormal at slice = 0 to account for that (see code below)
			internormal[numberofslices-1]= 0.0;
			intershear[numberofslices-1]= 0.0;
			// Set force function
			rdistance = 0.0 ; 
			
			sumlength = 0.0;
			for(slice=0;slice<=numberofslices-1;slice++)   //FORISSUE
				{
					sumlength = sumlength + length[slice];
				}	
			sliplength = cos(angle*3.14159/180)*sumlength; 
			
			for (slice=0;slice<=numberofslices-1;slice++)  //FORISSUE
				{
					rdistance = rdistance + length[slice] * cos(angle*3.14159/180);
					forcef[slice] = sin(3.14159 * rdistance / sliplength);
					//if(inode == 0){logmsg("slice = %d, forcef[slice]=%lf\n", slice, rdistance / sliplength);}
					//if(inode == data->MonitorNode){logmsg("slice = %d, rdistance = %lf, sliplength = %lf, forcef[slice]=%lf\n", slice, rdistance, sliplength, forcef[slice]);}
				}

			error = 9999.;
			iteration = 0; 
			slicechange = true;

			if (data->ANALYSISMETHOD != ORDINARY)	// I put "if (data->ANALYSISMETHOD != ORDINARY)" 
													// before the iterative search of FS (while cycle) 
													// since I do not have internormal and intershear forces
													// for ORDINARY method, therefore totalnormal
													// is not modified (somehow, misteriously, it is modified 
													// by the code with no apparent reason, if I put 
													// first the while cycle and then "if (data->ANALYSISMETHOD != ORDINARY)" ) 		
				{
					while ((error > data->accuracy && iteration < data->maxiter) || slicechange)
						{

							// Second iteration and up
							slicechange = false;

							// Compute interslice forces, beginning with normal force
							for (slice=firstslice;slice<=numberofslices-2;slice++)
								{
									layer = ( slice + multiplier ) / multiplier-1;				//Changed from the original CONCEPTS
											
									if (slice == 0)
										{
											internormal[slice] = 0 + (sin(angle*3.14159/180)-cos(angle*3.14159/180)*tan(Abank->material[layer].reposeangle*3.14159/180)/FactorOfSafety)*normal[slice]-cos(angle*3.14159/180)*(cohesion[slice]-pore[slice])/ FactorOfSafety;
										}
									else
										{
											internormal[slice] = internormal[slice-1] + (sin(angle*3.14159/180)-cos(angle*3.14159/180)*tan(Abank->material[layer].reposeangle*3.14159/180)/FactorOfSafety)*normal[slice]-cos(angle*3.14159/180)*(cohesion[slice]-pore[slice])/ FactorOfSafety;
										}						
									if(inode == data->MonitorNode) {logmsg("internormal[%d] BEFORE = %lf\n", slice, internormal[slice]);} //MMMMTEMP
									if(chktensioncrack && internormal[slice] < 0.0) {internormal[slice] = 0.0;}
									if(inode == data->MonitorNode) {logmsg("internormal[%d] AFTER = %lf\n", slice, internormal[slice]);} //MMMMTEMP
								}
								
							// Check if assumed tension crack can be shifted upvalley
							if (chktensioncrack && internormal[firstslice] > 0. && data->tensioncrackoption > 0.)
							{										
									// Decrement firstslice
									if (firstslice > 0) 
										{
											firstslice = firstslice - 1;
											slicechange = true;
										}
									//internormal[firstslice-1] = 0.0;

									// Reset force function
									for (slice=0;slice<=numberofslices-1;slice++)
										{
											forcef[slice] = 0.0;
										} 

									rdistance = 0.0;
									
									sumlength = 0.0;	
									for (slice=firstslice;slice<=numberofslices-1;slice++)
										{
											sumlength = sumlength + length[slice];		
										} 		
									sliplength = cos(angle*3.14159/180) * sumlength;
									
									for(slice=firstslice; slice<=numberofslices-1;slice++)  //FORISSUE
										{
											rdistance = rdistance + length[slice] * cos(angle*3.14159/180);
											forcef[slice] = sin(3.14159 * rdistance / sliplength);
										}

									// The following is done in the original CONCEPTS, not here
									// Recalculate the anchoring force due to roots
									//	if (doriparian != 0 && Abank.hasbuffer > 0)
									//	{
									//    REMM_RootAnchorForce(Abank.hasbuffer,failuresliceintersect[firstslice].elevation,anchorforce);
									//	}
									
								}

							// Check for negative interslice normal forces
							if (!chktensioncrack)
								{
									// Set chktensioncrack flag to TRUE
									chktensioncrack = true; 
									slicechange = true;

									// Find last slice boundary where there is tension
									for (int slice=0; slice<=numberofslices-1;slice++)
										{ 
											storingslice = slice;
											if (internormal[slice] >= 0.) {break;}
										}
									slice = storingslice;
									

									if (slice > 0) 
										{
											//There is tension between the slices.
											firstslice = slice - 1;
											//if(inode == data->MonitorNode){logmsg("FIRSTSLICE = %d\n", firstslice);}
											
											// I avoid this since, differently from CONCEPTS, we do not distinguish between bakn and floodplain
											// Check if slice face intersects the bank face.  If it does track back to find
											// the first slice from the bank top that intersects the floodplain.
											//for (int slice=firstslice;slice>=0;slice--)
											//	{
											//		if (Abank->direction*banksliceintersect[slice].station > Abank->direction*Abank->nodes[top].station) {break;}
											//	}
											//firstslice = slice;
											//if (firstslice == -1) {firstslice = 0;}*/

											// Check if slice face height is larger than the maximum allowable tension crack
											// depth.  If it is, track back to find the first slice with a face height
											// smaller than the maximum tension crack depth.

											//if(inode == data->MonitorNode){logmsg("maxtensioncrackdepth = %lf\n", maxtensioncrackdepth);}
											for(int slice=firstslice;slice>=0;slice--)  
												{
													storingslice = slice;
													//if(inode == data->MonitorNode){logmsg("slice = %d, distance = %lf\n", slice, (banksliceintersect[slice].elevation-failuresliceintersect[slice].elevation));}			
													if ((banksliceintersect[slice].elevation-failuresliceintersect[slice].elevation) <= maxtensioncrackdepth) {break;}
												}
											firstslice = storingslice;
											//if(inode == data->MonitorNode){logmsg("FIRSTSLICE = %d\n", firstslice);}
											//if (firstslice == -1) {firstslice = 0;}   //Not necessary

											// Check again if hydrostatic force is greater than weight of failure block. If so, return, streambank is stable.
											totalweight = 0.0;
											for(int slice=firstslice;slice<=numberofslices-1;slice++)
												{
													totalweight = totalweight + weight[slice];
												}
											if (hydro > 0.9 * tan(angle*3.14159/180) * totalweight) {return 999999.;}
											//if(inode == data->MonitorNode)printf("+++++++++++++++ \n");//MMMMTEMP Printing
											// If first slice is greater than zero, there is a tension crack.  In this case set tensioncrack to TRUE.
											if (firstslice > 0) 
												{
													tensioncrack = true;
														
													// Reset interslice normal forces, normal forces, and recalculate factor of safety	
													for(slice=0;slice<=numberofslices;slice++)  //FORISSUE
														{													
															internormal[slice] = 0. ; 
															normal[slice] = 0. ;
														}
													totalnormal = 0.;
												
													for(slice=firstslice;slice<=numberofslices-1;slice++)  //FORISSUE
														{
															layer = ( slice + multiplier ) / multiplier-1;
															normal[slice] = weight[slice] * cos(angle*3.14159/180);
															totalnormal = totalnormal + normal[slice] * tan(Abank->material[layer].reposeangle*3.14159/180);
														}
																						
													//normal[numberofslices-1] = normal[numberofslices-1] + hydro * sin(angle*3.14159/180);  //Not used

													layer = ( numberofslices-1 + multiplier ) / multiplier-1;			//DM-FEB2011 Previous version layer = ( slice + multiplier ) / multiplier-1 was wrong

													totalnormal = totalnormal + hydro * sin(angle*3.14159/180) * tan(Abank->material[layer].reposeangle*3.14159/180);

													fosnumerator = 0.0;
													fosdenominator = 0.0;
													for(slice=firstslice;slice<=numberofslices-1;slice++)		//FORISSUE
														{
															fosnumerator = fosnumerator + cohesion[slice] - pore[slice];
															fosdenominator = fosdenominator + weight[slice];
														}
													fosnumerator = max( cos(angle*3.14159/180) * ( fosnumerator + totalnormal ) , 0.001 );
													fosdenominator = max( fosdenominator * tan(angle*3.14159/180) - hydro , 1. );
													FactorOfSafety = fosnumerator / fosdenominator;

													// Reset force function
													for(slice=0;slice<=numberofslices-1;slice++)			//FORISSUE
														{
															forcef[slice] = 0.0 ; 
														}	
													rdistance = 0.0; 
													sumlength = 0.0;
													for(slice=firstslice;slice<=numberofslices-1;slice++)//FORISSUE
														{
															sumlength = sumlength + length[slice];
														}	
													sliplength = cos(angle*3.14159/180)*sumlength; 

													for (slice=firstslice;slice<=numberofslices-1;slice++)
														{
															rdistance = rdistance + length[slice] * cos(angle*3.14159/180);
															forcef[slice] = sin(3.14159 * rdistance / sliplength);
														}

													// The following is done in the original CONCEPTS, not here
													// Calculate the anchoring force due to roots
													//	if (doriparian != 0 && Abank.hasbuffer > 0) 
													//	{REMM_RootAnchorForce(Abank.hasbuffer,failuresliceintersect[firstslice].elevation,anchorforce);}
													// Restart the iteration process
													//	CYCLE

												}

										}	//End if (slice > 0) 

								}   //End if (!chktensioncrack)         

							//! Interslice shear force
							if (data->ANALYSISMETHOD == MORGENSTERNPRICE) 
								{
									for (slice=firstslice; slice<=numberofslices-2;slice++)
										{
											intershear[slice] = data->lambda * forcef[slice] * internormal[slice];
											//if(inode == data->MonitorNode) {logmsg("firstslice= %d, numberofslices = %d\n", firstslice, numberofslices);}//MMMMTEMP
											//if(inode == data->MonitorNode) {logmsg("Computed intershear[%d] = %lf, with internornal [%d] = %lf\n", slice, intershear[slice],slice, internormal[slice]);}//MMMMTEMP			
										}
								}

							if (data->ANALYSISMETHOD == JANBUSIMPLE)			//Addition MAR2010
								{
									for (slice=firstslice; slice<=numberofslices-2;slice++)
										{
											intershear[slice] = 0.0;		
										}
								}
							
							//! Compute normal force at base of slice
							totalnormal = 0.0;
							for (slice=firstslice; slice<=numberofslices-1;slice++)
								{//if(inode==0){logmsg("slice %d - ",firstslice);}
									layer = ( slice + multiplier ) / multiplier-1;				//Changed from the original CONCEPTS
									//if(inode == data->MonitorNode){logmsg("intershear[%d] = %lf\n", slice, intershear[slice]-intershear[slice-1]);}
									normal[slice] = (weight[slice]-(intershear[slice]-intershear[slice-1])-(cohesion[slice]-pore[slice])*sin(angle*3.14159/180)/FactorOfSafety)/(cos(angle*3.14159/180)+tan(Abank->material[layer].reposeangle*3.14159/180)*sin(angle*3.14159/180)/FactorOfSafety);
									//if(inode == data->MonitorNode){logmsg("normal[%d] = %lf\n", slice, normal[slice]);}
									if (normal[slice] < 0.0) {normal[slice] = weight[slice] * cos(angle*3.14159/180);}
									//if(inode == data->MonitorNode){logmsg("normal[%d] (IN ITERATIONS) = %lf\n", slice, normal[slice]);}
									totalnormal = totalnormal + normal[slice] * tan(Abank->material[layer].reposeangle*3.14159/180);
									if(inode == data->MonitorNode)logmsg("LOOP %d	inters[s] = %lf, inters[s-1] = %lf \n",slice,intershear[slice],intershear[slice-1]);//MMMMTEMP
								}
							//if(inode==0){logmsg("totalnormal %lf \n",totalnormal);}
							//if(inode == data->MonitorNode){logmsg("totalnormal (IN ITERATIONS) = %lf\n", totalnormal);}
							
							// Compute factor of safety
							fosnumerator = 0.0;
							for (slice=firstslice; slice<=numberofslices-1;slice++)
								{
									//if(inode == data->MonitorNode){logmsg("pore[%d] (IN ITERATIONS) = %lf\n", slice, pore[slice]);}
									fosnumerator = fosnumerator+cohesion[slice]-pore[slice];
								}
							
							
							/*for (slice=firstslice; slice<=numberofslices-1;slice++)
								{
									if(inode == data->MonitorNode){logmsg("cohesion[%d] (IN ITERATIONS) = %lf\n", slice, cohesion[slice]);}
								}*/
			
							fosnumerator = (fosnumerator + totalnormal)/cos(angle*3.14159/180);
							
							fosnumerator = max(fosnumerator, 0.001);

							fosdenominator = 0.0;
							for (slice=firstslice; slice<=numberofslices-1;slice++)
								{
									fosdenominator = fosdenominator+weight[slice];
									}

							fosdenominator = fosdenominator* tan(angle*3.14159/180)- (hydro + anchorforce);			// Observe that anchorforce is set equal to zero in the code
							fosdenominator = max(fosdenominator, 1.0);

							guess = fosnumerator / fosdenominator;
							
							// Check if factor of safety has converged
							error = fabs((guess - FactorOfSafety)/guess);
							FactorOfSafety = guess ; 
							//if(inode==0){logmsg("FactorOfSafety = NUM = %lf DEN = %lf Value = %lf \n",fosnumerator , fosdenominator, FactorOfSafety);}
							//if(inode == data->MonitorNode){logmsg("angle = %lf, fosnumerator = %lf, fosdenominator = %lf, FOS = %lf, error = %lf\n",  angle, fosnumerator, fosdenominator, FactorOfSafety, error);}
							//if(inode == data->MonitorNode){logmsg("FOS = %lf, error = %lf\n",  FactorOfSafety, error);}
							iteration = iteration + 1;

							//if(inode == data->MonitorNode)printf("iteration = %d, FactorOfSafety = %lf, error = %lf\n",iteration,FactorOfSafety,error);//MMMMTEMP Printing

						}				//End while
				
				}//End if (ANALYSISMETHOD != ORDINARY)

			//if(inode == data->MonitorNode)printf("FactorOfSafety = %lf,parameters.factorofsafety = %lf\n",FactorOfSafety,parameters.factorofsafety); //MMMMTEMP Printing
			//! Set failure parameters
			parameters.tensioncrack = tensioncrack;
			parameters.apparentcohesion = 0.0;
			for (slice=firstslice; slice<=numberofslices-1;slice++)
				{
					parameters.apparentcohesion = parameters.apparentcohesion + cohesion[slice]-pore[slice];
				}
			sumlength = 0.0;
			for (slice=firstslice; slice<=numberofslices-1;slice++)
				{
					sumlength = sumlength + length[slice];
				}
			parameters.apparentcohesion = parameters.apparentcohesion/sumlength;
			parameters.factorofsafety = FactorOfSafety;
			parameters.hydrostaticforce = hydro;
			parameters.mobilizedshear = 0.0;
			for (slice=firstslice; slice<=numberofslices-1;slice++)
				{
					parameters.mobilizedshear = parameters.mobilizedshear + cohesion[slice]-pore[slice];
				}
			parameters.mobilizedshear = (parameters.mobilizedshear+ totalnormal)/FactorOfSafety;
			parameters.normalforce = 0.0;
			for (slice=firstslice; slice<=numberofslices-1;slice++)
				{
					parameters.normalforce = parameters.normalforce + normal[slice];
				}
			parameters.porewaterforce = 0.0;
			for (slice=firstslice; slice<=numberofslices-1;slice++)
				{
					parameters.porewaterforce = parameters.porewaterforce + pore[slice];
				}
			parameters.porewaterforce = parameters.porewaterforce / Abank->material[0].reposeangle;
			if (tensioncrack)
				{
					parameters.bottomvalleypoint = failuresliceintersect[firstslice];
					parameters.topvalleypoint = banksliceintersect[firstslice];
				}
			else
				{
					parameters.bottomvalleypoint = failuresliceintersect[0];
					parameters.topvalleypoint = banksliceintersect[0];
				}
			parameters.weightslices = 0.0;													//MMMMTEMP Computation of weightslices
			for (slice=firstslice; slice<=numberofslices-1;slice++)							//MMMMTEMP Computation of weightslices
				{																			//MMMMTEMP Computation of weightslices
					parameters.weightslices = parameters.weightslices + weightstored[slice];//MMMMTEMP Computation of weightslices
				}																			//MMMMTEMP Computation of weightslices
			parameters.angle = angle; 														//MMMMTEMP Failure angle

		}


		if(Abank->failureparameters.factorofsafety > FactorOfSafety)			//MMMMTEMP Modification to get minimum factor of safety
			{																	//MMMMTEMP Modification to get minimum factor of safety
				Abank->failureparameters = parameters;							//MMMMTEMP Modification to get minimum factor of safety
				//if(inode == data->MonitorNode)printf("New minimum factor of safety\n");//MMMMTEMP Printing
				//if(inode == data->MonitorNode)printf("Factor of Safety = %lf\n",FactorOfSafety);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)for (slice=firstslice; slice<=numberofslices-1;slice++)printf("%d	%lf	%lf	%lf	%lf	%lf\n",slice,weightstored[slice],failuresliceintersect[slice].station,failuresliceintersect[slice].elevation,banksliceintersect[slice].station,banksliceintersect[slice].elevation);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)printf("%d	%lf	%lf	%lf	%lf	%lf\n",numberofslices,0.000000,failuresliceintersect[numberofslices].station,failuresliceintersect[numberofslices].elevation,banksliceintersect[numberofslices].station,banksliceintersect[numberofslices].elevation);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)for (slice=firstslice; slice<=numberofslices-1;slice++)printf("%d	%lf water weight on slice\n",slice,weight[slice]-weightstored[slice]);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)printf("%d	%lf water weight on slice\n",numberofslices,0.000000);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)for (slice=0; slice<=numberofslices-1;slice++)printf("%d	%lf	%lf	%lf	%lf	%lf\n",slice,weight[slice],failuresliceintersect[slice].station,failuresliceintersect[slice].elevation,banksliceintersect[slice].station,banksliceintersect[slice].elevation);//MMMMTEMP Printing
				//if(inode == data->MonitorNode)printf("%d	%lf	%lf	%lf	%lf	%lf\n",numberofslices,0.000000,failuresliceintersect[numberofslices].station,failuresliceintersect[numberofslices].elevation,banksliceintersect[numberofslices].station,banksliceintersect[numberofslices].elevation);//MMMMTEMP Printing
			}																	//MMMMTEMP Modification to get minimum factor of safety

   

	return FactorOfSafety;
}

Coordinates RMBankErosion::NodeAtFraction(const Coordinates& node1, const Coordinates& node2, double fraction)
{
	Coordinates result;

    result.elevation = node1.elevation + ( node2.elevation - node1.elevation ) * fraction;
    result.station = node1.station + ( node2.station - node1.station ) * fraction;

	return result;
}

int RMBankErosion::SetMarginY(const vector<Coordinates>& nodes, double station, int left, int right, int direction)
{
	int result;

	// Compute node number of margin
	result = left;
    while ((direction*nodes[result].station <= direction*station) && (direction*result < direction*right))
		{
			result = result + direction;
		}

	// Check whether node number is correct


	if (direction*nodes[result].station < direction*station)
		{
			//logmsg("Unable to set margin in function SetMarginY \n");
			//exit(EXIT_FAILURE);
		}

	return result;
}

Coordinates RMBankErosion::NodeAtYposition(const Coordinates& node1, const Coordinates& node2, double y)
{
	Coordinates result;

    result.station = y;
    result.elevation = node1.elevation + Slope(node1,node2) * ( y - node1.station );
	
	return result;
}

void RMBankErosion::AddWaterWeight(Bank *Abank, int toe, int top, Coordinates bankintersects[], Coordinates flowmargin, int numberofslices, double weight[], RMMigrationData* migdata, int inode, int monitornode)  //MMMMTEMP Added last two int's
{
	int point, slice, vertex;
	vector<Coordinates> nodeworkspace;
    nodeworkspace.resize(max(Abank->numberofnodes, 2000)); //NOO //DM-FEB2011 I changed 50 to 2000
	
	//if(monitornode == inode)printf("Abank->direction*flowmargin.station = %lf, Abank->direction*flowmargin.elevation = %lf\n", Abank->direction*flowmargin.station,Abank->direction*flowmargin.elevation);//MMMMTEMP Printing
	//if(monitornode == inode)for(int k = 0; k <= Abank->numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,Abank->nodes[k].station,Abank->nodes[k].elevation);//MMMMTEMP Printing
	// Loop over slices to determine weight of water on each slice
	for(slice=0;slice<=numberofslices-1;slice++)   //FORISSUE
		{
			// Check if slice contains water
			if (Abank->direction*flowmargin.station > Abank->direction*bankintersects[slice+1].station) 
				{
					// Initialization
					vertex = -1;
					// Set valley-side vertices of soil element
					if (Abank->direction*flowmargin.station > Abank->direction*bankintersects[slice].station) 
						{
							// Slice is fully submerged, two vertices: intersects of valley side of slice with bank profile and water surface
							nodeworkspace[vertex+1] = bankintersects[slice];
							nodeworkspace[vertex+2].station = bankintersects[slice].station;
							nodeworkspace[vertex+2].elevation = flowmargin.elevation;
							vertex = vertex + 2; 
						}
					else
						{
							// Slice is partly submerged, one vertex: intersect of bank surface and watersurface
							nodeworkspace[vertex+1] = flowmargin;
							vertex = vertex + 1;
						} 

					// Set top stream-side vertex: intersect of water surface and stream side of slice
					nodeworkspace[vertex+1].station = bankintersects[slice+1].station;
					nodeworkspace[vertex+1].elevation = flowmargin.elevation;
					vertex = vertex + 1;
		
					// Set bottom stream-side vertex of water element on slice: intersect of bank profile and stream side of slice
					nodeworkspace[vertex+1] = bankintersects[slice+1];
					vertex = vertex + 1;

					// Set bottom vertices of water element in case bank profile contains additional points
					if (Abank->direction*bankintersects[slice+1].station < Abank->direction*Abank->nodes[top].station)
						{
							// Find first node that may possibly be added to the polygon
							point = toe;
							while (Abank->nodes[point].elevation <= nodeworkspace[vertex].elevation)
								{point = point + Abank->direction;}

							// Add bank nodes to polygon until a node's elevation equals or falls above the elevation of the intersect of bank profile with the slice's valley side
							while (Abank->nodes[point].elevation < nodeworkspace[0].elevation && Abank->direction*point < Abank->direction*top)
								{
									nodeworkspace[vertex+1] = Abank->nodes[point];
									// Increment vertex and increment bank profile index
									vertex = vertex + 1;
									point = point + Abank->direction;
								}
						}

					
					// Compute area of polygon and add to weight of slice
					weight[slice] = weight[slice] + 9.80665 * migdata->channel.WaterDensity * PolygonArea(nodeworkspace,vertex+1);			// Recall that vertices are numbered from zero
					//if(inode == monitornode)printf("ADDWATERWEIGHT %d %lf\n",slice,PolygonArea(nodeworkspace,vertex+1));//MMMMTEMP Printing
				}// End If
		

		}// End For
	
}


void RMBankErosion::SliceWeight(Bank *Abank, Coordinates bankintersects[], Coordinates failureintersects[], int numberofslices, int multiplier, double weight[],int issubmerged, RMMigrationData* migdata, int inode, int monitornode)  //MMMMTEMP Added monitornode as argument
{
	//if(inode == 210)logmsg("HERE\n",inode);//MMMMTEMP Printing
	//if(inode == 210)for(int p = 0; p<=Abank->numberofnodes-1; p++)logmsg("TTT	%d	%lf	%lf\n",p,Abank->nodes[p].station,Abank->nodes[p].elevation);//MMMMTEMP Printing
    int ierr, layer, numberoflayers, oldsize, point, slice, subslice, vertex, top;
    bool intersectsbank;
    Coordinates streamvertex, valleyvertex;
	vector<Coordinates> nodeworkspace;
    nodeworkspace.resize(max(Abank->numberofnodes, 2000)); //NOO  //DM-FEB2011 I changed 50 to 2000
// if(inode == 7)printf("SLICEWEIGHT ROUTINE START\n");//MMMMTEMP Printing	
	// Determine top index
	if(Abank->direction == -1)
		{
			top = 0;
		}	
	else if(Abank->direction == 1)
		{
			top = Abank->numberofnodes - 1;
		}

	// Initialize weight of slices
	for (slice=0; slice<=numberofslices-1; slice++)			 
		{
			weight[slice] = 0.0;
		}
	
	//if(inode == 9)for(int p = 0; p <= Abank->numberofnodes-1; p++)printf("SLICEWEIGHT ROUTINE BANK %d	%lf	%lf\n",p,Abank->nodes[p].station,Abank->nodes[p].elevation);//MMMMTEMP Printing
	//if(inode == 9)for (int p = 0; p<=numberofslices; p++)printf("SW R SLICE %d	%lf	%lf	%lf	%lf\n",p,bankintersects[p].station,bankintersects[p].elevation,failureintersects[p].station,failureintersects[p].elevation);//MMMMTEMP Printing
	if(inode == 210)for (int p = 0; p<=numberofslices; p++)logmsg("SW R SLICE %d	%lf	%lf	%lf	%lf\n",p,bankintersects[p].station,bankintersects[p].elevation,failureintersects[p].station,failureintersects[p].elevation);//MMMMTEMP Printing
	// Loop over slices to determine weight of each slice
	for (slice=0; slice<=numberofslices-1; slice++)			 // sliceloop
		{//if(inode == 7)printf("SLICEWEIGHT ROUTINE slice = %d of number of slices = %d\n",slice,numberofslices);//MMMMTEMP Printing	
			numberoflayers = ( slice + multiplier ) / multiplier;		// Changed from the original CONCEPTS
			subslice = slice - ( numberoflayers - 1 ) * multiplier;

			// Loop over layers to determine weight of each soil element in a slice
			for(layer = numberoflayers-1; layer>=0; layer--)				 // layerloop      //FORISSUE
				{
					// Initialization
					vertex = -1;
					intersectsbank = false;

					// Set bottom vertices of soil element
					if (layer == numberoflayers-1 && subslice == 0) 
						{// Bottom, valley-side soil element, one vertex: intersect of failure surface and stream side of soil element
							nodeworkspace[vertex+1] = failureintersects[slice+1];
							vertex = vertex + 1;  
						}
					else if (layer == numberoflayers-1) 
						{// Bottom, most stream-side soil elements, two vertices: intersects of failure surface and slice sides
							nodeworkspace[vertex+1] = failureintersects[slice+1];
							nodeworkspace[vertex+2] = failureintersects[slice];
							vertex = vertex + 2; 
						}
					else 
						{// Two vertices: stream- and valley-side top vertices of soil element below
							nodeworkspace[vertex+1] = streamvertex;
							nodeworkspace[vertex+2] = valleyvertex;
							vertex = vertex + 2; 
						}
					//if(inode == monitornode && slice == 0)for (int p=0; p<=vertex;p++)printf("Bottom vertices %d	%lf	%lf\n",p,nodeworkspace[p].station,nodeworkspace[p].elevation);//MMMMTEMP Printing
					// Set top valley-side vertex of soil element
					//if(inode == monitornode && slice == 0)printf("SUBSLICE %d	%d	%d", layer,numberoflayers-1,subslice);//MMMMTEMP Printing
					if (layer == numberoflayers-1 && subslice == 0 && layer != 0) //MMMMTEMP Added third condition   //MMMMTEMP Modification to SliceWeight routine
						{// Bottom, valley-side soil element: intersect of failure surface and valley side of soil element
							valleyvertex = failureintersects[slice];
						}	
					else if (layer == 0 || Abank->material[layer].elevation > bankintersects[slice].elevation)
						{// Top soil element: intersect of bank profile and valley side of soil element
							valleyvertex = bankintersects[slice];
							if (layer != 0 || Abank->direction*bankintersects[slice].station <= Abank->direction*Abank->nodes[top].station || subslice == multiplier ) // CORRECTION MAR 2010 (top instead of top-1) 
								{intersectsbank = true;}
						}
					else
						{// Intersect of valley side of soil element and top of soil layer
							valleyvertex.station = failureintersects[slice].station;
							valleyvertex.elevation = Abank->material[layer].elevation;
						}
					nodeworkspace[vertex+1] = valleyvertex;
					vertex = vertex + 1; 
					//if(inode == monitornode && slice == 0)for (int p=0; p<=vertex;p++)printf("Bot + Top valley vertices %d	%lf	%lf\n",p,nodeworkspace[p].station,nodeworkspace[p].elevation);//MMMMTEMP Printing
				
					// Set top stream-side vertex of soil element.  If intersectsbank has been set TRUE, then there is no top stream-side vertex.
					if (!intersectsbank) 
						{
							if(Abank->direction*Abank->nodes[Abank->material[layer].topindex].station >= Abank->direction*bankintersects[slice+1].station) 
								{// Soil element intersects streambank: intersect of top of layer and streambank
									streamvertex = Abank->nodes[Abank->material[layer].topindex];
									intersectsbank = true;
									if(inode == monitornode && slice == 0)printf("AAAA ---");//MMMMTEMP Printing
								}
							else if (layer == 0)
								{// Top soil element: intersect of floodplain and stream side of soil element
									streamvertex = bankintersects[slice+1];
									if(inode == monitornode && slice == 0)printf("BBBB ---");//MMMMTEMP Printing
								}
							else
								{// Intersect of stream side of soil element and top of soil layer
									streamvertex.station = failureintersects[slice+1].station;
									streamvertex.elevation = Abank->material[layer].elevation;
									if(inode == monitornode && slice == 0)printf("CCCC ---");//MMMMTEMP Printing
								}
							nodeworkspace[vertex+1] = streamvertex;
							//if(inode == monitornode && slice == 0)printf("TOP STREAM SIDE 	%lf	%lf\n",nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation); //MMMMTEMP Printing
							vertex = vertex + 1; 	
							
						}
				
					//if(inode == monitornode && slice == 0)for (int p=0; p<=vertex;p++)printf("Bot + Top valley vertices (2)  %d	%lf	%lf\n",p,nodeworkspace[p].station,nodeworkspace[p].elevation);//MMMMTEMP Printing
					
					// Set stream-side vertices of soil element in case it is part of the bank profile
					if (intersectsbank) 
						{// Find first node that may possibly be added to the polygon
							if (Abank->direction == 1)      //  right bank
								{point = Abank->numberofnodes-1;}
							else if (Abank->direction == -1)		//  left bank
								{point = 0;}	
							//if(inode == monitornode && slice == 0)printf("point 	%d\n",point); //MMMMTEMP Printing
							//while (nodeworkspace[vertex].elevation < Abank->nodes[point].elevation)			// CORRECTION MAR 2010 (<= changed in <)
							while (nodeworkspace[vertex].elevation < Abank->nodes[point].elevation || (nodeworkspace[vertex].station-Abank->nodes[point].station)*Abank->direction < 0.0)			// CORRECTION MAR 2010 (<= changed in <)  //MMMMTEMP Modification to Sliceweight routine
								{
									point = point - Abank->direction;
								}
							//if(inode == monitornode && slice == 0)printf("point 	%d\n",point); //MMMMTEMP Printing
							// Add bank nodes to polygon until a node's elevation equals or drops below the
							//elevation of the intersect of bank profile with the stream side of slice or
							// bottom of soil layer.
							//if(inode == monitornode && slice == 4)printf("pointXXX 	%d	%lf	%lf -- %lf	%lf  ",point,Abank->nodes[point].station,Abank->nodes[point].elevation, nodeworkspace[vertex].station, nodeworkspace[vertex].elevation); //MMMMTEMP Printing
							
							while (Abank->nodes[point].elevation > nodeworkspace[0].elevation && Abank->nodes[point].elevation > bankintersects[slice+1].elevation)//MMMMTEMP Commented out
							//while (Abank->nodes[point].elevation > nodeworkspace[0].elevation && Abank->nodes[point].elevation > bankintersects[slice+1].elevation && (nodeworkspace[vertex].station-Abank->nodes[point].station)*Abank->direction> 0.0) //MMMMTEMP Added another condition to avoid including nodes that do not belong to the block (floodplain node especially) //MMMMTEMP Modification to SliceWeight routine
								{
									nodeworkspace[vertex+1] = Abank->nodes[point];
									//if(inode == monitornode && slice == 4)printf("ADDED NODE ON BANK 	%lf	%lf ",nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation); //MMMMTEMP Printing
									vertex = vertex + 1;
									point = point - Abank->direction;
								}

							if (bankintersects[slice+1].elevation > nodeworkspace[0].elevation) 
								{
									nodeworkspace[vertex+1] = bankintersects[slice+1];
									//if(inode == monitornode && slice == 0)printf("LAST NODE ADDED 	%lf	%lf\n",nodeworkspace[vertex+1].station,nodeworkspace[vertex+1].elevation); //MMMMTEMP Printing
									vertex = vertex + 1;
								}
							intersectsbank = false;  // reset intersectsbank
						}
		
					//if(inode == monitornode && slice == 4)for (int p=0; p<=vertex;p++)printf("Slice vertex %d	%lf	%lf\n",p,nodeworkspace[p].station,nodeworkspace[p].elevation);//MMMMTEMP Printing
					// Compute area of polygon and add to weight of slice
					if (issubmerged == 0) 
						{
							weight[slice] = weight[slice] + Abank->material[layer].unitweight * PolygonArea(nodeworkspace,vertex+1);		// Recall vertices are numbered from zero
						}
					else
						{
							weight[slice] = weight[slice] + (Abank->material[layer].unitweight - 9.80665 * migdata->channel.WaterDensity ) * PolygonArea(nodeworkspace,vertex+1);
						}          

					// Check if this was the topmost soil layer in the slice
					if (Abank->material[layer].elevation > bankintersects[slice].elevation) {break;}
				}// end layerloop
			
		}// end sliceloop   
 //if(inode == 7)printf("SLICEWEIGHT ROUTINE END\n");//MMMMTEMP Printing
	//if(inode == monitornode)for (slice=0; slice<=numberofslices-1; slice++)printf("IN SLICEWEIGHT %d	%lf	--- ",slice,weight[slice]);//MMMMTEMP Printing
}				

int RMBankErosion::CorrectP4Left(double point4, double point1, double point2, double tolerance)
{
	// This function redefines point4 by moving tolerance from point2 to the left. (Not right as said in the comments in CONCEPTS)
	int CorrectP4Left;
	CorrectP4Left = 1;
	point4 = point2 - tolerance;

	// If tolerance was small enough relative to point2 that the machine thinks
	// point4 still equals point2, which is more likely if point2 is large, this
	// could result in a divide fault later.  To avoid this, we redefine point4 as
	// the average of point1 and point2 if necessary.  If we still don't have point4
	// strictly between point1 and point2, we terminate the search.
	if (point4 >= point2)
		{
			point4 = 0.5*(point1+point2);
			if (point4 >= point2)
				{
					CorrectP4Left = 0;
				}
		}
        
    if (point4 <= point1) 
		{
			CorrectP4Left = 0;
		}
        
	return CorrectP4Left;
}

int RMBankErosion::CorrectP4Right(double point4, double point2, double point3, double tolerance)
{
	// This function redefines point4 by moving tolerance from point2 to the right.
	int CorrectP4Right;
    CorrectP4Right = 1;
	point4 = point2 + tolerance;

	// If tolerance was small enough relative to point2 that the machine thinks
	// point4 still equals point2, which is more likely if point2 is large, this
	// could result in a divide fault later.  To avoid this, we redefine point4 as
	// the average of point2 and point3 if necessary.  If we still don't have point4
	// strictly between point2 and point3, we terminate the search.
    if (point4 <= point2)
		{
            point4 = 0.5*(point2+point3);
            if (point4 <= point2)
				{
					CorrectP4Right = 0;
				}
		}

	 if (point4 >= point3) 
		{
			CorrectP4Right = 0;
		}

	return CorrectP4Right;
}



	


Bank RMBankErosion::BankInterpolated(Bank *bank1, Bank *bank2, double DSweight, int inode, double tolerance, int MonitorNode)							//CORRECTION FEB2010	//DM-FEB2011
{
	Bank BankInterpolated;
	int numbernodesbank1, numbernodesbank2, toe1, top1, toe2, top2, numbernodesbankinterpolated, indexbank1, indexbank2;
	double rangestation1, rangestation2, station1, elevation1, station2, elevation2, slope;
	int counter,leadingbank,numberlayersincommon,numberlayersinexcess,excessbank;   												

	if(tolerance <= 0)			//DM-FEB2011 
		{						//DM-FEB2011
			DSweight = 1.0;		//DM-FEB2011
		}						//DM-FEB2011

	if(DSweight != 1.0 && DSweight != 0.0)//DM-FEB2011
		{
			numbernodesbank1 = bank1->numberofnodes;    // Number of nodes of the first bank
			numbernodesbank2 = bank2->numberofnodes;    // Number of nodes of the second bank

			//Station range for both banks
			if(bank1->direction == 1)					// Right bank
				{
					toe1 = 0;
					top1 = numbernodesbank1-1;
					toe2 = 0;
					top2 = numbernodesbank2-1;
					rangestation1 = bank1->nodes[top1].station - bank1->nodes[toe1].station;
					rangestation2 = bank2->nodes[top2].station - bank2->nodes[toe2].station;
				}
			else if (bank1->direction == -1)			// Left left
				{
					toe1 = numbernodesbank1-1;
					top1 = 0;
					toe2 = numbernodesbank2-1;
					top2 = 0;
					rangestation1 = bank1->nodes[toe1].station-bank1->nodes[top1].station;
					rangestation2 = bank2->nodes[toe2].station-bank2->nodes[top2].station;
				}

			
			//Dimensionless stations (from 0 to 1 for both the right and the left bank) 
			for(int i = 0; i <= numbernodesbank1-1; i++)
				{
					bank1->nodes[i].dimensionlessstation = (bank1->nodes[i].station-bank1->nodes[0].station)/rangestation1; 
				}
			for(int i = 0; i <= numbernodesbank2-1; i++)
				{
					bank2->nodes[i].dimensionlessstation = (bank2->nodes[i].station-bank2->nodes[0].station)/rangestation2; 
				}

			//Number of nodes of the interpolated bank
			numbernodesbankinterpolated = numbernodesbank1 + numbernodesbank2 - 2;
            BankInterpolated.SetNumberOfNodes(numbernodesbankinterpolated);

			//Bank end points (master cords)
			BankInterpolated.nodes[0].dimensionlessstation = 0.0;
			BankInterpolated.nodes[numbernodesbankinterpolated-1].dimensionlessstation = 1.0;

			BankInterpolated.nodes[0].station = (1-DSweight)*bank1->nodes[0].station+DSweight*bank2->nodes[0].station;				//CORRECTION FEB2010	(bank1->nodes[0].station+bank2->nodes[0].station)/2;
			BankInterpolated.nodes[0].elevation = (1-DSweight)*bank1->nodes[0].elevation+DSweight*bank2->nodes[0].elevation ;		//CORRECTION FEB2010	(bank1->nodes[0].elevation+bank2->nodes[0].elevation)/2;				

			BankInterpolated.nodes[numbernodesbankinterpolated-1].station = (1-DSweight)*bank1->nodes[numbernodesbank1-1].station+DSweight*bank2->nodes[numbernodesbank2-1].station;				//CORRECTION FEB2010	(bank1->nodes[numbernodesbank1-1].station+bank2->nodes[numbernodesbank2-1].station)/2;
			BankInterpolated.nodes[numbernodesbankinterpolated-1].elevation = (1-DSweight)*bank1->nodes[numbernodesbank1-1].elevation+DSweight*bank2->nodes[numbernodesbank2-1].elevation;			//CORRECTION FEB2010	(bank1->nodes[numbernodesbank1-1].elevation+bank2->nodes[numbernodesbank2-1].elevation)/2;

			//Minor cords
			indexbank1 = 1;
			indexbank2 = 1;
			for(int i = 1; i <= numbernodesbankinterpolated-2; i++)
				{  

					//if(bank1->nodes[indexbank1].dimensionlessstation>bank2->nodes[indexbank2].dimensionlessstation)
					if((bank1->nodes[indexbank1].dimensionlessstation - bank2->nodes[indexbank2].dimensionlessstation) > tolerance)			// CORRECTION MAR2010 
						{	
							BankInterpolated.nodes[i].dimensionlessstation = bank2->nodes[indexbank2].dimensionlessstation; 

							station2 = bank2->nodes[indexbank2].station;																									//This is an existing node
							elevation2 = bank2->nodes[indexbank2].elevation;


							station1 = BankInterpolated.nodes[i].dimensionlessstation * rangestation1 + bank1->nodes[0].station;											//This is a creted node

							slope = (bank1->nodes[indexbank1].elevation - bank1->nodes[indexbank1-1].elevation)/(bank1->nodes[indexbank1].station - bank1->nodes[indexbank1-1].station);
							elevation1 = bank1->nodes[indexbank1-1].elevation+slope*(station1-bank1->nodes[indexbank1-1].station);

							BankInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	//CORRECTION FEB2010	((station1+station2)/2;
							BankInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;																//CORRECTION FEB2010	((elevation1+elevation2)/2;
							
							indexbank2 = indexbank2+1;

							////
							if(bank2->direction == 1)
								{
									if (indexbank2 > top2)
										{
											indexbank2 = top2;
										}
								}
							else if(bank2->direction == -1) 
								{
									if (indexbank2 > toe2)
										{
											indexbank2 = toe2;
										}
								}
							
						}
					//else if (bank1->nodes[indexbank1].dimensionlessstation<bank2->nodes[indexbank2].dimensionlessstation)
					else if ((bank1->nodes[indexbank1].dimensionlessstation - bank2->nodes[indexbank2].dimensionlessstation) < -tolerance)			// CORRECTION MAR2010
						{	
							BankInterpolated.nodes[i].dimensionlessstation = bank1->nodes[indexbank1].dimensionlessstation; 

							station1 = bank1->nodes[indexbank1].station;																									//This is an existing node
							elevation1 = bank1->nodes[indexbank1].elevation;
							
							station2 = BankInterpolated.nodes[i].dimensionlessstation * rangestation2 + bank2->nodes[0].station;											//This is a creted node

							slope = (bank2->nodes[indexbank2].elevation - bank2->nodes[indexbank2-1].elevation)/(bank2->nodes[indexbank2].station - bank2->nodes[indexbank2-1].station);
							elevation2 = bank2->nodes[indexbank2-1].elevation+slope*(station2-bank2->nodes[indexbank2-1].station);

							BankInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	//CORRECTION FEB2010	(station1+station2)/2;
							BankInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;																//CORRECTION FEB2010	(elevation1+elevation2)/2;

							indexbank1 = indexbank1+1;

							////
							if(bank1->direction == 1)
								{
									if (indexbank1 > top1)
										{
											indexbank1 = top1;
										}
								}
							else if(bank1->direction == -1) 
								{
									if (indexbank1 > toe1)
										{
											indexbank1 = toe1;
										}
								}

						}
					//else if (bank1->nodes[indexbank1].dimensionlessstation == bank2->nodes[indexbank2].dimensionlessstation)
					else			// CORRECTION MAR2010
						{	
							station1 = bank1->nodes[indexbank1].station;
							station2 = bank2->nodes[indexbank2].station;
							elevation1 = bank1->nodes[indexbank1].elevation;
							elevation2 = bank2->nodes[indexbank2].elevation;

							BankInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	//CORRECTION FEB2010	(station1+station2)/2;
							BankInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;	
							indexbank1 = indexbank1+1;
							indexbank2 = indexbank2+1;
							
							////
							if(bank1->direction == 1)
								{
									if (indexbank1 > top1)
										{
											indexbank1 = top1;
										}
									if (indexbank2 > top2)
										{
											indexbank2 = top2;
										}
								}
							else if(bank1->direction == -1) 
								{
									if (indexbank1 > toe1)
										{
											indexbank1 = toe1;
										}
									if (indexbank2 > toe2)
										{
											indexbank2 = toe2;
										}
								}
							/////
							
							numbernodesbankinterpolated = numbernodesbankinterpolated - 1;
							BankInterpolated.nodes[numbernodesbankinterpolated-1].station = BankInterpolated.nodes[numbernodesbankinterpolated].station;			//CORRECTION MAR2010			
							BankInterpolated.nodes[numbernodesbankinterpolated-1].elevation = BankInterpolated.nodes[numbernodesbankinterpolated].elevation;			//CORRECTION MAR2010
						}

				}
			
			BankInterpolated.numberofnodes = numbernodesbankinterpolated;
            BankInterpolated.SetNumberOfNodes(BankInterpolated.numberofnodes);
			BankInterpolated.direction = bank1->direction;
			BankInterpolated.groundwatertable = (1-DSweight)*bank1->groundwatertable+DSweight*bank2->groundwatertable;														//CORRECTION FEB2010(bank1->groundwatertable+bank2->groundwatertable)/2;
			
			//Check that the stations and elevations are monotonically varying
			BankGeometryQualityControl(BankInterpolated,inode); //DM-FEB2011  

			//Other parameters to be interpolated 
			if(bank1->initialnumberoflayers == 1 && bank2->initialnumberoflayers == 1)		//DM-FEB2011
				{																
					BankInterpolated.numberoflayers = 1;
					BankInterpolated.initialnumberoflayers = 1;					
					BankInterpolated.SetNumberOfLayers(BankInterpolated.numberoflayers);

					if(BankInterpolated.direction == 1)
						{
							BankInterpolated.material[0].bottomindex = 0;				
							BankInterpolated.material[0].topindex = BankInterpolated.numberofnodes - 1;					
							BankInterpolated.material[0].elevation = BankInterpolated.nodes[BankInterpolated.numberofnodes - 1].elevation;
						}
					if(BankInterpolated.direction == -1)
						{
							BankInterpolated.material[0].bottomindex = BankInterpolated.numberofnodes - 1;				
							BankInterpolated.material[0].topindex = 0;					
							BankInterpolated.material[0].elevation = BankInterpolated.nodes[0].elevation;					
						}
					BankInterpolated.material[0].unitweight = (1-DSweight)*bank1->material[0].unitweight+DSweight*bank2->material[0].unitweight;																		//CORRECTION FEB2010(bank1->material[0].unitweight+bank2->material[0].unitweight)/2; 
					BankInterpolated.material[0].erosionrate = (1-DSweight)*bank1->material[0].erosionrate+DSweight*bank2->material[0].erosionrate;																		//CORRECTION FEB2010(bank1->material[0].erosionrate+bank2->material[0].erosionrate)/2; 
					BankInterpolated.material[0].criticalshearstress = (1-DSweight)*bank1->material[0].criticalshearstress+DSweight*bank2->material[0].criticalshearstress;	//CORRECTION FEB2010(bank1->material[0].criticalshearstress+bank2->material[0].criticalshearstress)/2; 
					BankInterpolated.material[0].cohesion = (1-DSweight)*bank1->material[0].cohesion+DSweight*bank2->material[0].cohesion;																				//CORRECTION FEB2010(bank1->material[0].cohesion+bank2->material[0].cohesion)/2;  			
					BankInterpolated.material[0].reposeangle = (1-DSweight)*bank1->material[0].reposeangle+DSweight*bank2->material[0].reposeangle;																		//CORRECTION FEB2010(bank1->material[0].reposeangle+bank2->material[0].reposeangle)/2; 
					BankInterpolated.material[0].phib = (1-DSweight)*bank1->material[0].phib+DSweight*bank2->material[0].phib;			//CORRECTION FEB2010(bank1->material[0].phib+bank2->material[0].phib)/2; 
					//BankInterpolated.material[0].composition.numberofsizefractions = (1-DSweight)*bank1->material[0].composition.numberofsizefractions+DSweight*bank2->material[0].composition.numberofsizefractions;	//CORRECTION FEB2010(bank1->material[0].composition.numberofsizefractions+bank2->material[0].composition.numberofsizefractions)/2;  //DM-FEB2011 Commented out
					//BankInterpolated.material[0].composition.SetNumberOfSizeFractions(BankInterpolated.material[0].composition.numberofsizefractions); //DM-FEB2011 Commented out
					BankInterpolated.TotalAblock = (1-DSweight)*bank1->TotalAblock + DSweight*bank2->TotalAblock; //DM-FEB2012	
					BankInterpolated.Karmor = (1-DSweight)*bank1->Karmor + DSweight*bank2->Karmor; //DM-FEB2013	
				}																																		
			else																																		//DM-FEB2011
				{	
					//Find leading bank (upstream = 1, downstream = 2)																					
					if(DSweight>0.5)																													
						{																																
							BankInterpolated.initialnumberoflayers = bank2->initialnumberoflayers;														
							leadingbank = 2;
						}																																
					else																																
						{																																
							BankInterpolated.initialnumberoflayers = bank1->initialnumberoflayers;														
							leadingbank = 1;																											
						}	
					
					BankInterpolated.SetNumberOfLayers(BankInterpolated.initialnumberoflayers);

					//Find layers in common and in excess																							
					if(bank1->initialnumberoflayers > bank2->initialnumberoflayers) 
						{
							numberlayersincommon = bank2->initialnumberoflayers;
							numberlayersinexcess = bank1->initialnumberoflayers - bank2->initialnumberoflayers;
							excessbank = 1;
						}
					else if(bank1->initialnumberoflayers < bank2->initialnumberoflayers)
						{
							numberlayersincommon = bank1->initialnumberoflayers;
							numberlayersinexcess = bank2->initialnumberoflayers - bank1->initialnumberoflayers;
							excessbank = 2;
						}
					else
						{
							numberlayersincommon = bank1->initialnumberoflayers;
							numberlayersinexcess = 0;
							excessbank = 0;
						}

					//Layers in common					
					for(int ilayer = 0; ilayer <= numberlayersincommon - 1; ilayer++)
						{
							BankInterpolated.material[ilayer].elevation = (1-DSweight)*bank1->material[ilayer].elevation+DSweight*bank2->material[ilayer].elevation;
							BankInterpolated.material[ilayer].unitweight = (1-DSweight)*bank1->material[ilayer].unitweight+DSweight*bank2->material[ilayer].unitweight;																		//CORRECTION FEB2010(bank1->material[0].unitweight+bank2->material[0].unitweight)/2; 
							BankInterpolated.material[ilayer].erosionrate = (1-DSweight)*bank1->material[ilayer].erosionrate+DSweight*bank2->material[ilayer].erosionrate;																		//CORRECTION FEB2010(bank1->material[0].erosionrate+bank2->material[0].erosionrate)/2; 
							BankInterpolated.material[ilayer].criticalshearstress = (1-DSweight)*bank1->material[ilayer].criticalshearstress+DSweight*bank2->material[ilayer].criticalshearstress;												//CORRECTION FEB2010(bank1->material[0].criticalshearstress+bank2->material[0].criticalshearstress)/2; 
							BankInterpolated.material[ilayer].cohesion = (1-DSweight)*bank1->material[ilayer].cohesion+DSweight*bank2->material[ilayer].cohesion;																				//CORRECTION FEB2010(bank1->material[0].cohesion+bank2->material[0].cohesion)/2;  			
							BankInterpolated.material[ilayer].reposeangle = (1-DSweight)*bank1->material[ilayer].reposeangle+DSweight*bank2->material[ilayer].reposeangle;																		//CORRECTION FEB2010(bank1->material[0].reposeangle+bank2->material[0].reposeangle)/2; 
							BankInterpolated.material[ilayer].phib = (1-DSweight)*bank1->material[ilayer].phib+DSweight*bank2->material[ilayer].phib;		
						}

					//Layers in excess (if applicable)																								
					if(numberlayersinexcess > 0 && leadingbank == excessbank)
						{
							if(leadingbank == 1)
								{
									for(int ilayer = 0; ilayer <= numberlayersinexcess - 1; ilayer++)
										{
											BankInterpolated.material[numberlayersincommon+ilayer].elevation = bank1->material[numberlayersincommon+ilayer].elevation;
											BankInterpolated.material[numberlayersincommon+ilayer].unitweight = bank1->material[numberlayersincommon+ilayer].unitweight;
											BankInterpolated.material[numberlayersincommon+ilayer].erosionrate = bank1->material[numberlayersincommon+ilayer].erosionrate;
											BankInterpolated.material[numberlayersincommon+ilayer].criticalshearstress = bank1->material[numberlayersincommon+ilayer].criticalshearstress;
											BankInterpolated.material[numberlayersincommon+ilayer].cohesion = bank1->material[numberlayersincommon+ilayer].cohesion;
											BankInterpolated.material[numberlayersincommon+ilayer].reposeangle = bank1->material[numberlayersincommon+ilayer].reposeangle;
											BankInterpolated.material[numberlayersincommon+ilayer].phib  = bank1->material[numberlayersincommon+ilayer].phib;
										}	
								}
							else if(leadingbank == 2)
								{
									for(int ilayer = 0; ilayer <= numberlayersinexcess - 1; ilayer++)
										{
											BankInterpolated.material[numberlayersincommon+ilayer].elevation = bank2->material[numberlayersincommon+ilayer].elevation;
											BankInterpolated.material[numberlayersincommon+ilayer].unitweight = bank2->material[numberlayersincommon+ilayer].unitweight;
											BankInterpolated.material[numberlayersincommon+ilayer].erosionrate = bank2->material[numberlayersincommon+ilayer].erosionrate;
											BankInterpolated.material[numberlayersincommon+ilayer].criticalshearstress = bank2->material[numberlayersincommon+ilayer].criticalshearstress;
											BankInterpolated.material[numberlayersincommon+ilayer].cohesion = bank2->material[numberlayersincommon+ilayer].cohesion;
											BankInterpolated.material[numberlayersincommon+ilayer].reposeangle = bank2->material[numberlayersincommon+ilayer].reposeangle;
											BankInterpolated.material[numberlayersincommon+ilayer].phib  = bank2->material[numberlayersincommon+ilayer].phib;
										}	
								}
						}

					//Find number of current layers
					counter = BankInterpolated.initialnumberoflayers-1;

					if(BankInterpolated.direction == 1)
						{
							while(counter >= 0 && BankInterpolated.material[counter].elevation < BankInterpolated.nodes[0].elevation)
								{
									counter = counter - 1;
								}
							BankInterpolated.numberoflayers = counter + 1;
						}
					else if(BankInterpolated.direction == -1)
						{
							while(counter >= 0 && BankInterpolated.material[counter].elevation < BankInterpolated.nodes[BankInterpolated.numberofnodes-1].elevation)
								{	
									counter = counter - 1;
								}
							BankInterpolated.numberoflayers = counter + 1;
						}

					SetBottomAndTopIndex(BankInterpolated,inode);
					BankInterpolated.TotalAblock = (1-DSweight)*bank1->TotalAblock + DSweight*bank2->TotalAblock; //DM-FEB2012
					BankInterpolated.Karmor = (1-DSweight)*bank1->Karmor + DSweight*bank2->Karmor; //DM-FEB2013	
				}																
		}
	else if(DSweight == 1.0)//DM-FEB2011
		{
			BankInterpolated = *bank2;
		}
	else if(DSweight == 0.0)//DM-FEB2011
		{
			BankInterpolated = *bank1;
		}

	//Return the bank
	return BankInterpolated;
}	


Bank RMBankErosion::BankInterpolatedVARIANT(Bank *bank1, Bank *bank2, double DSweight, int inode, double tolerance, int MonitorNode)				//DM-FEB2012
{	
	Bank BankInterpolated;
	int counter,leadingbank,numberlayersincommon,numberlayersinexcess,excessbank;   
	Bank tempbank1,tempbank2,BankGeomeInterp ;						
	Bank* tbank1;	
	Bank* tbank2;	
	double DSweightstored; //MMMMTEMP TRIAL

	if(tolerance <= 0)			 
		{	
			DSweightstored = DSweight; //MMMMTEMP TRIAL
			DSweight = 1.0;	
		}						

	if(DSweight != 1.0 && DSweight != 0.0)
		{
			//Bank geometry interpolation
			tempbank1 = *bank1;																																																					
			tempbank2 = *bank2;																																																					
			tbank1 = &tempbank1;																																																				
			tbank2 = &tempbank2;
			//if(inode == 167 & tbank1->direction == 1)for(int k = 0; k<= tbank1->numberofnodes-1; k++)printf("[Before BGQControl] %d	%lf	%lf\n",k,tbank1->nodes[k].station,tbank1->nodes[k].elevation);//MMMMTEMP
			//if(inode == 167 & tbank1->direction == 1)for(int k = 0; k<= tbank2->numberofnodes-1; k++)printf("[Before BGQControl] %d	%lf	%lf\n",k,tbank2->nodes[k].station,tbank2->nodes[k].elevation);//MMMMTEMP
			if(inode == 167 & tbank1->direction == 1)printf("Before calling routine BankGeometryInterpolated\n");//MMMMTEMP Printing
			BankGeomeInterp = BankGeometryInterpolated(tbank1,tbank2,inode,DSweight,tolerance);																		
			BankInterpolated = BankGeomeInterp ;
			//if(inode == 167 & BankInterpolated.direction == 1)for(int k = 0; k<= BankInterpolated.numberofnodes-1; k++)printf("[Before BGQControl] %d	%lf	%lf\n",k,BankInterpolated.nodes[k].station,BankInterpolated.nodes[k].elevation);//MMMMTEMP
			
			//if(inode == 167 && BankInterpolated.direction == 1)printf("[BankInterpolatedVARIANT] Minimum elevation of bank1 = %lf\n",tbank1->nodes[0].elevation); //MMMMTEMP Printing
			//if(inode == 167 && BankInterpolated.direction == 1)printf("[BankInterpolatedVARIANT] Minimum elevation of bank2 = %lf\n",tbank2->nodes[0].elevation); //MMMMTEMP Printing
			//if(inode == 167 && BankInterpolated.direction == 1)printf("[BankInterpolatedVARIANT] Minimum elevation of bank = %lf\n",BankInterpolated.nodes[0].elevation); //MMMMTEMP Printing			

			//Check that the stations and elevations are monotonically varying
			BankGeometryQualityControl(BankInterpolated,inode); //DM-FEB2011  

			//if(inode == 167 && BankInterpolated.direction == 1)printf("[BankInterpolatedVARIANT] Minimum elevation of bank = %lf\n",BankInterpolated.nodes[0].elevation); //MMMMTEMP Printing

			//Other parameters to be interpolated 
			if(bank1->initialnumberoflayers == 1 && bank2->initialnumberoflayers == 1)		//DM-FEB2011
				{																
					BankInterpolated.numberoflayers = 1;
					BankInterpolated.initialnumberoflayers = 1;					
					BankInterpolated.SetNumberOfLayers(BankInterpolated.numberoflayers);

					if(BankInterpolated.direction == 1)
						{
							BankInterpolated.material[0].bottomindex = 0;				
							BankInterpolated.material[0].topindex = BankInterpolated.numberofnodes - 1;					
							BankInterpolated.material[0].elevation = BankInterpolated.nodes[BankInterpolated.numberofnodes - 1].elevation;
						}
					if(BankInterpolated.direction == -1)
						{
							BankInterpolated.material[0].bottomindex = BankInterpolated.numberofnodes - 1;				
							BankInterpolated.material[0].topindex = 0;					
							BankInterpolated.material[0].elevation = BankInterpolated.nodes[0].elevation;					
						}
					BankInterpolated.material[0].unitweight = (1-DSweight)*bank1->material[0].unitweight+DSweight*bank2->material[0].unitweight;																		//CORRECTION FEB2010(bank1->material[0].unitweight+bank2->material[0].unitweight)/2; 
					BankInterpolated.material[0].erosionrate = (1-DSweight)*bank1->material[0].erosionrate+DSweight*bank2->material[0].erosionrate;																		//CORRECTION FEB2010(bank1->material[0].erosionrate+bank2->material[0].erosionrate)/2; 
					BankInterpolated.material[0].criticalshearstress = (1-DSweight)*bank1->material[0].criticalshearstress+DSweight*bank2->material[0].criticalshearstress;	//CORRECTION FEB2010(bank1->material[0].criticalshearstress+bank2->material[0].criticalshearstress)/2; 
					BankInterpolated.material[0].cohesion = (1-DSweight)*bank1->material[0].cohesion+DSweight*bank2->material[0].cohesion;																				//CORRECTION FEB2010(bank1->material[0].cohesion+bank2->material[0].cohesion)/2;  			
					BankInterpolated.material[0].reposeangle = (1-DSweight)*bank1->material[0].reposeangle+DSweight*bank2->material[0].reposeangle;																		//CORRECTION FEB2010(bank1->material[0].reposeangle+bank2->material[0].reposeangle)/2; 
					BankInterpolated.material[0].phib = (1-DSweight)*bank1->material[0].phib+DSweight*bank2->material[0].phib;			//CORRECTION FEB2010(bank1->material[0].phib+bank2->material[0].phib)/2; 
					BankInterpolated.TotalAblock = (1-DSweight)*bank1->TotalAblock + DSweight*bank2->TotalAblock; //DM-FEB2012
					BankInterpolated.Karmor = (1-DSweight)*bank1->Karmor + DSweight*bank2->Karmor; //DM-FEB2013	
				}																																		
			else																																		//DM-FEB2011
				{	
					//Find leading bank (upstream = 1, downstream = 2)																					
					if(DSweight>0.5)																													
						{																																
							BankInterpolated.initialnumberoflayers = bank2->initialnumberoflayers;														
							leadingbank = 2;
						}																																
					else																																
						{																																
							BankInterpolated.initialnumberoflayers = bank1->initialnumberoflayers;														
							leadingbank = 1;																											
						}	
					
					BankInterpolated.SetNumberOfLayers(BankInterpolated.initialnumberoflayers);

					//Find layers in common and in excess																							
					if(bank1->initialnumberoflayers > bank2->initialnumberoflayers) 
						{
							numberlayersincommon = bank2->initialnumberoflayers;
							numberlayersinexcess = bank1->initialnumberoflayers - bank2->initialnumberoflayers;
							excessbank = 1;
						}
					else if(bank1->initialnumberoflayers < bank2->initialnumberoflayers)
						{
							numberlayersincommon = bank1->initialnumberoflayers;
							numberlayersinexcess = bank2->initialnumberoflayers - bank1->initialnumberoflayers;
							excessbank = 2;
						}
					else
						{
							numberlayersincommon = bank1->initialnumberoflayers;
							numberlayersinexcess = 0;
							excessbank = 0;
						}

					//Layers in common					
					for(int ilayer = 0; ilayer <= numberlayersincommon - 1; ilayer++)
						{
							BankInterpolated.material[ilayer].elevation = (1-DSweight)*bank1->material[ilayer].elevation+DSweight*bank2->material[ilayer].elevation;
							BankInterpolated.material[ilayer].unitweight = (1-DSweight)*bank1->material[ilayer].unitweight+DSweight*bank2->material[ilayer].unitweight;																		//CORRECTION FEB2010(bank1->material[0].unitweight+bank2->material[0].unitweight)/2; 
							BankInterpolated.material[ilayer].erosionrate = (1-DSweight)*bank1->material[ilayer].erosionrate+DSweight*bank2->material[ilayer].erosionrate;																		//CORRECTION FEB2010(bank1->material[0].erosionrate+bank2->material[0].erosionrate)/2; 
							BankInterpolated.material[ilayer].criticalshearstress = (1-DSweight)*bank1->material[ilayer].criticalshearstress+DSweight*bank2->material[ilayer].criticalshearstress;												//CORRECTION FEB2010(bank1->material[0].criticalshearstress+bank2->material[0].criticalshearstress)/2; 
							BankInterpolated.material[ilayer].cohesion = (1-DSweight)*bank1->material[ilayer].cohesion+DSweight*bank2->material[ilayer].cohesion;																				//CORRECTION FEB2010(bank1->material[0].cohesion+bank2->material[0].cohesion)/2;  			
							BankInterpolated.material[ilayer].reposeangle = (1-DSweight)*bank1->material[ilayer].reposeangle+DSweight*bank2->material[ilayer].reposeangle;																		//CORRECTION FEB2010(bank1->material[0].reposeangle+bank2->material[0].reposeangle)/2; 
							BankInterpolated.material[ilayer].phib = (1-DSweight)*bank1->material[ilayer].phib+DSweight*bank2->material[ilayer].phib;		
						}

					//Layers in excess (if applicable)																								
					if(numberlayersinexcess > 0 && leadingbank == excessbank)
						{
							if(leadingbank == 1)
								{
									for(int ilayer = 0; ilayer <= numberlayersinexcess - 1; ilayer++)
										{
											BankInterpolated.material[numberlayersincommon+ilayer].elevation = bank1->material[numberlayersincommon+ilayer].elevation;
											BankInterpolated.material[numberlayersincommon+ilayer].unitweight = bank1->material[numberlayersincommon+ilayer].unitweight;
											BankInterpolated.material[numberlayersincommon+ilayer].erosionrate = bank1->material[numberlayersincommon+ilayer].erosionrate;
											BankInterpolated.material[numberlayersincommon+ilayer].criticalshearstress = bank1->material[numberlayersincommon+ilayer].criticalshearstress;
											BankInterpolated.material[numberlayersincommon+ilayer].cohesion = bank1->material[numberlayersincommon+ilayer].cohesion;
											BankInterpolated.material[numberlayersincommon+ilayer].reposeangle = bank1->material[numberlayersincommon+ilayer].reposeangle;
											BankInterpolated.material[numberlayersincommon+ilayer].phib  = bank1->material[numberlayersincommon+ilayer].phib;
										}	
								}
							else if(leadingbank == 2)
								{
									for(int ilayer = 0; ilayer <= numberlayersinexcess - 1; ilayer++)
										{
											BankInterpolated.material[numberlayersincommon+ilayer].elevation = bank2->material[numberlayersincommon+ilayer].elevation;
											BankInterpolated.material[numberlayersincommon+ilayer].unitweight = bank2->material[numberlayersincommon+ilayer].unitweight;
											BankInterpolated.material[numberlayersincommon+ilayer].erosionrate = bank2->material[numberlayersincommon+ilayer].erosionrate;
											BankInterpolated.material[numberlayersincommon+ilayer].criticalshearstress = bank2->material[numberlayersincommon+ilayer].criticalshearstress;
											BankInterpolated.material[numberlayersincommon+ilayer].cohesion = bank2->material[numberlayersincommon+ilayer].cohesion;
											BankInterpolated.material[numberlayersincommon+ilayer].reposeangle = bank2->material[numberlayersincommon+ilayer].reposeangle;
											BankInterpolated.material[numberlayersincommon+ilayer].phib  = bank2->material[numberlayersincommon+ilayer].phib;
										}	
								}
						}

					//Find number of current layers
					counter = BankInterpolated.initialnumberoflayers-1;

					if(BankInterpolated.direction == 1)
						{
							while(counter >= 0 && BankInterpolated.material[counter].elevation < BankInterpolated.nodes[0].elevation)
								{
									counter = counter - 1;
								}
							BankInterpolated.numberoflayers = counter + 1;
						}
					else if(BankInterpolated.direction == -1)
						{
							while(counter >= 0 && BankInterpolated.material[counter].elevation < BankInterpolated.nodes[BankInterpolated.numberofnodes-1].elevation)
								{	
									counter = counter - 1;
								}
							BankInterpolated.numberoflayers = counter + 1;
						}

					SetBottomAndTopIndex(BankInterpolated,inode);
					BankInterpolated.TotalAblock = (1-DSweight)*bank1->TotalAblock + DSweight*bank2->TotalAblock; //DM-FEB2011
					BankInterpolated.Karmor = (1-DSweight)*bank1->Karmor + DSweight*bank2->Karmor; //DM-FEB2013	
				}																
		}
	else if(DSweight == 1.0)//DM-FEB2011
		{
			//BankInterpolated = *bank2;																					//MMMMTEMP TRIAL Commented out for now
			
			if(DSweightstored > 0.5)BankInterpolated = *bank2;																//MMMMTEMP TRIAL
			else BankInterpolated = *bank1;																					//MMMMTEMP TRIAL
			BankInterpolated.TotalAblock = (1-DSweightstored)*bank1->TotalAblock + DSweightstored*bank2->TotalAblock;		//MMMMTEMP TRIAL
			
		}
	else if(DSweight == 0.0)//DM-FEB2011
		{
			BankInterpolated = *bank1;
		}

	//Return the bank
	return BankInterpolated;
}	


Bank RMBankErosion::BankGeometryInterpolated(Bank *bank1, Bank *bank2, int inode, double DSweight, double tolerance)   //DM-FEB2012
{
	Bank BankGeometryInterpolated;
	int numbernodesbank1,numbernodesbank2,toe1,toe2,top1,top2,numbernodesbankinterpolated,indexbank1,indexbank2,iinitial,ifinal;
	double rangestation1,rangestation2,station1,elevation1,station2,elevation2,slope;
	bool leftbankinterpolated;
	//if(inode == 167 & bank1->direction == 1)printf("Entered BankGeometryInterpolated\n");//MMMMTEMP Printing	
	//if(inode == 167 & bank1->direction == 1)for(int k = 0; k<= bank1->numberofnodes-1; k++)printf("[In BankGeometryInterpolated]	%d	%lf	%lf\n",k,bank1->nodes[k].station,bank1->nodes[k].elevation);//MMMMTEMP
	//if(inode == 167 & bank1->direction == 1)for(int k = 0; k<= bank2->numberofnodes-1; k++)printf("[In BankGeometryInterpolated]	%d	%lf	%lf\n",k,bank2->nodes[k].station,bank2->nodes[k].elevation);//MMMMTEMP

	if(bank1->direction == -1)																							
		{																												
			leftbankinterpolated = true;																				
			Bank tempbank1, tempbank2;																					
			tempbank1 = *bank1;																							
			tempbank2 = *bank2;																							
			bank1->direction = 1;																						
			bank2->direction = 1;																						
			for(int r = 0; r <= bank1->numberofnodes-1; r++) 
				{
					bank1->nodes[r].station = -tempbank1.nodes[bank1->numberofnodes-1-r].station;			
					bank1->nodes[r].elevation = tempbank1.nodes[bank1->numberofnodes-1-r].elevation;		
				}
			//if(inode == nodeofinterest)for(int k = 0; k<=bank1->numberofnodes-1;k++)logmsg("bANK1flipped	%d	%lf	%lf\n",k,bank1->nodes[k].station,bank1->nodes[k].elevation);	
			for(int r = 0; r <= bank2->numberofnodes-1; r++) 
				{
					bank2->nodes[r].station = -tempbank2.nodes[bank2->numberofnodes-1-r].station;			
					bank2->nodes[r].elevation = tempbank2.nodes[bank2->numberofnodes-1-r].elevation;		
				}
			//if(inode == nodeofinterest)for(int k = 0; k<=bank2->numberofnodes-1;k++)logmsg("bANK2flipped	%d	%lf	%lf\n",k,bank2->nodes[k].station,bank2->nodes[k].elevation);	
		}	
	else if(bank1->direction == 1)																						
		{																												
			leftbankinterpolated = false;																				
		}

	numbernodesbank1 = bank1->numberofnodes;    // Number of nodes of the first bank
	numbernodesbank2 = bank2->numberofnodes;    // Number of nodes of the second bank

	toe1 = 0;
	top1 = numbernodesbank1-1;
	toe2 = 0;
	top2 = numbernodesbank2-1;
	rangestation1 = bank1->nodes[top1].station - bank1->nodes[toe1].station;
	rangestation1 = bank1->nodes[top1-1].station - bank1->nodes[toe1].station; 
	rangestation2 = bank2->nodes[top2].station - bank2->nodes[toe2].station;
	rangestation2 = bank2->nodes[top2-1].station - bank2->nodes[toe2].station; 

	//Dimensionless stations (from 0 to 1 for both the right and the left bank) 
	for(int i = 0; i <= numbernodesbank1-1; i++)
		{
			if(rangestation1 == 0.0)bank1->nodes[i].dimensionlessstation = 1.0;
			else bank1->nodes[i].dimensionlessstation = (bank1->nodes[i].station-bank1->nodes[0].station)/rangestation1;
			//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! (%d) bank1->nodes[i].dimensionlessstation = %lf (rangestation1 = %lf)\n",i,bank1->nodes[i].dimensionlessstation,rangestation1);//MMMMTEMP
		}
	for(int i = 0; i <= numbernodesbank2-1; i++)
		{
			if(rangestation2 == 0.0)bank2->nodes[i].dimensionlessstation = 1.0;
			else bank2->nodes[i].dimensionlessstation = (bank2->nodes[i].station-bank2->nodes[0].station)/rangestation2; 
		}

	//Number of nodes of the interpolated bank
	numbernodesbankinterpolated = numbernodesbank1 + numbernodesbank2 - 3; 

    BankGeometryInterpolated.SetNumberOfNodes(numbernodesbankinterpolated);

	//Bank end points (master cords)
	BankGeometryInterpolated.nodes[0].dimensionlessstation = 0.0;
	BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].dimensionlessstation = 1.0;

	BankGeometryInterpolated.nodes[0].station = (1-DSweight)*bank1->nodes[0].station+DSweight*bank2->nodes[0].station;				
	BankGeometryInterpolated.nodes[0].elevation = (1-DSweight)*bank1->nodes[0].elevation+DSweight*bank2->nodes[0].elevation ;																					
																																									
	BankGeometryInterpolated.nodes[numbernodesbankinterpolated-2].station = (1-DSweight)*bank1->nodes[numbernodesbank1-2].station+DSweight*bank2->nodes[numbernodesbank2-2].station;								
	BankGeometryInterpolated.nodes[numbernodesbankinterpolated-2].elevation = (1-DSweight)*bank1->nodes[numbernodesbank1-2].elevation+DSweight*bank2->nodes[numbernodesbank2-2].elevation;			
																																															
	BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].station = (1-DSweight)*bank1->nodes[numbernodesbank1-1].station+DSweight*bank2->nodes[numbernodesbank2-1].station;				
	BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].elevation = (1-DSweight)*bank1->nodes[numbernodesbank1-1].elevation+DSweight*bank2->nodes[numbernodesbank2-1].elevation;			

	//Minor cords
	indexbank1 = 1;			
	indexbank2 = 1;			
	iinitial = 1;			
	ifinal = 3;				

	for(int i = iinitial; i <= numbernodesbankinterpolated-ifinal; i++)
		{		  
			if((bank1->nodes[indexbank1].dimensionlessstation - bank2->nodes[indexbank2].dimensionlessstation) > tolerance || (indexbank1 == bank1->numberofnodes-2))			//DM-FEB2012
				{	
							BankGeometryInterpolated.nodes[i].dimensionlessstation = bank2->nodes[indexbank2].dimensionlessstation; 

							station2 = bank2->nodes[indexbank2].station;																									//This is an existing node
							elevation2 = bank2->nodes[indexbank2].elevation;


							station1 = BankGeometryInterpolated.nodes[i].dimensionlessstation * rangestation1 + bank1->nodes[0].station;											//This is a created node
							if(bank1->nodes[indexbank1].station != bank1->nodes[indexbank1-1].station)			//DM-FEB2012
							{																					//DM-FEB2012
							slope = (bank1->nodes[indexbank1].elevation - bank1->nodes[indexbank1-1].elevation)/(bank1->nodes[indexbank1].station - bank1->nodes[indexbank1-1].station);
							elevation1 = bank1->nodes[indexbank1-1].elevation+slope*(station1-bank1->nodes[indexbank1-1].station);
							}																					//DM-FEB2012
							else																				//DM-FEB2012
							{																					//DM-FEB2012
								elevation1 = bank1->nodes[indexbank1].elevation;								//DM-FEB2012
							}																					//DM-FEB2012

							BankGeometryInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	
							BankGeometryInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;		
							
							//if(inode == 167 & bank1->direction == 1)printf("[in BankGeometryInterpoolated, O1] %d	%lf %lf\n",i,BankGeometryInterpolated.nodes[i].station,BankGeometryInterpolated.nodes[i].elevation);//MMMMTEMP

							indexbank2 = indexbank2+1;

							if (indexbank2 > top2)
								{
									indexbank2 = top2;
								}							
				}
			else if ((bank1->nodes[indexbank1].dimensionlessstation - bank2->nodes[indexbank2].dimensionlessstation) < -tolerance || (indexbank2 == bank2->numberofnodes-2))			//DM-FEB2012
				{	
							BankGeometryInterpolated.nodes[i].dimensionlessstation = bank1->nodes[indexbank1].dimensionlessstation; 
							//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! BankGeometryInterpolated.nodes[i].dimensionlessstation = %lf \n",BankGeometryInterpolated.nodes[i].dimensionlessstation);//MMMMTEMP
							//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! indexbank1 = %d, bank1->nodes[indexbank1].dimensionlessstation = %lf \n",indexbank1,bank1->nodes[indexbank1].dimensionlessstation);//MMMMTEMP


							station1 = bank1->nodes[indexbank1].station;																									//This is an existing node
							elevation1 = bank1->nodes[indexbank1].elevation;
							
							station2 = BankGeometryInterpolated.nodes[i].dimensionlessstation * rangestation2 + bank2->nodes[0].station;											//This is a created node
							//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! bank2->nodes[0].station = %lf, station2 = %lf (%lf	%lf)\n",bank2->nodes[0].station,station2, BankGeometryInterpolated.nodes[i].dimensionlessstation,rangestation2);//MMMMTEMP

							if(bank2->nodes[indexbank2].station != bank2->nodes[indexbank2-1].station)		//DM-FEB2012
							{																				//DM-FEB2012
								slope = (bank2->nodes[indexbank2].elevation - bank2->nodes[indexbank2-1].elevation)/(bank2->nodes[indexbank2].station - bank2->nodes[indexbank2-1].station);
								elevation2 = bank2->nodes[indexbank2-1].elevation+slope*(station2-bank2->nodes[indexbank2-1].station);
								//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! = %d %lf	\n",i,elevation2);//MMMMTEMP
								//if(inode == 167 & bank1->direction == 1)printf("HERE!!!!!!!!!!! = %d	%lf	%lf	%lf	%lf\n",indexbank2,bank2->nodes[indexbank2-1].elevation,slope,station2,bank2->nodes[indexbank2-1].station);//MMMMTEMP
							}																				//DM-FEB2012
							else																			//DM-FEB2012
							{																				//DM-FEB2012
								elevation2 = bank2->nodes[indexbank2].elevation;							//DM-FEB2012
							}																				//DM-FEB2012

							BankGeometryInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	
							BankGeometryInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;																

							//if(inode == 167 & bank1->direction == 1)printf("[in BankGeometryInterpoolated, O2] %d	%lf %lf	%lf\n",i,BankGeometryInterpolated.nodes[i].station,BankGeometryInterpolated.nodes[i].elevation,DSweight);//MMMMTEMP
							//if(inode == 167 & bank1->direction == 1)printf("[in BankGeometryInterpoolated, O2] %lf %lf\n",elevation1,elevation2);//MMMMTEMP

							indexbank1 = indexbank1+1;

							if (indexbank1 > top1)
								{
									indexbank1 = top1;
								}
				}
			else			
				{	
					station1 = bank1->nodes[indexbank1].station;
					station2 = bank2->nodes[indexbank2].station;
					elevation1 = bank1->nodes[indexbank1].elevation;
					elevation2 = bank2->nodes[indexbank2].elevation;

					BankGeometryInterpolated.nodes[i].station = (1-DSweight)*station1+DSweight*station2;																	//CORRECTION FEB2010	(station1+station2)/2;
					BankGeometryInterpolated.nodes[i].elevation = (1-DSweight)*elevation1+DSweight*elevation2;	
					
					//if(inode == 167& bank1->direction == 1)printf("[in BankGeometryInterpoolated, O3] %d	%lf %lf\n",i,BankGeometryInterpolated.nodes[i].station,BankGeometryInterpolated.nodes[i].elevation);//MMMMTEMP

					indexbank1 = indexbank1+1;
					indexbank2 = indexbank2+1;
							
					if (indexbank1 > top1)
						{
							indexbank1 = top1;
						}
					if (indexbank2 > top2)
						{
							indexbank2 = top2;
						}


					numbernodesbankinterpolated = numbernodesbankinterpolated - 1;																							//DM-FEB2012
					BankGeometryInterpolated.nodes[numbernodesbankinterpolated-2].station = BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].station;			//DM-FEB2012
					BankGeometryInterpolated.nodes[numbernodesbankinterpolated-2].elevation = BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].elevation;		//DM-FEB2012
					BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].station = BankGeometryInterpolated.nodes[numbernodesbankinterpolated].station;			//DM-FEB2012
					BankGeometryInterpolated.nodes[numbernodesbankinterpolated-1].elevation = BankGeometryInterpolated.nodes[numbernodesbankinterpolated].elevation;		//DM-FEB2012
				}

		}
			
	BankGeometryInterpolated.numberofnodes = numbernodesbankinterpolated;
    BankGeometryInterpolated.SetNumberOfNodes(BankGeometryInterpolated.numberofnodes);
	BankGeometryInterpolated.direction = bank1->direction;

	if(leftbankinterpolated == true)																				
		{																										
			BankGeometryInterpolated.direction = -1;																
			bank1->direction = -1;
			bank2->direction = -1;
			Bank tempbankinterepolated;																					
			tempbankinterepolated = BankGeometryInterpolated;																							
			for(int r = 0; r <= BankGeometryInterpolated.numberofnodes-1; r++) 
				{
					BankGeometryInterpolated.nodes[r].station = -tempbankinterepolated.nodes[BankGeometryInterpolated.numberofnodes-1-r].station;	
					BankGeometryInterpolated.nodes[r].elevation = tempbankinterepolated.nodes[BankGeometryInterpolated.numberofnodes-1-r].elevation;	
				}
		}									
			
	BankGeometryInterpolated.groundwatertable = (1-DSweight)*bank1->groundwatertable+DSweight*bank2->groundwatertable;														//CORRECTION FEB2010(bank1->groundwatertable+bank2->groundwatertable)/2;
	

	//if(inode == 167 & bank1->direction == 1)for(int k = 0; k<= BankGeometryInterpolated.numberofnodes-1; k++)printf("[in BankGeometryInterpoolated] %d	%lf	%lf\n",k,BankGeometryInterpolated.nodes[k].station,BankGeometryInterpolated.nodes[k].elevation);//MMMMTEMP

	return BankGeometryInterpolated;
}



void RMBankErosion::BankGeometryQualityControl(Bank& bank, int inode) //DM-FEB2011
{
Bank temp;	
int nodetoerase;
bool SectionOk;
double m;

//if(inode == 167 & bank.direction == 1)for(int k = 0; k<= bank.numberofnodes-1; k++)printf("[In BGQControl] %d	%lf	%lf\n",k,bank.nodes[k].station,bank.nodes[k].elevation);//MMMMTEMP

if(bank.direction == 1)					//Right bank
	{
		//Copy to temporary bank
		temp = bank;
		
		//Trim nodes with incorrect elevation
		SectionOk = false;

		while(!SectionOk)
		{
			int i = 0;
			while(bank.nodes[i].elevation <= bank.nodes[i+1].elevation && i < bank.numberofnodes-2)i = i+1;
			nodetoerase = i;
  
			if(nodetoerase != bank.numberofnodes-2)//Trim
				{
					bank.SetNumberOfNodes(bank.numberofnodes - 1);

					for(int i = 0; i <= nodetoerase-1; i++)bank.nodes[i] = temp.nodes[i];
					for(int i = nodetoerase; i <= bank.numberofnodes - 1; i++)bank.nodes[i] = temp.nodes[i+1];

					temp = bank;
				}
			else if(nodetoerase == bank.numberofnodes-2)SectionOk = true;//Do not trim anymore
			
			
		}
		//Trim nodes with incorrect station	
		SectionOk = false;

		while(!SectionOk)
		{
			int i = 1;
			
			while(bank.nodes[i].station >= bank.nodes[i-1].station && i <= bank.numberofnodes-2)i = i+1;
			nodetoerase = i;

			if(nodetoerase < bank.numberofnodes-2)//Trim 
				{
					bank.SetNumberOfNodes(bank.numberofnodes - 1);

					for(int i = 0; i <= nodetoerase-1; i++)bank.nodes[i] = temp.nodes[i];
					for(int i = nodetoerase; i <= bank.numberofnodes - 1; i++)bank.nodes[i] = temp.nodes[i+1];

					temp = bank;
				}
			else if(nodetoerase == bank.numberofnodes-2 && bank.numberofnodes > 3)//Modify node																		
				{																											
					bank.nodes[nodetoerase].elevation = temp.nodes[nodetoerase+1].elevation;	
					if((temp.nodes[nodetoerase-1].elevation-temp.nodes[nodetoerase-2].elevation)!=0.0)
						{
							m = (temp.nodes[nodetoerase-1].station-temp.nodes[nodetoerase-2].station)/(temp.nodes[nodetoerase-1].elevation-temp.nodes[nodetoerase-2].elevation);
							bank.nodes[nodetoerase].station = temp.nodes[nodetoerase-1].station + m*(bank.nodes[nodetoerase].elevation-temp.nodes[nodetoerase-1].elevation);
						}
					else
						{
							bank.nodes[nodetoerase].station = bank.nodes[nodetoerase-1].station;
						}
				}																														
		
			else if(nodetoerase == bank.numberofnodes-1 || bank.numberofnodes <= 3)SectionOk = true;//Do not trim anymore  

		}
	}
else if(bank.direction == -1)					//Left bank
{
		//Copy to temporary bank
		temp = bank;

		//Trim nodes with incorrect elevation
		SectionOk = false;

		while(!SectionOk)
		{
			int i = bank.numberofnodes-1;
			while(bank.nodes[i].elevation <= bank.nodes[i-1].elevation && i > 1)
				{
					i = i-1;
				}	
			nodetoerase = i;

			if(nodetoerase != 1)//Trim
				{

					bank.SetNumberOfNodes(bank.numberofnodes - 1);

					for(int i = 0; i <= nodetoerase-1; i++)bank.nodes[i] = temp.nodes[i];
					for(int i = nodetoerase; i <= bank.numberofnodes - 1; i++)bank.nodes[i] = temp.nodes[i+1];

					temp = bank;
				}
			else if(nodetoerase == 1)SectionOk = true;//Do not trim anymore
		}
		//Trim nodes with incorrect station	
		SectionOk = false;

		while(!SectionOk)
		{
			int i = bank.numberofnodes-2;
			
			while(bank.nodes[i].station <= bank.nodes[i+1].station && i >= 1)i = i-1;
			nodetoerase = i;

			if(nodetoerase > 1)//Trim
				{

					bank.SetNumberOfNodes(bank.numberofnodes - 1);

					for(int i = 0; i <= nodetoerase-1; i++)bank.nodes[i] = temp.nodes[i];
					for(int i = nodetoerase; i <= bank.numberofnodes - 1; i++)bank.nodes[i] = temp.nodes[i+1];

					temp = bank;
				}
			else if(nodetoerase == 1 && bank.numberofnodes > 3)	//Modify node																									
				{																													
					bank.nodes[nodetoerase].elevation = temp.nodes[0].elevation;
					if((temp.nodes[2].elevation-temp.nodes[3].elevation)!=0.0)
						{
							m = (temp.nodes[2].station-temp.nodes[3].station)/(temp.nodes[2].elevation-temp.nodes[3].elevation);
							bank.nodes[nodetoerase].station = temp.nodes[2].station + m*(bank.nodes[1].elevation-temp.nodes[2].elevation);
						}
					else
						{
							bank.nodes[nodetoerase].station = bank.nodes[2].station;
						}
				}																														

			else if(nodetoerase == 0 || bank.numberofnodes <= 3)SectionOk = true;//Do not trim anymore
		}
	}

}


void RMBankErosion::SetBottomAndTopIndex(Bank& bank, int inode) 
{
	int counter;
	Bank Abank;
	Coordinates node1, node2,Anode;	
	
	//Check if nodes at layer elevation exist
	for(int ilayer = 1; ilayer <= bank.numberoflayers - 1; ilayer ++)  //Note that this is not done for the top layer
		{						
			counter = 0;
			while(counter < bank.numberofnodes && bank.nodes[counter].elevation != bank.material[ilayer].elevation)	
			//while(counter < bank.numberofnodes && fabs(bank.nodes[counter].elevation - bank.material[ilayer].elevation) > 0.000001) //MMMMTEMP AdNo  Using a tolerance
				{
					counter = counter + 1;
				}
			//if(inode == 0 && bank.direction == -1)printf("counter = %d, bank.material[ilayer].elevation = %lf \n", counter, bank.material[ilayer].elevation);//MMMMTEMP AdNo
			if(counter < bank.numberofnodes)			//Node exists, check if there is another one												
				{	//if(inode == 0 && bank.direction == -1)printf(">>>> NODE EXISTSSSSSSSSSSS <<<<\n");//MMMMTEMP AdNo																																																																																	
					if(bank.nodes[counter+1].elevation != bank.material[ilayer].elevation)	  
					//if(fabs(bank.nodes[counter+1].elevation - bank.material[ilayer].elevation)>0.000001)//MMMMTEMP AdNo	  //MMMMTEMP AdNo  Using a tolerance
						{																																				
							//Need to add node																														

							if(bank.direction == 1)																										
								{																																	
									//Store bank
									Abank = bank;																										
													
									//Create node
									node1 = bank.nodes[counter];																						
									node2 = bank.nodes[counter-1];																						
									Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);												
											
									//Update number of nodes
									bank.SetNumberOfNodes(bank.numberofnodes+1);															

									//Re-order nodes
									for(int j = bank.numberofnodes-1; j >=counter+1; j--)																
										{																															
											bank.nodes[j] = Abank.nodes[j-1];																			
										}																															
									bank.nodes[counter] = Anode;																						
								}																																	
							else if(bank.direction == -1)																								
								{																																	
									//Store bank
									Abank = bank;																										
			
									//Create node
									node1 = bank.nodes[counter];															
									node2 = bank.nodes[counter+1];																					
									Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);												
											
									//Update number of nodes
									bank.SetNumberOfNodes(bank.numberofnodes+1);											

									//Re-order nodes
									bank.nodes[counter+1] = Anode;																									
									for(int j = counter + 2; j <=bank.numberofnodes-1; j++)									
										{																			
											bank.nodes[j] = Abank.nodes[j-1];	
										}																																	
								}										
						}																													
					else																																
						{																																
							//Does not need to add node	
						}																																		
				}																																						
			else if(counter == bank.numberofnodes)		//Node does not exist, add it twice																
				{
					//Add node
					if(bank.direction == 1)
						{
							//Add node a first time		
											
							//Store bank
							Abank = bank;																												
											
							//Find location
							counter = 0;
							while(counter < bank.numberofnodes && bank.nodes[counter].elevation < bank.material[ilayer].elevation)
								{
									counter = counter + 1;
								}

							//Create node
							node1 = bank.nodes[counter];		
							node2 = bank.nodes[counter-1];	
							Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);
									
							//Update number of nodes
							bank.SetNumberOfNodes(bank.numberofnodes+1);

							//Re-order nodes
							for(int j = bank.numberofnodes-1; j >=counter+1; j--)
								{
									bank.nodes[j] = Abank.nodes[j-1];
								}
							bank.nodes[counter] = Anode;

							//Add node a second time																												
											
							//Store bank
							Abank = bank;																											

							//Create node
							node1 = bank.nodes[counter];																							
							node2 = bank.nodes[counter-1];																								
							Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);														

							//Update number of nodes
							bank.SetNumberOfNodes(bank.numberofnodes+1);																

							//Re-order nodes
							for(int j = bank.numberofnodes-1; j >=counter+1; j--)																		
								{																																	
									bank.nodes[j] = Abank.nodes[j-1];																					
								}																																	
							bank.nodes[counter] = Anode;																							

						}
					else if(bank.direction == -1)
						{ 
							//Add node a first time

							//Store bank
							Abank = bank;																												
							
							//Find location
							counter = bank.numberofnodes-1;
							while(counter > 0 && bank.nodes[counter].elevation < bank.material[ilayer].elevation)
								{
									counter = counter - 1;
								}
									
							//Create node
							node1 = bank.nodes[counter];		
							node2 = bank.nodes[counter+1];	
							Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);

							//Update number of nodes
							bank.SetNumberOfNodes(bank.numberofnodes+1);

							//Re-order nodes
							bank.nodes[counter+1] = Anode;											
							for(int j = counter + 2; j <=bank.numberofnodes-1; j++)
								{
									bank.nodes[j] = Abank.nodes[j-1];
								}

							//Add node a second time																											

							//Store bank
							Abank = bank;																												
																																					
							//Create node
							node1 = bank.nodes[counter];																								
							node2 = bank.nodes[counter+1];																								
							Anode = NodeAtZposition(node1, node2, bank.material[ilayer].elevation);														
									
							//Update number of nodes
							bank.SetNumberOfNodes(bank.numberofnodes+1);																	

							//Re-order nodes
							bank.nodes[counter+1] = Anode;																													
							for(int j = counter + 2; j <=bank.numberofnodes-1; j++)																		
								{																																	
									bank.nodes[j] = Abank.nodes[j-1];																					
								}																																	
						}	
				}	
		}

	//Updated top and bottom indices
	if(bank.direction == 1)
		{
			bank.material[0].topindex = bank.numberofnodes-1;
			if(bank.numberoflayers == 1)
				{
					bank.material[0].bottomindex = 0;
				}
			if(bank.numberoflayers > 1)
				{
					for(int ilayer = 0; ilayer <= bank.numberoflayers - 2; ilayer++)
						{
							counter = bank.material[ilayer].topindex;
							while(counter > 0 && bank.nodes[counter].elevation != bank.material[ilayer+1].elevation)
							//while(counter > 0 && fabs(bank.nodes[counter].elevation - bank.material[ilayer+1].elevation)>0.000001)//MMMMTEMP AdNo  Using a tolerance
								{
									counter = counter - 1;
								}
							bank.material[ilayer].bottomindex = counter;
							bank.material[ilayer+1].topindex = bank.material[ilayer].bottomindex-1;											
						}
					bank.material[bank.numberoflayers - 1].bottomindex = 0;
				}

		}		
	else if(bank.direction == -1)
		{
					bank.material[0].topindex = 0;
					if(bank.numberoflayers == 1)
						{
							bank.material[0].bottomindex = bank.numberofnodes-1;
						}
					if(bank.numberoflayers > 1)
						{
							for(int ilayer = 0; ilayer <= bank.numberoflayers - 2; ilayer++)
								{
									counter = bank.material[ilayer].topindex;
									while(counter < bank.numberofnodes-1 && bank.nodes[counter].elevation != bank.material[ilayer+1].elevation)
									//while(counter < bank.numberofnodes-1 && fabs(bank.nodes[counter].elevation - bank.material[ilayer+1].elevation)>0.000001)//MMMMTEMP AdNo  Using a tolerance
										{
											counter = counter + 1;
										}
									//if(inode == 0 && bank.direction == -1)printf("counterXXX = %d\n",counter);//MMMMTEMP AdNo
									bank.material[ilayer].bottomindex = counter;
									bank.material[ilayer+1].topindex = bank.material[ilayer].bottomindex+1;										
								}
							bank.material[bank.numberoflayers - 1].bottomindex = bank.numberofnodes-1;
						}
		}	


}


int RMBankErosion::UpdateMonitorNode(RMPrepData* prepData, RMMigrationData* migData, double& MonitorNodeStreamwiseDistance, int oldmonitornode)//MMMMTEMP Changes in UpdateMonitorNode routine
{
	int MonitorNode;
	double Distance, MinDistance, inodeMin;

	logmsg("Update monitor node\n");

	MonitorNode = 0;
	MinDistance = 9999999999999.9;

		for(int i = 0; i <= migData->channel.centerline.length - 1; i++)  //DM-FEB2011
		{
			Distance = abs(prepData->coords(i)*migData->channel.WIDTH/2 - MonitorNodeStreamwiseDistance);
			if(Distance < MinDistance)
				{
					MinDistance = Distance;
					inodeMin =  i;
				}
		}
	
	MonitorNode = inodeMin;
	
	//MonitorNodeStreamwiseDistance = prepData->coords(MonitorNode)*migData->channel.WIDTH/2;			//DM-FEB2011 Commented out


/*
	int i = 0;																						//MMMMTEMP Changes in UpdateMonitorNode routine
	while(prepData->inodeOldXsDS[i]!=oldmonitornode)i=i+1;											//MMMMTEMP Changes in UpdateMonitorNode routine
	if(prepData->wInterpolationDS(i)>0.5)MonitorNode = i;											//MMMMTEMP Changes in UpdateMonitorNode routine
	else MonitorNode = i-1;																			//MMMMTEMP Changes in UpdateMonitorNode routine
	*/

	return MonitorNode;
}

void RMBankErosion::ReachInterpolation(RMPrepData* prepData, RMMigrationData* migData, RMBankErosionData* bankErosionData, RMBankErosion* bankErosion)
{
	int MaxNumberXSNodesLeftBank;
	int inodeMaxLeft;
	int MaxNumberXSNodesRightBank;
	int inodeMaxRight;

	logmsg("Interpolate sections \n");

	//Store old geometry
    //NOO: removed this because we're doing copying in-place below.
    vector<CrossSection> copyOfXS;

	for(int i = 0; i <= bankErosionData->grids[0].numberofnodes-1; i++)
		{
			copyOfXS.push_back(bankErosionData->grids[0].nodes[i].xsection);
		}

	//Reinitialize XSs
	bankErosionData->ReInitBankProperties(migData->channel.centerline.length);
	bankErosionData->grids[0].SetNumberOfNodes(migData->channel.centerline.length);

	//Interpolate and Assign new geometry
	for(int i = 0; i <= migData->channel.centerline.length - 1; i++)								//ADDITION FEB2010
		{
			double DSweight = prepData->wInterpolationDS(i);

			Bank *Bank1, *Bank2;

			//Interpolate left bank
			Bank1 = &copyOfXS[prepData->inodeOldXsDS[i]-1].leftbank;		 //NOO Oldxsection --> xsection
			Bank2 = &copyOfXS[prepData->inodeOldXsDS[i]].leftbank;        //NOO Oldxsection --> xsection
			bankErosionData->grids[0].nodes[i].xsection.leftbank = bankErosion->BankInterpolatedVARIANT(Bank1, Bank2, DSweight, i, bankErosionData->BankInterpolationTolerance, bankErosionData->MonitorNode);//DM-FEB2012 Routine BankInterpolatedVARIANT is called instead of BankInterpolated (which produces more nodes on the right bank)

			//Interpolate right bank
			Bank1 = &copyOfXS[prepData->inodeOldXsDS[i]-1].rightbank; //NOO Oldxsection --> xsection
			Bank2 = &copyOfXS[prepData->inodeOldXsDS[i]].rightbank; //NOO Oldxsection --> xsection
			bankErosionData->grids[0].nodes[i].xsection.rightbank = bankErosion->BankInterpolatedVARIANT(Bank1, Bank2, DSweight, i, bankErosionData->BankInterpolationTolerance, bankErosionData->MonitorNode);//DM-FEB2012 Routine BankInterpolatedVARIANT is called instead of BankInterpolated (which produces more nodes on the right bank)

			//Interpolate the effective friction factor 
			bankErosionData->grids[0].nodes[i].xsection.effectivefrictionfactor = (copyOfXS[prepData->inodeOldXsDS[i]-1].effectivefrictionfactor + copyOfXS[prepData->inodeOldXsDS[i]].effectivefrictionfactor)/2;
		
			//Update station value of the centerline																																	//DM-FEB2011
			//This is done only for Type of physically-based migration = 2 (physically-based migration coefficient) that are for long periods of time that require bank interpolation	//DM-FEB2011
			//When using Type of physically-based migration = 1 do not use bank interpolation (i.e. no node-regrid or bank interpolation after splines)									//DM-FEB2011
			if(bankErosionData->MethodPhysicallyBasedBankEvolution == 2)																												//DM-FEB2011  
				{																																										//DM-FEB2011
					bankErosionData->StationCenterline[i] = bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[0].station-migData->channel.WIDTH/2;							//DM-FEB2011 
				}																																										//DM-FEB2011 
		
		}					

	//Computation of the maximum number of bank nodes
    MaxNumberXSNodesLeftBank = 0;
	MaxNumberXSNodesRightBank = 0;
	inodeMaxLeft = 0;
	inodeMaxRight = 0;

	for(int i = 0; i <= bankErosionData->grids[0].numberofnodes-1; i++)
		{
			if(bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes > MaxNumberXSNodesLeftBank)
				{
					MaxNumberXSNodesLeftBank = bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes;
					inodeMaxLeft = i;
				}
			if(bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes > MaxNumberXSNodesRightBank)
				{
					MaxNumberXSNodesRightBank = bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes;
					inodeMaxRight = i;
				}
		}  
	logmsg("	Max number of nodes in the left bank = %d, at the section = %d \n", MaxNumberXSNodesLeftBank, inodeMaxLeft);  //DM-FEB2012   
	logmsg("	Max number of nodes in the right bank = %d, at the section = %d \n", MaxNumberXSNodesRightBank, inodeMaxRight); //DM-FEB2012  
}


void RMBankErosion::ReadHeterogeneityGrid(RMBankErosionData* bankErosionData)		//HETEROGENEITY
{
	logmsg("ReadHeterogeneityGrid\n");
	
	FILE   *GridFile;	
	
	const char* filename = bankErosionData->floodplain_file.c_str();		//DM-FEB2011
	GridFile = fopen(filename, "r");										//DM-FEB2011

    int nCols = 0, nRows = 0;

	fscanf(GridFile,"%lf	%lf	%lf	%lf	%d	%d	%s",
        &bankErosionData->X_origin, &bankErosionData->Y_origin,
        &bankErosionData->DeltaX, &bankErosionData->DeltaY,
        &nCols, &nRows,
        &bankErosionData->Comment); 
	fscanf(GridFile,"%s", &bankErosionData->Header); 


    bankErosionData->Ncolumns = nCols;
    bankErosionData->Nrows = nRows;

	bankErosionData->X_grid.init(nCols, nRows);
    bankErosionData->Y_grid.init(nCols, nRows);
    bankErosionData->ErosionRate_grid.init(nCols, nRows);
    bankErosionData->CriticalTau_grid.init(nCols, nRows);
    bankErosionData->Unitweight_grid.init(nCols, nRows);
    bankErosionData->Cohesion_grid.init(nCols, nRows);
    bankErosionData->AngleRepose_grid.init(nCols, nRows);
    bankErosionData->AnglePhib_grid.init(nCols, nRows);
	bankErosionData->EC_grid.init(nCols, nRows);

	for(int i = 0; i <= bankErosionData->Ncolumns - 1; i++)							//Loop over columns
		{
			for(int j = 0; j <= bankErosionData->Nrows - 1; j++)					//Loop over rows
			{					
				fscanf(GridFile,"%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf \n",
                    &(bankErosionData->X_grid(i, j)),
                    &(bankErosionData->Y_grid(i, j)),
                    &(bankErosionData->EC_grid(i, j)),
                    &(bankErosionData->ErosionRate_grid(i, j)),
                    &(bankErosionData->CriticalTau_grid(i, j)),
                    &(bankErosionData->Unitweight_grid(i, j)),
                    &(bankErosionData->Cohesion_grid(i, j)),
                    &(bankErosionData->AngleRepose_grid(i, j)),
                    &(bankErosionData->AnglePhib_grid(i, j)));
			}			
		}
}


void RMBankErosion::ReadHeterogeneityGridWithLayers(RMBankErosionData* bankErosionData)		//HETEROGENEITY  //DM-FEB2011
{
	logmsg("ReadHeterogeneityGridWithLayers\n");

	FILE   *GridWithLayersFile;	

	const char* filename = bankErosionData->floodplain_file.c_str();		
	GridWithLayersFile = fopen(filename, "r");
	
    int nCols = 0, nRows = 0;
	int MaxNumberLayers = 0;

	fscanf(GridWithLayersFile,"%lf	%lf	%lf	%lf	%d	%d	%d	%s",
        &bankErosionData->X_origin, &bankErosionData->Y_origin,
        &bankErosionData->DeltaX, &bankErosionData->DeltaY,
        &nCols, &nRows, &MaxNumberLayers,
        &bankErosionData->Comment); 
	fscanf(GridWithLayersFile,"%s", &bankErosionData->Header); 

    bankErosionData->Ncolumns = nCols;
    bankErosionData->Nrows = nRows;
	bankErosionData->MaxNumberOfLayers = MaxNumberLayers;

	//Initialization
	bankErosionData->NoOfLayers_gridWL.resize(nCols);														
	for (int i = 0; i < nCols; i++)	bankErosionData->NoOfLayers_gridWL[i].resize(nRows);				
	
	bankErosionData->X_grid.init(nCols, nRows);
	bankErosionData->Y_grid.init(nCols, nRows);	
	bankErosionData->FloodplainElevation_gridWL.init(nCols, nRows);	//MMMMTEMP Floodplain elevation

	for(int ilayer = 0; ilayer <= bankErosionData->MaxNumberOfLayers - 1; ilayer++)
		{			
			bankErosionData->EC_gridWL[ilayer].init(nCols, nRows);												
			bankErosionData->ErosionRate_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->CriticalTau_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->Unitweight_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->Cohesion_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->AngleRepose_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->AnglePhib_gridWL[ilayer].init(nCols, nRows);
			bankErosionData->LayerElevation_gridWL[ilayer].init(nCols, nRows);
		}

	//Read
	for(int i = 0; i <= bankErosionData->Ncolumns - 1; i++)							//Loop over columns
		{
			for(int j = 0; j <= bankErosionData->Nrows - 1; j++)					//Loop over rows
			{	
				fscanf(GridWithLayersFile,"%lf	%lf	%d	\n",					
                    &(bankErosionData->X_grid(i, j)),								
                    &(bankErosionData->Y_grid(i, j)),								
					&(bankErosionData->NoOfLayers_gridWL[i][j]));					

				fscanf(GridWithLayersFile,"%lf\n",//MMMMTEMP Floodplain elevation
					&(bankErosionData->FloodplainElevation_gridWL(i, j)));			//MMMMTEMP Floodplain elevation

				for(int ilayer = 0; ilayer <= bankErosionData->MaxNumberOfLayers - 1; ilayer++)		
					{
						fscanf(GridWithLayersFile,"%lf	%lf	%lf	%lf	%lf	%lf	%lf	%lf \n",
							&(bankErosionData->EC_gridWL[ilayer](i, j)),
							&(bankErosionData->ErosionRate_gridWL[ilayer](i, j)),
							&(bankErosionData->CriticalTau_gridWL[ilayer](i, j)),
							&(bankErosionData->Unitweight_gridWL[ilayer](i, j)),
							&(bankErosionData->Cohesion_gridWL[ilayer](i, j)),
							&(bankErosionData->AngleRepose_gridWL[ilayer](i, j)),
							&(bankErosionData->AnglePhib_gridWL[ilayer](i, j)),
							&(bankErosionData->LayerElevation_gridWL[ilayer](i, j)));
					}

			}	
		}

}


void RMBankErosion::UpdateBankProperties(RMBankErosionData* bankErosionData, RMMigrationData* migData, RMPrepData* prepData)	//HETEROGENEITY  //DM-FEB2011
{
	logmsg("UpdateBankProperties\n");  

	double X_bar, Y_bar;

	//int  Mid;
	//Mid = int(migData->channel.nnodes/2.0 + 1.0);

	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			//Location
			//X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out
			//Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out

			//Left bank
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2-sin(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2+cos(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].erosionrate = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 2);	
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].criticalshearstress =InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 3);
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].unitweight = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 4);
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].cohesion = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 5);
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].reposeangle = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 6);
			bankErosionData->grids[0].nodes[i].xsection.leftbank.material[0].phib = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 7);
			
			//Right bank
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2+sin(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2-cos(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].erosionrate = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 2);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].criticalshearstress = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 3);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].unitweight = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 4);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].cohesion = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 5);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].reposeangle = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 6);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.material[0].phib = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 7);
		}

}

void RMBankErosion::UpdateBankPropertiesWithLayers(RMBankErosionData* bankErosionData, RMMigrationData* migData, RMPrepData* prepData)	//HETEROGENEITY  //DM-FEB2011
{
	logmsg("UpdateBankPropertiesWithLayers\n");

	double X_bar, Y_bar;
	int counter; 

/*
printf("BEFORE SetBottomAndTopIndex\n");//MMMMTEMP AdNo
if(bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.numberoflayers > 1)//MMMMTEMP AdNo
{//MMMMTEMP AdNo
printf("TopElev(BottomLayer) = %lf\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].elevation);//MMMMTEMP AdNo
printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[0].topindex,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[0].bottomindex);//MMMMTEMP AdNo
printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].topindex,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].bottomindex);//MMMMTEMP AdNo
}//MMMMTEMP AdNo
for(int k = 0; k<= bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.nodes[k].station,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.nodes[k].elevation);//MMMMTEMP
*/


	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			//Location
			//X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out
			//Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out

			///////////////////
			//Left bank
			///////////////////

			//Location
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2-sin(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2+cos(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011

			//Floodplain elevation

			bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 100);						//MMMMTEMP Floodplain elevation
			bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[0].elevation = bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation;						//MMMMTEMP Floodplain elevation
			bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[1].elevation = bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation;						//MMMMTEMP Floodplain elevation
			
			for(int m = 2; m <= bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes - 1; m++)																		//MMMMTEMP Floodplain elevation
				{																																									//MMMMTEMP Floodplain elevation
					if(bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m].elevation > bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation)			//MMMMTEMP Floodplain elevation
						{																																							//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m].elevation = bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation;		//MMMMTEMP Floodplain elevation
						}																																							//MMMMTEMP Floodplain elevation
				}																																									//MMMMTEMP Floodplain elevation

/*
			int m = 2;
			while(m <= bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes - 1)																						//MMMMTEMP Floodplain elevation
				{																																									//MMMMTEMP Floodplain elevation
					if(bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[m].elevation > bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation)			//MMMMTEMP Floodplain elevation
						{																																							//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes = bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes -1;				//MMMMTEMP Floodplain elevation
							for(int p = m; p <= bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes - 1; p++)														//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[p] = bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[p+1];						//MMMMTEMP Floodplain elevation
							m = 2;																																					//MMMMTEMP Floodplain elevation
						}																																							//MMMMTEMP Floodplain elevation
					else m = m+1;																																					//MMMMTEMP Floodplain elevation
				}																																									//MMMMTEMP Floodplain elevation
*/

			bankErosionData->grids[0].nodes[i].xsection.leftbank.groundwatertable = bankErosionData->grids[0].nodes[i].xsection.leftbank.FloodplainElevation;						//MMMMTEMP Floodplain elevation
			

			//Initial number of layers
			bankErosionData->grids[0].nodes[i].xsection.leftbank.initialnumberoflayers = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 0);
			bankErosionData->grids[0].nodes[i].xsection.leftbank.SetNumberOfLayers(bankErosionData->grids[0].nodes[i].xsection.leftbank.initialnumberoflayers);

			//Update layer elevations																																						//DM-FEB2012
			for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.leftbank.initialnumberoflayers - 1; ilayer++)															//DM-FEB2012
				{																																											//DM-FEB2012
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].elevation = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 8 + 8 * ilayer);	//DM-FEB2012 
				}																																											//DM-FEB2012

			//Compute current number of layers
			counter = bankErosionData->grids[0].nodes[i].xsection.leftbank.initialnumberoflayers - 1;

			while(counter >= 0 && bankErosionData->grids[0].nodes[i].xsection.leftbank.nodes[bankErosionData->grids[0].nodes[i].xsection.leftbank.numberofnodes-1].elevation > bankErosionData->grids[0].nodes[i].xsection.leftbank.material[counter].elevation)	
				{		
					counter = counter - 1;																																						
				}																																												
			bankErosionData->grids[0].nodes[i].xsection.leftbank.numberoflayers = counter+1;																										

			//Interpolate layer properties
			//for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.leftbank.numberoflayers - 1; ilayer++)  //MMMMTEMP Commented out
			for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.leftbank.initialnumberoflayers - 1; ilayer++)	//MMMMTEMP New version
				{		
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].erosionrate = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 2 + 8 * ilayer);	
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].criticalshearstress =InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 3 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].unitweight = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 4 + 8 * ilayer);	
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].cohesion = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 5 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].reposeangle = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 6 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.leftbank.material[ilayer].phib = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 7 + 8 * ilayer);
				}
			
			
			//Update top and bottom indices
			SetBottomAndTopIndex(bankErosionData->grids[0].nodes[i].xsection.leftbank,i);



			///////////////////
			//Right bank
			///////////////////

			//Location
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2+sin(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2-cos(prepData->theta(i))*migData->channel.WIDTH/2;//DM-FEB2011

			//Floodplain elevation

			bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 100);						//MMMMTEMP Floodplain elevation
			bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes-1].elevation = bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation;				//MMMMTEMP Floodplain elevation
			bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes-2].elevation = bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation;				//MMMMTEMP Floodplain elevation

			for(int m = 0; m <= bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes - 3; m++)																			//MMMMTEMP Floodplain elevation
				{																																										//MMMMTEMP Floodplain elevation
					if(bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m].elevation > bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation)			//MMMMTEMP Floodplain elevation
						{																																								//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m].elevation = bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation;		//MMMMTEMP Floodplain elevation
						}																																								//MMMMTEMP Floodplain elevation
				}																																										//MMMMTEMP Floodplain elevation

/*
			m = 0;
			while(m <= bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes - 3)																							//MMMMTEMP Floodplain elevation
				{																																										//MMMMTEMP Floodplain elevation
					if(bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[m].elevation > bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation)			//MMMMTEMP Floodplain elevation
						{																																								//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes = bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes -1;				//MMMMTEMP Floodplain elevation
							for(int p = m; p <= bankErosionData->grids[0].nodes[i].xsection.rightbank.numberofnodes - 1; p++)															//MMMMTEMP Floodplain elevation
							bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[p] = bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[p+1];							//MMMMTEMP Floodplain elevation
							m = 0;																																						//MMMMTEMP Floodplain elevation
						}																																								//MMMMTEMP Floodplain elevation
					else m = m+1;																																						//MMMMTEMP Floodplain elevation
				}																																										//MMMMTEMP Floodplain elevation
*/
	
			bankErosionData->grids[0].nodes[i].xsection.rightbank.groundwatertable = bankErosionData->grids[0].nodes[i].xsection.rightbank.FloodplainElevation;							//MMMMTEMP Floodplain elevation
			

			//Initial number of layers
			bankErosionData->grids[0].nodes[i].xsection.rightbank.initialnumberoflayers = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 0);
			bankErosionData->grids[0].nodes[i].xsection.rightbank.SetNumberOfLayers(bankErosionData->grids[0].nodes[i].xsection.rightbank.initialnumberoflayers);

			//Update layer elevations																																						//DM-FEB2012
			for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.rightbank.initialnumberoflayers - 1; ilayer++)														//DM-FEB2012
				{																																											//DM-FEB2012
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].elevation = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 8 + 8 * ilayer);	//DM-FEB2012
				}																																											//DM-FEB2012

			//Compute current number of layers
			counter = bankErosionData->grids[0].nodes[i].xsection.rightbank.initialnumberoflayers - 1;	

			while(counter >= 0 && bankErosionData->grids[0].nodes[i].xsection.rightbank.nodes[0].elevation > bankErosionData->grids[0].nodes[i].xsection.rightbank.material[counter].elevation)	
				{																																												
					counter = counter - 1;																																						
				}																																												
			bankErosionData->grids[0].nodes[i].xsection.rightbank.numberoflayers = counter+1;																										

			//Interpolate layer properties
			//for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.rightbank.numberoflayers - 1; ilayer++)  //MMMMTEMP Commented out
			for(int ilayer = 0; ilayer <= bankErosionData->grids[0].nodes[i].xsection.rightbank.initialnumberoflayers - 1; ilayer++)	//MMMMTEMP New version
				{		
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].erosionrate = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 2 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].criticalshearstress =InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 3 + 8 * ilayer);	
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].unitweight = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 4 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].cohesion = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 5 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].reposeangle = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 6 + 8 * ilayer);
					bankErosionData->grids[0].nodes[i].xsection.rightbank.material[ilayer].phib = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 7 + 8 * ilayer);
				}

			//Update top and bottom indices
			SetBottomAndTopIndex(bankErosionData->grids[0].nodes[i].xsection.rightbank,i);
		}

/*
printf("AFTER SetBottomAndTopIndex\n");//MMMMTEMP AdNo
if(bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.numberoflayers > 1)//MMMMTEMP AdNo
{//MMMMTEMP AdNo
printf("TopElev(BottomLayer) = %lf\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].elevation);//MMMMTEMP AdNo
printf("TopIndex(TopLayer) = %d, BottomIndex(TopLayer) = %d\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[0].topindex,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[0].bottomindex);//MMMMTEMP AdNo
printf("TopIndex(BottomLayer) = %d, BottomIndex(BottomLayer) = %d\n",bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].topindex,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.material[1].bottomindex);//MMMMTEMP AdNo
}//MMMMTEMP AdNo
for(int k = 0; k<= bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.numberofnodes-1; k++)printf("%d	%lf	%lf\n",k,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.nodes[k].station,bankErosionData->grids[0].nodes[bankErosionData->MonitorNode].xsection.leftbank.nodes[k].elevation);//MMMMTEMP AdNo
*/

}

void RMBankErosion::UpdateMigrationCoefficient(RMBankErosionData* bankErosionData, RMMigrationData* migData,RMPrepData* prepData)	//HETEROGENEITY  //DM-FEB2011
{
	logmsg("UpdateMigrationCoefficient\n");

	double X_bar, Y_bar;

	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			//Location
			//X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out 
			//Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out

			//Eo
			//bankErosionData->ECnu(i) = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 1);//DM-FEB2011 Commented out

			//Left bank
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2-sin(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2+cos(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			bankErosionData->ECnu_left(i) = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 1);								//DM-FEB2011

			//Right bank
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2+sin(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2-cos(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			bankErosionData->ECnu_right(i) = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 1);								//DM-FEB2011
		}
}

void RMBankErosion::UpdateMigrationCoefficientWithLayers(RMBankErosionData* bankErosionData, RMMigrationData* migData,RMPrepData* prepData)	//HETEROGENEITY  //DM-FEB2011
{
	logmsg("UpdateMigrationCoefficientWithLayers\n");

	double X_bar, Y_bar;

	for(int i = 0; i <= migData->channel.centerline.length-1; i++)
		{
			//Location
			//X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out
			//Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2;  //DM-FEB2011 Commented out

			//Eo
			//bankErosionData->ECnu(i) = InterpolatedValueFromGrid(bankErosionData, X_bar, Y_bar, 1);//DM-FEB2011 Commented out

			//Left bank	
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2-sin(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2+cos(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			bankErosionData->ECnu_left(i) = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 1);						//DM-FEB2011

			//Right bank
			X_bar = migData->channel.centerline(i,0)*migData->channel.WIDTH/2+sin(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			Y_bar = migData->channel.centerline(i,1)*migData->channel.WIDTH/2-cos(prepData->theta(i))*migData->channel.WIDTH/2;			//DM-FEB2011
			bankErosionData->ECnu_right(i) = InterpolatedValueFromGridWithLayers(bankErosionData, X_bar, Y_bar, 1);						//DM-FEB2011
		}
}


double RMBankErosion::InterpolatedValueFromGrid(RMBankErosionData* bankErosionData, double X_bar, double Y_bar, int IndexProperty)	//HETEROGENEITY  DM-FEB2011
{
	double InterpolatedValueFromGrid;

	double DistanceX, DistanceY;
	int i_bar, j_bar;
    double X_cell[2][2], Y_cell[2][2], Value_cell[2][2];
    double Distance00,Distance01,Distance10,Distance11;
	double Weight00,Weight01,Weight10,Weight11;
	bool PointOnLastColumn = false;	//DM-FEB2011
	bool PointOnLastRow = false;	//DM-FEB2011

	//Check if point is outside the grid
	if(bankErosionData->DeltaX > 0 && bankErosionData->DeltaY > 0)
		{
			if(X_bar < bankErosionData->X_grid(0, 0) || X_bar > bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar < bankErosionData->Y_grid(0, 0) || Y_bar > bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX < 0 && bankErosionData->DeltaY > 0)
		{
			if(X_bar > bankErosionData->X_grid(0, 0) || X_bar < bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar < bankErosionData->Y_grid(0, 0) || Y_bar > bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX > 0 && bankErosionData->DeltaY < 0)
		{
			if(X_bar < bankErosionData->X_grid(0, 0) || X_bar > bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar > bankErosionData->Y_grid(0, 0) || Y_bar < bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX < 0 && bankErosionData->DeltaY < 0)
		{
			if(X_bar > bankErosionData->X_grid(0, 0) || X_bar < bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar > bankErosionData->Y_grid(0, 0) || Y_bar < bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}

	//Locate the point in a grid-cell element
	DistanceX = fabs(X_bar - bankErosionData->X_origin);
	DistanceY = fabs(Y_bar - bankErosionData->Y_origin);
	i_bar = int(DistanceX/fabs(bankErosionData->DeltaX));	//Column index of the lower left corner (if DeltaX > 0) or lower right corner (if DeltaX < 0) of the grid cell containing the point of interest 
	j_bar = int(DistanceY/fabs(bankErosionData->DeltaY));	//Row index of the lower left corner (if DeltaX > 0) or lower right corner (if DeltaX < 0) of the grid cell containing the point of interest 

	//Case the point coincides with one of the grid nodes
	if(DistanceX/fabs(bankErosionData->DeltaX) == i_bar && DistanceY/fabs(bankErosionData->DeltaY) == j_bar)																				
		{																																									
			if (IndexProperty == 1)InterpolatedValueFromGrid = bankErosionData->EC_grid(i_bar, j_bar);
			else if (IndexProperty == 2)InterpolatedValueFromGrid = bankErosionData->ErosionRate_grid(i_bar, j_bar);
			else if(IndexProperty == 3)InterpolatedValueFromGrid = bankErosionData->CriticalTau_grid(i_bar, j_bar);
			else if(IndexProperty == 4)InterpolatedValueFromGrid = bankErosionData->Unitweight_grid(i_bar, j_bar);
			else if(IndexProperty == 5)InterpolatedValueFromGrid = bankErosionData->Cohesion_grid(i_bar, j_bar);
			else if(IndexProperty == 6)InterpolatedValueFromGrid = bankErosionData->AngleRepose_grid(i_bar, j_bar);
			else if(IndexProperty == 7)InterpolatedValueFromGrid = bankErosionData->AnglePhib_grid(i_bar, j_bar);
		}	
	//Case the point does not coincide with one of the grid nodes																																												
	else																																													
		{																																													
			//Correction of i_bar if the point is on the last column
			if(i_bar == bankErosionData->Ncolumns - 1  && j_bar < bankErosionData->Nrows - 1)			//DM-FEB2011		
				{																						//DM-FEB2011
					i_bar = i_bar-1;																	//DM-FEB2011
					PointOnLastColumn = true;															//DM-FEB2011
				}																						//DM-FEB2011																			
			//Correction of j_bar if the point is on the last row
			else if(i_bar < bankErosionData->Ncolumns - 1  && j_bar == bankErosionData->Nrows - 1)		//DM-FEB2011		
				{																						//DM-FEB2011
					j_bar = j_bar-1;																	//DM-FEB2011
					PointOnLastRow = true;																//DM-FEB2011
				}																						//DM-FEB2011																
			
			//Coordinates of the vertices of the grid cell containing the point 
			X_cell[0][0] = bankErosionData->X_grid(i_bar, j_bar);						
			X_cell[0][1] = bankErosionData->X_grid(i_bar, j_bar + 1);
			X_cell[1][0] = bankErosionData->X_grid(i_bar + 1, j_bar);
			X_cell[1][1] = bankErosionData->X_grid(i_bar + 1, j_bar + 1);
			Y_cell[0][0] = bankErosionData->Y_grid(i_bar, j_bar);					
			Y_cell[0][1] = bankErosionData->Y_grid(i_bar, j_bar + 1);
			Y_cell[1][0] = bankErosionData->Y_grid(i_bar + 1, j_bar);
			Y_cell[1][1] = bankErosionData->Y_grid(i_bar + 1, j_bar + 1);

			//Property values at the vertices of the grid cell containing the point 
			if (IndexProperty == 1)
				{
					Value_cell[0][0] = bankErosionData->EC_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->EC_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->EC_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->EC_grid(i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 2)
				{
					Value_cell[0][0] = bankErosionData->ErosionRate_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->ErosionRate_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->ErosionRate_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->ErosionRate_grid(i_bar + 1, j_bar + 1);
				}
			else if(IndexProperty == 3)
				{
					Value_cell[0][0] = bankErosionData->CriticalTau_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->CriticalTau_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->CriticalTau_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->CriticalTau_grid(i_bar + 1, j_bar + 1);
				}
			else if(IndexProperty == 4)
				{
					Value_cell[0][0] = bankErosionData->Unitweight_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->Unitweight_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->Unitweight_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->Unitweight_grid(i_bar + 1, j_bar + 1);
				}
			else if(IndexProperty == 5)
				{
					Value_cell[0][0] = bankErosionData->Cohesion_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->Cohesion_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->Cohesion_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->Cohesion_grid(i_bar + 1, j_bar + 1);
				}
			else if(IndexProperty == 6)
				{
					Value_cell[0][0] = bankErosionData->AngleRepose_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->AngleRepose_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->AngleRepose_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->AngleRepose_grid(i_bar + 1, j_bar + 1);
				}
			else if(IndexProperty == 7)
				{
					Value_cell[0][0] = bankErosionData->AnglePhib_grid(i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->AnglePhib_grid(i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->AnglePhib_grid(i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->AnglePhib_grid(i_bar + 1, j_bar + 1);
				}

			//Distances between point and four vertices of the grid cell that contains the point
			Distance00 = sqrt(pow((X_cell[0][0] - X_bar),2.0)+pow((Y_cell[0][0] - Y_bar),2.0));			
			Distance01 = sqrt(pow((X_cell[0][1] - X_bar),2.0)+pow((Y_cell[0][1] - Y_bar),2.0));
			Distance10 = sqrt(pow((X_cell[1][0] - X_bar),2.0)+pow((Y_cell[1][0] - Y_bar),2.0));
			Distance11 = sqrt(pow((X_cell[1][1] - X_bar),2.0)+pow((Y_cell[1][1] - Y_bar),2.0));

			//Case the point is on a vertical grid segment but not on the last column
			if (DistanceX/fabs(bankErosionData->DeltaX) == i_bar && !PointOnLastColumn)																																
				{
					Weight00 = 1/Distance00;
					Weight01 = 1/Distance01;
					Weight10 = 0.0;
					Weight11 = 0.0;	
				}		
			//Case the point is on the last column
			else if (PointOnLastColumn)																																
				{																																													
					Weight00 = 0.0;
					Weight01 = 0.0;
					Weight10 = 1/Distance10;
					Weight11 = 1/Distance11;
				}			
			//Case the point is on a horizontal grid segment but not on the last row
			else if (DistanceY/fabs(bankErosionData->DeltaY) == j_bar && !PointOnLastRow)																																
				{																																													
					Weight00 = 1/Distance00;
					Weight01 = 0.0;
					Weight10 = 1/Distance10;
					Weight11 = 0.0;		
				}				
			//Case the point is on the last row
			else if (PointOnLastRow)																																
				{
					Weight00 = 0.0;
					Weight01 = 1/Distance01;
					Weight10 = 0.0;
					Weight11 = 1/Distance11;	
				}				
			//Case the point is inside the grid cell
			else																		
				{
					Weight00 = 1/Distance00;
					Weight01 = 1/Distance01;
					Weight10 = 1/Distance10;
					Weight11 = 1/Distance11;
				} 

			//Interpolated value
			InterpolatedValueFromGrid = (Weight00 * Value_cell[0][0] + Weight01 * Value_cell[0][1] + Weight10 * Value_cell[1][0] + Weight11 * Value_cell[1][1])/(Weight00 + Weight01 + Weight10 + Weight11);

		}

	return InterpolatedValueFromGrid;   
}


double RMBankErosion::InterpolatedValueFromGridWithLayers(RMBankErosionData* bankErosionData, double X_bar, double Y_bar, int IndexProperty)	//HETEROGENEITY  //DM-FEB2011
{   
	double InterpolatedValueFromGrid;

	double DistanceX, DistanceY;
	int i_bar, j_bar;
    double X_cell[2][2], Y_cell[2][2], Value_cell[2][2];
    double Distance00,Distance01,Distance10,Distance11;
	double Weight00,Weight01,Weight10,Weight11;
	bool PointOnLastColumn = false;	//DM-FEB2011
	bool PointOnLastRow = false;	//DM-FEB2011

	//Check if point is outside the grid
	if(bankErosionData->DeltaX > 0 && bankErosionData->DeltaY > 0)
		{
			if(X_bar < bankErosionData->X_grid(0, 0) || X_bar > bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar < bankErosionData->Y_grid(0, 0) || Y_bar > bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX < 0 && bankErosionData->DeltaY > 0)
		{
			if(X_bar > bankErosionData->X_grid(0, 0) || X_bar < bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar < bankErosionData->Y_grid(0, 0) || Y_bar > bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX > 0 && bankErosionData->DeltaY < 0)
		{
			if(X_bar < bankErosionData->X_grid(0, 0) || X_bar > bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar > bankErosionData->Y_grid(0, 0) || Y_bar < bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}
	else if(bankErosionData->DeltaX < 0 && bankErosionData->DeltaY < 0)
		{
			if(X_bar > bankErosionData->X_grid(0, 0) || X_bar < bankErosionData->X_grid(bankErosionData->Ncolumns - 1, 0) || Y_bar > bankErosionData->Y_grid(0, 0) || Y_bar < bankErosionData->Y_grid(0, bankErosionData->Nrows - 1))			//Check if the point is inside the grid
				{
					logmsg ("Point outside grid = %lf %lf\n", X_bar,Y_bar);//DM-FEB2011
					exit(1);
				}
		}

	//Locate the point in a grid-cell element
	DistanceX = fabs(X_bar - bankErosionData->X_origin);
	DistanceY = fabs(Y_bar - bankErosionData->Y_origin);
	i_bar = int(DistanceX/fabs(bankErosionData->DeltaX));	//Column index of the lower left corner (if DeltaX > 0) or lower right corner (if DeltaX < 0) of the grid cell containing the point of interest 
	j_bar = int(DistanceY/fabs(bankErosionData->DeltaY));	//Row index of the lower left corner (if DeltaX > 0) or lower right corner (if DeltaX < 0) of the grid cell containing the point of interest 

	//Case the point coincides with one of the grid nodes
	if(DistanceX/fabs(bankErosionData->DeltaX) == i_bar && DistanceY/fabs(bankErosionData->DeltaY) == j_bar)																				
		{	
			if(IndexProperty == 0)InterpolatedValueFromGrid = bankErosionData->NoOfLayers_gridWL[i_bar][j_bar];
			else if (IndexProperty == 1)InterpolatedValueFromGrid = bankErosionData->EC_gridWL[0](i_bar, j_bar);
			else if (IndexProperty == 2 || IndexProperty == 10 || IndexProperty == 18 || IndexProperty == 26 || IndexProperty == 34) InterpolatedValueFromGrid = bankErosionData->ErosionRate_gridWL[(IndexProperty-2)/8](i_bar, j_bar);
			else if (IndexProperty == 3 || IndexProperty == 11 || IndexProperty == 19 || IndexProperty == 27 || IndexProperty == 35) InterpolatedValueFromGrid = bankErosionData->CriticalTau_gridWL[(IndexProperty-3)/8](i_bar, j_bar);
			else if (IndexProperty == 4 || IndexProperty == 12 || IndexProperty == 20 || IndexProperty == 28 || IndexProperty == 36) InterpolatedValueFromGrid = bankErosionData->Unitweight_gridWL[(IndexProperty-4)/8](i_bar, j_bar);
			else if (IndexProperty == 5 || IndexProperty == 13 || IndexProperty == 21 || IndexProperty == 29 || IndexProperty == 37) InterpolatedValueFromGrid = bankErosionData->Cohesion_gridWL[(IndexProperty-5)/8](i_bar, j_bar);
			else if (IndexProperty == 6 || IndexProperty == 14 || IndexProperty == 22 || IndexProperty == 30 || IndexProperty == 38) InterpolatedValueFromGrid = bankErosionData->AngleRepose_gridWL[(IndexProperty-6)/8](i_bar, j_bar);
			else if (IndexProperty == 7 || IndexProperty == 15 || IndexProperty == 23 || IndexProperty == 31 || IndexProperty == 39) InterpolatedValueFromGrid = bankErosionData->AnglePhib_gridWL[(IndexProperty-7)/8](i_bar, j_bar);
			else if (IndexProperty == 8 || IndexProperty == 16 || IndexProperty == 24 || IndexProperty == 32 || IndexProperty == 40) InterpolatedValueFromGrid = bankErosionData->LayerElevation_gridWL[(IndexProperty-8)/8](i_bar, j_bar);
			else if (IndexProperty == 100) InterpolatedValueFromGrid = bankErosionData->FloodplainElevation_gridWL(i_bar, j_bar); //MMMMTEMP Floodplain elevation
		}	
	//Case the point does not coincide with one of the grid nodes																																												
	else																																													
		{																																													
			//Correction of i_bar if the point is on the last column
			if(i_bar == bankErosionData->Ncolumns - 1  && j_bar < bankErosionData->Nrows - 1)			//DM-FEB2011		
				{																						//DM-FEB2011
					i_bar = i_bar-1;																	//DM-FEB2011
					PointOnLastColumn = true;															//DM-FEB2011
				}																						//DM-FEB2011																			
			//Correction of j_bar if the point is on the last row
			else if(i_bar < bankErosionData->Ncolumns - 1  && j_bar == bankErosionData->Nrows - 1)		//DM-FEB2011		
				{																						//DM-FEB2011
					j_bar = j_bar-1;																	//DM-FEB2011
					PointOnLastRow = true;																//DM-FEB2011
				}																						//DM-FEB2011																
			
			//Coordinates of the vertices of the grid cell containing the point 
			X_cell[0][0] = bankErosionData->X_grid(i_bar, j_bar);						
			X_cell[0][1] = bankErosionData->X_grid(i_bar, j_bar + 1);
			X_cell[1][0] = bankErosionData->X_grid(i_bar + 1, j_bar);
			X_cell[1][1] = bankErosionData->X_grid(i_bar + 1, j_bar + 1);
			Y_cell[0][0] = bankErosionData->Y_grid(i_bar, j_bar);					
			Y_cell[0][1] = bankErosionData->Y_grid(i_bar, j_bar + 1);
			Y_cell[1][0] = bankErosionData->Y_grid(i_bar + 1, j_bar);
			Y_cell[1][1] = bankErosionData->Y_grid(i_bar + 1, j_bar + 1);

			//Property values at the vertices of the grid cell containing the point 
			if (IndexProperty == 0)																	
				{																						
					Value_cell[0][0] = bankErosionData->NoOfLayers_gridWL[i_bar][j_bar];			    
					Value_cell[0][1] = bankErosionData->NoOfLayers_gridWL[i_bar][j_bar + 1];			
					Value_cell[1][0] = bankErosionData->NoOfLayers_gridWL[i_bar + 1][j_bar];			
					Value_cell[1][1] = bankErosionData->NoOfLayers_gridWL[i_bar + 1][j_bar + 1];		
				}																						
			else if (IndexProperty == 1)
				{
					Value_cell[0][0] = bankErosionData->EC_gridWL[0](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->EC_gridWL[0](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->EC_gridWL[0](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->EC_gridWL[0](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 2 || IndexProperty == 10 || IndexProperty == 18 || IndexProperty == 26 || IndexProperty == 34)
				{
					Value_cell[0][0] = bankErosionData->ErosionRate_gridWL[(IndexProperty-2)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->ErosionRate_gridWL[(IndexProperty-2)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->ErosionRate_gridWL[(IndexProperty-2)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->ErosionRate_gridWL[(IndexProperty-2)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 3 || IndexProperty == 11 || IndexProperty == 19 || IndexProperty == 27 || IndexProperty == 35)
				{
					Value_cell[0][0] = bankErosionData->CriticalTau_gridWL[(IndexProperty-3)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->CriticalTau_gridWL[(IndexProperty-3)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->CriticalTau_gridWL[(IndexProperty-3)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->CriticalTau_gridWL[(IndexProperty-3)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 4 || IndexProperty == 12 || IndexProperty == 20 || IndexProperty == 28 || IndexProperty == 36)
				{
					Value_cell[0][0] = bankErosionData->Unitweight_gridWL[(IndexProperty-4)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->Unitweight_gridWL[(IndexProperty-4)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->Unitweight_gridWL[(IndexProperty-4)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->Unitweight_gridWL[(IndexProperty-4)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 5 || IndexProperty == 13 || IndexProperty == 21 || IndexProperty == 29 || IndexProperty == 37)
				{
					Value_cell[0][0] = bankErosionData->Cohesion_gridWL[(IndexProperty-5)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->Cohesion_gridWL[(IndexProperty-5)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->Cohesion_gridWL[(IndexProperty-5)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->Cohesion_gridWL[(IndexProperty-5)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 6 || IndexProperty == 14 || IndexProperty == 22 || IndexProperty == 30 || IndexProperty == 38)
				{
					Value_cell[0][0] = bankErosionData->AngleRepose_gridWL[(IndexProperty-6)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->AngleRepose_gridWL[(IndexProperty-6)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->AngleRepose_gridWL[(IndexProperty-6)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->AngleRepose_gridWL[(IndexProperty-6)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 7 || IndexProperty == 15 || IndexProperty == 23 || IndexProperty == 31 || IndexProperty == 39)
				{
					Value_cell[0][0] = bankErosionData->AnglePhib_gridWL[(IndexProperty-7)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->AnglePhib_gridWL[(IndexProperty-7)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->AnglePhib_gridWL[(IndexProperty-7)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->AnglePhib_gridWL[(IndexProperty-7)/8](i_bar + 1, j_bar + 1);
				}
			else if (IndexProperty == 8 || IndexProperty == 16 || IndexProperty == 24 || IndexProperty == 32 || IndexProperty == 40) 
				{
					Value_cell[0][0] = bankErosionData->LayerElevation_gridWL[(IndexProperty-8)/8](i_bar, j_bar);
					Value_cell[0][1] = bankErosionData->LayerElevation_gridWL[(IndexProperty-8)/8](i_bar, j_bar + 1);
					Value_cell[1][0] = bankErosionData->LayerElevation_gridWL[(IndexProperty-8)/8](i_bar + 1, j_bar);
					Value_cell[1][1] = bankErosionData->LayerElevation_gridWL[(IndexProperty-8)/8](i_bar + 1, j_bar + 1);				
				}
			else if (IndexProperty == 100)																							//MMMMTEMP Floodplain elevation
				{																													//MMMMTEMP Floodplain elevation
					Value_cell[0][0] = bankErosionData->FloodplainElevation_gridWL(i_bar, j_bar);									//MMMMTEMP Floodplain elevation
					Value_cell[0][1] = bankErosionData->FloodplainElevation_gridWL(i_bar, j_bar + 1);								//MMMMTEMP Floodplain elevation
					Value_cell[1][0] = bankErosionData->FloodplainElevation_gridWL(i_bar + 1, j_bar);								//MMMMTEMP Floodplain elevation
					Value_cell[1][1] = bankErosionData->FloodplainElevation_gridWL(i_bar + 1, j_bar + 1);							//MMMMTEMP Floodplain elevation			
				}																													//MMMMTEMP Floodplain elevation

			//Distances between point and four vertices of the grid cell that contains the point
			Distance00 = sqrt(pow((X_cell[0][0] - X_bar),2.0)+pow((Y_cell[0][0] - Y_bar),2.0));			
			Distance01 = sqrt(pow((X_cell[0][1] - X_bar),2.0)+pow((Y_cell[0][1] - Y_bar),2.0));
			Distance10 = sqrt(pow((X_cell[1][0] - X_bar),2.0)+pow((Y_cell[1][0] - Y_bar),2.0));
			Distance11 = sqrt(pow((X_cell[1][1] - X_bar),2.0)+pow((Y_cell[1][1] - Y_bar),2.0));

			//Case the point is on a vertical grid segment but not on the last column
			if (DistanceX/fabs(bankErosionData->DeltaX) == i_bar && !PointOnLastColumn)																																
				{
					Weight00 = 1/Distance00;
					Weight01 = 1/Distance01;
					Weight10 = 0.0;
					Weight11 = 0.0;				
				}		
			//Case the point is on the last column
			else if (PointOnLastColumn)																																
				{																																													
					Weight00 = 0.0;
					Weight01 = 0.0;
					Weight10 = 1/Distance10;
					Weight11 = 1/Distance11;																																										
				}			
			//Case the point is on a horizontal grid segment but not on the last row
			else if (DistanceY/fabs(bankErosionData->DeltaY) == j_bar && !PointOnLastRow)																																
				{																																													
					Weight00 = 1/Distance00;
					Weight01 = 0.0;
					Weight10 = 1/Distance10;
					Weight11 = 0.0;																																										
				}				
			//Case the point is on the last row
			else if (PointOnLastRow)																																
				{
					Weight00 = 0.0;
					Weight01 = 1/Distance01;
					Weight10 = 0.0;
					Weight11 = 1/Distance11;				
				}				
			//Case the point is inside the grid cell
			else																		
				{
					Weight00 = 1/Distance00;
					Weight01 = 1/Distance01;
					Weight10 = 1/Distance10;
					Weight11 = 1/Distance11;
				} 

			//Interpolated value
			if (IndexProperty == 0)													
				{																	
					double max;														
					max = Weight00;													
					InterpolatedValueFromGrid =Value_cell[0][0];					
					if(Weight01 > max)												
						{															
							max = Weight01;											
							InterpolatedValueFromGrid = Value_cell[0][1];			
						}															
					if(Weight10 > max)												
						{															
							max = Weight10;											
							InterpolatedValueFromGrid = Value_cell[1][0];			
						}															
					if(Weight11 > max)												
						{															
							max = Weight11;											
							InterpolatedValueFromGrid = Value_cell[1][1];			
						}															
				}																	
			else																	
				{																	
					InterpolatedValueFromGrid = (Weight00 * Value_cell[0][0] + Weight01 * Value_cell[0][1] + Weight10 * Value_cell[1][0] + Weight11 * Value_cell[1][1])/(Weight00 + Weight01 + Weight10 + Weight11);
					if (IndexProperty == 8 || IndexProperty == 16 || IndexProperty == 24 || IndexProperty == 32 || IndexProperty == 40) InterpolatedValueFromGrid = RoundNumber(InterpolatedValueFromGrid, 6);//MMMMTEMP Round value
				}																	

		}
	return InterpolatedValueFromGrid;   
}

void RMBankErosion::CutoffPBA(RMMigrationData* migdata,RMBankErosionData* data)			// HETEROGENEITY  DM-FEB2011	//MMMMTEMP Cutoff with PB method
{
	int i = 0;
	int j;
	bool cutoff;
	double distance;
	double oldlength = migdata->channel.centerline.length;
	
	while (i <= migdata->channel.centerline.length-3)	//DM-FEB2011
		{
			cutoff = false;
			j = i+2;
			while(!cutoff && j <= migdata->channel.centerline.length-1)  
				{
					
					distance = sqrt(pow((migdata->channel.centerline(i,0)-migdata->channel.centerline(j,0)),2.0)+pow((migdata->channel.centerline(i,1)-migdata->channel.centerline(j,1)),2.0));		
					
					
					if (distance < migdata->CutoffThreshold)
						{
							logmsg("Cutoff \n");
							printf("Cutoff at %lf	%lf \n", migdata->channel.centerline(i,0)*migdata->channel.WIDTH/2, migdata->channel.centerline(i,1)*migdata->channel.WIDTH/2); 
							printf("Length before = %d\n",migdata->channel.centerline.length);
							cutoff = true;
							
							//New number of centerline nodes
							migdata->channel.centerline.length = migdata->channel.centerline.length - (j-i-1);
							printf("Length after = %d\n",migdata->channel.centerline.length);

							//Store old geometry
							vector<CrossSection> copyXS;
							for(int m = 0; m <= data->grids[0].numberofnodes-1; m++)
								{
									copyXS.push_back(data->grids[0].nodes[m].xsection);
								}

							//Reinitialize XSs
							data->ReInitBankProperties(migdata->channel.centerline.length);
							data->grids[0].SetNumberOfNodes(migdata->channel.centerline.length);

							for (int k = 0; k <= i; k++)
								{
									data->grids[0].nodes[k].xsection = copyXS[k]; 
								}
							for (int k = 1; k <= migdata->channel.centerline.length-j; k++)
								{
									data->grids[0].nodes[i+k].xsection = copyXS[j+k-1]; 
								}

							for (int k = 1; k <= migdata->channel.centerline.length-j; k++)
								{
									migdata->channel.centerline(i+k,0)= migdata->channel.centerline(j+k-1,0);	
									migdata->channel.centerline(i+k,1)= migdata->channel.centerline(j+k-1,1);
								}

						}
					j = j+1;

				}
			i = i+1;
		}

}

void RMBankErosion::AddAnotherBankNode(Bank *Abank, Coordinates topintersect, int& toe, int& top, int ilayer, int numberoflayers, int& topindex, int monitornode, int inode)//MMMMTEMP BottomCantileverBlock
{

	//if(inode == monitornode && Abank->direction == -1)printf("BEF (TopLayer)bottomindex = %d, (TopLayer)topindex = %d\n",Abank->material[0].bottomindex,Abank->material[0].topindex);//MMMMTEMP
	//if(inode == monitornode && Abank->direction == -1)if(Abank->numberoflayers>1)printf("BEF (BottomLayer)bottomindex = %d, (BottomLayer)topindex = %d\n",Abank->material[1].bottomindex,Abank->material[1].topindex);//MMMMTEMP

	AddNodeElevation(Abank->nodes, Abank->numberofnodes, topintersect);	

	//Set this variable in AddNodeElevation, rather than here.  Abank->numberofnodes = Abank->numberofnodes +1;				// Added by DM, since nodes is not an allocatable file as in CONCEPTS
					
	//  Reset toe of leftbank and top of rightbank
	if (Abank->direction == -1) {toe = toe + 1;}			// Left bank
	if (Abank->direction == 1) {top = top + 1;}			// Right bank
	
	//  Reset indices of soil layers
	if (Abank->direction == 1)							// Right bank
		{
			//if (ilayer-1>=0)
			for (int counter = 0; counter <= ilayer-1; counter++)
				{//Abank%material(1:ilayer-1)%bottomindex = Abank%material(1:ilayer-1)%bottomindex + 1
					Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
				}
			for (int counter = 0; counter <= ilayer; counter++)
				{//Abank%material(1:ilayer)%topindex = Abank%material(1:ilayer)%topindex + 1
					Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
				}
		}
	else if (Abank->direction == -1)						// Left bank
		{
			for (int counter = ilayer; counter <= numberoflayers-1; counter++)
				{//Abank%material(ilayer:numberoflayers)%bottomindex = Abank%material(ilayer:numberoflayers)%bottomindex + 1
					Abank->material[counter].bottomindex = Abank->material[counter].bottomindex + 1;
				}
			for (int counter = ilayer+1; counter <= numberoflayers-1; counter++)
				{//Abank%material(ilayer+1:numberoflayers)%topindex = Abank%material(ilayer+1:numberoflayers)%topindex + 1
					Abank->material[counter].topindex = Abank->material[counter].topindex + 1;
				}
		}
		
	// Set top index 
	if (Abank->direction == -1) {topindex = topindex+1;}		// Left bank				//WARNING2  //DM-FEB2011

	//if(inode == monitornode && Abank->direction == -1)printf("AFT (TopLayer)bottomindex = %d, (TopLayer)topindex = %d\n",Abank->material[0].bottomindex,Abank->material[0].topindex);//MMMMTEMP
	//if(inode == monitornode && Abank->direction == -1)if(Abank->numberoflayers>1)printf("AFT (BottomLayer)bottomindex = %d, (BottomLayer)topindex = %d\n",Abank->material[1].bottomindex,Abank->material[1].topindex);//MMMMTEMP
}

double RMBankErosion::RoundNumber(double value, int digits)//MMMMTEMP Round value
{
	double RoundNumber;

	RoundNumber = ceil( ( value * pow(10.0,digits) ) - 0.49 ) / pow(10.0,digits);

	return RoundNumber;
}