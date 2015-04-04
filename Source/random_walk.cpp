#include "ros/ros.h"
#include "Robot.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RobotWanderer");
	ros::NodeHandle n;
	//Creating a node that is going to control Robot_0
	Robot wander(n, 0, -10.0/180*M_PI, +10.0/180*M_PI, 2.0, 1.0, M_PI/2);
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
