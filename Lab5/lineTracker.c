#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
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

task main()
{
	while(true)
	{
		while(SensorValue(S1) > 4)
		{
			driveLeft1(50);
		}

		while(SensorValue(S1) < 4)
		{
			driveRight1(50);
		}
	}

}
