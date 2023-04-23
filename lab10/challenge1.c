/*
QEV3 Bot Sim. 
Navigates X,Y coordinate plane
*/

// Turned 1 == Right
// Turned 2 == Down
// Turned 3 == Left
// Turned 4 == Up

// This is just locationfinder.c with one additional check to see if the robot is at the target location
// Oh yeah also I added a function to get the x and y coordinates of the target location

int getCords(int target, char axis) {
	
	
	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed.
	{
		// GET DISTANCES
		if (getButtonPress(buttonUp))
		{
			target = target+1;
			displayCenteredBigTextLine(4,"%c:%d",axis, target);
		}
		else if (getButtonPress(buttonDown))
		{
			if(target != 0){	target = target-1;}
			displayCenteredBigTextLine(4,"%c:%d",axis, target);
		}
		sleep(100);
	}
	
	// Wait 20 ms, this gives us 50 readings per second
	sleep(100);
	eraseDisplay();
	
	return target;
	
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
	int y = 1;
	int turned = 1; // 1 = right, 2 = down, 3 = left, 4 = up
	
	
	bool atTargetX = false;
	bool atTargetY = false;
	int targetX = 5;
	int targetY = 5;
	x = getCords(x,'X'); // Get target X
	sleep(100); // Wait 100 ms prevent enter button from being pressed twice
	y = getCords(y, 'Y'); // Get target Y
	
	
	if(x > targetX)
	{
		turn90(90+90);
		turned = 3;
	}
	
	// check if already on coordinates
	if(x == targetX && y == targetY)
	{
		stopAllTasks();
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