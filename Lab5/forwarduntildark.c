#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

void drive(long power)
{
      setMotorSpeed(rightMotor, power);
      setMotorSpeed(leftMotor, power);
}

task main()
{
      int light = SensorValue(S1);
      while(true)
      {
            while(SensorValue(S1) > 4)
            {
                  drive(50);
                  displayCenteredTextLine(5, "No black");
            }

            while(SensorValue(S1) < 4)
            {
                  drive(0);
                  displayCenteredTextLine(5, "Detected Black");
            }
      }

}forwarduntildark.c