/*
	Note: This code is based on the following example:
	http://www.cim.mcgill.ca/~yiannis/417/2013/COMP417_A1.pdf
*/

#include "ros/ros.h"
#include "Robot.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RobotFollower");
	ros::NodeHandle n;
	//Creating a node that is going to control Robot_1
	Robot wander(n, 1, -10.0/180*M_PI, +10.0/180*M_PI, 2.0, 1.0, M_PI/2);
	/*Parameters
		NodeHandle
		Number of the robot to control
		Minimum angle of 10 degrees that we are converting it to radians.
		Maximum angle of 10 degrees that we are converting it to radians.
		Minimum distance of 2 meters permitted before the robot begin to turn to another direction.
		Forward velocity in miles per second
		Angular velocity in miles per second
	*/
	wander.spin();
 
	return 0;
};
