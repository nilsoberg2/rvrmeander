#include <vector>								//DM-FEB2011 For use of vector
#include "../rmbankerosion/rmbankerosion.h"		//DM-FEB2011 For use of vector
#include "rmmigration.h"
#include <math.h>




void RMMigrationData::Init(int length, int width)
{
	x.init(length, width);
	y.init(length, width);
	XX.init(length, width);
	YY.init(length, width);
}



RMMigration::~RMMigration()
{
}


void RMMigration::CalculateInitialWidth(RMBankErosionData* data, RMMigrationData* migdata)
{
	double width, localwidth;
	int firstnode, toeleft, toeright;

	firstnode = 0;

	// Width: method 1
	if(data->MethodUpdateWidth == 3)
		{
			width = 0;
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{
							toeleft = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1; 
							toeright = 0;
							width = width + (data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toeright].station - data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toeleft].station);
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = width/migdata->channel.centerline.length;	//DM-FEB2011
		}
	
	// Width: method 2
	else if(data->MethodUpdateWidth == 4)
		{	
			width = 1.7E+308;		// Max value in C++, we are looking for a minimum
			for(int igrid = 0; igrid<=data->numberofgrids-1; igrid++)
				{  
					for(int inode = 0; inode<=data->grids[igrid].numberofnodes-1; inode++)	
						{
						    toeleft = data->grids[igrid].nodes[inode].xsection.leftbank.numberofnodes - 1; 
							toeright = 0;
							localwidth = data->grids[igrid].nodes[inode].xsection.rightbank.nodes[toeright].station - data->grids[igrid].nodes[inode].xsection.leftbank.nodes[toeleft].station;
							if( localwidth < width)
								{
									width = localwidth;
								}
						}
					firstnode = firstnode+data->grids[igrid].numberofnodes;
				}
			migdata->channel.WIDTH = width;
		}
}


void RMMigration::CalculateDepthStraightChannel(RMMigrationData* migdata)	
{
	migdata->channel.D0 = migdata->channel.FLOW * migdata->channel.ManningCoefficient / migdata->channel.WIDTH / sqrt(migdata->channel.ValleySlope);	
	migdata->channel.D0 = pow(migdata->channel.D0,0.6);
}


void RMMigration::Cutoff(RMMigrationData* migdata)			// cutoff
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
							
							cutoff = true;
							migdata->CutoffHappened = true; //DM-FEB2011 

							for (int k = 1; k <= migdata->channel.centerline.length-j; k++)
								{
									migdata->channel.centerline(i+k,0)= migdata->channel.centerline(j+k-1,0);	
									migdata->channel.centerline(i+k,1)= migdata->channel.centerline(j+k-1,1);
								}

							//New number of centerline nodes
							migdata->channel.centerline.length = migdata->channel.centerline.length - (j-i-1);
						}
					j = j+1;

				}
			i = i+1;
		}
}



void RMMigration::CutoffPBA(RMMigrationData* migdata,RMBankErosionData* data)			// HETEROGENEITY  DM-FEB2011	//MMMMTEMP Cutoff with PB method
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
							//vector<CrossSection> copyOfXS;
							CrossSection copyXS[1000];//Need to change this
							for(int m = 0; m <= data->grids[0].numberofnodes-1; m++)
								{
									//copyOfXS.push_back(bankErosionData->grids[0].nodes[m].xsection);
									copyXS[m] = data->grids[0].nodes[m].xsection;//Need to change this
								}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Store old geometry
//vector<CrossSection> copyOfXS;
/*
CrossSection copyOfXS[5000];

	for(int m = 0; m <= data->grids[0].numberofnodes-1; m++)
		{
			//copyOfXS.push_back(bankErosionData->grids[0].nodes[m].xsection);
			copyOfXS[m] = data->grids[0].nodes[m].xsection;//Need to change this
		}

	//Reinitialize XSs
	data->ReInitBankProperties(migdata->channel.centerline.length);
	data->grids[0].SetNumberOfNodes(migdata->channel.centerline.length);
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





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

						}
					j = j+1;

				}
			i = i+1;
		}

}


bool RMMigration::Migrate(int iter,
									const RMPrepData* prepData,
									const RMBankErosionData* beData,
									RMMigrationData* data)
{	
	logmsg("Migrate\n");//DM-FEB2012
	ComputeNewCenterline(prepData, beData, data);

	return true;
}


bool RMMigration::Preprocess(int iter,
									   const RMPrepData* prepData,
									   RMMigrationData* data)
{
	// Computation of x's and y's for TecPlot visualization

	ComputeXandY(prepData, data);
	
	return true;
}


void RMMigration::ComputeNewCenterline(const RMPrepData* prepData,
												 const RMBankErosionData* beData,
												 RMMigrationData* data)
{   
#define CL data->channel.centerline
#define TH prepData->theta


	// All nodes except first and last ones
    for (int i = 1; i <= CL.length-2; i++)    
	{	
		CL(i,0) = CL(i,0) - beData->ErosionBank(i,1) * sin(TH(i));			//CORRECTION FEB 2010, CHANGE OF SIGN
		CL(i,1) = CL(i,1) + beData->ErosionBank(i,1) * cos(TH(i));			//CORRECTION FEB 2010, CHANGE OF SIGN
		//if(i == 115)printf("B %d	%lf	%lf %lf\n", i, - beData->ErosionBank(i,1) * sin(TH(i)), beData->ErosionBank(i,1) * cos(TH(i)),TH(i));
	}	
    
	// First node
	if (data->MigrateUpstreamNode)
	{
		CL(0,0) = CL(0,0) - beData->ErosionBank(0,1) * sin(TH(0));			//CORRECTION FEB 2010, CHANGE OF SIGN
		CL(0,1) = CL(0,1) + beData->ErosionBank(0,1) * cos(TH(0));			//CORRECTION FEB 2010, CHANGE OF SIGN
	}

	// Last node
	if (data->MigrateDownstreamNode)
	{
		CL(CL.length-1,0) = CL(CL.length-1,0) - beData->ErosionBank(CL.length-1,1) * sin(TH(CL.length-1));			//CORRECTION FEB 2010, CHANGE OF SIGN
		CL(CL.length-1,1) = CL(CL.length-1,1) + beData->ErosionBank(CL.length-1,1) * cos(TH(CL.length-1));			//CORRECTION FEB 2010, CHANGE OF SIGN
	}
	
#undef TH
#undef CL
}



void RMMigration::ComputeXandY(const RMPrepData* prepData,
										 RMMigrationData* data)
{
	#define CL data->channel.centerline
	data->Init(CL.length, data->channel.nnodes);			//CORRECTION AUG2009

/*  //DM-FEB2011 Commented out
	int Mid = int(data->channel.nnodes/2.0 + 1.0);

	for (int i = 0; i <= CL.length-1; i++)
		{
			data->x(i,Mid-1) = CL(i,0); 
			data->y(i,Mid-1) = CL(i,1);  
		}

	for (int i = 0; i <= CL.length-1; i++)
		{
			for(int j = 0; j <= Mid - 2; j++)
				{
					//data->x(i,j) = data->x(i,Mid-1) + prepData->coordn(j)*sin(prepData->theta(i)); 
					//data->y(i,j) = data->y(i,Mid-1) - prepData->coordn(j)*cos(prepData->theta(i));	
					data->x(i,j) = data->x(i,Mid-1) - prepData->coordn(j)*sin(prepData->theta(i));		//CORRECTION AUG2009
					data->y(i,j) = data->y(i,Mid-1) + prepData->coordn(j)*cos(prepData->theta(i));		//CORRECTION AUG2009	
				}
			for(int j = Mid; j <= data->channel.nnodes-1; j++)
				{
					//data->x(i,j) = data->x(i,Mid-1) + prepData->coordn(j)*sin(prepData->theta(i)); 
					//data->y(i,j) = data->y(i,Mid-1) - prepData->coordn(j)*cos(prepData->theta(i));
					data->x(i,j) = data->x(i,Mid-1) - prepData->coordn(j)*sin(prepData->theta(i));		//CORRECTION AUG2009
					data->y(i,j) = data->y(i,Mid-1) + prepData->coordn(j)*cos(prepData->theta(i));		//CORRECTION AUG2009
				}
		}
*/

//DM-FEB2011
	for (int i = 0; i <= CL.length-1; i++)
		{
			for(int j = 0; j <= data->channel.nnodes-1; j++)
				{
					data->x(i,j) = CL(i,0) - prepData->coordn(j)*sin(prepData->theta(i));		
					data->y(i,j) = CL(i,1) + prepData->coordn(j)*cos(prepData->theta(i));
				}
		}

#undef CL
}

double RMMigration::ComputeSCRCoeff(double depth, double sediment_size)
{
    return pow(6.0 + 2.5 * log(depth / (2.5*sediment_size)), -2.0);
}


double RMMigration::ComputeHCWDRatio(double depth, double width)
{
    return width / 2.0 / depth;
}


double RMMigration::ComputeFroudeSquared(double flow, double depth, double width)
{
    double u0 = flow / width / depth;
	return (u0 * u0) / (9.80665 * depth);
}


double RMMigration::ComputePPERate(double bank_erosion_coeff, double flow, double depth, double width, double num_years, double num_iter)
{
    double u0 = flow / width / depth;
    double temp = 2.0 * bank_erosion_coeff * u0 * num_years / width / num_iter;
   
    if (temp > 1.0)
        return -1.0;
    else
        return (temp * 3600.0 * 24.0 * 365.0);
	
}
