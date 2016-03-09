// Put your includes up here
#include <pololu/3pi.h>
#include "follow_segment.h"
#include "initialize.h"
#include "turn.h"

char path[100] = "";
unsigned char path_length = 0; // the length of the path


// Displays the current path on the LCD, using two rows if necessary.
//  You can get more details if you'd like from this
void display_path()
{
    // Set the last character of the path to a 0 so that the print()
    // function can find the end of the string.  This is how strings
    // are normally terminated in C.
    path[path_length] = 0;

    clear();
    print(path);

    if(path_length > 8)
    {
        lcd_goto_xy(0,1);
        print(path+8);
    }
}

int first_main_loop()
{
    /* This function returns 1 when it finds "something interesting"
     * Otherwise, it just lets itself finish. It will be run until
     *  something "truthy" is returned
     */

    follow_segment();
     
    // Drive straight a bit.  This helps us in case we entered the
    // intersection at an angle.
    // Note that we are slowing down - this prevents the robot
    // from tipping forward too much.
    set_motors(50,50);
    delay_ms(50);
     
    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, while examining the current
    // intersection.
    unsigned char found_left=0;
    unsigned char found_straight=0;
    unsigned char found_right=0;
     
    // Now read the sensors and check the intersection type.
    unsigned int sensors[5];
        read_line(sensors,IR_EMITTERS_ON);
     
    // Check for left and right exits.
    if(sensors[0] > 100)
        found_left = 1;
    if(sensors[4] > 100)
        found_right = 1;
     
    // Drive straight a bit more - this is enough to line up our
    // wheels with the intersection.
    set_motors(40,40);
    delay_ms(200);
     
    // Check for a straight exit.
    read_line(sensors,IR_EMITTERS_ON);
    if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
        found_straight = 1;
     
    // Check for the ending spot.
    // If all three middle sensors are on dark black, we have
    // solved the maze.
    if(sensors[1] > 400 && sensors[2] > 400 && sensors[3] > 400)
        return 1;
     
    // Intersection identification is complete.
    // If the maze has been solved, we can follow the existing
    // path.  Otherwise, we need to learn the solution.
    //      from `turn.c`
    unsigned char dir = select_turn(found_left, found_straight, found_right);
     
    // Make the turn indicated by the path.
    //      from `turn.c`
    turn(dir);
     
    // Store the intersection in the path variable.
    if(path_length < 100){
        path[path_length] = dir;
        path_length ++;
    }
    else{
        //throw it away for now
    }
     
    // Display the path on the LCD.
    display_path();

    return 0;
}

void second_main_loop(int i)
{
    // SECOND MAIN LOOP BODY
    follow_segment();
     
    // Drive straight while slowing down, as before.
    set_motors(50,50);
    delay_ms(50);
    set_motors(40,40);
    delay_ms(200);
     
    // Make a turn according to the instruction stored in
    // path[i].
    turn(path[i]);
}

// This function is called once, from main.c.
void maze_solve()
{
    // FIRST MAIN LOOP BODY
    // (when we find the goal, we use break; to get out of this)
    while(!first_main_loop()) {}
    set_motors(0,0);
    
    // Simplify the learned path. You can implement this!
    //  You can either do this at the end, or during the main loop
    // delay(1000);
    // simplify_path();
    // Now enter an infinite loop - we can re-run the maze as many
    // times as we want to.
    while(1)
    {
        // Beep to show that we finished the maze.
        // Display battery voltage and wait for button press
        while(!button_is_pressed(BUTTON_B))
        {
            clear();
            print("Ready to");
            lcd_goto_xy(0,1);
            print("go again");

            delay_ms(100);
        }
        initialize();

        // Wait for the user to press a button...
        int i;
        for(i=0;i<path_length;i++)
        {
            // SECOND MAIN LOOP BODY
            second_main_loop(i);
        }
        // Follow the last segment up to the finish.
        follow_segment();
        // Now we should be at the finish! Restart the loop.
        set_motors(0,0);
    }
}

int main()
{
    // First get your robot to properly calibrate
    initialize();

    // Now solve the maze!
    maze_solve();
}
