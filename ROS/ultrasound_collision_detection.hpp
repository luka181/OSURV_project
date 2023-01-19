
#pragma once

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <nav_msgs/Odometry.h>


class UltrasoundCollisionDetection {
public:
	UltrasoundCollisionDetection(ros::NodeHandle& nh);
	~UltrasoundCollisionDetection();
	
protected:
	bool all_motors_sim;
	
	int drv_fd;
	int file_desc;
	void seek();
	
	ros::NodeHandle nh;

	ros::Subscriber motors_en_sub;
	void motors_en_cb(const std_msgs::Bool::ConstPtr& msg);
	volatile bool motors_en;

	float speed;
	float dist; // distance from object
	ros::Subscriber in_cmd_sub;
	void in_cmd_cb(const ackermann_msgs::AckermannDriveStamped::ConstPtr& msg);
	
	ros::Timer non_realtime_loop_timer;
	void poll_ultrasound(const ros::TimerEvent& e);
	ros::Publisher out_cmd_pub;
	ros::Time last;
	float traversed_path;
};
