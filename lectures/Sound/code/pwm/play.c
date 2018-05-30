#include "gpio.h"
#include "pwm.h"

#define FREQ (19200000/2)
#define HZ 1000

void main ( void )
{
    gpio_init();
    gpio_set_function( 18, GPIO_FUNC_ALT5 ); 
    gpio_set_function( 40, GPIO_FUNC_ALT0 ); // Stereo left
    gpio_set_function( 45, GPIO_FUNC_ALT0 ); // Stereo right

    pwm_init();
    pwm_set_clock( FREQ );

    pwm_set_mode( 0, PWM_MARKSPACE);
    pwm_set_fifo( 0, 0 );
    pwm_enable( 0 );

    pwm_set_mode( 1, PWM_MARKSPACE);
    pwm_set_fifo( 1, 0 );
    pwm_enable( 1 );

    int width = FREQ/HZ;
    pwm_set_range( 0, width ); 
    pwm_set_width( 0, width/4 );  
    pwm_set_range( 1, width ); 
    pwm_set_width( 1, width/2 );  

    while(1) ;
}

