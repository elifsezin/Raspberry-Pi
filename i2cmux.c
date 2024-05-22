/*
Code block to control I2C mux and communicate with choosen digital potentiometer
*/

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>

#define I2C_MUX_ADDR 0x70
#define I2C_DPOT_ADDR 0x2F

#define MUX_CHANNEL_COUNT 8
#define POT_MAX_RESISTANCE 45e3





void set_mux_out_ch( int num )
{
    if ( num >= 7 )
    {
        num = 7;
    }

    int i2c_handle = i2cOpen( 1, I2C_MUX_ADDR, 0 );
    if ( i2c_handle < 0 )
    {
        fprintf(stderr, "I2C initialisation failed : %d\n", i2c_handle );
        return;
    }

    uint8_t mux_channel_mask = 1 << num;

    int ret = i2cWriteByte( i2c_handle, mux_channel_mask );
    if ( ret != 0 )
    {
        fprintf( stderr, "Problem!: %d\n", mux_channel_mask );
    }

    i2cClose( i2c_handle );
}


int i2c_select_and_write( const int channel, uint8_t bayt  )
{
    // open the i2c port
    int i2c_handle_mux = i2cOpen( 1, I2C_MUX_ADDR, 0 );
    if ( i2c_handle_mux < 0 )
    {
        fprintf(stderr, "I2C initialisation failed : %d\n", i2c_handle_mux );
        return -1;
    }

    // check if the channel number is valid: 0 to 7.
    if ( channel > MUX_CHANNEL_COUNT )
    {
        fprintf(stderr, "Invalid channel number!\n" );
        return -1;
    }

    // set the channel select mask
    uint8_t mux_channel_mask = 1 << channel;

    // operation return variable
    int ret = 0;

    // write the channel select command
    ret = i2cWriteByte( i2c_handle_mux, mux_channel_mask );
    if ( ret != 0 )
    {
        fprintf( stderr, "Failed to select channel!: %d\n", ret );
        return -1;
    }

    i2cClose( i2c_handle_mux );





    int i2c_handle_pot = i2cOpen( 1, I2C_DPOT_ADDR, 0 );
    if ( i2c_handle_pot < 0 )
    {
        fprintf(stderr, "I2C initialisation failed : %d\n", i2c_handle_pot );
        return -1;
    }


    // write the data
    ret = i2cWriteByte( i2c_handle_pot, bayt );
    if ( ret != 0 )
    {
        fprintf( stderr, "Failed to write data!: %d\n", ret );
        return -1;
    }

    // close the i2c port.
    i2cClose( i2c_handle_pot );

    return 0;
}




int main( int argc, char *argv[] )
{
    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    int8_t dpot_values[ MUX_CHANNEL_COUNT ] = { 0 };

    // 4th, 6th and 7th channels are used for the test.

    // MAIN LOOP
    for ( ;; )
    {
        int pot_number = 0;
        int resistance = 0;

        // printf( "Enter pot number and the resistance respectively (<pot number>, <resistance in Ohms>):" );
        // int ret = scanf( "%d,%d", &pot_number, &resistance );

        // if ( ret != 2 )
        // {
        //     fprintf( stderr, "Arguments mismatched!\n" );
        // }

        printf( "Enter channel number: " );
        scanf( "%d", &pot_number );

        printf( "Enter resistance value in Ohms: " );
        scanf( "%d", &resistance );

        // map the resistance value between 0 and 127
        int temp_pot = ( (double)resistance / (double)POT_MAX_RESISTANCE  ) * 127;

        // clamp if requested value exceed maximum resistance of the pot
        temp_pot = temp_pot > 127 ? 127 : temp_pot;

        dpot_values[ pot_number ] = temp_pot;

        i2c_select_and_write( pot_number, dpot_values[ pot_number ] );
    }

    return 0;

}
