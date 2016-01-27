#include <polulu/3pi.h>

// Define speeds that you'd like to be using
#define SLOW 25
#define MEDIUM 80
#define FAST 150

// Define the time you'd like to take to turn
#define DELAY_TURN      200
#define DELAY_UTURN     400

// Define your code for turning here.
//  Define your 'API' here:
void turn(char direction)
{
    // The easiest way to do this is with a switch/case statement
    switch(direction)
    {
        // Example case statement
        //  You need to provide your own logic here
        case 'L':
            // Turn left
            set_motors(-MEDIUM, MEDIUM);
            delay_ms(DELAY_TURN);
            break;
    }
}
