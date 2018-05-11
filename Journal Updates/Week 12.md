Robotics Competition Last Week

This week has proved to be our final one for major modifications and learning steps.
We have passed all assigned milestones and, as it seems now, are incredibly close to being ready for the final competition.

Tuesday started off with pouring over our existing working code modeled after 9.605 in Github.
Moving through selections, we could only successfully navigate through our logic in the first 30 seconds of each run.
This lead to a lot of confusion and repeated trial and error with new introductions into our logic matrix.
We did utilize this time to successfully narrow and sharpen how our PID function was operating with newly introduced speed.

Each trial would break down after 30 seconds while trying to navigate captured blocks to the desired quadrants. The first block was always successful, but then the robot appeared to get stuck in some loop and stop responding to its environment. This part of the code was using 'while' loops to perform turns for a designated amount of time while still checking the floor sensors for white boundaries and quadrant changes. Parker had the idea of trying 'for' loops instead, although it meant using iterations instead of seconds to measure out the turns. This fixed the 30-second problem, and it wasn't until later that night that Jonathan realized that the 'while' loops weren't working past 30 seconds because the variable assigned to keep track of time was an integer when it should have been an unsigned long. After roughly 32000 milliseconds the integer variable gets an overflow error and becomes negative. Unsigned long variables do not have this problem, and they're what we were already using to keep track of time for our PID functions.  

Unfortunately, we went through many different codes trying to fix the logic that gets the robot to deposit blocks in the correct quadrants, but the problem was never with the logic; it was just a variable assigned to the wrong datatype.

We then recorded and ran a successful competition style round for obtaining points and passed the final milestones, Putting us in good positioning going into the real competition. Monday morning, Jonathan brought in a new forward plate mount for the robot making this Version 5.1. This newest edition puts the color sensors further forward so that white lines can be detected without the robot dropping blocks out of bounds.
