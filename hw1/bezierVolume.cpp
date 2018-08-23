/*
By: Louis Leon
Spring 2018
CS 475 - Parallel Programming
*/

#include <stdio.h>
#include <omp.h>

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.

#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.

#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.

#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.

#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.

#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.

#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.

//#define NUMNODES 4000
//#define NUMT	 8

double
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	double u = (double)iu / (double)(NUMNODES-1);
	double v = (double)iv / (double)(NUMNODES-1);

	// the basis functions:

	double bu0 = (1.-u) * (1.-u) * (1.-u);
	double bu1 = 3. * u * (1.-u) * (1.-u);
	double bu2 = 3. * u * u * (1.-u);
	double bu3 = u * u * u;

	double bv0 = (1.-v) * (1.-v) * (1.-v);
	double bv1 = 3. * v * (1.-v) * (1.-v);
	double bv2 = 3. * v * v * (1.-v);
	double bv3 = v * v * v;

	// finally, we get to compute something:


        double top =       bu0 * ( bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03 )
                        + bu1 * ( bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13 )
                        + bu2 * ( bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23 )
                        + bu3 * ( bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33 );

        double bot =       bu0 * ( bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03 )
                        + bu1 * ( bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13 )
                        + bu2 * ( bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23 )
                        + bu3 * ( bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33 );

        return top - bot;	// if the bottom surface sticks out above the top surface
				// then that contribution to the overall volume is negative
}



int main( int argc, char *argv[ ] )
{
	
	// Set the number of threads we are using
	omp_set_num_threads(NUMT);

	fprintf( stderr, "Using %d threads\n", NUMT );

	// The area of a single full-sized tile:
	double fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );

	double halfTileArea = fullTileArea/2;

	double quarterTileArea = fullTileArea/4;


	double height = 0;
	double volume = 0;
	int iu = 0;
	int iv = 0;

	// Begin the timing
	double start = omp_get_wtime();

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
	#pragma omp parallel for default(none), reduction(+:volume), private(iv,iu,height), shared(fullTileArea, halfTileArea, quarterTileArea)
	for( int i = 0; i < NUMNODES*NUMNODES; i++ )
	{
		iu = i % NUMNODES;
		iv = i / NUMNODES;

		height = Height(iu,iv);
		
		// FULL TILE
		if (iu > 0 && iu < NUMNODES-1 && iv > 0 && iv < NUMNODES-1) {
			volume += height * fullTileArea;
		}
		// QUARTER TILE
		else if ((iu == NUMNODES-1 || iu == 0) && (iv == NUMNODES-1 || iv == 0))
		{
			volume += height * quarterTileArea;
		}
		// HALF TILE 
		else 
		{
			volume += height * halfTileArea;
		}

	}
	
	// End the timing
	double end = omp_get_wtime();
	// Performance in MEGAFLOPS
	double mflops = (double)NUMNODES*NUMNODES/(end-start)/1000000.; 


	printf("Total volume: %f\n", volume);
	printf("Nodes: %i\n", NUMNODES);
	printf("Performance: %10.2lf MFLOPS\n", mflops);
	printf("Elapsed Time: %10.2lf ms\n", 1000000. * (end-start));
}
