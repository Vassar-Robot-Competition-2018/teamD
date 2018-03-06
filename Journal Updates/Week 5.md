This week leading into spring break was our final push to simplify the PID functionality of our robot. This iteration of our bot was the first in which we completely ignored the sweeping functionality of our servo camera in trade for a standard mount. We worked especially hard on refining the use of IR sensors on the frame, and indicating that we had found a block matching our starting quadrant for the 7th milestone.
Tuesday’s lab served as as time for us to finish installing the new lower chassis and rewiring the motors and receivers. We had some time to briefly test the bot with some new code, trying to eliminate the possibility of mistaking quadrant separation lines for blocks. Friday was a longer session that proved very fruitful in applying some of our new code. This time our priority for the objective was to identify a block corresponding to our home quadrant and seek out a matching block while ignoring blocks of any other color. Our first iteration did not do a good enough job of ignoring random block signals of the side IR sensors, so we commented them out entirely from the running code, and focusing on only forward facing information.

````

if (((irShortDist < CAPTURED) || (irPixyDist < 15)) && (maxSig != 6)) { //Block Captured
	   digitalWrite(WHITE, HIGH);    // LED
	   DriveStop();
	   delay (4000);
	 }

	 if (irShortDist > CAPTURED) {
	   digitalWrite(WHITE, LOW);    // LED
	   CheckBlocks();
	   //if (irPixyDist < 25) {
	    // IR_Pixy();
	    // if (irPixyDist < 25) {
	       //IR_Pixy();
	      // if (irPixyDist < 25) {
	         //CheckBlocks();
	      // }
	    // }
	   }
````
Making it to milestone 7 was a very nice jump into what will be our final iterations of our robot. Our next objective will be to successfully implement code to operate the newly 3D printed arms of the robot. Using them, along with implementing a function to act rather than stop in place pending an identification of a matching block will move us forward to possibly complete the remaining milestones in one go. Including a widened range of PID, reimplementing side IR sensors, and possibly adding more color sensors to detect blocks and line movement more completely serve as goals moving forward.
