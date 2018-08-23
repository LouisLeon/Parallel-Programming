#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


#define NUM_AGENTS 4

// GLOBALS
int   NowYear;           // 2014 - 2019
int   NowMonth;          // 0 - 11

float NowPrecip;        // inches of rain per month
float NowTemp;          // temperature this month
float NowHeight;        // grain height in inches
int   NowNumDeer;       // current deer population

float NowRGDPercentage;	// The percentage of the current crops affected by
						// Rare Grain Disease.

unsigned int seed;

// CONSTANTS FOR MONTHLY TIMESTEPS
// Units of grain growth are inches. 
// Units of temperature are degrees Farhrenheit (°F). 
// Units of precipitation are inches.
const float GRAIN_GROWS_PER_MONTH =             8.0;
const float ONE_DEER_EATS_PER_MONTH =           0.5;

const float AVG_PRECIP_PER_MONTH =              6.0;
const float AMP_PRECIP_PER_MONTH =              6.0;
const float RANDOM_PRECIP =                     2.0;

const float AVG_TEMP =                          50.0;
const float AMP_TEMP =                          20.0;
const float RANDOM_TEMP =                       10.0;

const float MIDTEMP =                           40.0;
const float MIDPRECIP =                         10.0;

const int END_YEAR = 							2020;
const float RANDOM_RARE_GRAIN_DISEASE =         13.0;
const float AVG_RARE_GRAIN_DISEASE =			5.0;

// PROTOTYPES
float Ranf(float, float, unsigned int*);
void GrainDeer();
void Grain();
void Watcher();
void RareGrainDisease();
void PrintGlobals();
void CalcTempPrecip(unsigned int);

int main(int argc, char const *argv[])
{
	// Set up initial state
	NowNumDeer 	= 	   1;
	NowHeight 	=  	  1.;
	NowMonth 	=      0;
	NowYear  	= 	2014;

	unsigned int seed = 0;
	// Calculate the initial Temperature and Precipitation
	CalcTempPrecip(seed);

	// BEGIN THE SIMULATION
	// Need num_threads equal to sections
	omp_set_num_threads(NUM_AGENTS);
	#pragma omp parallel sections
	{
	    #pragma omp section
	    {
	        GrainDeer();
	    }
	    #pragma omp section
	    {
	        Grain();
	    }
	    #pragma omp section
	    {
	    	RareGrainDisease();
	    }
	    #pragma omp section
	    {
	    	printf("Date,Temp,Precip,Deer,Grain,RareGrainDisease\n");
	        Watcher();
	    }
	    // implied barrier: all sections must complete before we get here
	}

	return 0;
}

// Calculates Deer Population Growth
void GrainDeer()
{
	// Temporary Variables
	int tmpDeer;

	while(NowYear <= END_YEAR)
	{
		tmpDeer = NowNumDeer;
		if((float)tmpDeer > NowHeight)
			tmpDeer--;
		else if((float) tmpDeer < NowHeight)
			tmpDeer++;

		// Rare Grain Disease is prominent and GrainDeer try to eat it
		// This kills the GrainDeer.
		if(NowRGDPercentage > 5.)
			tmpDeer--;

		if(tmpDeer < 0)
			tmpDeer = 0;

		// DONE COMPUTING BARRIER
		#pragma omp barrier
		NowNumDeer = tmpDeer;

		// DONE ASSIGNING BARRIER
		#pragma omp barrier


		// DONE PRINTING BARRIER
		#pragma omp barrier

	}

}

// Calculates New Grain Height
void Grain()
{
	// Temporary Variables
	float tempHeight;
	float tempFactor;
	float precipFactor;

	while(NowYear <= END_YEAR)
	{
		tempHeight = NowHeight;
		tempFactor = exp( - (pow( ((NowTemp - MIDTEMP)/10.), 2.0)));
		precipFactor = exp( - (pow( ((NowPrecip - MIDPRECIP)/10.), 2.0)));

		tempHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		tempHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

		if(tempHeight < 0.)
			tempHeight = 0.;
		// DONE COMPUTING BARRIER
		#pragma omp barrier
		NowHeight = tempHeight;

		// DONE ASSIGNING BARRIER
		#pragma omp barrier


		// DONE PRINTING BARRIER
		#pragma omp barrier
	}
}

// Updates the temperature and precipitation per month
// as well as prints out the current globals
void Watcher()
{
	int monthIncr;
	unsigned int seed = 6;
	while(NowYear <= END_YEAR)
	{
		// DONE COMPUTING BARRIER
		#pragma omp barrier

		// DONE ASSIGNING BARRIER
		#pragma omp barrier

		PrintGlobals();

		// Increment Month and year if needed
		NowMonth++;
		if(NowMonth > 11)
		{
			NowMonth = 0;
			NowYear++;
		}

		CalcTempPrecip(seed);

		// DONE PRINTING BARRIER
		#pragma omp barrier
	}
}

// This is my custom agent. It introduces a special
// disease which spawns randomly in some areas of the grain.
// When a GrainDeer ingests these areas of the grain, they 
// become sick and die.
void RareGrainDisease()
{
	float tempRGDIncr; // A temporary value for storing the percentage of Rare Grain Disease
	unsigned int seed = 0;

	while(NowYear <= END_YEAR)
	{
		tempRGDIncr = NowRGDPercentage;

		if(NowHeight > 5. && NowPrecip < 5.)
		{
			tempRGDIncr = AVG_RARE_GRAIN_DISEASE + Ranf(-RANDOM_RARE_GRAIN_DISEASE, RANDOM_RARE_GRAIN_DISEASE, &seed);
		}

		if(tempRGDIncr < 0.)
			tempRGDIncr = 0.;

		// DONE COMPUTING BARRIER
		#pragma omp barrier
		NowRGDPercentage = tempRGDIncr;

		// DONE ASSIGNING BARRIER
		#pragma omp barrier


		// DONE PRINTING BARRIER
		#pragma omp barrier

	}
}


void PrintGlobals()
{
    printf("%d/%d,%f,%f,%d,%f,%f\n", NowMonth + 1, NowYear, (5./9.)*(NowTemp-32.), NowPrecip * 2.54, NowNumDeer, NowHeight * 2.54, NowRGDPercentage);
}

void CalcTempPrecip(unsigned int seed)
{
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
	float temp = AVG_TEMP - AMP_TEMP * cos( ang );

	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP, &seed );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );

	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP, &seed );

	if( NowPrecip < 0. )
		NowPrecip = 0.;
	
}

float
Ranf( float low, float high, unsigned int* seed )
{
    float r = (float) rand_r(seed);      // 0 - RAND_MAX
    return( low + r * ( high - low ) / (float)RAND_MAX );
}
