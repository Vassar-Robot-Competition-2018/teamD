Robotics Competition Last Week

This week has proved to be our final one for major modifications and learning steps.
We have passed all assigned milestones and, as it seems now, are incredibly close to being ready for the final competition. 

Tuesday started off with pouring over our existing working code modeled after 9.605 in Github.
Moving through selections, we could only successfully navigate through our logic in the first transitional loop time, which was around 30 seconds. 
This lead to a lot of confusion and repeated trial and error with new introductions into our logic matrix. 
We did utilize this time to successfully narrow and sharpen how our PID function was operating with newly introduced speed. 

Working separately and convening back together on Friday, we proceeded forwarded with two different iterations of the code.
Parker independently set out to solve the navigational problems between quadrants. 
The first round of navigational code taken for the first two thirds of the milestones was entirely sporadic in movement and just knew it's orientation within the quadrants. 

Our new tactic was, once a block had been found and captured, to navigate back towards it's designated quadrant. 
If the block matched home, then the robot would simply take the blocks back towards it's designated quadrant. 
If it matched the enemy quadrant, then I would be moved into a non-matching, non-home quadrant. 
All other blocks would move into the enemy quadrant. (Include code here). 


Up until Friday, this logic had not worked correctly in any real world tests that we had conducted. 
Only once Parker brilliantly replaced while-loops for for-loops did we see a stark difference in the movement of the robot. 
The variables within this code section (included below) are not the same as the ones originally assigned in our first iteration of code, but they still serve the same purpose wonderfully. 

We then recorded and ran a successful competition style round for obtaining points and passed the final milestones, Putting us in good positioning going into the real competition. Monday morning, Jonathan brought along a new forward plate mount for the robot making this Version 5.1. This newest edition (pictured in INSERT PHOTO NAME) has more forward bulbous mounts that increase our radius of line recognition. It also means that blocks are less likely to accidentally caught on the forward section of the robot. 

