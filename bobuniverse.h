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
        void restart(int initialDistance, int setPoint, int friction = 100);
        int getStatus();
        void move(int power);
        void brake();
        int getDistance();
        int getSpeed();
        int getTime();
        
    private:
        int _initialDistance;
        int _setPoint;
        int _friction;
        float speed;
        float acceleration;
        float position;
        int status;
        int seconds;
        
};
#endif
