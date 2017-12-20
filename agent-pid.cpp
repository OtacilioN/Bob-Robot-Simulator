#include <stdio.h>
#include "bobuniverse.h"

#define INITIALDISTANCE 60
#define SETPOINT 30
#define KP 6
#define KI 0.03
#define KD 0.7
#define AUTOMATICSTOP 600

int main(void)
{
    Universe myBob(INITIALDISTANCE, SETPOINT);
    int distance, time, status, power, sum, error, delta = 0, speed, lasterror = 0, counter = 0;
    
    status = myBob.getStatus();
    while(status == 1 && counter < AUTOMATICSTOP)
    {
        distance = myBob.getDistance();
        error = distance - SETPOINT;
        sum = sum + error;

        if(error == 0 || (error >= 0 && lasterror <= 0) || (error <= 0 && lasterror >= 0))
        {
            sum = 0;
        }

        /*          P           I           D     */
        power = (error*KP) + (sum*KI) + (delta*KD);
        myBob.move(power);
        status = myBob.getStatus();
        counter++;
        printf("distance: %d error: %d sum: %d delta: %d power: %d \n", distance, error, sum, delta, power);
        delta = error - lasterror;
        lasterror = error;
    }
    status = myBob.getStatus();
    distance = myBob.getDistance();
    speed = myBob.getSpeed();
    time = myBob.getTime();
    printf("finished with distance %d speed %d time: %d status: %d\n", distance, speed, time, status);
    return 0;
}
