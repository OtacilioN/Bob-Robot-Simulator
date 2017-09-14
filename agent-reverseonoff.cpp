#include <stdio.h>
#include "bobuniverse.h"

#define INITIALDISTANCE 50
#define SETPOINT 30
#define AUTOMATICSTOP 100

int main(void){
    int distance, status, counter = 0; 
    Universe myBob(INITIALDISTANCE, SETPOINT);
    
    distance = myBob.getDistance();
    status = myBob.getStatus();
    
    while(status == 1 && counter < AUTOMATICSTOP){
        printf("The distance: %d\n", distance);
        
        if(distance > SETPOINT){
            printf("Moving with 100 of power\n");
            myBob.move(1000);
        }
        else if(distance == SETPOINT){
            printf("Applying brake\n");
            myBob.brake();
        }
        else{
            printf("Moving with -100 of power\n");
            myBob.move(-1000);
        }
        distance = myBob.getDistance();
        status = myBob.getStatus();
        counter++;
    }
    printf("Final distance: %d\n", distance);
    printf("Final status: %d\n", status);
    
    return 0;
}