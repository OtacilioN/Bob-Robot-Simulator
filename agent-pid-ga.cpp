#include <stdio.h>
#include "bobuniverse.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define INITIALDISTANCE 60
#define SETPOINT 30
#define AUTOMATICSTOP 1000
#define GENERATIONS 5000
#define INDIVIDUALS 30
#define CROMO 3
#define GENES 4
#define KEEP 2
#define FLOATADJUST 10000000.0
#define PERCENTADJUST (FLOATADJUST/10)
#define MAXVALUE (10*FLOATADJUST)
#define KIADJUST 10.0
#define KDADJUST 10.0
#define KBASE 10.0
#define MUTATIONCHANCE 40 // 10 Means 10%

/* Cromo dictionary */
#define KP 0
#define KI 1
#define KD 2

/* cross dictionary */
#define MOTHER 0
#define FATHER 1
#define COUPLE 2

#define DEBUGLEVEL 0

void generateGens(int bobsGens[INDIVIDUALS][CROMO][GENES]);
void getHealth(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS]);
int getPower(int bobsGens[INDIVIDUALS][CROMO][GENES], int individual, int error, int sum, int delta);
void printPopulation(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS], int generation);
void getCromosum(int bobsGens[INDIVIDUALS][CROMO][GENES], int cromosum[CROMO], int individual);
void clearCromosum(int cromosum[CROMO]);
void orderByHealth(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS], int start, int end);
void cross(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS]);
void mutation(int bobsGens[INDIVIDUALS][CROMO][GENES]);
void printbest(int health[INDIVIDUALS], int generation);
void printgens(int bobsGens[INDIVIDUALS][CROMO][GENES]);
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
        if(DEBUGLEVEL>1)
            printPopulation(bobsGens, health, generation);
        
        orderByHealth(bobsGens, health, 0, (INDIVIDUALS-1));
        printbest(health, generation);
        if(DEBUGLEVEL)
        {
            if(DEBUGLEVEL > 1)
                printf("\n Ordered \n");
            printPopulation(bobsGens, health, generation);
        }
        
        cross(bobsGens, health);
        if(DEBUGLEVEL > 2)
        {
            printf("\n After crossing \n");
            printPopulation(bobsGens, health, generation);
        }
        mutation(bobsGens);
    }

    printgens(bobsGens);

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
                bobsGens[x][y][z] = rand()%(int)(MAXVALUE/GENES);
                if(DEBUGLEVEL > 1)
                {   
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
    int power;
    float kp, ki, kd;
    
    getCromosum(bobsGens, cromosum, individual);
    
    kp = KBASE * ((cromosum[KP]/PERCENTADJUST) / 100.0); 
    ki = KBASE * ((cromosum[KI]/PERCENTADJUST) / (100.0*KIADJUST));
    kd = KBASE * ((cromosum[KD]/PERCENTADJUST) / (100.0*KDADJUST));

    if(DEBUGLEVEL > 5)
        printf("\n\n%f %f %f\n\n", kp, ki, kd);

    power = (error*kp) + (sum*ki) + (delta*kp);

    return power;
}

void printPopulation(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS], int generation)
{
    int individual, genes;
    int cromosum[CROMO] = {0};
    float kp, ki, kd;
    
    printf("\n-------------- Generation %d --------------\n\n", generation);
    for(individual = 0; individual < INDIVIDUALS; individual++)
    {
        printf("    --- Individual --- %d\n", individual);
        
        getCromosum(bobsGens, cromosum, individual);
        kp = KBASE * ((cromosum[KP]/PERCENTADJUST) / 100.0); 
        ki = KBASE * ((cromosum[KI]/PERCENTADJUST) / (100.0*KIADJUST));
        kd = KBASE * ((cromosum[KD]/PERCENTADJUST) / (100.0*KDADJUST));
        printf("    KP: %f  KI: %f  KD: %f  Health: %d\n", kp, ki, kd, health[individual]);
        
        clearCromosum(cromosum);
    }
    return ;
}

void getCromosum(int bobsGens[INDIVIDUALS][CROMO][GENES], int cromosum[CROMO], int individual)
{
    int cromo, gene;
    for(cromo = 0; cromo < CROMO; cromo++)
    {
        for(gene = 0; gene < GENES; gene++)
        {
            cromosum[cromo] += bobsGens[individual][cromo][gene];
        }
    }
    
    return ;
}

void clearCromosum(int cromosum[CROMO])
{
    int cromo;
    for(cromo = 0; cromo < CROMO; cromo++)
        cromosum[cromo] = 0;
    return ;
} 

void orderByHealth(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS], int start, int end)
{
    int pivot, cromo, gene, aux, aux2[CROMO][GENES], i, j, half;
   
    i = start;
    j = end;
   
    half = (int) ((i + j) / 2);
    pivot = health[half];
   
    do{
        while (health[i] < pivot) 
            i = i + 1;
        while (health[j] > pivot) 
            j = j - 1;
      
        if(i <= j){
         
            // Saving in aux
            aux = health[i];
            for(cromo = 0; cromo < CROMO; cromo++)
            {
                for(gene = 0; gene < GENES; gene++)
                    aux2[cromo][gene] = bobsGens[i][cromo][gene];
             }
         
            // Replacing places
            health[i] = health[j];
            for(cromo = 0; cromo < CROMO; cromo++)
            {
                for(gene = 0; gene < GENES; gene++)
                    bobsGens[i][cromo][gene] = bobsGens[j][cromo][gene];
            }
         
            // Updating j with aux
            health[j] = aux;
            for(cromo = 0; cromo < CROMO; cromo++)
            {
                for(gene = 0; gene < GENES; gene++)
                    bobsGens[j][cromo][gene] = aux2[cromo][gene];
            }
         
            i = i + 1;
            j = j - 1;
        }
    }while(j > i);
   
    if(start < j)
        orderByHealth(bobsGens, health, start, j); 
    if(i < end)
        orderByHealth(bobsGens, health, i, end);
    
    return ;
}

void cross(int bobsGens[INDIVIDUALS][CROMO][GENES], int health[INDIVIDUALS])
{
    int luck, cromo, gene, individual, match[COUPLE], accumulatedHealth[INDIVIDUALS], sons[INDIVIDUALS][CROMO][GENES], x;
    
    accumulatedHealth[0] = health[0];
    for(individual = 1; individual < INDIVIDUALS; individual++)
        accumulatedHealth[individual] = accumulatedHealth[individual-1] + health[individual];
    
    for(individual = 0; individual < (INDIVIDUALS-KEEP); individual++)
    {
        luck = rand()%accumulatedHealth[INDIVIDUALS-1];
        for(x = INDIVIDUALS-1; x >= 0; x--) // > probability to came from right
        {
            if(luck > (accumulatedHealth[x]))
                break;
        }
        match[MOTHER] = x+1;
        
        do
        {
            luck = rand()%accumulatedHealth[INDIVIDUALS-1];
            for(x = INDIVIDUALS-1; x >= 0; x--) // > probability to came from right
            {
                if(luck > (accumulatedHealth[x]))
                    break;
            }
            match[FATHER] = x+1;
        }while(match[MOTHER] == match[FATHER]);
        
        if(DEBUGLEVEL > 2)
            printf("CROSS: match found for: %d   %d\n", match[MOTHER], match[FATHER]);
        
        // Each couple generates 2 sons, each sons have half genes from mother and half from father for each cromo
        for(cromo = 0; cromo < CROMO; cromo++)
        {
            for(gene = 0; gene < GENES/2; gene++)
                sons[individual][cromo][gene] = bobsGens[match[MOTHER]][cromo][gene];
            for(; gene < GENES; gene++)
                sons[individual][cromo][gene] = bobsGens[match[FATHER]][cromo][gene];
        }
        
        // Second son
            individual++;
        
        for(cromo = 0; cromo < CROMO; cromo++)
        {
            for(gene = 0; gene < GENES/2; gene++)
                sons[individual][cromo][gene] = bobsGens[match[FATHER]][cromo][gene];
            for(; gene < GENES; gene++)
                sons[individual][cromo][gene] = bobsGens[match[MOTHER]][cromo][gene];
        }
            
    }
    
    // Generation Update
    for(individual = 0; individual < (INDIVIDUALS-KEEP); individual++)
    {
        for(cromo = 0; cromo < CROMO; cromo++)
        {
            for(gene = 0; gene < GENES; gene++)
                bobsGens[individual][cromo][gene] = sons[individual][cromo][gene];
        }
    }
    
    return ;
}

void mutation(int bobsGens[INDIVIDUALS][CROMO][GENES])
{
    int luck, individual, cromo, gene;

    for(individual = 0; individual < (INDIVIDUALS-KEEP); individual++)
    {
        luck = rand()%100;
        if(luck < MUTATIONCHANCE)
        {
            cromo = rand()%CROMO;
            gene = rand()%GENES;
            bobsGens[individual][cromo][gene] = rand()%(int)(MAXVALUE/GENES);
        }   
    }

    return ;
}

void printbest(int health[INDIVIDUALS], int generation)
{
    int bobtime;
    bobtime = AUTOMATICSTOP - health[INDIVIDUALS - 1];
    printf("%d %d\n", bobtime, generation);
    return ;
}

void printgens(int bobsGens[INDIVIDUALS][CROMO][GENES])
{
    int cromosum[CROMO] = {0};

    float kp, ki, kd;

    getCromosum(bobsGens, cromosum, INDIVIDUALS-1);


    kp = KBASE * ((cromosum[KP]/PERCENTADJUST) / 100.0); 
    ki = KBASE * ((cromosum[KI]/PERCENTADJUST) / (100.0*KIADJUST));
    kd = KBASE * ((cromosum[KD]/PERCENTADJUST) / (100.0*KDADJUST));

    printf("\n\n%f %f %f\n\n", kp, ki, kd);

    return ;
}
