/*
This code travels a certain distance at a random speed using the QEV3 Simulator
*/


int getDistance(int distance) {
	
	// Loop until button pressed
	while (getButtonPress(buttonEnter) == 0 )  // this will fail once the button is enter pressed. 
	{
		
		displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		
		if (getButtonPress(buttonUp)) {
			distance = distance+1;
			displayCenteredBigTextLine(4,"Distance = %dcm", distance);
		}
		else if (getButtonPress(buttonDown)) 
		{
			
			if(distance != 0)
			{
				distance = distance-1;
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
	sleep(10000);
}


task main()
{
	
	int distance = 0;
	int randomNum = random(100);
	
	displayCenteredTextLine(1, "Pressed button:");
	displayCenteredBigTextLine(6,"Final Dist = %dcm", distance = getDistance(distance));
	sleep(1000);
	drive(0,distance * 10,randomNum);
}