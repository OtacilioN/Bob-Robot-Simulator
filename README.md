# Bob Robot Simulator

## Who is bob?

If you have watched [this video](https://www.youtube.com/watch?v=7BDjZYGHupE&t=115s) you already meet Bob.

Bob is a robot who has a distance sensor, and he is capable of moving forward and backward, at different speeds. Bob likes to apreciate walls, and is better for his camera if he keeps a very particular defined distance of the wall, wich it call **setPoint**

## The bob universe

Bob lives in a universe with many dimensions, but Bob only moves in one dimension, forward or backward, and his sensor sees the wall like a point, so for Bob the universe is one-dimensional.

The Bob Robot Simulator is used for tests about control methods, like on-off controll, Fuzzy Control, PID Control, DeadBeat control and others. You can use the Bob Universe Simulator as a banchmark to compare differents forms of control.

## Getting Started

-First, [download](https://github.com/OtacilioN/Bob-Robot-Simulator/archive/master.zip) the repository with the library. 
-Into the folder of the repository, create a new file called agent-NAME-OF-AGENT.cpp, check the [bare-minimum agent](https://github.com/OtacilioN/Bob-Robot-Simulator/blob/master/agent-bare-minimum.cpp)

### Functions

``Universe(int initialDistance, int setPoint, int friction)`` Initializes Bob's universe, the param ``initialDistance`` receives a int with the initial distance from Bob to the wall, ``setPoint`` is the target distance, and ``friction`` is the main friction of the universe, 100 is the default value

``getStatus()`` Returns the status of the simulation, 1 if is running, 0 if it was successful (stop at the setPoint), and -1 if Bob crash into the wall

``move(int power)`` Makes the robot move, positive ``power`` to forward, and negative ``power`` to backward, it is recommended to use values between -1000 and 1000 for ``power``

``brake()`` Makes the robot activate the brake

``getDistance()`` Returns the current distance from Bob to the wall

``getSpeed()`` Returns the current speed from Bob

``getTime()`` Returns the current time in the Bob Universe, since the universe was initialized, time can be used by parameter for benchmarks
