#include "ros/ros.h"
#include "Robot.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RobotFollower");
	ros::NodeHandle n;
	//Creating a node that is going to control Robot_1
	Robot wander(n, 1, -10.0/180*M_PI, +10.0/180*M_PI, 2.0, 1.0, M_PI/2);
	wander.spin();
 
	return 0;
};
