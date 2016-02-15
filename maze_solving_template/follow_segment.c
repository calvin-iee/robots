#include <pololu/3pi.h>

#include "turn.h"

void follow_segment()
{
    // Set up the constants for the PID Control
    //  You probably will not need to worry about those things
    int last_proportional = 0;
    long integral = 0;

    while(1)
    {
        // This will help us for following a line.

        // Get the position of the line
        unsigned int sensors[5];
        unsigned int position = read_line(sensors, IR_EMITTERS_ON);

        // Handle the P in our PID Control
        int proportional = ((int)position) - 2000;

        // Compute the derivative (change) and integral (sum) of the
        // position.
        int derivative = proportional - last_proportional;
        integral += proportional;
 
        // Remember the last position.
        last_proportional = proportional;

        // Compute the difference between the two motor power settings,
        // m1 - m2.  If this is a positive number the robot will turn
        // to the left.  If it is a negative number, the robot will
        // turn to the right, and the magnitude of the number determines
        // the sharpness of the turn.
        int power_difference = proportional/20 + integral/10000 + derivative*3/2;
 
        // Compute the actual motor settings.  We never set either motor
        // to a negative value.
        const int max = 60; // the maximum speed
        if(power_difference > max)
            power_difference = max;
        if(power_difference < -max)
            power_difference = -max;
         
        if(power_difference < 0)
            set_motors(max+power_difference,max);
        else
            set_motors(max,max-power_difference);
 
        // We use the inner three sensors (1, 2, and 3) for
        // determining whether there is a line straight ahead, and the
        // sensors 0 and 4 for detecting lines going to the left and
        // right.
 
        if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
        {
            // There is no line visible ahead, and we didn't see any
            // intersection.  Must be a dead end.
            return;
        }
        else if(sensors[0] > 200 || sensors[4] > 200)
        {
            // Found an intersection.
            return;
        }
    }
}
