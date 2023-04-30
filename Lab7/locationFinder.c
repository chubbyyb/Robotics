/*
QEV3 Bot Sim. 
Navigates X,Y coordinate plane.
Using a gyro and a color sensor.
Displays current coordiantes on screen.

This program begins and will ask you for coordinates, either (4,1) or (5,6),
When the coordinates are inputted, your robot will travel to them.

The robot should be starting in (1,4)
*/

// Turned 1 == Right
// Turned 2 == Down
// Turned 3 == Left
// Turned 4 == Up

int getCords() {
	
	int cords = 0;
	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed.
	{
		// GET DISTANCES
		if (getButtonPress(buttonUp))
		{
			displayCenteredBigTextLine(4,"Coordinates (4,1)");
			cords = 0;
		}
		else if (getButtonPress(buttonDown))
		{
			displayCenteredBigTextLine(4,"Coordinates (5,6)");
			cords = 1;
		}
		sleep(20);
	}
	
	// Wait 20 ms, this gives us 50 readings per second
	sleep(20);
	return cords;
	
}


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
	int x = 1;
	int y = 4;
	int turned = 1;
	
	
	bool atTargetX = false;
	bool atTargetY = false;
	int targetX = 0;
	int targetY = 0;
	
	if(getCords() == 1)
	{
		targetX = 5;
		targetY = 6;
	}else
	{	
		
		targetX = 4;
		targetY = 1;	
	}
	
	if(x > targetX) // Error checking for future updates.
	{
		turn90(90+90);
		turned = 3;
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
				sleep(2000);

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

				// We have reached position Y
				atTargetY = true;
			}
			
			
			
			
			// Keep track of current X position
			if(SensorValue[S3] < threshold && onLine == false && (turned == 1 || turned == 3))
			{
				onLine = true;
				n=n+1;
				//displayCenteredBigTextLine(8,"Lines = %d", n);
				if(turned == 1)
				{
					x = x+1;
				}
				else{	x = x-1;}
				displayCenteredTextLine(2, "X:%d Y:%d",x,y);
			} // Keep track of current Y position
			else if(SensorValue[S3] < threshold && onLine == false && (turned == 2 || turned == 4))
			{
				onLine = true;
				n=n+1;
				//displayCenteredBigTextLine(8,"Lines = %d", n);
				if(turned == 2)
				{
					y = y-1;
				}
				else{	y = y+1;}
				displayCenteredTextLine(2, "X:%d Y:%d",x,y);
			}
			sleep(10);
		}
	}
	
}