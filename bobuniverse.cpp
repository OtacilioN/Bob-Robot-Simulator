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
 
#include "Brasilino.h"

Universe::Universe(int initialDistance, int setPoint, int friction) {
	_initialDistance = initialDistance;
	_setPoint = setPoint;
	_friction = friction;
	position = _initialDistance;
}

void Universe::move(int power) {
	if(status){
		if(speed >= 0)
    		acceleration = (power - _friction)/1000.0;
    	else
    		acceleration = (power + _friction)/1000.0;
    	
    	speed += acceleration;
		position += speed;
		
		if(position < 0) // The robot crash into the wall
			status = 0;
	}
}