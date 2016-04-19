#include <math.h>
#include "rmfilter_savitzkygolay.h"

void RMFilter_SavitzkyGolay::SetWindowSize(int leftWindowSize, int rightWindowSize)
{
	m_leftWindowSize = leftWindowSize;
	m_rightWindowSize = rightWindowSize;
}

void RMFilter_SavitzkyGolay::GetWindowSize(int& left, int& right)
{
	left = m_leftWindowSize;
	right = m_rightWindowSize;
}

void RMFilter_SavitzkyGolay::SetCoefficients(const RMSeries& coeffs)
{
	m_coeffs = coeffs;
}

RMSeries RMFilter_SavitzkyGolay::GetCoefficients()
{
	return m_coeffs;
}

bool RMFilter_SavitzkyGolay::ApplyFilter(RMSeries& data, int polynomial_order, int window_nodes)    //CORRECTION FEB2010
{
	if(polynomial_order == 4)								// Polynomial order = 4
		{
			if (window_nodes == 7)
				{
					SavitzkyGolay_p4m7(data);
				}
			else if (window_nodes == 9)
				{
					SavitzkyGolay_p4m9(data);
				}
			else if (window_nodes == 11)
				{
					SavitzkyGolay_p4m11(data);
				}
			else if (window_nodes == 13)
				{
					SavitzkyGolay_p4m13(data);
				}
			else if (window_nodes == 15)
				{
					SavitzkyGolay_p4m15(data);
				}
		}
	else if (polynomial_order == 2)							// Polynomial order = 4
		{
			if (window_nodes == 5)
				{
					SavitzkyGolay_p2m5(data);
				}
			else if (window_nodes == 7)
				{
					SavitzkyGolay_p2m7(data);
				}
			else if (window_nodes == 9)
				{
					SavitzkyGolay_p2m9(data);
				}
			else if (window_nodes == 11)
				{
					SavitzkyGolay_p2m11(data);
				}
			else if (window_nodes == 13)
				{
					SavitzkyGolay_p2m13(data);
				}
			else if (window_nodes == 15)
				{
					SavitzkyGolay_p2m15(data);
				}
		}

	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p4m7(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = 0.0216;			//-3
	m_coeffs(1) = -0.1299;			//-2
	m_coeffs(2) = 0.3247;			//-1
	m_coeffs(3) = 0.5672;			//0
	m_coeffs(4) = 0.3247;			//+1
	m_coeffs(5) = -0.1299;			//+2
	m_coeffs(6) = 0.0216;			//+3

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.0000*dataCopy(0,0)+1.0000*dataCopy(1,0)+0.0000*dataCopy(2,0)+0.0000*dataCopy(3,0)+0.0000*dataCopy(4,0);		//DM-FEB2011
			data(1,1) = 0.0000*dataCopy(0,1)+1.0000*dataCopy(1,1)+0.0000*dataCopy(2,1)+0.0000*dataCopy(3,1)+0.0000*dataCopy(4,1);		//DM-FEB2011
				
			data(2,0) = -0.0397*dataCopy(0,0)+0.1984*dataCopy(1,0)+0.6032*dataCopy(2,0)+0.3968*dataCopy(3,0)-0.1984*dataCopy(4,0)+0.0397*dataCopy(5,0);//DM-FEB2011
			data(2,1) = -0.0397*dataCopy(0,1)+0.1984*dataCopy(1,1)+0.6032*dataCopy(2,1)+0.3968*dataCopy(3,1)-0.1984*dataCopy(4,1)+0.0397*dataCopy(5,1);//DM-FEB2011

			data(len-3,0) = 0.0397*dataCopy(len-6,0)-0.1984*dataCopy(len-5,0)+0.3968*dataCopy(len-4,0)+0.6032*dataCopy(len-3,0)+0.1984*dataCopy(len-2,0)-0.0397*dataCopy(len-1,0);//DM-FEB2011
			data(len-3,1) = 0.0397*dataCopy(len-6,1)-0.1984*dataCopy(len-5,1)+0.3968*dataCopy(len-4,1)+0.6032*dataCopy(len-3,1)+0.1984*dataCopy(len-2,1)-0.0397*dataCopy(len-1,1);//DM-FEB2011
																																																																																																											
			data(len-2,0) = +0.0000*dataCopy(len-5,0)+0.0000*dataCopy(len-4,0)+0.0000*dataCopy(len-3,0)+1.0000*dataCopy(len-2,0)+0.0000*dataCopy(len-1,0);//DM-FEB2011
			data(len-2,1) = +0.0000*dataCopy(len-5,1)+0.0000*dataCopy(len-4,1)+0.0000*dataCopy(len-3,1)+1.0000*dataCopy(len-2,1)+0.0000*dataCopy(len-1,1);//DM-FEB2011

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p4m9(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = 0.035;			//-4
	m_coeffs(1) = -0.1282;			//-3
	m_coeffs(2) = 0.0699;			//-2
	m_coeffs(3) = 0.3147;			//-1
	m_coeffs(4) = 0.4172;			//0
	m_coeffs(5) = 0.3147;			//+1
	m_coeffs(6) = 0.0699;			//+2
	m_coeffs(7) = -0.1282;			//+3
	m_coeffs(8) = 0.035;			//+4

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.0198*dataCopy(0,0)+0.9008*dataCopy(1,0)+0.1984*dataCopy(2,0)-0.1984*dataCopy(3,0)+0.0992*dataCopy(4,0)-0.0198*dataCopy(5,0);
			data(1,1) = 0.0198*dataCopy(0,1)+0.9008*dataCopy(1,1)+0.1984*dataCopy(2,1)-0.1984*dataCopy(3,1)+0.0992*dataCopy(4,1)-0.0198*dataCopy(5,1);

			data(2,0) = -0.0758*dataCopy(0,0)+0.3355*dataCopy(1,0)+0.4589*dataCopy(2,0)+0.3247*dataCopy(3,0)+0.0541*dataCopy(4,0)-0.1407*dataCopy(5,0)+0.0433*dataCopy(6,0);
			data(2,1) = -0.0758*dataCopy(0,1)+0.3355*dataCopy(1,1)+0.4589*dataCopy(2,1)+0.3247*dataCopy(3,1)+0.0541*dataCopy(4,1)-0.1407*dataCopy(5,1)+0.0433*dataCopy(6,1); 
								
			data(3,0) = -0.0189*dataCopy(0,0)-0.0081*dataCopy(1,0)+0.2679*dataCopy(2,0)+0.4452*dataCopy(3,0)+0.3653*dataCopy(4,0)+0.0731*dataCopy(5,0)-0.1813*dataCopy(6,0)+0.0568*dataCopy(7,0);
			data(3,1) = -0.0189*dataCopy(0,1)-0.0081*dataCopy(1,1)+0.2679*dataCopy(2,1)+0.4452*dataCopy(3,1)+0.3653*dataCopy(4,1)+0.0731*dataCopy(5,1)-0.1813*dataCopy(6,1)+0.0568*dataCopy(7,1);
									
			data(len-4,0) = 0.0568*dataCopy(len-8,0)-0.1813*dataCopy(len-7,0)+0.0731*dataCopy(len-6,0)+0.3653*dataCopy(len-5,0)+0.4452*dataCopy(len-4,0)+0.2679*dataCopy(len-3,0)-0.0081*dataCopy(len-2,0)-0.0189*dataCopy(len-1,0);
			data(len-4,1) = 0.0568*dataCopy(len-8,1)-0.1813*dataCopy(len-7,1)+0.0731*dataCopy(len-6,1)+0.3653*dataCopy(len-5,1)+0.4452*dataCopy(len-4,1)+0.2679*dataCopy(len-3,1)-0.0081*dataCopy(len-2,1)-0.0189*dataCopy(len-1,1);
									
			data(len-3,0) = 0.0433*dataCopy(len-7,0)-0.1407*dataCopy(len-6,0)+0.0541*dataCopy(len-5,0)+0.3247*dataCopy(len-4,0)+0.4589*dataCopy(len-3,0)+0.3355*dataCopy(len-2,0)-0.0758*dataCopy(len-1,0);
			data(len-3,1) = 0.0433*dataCopy(len-7,1)-0.1407*dataCopy(len-6,1)+0.0541*dataCopy(len-5,1)+0.3247*dataCopy(len-4,1)+0.4589*dataCopy(len-3,1)+0.3355*dataCopy(len-2,1)-0.0758*dataCopy(len-1,1); 
			 																				                                    																																																																																																								
			data(len-2,0) = -0.0198*dataCopy(len-6,0)+0.0992*dataCopy(len-5,0)-0.1984*dataCopy(len-4,0)+0.1984*dataCopy(len-3,0)+0.9008*dataCopy(len-2,0)+0.0198*dataCopy(len-1,0);
			data(len-2,1) = -0.0198*dataCopy(len-6,1)+0.0992*dataCopy(len-5,1)-0.1984*dataCopy(len-4,1)+0.1984*dataCopy(len-3,1)+0.9008*dataCopy(len-2,1)+0.0198*dataCopy(len-1,1);
							
			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p4m11(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = 0.042;			//-5		//0.041950;		//DM-FEB2011	
	m_coeffs(1) = -0.104900;		//-4
	m_coeffs(2) = -0.023300;		//-3
	m_coeffs(3) = 0.139900;			//-2
	m_coeffs(4) = 0.279700;			//-1
	m_coeffs(5) = 0.333200;			//0
	m_coeffs(6) = 0.279700;			//+1
	m_coeffs(7) = 0.139900;			//+2
	m_coeffs(8) = -0.023300;		//+3
	m_coeffs(9) = -0.104900;		//+4
	m_coeffs(10) = 0.042;			//+5		//0.041950;		//DM-FEB2011

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.0541*dataCopy(0,0)+0.7706*dataCopy(1,0)+0.3355*dataCopy(2,0)-0.1299*dataCopy(3,0)-0.1407*dataCopy(4,0)+0.1515*dataCopy(5,0)-0.0411*dataCopy(6,0);//DM-FEB2011
			data(1,1) = 0.0541*dataCopy(0,1)+0.7706*dataCopy(1,1)+0.3355*dataCopy(2,1)-0.1299*dataCopy(3,1)-0.1407*dataCopy(4,1)+0.1515*dataCopy(5,1)-0.0411*dataCopy(6,1);//DM-FEB2011
			
			data(2,0) = -0.0947*dataCopy(0,0)+0.3923*dataCopy(1,0)+0.4323*dataCopy(2,0)+0.2679*dataCopy(3,0)+0.0731*dataCopy(4,0)-0.046*dataCopy(5,0)-0.0514*dataCopy(6,0)+0.0265*dataCopy(7,0);//DM-FEB2011
			data(2,1) = -0.0947*dataCopy(0,1)+0.3923*dataCopy(1,1)+0.4323*dataCopy(2,1)+0.2679*dataCopy(3,1)+0.0731*dataCopy(4,1)-0.046*dataCopy(5,1)-0.0514*dataCopy(6,1)+0.0265*dataCopy(7,1);//DM-FEB2011
				
			data(3,0) = -0.0583*dataCopy(0,0)+0.0874*dataCopy(1,0)+0.2622*dataCopy(2,0)+0.3545*dataCopy(3,0)+0.3147*dataCopy(4,0)+0.1573*dataCopy(5,0)-0.0408*dataCopy(6,0)-0.1399*dataCopy(7,0)+0.0629*dataCopy(8,0);//DM-FEB2011
			data(3,1) = -0.0583*dataCopy(0,1)+0.0874*dataCopy(1,1)+0.2622*dataCopy(2,1)+0.3545*dataCopy(3,1)+0.3147*dataCopy(4,1)+0.1573*dataCopy(5,1)-0.0408*dataCopy(6,1)-0.1399*dataCopy(7,1)+0.0629*dataCopy(8,1);//DM-FEB2011
			
			data(4,0) = 0.0000*dataCopy(0,0)-0.0583*dataCopy(1,0)+0.0874*dataCopy(2,0)+0.2622*dataCopy(3,0)+0.3545*dataCopy(4,0)+0.3147*dataCopy(5,0)+0.1573*dataCopy(6,0)-0.0408*dataCopy(7,0)-0.1399*dataCopy(8,0)+0.0629*dataCopy(9,0);//DM-FEB2011
			data(4,1) = 0.0000*dataCopy(0,1)-0.0583*dataCopy(1,1)+0.0874*dataCopy(2,1)+0.2622*dataCopy(3,1)+0.3545*dataCopy(4,1)+0.3147*dataCopy(5,1)+0.1573*dataCopy(6,1)-0.0408*dataCopy(7,1)-0.1399*dataCopy(8,1)+0.0629*dataCopy(9,1);//DM-FEB2011

			data(len-5,0) = 0.0629*dataCopy(len-10,0)-0.1399*dataCopy(len-9,0)-0.0408*dataCopy(len-8,0)+0.1573*dataCopy(len-7,0)+0.3147*dataCopy(len-6,0)+0.3545*dataCopy(len-5,0)+0.2622*dataCopy(len-4,0)+0.0874*dataCopy(len-3,0)-0.0583*dataCopy(len-2,0)+0.0000*dataCopy(len-1,0);//DM-FEB2011
			data(len-5,1) = 0.0629*dataCopy(len-10,1)-0.1399*dataCopy(len-9,1)-0.0408*dataCopy(len-8,1)+0.1573*dataCopy(len-7,1)+0.3147*dataCopy(len-6,1)+0.3545*dataCopy(len-5,1)+0.2622*dataCopy(len-4,1)+0.0874*dataCopy(len-3,1)-0.0583*dataCopy(len-2,1)+0.0000*dataCopy(len-1,1);//DM-FEB2011

			data(len-4,0) = 0.0629*dataCopy(len-9,0)-0.1399*dataCopy(len-8,0)-0.0408*dataCopy(len-7,0)+0.1573*dataCopy(len-6,0)+0.3147*dataCopy(len-5,0)+0.3545*dataCopy(len-4,0)+0.2622*dataCopy(len-3,0)+0.0874*dataCopy(len-2,0)-0.0583*dataCopy(len-1,0);//DM-FEB2011
			data(len-4,1) = 0.0629*dataCopy(len-9,1)-0.1399*dataCopy(len-8,1)-0.0408*dataCopy(len-7,1)+0.1573*dataCopy(len-6,1)+0.3147*dataCopy(len-5,1)+0.3545*dataCopy(len-4,1)+0.2622*dataCopy(len-3,1)+0.0874*dataCopy(len-2,1)-0.0583*dataCopy(len-1,1);//DM-FEB2011

			data(len-3,0) = 0.0265*dataCopy(len-8,0)-0.0514*dataCopy(len-7,0)-0.046*dataCopy(len-6,0)+0.0731*dataCopy(len-5,0)+0.2679*dataCopy(len-4,0)+0.4323*dataCopy(len-3,0)+0.3923*dataCopy(len-2,0)-0.0947*dataCopy(len-1,0);//DM-FEB2011
			data(len-3,1) = 0.0265*dataCopy(len-8,1)-0.0514*dataCopy(len-7,1)-0.046*dataCopy(len-6,1)+0.0731*dataCopy(len-5,1)+0.2679*dataCopy(len-4,1)+0.4323*dataCopy(len-3,1)+0.3923*dataCopy(len-2,1)-0.0947*dataCopy(len-1,1);//DM-FEB2011

			data(len-2,0) = -0.0411*dataCopy(len-7,0)+0.1515*dataCopy(len-6,0)-0.1407*dataCopy(len-5,0)-0.1299*dataCopy(len-4,0)+0.3355*dataCopy(len-3,0)+0.7706*dataCopy(len-2,0)+0.0541*dataCopy(len-1,0);//DM-FEB2011
			data(len-2,1) = -0.0411*dataCopy(len-7,1)+0.1515*dataCopy(len-6,1)-0.1407*dataCopy(len-5,1)-0.1299*dataCopy(len-4,1)+0.3355*dataCopy(len-3,1)+0.7706*dataCopy(len-2,1)+0.0541*dataCopy(len-1,1);//DM-FEB2011

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p4m13(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = 0.0452;			//-6
	m_coeffs(1) = -0.0814;			//-5
	m_coeffs(2) = -0.0555;			//-4
	m_coeffs(3) = 0.0452;			//-3
	m_coeffs(4) = 0.1604;			//-2
	m_coeffs(5) = 0.2468;			//-1
	m_coeffs(6) = 0.2786;			//0
	m_coeffs(7) = 0.2468;			//+1
	m_coeffs(8) = 0.1604;			//+2
	m_coeffs(9) = 0.0452;			//+3
	m_coeffs(10) = -0.0555;			//+4
	m_coeffs(11) = -0.0814;			//+5
	m_coeffs(12) = 0.0452;			//+6
											
	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.0947*dataCopy(0,0)+0.6488*dataCopy(1,0)+0.3923*dataCopy(2,0)-0.0081*dataCopy(3,0)-0.1813*dataCopy(4,0)-0.0514*dataCopy(5,0)+0.1618*dataCopy(6,0)-0.0568*dataCopy(7,0);
			data(1,1) = 0.0947*dataCopy(0,1)+0.6488*dataCopy(1,1)+0.3923*dataCopy(2,1)-0.0081*dataCopy(3,1)-0.1813*dataCopy(4,1)-0.0514*dataCopy(5,1)+0.1618*dataCopy(6,1)-0.0568*dataCopy(7,1); 

			data(2,0) = -0.0971*dataCopy(0,0)+0.3982*dataCopy(1,0)+0.432*dataCopy(2,0)+0.2622*dataCopy(3,0)+0.0699*dataCopy(4,0)-0.0408*dataCopy(5,0)-0.0427*dataCopy(6,0)+0.0144*dataCopy(7,0)+0.0039*dataCopy(8,0);
			data(2,1) = -0.0971*dataCopy(0,1)+0.3982*dataCopy(1,1)+0.432*dataCopy(2,1)+0.2622*dataCopy(3,1)+0.0699*dataCopy(4,1)-0.0408*dataCopy(5,1)-0.0427*dataCopy(6,1)+0.0144*dataCopy(7,1)+0.0039*dataCopy(8,1); 

			data(3,0) = -0.0874*dataCopy(0,0)+0.1457*dataCopy(1,0)+0.2768*dataCopy(2,0)+0.3107*dataCopy(3,0)+0.2622*dataCopy(4,0)+0.1573*dataCopy(5,0)+0.0321*dataCopy(6,0)-0.067*dataCopy(7,0)-0.0828*dataCopy(8,0)+0.0524*dataCopy(9,0);
			data(3,1) = -0.0874*dataCopy(0,1)+0.1457*dataCopy(1,1)+0.2768*dataCopy(2,1)+0.3107*dataCopy(3,1)+0.2622*dataCopy(4,1)+0.1573*dataCopy(5,1)+0.0321*dataCopy(6,1)-0.067*dataCopy(7,1)-0.0828*dataCopy(8,1)+0.0524*dataCopy(9,1); 

			data(4,0) = -0.035*dataCopy(0,0)+0.0000*dataCopy(1,0)+0.1166*dataCopy(2,0)+0.2331*dataCopy(3,0)+0.2961*dataCopy(4,0)+0.2797*dataCopy(5,0)+0.1865*dataCopy(6,0)+0.0466*dataCopy(7,0)-0.0816*dataCopy(8,0)-0.1119*dataCopy(9,0)+0.0699*dataCopy(10,0);
			data(4,1) = -0.035*dataCopy(0,1)+0.0000*dataCopy(1,1)+0.1166*dataCopy(2,1)+0.2331*dataCopy(3,1)+0.2961*dataCopy(4,1)+0.2797*dataCopy(5,1)+0.1865*dataCopy(6,1)+0.0466*dataCopy(7,1)-0.0816*dataCopy(8,1)-0.1119*dataCopy(9,1)+0.0699*dataCopy(10,1); 

			data(5,0) = 0.0128*dataCopy(0,0)-0.0641*dataCopy(1,0)+0.0058*dataCopy(2,0)+0.1301*dataCopy(3,0)+0.2409*dataCopy(4,0)+0.2945*dataCopy(5,0)+0.272*dataCopy(6,0)+0.1787*dataCopy(7,0)+0.0447*dataCopy(8,0)-0.0758*dataCopy(9,0)-0.1037*dataCopy(10,0)+0.0641*dataCopy(11,0);
			data(5,1) = 0.0128*dataCopy(0,1)-0.0641*dataCopy(1,1)+0.0058*dataCopy(2,1)+0.1301*dataCopy(3,1)+0.2409*dataCopy(4,1)+0.2945*dataCopy(5,1)+0.272*dataCopy(6,1)+0.1787*dataCopy(7,1)+0.0447*dataCopy(8,1)-0.0758*dataCopy(9,1)-0.1037*dataCopy(10,1)+0.0641*dataCopy(11,1);

			data(len-6,0) = 0.0641*dataCopy(len-12,0)-0.1037*dataCopy(len-11,0)-0.0758*dataCopy(len-10,0)+0.0447*dataCopy(len-9,0)+0.1787*dataCopy(len-8,0)+0.272*dataCopy(len-7,0)+0.2945*dataCopy(len-6,0)+0.2409*dataCopy(len-5,0)+0.1301*dataCopy(len-4,0)+0.0058*dataCopy(len-3,0)-0.0641*dataCopy(len-2,0)+0.0128*dataCopy(len-1,0);
			data(len-6,1) = 0.0641*dataCopy(len-12,1)-0.1037*dataCopy(len-11,1)-0.0758*dataCopy(len-10,1)+0.0447*dataCopy(len-9,1)+0.1787*dataCopy(len-8,1)+0.272*dataCopy(len-7,1)+0.2945*dataCopy(len-6,1)+0.2409*dataCopy(len-5,1)+0.1301*dataCopy(len-4,1)+0.0058*dataCopy(len-3,1)-0.0641*dataCopy(len-2,1)+0.0128*dataCopy(len-1,1); 

			data(len-5,0) = 0.0699*dataCopy(len-11,0)-0.1119*dataCopy(len-10,0)-0.0816*dataCopy(len-9,0)+0.0466*dataCopy(len-8,0)+0.1865*dataCopy(len-7,0)+0.2797*dataCopy(len-6,0)+0.2961*dataCopy(len-5,0)+0.2331*dataCopy(len-4,0)+0.1166*dataCopy(len-3,0)+0.0000*dataCopy(len-2,0)-0.035*dataCopy(len-1,0);
			data(len-5,1) = 0.0699*dataCopy(len-11,1)-0.1119*dataCopy(len-10,1)-0.0816*dataCopy(len-9,1)+0.0466*dataCopy(len-8,1)+0.1865*dataCopy(len-7,1)+0.2797*dataCopy(len-6,1)+0.2961*dataCopy(len-5,1)+0.2331*dataCopy(len-4,1)+0.1166*dataCopy(len-3,1)+0.0000*dataCopy(len-2,1)-0.035*dataCopy(len-1,1);

			data(len-4,0) = 0.0524*dataCopy(len-10,0)-0.0828*dataCopy(len-9,0)-0.067*dataCopy(len-8,0)+0.0321*dataCopy(len-7,0)+0.1573*dataCopy(len-6,0)+0.2622*dataCopy(len-5,0)+0.3107*dataCopy(len-4,0)+0.2768*dataCopy(len-3,0)+0.1457*dataCopy(len-2,0)-0.0874*dataCopy(len-1,0);
			data(len-4,1) = 0.0524*dataCopy(len-10,1)-0.0828*dataCopy(len-9,1)-0.067*dataCopy(len-8,1)+0.0321*dataCopy(len-7,1)+0.1573*dataCopy(len-6,1)+0.2622*dataCopy(len-5,1)+0.3107*dataCopy(len-4,1)+0.2768*dataCopy(len-3,1)+0.1457*dataCopy(len-2,1)-0.0874*dataCopy(len-1,1);

			data(len-3,0) = 0.0039*dataCopy(len-9,0)+0.0144*dataCopy(len-8,0)-0.0427*dataCopy(len-7,0)-0.0408*dataCopy(len-6,0)+0.0699*dataCopy(len-5,0)+0.2622*dataCopy(len-4,0)+0.432*dataCopy(len-3,0)+0.3982*dataCopy(len-2,0)-0.0971*dataCopy(len-1,0);
			data(len-3,1) = 0.0039*dataCopy(len-9,1)+0.0144*dataCopy(len-8,1)-0.0427*dataCopy(len-7,1)-0.0408*dataCopy(len-6,1)+0.0699*dataCopy(len-5,1)+0.2622*dataCopy(len-4,1)+0.432*dataCopy(len-3,1)+0.3982*dataCopy(len-2,1)-0.0971*dataCopy(len-1,1); 

			data(len-2,0) = -0.0568*dataCopy(len-8,0)+0.1618*dataCopy(len-7,0)-0.0514*dataCopy(len-6,0)-0.1813*dataCopy(len-5,0)-0.0081*dataCopy(len-4,0)+0.3923*dataCopy(len-3,0)+0.6488*dataCopy(len-2,0)+0.0947*dataCopy(len-1,0);
			data(len-2,1) = -0.0568*dataCopy(len-8,1)+0.1618*dataCopy(len-7,1)-0.0514*dataCopy(len-6,1)-0.1813*dataCopy(len-5,1)-0.0081*dataCopy(len-4,1)+0.3923*dataCopy(len-3,1)+0.6488*dataCopy(len-2,1)+0.0947*dataCopy(len-1,1);

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011

	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p4m15(RMSeries& data)  //ADDITION FEB2010
{
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m5(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients

/*
    m_coeffs(0) = -0.0857;			//-2
	m_coeffs(1) = 0.3429;			//-1
	m_coeffs(2) = 0.4857;			//0
	m_coeffs(3) = 0.3429;			//+1
	m_coeffs(4) = -0.0857;			//+2
*/

	m_coeffs(0) = -0.086;			//-2		//DM-FEB2011
	m_coeffs(1) = 0.343;			//-1		//DM-FEB2011
	m_coeffs(2) = 0.486;			//0			//DM-FEB2011
	m_coeffs(3) = 0.343;			//+1		//DM-FEB2011
	m_coeffs(4) = -0.086;			//+2		//DM-FEB2011


	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.15*dataCopy(0,0)+0.55*dataCopy(1,0)+0.45*dataCopy(2,0)-0.15*dataCopy(3,0);
			data(1,1) = 0.15*dataCopy(0,1)+0.55*dataCopy(1,1)+0.45*dataCopy(2,1)-0.15*dataCopy(3,1);

			data(len-2,0) = -0.15*dataCopy(len-4,0)+0.45*dataCopy(len-3,0)+0.55*dataCopy(len-2,0)+0.15*dataCopy(len-1,0);
			data(len-2,1) = -0.15*dataCopy(len-4,1)+0.45*dataCopy(len-3,1)+0.55*dataCopy(len-2,1)+0.15*dataCopy(len-1,1);

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);  
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);


			
		}//DM-FEB2011
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m7(RMSeries& data)  //ADDITION FEB2010
{
    int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = -0.0952;			//-3
	m_coeffs(1) = 0.1429;			//-2
	m_coeffs(2) = 0.2857;			//-1
	m_coeffs(3) = 0.3332;			//0 //DM-FEB2011
	m_coeffs(4) = 0.2857;			//+1
	m_coeffs(5) = 0.1429;			//+2
	m_coeffs(6) = -0.0952;			//+3

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.2571*dataCopy(0,0)+0.3715*dataCopy(1,0)+0.3429*dataCopy(2,0)+0.1714*dataCopy(3,0)-0.1429*dataCopy(4,0);//DM-FEB2011
			data(1,1) = 0.2571*dataCopy(0,1)+0.3715*dataCopy(1,1)+0.3429*dataCopy(2,1)+0.1714*dataCopy(3,1)-0.1429*dataCopy(4,1);//DM-FEB2011
							
			data(2,0) = +0.0000*dataCopy(0,0)+0.2571*dataCopy(1,0)+0.3715*dataCopy(2,0)+0.3429*dataCopy(3,0)+0.1714*dataCopy(4,0)-0.1429*dataCopy(5,0);//DM-FEB2011
			data(2,1) = +0.0000*dataCopy(0,1)+0.2571*dataCopy(1,1)+0.3715*dataCopy(2,1)+0.3429*dataCopy(3,1)+0.1714*dataCopy(4,1)-0.1429*dataCopy(5,1);//DM-FEB2011

			data(len-3,0) = -0.1429*dataCopy(len-6,0)+0.1714*dataCopy(len-5,0)+0.3429*dataCopy(len-4,0)+0.3715*dataCopy(len-3,0)+0.2571*dataCopy(len-2,0)+0.0000*dataCopy(len-1,0);//DM-FEB2011
			data(len-3,1) = -0.1429*dataCopy(len-6,1)+0.1714*dataCopy(len-5,1)+0.3429*dataCopy(len-4,1)+0.3715*dataCopy(len-3,1)+0.2571*dataCopy(len-2,1)+0.0000*dataCopy(len-1,1);//DM-FEB2011
																																																																																																											
			data(len-2,0) = -0.1429*dataCopy(len-5,0)+0.1714*dataCopy(len-4,0)+0.3429*dataCopy(len-3,0)+0.3715*dataCopy(len-2,0)+0.2571*dataCopy(len-1,0);//DM-FEB2011
			data(len-2,1) = -0.1429*dataCopy(len-5,1)+0.1714*dataCopy(len-4,1)+0.3429*dataCopy(len-3,1)+0.3715*dataCopy(len-2,1)+0.2571*dataCopy(len-1,1);//DM-FEB2011

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 

			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011
	return true;

}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m9(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = -0.0909;			//-4
	m_coeffs(1) = 0.0606;			//-3
	m_coeffs(2) = 0.1688;			//-2
	m_coeffs(3) = 0.2338;			//-1
	m_coeffs(4) = 0.2554;			//0
	m_coeffs(5) = 0.2338;			//+1
	m_coeffs(6) = 0.1688;			//+2
	m_coeffs(7) = 0.0606;			//+3
	m_coeffs(8) = -0.0909;			//+4

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.3214*dataCopy(0,0)+0.3072*dataCopy(1,0)+0.2571*dataCopy(2,0)+0.1714*dataCopy(3,0)+0.05*dataCopy(4,0)-0.1071*dataCopy(5,0);
			data(1,1) = 0.3214*dataCopy(0,1)+0.3072*dataCopy(1,1)+0.2571*dataCopy(2,1)+0.1714*dataCopy(3,1)+0.05*dataCopy(4,1)-0.1071*dataCopy(5,1); 

			data(2,0) = 0.0714*dataCopy(0,0)+0.2143*dataCopy(1,0)+0.2858*dataCopy(2,0)+0.2857*dataCopy(3,0)+0.2143*dataCopy(4,0)+0.0714*dataCopy(5,0)-0.1429*dataCopy(6,0);
			data(2,1) = 0.0714*dataCopy(0,1)+0.2143*dataCopy(1,1)+0.2858*dataCopy(2,1)+0.2857*dataCopy(3,1)+0.2143*dataCopy(4,1)+0.0714*dataCopy(5,1)-0.1429*dataCopy(6,1);

			data(3,0) = -0.0417*dataCopy(0,0)+0.125*dataCopy(1,0)+0.2321*dataCopy(2,0)+0.2798*dataCopy(3,0)+0.2679*dataCopy(4,0)+0.1964*dataCopy(5,0)+0.0655*dataCopy(6,0)-0.125*dataCopy(7,0);
			data(3,1) = -0.0417*dataCopy(0,1)+0.125*dataCopy(1,1)+0.2321*dataCopy(2,1)+0.2798*dataCopy(3,1)+0.2679*dataCopy(4,1)+0.1964*dataCopy(5,1)+0.0655*dataCopy(6,1)-0.125*dataCopy(7,1);

			data(len-4,0) = -0.125*dataCopy(len-8,0)+0.0655*dataCopy(len-7,0)+0.1964*dataCopy(len-6,0)+0.2679*dataCopy(len-5,0)+0.2798*dataCopy(len-4,0)+0.2321*dataCopy(len-3,0)+0.125*dataCopy(len-2,0)-0.0417*dataCopy(len-1,0);
			data(len-4,1) = -0.125*dataCopy(len-8,1)+0.0655*dataCopy(len-7,1)+0.1964*dataCopy(len-6,1)+0.2679*dataCopy(len-5,1)+0.2798*dataCopy(len-4,1)+0.2321*dataCopy(len-3,1)+0.125*dataCopy(len-2,1)-0.0417*dataCopy(len-1,1);

			data(len-3,0) = -0.1429*dataCopy(len-7,0)+0.0714*dataCopy(len-6,0)+0.2143*dataCopy(len-5,0)+0.2857*dataCopy(len-4,0)+0.2858*dataCopy(len-3,0)+0.2143*dataCopy(len-2,0)+0.0714*dataCopy(len-1,0);
			data(len-3,1) = -0.1429*dataCopy(len-7,1)+0.0714*dataCopy(len-6,1)+0.2143*dataCopy(len-5,1)+0.2857*dataCopy(len-4,1)+0.2858*dataCopy(len-3,1)+0.2143*dataCopy(len-2,1)+0.0714*dataCopy(len-1,1);
			                                               																																																																																																								
			data(len-2,0) = -0.1071*dataCopy(len-6,0)+0.05*dataCopy(len-5,0)+0.1714*dataCopy(len-4,0)+0.2571*dataCopy(len-3,0)+0.3072*dataCopy(len-2,0)+0.3214*dataCopy(len-1,0);
			data(len-2,1) = -0.1071*dataCopy(len-6,1)+0.05*dataCopy(len-5,1)+0.1714*dataCopy(len-4,1)+0.2571*dataCopy(len-3,1)+0.3072*dataCopy(len-2,1)+0.3214*dataCopy(len-1,1);

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011

	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m11(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = -0.0839;			//-5
	m_coeffs(1) = 0.021;			//-4
	m_coeffs(2) = 0.1026;			//-3
	m_coeffs(3) = 0.1608;			//-2
	m_coeffs(4) = 0.1958;			//-1
	m_coeffs(5) = 0.2074;			//0
	m_coeffs(6) = 0.1958;			//+1
	m_coeffs(7) = 0.1608;			//+2
	m_coeffs(8) = 0.1026;			//+3
	m_coeffs(9) = 0.021;			//+4
	m_coeffs(10) = -0.0839;			//+5

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.3571*dataCopy(0,0)+0.2857*dataCopy(1,0)+0.2143*dataCopy(2,0)+0.1429*dataCopy(3,0)+0.0714*dataCopy(4,0)+0.000*dataCopy(5,0)-0.0714*dataCopy(6,0);
			data(1,1) = 0.3571*dataCopy(0,1)+0.2857*dataCopy(1,1)+0.2143*dataCopy(2,1)+0.1429*dataCopy(3,1)+0.0714*dataCopy(4,1)+0.000*dataCopy(5,1)-0.0714*dataCopy(6,1);

			data(2,0) = 0.125*dataCopy(0,0)+0.1964*dataCopy(1,0)+0.2322*dataCopy(2,0)+0.2321*dataCopy(3,0)+0.1964*dataCopy(4,0)+0.125*dataCopy(5,0)+0.0179*dataCopy(6,0)-0.125*dataCopy(7,0);
			data(2,1) = 0.125*dataCopy(0,1)+0.1964*dataCopy(1,1)+0.2322*dataCopy(2,1)+0.2321*dataCopy(3,1)+0.1964*dataCopy(4,1)+0.125*dataCopy(5,1)+0.0179*dataCopy(6,1)-0.125*dataCopy(7,1);
					
			data(3,0) = 0.0061*dataCopy(0,0)+0.1182*dataCopy(1,0)+0.1935*dataCopy(2,0)+0.232*dataCopy(3,0)+0.2338*dataCopy(4,0)+0.1987*dataCopy(5,0)+0.1268*dataCopy(6,0)+0.0182*dataCopy(7,0)-0.1273*dataCopy(8,0);
			data(3,1) = 0.0061*dataCopy(0,1)+0.1182*dataCopy(1,1)+0.1935*dataCopy(2,1)+0.232*dataCopy(3,1)+0.2338*dataCopy(4,1)+0.1987*dataCopy(5,1)+0.1268*dataCopy(6,1)+0.0182*dataCopy(7,1)-0.1273*dataCopy(8,1);
				
			data(4,0) = -0.0545*dataCopy(0,0)+0.0606*dataCopy(1,0)+0.1455*dataCopy(2,0)+0.2*dataCopy(3,0)+0.2241*dataCopy(4,0)+0.2182*dataCopy(5,0)+0.1818*dataCopy(6,0)+0.1152*dataCopy(7,0)+0.0182*dataCopy(8,0)-0.1091*dataCopy(9,0);
			data(4,1) = -0.0545*dataCopy(0,1)+0.0606*dataCopy(1,1)+0.1455*dataCopy(2,1)+0.2*dataCopy(3,1)+0.2241*dataCopy(4,1)+0.2182*dataCopy(5,1)+0.1818*dataCopy(6,1)+0.1152*dataCopy(7,1)+0.0182*dataCopy(8,1)-0.1091*dataCopy(9,1);

			data(len-5,0) = -0.1091*dataCopy(len-10,0)+0.0182*dataCopy(len-9,0)+0.1152*dataCopy(len-8,0)+0.1818*dataCopy(len-7,0)+0.2182*dataCopy(len-6,0)+0.2241*dataCopy(len-5,0)+0.2*dataCopy(len-4,0)+0.1455*dataCopy(len-3,0)+0.0606*dataCopy(len-2,0)-0.0545*dataCopy(len-1,0);
			data(len-5,1) = -0.1091*dataCopy(len-10,1)+0.0182*dataCopy(len-9,1)+0.1152*dataCopy(len-8,1)+0.1818*dataCopy(len-7,1)+0.2182*dataCopy(len-6,1)+0.2241*dataCopy(len-5,1)+0.2*dataCopy(len-4,1)+0.1455*dataCopy(len-3,1)+0.0606*dataCopy(len-2,1)-0.0545*dataCopy(len-1,1);
																																																																																																																																																													
			data(len-4,0) = -0.1273*dataCopy(len-9,0)+0.0182*dataCopy(len-8,0)+0.1268*dataCopy(len-7,0)+0.1987*dataCopy(len-6,0)+0.2338*dataCopy(len-5,0)+0.232*dataCopy(len-4,0)+0.1935*dataCopy(len-3,0)+0.1182*dataCopy(len-2,0)+0.0061*dataCopy(len-1,0);
			data(len-4,1) = -0.1273*dataCopy(len-9,1)+0.0182*dataCopy(len-8,1)+0.1268*dataCopy(len-7,1)+0.1987*dataCopy(len-6,1)+0.2338*dataCopy(len-5,1)+0.232*dataCopy(len-4,1)+0.1935*dataCopy(len-3,1)+0.1182*dataCopy(len-2,1)+0.0061*dataCopy(len-1,1);

			data(len-3,0) = -0.125*dataCopy(len-8,0)+0.0179*dataCopy(len-7,0)+0.125*dataCopy(len-6,0)+0.1964*dataCopy(len-5,0)+0.2321*dataCopy(len-4,0)+0.2322*dataCopy(len-3,0)+0.1964*dataCopy(len-2,0)+0.125*dataCopy(len-1,0);
			data(len-3,1) = -0.125*dataCopy(len-8,1)+0.0179*dataCopy(len-7,1)+0.125*dataCopy(len-6,1)+0.1964*dataCopy(len-5,1)+0.2321*dataCopy(len-4,1)+0.2322*dataCopy(len-3,1)+0.1964*dataCopy(len-2,1)+0.125*dataCopy(len-1,1); 
																																																																																																												
			data(len-2,0) = -0.0714*dataCopy(len-7,0)+0.0000*dataCopy(len-6,0)+0.0714*dataCopy(len-5,0)+0.1429*dataCopy(len-4,0)+0.2143*dataCopy(len-3,0)+0.2857*dataCopy(len-2,0)+0.3571*dataCopy(len-1,0);
			data(len-2,1) = -0.0714*dataCopy(len-7,1)+0.0000*dataCopy(len-6,1)+0.0714*dataCopy(len-5,1)+0.1429*dataCopy(len-4,1)+0.2143*dataCopy(len-3,1)+0.2857*dataCopy(len-2,1)+0.3571*dataCopy(len-1,1); 

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011

	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m13(RMSeries& data)  //ADDITION FEB2010
{
	int nT = m_numIter;
	int nL = m_leftWindowSize;
	int nR = m_rightWindowSize;
	int len = data.length;
	double musx, musy, mdsx, mdsy;

	// Set of weight coefficients
	m_coeffs(0) = -0.0769;			//-6
	m_coeffs(1) = 0;				//-5
	m_coeffs(2) = 0.0629;			//-4
	m_coeffs(3) = 0.1119;			//-3
	m_coeffs(4) = 0.1469;			//-2
	m_coeffs(5) = 0.1678;			//-1
	m_coeffs(6) = 0.1748;			//0
	m_coeffs(7) = 0.1678;			//+1
	m_coeffs(8) = 0.1469;			//+2
	m_coeffs(9) = 0.1119;			//+3
	m_coeffs(10) = 0.0629;			//+4
	m_coeffs(11) = 0;				//+5
	m_coeffs(12) = -0.0769;			//+6

	for(int m = 0; m <= nT-1; m++)//DM-FEB2011
		{//DM-FEB2011
			// Copy of data
			RMSeries dataCopy = data;
				
			// Filter application 
			for (int n = 0; n <= nT - 1; n++)
			{
				dataCopy = data;

				for (int i = nL; i <= len-1-nR; i++)
				{
					double xtemp = 0.0;
					double ytemp = 0.0;
					for (int k = 0; k <= (nL+nR); k++)
					{
						xtemp = xtemp + dataCopy(i-nL+k,0) * m_coeffs(k);
						ytemp = ytemp + dataCopy(i-nL+k,1) * m_coeffs(k);
					}
					data(i,0)= xtemp;
					data(i,1)= ytemp;
				}
			}

			// Filter application for the nodes close to the ends
			// Observe that the coefficients of the filter are here entered manually for each node 
			data(0,0) = dataCopy(0,0);
			data(0,1) = dataCopy(0,1);

			data(1,0) = 0.375*dataCopy(0,0)+0.2798*dataCopy(1,0)+0.1964*dataCopy(2,0)+0.125*dataCopy(3,0)+0.0655*dataCopy(4,0)+0.0179*dataCopy(5,0)-0.0179*dataCopy(6,0)-0.0417*dataCopy(7,0);
			data(1,1) = 0.375*dataCopy(0,1)+0.2798*dataCopy(1,1)+0.1964*dataCopy(2,1)+0.125*dataCopy(3,1)+0.0655*dataCopy(4,1)+0.0179*dataCopy(5,1)-0.0179*dataCopy(6,1)-0.0417*dataCopy(7,1); 


			data(2,0) = 0.1636*dataCopy(0,0)+0.1909*dataCopy(1,0)+0.2010*dataCopy(2,0)+0.1935*dataCopy(3,0)+0.1688*dataCopy(4,0)+0.1268*dataCopy(5,0)+0.0675*dataCopy(6,0)-0.0091*dataCopy(7,0)-0.103*dataCopy(8,0);
			data(2,1) = 0.1636*dataCopy(0,1)+0.1909*dataCopy(1,1)+0.2010*dataCopy(2,1)+0.1935*dataCopy(3,1)+0.1688*dataCopy(4,1)+0.1268*dataCopy(5,1)+0.0675*dataCopy(6,1)-0.0091*dataCopy(7,1)-0.103*dataCopy(8,1);

				
			data(3,0) = 0.0455*dataCopy(0,0)+0.1182*dataCopy(1,0)+0.1682*dataCopy(2,0)+0.1954*dataCopy(3,0)+0.2*dataCopy(4,0)+0.1818*dataCopy(5,0)+0.1409*dataCopy(6,0)+0.0773*dataCopy(7,0)-0.0091*dataCopy(8,0)-0.1182*dataCopy(9,0);
			data(3,1) = 0.0455*dataCopy(0,1)+0.1182*dataCopy(1,1)+0.1682*dataCopy(2,1)+0.1954*dataCopy(3,1)+0.2*dataCopy(4,1)+0.1818*dataCopy(5,1)+0.1409*dataCopy(6,1)+0.0773*dataCopy(7,1)-0.0091*dataCopy(8,1)-0.1182*dataCopy(9,1);
			
			data(4,0) = -0.021*dataCopy(0,0)+0.0643*dataCopy(1,0)+0.1287*dataCopy(2,0)+0.172*dataCopy(3,0)+0.1945*dataCopy(4,0)+0.1958*dataCopy(5,0)+0.1762*dataCopy(6,0)+0.1357*dataCopy(7,0)+0.0741*dataCopy(8,0)-0.0084*dataCopy(9,0)-0.1119*dataCopy(10,0);
			data(4,1) = -0.021*dataCopy(0,1)+0.0643*dataCopy(1,1)+0.1287*dataCopy(2,1)+0.172*dataCopy(3,1)+0.1945*dataCopy(4,1)+0.1958*dataCopy(5,1)+0.1762*dataCopy(6,1)+0.1357*dataCopy(7,1)+0.0741*dataCopy(8,1)-0.0084*dataCopy(9,1)-0.1119*dataCopy(10,1);

			data(5,0) = -0.0577*dataCopy(0,0)+0.0262*dataCopy(1,0)+0.0927*dataCopy(2,0)+0.1416*dataCopy(3,0)+0.1731*dataCopy(4,0)+0.1870*dataCopy(5,0)+0.1836*dataCopy(6,0)+0.1626*dataCopy(7,0)+0.1241*dataCopy(8,0)+0.0682*dataCopy(9,0)-0.0052*dataCopy(10,0)-0.0962*dataCopy(11,0);
			data(5,1) = -0.0577*dataCopy(0,1)+0.0262*dataCopy(1,1)+0.0927*dataCopy(2,1)+0.1416*dataCopy(3,1)+0.1731*dataCopy(4,1)+0.1870*dataCopy(5,1)+0.1836*dataCopy(6,1)+0.1626*dataCopy(7,1)+0.1241*dataCopy(8,1)+0.0682*dataCopy(9,1)-0.0052*dataCopy(10,1)-0.0962*dataCopy(11,1);

			data(len-6,0) = -0.0962*dataCopy(len-12,0)-0.0052*dataCopy(len-11,0)+0.0682*dataCopy(len-10,0)+0.1241*dataCopy(len-9,0)+0.1626*dataCopy(len-8,0)+0.1836*dataCopy(len-7,0)+0.1870*dataCopy(len-6,0)+0.1731*dataCopy(len-5,0)+0.1416*dataCopy(len-4,0)+0.0927*dataCopy(len-3,0)+0.0262*dataCopy(len-2,0)-0.0577*dataCopy(len-1,0);
			data(len-6,1) = -0.0962*dataCopy(len-12,1)-0.0052*dataCopy(len-11,1)+0.0682*dataCopy(len-10,1)+0.1241*dataCopy(len-9,1)+0.1626*dataCopy(len-8,1)+0.1836*dataCopy(len-7,1)+0.1870*dataCopy(len-6,1)+0.1731*dataCopy(len-5,1)+0.1416*dataCopy(len-4,1)+0.0927*dataCopy(len-3,1)+0.0262*dataCopy(len-2,1)-0.0577*dataCopy(len-1,1);

			data(len-5,0) = -0.1119*dataCopy(len-11,0)-0.0084*dataCopy(len-10,0)+0.0741*dataCopy(len-9,0)+0.1357*dataCopy(len-8,0)+0.1762*dataCopy(len-7,0)+0.1958*dataCopy(len-6,0)+0.1945*dataCopy(len-5,0)+0.172*dataCopy(len-4,0)+0.1287*dataCopy(len-3,0)+0.0643*dataCopy(len-2,0)-0.021*dataCopy(len-1,0);
			data(len-5,1) = -0.1119*dataCopy(len-11,1)-0.0084*dataCopy(len-10,1)+0.0741*dataCopy(len-9,1)+0.1357*dataCopy(len-8,1)+0.1762*dataCopy(len-7,1)+0.1958*dataCopy(len-6,1)+0.1945*dataCopy(len-5,1)+0.172*dataCopy(len-4,1)+0.1287*dataCopy(len-3,1)+0.0643*dataCopy(len-2,1)-0.021*dataCopy(len-1,1);

			data(len-4,0) = -0.1182*dataCopy(len-10,0)-0.0091*dataCopy(len-9,0)+0.0773*dataCopy(len-8,0)+0.1409*dataCopy(len-7,0)+0.1818*dataCopy(len-6,0)+0.2*dataCopy(len-5,0)+0.1954*dataCopy(len-4,0)+0.1682*dataCopy(len-3,0)+0.1182*dataCopy(len-2,0)+0.0455*dataCopy(len-1,0);
			data(len-4,1) = -0.1182*dataCopy(len-10,1)-0.0091*dataCopy(len-9,1)+0.0773*dataCopy(len-8,1)+0.1409*dataCopy(len-7,1)+0.1818*dataCopy(len-6,1)+0.2*dataCopy(len-5,1)+0.1954*dataCopy(len-4,1)+0.1682*dataCopy(len-3,1)+0.1182*dataCopy(len-2,1)+0.0455*dataCopy(len-1,1); 

			data(len-3,0) = -0.103*dataCopy(len-9,0)-0.0091*dataCopy(len-8,0)+0.0675*dataCopy(len-7,0)+0.1268*dataCopy(len-6,0)+0.1688*dataCopy(len-5,0)+0.1935*dataCopy(len-4,0)+0.2010*dataCopy(len-3,0)+0.1909*dataCopy(len-2,0)+0.1636*dataCopy(len-1,0);
			data(len-3,1) = -0.103*dataCopy(len-9,1)-0.0091*dataCopy(len-8,1)+0.0675*dataCopy(len-7,1)+0.1268*dataCopy(len-6,1)+0.1688*dataCopy(len-5,1)+0.1935*dataCopy(len-4,1)+0.2010*dataCopy(len-3,1)+0.1909*dataCopy(len-2,1)+0.1636*dataCopy(len-1,1); 

			data(len-2,0) = -0.0417*dataCopy(len-8,0)-0.0179*dataCopy(len-7,0)+0.0179*dataCopy(len-6,0)+0.0655*dataCopy(len-5,0)+0.125*dataCopy(len-4,0)+0.1964*dataCopy(len-3,0)+0.2798*dataCopy(len-2,0)+0.375*dataCopy(len-1,0);
			data(len-2,1) = -0.0417*dataCopy(len-8,1)-0.0179*dataCopy(len-7,1)+0.0179*dataCopy(len-6,1)+0.0655*dataCopy(len-5,1)+0.125*dataCopy(len-4,1)+0.1964*dataCopy(len-3,1)+0.2798*dataCopy(len-2,1)+0.375*dataCopy(len-1,1); 

			data(len-1,0) = dataCopy(len-1,0);
			data(len-1,1) = dataCopy(len-1,1);

			// End nodes
			// The end nodes are calculated with linear extrapolation 
			musx = (data(2,0)-data(1,0)) / m_intervals(2);
			musy = (data(2,1)-data(1,1)) / m_intervals(2);
			data(0,0) = data(1,0) - musx * m_intervals(1);
			data(0,1) = data(1,1) - musy * m_intervals(1);

			mdsx = (data(len-2,0)-data(len-3,0)) / m_intervals(len-2);
			mdsy = (data(len-2,1)-data(len-3,1)) / m_intervals(len-2);
			data(len-1,0) = data(len-2,0) + mdsx * m_intervals(len-1);
			data(len-1,1) = data(len-2,1) + mdsy * m_intervals(len-1);
		}//DM-FEB2011
	return true;
}

bool RMFilter_SavitzkyGolay::SavitzkyGolay_p2m15(RMSeries& data)  //ADDITION FEB2010
{
	return true;
}
