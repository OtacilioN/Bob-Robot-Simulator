/*
 * bobuniverse.cpp
 * 
 * It is a Bob Robot Universe Simulator
 *
 * Created on September 13, 2017
 * By Otacilio Neto (github: @OtacilioN | Email: contato@otaciliomaia.com)
 * 
 * Published under mit license
 */
 
#include "bobuniverse.h"
#include <stdio.h>

#define DEFAULTBRAKE 4
#define DEBUG 0

Universe::Universe(int initialDistance, int setPoint, int friction) {
	_initialDistance = initialDistance;
	_setPoint = setPoint;
	_friction = friction;
	position = _initialDistance;
	speed = 0;
	status = 1;
	seconds = 0;
}

void Universe::restart(int initialDistance, int setPoint, int friction) { 
	_initialDistance = initialDistance;
	_setPoint = setPoint;
	_friction = friction;
	position = _initialDistance;
	speed = 0;
	status = 1;
	seconds = 0;
    return ;
}

int Universe::getStatus() {
	if(position < 0) // The robot crash into the wall
		status = -1;
		
	if((int)position == _setPoint && speed < 0.1 && speed > - 0.1) { // Success
		status = 0;
	}
	
	return status;
}

void Universe::move(int power) {
	if(status > 0){
		if(speed > 0.09)
    		acceleration = (power - _friction)/1000.0;
    	else if(speed < -0.09)
    		acceleration = (power + _friction)/1000.0;
    	else if(power > _friction)
    		acceleration = (power - _friction)/1000.0;
    	else if(power < _friction*-1)
    		acceleration = (power + _friction)/1000.0;
        else
            acceleration = power/10000.0;
    	
    	speed += acceleration;
		position -= speed;
		seconds++;
        if(DEBUG)
        {
		    printf("the acceleration: %f\n", acceleration);
		    printf("the speed: %f\n", speed);
        }
		
		getStatus();
	}
	return ;
}

void Universe::brake() {
	printf("the speed: %f\n", speed);
	if(speed < DEFAULTBRAKE && speed > DEFAULTBRAKE*-1)
		speed = 0;
	else if(speed >= DEFAULTBRAKE)
		speed -= DEFAULTBRAKE;
	else
		speed += DEFAULTBRAKE;

    return ;
}

int Universe::getDistance() {
	return (int)position; // The sensor has a int precision
}
	
int Universe::getSpeed() {
	return (int)speed; // The sensor has a int precision
}

int Universe::getTime() {
	return seconds; // The sensor has a int precision
}
