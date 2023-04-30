/*
This code allows you to travel to a certain distance at a random speed
 */

#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)



int getDistance(int distance) {
	
	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed. 
	{
		
		displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		
		if (getButtonPress(buttonUp)) {
			distance = distance+1; // Increment distance
			displayCenteredBigTextLine(4,"Distance = %dcm", distance); // Display distance
		}
		else if (getButtonPress(buttonDown)) 
		{
			
			if(distance != 0) // Error checking 
			{
				distance = distance-1; // Decrement distance
				displayCenteredBigTextLine(4,"Distance = %dcm", distance);
			}
		}
		
		// Wait 20 ms, this gives us 50 readings per second
		sleep(20);
	}
	return distance;
	
}

void drive(long nMotorRatio, long dist, long power)
{
	setMotorSyncEncoder(motorB, motorC, nMotorRatio, dist, power);
}


task main()
{
	int distance = 0;
	int randomNum = random(100); // Generate a random number
	
	displayCenteredTextLine(1, "Pressed button:");
	displayCenteredBigTextLine(6,"Final Dist = %dcm", distance = getDistance(distance));
	sleep(1000);
	drive(0,distance * 36 ,randomNum);
	sleep(10000);
}