/*
I2C Communication code block
*/

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

int main(int argc, char *argv[])
{
    double start;

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    int i2c_handle = i2cOpen( 1, 0x2F, 0 );
    if ( i2c_handle < 0 )
    {
        fprintf(stderr, "I2C initialisation failed : %d\n", i2c_handle );
        return 1;
    }

    uint8_t data = 127;
    int ret = i2cWriteByte( i2c_handle, 0 );

    if ( ret != 0 )
    {
        fprintf( stderr, "Problem!: %d\n", ret );
    }


    

    time_sleep( 60 );

    i2cClose( i2c_handle );

    return 0;

}
