
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <ros/ros.h>
#include <std_msgs/Char.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <sensor_msgs/LaserScan.h>



class pidNode{
public:

  // Attribute definition
  pidNode(){
    ros::NodeHandle node;
    node.getParam("Kp",Kp); // default 5
    node.getParam("Kd",Kd);
    node.getParam("Ki",Ki);
    node.getParam("look_ahead_coeff",look_ahead_coeff);
    node.getParam("sub_freq",sub_freq);
    node.getParam("speed_limit",speed_limit);
    pub = node.advertise<ackermann_msgs::AckermannDriveStamped>("/pid", 100); 
    sub = node.subscribe("/scan",sub_freq, &pidNode::callback, this);
    mode_sub = node.subscribe("/mode",1, &pidNode::modeCallBack, this);
    current_time = ros::Time::now().toNSec();
    prev_time = ros::Time::now().toNSec();


  }
  
  void modeCallBack(const std_msgs::Char::ConstPtr &msg){
    if (msg->data == 'r'){
      max_error = 0;
      max_derrive_error = 0;
    }
  }

  void smallestIndexRight(){
    // control value base
    int index_cache = 540;
    float depth_cache = 100;
    for(int i_l = 180; i_l<540; i_l++){
        if(scan_array[i_l] < scan_array[index_cache] && scan_array[i_l] != 0){
          index_cache = i_l;
          depth_cache = scan_array[i_l];
        }
    }
    min_index_right = index_cache;
    min_depth_right = depth_cache;
  }

  float pidControl(float error){

    prev_time = current_time;
    current_time = ros::Time::now().toSec();
    const auto dt = current_time - prev_time;
    error_integ += error;  
    if (abs(error_integ) > 10){
      error_integ = 0;
    }
    float controlValue = Kp*error + Kd*(error-error_last)/dt + Ki*error_integ;
    if (abs(error-error_last)>abs(max_derrive_error)){
      max_derrive_error = error-error_last;
      std::cout << "biggest derrive_error: " << max_derrive_error << std::endl;
    }
    if (abs(error) > abs(max_error)){
      max_error = error;
      std::cout << "biggest error: " << max_error << std::endl;
    }
    error_last = error;


    return controlValue;
  }
  float calculError(){

    //float angle = ((float)max_depth_index-(float)min_index_right) * (270.0/1080.0);
    float angle = (540.0-(float)min_index_right) * (270.0/1080.0);
    angle = (angle - 90.0)*(3.14/180.0);
    float cd = min_depth_right + look_ahead_coeff*sin(angle); //test
    float error = 0.7 - cd;  //test
    //float error = (abs(min_depth_left)+abs(min_depth_right))*0.5 - abs(cd);

    return error;
  }
  void callback(const sensor_msgs::LaserScan::ConstPtr &msg){
    ros::param::get("sub_freq",sub_freq);
    ros::param::get("Kp",Kp);
    ros::param::get("Kd",Kd);
    ros::param::get("Ki",Ki);
    ros::param::get("look_ahead_coeff",look_ahead_coeff);
    ros::param::get("speed_limit",speed_limit);
    float speed = 0.0;
    float angle = 0.0;

    for(int i_t=0; i_t<1080; i_t ++){
      scan_array[i_t] = msg->ranges[i_t];
    }

    pidNode::smallestIndexRight();
    //std::cout << "Kp: " << Kp <<std::endl;
    //std::cout << "Kd: " << Kd <<std::endl;
    //std::cout << "Ki: " << Ki <<std::endl;
    float error = pidNode::calculError();
    //std::cout << "error: " << error <<std::endl;
    float controlValue = pidNode::pidControl(error);
    //std::cout << "controlVlue: " << controlValue <<std::endl;

    if (abs(controlValue) < 0.0){                
      //std::cout << "acceleration" << std::endl;
      speed = 2;
      angle = 0;
    } else {
      //std::cout << "turning" << std::endl;
      //speed = 0.5*(5-abs(controlValue));
      //speed = abs(std::sqrt(scan_array[540])); // default: 1.2
      speed = 1.5;
      angle = controlValue;
    }

    //std::cout << "speed of car is: " << speed <<std::endl;
    //std::cout << "angular_speed is: " << angle <<std::endl;
    //std::cout << "\n";
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    //  Ackermann
    ackermann_msgs::AckermannDrive drive_msg;
    if (speed*speed_limit > 2){
      drive_msg.speed = 2;
    } else {
      drive_msg.speed = speed * speed_limit;
    }
    //std::cout << "spd in message: " << drive_msg.speed <<std::endl;
    //std::cout << "\n";
    drive_msg.steering_angle = angle * angle_limit;
    //  AckermannStamped
    ackermann_msgs::AckermannDriveStamped drive_st_msg;
    drive_st_msg.header = header;
    drive_st_msg.drive = drive_msg;
    // publish AckermannDriveStamped message to drive topic
    pub.publish(drive_st_msg);
  }


private:
  //std::vector<float> scan_range(1080, 0.0);

  float scan_array[1080] = {0.};
  int   min_index_right = 0;
  int   sub_freq;
  float min_depth_right = 0.0;  
  double speed_limit; //default 2.0
  float angle_limit = 0.5; //default 0.5

  double look_ahead_coeff; // test default 0.5
  double prev_time;
  double current_time;
  double Kp;     
  double Kd; // default 0.2
  double Ki; // default 0.0
  float error_last = 0.0;
  float error_integ = 0.0;
  double max_derrive_error=0;
  double max_error=0;
  ros::Publisher pub;
  ros::Subscriber sub;
  ros::Subscriber mode_sub;
};

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "pid control");
  pidNode pid_node;
  ROS_INFO("Kp: %f, Kd: %f, Ki: %f");
  ros::spin();
  return 0;
}