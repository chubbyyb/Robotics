void turn90(long nMotorRatio, long power)
{
	setMotorSyncEncoder(motorB, motorC, nMotorRatio, 1000, power);
	sleep(1400);
}

void goForwards()
{
	setMotorSyncEncoder(motorB, motorC, 0, 100, 100);
}

task main()
{
	// Configure the ultrasonic sensor
	SensorType[S2] = sensorEV3_Ultrasonic;
	int n = 0;
	int previousDistance = 100;
	
	while (n < 3)
	{
		goForwards();
		// Read the distance to the nearest object in front of the sensor
		int distance = getUSDistance(S2);
		
		
		// Check if the distance is less than a threshold value
		if (distance < 25) // Replace 30 with the distance at which you want the robot to detect the wall
		{
			turn90(25,50);
			n=n+1;
		}
		
		
		
		// Wait a short time before checking the sensor again
		wait1Msec(100);
		
	}
}