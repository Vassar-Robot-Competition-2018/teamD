This week our group continued to work on our code in order to complete milestone 9 and perfect our PID code. We have all the components to complete the upcoming milestone, but are holding back on testing as we would We have included the sweeping function into our format for tracking, and it has greatly improved our recognition response time for identifying blocks matching home quadrant color.
````
void UpdatePan(double pos) {




 double pan_pulse = map(pos, -100, 100, PAN_MIN, PAN_MAX); // straight


 pwm.setPWM(PAN_SERVO, 0, pan_pulse);


 // delay(20);
````




````
We are currently troubleshooting our return home function as we are currently using repetitive movement to bounce back lack a pool ball into our correct quadrant which is somewhat time consuming and inefficient. 
void SetHome(int r, int g, int b) { // sets home quadrant




 if ((homeQuad == 5) && (quadrant != 0)) {


   homeQuad = quadrant;


 }


}


void Quadrant(int r, int g, int b) {


 if (r > (g + b))  //red


   quadrant = 1;


 else if ((r + g) > (4 * b))  //yellow


   quadrant = 2;



 else if ((3 * g) > ((r + b) * 2))  //green


   quadrant = 3;


 else if (b > (2 * r))  //blue


   quadrant = 4;


 SetHome(r, g, b);



 if (quadrant != tempQuad) {


   prevQuad = tempQuad;


   tempQuad = quadrant;


 }


 //  Serial.print("Home Quadrant: ");


 //  Serial.print(homeQuad);


 //  Serial.print("Quadrant color: ");


 //  Serial.println(quadrant);


 QuadrantLED();
 
 HomeLED();


}
````





Our goals are to improve the home quadrant function to not only work better, but to find the block quickly and go back home. Finding that is our main goal this week, and looking forward we hope to get some prioritization. We also have each submitted new ideas for designs for the chassis, and are debating in either growing or shrinking the form factor of the current bot setup. We might implement a re-orientation function that squares up the bot as it approaches any given new quadrant.  
