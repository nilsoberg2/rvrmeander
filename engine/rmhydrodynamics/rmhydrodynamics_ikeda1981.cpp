#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES		//DM-FEB2011  to use M_PI     //DM-FEB2011
#include <cmath>
#include "rmhydrodynamics_ikeda1981.h"
#include "../rmmigration/rmmigrationdata.h"
#include "../rmdebug/debug_printing.h"
#define TTMATH_NOASM 1 // http://www.ttmath.org/forum/how_to_compile_with_visual_studio_2010_64-bit
#include "ttmath/ttmath.h" //MMMMTEMP Secondary flow correction


void RMHydrodynamicsData_Ikeda1981::Init(int length, int width)
{
	InitBase(length, width);
}

void RMHydrodynamicsData_Ikeda1981::InitNewHydroValues(int length, int width)  //ADDITION MAR2010
{
	InitBaseNewHydroValues(length, width);
}

void RMHydrodynamicsData_Ikeda1981::InitOldHydroValues(int length, int width)  //ADDITION MAR2010
{
	InitBaseOldHydroValues(length, width);
}


RMHydrodynamics_Ikeda1981::RMHydrodynamics_Ikeda1981()
{
}


RMHydrodynamics_Ikeda1981::~RMHydrodynamics_Ikeda1981()
{
}


void RMHydrodynamics_Ikeda1981::Preprocess(int iter, const RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data_in)//DM-FEB2011
{
	ComputeXi(prepData, migData, data_in);
    //CalculateAlpha(migData, data_in);		//DM-FEB2011
}


void RMHydrodynamics_Ikeda1981::CalculateAlpha(const RMMigrationData* migData, RMHydrodynamicsData* data_in)
{
	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)data_in;
	data->ALF = 3.8*(1+(migData->channel.WIDTH/2)/6.96/(migData->channel.D0/data->xi)*exp(-6.96*(migData->channel.D0/data->xi)/(migData->channel.WIDTH/2)));
	data->ALF = 5.0;
	//logmsg("alpha = %lf \n", Ikmigdata->ALF);
}



void RMHydrodynamics_Ikeda1981::ComputeXi(const RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data_in)//DM-FEB2011
{
	#define CL migData->channel.centerline
	#define CLvalley migData->channel.valley_centerline
	
	double distancestraightx;   
	double distancestraighty;	
	double distancevalley = 0.0;	
	double m, xc, yc;
	
	if(migData->channel.use_valley_centerl == false)			//DM-FEB2011
	{	
			distancestraightx = (CL(CL.length-1,0)-CL(0,0));  
			distancestraighty = CL(CL.length-1,1)-CL(0,1);  
			distancevalley = sqrt(pow(distancestraightx,2.0)+pow(distancestraighty,2.0))*(migData->channel.WIDTH/2);
		}														//DM-FEB2011
	else if (migData->channel.use_valley_centerl == true)		//DM-FEB2011
		{	
			// Find the orthogonal projection of the upstream node of the channel centerline on the valley centerline: 
			// this becomes the new upstream node of the valley centerline
			//Case 1: valley direction is neither horizontal nor vertical																				//DM-FEB2011
			if((CLvalley(1,1) != CLvalley(0,1)) && (CLvalley(1,0)!=CLvalley(0,0)))																		//DM-FEB2011
				{																																		//DM-FEB2011
					m = (CLvalley(1,1)-CLvalley(0,1))/(CLvalley(1,0)-CLvalley(0,0));
					xc = (CL(0,1)*(migData->channel.WIDTH/2)+CL(0,0)*(migData->channel.WIDTH/2)/m-CLvalley(0,1)+m*CLvalley(0,0))/(m+1/m);
					yc = -1/m*xc+CL(0,1)*(migData->channel.WIDTH/2)+1/m*CL(0,0)*(migData->channel.WIDTH/2);
				}																																		//DM-FEB2011
			//Case 2: valley direction is horizontal																									//DM-FEB2011
			else if(CLvalley(1,1) == CLvalley(0,1))																										//DM-FEB2011
				{																																		//DM-FEB2011
					xc = CL(0,0)*(migData->channel.WIDTH/2);																							//DM-FEB2011
					yc = CLvalley(0,1);																													//DM-FEB2011
				}																																		//DM-FEB2011
			//Case 3: valley direction is vertical																										//DM-FEB2011
			else if(CLvalley(1,0) == CLvalley(0,0))																										//DM-FEB2011
				{																																		//DM-FEB2011
					xc = CLvalley(0,0);																													//DM-FEB2011
					yc = CL(0,1)*(migData->channel.WIDTH/2);																							//DM-FEB2011
				}																																		//DM-FEB2011
			CLvalley(0,0) = xc;
			CLvalley(0,1) = yc;

			// Find the orthogonal projection of the downstream node of the channel centerline on the valley centerline: 
			// this becomes the new downstream node of the valley centerline
			//Case 1: valley direction is neither horizontal nor vertical																				//DM-FEB2011
			if((CLvalley(CLvalley.length-1,1) != CLvalley(CLvalley.length-2,1)) && (CLvalley(CLvalley.length-1,0)!=CLvalley(CLvalley.length-2,0)))		//DM-FEB2011
				{																																		//DM-FEB2011
					m = (CLvalley(CLvalley.length-1,1)-CLvalley(CLvalley.length-2,1))/(CLvalley(CLvalley.length-1,0)-CLvalley(CLvalley.length-2,0));
					xc = (CL(CL.length-1,1)*(migData->channel.WIDTH/2)+CL(CL.length-1,0)*(migData->channel.WIDTH/2)/m-CLvalley(CLvalley.length-1,1)+m*CLvalley(CLvalley.length-1,0))/(m+1/m);//DM-FEB2011
					yc = -1/m*xc+CL(CL.length-1,1)*(migData->channel.WIDTH/2)+1/m*CL(CL.length-1,0)*(migData->channel.WIDTH/2);
				}																																		//DM-FEB2011
			//Case 2: valley direction is horizontal																									//DM-FEB2011
			else if(CLvalley(CLvalley.length-1,1) == CLvalley(CLvalley.length-2,1))																		//DM-FEB2011																															
				{																																		//DM-FEB2011
					xc = CL(CL.length-1,0)*(migData->channel.WIDTH/2);																					//DM-FEB2011
					yc = CLvalley(CLvalley.length-1,1);																									//DM-FEB2011
				}																																		//DM-FEB2011
			//Case 2: valley direction is vertical																										//DM-FEB2011
			else if(CLvalley(CLvalley.length-1,0) == CLvalley(CLvalley.length-2,0))																		//DM-FEB2011																													
				{																																		//DM-FEB2011
					xc = CLvalley(CLvalley.length-1,0);																									//DM-FEB2011
					yc = CL(CL.length-1,1)*(migData->channel.WIDTH/2);																					//DM-FEB2011
				}																																		//DM-FEB2011
			CLvalley(CLvalley.length-1,0) = xc;
			CLvalley(CLvalley.length-1,1) = yc;		
			
			// Calculate the length of the vally centerline
			for(int i = 0; i <= CLvalley.length-2; i++)				// Observe that distancevalley is in dimensions
				{
					distancestraightx = CLvalley(i+1,0)-CLvalley(i,0);   
					distancestraighty = CLvalley(i+1,1)-CLvalley(i,1); 
					distancevalley = distancevalley + sqrt(pow(distancestraightx,2.0)+pow(distancestraighty,2.0)); 
				}
		}														//DM-FEB2011

    ((RMHydrodynamicsData_Ikeda1981*)data_in)->xi = pow((distancevalley/ (prepData->sSum*(migData->channel.WIDTH/2))),0.33333333);

	#undef CL
	#undef CLvalley
}


void RMHydrodynamics_Ikeda1981::ComputeDimensionlessPerturbationUVHDE(const RMMigrationData* migData,
																	  const RMPrepData* prepData,
																	  RMHydrodynamicsData* data_in)
{
#define CL migData->channel.centerline
#define CH migData->channel
		
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////ADDITION SEP2010//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
double Dold,Tolerance,Cf,f,fprime,Dnew,U,S,dCfdD,dU2dD,Difference;		//For Netwon-Raphson method
int Iterations;															//For Netwon-Raphson method
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
	double xione,xi,xitwenty,As,HB,delta,Coeff,INTEGRAL,integralterm1,integralterm2,integralterm3;	//MMMMTEMP Secondary flow correction
	ttmath::Big<1,2> intg1,intg2,intg3,temp,INTEGRALTTMATH,CsecTTMATH;						//MMMMTEMP Secondary flow correction (definition of large number variables for computation of convolution integral for secondary flow

	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)data_in;

	if(migData->curvMethod.UpstreamCurvatureMethod == 1)			//DM-FEB2011
		{															//DM-FEB2011
			data->C_0 = prepData->curvature(0);						//DM-FEB2011
		}															//DM-FEB2011
	else if(migData->curvMethod.UpstreamCurvatureMethod == 3)		//DM-FEB2011
		{															//DM-FEB2011
			data->C_0 = 0.0;										//DM-FEB2011
		}															//DM-FEB2011			


	// Calculation of the flow velocity and Froude number for straight channel and calculation of BE (recalculated here fort safety)
	data->U0 = migData->channel.FLOW/(migData->channel.D0*migData->channel.WIDTH);
	data->F_02 = data->U0/sqrt(migData->channel.D0*9.80665);
	data->F_02 = pow(data->F_02,2.0);
	data->BE = CH.WIDTH/2/CH.D0;
	data->CF_0 = pow(6.0 + 2.5 * log(migData->channel.D0 / (2.5*migData->channel.SEDSIZE)), -2.0);	
    //data->CF_ch = pow(6.0 + 2.5 * log(migData->channel.D0 / (2.5*data->xi*migData->channel.SEDSIZE)), -2.0);   //ADDITION DEC2009  //DM-FEB2011 Commented out
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////ADDITION SEP2010//////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	//Iterative calculation of the reach-averaged uniform flow depth and velocity, using Newton-Raphson method	
	S = pow(data->xi,3.0) * migData->channel.ValleySlope;						// Channel slope at this particular time step
	logmsg("Channel slope = %lf \n", S);  
	Dold = CH.D0/data->xi;													// Initial guess for water depth
	Tolerance = 0.0000001;														// Tolerance for convergence
	Iterations = 0;																// Initialization of the number of iterations
	
	do
	{
		Iterations = Iterations + 1;											// Add one iteration

		Cf = pow((6.0+2.5*log(Dold/2.5/migData->channel.SEDSIZE)),-2.0);			// Friction coefficient for depth = Dold		
		U = migData->channel.FLOW/(migData->channel.WIDTH * Dold);		// Flow velocity for depth = Dold
		
		f = Cf * pow(U,2.0) - 9.80665*Dold*S;										// Function that has to be equal to zero   
		
		dCfdD = -2.0 * pow(6.0+2.5*log(Dold/2.5/migData->channel.SEDSIZE),-3.0) *2.5 / Dold;
		dU2dD = -2.0 * pow((migData->channel.FLOW/migData->channel.WIDTH), 2.0) * pow(Dold,-3.0);

		fprime = dCfdD* pow(U,2.0)+ Cf * dU2dD - 9.80665 * S;						// Derivative, with respect to Dold, of the function that has to be equal to zero		

		Dnew = Dold-f/fprime;													// New guess
		
		Difference = abs((Dnew-Dold)/Dold);										// Relative variation of the guess

		Dold = Dnew;															// Update Dold
	}
	while(Difference > Tolerance);

	data->D_ch = Dold;			//Reach-averaged uniform-flow depth
	data->U_ch = migData->channel.FLOW/(data->D_ch * migData->channel.WIDTH);				//Reach-averaged uniform-flow velocity
	data->F_02_ch = data->U_ch/sqrt(data->D_ch*9.80665);
	data->F_02_ch = pow(data->F_02_ch,2.0);														//Reach-averaged uniform-flow Froude number square
    data->CF_ch =  pow(6.0 + 2.5 * log(data->D_ch / (2.5*migData->channel.SEDSIZE)), -2.0);	// Reach-averaged friction coefficient
	data->BE_ch = CH.WIDTH/2.0/data->D_ch;															// Half width-to-depth ration
	logmsg("Iteration for D_ch = %d, D_ch = %lf, U_ch = %lf\n", Iterations, data->D_ch, data->U_ch);
	logmsg("F02_ch = %lf, Cf_ch = %lf, Beta_ch = %lf\n", data->F_02_ch, data->CF_ch, data->BE_ch);

	// Alternative computation of friction coefficient with Manning's roughness coefficient
	if(data->use_mann_for_fric_coeff == true)																																		//DM-FEB2012
		{																																											//DM-FEB2012
			data->D_ch = (migData->channel.FLOW*migData->channel.ManningCoefficient)/(migData->channel.WIDTH*pow(S,0.5));															//DM-FEB2012
			data->D_ch = pow(data->D_ch,0.6);																				//Reach-averaged uniform-flow depth						//DM-FEB2012
			data->U_ch = migData->channel.FLOW/(data->D_ch * migData->channel.WIDTH);										//Reach-averaged uniform-flow velocity					//DM-FEB2012
			data->F_02_ch = data->U_ch/sqrt(data->D_ch*9.80665);																													//DM-FEB2012
			data->F_02_ch = pow(data->F_02_ch,2.0);																			//Reach-averaged uniform-flow Froude number square		//DM-FEB2012
			data->CF_ch = 9.80665*pow(migData->channel.ManningCoefficient,2.0)/pow(data->D_ch,0.33333333);					// Reach-averaged friction coefficient					//DM-FEB2012
			data->BE_ch = CH.WIDTH/2.0/data->D_ch;																			// Half width-to-depth ration							//DM-FEB2012
			logmsg("Values computed using Manning's roughness coefficient, D_ch = %lf, U_ch = %lf\n", data->D_ch, data->U_ch);														//DM-FEB2012
			logmsg("F02_ch = %lf, Cf_ch = %lf, Beta_ch = %lf\n", data->F_02_ch, data->CF_ch, data->BE_ch);																			//DM-FEB2012
		}																																											//DM-FEB2012

	data_in->U_ch = data->U_ch;				//ADDITION SEP2010bis //DM-FEB2011

	printf("REACH-AVERAGED DEPTH = %lf\n",data->D_ch);//MMMMTEMP
	printf("STRESS = %lf\n",CH.WaterDensity *data->CF_ch*pow(data->U_ch,2.0));//MMMMTEMP
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	// Constants
	/* //DM-FEB2011 Commented out
	data->a2 = 2*data->CF_ch*data->BE*data->xi;     //CORRECTION DEC2009
	data->a3 = -data->xi;
	data->a4 = data->CF_ch*data->BE*( pow(data->xi,5.0)*data->F_02 + (data->ALF+1.0)* pow(data->xi,2.0) + 5*sqrt(data->CF_ch)*pow(data->xi,2.0)*(data->ALF+pow(data->xi,3.0)*data->F_02));   //CORRECTION DEC2009
	data->a4 = data->CF_ch*data->BE*( pow(data->xi,5.0)*data->F_02 + (data->ALF+1.0)* pow(data->xi,2.0)); //ADDITION SEP2010tris
	data->s2 = -5.0*(data->U0/CH.D0)*pow(data->xi,2.0)*pow(data->CF_ch,0.5);    //CORRECTION DEC2009  // Flat bed
	data->s2 = 0.0; //ADDITION SEP2010tris
	data->a6 = (data->U0/CH.D0)*data->F_02*pow(data->xi,5.0)+(data->U0/CH.D0)*(data->ALF-1)*pow(data->xi,2.0)-data->s2*(data->F_02*pow(data->xi,3.0)+data->ALF);
	*/ //DM-FEB2011 Commented out
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////ADDITION SEP2010//////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	// Parameters for correction for secondary flow																				
	HB = data->D_ch/(CH.WIDTH/2);																																					//MMMMTEMP Secondary flow correction
	xione= 0.077/sqrt(data->CF_ch);																																					//MMMMTEMP Secondary flow correction
	xi = xione - 1.0/3.0;																																							//MMMMTEMP Secondary flow correction
	xitwenty=1.0/pow(xione,3.0)*(pow(xi,3.0)+pow(xi,2.0)+2.0/5.0*xi+2.0/35.0);																																//MMMMTEMP Secondary flow correction
	As = 181.0 * pow(HB,2.0) / xione * (2.0 * pow(xi,2.0) + 4.0 / 5.0 * xi + 1.0 / 15.0);																							//MMMMTEMP Secondary flow correction
	delta = pow(xione,2.0)*(xi+1.0/4.0)/(1.0/12.0*pow(xi,2.0)+11.0/360.0*xi+1.0/504.0);																								//MMMMTEMP Secondary flow correction
	Coeff = delta * data->BE_ch * data->CF_ch;																																		//MMMMTEMP Secondary flow correction																										//DM-FEB2012
	RMSeries Csecflow;																																								//MMMMTEMP Secondary flow correction
	Csecflow.init(CL.length);																																						//MMMMTEMP Secondary flow correction
	INTEGRAL = 0.0;																																									//MMMMTEMP Secondary flow correction
	Csecflow(0) = 0.0;																																								//MMMMTEMP Secondary flow correction
	INTEGRALTTMATH = 0.0;																																							//MMMMTEMP Secondary flow correction

	for (int i = 1; i <= CL.length-1; i++)																																			//MMMMTEMP Secondary flow correction
		{																																											//MMMMTEMP Secondary flow correction
			//Method 1: Computation of secondary flow using trapezoidal rule for convolution integral
			//INTEGRAL = INTEGRAL + (prepData->curvature(i) * exp(Coeff*prepData->coords(i)) + prepData->curvature(i-1) * exp(Coeff*prepData->coords(i-1))) * (prepData->coords(i)-prepData->coords(i-1))/2.0;	
			//Csecflow(i) = Coeff * exp(-Coeff*prepData->coords(i)) * INTEGRAL;
			
			//Method 2: Computation of secondary flow using Simpson's rule for convolution integral
			//integralterm1 = prepData->curvature(i) * exp(Coeff*prepData->coords(i));
			//integralterm2 = prepData->curvature(i-1) * exp(Coeff*prepData->coords(i-1));
			//integralterm3 = 4.0 * (prepData->curvature(i) + prepData->curvature(i-1))/2.0 * exp(Coeff*((prepData->coords(i)+prepData->coords(i-1))/2.0));
			//INTEGRAL = INTEGRAL + (integralterm1 + integralterm2 + integralterm3) * (prepData->coords(i)-prepData->coords(i-1))/6.0;	
			//Csecflow(i) = Coeff * exp(-Coeff*prepData->coords(i)) * INTEGRAL;	

			//printf("%d	%lf	",i,Csecflow(i));//MMMMTEMP

			//Method 3: Computation of secondary flow using Simpson's rule for convolution integral and using library ttmath for handling large numbers (double variable type has E308 limit)

			temp = exp(Coeff*prepData->coords(i)/10);   
			intg1 = exp(Coeff*prepData->coords(i)/10);
			for(int k = 2; k <= 10; k++)intg1 = intg1*temp;
			temp =prepData->curvature(i);
			intg1 = temp*intg1;

			temp = exp(Coeff*prepData->coords(i-1)/10);
			intg2 = exp(Coeff*prepData->coords(i-1)/10);
			for(int k = 2; k <= 10; k++)intg2 = intg2*temp;
			temp =prepData->curvature(i-1);
			intg2 = temp*intg2;

			temp = exp(Coeff*((prepData->coords(i)+prepData->coords(i-1))/2.0)/10);
			intg3 = exp(Coeff*((prepData->coords(i)+prepData->coords(i-1))/2.0)/10);
			for(int k = 2; k <= 10; k++)intg3 = intg3*temp;
			temp = 4.0 * (prepData->curvature(i) + prepData->curvature(i-1))/2.0;
			intg3 = temp*intg3;

			temp = (prepData->coords(i)-prepData->coords(i-1))/6.0;
			INTEGRALTTMATH = INTEGRALTTMATH + (intg1 + intg2 + intg3) * temp;		

			temp = Coeff * exp(-Coeff*prepData->coords(i));
			CsecTTMATH =  INTEGRALTTMATH * temp;
			CsecTTMATH.ToDouble(Csecflow(i));

			//logmsg("SECFLOW	%lf	%lf	%lf\n",prepData->coords(i),prepData->curvature(i),Csecflow(i));//MMMMTEMP
			logmsg("SECFLOW	%d	%lf	%lf\n",i,prepData->curvature(i),Csecflow(i));//MMMMTEMP
			//if(i == 289)printf("SECFLOW	%lf\n",Csecflow(i));//MMMMTEMP
		}																																											//MMMMTEMP Secondary flow correction
	if(data->sec_flow_corr == true)logmsg("Secondary flow parameters D_ch/B = %lf,	xi1 = %lf,	xi = %lf,	As = %lf, delta = %lf, xitwenty = %lf\n", HB,xione,xi,As, delta,xitwenty);						//MMMMTEMP Secondary flow correction	

	// Optional amplification of friction coefficient
	if(data->amplify_fric_coeff == true)																				//DM-FEB2012
		{																												//DM-FEB2012
			data->CF_ch = data->CF_ch * data->ampl_fric_coeff;															//DM-FEB2012
		}																												//DM-FEB2012

	data->a2 = 2*data->CF_ch*data->BE_ch;   

	data->a3 = -1.0; 
	//data->a4 = data->CF_ch*data->BE_ch*(data->F_02_ch + (data->ALF+1.0) + 5*sqrt(data->CF_ch)*(data->ALF+data->F_02_ch)); //DM-FEB2011 Commented out
	data->a4 = data->CF_ch*data->BE_ch*(data->F_02_ch + (data->ALF+1.0));//ADDITION SEP2010tris
	if(data->sec_flow_corr == true)													//MMMMTEMP Secondary flow correction
		{																			//MMMMTEMP Secondary flow correction
			data->a4_SC1 = data->CF_ch*data->BE_ch*(data->F_02_ch + 1.0);			//MMMMTEMP Secondary flow correction
			data->a4_SC2 = data->CF_ch*data->BE_ch*(data->ALF+As);					//MMMMTEMP Secondary flow correction
		}																			//MMMMTEMP Secondary flow correction	
	data->s2 = -5.0*(data->U_ch/data->D_ch)*pow(data->CF_ch,0.5);    // Flat bed
	data->s2 = 0.0; //ADDITION SEP2010tris
	data->a6 = (data->U_ch/data->D_ch)*data->F_02_ch+(data->U_ch/data->D_ch)*(data->ALF-1)-data->s2*(data->F_02_ch+data->ALF);
	//printf("a2 = %lf, a3 = %lf, a4 = %lf, s2 = %lf, a6 = %lf \n", data->a2, data->a3, data->a4, data->s2, data->a6); 
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////


	//logmsg(" D0 = %lf, U0 = %lf\n", migData->channel.D0, data->U0);   
	//logmsg(" F02 = %lf, Cf0 = %lf, Beta = %lf, xi = %lf\n", data->F_02, data->CF_0, data->BE, data->xi);
	// Calculating u
	RMSeries INT;
	INT.init(CH.nnodes);

	RMSeries INT_SC;														//MMMMTEMP Secondary flow correction
	INT_SC.init(CH.nnodes);													//MMMMTEMP Secondary flow correction
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////ADDITION SEP2010//////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
    RMSeries INT_v;
	INT_v.init(CH.nnodes);
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	for (int j=0; j<=CH.nnodes-1; j++)
	{
		data->u(0,j)=data->UB_0 * prepData->coordn(j);
		//data->a1 = data->u(0,j) + data->xi * data->C_0 * prepData->coordn(j);  //DM-FEB2011 Commented out

		////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////ADDITION SEP2010//////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////

		data->a1 = data->u(0,j) + data->C_0 * prepData->coordn(j);

		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
		INT(j) = 0.0;
		INT_SC(j) = 0.0; //MMMMTEMP Secondary flow correction

		if (data->HydrodynamicsLastNodeMethod == 1)
		{	
			for (int i = 1; i <= CL.length-2; i++)
				{	
					//Method 1: Computation of secondary flow using trapezoidal rule for convolution integral
					INT(j) = INT(j) + (prepData->curvature(i) * exp(data->a2*prepData->coords(i)) + prepData->curvature(i-1) * exp(data->a2*prepData->coords(i-1))) * (prepData->coords(i)-prepData->coords(i-1))/2;
					//Method 2: Computation of secondary flow using Simpson's rule for convolution integral
					//double t1 = prepData->curvature(i) * exp(data->a2*prepData->coords(i));																		//MMMMTEMP Simpson's rule
					//double t2 = prepData->curvature(i-1) * exp(data->a2*prepData->coords(i-1));																	//MMMMTEMP Simpson's rule
					//double t3 = 4.0 * (prepData->curvature(i-1)+prepData->curvature(i))/2.0 * exp(data->a2*(prepData->coords(i-1)+prepData->coords(i))/2.0);		//MMMMTEMP Simpson's rule
					//INT(j) = INT(j) + (t1 + t2 + t3) * (prepData->coords(i)-prepData->coords(i-1))/6.0;															//MMMMTEMP Simpson's rule					
					if(data->sec_flow_corr == true)	INT_SC(j) = INT_SC(j) + (Csecflow(i) * exp(data->a2*prepData->coords(i)) + Csecflow(i-1) * exp(data->a2*prepData->coords(i-1))) * (prepData->coords(i)-prepData->coords(i-1))/2;  //MMMMTEMP Secondary flow correction
					data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4 * exp(-data->a2*prepData->coords(i)) * INT(j));
					if(data->sec_flow_corr == true)data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4_SC1 * exp(-data->a2*prepData->coords(i)) * INT(j) + data->a4_SC2 * exp(-data->a2*prepData->coords(i)) * INT_SC(j)); //MMMMTEMP Secondary flow correction
			}
			double mu = (data->u(CL.length-2,j)-data->u(CL.length-3,j)) / prepData->intervals(CL.length-2);
			data->u(CL.length-1,j) = data->u(CL.length-2,j) + mu * prepData->intervals(CL.length-1);    
		}
		else if (data->HydrodynamicsLastNodeMethod == 2)
		{  	
			for (int i = 1; i <= CL.length-1; i++)
			{
				//Method 1: Computation of secondary flow using trapezoidal rule for convolution integral
				INT(j) = INT(j) + (prepData->curvature(i)*exp(data->a2*prepData->coords(i))+prepData->curvature(i-1)*exp(data->a2*prepData->coords(i-1)))*(prepData->coords(i)-prepData->coords(i-1))/2;
				//Method 2: Computation of secondary flow using Simpson's rule for convolution integral
				//double t1 = prepData->curvature(i) * exp(data->a2*prepData->coords(i));																			//MMMMTEMP Simpson's rule
				//double t2 = prepData->curvature(i-1) * exp(data->a2*prepData->coords(i-1));																		//MMMMTEMP Simpson's rule
				//double t3 = 4.0 * (prepData->curvature(i-1)+prepData->curvature(i))/2.0 * exp(data->a2*(prepData->coords(i-1)+prepData->coords(i))/2.0);			//MMMMTEMP Simpson's rule
				//INT(j) = INT(j) + (t1 + t2 + t3) * (prepData->coords(i)-prepData->coords(i-1))/6.0;																//MMMMTEMP Simpson's rule
				if(data->sec_flow_corr == true)	INT_SC(j) = INT_SC(j) + (Csecflow(i) * exp(data->a2*prepData->coords(i)) + Csecflow(i-1) * exp(data->a2*prepData->coords(i-1))) * (prepData->coords(i)-prepData->coords(i-1))/2;  //MMMMTEMP Secondary flow correction
				data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4 * exp(-data->a2*prepData->coords(i)) * INT(j));
				if(data->sec_flow_corr == true)data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4_SC1 * exp(-data->a2*prepData->coords(i)) * INT(j) + data->a4_SC2 * exp(-data->a2*prepData->coords(i)) * INT_SC(j)); //MMMMTEMP Secondary flow correction
			}    
		}
		else if (data->HydrodynamicsLastNodeMethod == 3)																																						//DM-FEB2011
		{																																																		//DM-FEB2011
			for (int i = 1; i <= CL.length-2; i++)																																								//DM-FEB2011
			{																																																	//DM-FEB2011
				//Method 1: Computation of secondary flow using trapezoidal rule for convolution integral
				INT(j) = INT(j) + (prepData->curvature(i)*exp(data->a2*prepData->coords(i))+prepData->curvature(i-1)*exp(data->a2*prepData->coords(i-1)))*(prepData->coords(i)-prepData->coords(i-1))/2;		//DM-FEB2011
				//Method 2: Computation of secondary flow using Simpson's rule for convolution integral
				//double t1 = prepData->curvature(i) * exp(data->a2*prepData->coords(i));																			//MMMMTEMP Simpson's rule
				//double t2 = prepData->curvature(i-1) * exp(data->a2*prepData->coords(i-1));																		//MMMMTEMP Simpson's rule
				//double t3 = 4.0 * (prepData->curvature(i-1)+prepData->curvature(i))/2.0 * exp(data->a2*(prepData->coords(i-1)+prepData->coords(i))/2.0);			//MMMMTEMP Simpson's rule
				//INT(j) = INT(j) + (t1 + t2 + t3) * (prepData->coords(i)-prepData->coords(i-1))/6.0;																//MMMMTEMP Simpson's rule
				if(data->sec_flow_corr == true)	INT_SC(j) = INT_SC(j) + (Csecflow(i) * exp(data->a2*prepData->coords(i)) + Csecflow(i-1) * exp(data->a2*prepData->coords(i-1))) * (prepData->coords(i)-prepData->coords(i-1))/2;//MMMMTEMP Secondary flow correction
				data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4 * exp(-data->a2*prepData->coords(i)) * INT(j));				//DM-FEB2011
				if(data->sec_flow_corr == true)data->u(i,j) = data->a1*exp(-data->a2*prepData->coords(i)) + prepData->coordn(j) * (data->a3 * prepData->curvature(i) + data->a4_SC1 * exp(-data->a2*prepData->coords(i)) * INT(j) + data->a4_SC2 * exp(-data->a2*prepData->coords(i)) * INT_SC(j)); //MMMMTEMP Secondary flow correction
			}																																																	//DM-FEB2011
			data->u(CL.length-1,j) = data->u(CL.length-2,j);																																					//DM-FEB2011
		}																																																		//DM-FEB2011
	}

	for (int i = 0; i <= CL.length-1; i++)
	{
		data->ub(i) = data->u(i,0);//DM-FEB2011
	}

	// Calculating v, d and e 
	for (int j=0; j<=CH.nnodes-1; j++)
	{
		////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////ADDITION SEP2010//////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////

		//This is the integral from 1 to n of u(0,j)
		
		if(j == 0)				// Recall: at j = 0, n = 1; at j = CH.nnodes - 1, n = -1
			{
				INT_v(j) = 0;
			}
		else
			{
				INT_v(j) = INT_v(j-1) + (data->u(0,j) + data->u(0,j-1))*(prepData->coordn(j) - prepData->coordn(j-1))/2;
			}
   
		//logmsg("AAAAAAAAAAAAAAAAAAA =%lf  %lf , %lf \n", prepData->coordn(j), data->u(0,j), INT_v(j));

		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////// 

		if (data->HydrodynamicsLastNodeMethod == 1)								//ADDITION JAN2010
		{																		//ADDITION JAN2010
			for (int i = 0; i <= CL.length-2; i++)
			{   
				/* //DM-FEB2011 Commented out
				//data->a5 = migData->xi*prepData->dcurvatureds(i)*(1.0-migData->ALF)-pow(migData->xi,4.0)/migData->U0*migData->F_02*prepData->dcurvatureds(i)-(migData->CF_0/migData->U0)*data->a6*CH.WIDTH*prepData->curvature(i);
				data->a5 = data->xi*prepData->dcurvatureds(i)*(1.0-data->ALF)-pow(data->xi,4.0)*data->F_02*prepData->dcurvatureds(i)-(data->CF_ch/data->U0)*data->a6*CH.WIDTH/2*prepData->curvature(i);  //CORRECTION DEC2009 and SEP2010 
				//double DUDE = 0.0;   //CORRECTION JAN2010
				data->v(i,j) = (data->a2/2.0)*exp(-data->a2*prepData->coords(i))*(2*INT_v(j)-prepData->coordn(j)*data->u(0,j)+data->u(0,0))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);    //CORRECTION SEP2010           
				//data->v(i,j) = (DUDE/2.0)*data->a2*exp(-data->a2*prepData->coords(i))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);   //CORRECTION JAN2010
				//data->v(i,j) = -data->v(i,j);			//CORRECTION AUG2009
				data->e(i,j) = -data->ALF/data->xi * prepData->curvature(i) * prepData->coordn(j);									//CORRECTION AUG2009
				data->d(i,j) = (pow(data->xi,2.0)*data->F_02+data->ALF/data->xi)*prepData->curvature(i)*prepData->coordn(j);	//CORRECTION AUG2009
				*/  //DM-FEB2011 Commented out
				////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////ADDITION SEP2010//////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////

				data->a5 = prepData->dcurvatureds(i)*(1.0-data->ALF-data->F_02_ch)-(data->CF_ch/data->U_ch)*data->a6*CH.WIDTH/2*prepData->curvature(i);       
				data->v(i,j) = (data->a2/2.0)*exp(-data->a2*prepData->coords(i))*(2*INT_v(j)-prepData->coordn(j)*data->u(0,j)+data->u(0,0))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);   
				data->e(i,j) = -data->ALF * prepData->curvature(i) * prepData->coordn(j);
				if(data->sec_flow_corr == true) data->e(i,j) = -data->ALF * Csecflow(i) * prepData->coordn(j);//MMMMTEMP Secondary flow correction
				data->d(i,j) = (data->F_02_ch+data->ALF)*prepData->curvature(i)*prepData->coordn(j);
				if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP Secondary flow correction
				//if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)*xitwenty+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP xitwenty
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
			}
			double mv = (data->v(CL.length-2,j)-data->v(CL.length-3,j)) / prepData->intervals(CL.length-2);
			data->v(CL.length-1,j) = data->v(CL.length-2,j) + mv * prepData->intervals(CL.length-1);
			double me = (data->e(CL.length-2,j)-data->e(CL.length-3,j)) / prepData->intervals(CL.length-2);
			data->e(CL.length-1,j) = data->e(CL.length-2,j) + me * prepData->intervals(CL.length-1);
			double md = (data->d(CL.length-2,j)-data->d(CL.length-3,j)) / prepData->intervals(CL.length-2);
			data->d(CL.length-1,j) = data->d(CL.length-2,j) + md * prepData->intervals(CL.length-1);
		}																		//ADDITION JAN2010
		else if (data->HydrodynamicsLastNodeMethod == 2)						//ADDITION JAN2010
		{																		//ADDITION JAN2010
			for (int i = 0; i <= CL.length-1; i++)								//ADDITION JAN2010
			{																	//ADDITION JAN2010
				/*		//DM-FEB2011 Commented out
				//data->a5 = data->xi*prepData->dcurvatureds(i)*(1.0-data->ALF)-pow(data->xi,4.0)/migData->U0*data->F_02*prepData->dcurvatureds(i)-(data->CF_0/data->U0)*data->a6*CH.WIDTH*prepData->curvature(i);
				data->a5 = data->xi*prepData->dcurvatureds(i)*(1.0-data->ALF)-pow(data->xi,4.0)*data->F_02*prepData->dcurvatureds(i)-(data->CF_ch/data->U0)*data->a6*CH.WIDTH/2*prepData->curvature(i);  //CORRECTION DEC2009 and SEP2010tris
				//double DUDE = 0.0;   //CORRECTION JAN2010
            	data->v(i,j) = (data->a2/2.0)*exp(-data->a2*prepData->coords(i))*(2*INT_v(j)-prepData->coordn(j)*data->u(0,j)+data->u(0,0))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);    //CORRECTION SEP2010tris  
				//data->v(i,j) = (DUDE/2.0)*data->a2*exp(-data->a2*prepData->coords(i))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);   //CORRECTION JAN2010
				//data->v(i,j) = -data->v(i,j);			//CORRECTION AUG2009
				data->e(i,j) = -data->ALF/data->xi * prepData->curvature(i) * prepData->coordn(j);									//CORRECTION AUG2009
				data->d(i,j) = (pow(data->xi,2.0)*data->F_02+data->ALF/data->xi)*prepData->curvature(i)*prepData->coordn(j);	//CORRECTION AUG2009
				*/		//DM-FEB2011 Commented out
				////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////ADDITION SEP2010tris//////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////

				data->a5 = prepData->dcurvatureds(i)*(1.0-data->ALF-data->F_02_ch)-(data->CF_ch/data->U_ch)*data->a6*CH.WIDTH/2*prepData->curvature(i);             
				data->v(i,j) = (data->a2/2.0)*exp(-data->a2*prepData->coords(i))*(2*INT_v(j)-prepData->coordn(j)*data->u(0,j)+data->u(0,0))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);   
				data->e(i,j) = -data->ALF * prepData->curvature(i) * prepData->coordn(j);	
				if(data->sec_flow_corr == true) data->e(i,j) = -data->ALF * Csecflow(i) * prepData->coordn(j);//MMMMTEMP Secondary flow correction
				data->d(i,j) = (data->F_02_ch+data->ALF)*prepData->curvature(i)*prepData->coordn(j);
				if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP Secondary flow correction
				//if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)*xitwenty+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP xitwenty
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////					

			}																	//ADDITION JAN2010
		}																		//ADDITION JAN2010
		else if (data->HydrodynamicsLastNodeMethod == 3)																																																			//DM-FEB2011
		{																																																															//DM-FEB2011
			for (int i = 0; i <= CL.length-2; i++)																																																					//DM-FEB2011
			{																																																														//DM-FEB2011
				data->a5 = prepData->dcurvatureds(i)*(1.0-data->ALF-data->F_02_ch)-(data->CF_ch/data->U_ch)*data->a6*CH.WIDTH/2*prepData->curvature(i);																												//DM-FEB2011    
				data->v(i,j) = (data->a2/2.0)*exp(-data->a2*prepData->coords(i))*(2*INT_v(j)-prepData->coordn(j)*data->u(0,j)+data->u(0,0))+(data->a2/2.0)*(prepData->coordn(j)*data->u(i,j)-data->u(i,0))+(data->a5/2.0)*(pow(prepData->coordn(j),2.0)-1.0);		//DM-FEB2011
				data->e(i,j) = -data->ALF * prepData->curvature(i) * prepData->coordn(j);																																											
				if(data->sec_flow_corr == true) data->e(i,j) = -data->ALF * Csecflow(i) * prepData->coordn(j);//MMMMTEMP Secondary flow correction																																													
				data->d(i,j) = (data->F_02_ch+data->ALF)*prepData->curvature(i)*prepData->coordn(j);																																								//DM-FEB2011
				if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP Secondary flow correction
				//if(data->sec_flow_corr == true) data->d(i,j) = (data->F_02_ch*prepData->curvature(i)*xitwenty+data->ALF*Csecflow(i))*prepData->coordn(j); //MMMMTEMP xitwenty
			}																																																														//DM-FEB2011
			data->v(CL.length-1,j) = data->v(CL.length-2,j);																																																		//DM-FEB2011
			data->e(CL.length-1,j) = data->e(CL.length-2,j);																																																		//DM-FEB2011
			data->d(CL.length-1,j) = data->d(CL.length-2,j);																																																		//DM-FEB2011
		}																																																															//DM-FEB2011	
	}		

	// Calculating h
	for (int j=0; j<=CH.nnodes-1; j++)
	{
		for (int i = 0; i <= CL.length-1; i++)
		{
			//data->h(i,j) = (data->e(i,j) + data->d(i,j))/data->F_02;	//DM-FEB2011 Commented out	
			////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////ADDITION SEP2010//////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
			
			//data->h(i,j) = (data->e(i,j) + data->d(i,j))/data->F_02_ch;	//MMMMTEMP //Commented out
			data->h(i,j) = data->e(i,j) + data->d(i,j);	//MMMMTEMP //New version
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
		}
	}

    /////////LINES TO CHECK (KEEP COMMENTED OUT)
	//for (int j = 0; j <= CH.nnodes-1; j++)
	//	{
	//		logmsg("j = %d, coordn = %lf, u = %lf \n", j, prepData->coordn(j), data->u(0,j)); 
	//	}
	////////

#undef CH
#undef CL
}



void RMHydrodynamics_Ikeda1981::ComputeDimensionedPerturbationUVHDE(const RMPrepData* prepData,
                                                                    RMMigrationData* migData,
																	RMHydrodynamicsData* data_in)
{
#define CL migData->channel.centerline
#define CH migData->channel

	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)data_in;

	for (int j=0; j<=CH.nnodes-1; j++)
	{
		for (int i = 0; i <= CL.length-1; i++)
		{
			
			migData->XX(i,j) = migData->x(i,j)*CH.WIDTH/2;
			migData->YY(i,j) = migData->y(i,j)*CH.WIDTH/2;
			/*		//DM-FEB2011 Commented out
			data->U1(i,j) = data->u(i,j)*data->U0;
			data->V1(i,j) = data->v(i,j)*data->U0;
			data->H1(i,j) = data->h(i,j)*pow(data->U0,2.0)/9.80665;		//CORRECTION AUG2009
			data->D1(i,j) = data->d(i,j)*CH.D0;		
			data->E1(i,j) = data->e(i,j)*CH.D0;	
			*/		//DM-FEB2011 Commented out
			////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////ADDITION SEP2010//////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
			
			data->U1(i,j) = data->u(i,j)*data->U_ch;
			data->V1(i,j) = data->v(i,j)*data->U_ch;
			//data->H1(i,j) = data->h(i,j)*pow(data->U_ch,2.0)/9.80665;	//MMMMTEMP //Commented out
			data->H1(i,j) = data->h(i,j)*data->D_ch;//MMMMTEMP	//New version
			data->D1(i,j) = data->d(i,j)*data->D_ch;		
			data->E1(i,j) = data->e(i,j)*data->D_ch;
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
		}
	}

#undef CH
#undef CL
}

void RMHydrodynamics_Ikeda1981::ComputeDimensionedFlowField(const RMMigrationData* migData,
															const RMPrepData* prepData,
															RMHydrodynamicsData* data_in)
{
#define CL migData->channel.centerline
#define CH migData->channel
	
	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)data_in;
	double DeviationAngle;//DM-FEB2011

    for (int j=0; j<=CH.nnodes-1; j++)
    {
	    for (int i = 0; i <= CL.length-1; i++)
	    {
			/*  //DM-FEB2011 Commented out
			data->UU(i,j) = data->U0 * data->xi + data->U1(i,j);
			//logmsg("VELOCITY = %lf \n", migData->U0);
			data->VV(i,j) = data->V1(i,j);
			data->DD(i,j) = CH.D0 / data->xi + data->D1(i,j);
			*/   //DM-FEB2011 Commented out
			data->EE(i,j) = CH.USBed - prepData->coords(i) * (CH.WIDTH/2.0) * CH.ValleySlope * pow(data->xi, 3.0) + data->E1(i,j);	//CORRECTION DEC2009	

			////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////ADDITION SEP2010//////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////	
			
			data->UU(i,j) = data->U_ch + data->U1(i,j);
			data->VV(i,j) = data->V1(i,j);
			if(j == 0 || j == CH.nnodes-1)  //ADDITION NOV2010quatris
				{//ADDITION NOV2010quatris
					data->VV(i,j)=0.0;//ADDITION NOV2010quatris
				}//ADDITION NOV2010quatris
			data->DD(i,j) = data->D_ch + data->D1(i,j);
			
			if(data->VV(i,j)==0 && data->UU(i,j) == 0)														//DM-FEB2011
				{																							//DM-FEB2011
					DeviationAngle = 0.0;																	//DM-FEB2011
				}																							//DM-FEB2011
			else																							//DM-FEB2011
				{																							//DM-FEB2011
					if(data->UU(i,j) >= 0)DeviationAngle = atan(data->VV(i,j)/data->UU(i,j));				//DM-FEB2011
					else DeviationAngle = atan(data->VV(i,j)/data->UU(i,j))+M_PI;							//DM-FEB2011
				}																							//DM-FEB2011
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////

			data->HH(i,j) = data->DD(i,j) + data->EE(i,j);
			data->VEL(i,j) = sqrt(pow(data->UU(i,j),2.0)+pow(data->VV(i,j),2.0));
			//data->Ux(i,j) = data->VEL(i,j)*cos(prepData->theta(i));//DM-FEB2011 Commented out
			//data->Vy(i,j) = data->VEL(i,j)*sin(prepData->theta(i));//DM-FEB2011 Commented out
			data->Ux(i,j) = data->VEL(i,j)*cos(prepData->theta(i)+DeviationAngle);//DM-FEB2011
			data->Vy(i,j) = data->VEL(i,j)*sin(prepData->theta(i)+DeviationAngle);//DM-FEB2011
			data->CF(i,j) = data->CF_ch;  //CORRECTION DEC2009
			//data->CF(i,j) = ComputeSCRCoeff(data->DD(i,j), data->channel.SEDSIZE);
			data->TAU(i,j) = CH.WaterDensity * data->CF(i,j) * pow(data->VEL(i,j),2.0);
			//data->TauS(i,j) = CH.WaterDensity * data->CF(i,j) * pow(data->UU(i,j),2.0);		//ADDITION MAY 2010   //DM-FEB2011 Commented out
			data->TauS(i,j) = CH.WaterDensity * data->CF(i,j) * data->UU(i,j)*data->VEL(i,j);   //ADDITION NOV2010quatris
            //data->TauS(i,j) = data->TauS(i,j) *0.5;
			//data->TauN(i,j) = CH.WaterDensity * data->CF(i,j) * pow(data->VV(i,j),2.0);		//ADDITION MAY 2010    //DM-FEB2011 Commented out
			data->TauN(i,j) = CH.WaterDensity * data->CF(i,j) * data->VV(i,j)*data->VEL(i,j);   //ADDITION NOV2010quatris
			//data->TauX(i,j) = data->TAU(i,j)*cos(prepData->theta(i));//DM-FEB2011 Commented out
			//data->TauY(i,j) = data->TAU(i,j)*sin(prepData->theta(i));//DM-FEB2011 Commented out
			data->TauX(i,j) = data->TAU(i,j)*cos(prepData->theta(i)+DeviationAngle);//DM-FEB2011
			data->TauY(i,j) = data->TAU(i,j)*sin(prepData->theta(i)+DeviationAngle);//DM-FEB2011
        }
	}

#undef CH
#undef CL
}



void RMHydrodynamics_Ikeda1981::StoreHydrodynamics (RMMigrationData* migData, RMHydrodynamicsData* data)   //ADDITION MAR2010
{
	logmsg("Store hydrodynamics \n");

	for (int j=0; j<=migData->channel.nnodes-1; j++)
		{
			for (int i = 0; i <= migData->channel.centerline.length-1; i++)
				{
					data->OldUU(i,j) = data->UU(i,j);
					data->OldVV(i,j) = data->VV(i,j);
					data->OldDD(i,j) = data->DD(i,j);
					data->OldEE(i,j) = data->EE(i,j);		
					data->OldHH(i,j) = data->HH(i,j);
					data->OldVEL(i,j) = data->VEL(i,j);
					data->OldUx(i,j) = data->Ux(i,j);
					data->OldVy(i,j) = data->Vy(i,j);
					data->OldCF(i,j) = data->CF(i,j);
					data->OldTAU(i,j) = data->TAU(i,j);
					data->OldTauX(i,j) = data->TauX(i,j);
					data->OldTauY(i,j) = data->TauY(i,j);
				}
		}
}


void RMHydrodynamics_Ikeda1981::InterpolateHydrodynamics (RMPrepData* prepData, RMMigrationData* migData, RMHydrodynamicsData* data, double tolerance)   //ADDITION MAR2010  //DM-FEB2011
{
	double DSweight;
	int DSnode;

	logmsg("Interpolate hydrodynamics\n");
	
	for (int j=0; j<=migData->channel.nnodes-1; j++)
		{
			for (int i = 0; i <= migData->channel.centerline.length-1; i++)
				{ 
					DSweight = prepData->wInterpolationDS(i);
					if(tolerance <= 0)DSweight = 1.0; //DM-FEB2011
					DSnode = prepData->inodeOldXsDS[i];

					data->UU(i,j) = (1-DSweight)*data->OldUU(DSnode-1,j)+DSweight*data->OldUU(DSnode,j);
					data->VV(i,j) = (1-DSweight)*data->OldVV(DSnode-1,j)+DSweight*data->OldVV(DSnode,j);
					data->DD(i,j) = (1-DSweight)*data->OldDD(DSnode-1,j)+DSweight*data->OldDD(DSnode,j);
					data->EE(i,j) = (1-DSweight)*data->OldEE(DSnode-1,j)+DSweight*data->OldEE(DSnode,j);		
					data->HH(i,j) = (1-DSweight)*data->OldHH(DSnode-1,j)+DSweight*data->OldHH(DSnode,j);
					data->VEL(i,j) = (1-DSweight)*data->OldVEL(DSnode-1,j)+DSweight*data->OldVEL(DSnode,j);
					data->Ux(i,j) = (1-DSweight)*data->OldUx(DSnode-1,j)+DSweight*data->OldUx(DSnode,j);
					data->Vy(i,j) = (1-DSweight)*data->OldVy(DSnode-1,j)+DSweight*data->OldVy(DSnode,j);
					data->CF(i,j) = (1-DSweight)*data->OldCF(DSnode-1,j)+DSweight*data->OldCF(DSnode,j); 
					data->TAU(i,j) = (1-DSweight)*data->OldTAU(DSnode-1,j)+DSweight*data->OldTAU(DSnode,j);
					data->TauX(i,j) = (1-DSweight)*data->OldTauX(DSnode-1,j)+DSweight*data->OldTauX(DSnode,j);
					data->TauY(i,j) = (1-DSweight)*data->OldTauY(DSnode-1,j)+DSweight*data->OldTauY(DSnode,j);	

				}
		}
}
void RMHydrodynamics_Ikeda1981::InputHydrograph(int j, double& flow, double& intermittency)  //DM-FEB2012
{
	//Stepwise hydrograph (value directly assigned at each iteration)
	//Manually edit values of flow and intermittency as needed
	/*
	if(j==	1	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	2	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	3	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	4	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	5	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	6	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	7	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	8	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	9	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	10	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	11	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	12	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	13	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	14	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	15	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	16	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	17	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	18	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	19	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	20	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	21	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	22	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	23	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	24	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	25	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	26	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	27	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	28	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	29	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	30	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	31	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	32	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	33	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	34	){flow=	190	;	intermittency = 	0.03	;}
	if(j==	35	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	36	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	37	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	38	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	39	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	40	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	41	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	42	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	43	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	44	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	45	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	46	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	47	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	48	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	49	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	50	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	51	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	52	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	53	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	54	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	55	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	56	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	57	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	58	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	59	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	60	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	61	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	62	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	63	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	64	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	65	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	66	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	67	){flow=	190	;	intermittency = 	0.03	;}
	if(j==	68	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	69	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	70	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	71	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	72	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	73	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	74	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	75	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	76	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	77	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	78	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	79	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	80	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	81	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	82	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	83	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	84	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	85	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	86	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	87	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	88	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	89	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	90	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	91	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	92	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	93	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	94	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	95	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	96	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	97	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	98	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	99	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	100	){flow=	190	;	intermittency = 	0.03	;}
	if(j==	101	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	102	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	103	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	104	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	105	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	106	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	107	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	108	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	109	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	110	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	111	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	112	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	113	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	114	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	115	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	116	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	117	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	118	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	119	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	120	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	121	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	122	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	123	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	124	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	125	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	126	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	127	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	128	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	129	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	130	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	131	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	132	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	133	){flow=	190	;	intermittency = 	0.03	;}
	if(j==	134	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	135	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	136	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	137	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	138	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	139	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	140	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	141	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	142	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	143	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	144	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	145	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	146	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	147	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	148	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	149	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	150	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	151	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	152	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	153	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	154	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	155	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	156	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	157	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	158	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	159	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	160	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	161	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	162	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	163	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	164	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	165	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	166	){flow=	190	;	intermittency = 	0.03	;}
	if(j==	167	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	168	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	169	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	170	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	171	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	172	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	173	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	174	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	175	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	176	){flow=	130	;	intermittency = 	0.08	;}
	if(j==	177	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	178	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	179	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	180	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	181	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	182	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	183	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	184	){flow=	70	;	intermittency = 	0.13	;}
	if(j==	185	){flow=	70	;	intermittency = 	0.13	;}
	*/
	if(j==1){flow=70;intermittency = 0.1;}
	if(j==2){flow=70;intermittency = 0.1;}
	if(j==3){flow=70;intermittency = 0.1;}
	if(j==4){flow=70;intermittency = 0.1;}
	if(j==5){flow=70;intermittency = 0.1;}
	if(j==6){flow=130;intermittency = 0.02;}
	if(j==7){flow=70;intermittency = 0.1;}
	if(j==8){flow=70;intermittency = 0.1;}
	if(j==9){flow=70;intermittency = 0.1;}
	if(j==10){flow=70;intermittency = 0.1;}
	if(j==11){flow=70;intermittency = 0.1;}
	if(j==12){flow=130;intermittency = 0.02;}
	if(j==13){flow=190;intermittency = 0.008;}
	if(j==14){flow=70;intermittency = 0.1;}
	if(j==15){flow=70;intermittency = 0.1;}
	if(j==16){flow=70;intermittency = 0.1;}
	if(j==17){flow=70;intermittency = 0.1;}
	if(j==18){flow=70;intermittency = 0.1;}
	if(j==19){flow=130;intermittency = 0.02;}
	if(j==20){flow=70;intermittency = 0.1;}
	if(j==21){flow=70;intermittency = 0.1;}
	if(j==22){flow=70;intermittency = 0.1;}
	if(j==23){flow=70;intermittency = 0.1;}
	if(j==24){flow=70;intermittency = 0.1;}
	if(j==25){flow=130;intermittency = 0.02;}
	if(j==26){flow=190;intermittency = 0.008;}
	if(j==27){flow=70;intermittency = 0.1;}
	if(j==28){flow=70;intermittency = 0.1;}
	if(j==29){flow=70;intermittency = 0.1;}
	if(j==30){flow=70;intermittency = 0.1;}
	if(j==31){flow=70;intermittency = 0.1;}
	if(j==32){flow=130;intermittency = 0.02;}
	if(j==33){flow=70;intermittency = 0.1;}
	if(j==34){flow=70;intermittency = 0.1;}
	if(j==35){flow=70;intermittency = 0.1;}
	if(j==36){flow=70;intermittency = 0.1;}
	if(j==37){flow=70;intermittency = 0.1;}
	if(j==38){flow=130;intermittency = 0.02;}
	if(j==39){flow=190;intermittency = 0.008;}
	if(j==40){flow=70;intermittency = 0.1;}
	if(j==41){flow=70;intermittency = 0.1;}
	if(j==42){flow=70;intermittency = 0.1;}
	if(j==43){flow=70;intermittency = 0.1;}
	if(j==44){flow=70;intermittency = 0.1;}
	if(j==45){flow=130;intermittency = 0.02;}
	if(j==46){flow=70;intermittency = 0.1;}
	if(j==47){flow=70;intermittency = 0.1;}
	if(j==48){flow=70;intermittency = 0.1;}
	if(j==49){flow=70;intermittency = 0.1;}
	if(j==50){flow=70;intermittency = 0.1;}
	if(j==51){flow=130;intermittency = 0.02;}
	if(j==52){flow=190;intermittency = 0.008;}
	if(j==53){flow=70;intermittency = 0.1;}
	if(j==54){flow=70;intermittency = 0.1;}
	if(j==55){flow=70;intermittency = 0.1;}
	if(j==56){flow=70;intermittency = 0.1;}
	if(j==57){flow=70;intermittency = 0.1;}
	if(j==58){flow=130;intermittency = 0.02;}
	if(j==59){flow=70;intermittency = 0.1;}
	if(j==60){flow=70;intermittency = 0.1;}
	if(j==61){flow=70;intermittency = 0.1;}
	if(j==62){flow=70;intermittency = 0.1;}
	if(j==63){flow=70;intermittency = 0.1;}
	if(j==64){flow=130;intermittency = 0.02;}
	if(j==65){flow=190;intermittency = 0.008;}
	if(j==66){flow=70;intermittency = 0.1;}
	if(j==67){flow=70;intermittency = 0.1;}
	if(j==68){flow=70;intermittency = 0.1;}
	if(j==69){flow=70;intermittency = 0.1;}
	if(j==70){flow=70;intermittency = 0.1;}
	if(j==71){flow=130;intermittency = 0.02;}
	if(j==72){flow=70;intermittency = 0.1;}
	if(j==73){flow=70;intermittency = 0.1;}
	if(j==74){flow=70;intermittency = 0.1;}
	if(j==75){flow=70;intermittency = 0.1;}
	if(j==76){flow=70;intermittency = 0.1;}
	if(j==77){flow=130;intermittency = 0.02;}
	if(j==78){flow=190;intermittency = 0.008;}
	if(j==79){flow=70;intermittency = 0.1;}
	if(j==80){flow=70;intermittency = 0.1;}
	if(j==81){flow=70;intermittency = 0.1;}
	if(j==82){flow=70;intermittency = 0.1;}
	if(j==83){flow=70;intermittency = 0.1;}
	if(j==84){flow=130;intermittency = 0.02;}
	if(j==85){flow=70;intermittency = 0.1;}
	if(j==86){flow=70;intermittency = 0.1;}
	if(j==87){flow=70;intermittency = 0.1;}
	if(j==88){flow=70;intermittency = 0.1;}
	if(j==89){flow=70;intermittency = 0.1;}
	if(j==90){flow=130;intermittency = 0.02;}
	if(j==91){flow=190;intermittency = 0.008;}
	if(j==92){flow=70;intermittency = 0.1;}
	if(j==93){flow=70;intermittency = 0.1;}
	if(j==94){flow=70;intermittency = 0.1;}
	if(j==95){flow=70;intermittency = 0.1;}
	if(j==96){flow=70;intermittency = 0.1;}
	if(j==97){flow=130;intermittency = 0.02;}
	if(j==98){flow=70;intermittency = 0.1;}
	if(j==99){flow=70;intermittency = 0.1;}
	if(j==100){flow=70;intermittency = 0.1;}
	if(j==101){flow=70;intermittency = 0.1;}
	if(j==102){flow=70;intermittency = 0.1;}
	if(j==103){flow=130;intermittency = 0.02;}
	if(j==104){flow=190;intermittency = 0.008;}
	if(j==105){flow=70;intermittency = 0.1;}
	if(j==106){flow=70;intermittency = 0.1;}
	if(j==107){flow=70;intermittency = 0.1;}
	if(j==108){flow=70;intermittency = 0.1;}
	if(j==109){flow=70;intermittency = 0.1;}
	if(j==110){flow=130;intermittency = 0.02;}
	if(j==111){flow=70;intermittency = 0.1;}
	if(j==112){flow=70;intermittency = 0.1;}
	if(j==113){flow=70;intermittency = 0.1;}
	if(j==114){flow=70;intermittency = 0.1;}
	if(j==115){flow=70;intermittency = 0.1;}
	if(j==116){flow=130;intermittency = 0.02;}
	if(j==117){flow=190;intermittency = 0.008;}
	if(j==118){flow=70;intermittency = 0.1;}
	if(j==119){flow=70;intermittency = 0.1;}
	if(j==120){flow=70;intermittency = 0.1;}
	if(j==121){flow=70;intermittency = 0.1;}
	if(j==122){flow=70;intermittency = 0.1;}
	if(j==123){flow=130;intermittency = 0.02;}
	if(j==124){flow=70;intermittency = 0.1;}
	if(j==125){flow=70;intermittency = 0.1;}
	if(j==126){flow=70;intermittency = 0.1;}
	if(j==127){flow=70;intermittency = 0.1;}
	if(j==128){flow=70;intermittency = 0.1;}
	if(j==129){flow=130;intermittency = 0.02;}
	if(j==130){flow=190;intermittency = 0.008;}
	if(j==131){flow=70;intermittency = 0.1;}
	if(j==132){flow=70;intermittency = 0.1;}
	if(j==133){flow=70;intermittency = 0.1;}
	if(j==134){flow=70;intermittency = 0.1;}
	if(j==135){flow=70;intermittency = 0.1;}
	if(j==136){flow=130;intermittency = 0.02;}
	if(j==137){flow=70;intermittency = 0.1;}
	if(j==138){flow=70;intermittency = 0.1;}
	if(j==139){flow=70;intermittency = 0.1;}
	if(j==140){flow=70;intermittency = 0.1;}
	if(j==141){flow=70;intermittency = 0.1;}
	if(j==142){flow=130;intermittency = 0.02;}
	if(j==143){flow=190;intermittency = 0.008;}
	if(j==144){flow=70;intermittency = 0.1;}
	if(j==145){flow=70;intermittency = 0.1;}
	if(j==146){flow=70;intermittency = 0.1;}
	if(j==147){flow=70;intermittency = 0.1;}
	if(j==148){flow=70;intermittency = 0.1;}
	if(j==149){flow=130;intermittency = 0.02;}
	if(j==150){flow=70;intermittency = 0.1;}
	if(j==151){flow=70;intermittency = 0.1;}
	if(j==152){flow=70;intermittency = 0.1;}
	if(j==153){flow=70;intermittency = 0.1;}
	if(j==154){flow=70;intermittency = 0.1;}
	if(j==155){flow=130;intermittency = 0.02;}
	if(j==156){flow=190;intermittency = 0.008;}
	if(j==157){flow=70;intermittency = 0.1;}
	if(j==158){flow=70;intermittency = 0.1;}
	if(j==159){flow=70;intermittency = 0.1;}
	if(j==160){flow=70;intermittency = 0.1;}
	if(j==161){flow=70;intermittency = 0.1;}
	if(j==162){flow=130;intermittency = 0.02;}
	if(j==163){flow=70;intermittency = 0.1;}
	if(j==164){flow=70;intermittency = 0.1;}
	if(j==165){flow=70;intermittency = 0.1;}
	if(j==166){flow=70;intermittency = 0.1;}
	if(j==167){flow=70;intermittency = 0.1;}
	if(j==168){flow=130;intermittency = 0.02;}
	if(j==169){flow=190;intermittency = 0.008;}
	if(j==170){flow=70;intermittency = 0.1;}
	if(j==171){flow=70;intermittency = 0.1;}
	if(j==172){flow=70;intermittency = 0.1;}
	if(j==173){flow=70;intermittency = 0.1;}
	if(j==174){flow=70;intermittency = 0.1;}
	if(j==175){flow=130;intermittency = 0.02;}
	if(j==176){flow=70;intermittency = 0.1;}
	if(j==177){flow=70;intermittency = 0.1;}
	if(j==178){flow=70;intermittency = 0.1;}
	if(j==179){flow=70;intermittency = 0.1;}
	if(j==180){flow=70;intermittency = 0.1;}
	if(j==181){flow=130;intermittency = 0.02;}
	if(j==182){flow=190;intermittency = 0.008;}
	if(j==183){flow=70;intermittency = 0.1;}
	if(j==184){flow=70;intermittency = 0.1;}
	if(j==185){flow=70;intermittency = 0.1;}

	logmsg("Updated discharge = %lf\n",flow);
}



void RMHydrodynamics_Ikeda1981::OscillatingUSBoundaryCondition(int j,RMHydrodynamicsData* data_in, RMPrepData* prepData)  //DM-FEB2012
{	

	//Manually edit values of amplitude and period of oscillation for dimensionless streamwise velocity perturbation at left bank and dimensionless curvature at the upstream end of the channel
	RMHydrodynamicsData_Ikeda1981* data = (RMHydrodynamicsData_Ikeda1981*)data_in;
	int period_UB_0,period_C_0;
	double amplitude_UB_0,amplitude_C_0;

	//Edit values as needed
	period_UB_0 = 200;			//Oscillation period in number of iterations for dimensionless streamwise velocity perturbation at left bank at upstream end
	amplitude_UB_0 = 1.0;		//Dimensionless streamwise velocity perturbation at left bank at upstream end
	period_C_0 = 200;			//Oscillation period in number of iterations for dimensionless curvature at upstream end
	amplitude_C_0 = 1.0;		//Dimensionless curvature at upstream end

	data->UB_0 = amplitude_UB_0*cos(2.0*M_PI*double((j-1))/double(period_UB_0));
	data->C_0 = amplitude_C_0*cos(2.0*M_PI*double((j-1))/double(period_C_0));
	prepData->C_0 = data->C_0;
	
	logmsg("Updated upstream boundary condition for dimensionless streamwise velocity at left bank = %lf\n",data->UB_0);
	logmsg("Updated upstream boundary condition for dimensionless curvature = %lf\n",data->C_0);

}