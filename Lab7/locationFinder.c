/*
QEV3 Bot Sim. 
Navigates X,Y coordinate plane
*/

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
    int n = 0;
    bool onLine = false;
    float threshold = 30;
    int x = 1;
    int y = 4;
    int turned = 1;
    
    
    bool atTargetX = false;
    bool atTargetY = false;
    int targetX = 5;
    int targetY = 6;
      
    // Target Y is always reached second, so if targetY is not reached, keep the code running
    while(atTargetY==false)
    {
        displayCenteredTextLine(1, "Turned: %d",turned);

        // While the sensor does not detect a line [going over the plot], it will resume as normal
        while(SensorValue[S3] > threshold)
        {
            onLine = false;
            drive(30);
            displayCenteredTextLine(10, "No black");
            displayCenteredTextLine(13, "%d",SensorValue[S3]);
            displayCenteredTextLine(15, "%d",SensorValue[S2]);

            // Sleep program to prevent too much readings
            sleep(10);
        }
            
        // While sensor detects a line
        while(SensorValue[S3] < threshold)
        {
            displayCenteredTextLine(14, "%d",SensorValue[S3]);
            drive(0);
            displayCenteredTextLine(10, "Detected Black");
            drive(10);
            
            // If current X position is the target, and if the x position has not previously been reached 
            // and if current Y position is less than the target, turn up
            if(x == targetX && y < targetY && atTargetX == false)
            {
                // Stop the robot
                drive(0);
                // Turn 270 degrees
                turn90(90+90+90);
                // Keep track of facing position
                turned = turned-1;
                atTargetX = true;
            } // If the Y position is more than the current Y position, turn down instead
            else if(x == targetX && y > targetY && atTargetX == false)
            {
                drive(0);
                turn90(90);
                turned = turned+1;
                atTargetX = true;
            }
            
            // Position Y reached [Terminates loop]
            if(y == targetY)
            {
                atTargetY = true;
            }
            
            // Keep track of current X position
            if(SensorValue[S3] < threshold && onLine == false && (turned == 1 || turned == -1))
            {
                onLine = true;
                n=n+1;
                displayCenteredBigTextLine(8,"Lines = %d", n);
                if(turned == 1)
                {
                    x = x+1;
                }
                else{ x = x-1;}
                displayCenteredTextLine(2, "X:%d Y:%d",x,y);
            } // Keep track of current Y position
            else if(SensorValue[S3] < threshold && onLine == false && (turned == 0 || turned == 2))
            {
                onLine = true;
                n=n+1;
                displayCenteredBigTextLine(8,"Lines = %d", n);
                if(turned == 0)
                {
                    y = y+1;
                }
                else{ y = y-1;}
                displayCenteredTextLine(2, "X:%d Y:%d",x,y);
            }
            sleep(10);
        }
    }
      
}
