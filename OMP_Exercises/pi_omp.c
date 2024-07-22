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


int main ()
{
	  static const long num_steps = 100000000;
	  //static const long num_steps = 100;
	  double step;

	  double start_time, run_time;
	  //const unsigned int NUM_THREAD_REQ = 4;
	  const unsigned int NUM_THREAD_REQ = std::max(atoi(std::getenv("OMP_NUM_THREADS")), 1);
	  std::cout << "Num threads: " << NUM_THREAD_REQ << std::endl;

	  step = 1.0/(double) num_steps;

	  start_time = omp_get_wtime();

	  double sum[NUM_THREAD_REQ];
	  for (int i=0; i<NUM_THREAD_REQ; i++)
	  {
		sum[i] = 0.0;
	  }

	  //omp_set_num_threads(NUM_THREAD_REQ);
	  #pragma omp parallel
	  {
		int ID = omp_get_thread_num();

		const unsigned int NUM_THREADS = omp_get_num_threads();
		const unsigned int NUM_STEPS_PER_THREAD = num_steps / NUM_THREADS;
		const unsigned int NUM_STEPS_LEFTOVER = num_steps % NUM_THREADS;
		// printf("Num threads: %d\n", NUM_THREADS);
		// printf("Num steps per thread: %d\n", NUM_STEPS_PER_THREAD);
		// printf("Num steps leftover: %d\n", NUM_STEPS_LEFTOVER);
		if (ID == 0)
		{
			printf("OMP using %d threads", NUM_THREADS);
		}

		int start = NUM_STEPS_PER_THREAD * ID + 1;
		int end = (ID+1) * NUM_STEPS_PER_THREAD;
		if (ID == NUM_THREADS-1)
		{
			end = num_steps;
		}

		//printf("\n Thread %d starts loop at %d, ends at %d\n ", ID, start, end);
		sum[ID] == 0.0;
		for (int i=start; i<=end; i++)
		{
			double x = (i-0.5)*step;
			sum[ID] = sum[ID] + 4.0/(1.0+x*x);
			// if (ID == 0)
			// {
			// 	printf("i: %d\n", i);
			// 	if (i>end && ID == 0)
			// 	{
			// 		printf("BREAK: i=%d, end=%i\n", i, end);
			// 	}
			// }
		}
	  }

	  double pi = 0.0;
	  for (int i = 0; i < NUM_THREAD_REQ; i++)
	  {
		//printf("Sum[%d] = %f\n", i, sum[i]);
		pi += step * sum[i];
	  }

	  run_time = omp_get_wtime() - start_time;

	  //Expect: pi with 100000000 steps is 3.141593 in 0.371373 seconds
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}	  





