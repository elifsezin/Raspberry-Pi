/*
    // gcc -Wall  -o onoff onoff.c -lpigpio -lrt -lpthread
*/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pigpio.h>

#define PIN_PWR_CTRL 17

int main( int argc, char **argv )
{

    if ( gpioInitialise() < 0 )
    {
        fprintf( stderr, "pigpio initialisation failed\n" );
        return -1;
    }

    gpioSetMode( PIN_PWR_CTRL, PI_OUTPUT );

    char command[ 64 ] = { 0 };

    // MAIN LOOP
    for( ;; )
    {
        if ( fgets( command, sizeof command, stdin ) == NULL )
        {
            fprintf( stderr, "Failed to read stdio.\n" );
        }

        if ( strcmp( command, "POWERUP\n" ) == 0 )
        {
            printf( "Powering up...\n" );
            gpioWrite( PIN_PWR_CTRL, 1 );
        }
        else if ( strcmp( command, "SHUTDOWN\n" ) == 0 )
        {
            printf( "Shutting down...\n" );
            gpioWrite( PIN_PWR_CTRL, 0 );
        }
       
        else
        {
            printf( "Could not make sense...\n" );
        }
    }


    return 0;
}