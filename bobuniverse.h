/*
 * bobuniverse.h
 * 
 * It is a Bob Robot Universe Simulator
 *
 * Created on September 13, 2017
 * By Otacilio Neto (github: @OtacilioN | Email: contato@otaciliomaia.com)
 * 
 * Published under mit license
 */
 
#ifndef bobuniverse_h
#define bobuniverse_h
class Universe {
    public:
        Universe(int initialDistance, int setPoint, int friction = 100);
        void move(int power);
        int getDistance();
        int getSpeed();
        
    private
        int _initialDistance;
        int _setPoint;
        int _friction;
        int speed = 0;
        float acceleration;
        int status;
        int position;
};
#endif
