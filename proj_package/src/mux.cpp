
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Char.h>
#include <nav_msgs/Odometry.h>




class Mux{
public:
  // Attribute definition

  Mux(){
    ros::NodeHandle node;
    pub = node.advertise<ackermann_msgs::AckermannDriveStamped>("/vesc/low_level/ackermann_cmd_mux/input/teleop", 1); ;
    mode_sub = node.subscribe("/mode",1, &Mux::callback, this);
    kb_sub = node.subscribe("/kb",1, &Mux::kbMode,this); //keyboard control sub
    fgm_sub = node.subscribe("/fgm",1, &Mux::fgmMode,this); //follow gap method sub
    pid_sub = node.subscribe("/pid",1, &Mux::pidMode,this); //pid method sub
    js_sub = node.subscribe("/vesc/js",1, &Mux::jsMode,this); // joy stick mode sub
  }
  void kbMode(ackermann_msgs::AckermannDriveStamped input_msg){
    if(mode == 'k') {pub.publish(input_msg);}
  }
  void fgmMode(ackermann_msgs::AckermannDriveStamped input_msg){
    if(mode == 'f') {pub.publish(input_msg);}
  }
  void pidMode(ackermann_msgs::AckermannDriveStamped input_msg){
    if(mode == 'p') {pub.publish(input_msg);}
  }
  void jsMode(ackermann_msgs::AckermannDriveStamped input_msg){
    if(mode == 'j') {pub.publish(input_msg);}
  }
  void callback(std_msgs::Char input_char){
    if(input_char.data == 'k'){
      mode = 'k';
    };
    if (input_char.data == 'f'){
      mode = 'f';
    }
    if (input_char.data == 'p'){
      mode = 'p';
    }
    if (input_char.data == 'j'){
      mode = 'j';
    }
    // return flag;
}

private:
  ros::Publisher pub;
  ros::Subscriber mode_sub;
  ros::Subscriber kb_sub;
  ros::Subscriber fgm_sub;
  ros::Subscriber aeb_flag_sub;
  ros::Subscriber aeb_mode_sub;
  ros::Subscriber spd_sub;
  ros::Subscriber pid_sub;
  ros::Subscriber js_sub;
  char mode;
  bool aeb_flag = false;
  bool aeb_engaged =false;
  double speed = 0.0;
};

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "mux");
  Mux mux;
  ros::spin();
  return 0;
}