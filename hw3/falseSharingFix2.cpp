#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


// Don't need to define these if I am using a script
// The script defines them for me and uses them to 
// run the program.
//#define NUMT 4
//#define NUM 15

struct s
{

    float value;
    // wE DoN't uSe PadDing hErE!

} Array[4];

 

int main(int argc, char const *argv[])
{
    omp_set_num_threads( NUMT );
    int someBigNumber = 1000000000;

    fprintf( stderr, "Using %d threads\n", NUMT );

    double start = omp_get_wtime( );

 
    #pragma omp parallel for
    for( int i = 0; i < 4; i++ )
    {
        //FIX 2
        float tmp = Array[i].value;
        for( int j = 0; j < someBigNumber; j++ )
        {
            //FIX 2
            tmp = tmp + 2.;
        }
        //FIX 2
        Array[i].value = tmp;

    }

    double end = omp_get_wtime( );


    double msomethings = ((double)(4.0*someBigNumber))/(end-start)/1000000.;

    //printf("Start: %d\n", start);
    //printf("End:   %d\n", end);

    printf("Performance: %10.2lf MFLOPS\n\n", msomethings);
    //printf("Elapsed Time: %10.2lf ms\n", 1000000. * (end-start));

}
