# MiniProject-UNR

The repository contains four files at the moment:

*Robot.h
*Robot.cpp
*random_walk.cpp
*random_walk_1.cpp

The project execute two nodes on the stageros simulator: RobotWander and RobotFollower. The RobotWander node is created in the random_walk.cpp file and the RobotFollower is created in the random_walk_1.cpp file. Then, it is necessary to create two robots in the .world file to execute them correctly. These two robots have to have their laser scan a little bit below to be able to detect them each other.

The class Robot is declared and defined in the files: Robot.h and Robot.cpp respectively. It contains all the functions and attributes necessary to make a robot wander in a random place.

Currently, the RobotWander and RobotFollower nodes will wander in a world in stageros simulator. I'm still working to make RobotFollower to make honor to its name and actually follow the RobotWander instead of just wander like the other robot.

This is the algorithm I am thinking to make RobotFollower to follow RobotWander (Note: I haven't tested it):

Initial state: The RobotWanderer will be in front of the RobotFollower in the same orientation.

This robot is going to have an additional state but also of the other two: FSM_DETECT_ROBOT. This will be the initial state of the robot.

It will have a vector array to hold the values of the past laser data points.

The RobotFollower will have a Field of View (FOV) of 45 degrees. The RobotWander have a FOV of 20 degrees.

In the method commandCallback

1.- If the state of the Robot is FSM_DETECT_ROBOT
	It will calculate the difference between the current laser data points and the past laser data points
	While the currIndex is less than maxIndex and the vector array size is not zero
		If in one data laser point the difference is not close to zero.
			Get the index of that point
			Save the range variable in the variable distance_to_robot
			Calculate the angle of that point with respect to the laser scan
			It will set the angle calculated to the variable angles_to_turn
			Set the Finite State Machine to the state FSM_TURN and break the loop
2.- Save current values of the laser data points in the vector array

In the method spin 

1.- If the state of the Robot is FSM_DETECT_ROBOT
	It will stood still
else if the state of the Robot is FSM_TURN
	It will keep turning until it reached the desired angles in the variable angles_to_turn using as a reference the odometry of the robot
	if the robot reached the value angles_to_turn
		Stop turning
		Set as a new state FSM_MOVE_FORWARD
	else
		Keep turning
else if the state of the Robot is FSM_MOVE_FORWARD
	It will move forward until it reached the value of the difference distance_to_robot - PROXIMITY_RANGE_M using as a reference the odometry of the robot
	if the robot reached the value distance_to_robot
		Stop moving forward
		Set as a new state FSM_DETECT_ROBOT
	else
		Keep moving forward		

Stay tuned!

Best,

Andres Torres Garcia