#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUMTRIES 10

#ifndef NUMT 
#define NUMT 2
#endif
#ifndef NUMS
#define NUMS 1000
#endif


float Ranf(float, float);

int main(int argc, char const *argv[])
{
	omp_set_num_threads(NUMT);

    fprintf( stderr, "Using %d threads\n", NUMT );

	float *inputArray = new float[NUMS];
	float *outputArray = new float[NUMS];

	double sumMegaSqrts = 0.;

	// Initialize inputArray with random stuff
	for (int i = 0; i < NUMS; i++)
	{
		inputArray[i] = Ranf(-1.f, 1.f);
	}

	// Print out the contents of the array
	/*for (int i = 0; i < NUMS; ++i)
	{
		printf("%f\n", inputArray[i]);
	}*/

	for (int i = 0; i < NUMTRIES; i++)
	{
		/* code */
		double start = omp_get_wtime();

		#pragma omp parallel for simd 
		for( int i = 0; i < NUMS; i++ )
		{
		    outputArray[i] = sqrt( inputArray[i] );
		}

		double end = omp_get_wtime();
		double mflops = (double)NUMS/(end-start)/1000000.;
		sumMegaSqrts += mflops;
	}
	
	double avgMegaSqrts = sumMegaSqrts/(double)NUMTRIES;
	// Print out the results
	printf("Nodes: %d\n", NUMS);
	printf("Average Performance: %10.2lf MFLOPS\n", avgMegaSqrts);

	return 0;
}


float
Ranf(float low, float high)
{
    float r = (float) rand();   // 0 - RAND_MAX
    return(   low  +  r * ( high - low ) / (float)RAND_MAX);
}