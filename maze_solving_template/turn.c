#include <pololu/3pi.h>

// Define speeds that you'd like to be using
#define SLOW 25
#define MEDIUM 80
#define FAST 150

// Define the time you'd like to take to turn
#define DELAY_TURN      200
#define DELAY_UTURN     400

/* Define your code for turning here.
 *  Define your 'API' here:
 *      In this example, an 'L' means to turn left.
 *      This 'L' is selected from `select_turn`
 *  See where this implemented in the first_main_loop() in main.c
 */

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
        default:
            // do nothing
            break;
    }
}

// This function decides which way to turn during the learning phase of
// maze solving. It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
    // Make a decision about how to turn. The following code
    // implements a left-hand-on-the-wall strategy, where we always
    // turn as far to the left as possible.
    if(found_left)
        return 'L';
    else if(found_straight)
        // Straight goes here
        // FIX ME
        return 'L';
    else if(found_right)
        // Return Right
        // FIX ME
        return 'L';
    else
        // Go backwards
        // FIX ME
        return 'L';
}
