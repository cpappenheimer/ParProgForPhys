/*

This program will numerically compute the integral of

				  4/(1+x*x)

from 0 to 1.  The value of this integral is pi -- which
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/

#include <stdio.h>
#include <omp.h>

int main()
{
	static long NUM_STEPS = 100000000;
	double step = 1.0 / (double)NUM_STEPS;

	double start_time = omp_get_wtime();

	double sum = 0.0;
	for (int i = 1; i <= NUM_STEPS; i++)
	{
		double x = (i - 0.5) * step;
		sum = sum + 4.0 / (1.0 + x * x);
	}
	double pi = step * sum;

	double run_time = omp_get_wtime() - start_time;

	// Expected output: pi with 100000000 steps is 3.141593 in 0.371373 seconds
	printf("\n pi with %ld steps is %lf in %lf seconds\n ", NUM_STEPS, pi, run_time);
}