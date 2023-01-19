
#include "ultrasound_collision_detection.hpp"

// Config.
#define LOOP_HZ 25

#include "motor_ctrl.h"


// Includes for driver.
#include <string.h> // strerror()
#include <unistd.h> // file ops
#include <fcntl.h> // open() flags
#include <sys/ioctl.h> // ioctl()

#define BUF_LEN 80 //

#include <algorithm>
using namespace std;

#include <angles/angles.h>


#define DEBUG(x) do{ ROS_DEBUG_STREAM(#x << " = " << x); }while(0)

template<typename T>
static T sym_clamp(T x, T limit) {
	return clamp(x, -limit, limit);
}

UltrasoundCollisionDetection::UltrasoundCollisionDetection(ros::NodeHandle& nh) :
	nh(nh),
	all_motors_sim(false),
	drv_fd(-1),
	motors_en(true)
{


	// Open driver.
	drv_fd = open(DEV_FN, O_RDWR);
	if (drv_fd < 0) {
		ROS_WARN(
			"\"%s\" not opened! drv_fd = %d -> %s",
			DEV_FN, drv_fd, strerror(-drv_fd)
		);
	}

	//Open gpio driver
	file_desc = open("/dev/Driver", O_RDWR);
	if (file_desc < 0)
	{
		ROS_INFO("Error, 'dummy' not opened\n");

	}
	////////////
	motor_ctrl__ioctl_arg_moduo_t ia;
	ia.ch = 0;
	ia.moduo = 20; // 5kHz
	int r = ioctl(
		drv_fd,
		IOCTL_MOTOR_CLTR_SET_MODUO,
		*(unsigned long*)&ia
	);
	if (r) {
		ROS_WARN("ioctl went wrong!");
	}


	in_cmd_sub = nh.subscribe(
		"in_cmd",
		1,
		&UltrasoundCollisionDetection::in_cmd_cb,
		this
	);


	ros::Duration timer_period = ros::Duration(1.0 / LOOP_HZ); // osvezavanje
	non_realtime_loop_timer = nh.createTimer(
		timer_period,
		&UltrasoundCollisionDetection::poll_ultrasound,
		this
	);

	out_cmd_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>(
		"out_cmd",
		1
		);

	last = ros::Time::now();
}

UltrasoundCollisionDetection::~UltrasoundCollisionDetection() {
	// Close driver.
	ROS_INFO("closing drv_fd");
	close(drv_fd);

	//Close gpio driver
	ROS_INFO("closing file_desc");
	close(file_desc);
}


void UltrasoundCollisionDetection::poll_ultrasound(const ros::TimerEvent& e) {
	ros::Time now = ros::Time::now();


	//TODO Read from driver.		
	int time;
	int file_desc;
	char tmp[BUF_LEN];
	char tm1[BUF_LEN];

	tm1[0] = '1'; // to triger ultrasonic senzor    

	write(file_desc, tm1, BUF_LEN);

	read(file_desc, tmp, BUF_LEN);


	time = atoi(tmp);
	dist = time * 34000 / 2000000.0;
	ROS_DEBUG("vreme: %d\n", time);
	ROS_DEBUG("udaljenost od objekta: %f\n", dist);
	tmp[0] = '0';

}

void UltrasoundCollisionDetection::motors_en_cb(		//motor ukljucen/iskljucen
	const std_msgs::Bool::ConstPtr& msg
) {
	motors_en = msg->data;
	ROS_INFO(
		"Motors %s", motors_en ? "enabled" : "disabled"
	);
}


void UltrasoundCollisionDetection::in_cmd_cb(
	const ackermann_msgs::AckermannDriveStamped::ConstPtr& msg
) {
	speed = msg->drive.speed;
	float steering_angle = msg->drive.steering_angle;

#if 0
	ROS_DEBUG("speed = %f", speed);
	ROS_DEBUG("steering_angle = %f", steering_angle);
#endif
	//TODO Publish on out_cmd
	// Take into account ultrasound stuff.

	float max_speed = 100;
	if (!all_motors_sim && motors_en)
	{
		if ((dist > 20) && (dist < 100))
		{
			speed = max_speed * (1 - (20 / dist));	// linear reduce speed
		}
		if (dist < 20)
		{
			speed = 0;
		}
	}
	ackermann_msgs::AckermannDriveStamped out_msg;
	out_msg.drive.speed = speed;
	out_cmd_pub.publish(out_msg);

}
