#include <stdio.h>
#include "bobuniverse.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define INITIALDISTANCE 60
#define SETPOINT 30
#define AUTOMATICSTOP 1000
#define GENERATIONS 5
#define INDIVIDUALS 40
#define CROMO 4
#define GENES 4
#define KEEP (INDIVIDUALS/8)
#define FLOATADJUST 10000000.0
#define PERCENTADJUST (FLOATADJUST/10)
#define MAXVALUE (10*FLOATADJUST)
#define KIADJUST 10.0
#define KDADJUST 10.0

/* Cromo dictionary */
#define BASEVALUE 0
#define KP 1
#define KI 2
#define KD 3

#define DEBUGLEVEL -1

void generateGens(int bobsGens[INDIVIDUALS][CROMO][GENES]);
void getHealth(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS]);
int getPower(int bobsGens[INDIVIDUALS][CROMO][GENES], int individual, int error, int sum, int delta);

std::vector<Universe> myBob(INDIVIDUALS, Universe(INITIALDISTANCE, SETPOINT));

int main(void)
{
    int bobsGens[INDIVIDUALS][CROMO][GENES], health[INDIVIDUALS];
    int generation;
    srand(time(NULL));
    generateGens(bobsGens);

    for(generation = 0; generation < GENERATIONS; generation++)
    {
        getHealth(bobsGens, health);
    }

    return 0;
}

void generateGens(int bobsGens[INDIVIDUALS][CROMO][GENES])
{
    int x, y, z;
    for(x = 0; x < INDIVIDUALS; x++)
    {
        if(DEBUGLEVEL > 1)
            printf("\n INDIVIDUAL %d\n", x);
        for(y = 0; y < CROMO; y++)
        {
            if(DEBUGLEVEL > 1)
                printf("\n CROMO %d: ", y);
            for(z = 0; z < GENES; z++)
            {
                bobsGens[x][y][z] = rand()%(int)(MAXVALUE/CROMO);
                if(DEBUGLEVEL > 1)
                {   
                    if(!y)  
                        printf("%f ", (bobsGens[x][y][z]/FLOATADJUST));
                    else
                        printf("%f ", (bobsGens[x][y][z] / PERCENTADJUST));
                }
            }
        }
    }
    return ;
}

void getHealth(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS])
{
    int individual, error, lasterror, power, distance, sum, delta, status, counter = 0;

    for(individual = 0; individual < INDIVIDUALS; individual++)
    {
        counter = sum = delta = lasterror = 0;
        status = myBob[individual].getStatus();
        while(status == 1 && counter < AUTOMATICSTOP)
        {
            distance = myBob[individual].getDistance();
            error = distance - SETPOINT;
            sum = sum + error;

            if(error == 0 || (error >= 0 && lasterror <= 0) || (error <= 0 && lasterror >= 0))
            {
                sum = 0;
            }

            power = getPower(bobsGens, individual, error, sum, delta);
            myBob[individual].move(power);
            status = myBob[individual].getStatus();
            counter++;
            if(DEBUGLEVEL > 3)
                printf("distance: %d error: %d sum: %d delta: %d power: %d \n", distance, error, sum, delta, power);
            delta = error - lasterror;
            lasterror = error;
        }
        status = myBob[individual].getStatus();
        if(status == -1)
        {
            health[individual] = 1;
            if(DEBUGLEVEL < 0)
                printf("\n individual :%d crash the wall\n", individual);
        }
        else
        {
            if(status == 1)
            {
                health[individual] = 1;
                if(DEBUGLEVEL < 0)
                    printf("\n individual :%d timeout\n", individual);
            }
            else
            {
                health[individual] = AUTOMATICSTOP - counter;
                if(DEBUGLEVEL < 0)
                    printf("\n individual :%d success with health %d", individual, health[individual]);
            }
        }

        myBob[individual].restart(INITIALDISTANCE, SETPOINT);
    }
    return ;
}

int getPower(int bobsGens[INDIVIDUALS][CROMO][GENES], int individual, int error, int sum, int delta)
{
    int cromosum[CROMO] = {0};
    int x, y, power;
    float kp, ki, kd;

    for(x = 0; x < CROMO; x++)
    {
        for(y = 0; y < GENES; y++)
        {
            cromosum[x] += bobsGens[individual][x][y];
        }
    }

    kp = (cromosum[BASEVALUE]/FLOATADJUST) * ((cromosum[KP]/PERCENTADJUST) / 100.0); 
    ki = (cromosum[BASEVALUE]/FLOATADJUST) * ((cromosum[KI]/PERCENTADJUST) / (100.0*KIADJUST));
    kd = (cromosum[BASEVALUE]/FLOATADJUST) * ((cromosum[KD]/PERCENTADJUST) / (100.0*KDADJUST));

    if(DEBUGLEVEL > 1)
        printf("\n\n%f %f %f\n\n", kp, ki, kd);

    power = (error*kp) + (sum*ki) + (delta*kp);

    return power;
}


