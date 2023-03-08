#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)




task main()
{

	int gyroRate = 0;
	SensorType[S2] = sensorEV3_Gyro;
	displayBigTextLine(1,"Lab Demo GyroThrottle");   // Useful command but not required
	int Angle;
	
	sleep(1000);
	resetGyro(S2);
	sleep(2000);


	// The code below will turn the motor proportionally to the gyro rate
	while (true)
	{
	
		// Turn on the motor until gyro is 90 degrees
		setMotorSpeed(motorA, -5);
		while (abs(getGyroHeading(S2))<50)
		{
			sleep(1);
			Angle=getGyroHeading(S2);
			displayCenteredBigTextLine(5,"current angle %4d",Angle);
		}
		
		setMotorSpeed(motorA, 0); // stop the motors
		
		// Play tone when 90 degree turn achieved 
		playTone(440,10);
		// Read the current rate value in degrees/second
		gyroRate = getGyroHeading(S2);

		displayBigTextLine(2,"Heading= %d",gyroRate);

		// Clip the value of the gyro to -100 to +100 (min and max motor speeds)
		if (gyroRate > 100)
			gyroRate = 100;
		else if (gyroRate < -100)
			gyroRate = -100;

		// Set motor speeds to the rate of the gyro
		setMotorSync(motorB, motorC, 0, gyroRate);

		sleep(10);
	}
}