#include <stdio.h>
#include "bobuniverse.h"

int main(void){
    int distance; 
    Universe myBob(100, 30, 100);
    
    distance = myBob.getDistance();
    printf("%d", distance);
    
    return 0;
}