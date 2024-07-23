/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS 1000
#define MAXITER 1000

bool testpoint(double, double);

int main()
{
  const double EPS = 1.0e-5;

  //   Loop over grid of points in the complex plane which contains the Mandelbrot set,
  //   testing each point to see whether it is inside or outside the set.
  size_t numoutside = 0;
  #pragma omp parallel for reduction(+:numoutside)
  for (int i = 0; i < NPOINTS; i++)
  {
    for (int j = 0; j < NPOINTS; j++)
    {
      double creal = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + EPS;
      double cimag = 1.125 * (double)(j) / (double)(NPOINTS) + EPS;
      bool is_outside = testpoint(creal, cimag);
      if (is_outside)
      {
        numoutside++;
      }
    }
  }

  // Calculate area of set and error estimate and output the results
  double area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  double error = area / (double)NPOINTS;

  printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n", area, error);
  printf("%12.8f <= area <= %12.8f\n", area-error, area+error);
  printf("Correct answer should be around 1.510659\n");
}

bool testpoint(double creal, double cimag)
{
  // iterate z=z*z+c, until |z| > 2 when point is known to be outside set
  // If loop count reaches MAXITER, point is considered to be inside the set

  double zreal, zimag, temp;
  zreal = creal;
  zimag = cimag;

  for (int iter = 0; iter < MAXITER; iter++)
  {
    temp = (zreal * zreal) - (zimag * zimag) + creal;
    zimag = zreal * zimag * 2 + cimag;
    zreal = temp;
    if ((zreal * zreal + zimag * zimag) > 4.0)
    {
      return true;
    }
  }

  return false;
}