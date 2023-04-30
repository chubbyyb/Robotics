/*
QEV3 Bot Sim. 
Navigates X,Y coordinate plane.
Using a gyro and a color sensor.
Displays current coordiantes on screen.

This program begins and calls getCords: This will allow you to input your starting position
Then it will call getCords again: This will allow you to input your target position
*/

// Turned 1 == Right
// Turned 2 == Down
// Turned 3 == Left
// Turned 4 == Up

// This is locationfinder.c with the following changes:
// * getCords function will get the starting position, and the target position
// * Additional check to ensure if the robot is currently on the target position, kill all process
// * Object Detection & Avoidance (pain)
// * Extra error checks

// * Revamped the coordinate traversal system, OLD: get to X pos, then Y pos
//  NEW: Go to x first, if cant reach, go to y, then to x, then to y, then to x, no idea how it works anymore, thinking about it sends me down a spiral, but it works, im not touching it
//  Go to x, wall in the way, go to Y, wall in the way, go to x, (and make sure you go back to y when reached x). I hope this was a sufficient explanation of the spaghetti below.



int getCords(int target, char axis[]) {
	
	
	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed.
	{
		// GET DISTANCES
		if (getButtonPress(buttonUp))
		{
			target = target+1;
			displayCenteredBigTextLine(4,"%s:%d",axis, target);
		}
		else if (getButtonPress(buttonDown))
		{
			if(target != 0){	target = target-1;}
			displayCenteredBigTextLine(4,"%s:%d",axis, target);
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
	SensorType[S1] = sensorEV3_Ultrasonic;
	int n = 0;
	bool onLine = false;
	float threshold = 30;
	
	// Input current position
	int x = 6;
	int y = 1;
	int turned = 1; // 1 = right, 2 = down, 3 = left, 4 = up
	
	
	bool atTargetX = false;
	bool atTargetY = false;
	int targetX = 1;
	int targetY = 7;
	
	// Get current position
	x = getCords(x,"Start X"); 
	sleep(100); // Wait 100 ms prevent enter button from being pressed twice
	y = getCords(y, "Start Y"); 
	sleep(100);
	
	// Get target position
	targetX = getCords(targetX, "Target X");
	sleep(100);
	targetY = getCords(targetY, "Target Y");
	
	// if x is behind the robot
	if(x > targetX)
	{
		turn90(90+90);
		turned = 3;
	}
	else if(y > targetY) // if robot is higher than target
	{
		turn90(90+90+90);
		turned = 2;
	}
	else if(y < targetY) // if robot is lower than target
	{
		turn90(90);
		turned = 4;
	}
	
	// check if already on coordinates
	if(x == targetX && y == targetY)
	{
		stopAllTasks();
	}
	if(x == targetX)
	{
		atTargetX = true;
	}
	if(y == targetY)
	{
		atTargetY = true;
	}
	
	
	// Target Y is always reached second, so if targetY is not reached, keep the code running
	while(atTargetY==false || atTargetX==false)
	{
		displayCenteredTextLine(8,"X: %d",atTargetX);
		displayCenteredTextLine(9,"Y:%d",atTargetY);
		




        // OBSTACLE DETECTION
		if(SensorValue[S1] < 12 && turned == 1) // Turned right
		{
			drive(0); // Stop the robot
			
			if(y > targetY)
			{
				displayCenteredTextLine(5, "Objected Detected: v");
				turn90(90); // Turn down
				turned = 2;
			}
			else
			{
				displayCenteredTextLine(5, "Objected Detected: ^");
				turn90(90+90+90); // Turn up
				turned = 4;
			}
		}
		else if(SensorValue[S1] < 12 && turned == 3) // Turned left
		{
			drive(0); // Stop the robot
			
			if(y > targetY)
			{
				displayCenteredTextLine(5, "Objected Detected: ^");
				turn90(90); // Turn up
				turned = 2;
			}
			else
			{
				displayCenteredTextLine(5, "Objected Detected: v");
				turn90(90+90+90); // Turn down
				turned = 4;
			}
		}
		else if(SensorValue[S1] < 12 && turned == 2) // Turned down
		{
			drive(0); // Stop the robot
			
			if(x > targetX)
			{
				displayCenteredTextLine(5, "Objected Detected: <");
				turn90(90+90+90); // Turn left
				turned = 3;
			}
			else
			{
				displayCenteredTextLine(5, "Objected Detected: >");
				turn90(90); // Turn right
				turned = 1;
			}
		}
		else if(SensorValue[S1] < 12 && turned == 4) // Turned up
		{
			drive(0); // Stop the robot
			
			if(x > targetX)
			{
				displayCenteredTextLine(5, "Objected Detected: <");
				turn90(90); // Turn left
				turned = 3;
			}
			else
			{
				displayCenteredTextLine(5, "Objected Detected: >");
				turn90(90+90+90); // Turn right
				turned = 1;
			}
		}
        // OBSTACLE DETECTION END



		// Error checking so robot does not wonder off in the wrong direction
		if(atTargetX == false && turned == 1 && x > targetX)
		{
			turn90(90); // Turn down
			turned = 4;
		}
		else if(atTargetY == false && turned == 4 && y > targetY)
		{
			turn90(90); // Turn left
			turned = 3;
		}
		
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
			else if(y == targetY && x > targetX && atTargetY == false)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				
				// Stop the robot
				drive(0);
				
				// Turn [LEFT]
				if(turned == 2)
				{
					turn90(90+90+90);
				}else{	turn90(90);}
				
				// Keep track of facing position
				turned = 3;
				
				// We have reached X
				atTargetY = true;
			}
			else if(y == targetY && x < targetX && atTargetY == false)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				
				// Stop the robot
				drive(0);
				
				// Turn [RIGHT]
				if(turned == 2)
				{
					turn90(90);
				}else{	turn90(90+90+90);}
				
				// Keep track of facing position
				turned = 1;
				
				// We have reached X
				atTargetY = true;
			}
			
			
			// Position Y reached
			if(y == targetY && atTargetY == false)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				
				// We have reached position Y
				atTargetY = true;
			}
			// Position X reached
			if(x == targetX && atTargetX == false)
			{
				// Buffer in order to get onto the right spot
				sleep(2000);
				
				// We have reached position Y
				atTargetX = true;
			}
			
			if(x != targetX)
			{
				atTargetX = false;
			}
			if(y != targetY)
			{
				atTargetY = false;
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