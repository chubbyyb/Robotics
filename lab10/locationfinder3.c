/*
QEV3 Bot Sim. 
Navigates X,Y coordinate plane.
Using a gyro and a color sensor.
Displays current coordiantes on screen.

This program begins on position (3,1)
The robot will travel to (3,7)
The robot will stop for 5 seconds while beeping
The robot will then travel to position (1,1)

The robot will beep on every enterance of a new coordinate position
*/

// Turned 1 == Right
// Turned 2 == Down
// Turned 3 == Left
// Turned 4 == Up

// This is locationfinder.c with the following changes:
// * getCords function replaced with set coordinates
// * Robot will beep on enterance of every new square
// * Robot will always end on position (1,1)
// * Robot will stop and beep when the first position is reached



void drive(long power)
{
	// Push motors forward
	setMotorSpeed(motorC, power);
	setMotorSpeed(motorB, power);
}


void turn90(float degrees)
{
	// Initialize angle and reset gyri
	int Angle;
	wait1Msec(1000);
	resetGyro(S2);
	wait1Msec(1000);
	
	// Turn on the motor until gyro is 90 degrees
	setMotorSync(motorB, motorC, -100, 10);
	while (abs(getGyroDegrees(S2))<degrees)
	{
		sleep(1);
		Angle=getGyroDegrees(S2);
		displayCenteredBigTextLine(5,"current angle %4d",Angle);
	}
	
	// Play tone when 90 degree turn achieved
	playTone(440,10);
	setMotorSync(motorB, motorC, 0, 0); // stop the motors
	wait1Msec(1000); // Allow sensor to settle
}



task main()
{
	// Initilization
	SensorType[S3] = sensorEV3_Color;
	SensorType[S2] = sensorEV3_Gyro;
	int n = 0;
	bool onLine = false;
	float threshold = 30;
	
	// Input current position
	int x = 3;
	int y = 1;
	int turned = 1;
	
	
	bool atTargetX = false;
	bool atTargetY = false;
	int targetX = 3;
	int targetY = 7;
	
	
	
	if(x > targetX) // If the current position is greater than the target positon, flip the robot
	{
		turn90(90+90);
		turned = 3; // adjust facing position
	}
	
	// Target Y is always reached second, so if targetY is not reached, keep the code running
	while(atTargetY==false || atTargetX==false)
	{
		//displayCenteredTextLine(1, "Turned: %d",turned);
		// While the sensor does not detect a line [going over the plot], it will resume as normal
		while(SensorValue[S3] > threshold)
		{
			onLine = false;
			drive(30);
			
			// Sleep program to prevent too much readings
			sleep(10);
		}
		
		// While sensor detects a line
		while(SensorValue[S3] < threshold)
		{
			// Slow down the vehicle
			drive(0);
			drive(10);
			
			// If current X position is the target, and if the x position has not previously been reached 
			// and if current Y position is less than the target, turn up
			if(x == targetX && y < targetY && atTargetX == false)
			{
				// Buffer in order to get onto the right spot
				//sleep(2000);
				
				// Stop the robot
				drive(0);
				
				// Turn [UP]
				if(turned == 1)
				{
					turn90(90);
				}else{	turn90(90+90+90);}
				
				// Keep track of facing position
				turned = 4;
				
				// We have reached X
				atTargetX = true;
			} // If the Y position is more than the current Y position, turn down instead
			else if(x == targetX && y > targetY && atTargetX == false)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				
				// Stop the robot
				drive(0);
				
				// Turn [DOWN]
				if(turned == 1)
				{
					turn90(90+90+90);
				}else{	turn90(90);}
				
				// Keep track of facing position
				turned = 2;
				
				// We have reached X
				atTargetX = true;
			}
			
			
			// Position Y reached [Terminates loop]
			if(y == targetY)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				drive(0);		

                // Fettuccine Pasta code
				if(y!=1)
				{
                    // Stop for 5 seconds and beep
                    // I didnt test this lol im tired
                    sleep(1000);
                    playTone(440,10);
                    sleep(1000);
                    playTone(440,10);
                    sleep(1000);
                    playTone(440,10);
                    sleep(1000);
                    playTone(440,10);
                    sleep(1000);
                    playTone(440,10);
                    // end of beep
                    
                    // Return to 1,1
					targetY = 1;
					targetX = 1;
					atTargetX = false;
					turn90(90);
					turned = 3;
				}
				else{	atTargetY = true;}
			}
			
			
			
			
			// Keep track of current X position
			if(SensorValue[S3] < threshold && onLine == false && (turned == 1 || turned == 3))
			{
				onLine = true;
				n=n+1;
				//displayCenteredBigTextLine(8,"Lines = %d", n);
				if(turned == 1)
				{
					x = x+1; // Increment positon
					playTone(440,10); // Play tone on entry of new square
				}
				else{	x = x-1; playTone(440,10);} // Decrement positon, Play tone on entry of new square
				displayCenteredTextLine(2, "X:%d Y:%d",x,y);
			} // Keep track of current Y position
			else if(SensorValue[S3] < threshold && onLine == false && (turned == 2 || turned == 4))
			{
				onLine = true;
				n=n+1;
				//displayCenteredBigTextLine(8,"Lines = %d", n);
				if(turned == 2)
				{
					y = y-1; // Decrement positon
					playTone(440,10); // Play tone on entry of new square
				}
				else{	y = y+1; playTone(440,10);} // Increment positon, Play tone on entry of new square
				displayCenteredTextLine(2, "X:%d Y:%d",x,y);
			}
			sleep(10);
			
		}
		
	}
	
}