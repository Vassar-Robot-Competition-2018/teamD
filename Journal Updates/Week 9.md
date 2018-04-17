# teamD

Our robot tracks and captures blocks well now, so this week has been focused on writing the logic to prioritize the robot’s actions (e.g. what blocks the robot should look for in each quadrant and where it should relocate those blocks). Most importantly, we’re working on getting the robot to navigate the arena more efficiently so that it can find quadrants quickly rather than bouncing off the white boundary until it happens upon the desired quadrant. We have written code that makes the robot align itself (more or less) perpendicular to the quadrant lines whenever it enters a new quadrant. Once aligned, this allows the robot to make 90 degree turns to find whatever quadrant it needs.

```
if (quadL == quadR) {  // left color sensor matches right color sensor
    quadrant = quadL;
  }

  else if (quadL != quadrant) {  // left color sensor reads a new quadrant color
    UpdateWheels(-highSpeed, highSpeed);  // turn left until the sensors match
  }

  else if (quadR != quadrant) {  // right color sensor reads a new quadrant color
    UpdateWheels(highSpeed, -highSpeed);  // turn right until the sensors match
  }
```
We picked up a magnetometer/accelerometer board, which may be useful in providing the robot with directional information. If it works out, this will simplify our code a lot and basically allow the robot to drive directly to any quadrant.

We also mounted a time-of-flight sensor under the pixy cam, which can be used to help differentiate between quadrant boundaries and blocks, and to verify when a block is captured.

We’re using timers in most functions now so that the functions only run as often as needed and at consistent time intervals. This makes the code run more quickly and efficiently, and makes the PID functions run more smoothly. Here is an example of a function that uses a timer to ensure that the function runs every 20ms:

```
void Scan() {  // servo sweeping motion for block searching  
  current = millis();

  if ((current - lastScanUpdate) >= scanInterval)   // scanInterval = 20ms
  {
    lastScanUpdate = millis();
    servoPos += increment;
    UpdatePan(servoPos);

    if ((servoPos >= servoMax) || (servoPos <= servoMin))   // end of sweep
    {
      increment = -increment;   // reverse direction
    }
  }
}
```
