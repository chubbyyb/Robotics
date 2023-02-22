#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

void driveLeft1(long power)
{
	setMotorSpeed(rightMotor, power);
	setMotorSpeed(leftMotor, power / 2);
}

void driveRight1(long power)
{
	setMotorSpeed(rightMotor, power / 2);
	setMotorSpeed(leftMotor, power);
}

void drive(long power)
{
	setMotorSpeed(rightMotor, power);
	setMotorSpeed(leftMotor, power);
}

void turn90degreesRight()
{
	// snap right
	setMotorSpeed(leftMotor, 30);
	setMotorSpeed(rightMotor, -30);
	sleep(650);
}

void turn90degreesLeft()
{
	// snap left
	setMotorSpeed(leftMotor, -30);
	setMotorSpeed(rightMotor, 30);
	sleep(650);
}

void forward1S()
{
	// go straight
	setMotorSpeed(leftMotor, 50);
	setMotorSpeed(rightMotor, 50);
	sleep(1000);
}

void avoidObstacle()
{
	setMotorSpeed(rightMotor, 0);
	setMotorSpeed(leftMotor, 0);
	sleep(500);
	turn90degreesRight();
	setMotorSpeed(rightMotor, 0);
	setMotorSpeed(leftMotor, 0);
	forward1S();
	setMotorSpeed(rightMotor, 0);
	setMotorSpeed(leftMotor, 0);
	turn90degreesLeft();
	setMotorSpeed(rightMotor, 0);
	setMotorSpeed(leftMotor, 0);
	forward1S();
}


task main()
{
	bool noObstacle = true;

	while(true)
	{
		// 25
		if(SensorValue(S3) < 20)
		{
			avoidObstacle();
		}

		while(SensorValue(S1) > 4 & noObstacle)
		{
			driveLeft1(50);
		}

		while(SensorValue(S1) < 4 & noObstacle)
		{
			driveRight1(50);
		}

	}

}