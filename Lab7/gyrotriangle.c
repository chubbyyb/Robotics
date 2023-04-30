/*
This program drives in a square/triangle, using a gyro, given a distance for the perimeter.
The getDistance function is used to get the perimeter length of travel,
square is used to drive in a square,
triangle is used to drive in a triangle,
turn90 is used to turn the robot given an angle.
*/

#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)



int getDistance() {

	// Initialize distance variable 10 20 25 40
	int distance = 0;


	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed.
	{

		displayCenteredBigTextLine(4,"Distance = %dcm", distance);


		// GET DISTANCES
		if (getButtonPress(buttonUp))
		{
			distance = 10;
			displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		}
		else if (getButtonPress(buttonDown))
		{
			distance = 20;
			displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		}
		else if (getButtonPress(buttonLeft))
		{
			distance = 25;
			displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		}
		else if (getButtonPress(buttonRight))
		{
			distance = 40;
			displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		}

	}

	// Wait 20 ms, this gives us 50 readings per second
	sleep(20);
	return distance;

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


void drive(long dist, float degrees)
{
	setMotorSyncEncoder(motorB, motorC, 0, dist*36, 100);
	wait1Msec(dist * 100);
	turn90(degrees);
}

void square(int distance)
{
	int n = 0;
	
	// 4 Times for a square
	while(n < 4)
	{
		drive(distance, 90);
		n=n+1;
	}
}

void triangle(int distance)
{
	int n = 0;
	
	// 3 Times for triangle
	while(n < 3)
	{
			// 180 - 60 = 120
			drive(distance, 120);
			n=n+1;
	}
}


task main()
{
	int distance = getDistance();
	sleep(100);
	
	//square(distance);
	triangle(distance);
	
	sleep(1000);


}