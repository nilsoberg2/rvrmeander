#ifndef __ACMALGORITHM463_H_________________200505051555__
#define __ACMALGORITHM463_H_________________200505051555__
#include <vtchl/math.hpp>



// ACMALGORITHM463 is used to compute the optimal tick spacing
// for an axis.
static bool ACMALGORITHM463(double min,
              double max,
              int numIntervals,
              double& newMin,
              double& newMax,
              double& tickSpacing)
{
  static const double vint[] = { 1.0, 2.0, 5.0, 10.0 };
  static const double sqr[] = { M_SQRT2, 3.16228, 7.07107 };

  if (min > max || numIntervals <= 0)
    return false;

  static const double NUMBERFUZZ = 0.00002;

  int FN = numIntervals;

  // Find approximate interval size A
  double A = (max - min) / FN;
  double AL = log10(A);
  double NAL = AL;
  if (A < 1.0)
    NAL = NAL - 1.0;

  // A is scaled into variable named B between 1 and 10
  double NAL_10 = pow(10.0, NAL);
  double B = A / NAL_10;

  // The closest permissible value for B is found
  bool foundValue = false;
  int i;
  for (i = 0; i < 3; i++)
  {
    if (B < sqr[i])
    {
      foundValue = true;
      break;
    }
  }

  if (! foundValue)
    i = 3;

  // The interval size is computed
  tickSpacing = vint[i] * NAL_10;
  double FM1 = min / tickSpacing;
  double M1 = FM1;
  if (FM1 < 0.0)
    M1 = M1 - 1.0;
  if (fabs(M1 + 1.0 - FM1) < NUMBERFUZZ)
    M1 = M1 + 1.0;

  // The new minimum and maximum limits are found
  newMin = tickSpacing * M1;
  double FM2 = max / tickSpacing;
  double M2 = FM2 + 1.0;
  if (FM2 < -1.0)
    M2 = M2 - 1.0;
  if (fabs(FM2 + 1.0 - M2) < NUMBERFUZZ)
    M2 = M2 - 1.0;
  newMax = tickSpacing * M2;

  // Adjust limits to account for round-off if necessary
  if (newMin > min)
    newMin = min;
  if (newMax < max)
    newMax = max;

  return true;
}


#endif//__ACMALGORITHM463_H_________________200505051555__
