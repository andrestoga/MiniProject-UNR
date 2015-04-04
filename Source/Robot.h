/*
	Note: This code is based on the following example:
	http://www.cim.mcgill.ca/~yiannis/417/2013/COMP417_A1.pdf
*/

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

class Robot
{
	public:
		
		//Constructor to initialize a robot.
		Robot(ros::NodeHandle& nh, int numRobot,
		double min_scan_angle_rad, double max_scan_angle_rad, float proximity_range_m,
		double forward_speed_mps, double rotate_speed_radps);
 		
 		//Method to send commands of movement to the robot.
		void move(double linearVelMPS, double angularVelRadPS);
		
 		//This method is going to be called every time we got some data from the laser.
		void commandCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
		
		//Execute loop at 10 Hz
		void spin();
		
 		//Finite State Machine
		enum FSM {FSM_MOVE_FORWARD, FSM_ROTATE};
 
		const double MIN_SCAN_ANGLE_RAD;//Specify the min angle in radians to take into account in the laser data.
		const double MAX_SCAN_ANGLE_RAD;//Specify the max angle in radians to take into account in the laser data.
		const float PROXIMITY_RANGE_M;//The minimum range in meters to accept before the robot stop and turn to another random direction.
		const double FORWARD_SPEED_MPS;//Miles per second
		const double ROTATE_SPEED_RADPS;//Radians per second
 
	protected:
	
		ros::Publisher commandPub;//Publisher to the velocity command topic
		ros::Subscriber laserSub;//Subscriber to the laser scan topic
		enum FSM fsm;
		ros::Time rotateStartTime;//Start time of the rotation.
		ros::Duration rotateDuration;//Duration of the rotation.
};
