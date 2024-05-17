/*
    // gcc -Wall  -o pwm  pwm.c -lpigpio -lrt -lpthread
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

/*
 * Assuming maximum distance of interest is 5m:
 * Maximum travel time of the sound wave is 30 ms.
 * Therefore the "listen" time must be at least 30 ms.
 *
 * In each burst, it is required to have 20 pulses. 
 * At nominal frequency og 40 kHz, the burst duration is 500 us.
 * Changes in burst duration w.r.t. frequency are neglected.

 * Therefore the total period of one transmit cycle must be 30.5 ms.
 * Which results in 32.79 Hz frequency for the inhibit pin control PWM.

 * For the nominal 40 kHz, the duty therefore must be 60/61.
 * duty = 1 - (500e-6)/(30*5e-3) = 60/61
 * Out of 1M, that equals to 983607.
*/

#define PWM_FREQ 32.79
#define PWM_DUTY 983607


int calculate_duty( const int freq )
{
    return ( 1.0 - ( ( 20.0 / freq ) / 30.5e-3 ) ) * 1e6 ;
}


int main( int argc, char **argv )
{

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    // gpioSetMode(4, PI_OUTPUT);


    // int gpioHardwarePWM(unsigned gpio, unsigned PWMfreq, unsigned PWMduty)
    // PWM : 12 13
    // PWMduty out of 1.000.000. 800e3 --> %80 on time.
    int ret = gpioHardwarePWM( 12, PWM_FREQ, PWM_DUTY );
    
    if ( ret != 0 )
    {
        fprintf( stderr, "Failed to generate PWM\n" );
        return -1;
    }

    char response[ 64 ] = { 0 };

    for ( ;; )
    {
        // ask user for frequency:
        printf( "Frequency [Hz]: " );
        
        // get and parse the response:
        if ( fgets( response, sizeof response, stdin ) == NULL )
        {
            fprintf( stderr, "Could not make sense...\n" );
        }

        char *end;
        long frequency = strtol( response, &end, 10 );
        
        if ( response == end )
        {
            fprintf( stderr, "Failed to parse input frequency...\n" );
            continue;
        }

        // calculate the necessary duty at given frequency:
        int duty = calculate_duty( frequency );

        // adjust the PWM frequency:
        int ret = gpioHardwarePWM( 12, PWM_FREQ, duty );
    
        if ( ret != 0 )
        {
            fprintf( stderr, "Failed to generate PWM\n" );
            return -1;
        }
    }



    return 0;
}
