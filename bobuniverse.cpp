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

Universe::Universe(int initialDistance, int setPoint, int friction) {
	_initialDistance = initialDistance;
	_setPoint = setPoint;
	_friction = friction;
	position = _initialDistance;
}

int Universe::getStatus() {
	if(position < 0) // The robot crash into the wall
		status = -1;
		
	if(position = _setPoint && speed == 0) // Success
		status = 0;
	
	return status;
}

void Universe::move(int power) {
	if(status > 0){
		if(speed > 0.9)
    		acceleration = (power - _friction)/1000.0;
    	else if(speed < 0.9)
    		acceleration = (power + _friction)/1000.0;
    	else if(power > _friction)
    		acceleration = (power - _friction)/1000.0;
    	else if(power < _friction*-1)
    		acceleration = (power + _friction)/1000.0;
    		
    	speed += acceleration;
		position += speed;
		seconds++;
		
		getStatus();
	}
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