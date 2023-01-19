
#include "ultrasound_collision_detection.hpp"

int main(int argc, char** argv) {
	ros::init(argc, argv, "ultrasound_collision_detection");
	ros::NodeHandle nh;

	UltrasoundCollisionDetection uscd(nh);
	
	ros::MultiThreadedSpinner spinner(2); //TODO
	spinner.spin();
	
	//TODO Exit gracefully from here on Ctrl+C signal: 
	
	return 0;
}
