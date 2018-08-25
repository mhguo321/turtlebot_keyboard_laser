// ROS头文件
#include <ros/ros.h>
//包含自定义msg产生的头文件
// ROS标准msg头文件
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
  //激光测距
  float ranges[10];
  ranges[0] = msg->ranges[0];
  ranges[1] = msg->ranges[180];
  ranges[2] = msg->ranges[360];
  ranges[3] = msg->ranges[540];
  ranges[4] = msg->ranges[720];
  ranges[5] = msg->ranges[900];
  ranges[6] = msg->ranges[1080];
  // distance.data = sqrt(pow(msg->x,2)+pow(msg->y,2));
  // float distance = sqrt(pow(msg->x,2)+pow(msg->y,2));
  // ROS_INFO("Listener: Distance to origin = %f, state:
  // %s",distance.data,msg->state.c_str());
  printf("%f %f %f %f %f\n", ranges[0], ranges[1], ranges[3], ranges[5], ranges[6]);
  //printf("Hi%f\n",ranges[0]);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_listen_node");
  ros::NodeHandle n;
  ros::Subscriber sub =
      n.subscribe<sensor_msgs::LaserScan>("scan", 1, LaserCallback);
  // ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
  ros::spin();
  return 0;
}
