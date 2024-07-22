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
#include <algorithm>
#include <iostream>

int main()
{
	static const long NUM_STEPS = 100000000;

	double start_time = omp_get_wtime();

	double step = 1.0 / (double)NUM_STEPS;

	double tot_sum = 0.0;
	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		int num_threads = omp_get_num_threads();

		if (ID == 0)
		{
			printf("OMP using %d threads", num_threads);
		}

		int num_steps_per_thread = NUM_STEPS / num_threads;
		int start = num_steps_per_thread*ID + 1;
		int end = (ID == num_threads-1) ? NUM_STEPS : (ID+1) * num_steps_per_thread;
	
		// printf("\n Thread %d starts loop at %d, ends at %d\n ", ID, start, end);
		double thread_sum = 0.0;
		for (int i=start; i<=end; i++)
		{
			double x = (i - 0.5) * step;
			thread_sum += 4.0 / (1.0 + x * x);
		}

		#pragma omp critical
		{
			tot_sum += thread_sum;
		}
	} // end omp parallel

	double pi = step * tot_sum;

	double run_time = omp_get_wtime() - start_time;

	// Single thread result: pi with 100000000 steps is 3.141593 in 0.371373 seconds
	printf("\nPi with %ld steps is %lf in %lf seconds\n ", NUM_STEPS, pi, run_time);
}