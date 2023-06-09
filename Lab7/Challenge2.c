/*
This program is challenge1.c from Lab3, except using a gyro.
This code is designed to avoid a set map that has obstacles in the way.
*/


//#pragma config(Sensor, S3,     touchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*------------------------------------------------------------------------------------------------
  This program will move your robot make a series of moves for different lengths of time.
  At the end of the program, all motors will shut down automatically and turn off.

                            ROBOT CONFIGURATION: LEGO EV3 REM Bot
    MOTORS & SENSORS:
    [I/O Port]          [Name]              [Type]                [Description]
    MotorC        	leftMotor           LEGO EV3 Motor	  Left side motor
    MotorB       	rightMotor          LEGO EV3 Motor	  Right side motor (reversed)
------------------------------------------------------------------------------------------------*/





void moveRobot(long nMotorRatio, long power)
{
	setMotorSyncEncoder(motorB, motorC, nMotorRatio, 1000, power);
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
	SensorType[S4] = sensorEV3_Ultrasonic;
	int n = 0;

	// Stop when turned 3 times
	while(n != 3)
	{
		int distance = getUSDistance(S4);

		if(getUSDistance(S4) > 50)
		{
			// If distance is high, move the robot
			moveRobot(0,100);
		}

		if(getUSDistance(S4) < 50)
		{
			// if distance is low, turn the robot to avoid collision
			turn90(90);
			// increment 
			n = n+1;
		}

	}

}