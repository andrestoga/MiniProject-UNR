/*
	Note: This code is based on the following example:
	http://www.cim.mcgill.ca/~yiannis/417/2013/COMP417_A1.pdf
*/


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "Robot.h"
#include <cstdlib>//Needed to generate 'random' numbers.
#include <ctime>//Needed to seed random number.
#include <string>//Needed to concatenate strings.

//Initializing the object robot.
Robot::Robot(ros::NodeHandle& nh, int numRobot,
double min_scan_angle_rad, double max_scan_angle_rad, float proximity_range_m,
double forward_speed_mps, double rotate_speed_radps) : 
fsm(FSM_MOVE_FORWARD), 
rotateStartTime(ros::Time::now()),
rotateDuration(0.f),
MIN_SCAN_ANGLE_RAD(min_scan_angle_rad),
MAX_SCAN_ANGLE_RAD(max_scan_angle_rad),
PROXIMITY_RANGE_M(proximity_range_m),
FORWARD_SPEED_MPS(forward_speed_mps),
ROTATE_SPEED_RADPS(rotate_speed_radps)
{
	std::stringstream vel;
	std::stringstream las;
	
	//Initializing random time generator.
	srand(time(NULL));
	
	//Concatenating the number of the robot.
	vel << "robot_" << numRobot << "/cmd_vel";
	las << "robot_" << numRobot << "/base_scan";
	
	//Publishing to the topic 'cmd_vel' with message of type 'geometry_msgs'.
	commandPub = nh.advertise<geometry_msgs::Twist>(vel.str(), 1);
	//Subscribing to the topic 'base_scan' and to receive the data in the function 'commandCallback'.
	laserSub = nh.subscribe(las.str(), 1, &Robot::commandCallback, this);
}
 
void Robot::move(double linearVelMPS, double angularVelRadPS)
{
	// Creating a message of type 'geometry_msgs'.
	geometry_msgs::Twist msg;
	//Assigning the linear and angular velocities.
	msg.linear.x = linearVelMPS;
	msg.angular.z = angularVelRadPS;
	//Publishing the message to the topic.
	commandPub.publish(msg);
}
 
void Robot::commandCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	//If the robot is in the state of moving forward.
	if (fsm == FSM_MOVE_FORWARD)
	{
		//Converting min and max radians to steps in order to iterate over the array.
		unsigned int minIndex = ceil((MIN_SCAN_ANGLE_RAD - msg->angle_min) / msg->angle_increment);
		unsigned int maxIndex = ceil((MAX_SCAN_ANGLE_RAD - msg->angle_min) / msg->angle_increment);
		
		//Setting initial value to closest range.
		float closestRange = msg->ranges[minIndex];
		
		//Iterating over a subgroup of steps of the selected Field of View(FOV).
		for (unsigned int currIndex = minIndex + 1; currIndex < maxIndex; currIndex++)
		{
			//Gathering the closest range.
			if (msg->ranges[currIndex] < closestRange)
			{
				closestRange = msg->ranges[currIndex];
			}
		}
		
		//Displaying to the console the closest range.
		ROS_INFO_STREAM("Range: " << closestRange);

		//If it is less than the allowed range, rotate to a random direction.
		if(closestRange < PROXIMITY_RANGE_M)
		{
			fsm = FSM_ROTATE;//If yes, change to turn state.

			rotateStartTime = ros::Time::now();
			rotateDuration = ros::Duration(rand() % 5 + 1);
		}
	}
	else
	{		
		//Checking if it has passed the duration of the rotation.
		if((ros::Time::now().sec - rotateStartTime.sec) >= rotateDuration.sec)
		{
			fsm = FSM_MOVE_FORWARD;//If yes, change to move forward state.
		}
	}

}

void Robot::spin()
{
	//Set the loop at 10 HZ
	ros::Rate rate(10);

	while (ros::ok())
	{
		//If the state is rotate, rotate the robot
		if(fsm == FSM_ROTATE)
		{
			move(0, ROTATE_SPEED_RADPS);
		}//If the state is move forward, move forward
		else if(fsm == FSM_MOVE_FORWARD)
		{
			move(FORWARD_SPEED_MPS, 0);
		}
		else
		{
			//This piece of code shouldn't be reached.
			ROS_INFO_STREAM("Error\n");
		}

		ros::spinOnce();//Call this function to process ROS incoming messages.
		rate.sleep();//Sleep the rest of the cycle until 10 Hz
	
	}
}
