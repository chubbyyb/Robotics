/*
This code generates a threshold based on two values. You put the light sensor on the light value, and the dark value.
It will average them out and return the threshold
*/

#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)




float generateThreshold()
{
  int n = 0;
  float threshold = 0;
  int countLight = 0; // sum of values

  while(n < 2)
  {
    if(getButtonPress(buttonEnter) == 1)
    {
      sleep(1000);
      displayCenteredBigTextLine(n*2,"Light = %d", SensorValue(S1));
      countLight = countLight+SensorValue(S1); // add the value
      n=n+1;
    }
  }

  threshold = countLight/n; // average it 
  displayCenteredTextLine(5, "Average = %f",threshold);
  sleep(2000);
  return threshold; // return it

}


task main()
{

  float threshold = generateThreshold();
  while(true){ displayCenteredTextLine(10, "%f",threshold);}

      //while(true)
      //{
      //    if(getButtonPress(buttonEnter) == 1)
      //    {
      //    int light = SensorValue(S1);
      //    displayCenteredBigTextLine(4,"Light = %d", light);
      //    }
      //}

}