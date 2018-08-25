// ROS头文件
#include <ros/ros.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <boost/thread/thread.hpp>
//包含自定义msg产生的头文件
// ROS标准msg头文件
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>

#define KEYCODE_R 0x43 
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71


float ranges[10];
static float timeClock = 0;

int kfd = 0;
struct termios cooked, raw;


void LaserCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
  //激光测距
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
  //printf("%f %f %f %f %f\n", ranges[0], ranges[1], ranges[3], ranges[5], ranges[6]);
  //printf("Hi%f\n",ranges[0]);
}

void TimerCallback(const ros::TimerEvent&)
{
  //ROS_INFO("Callback 1 triggered\n");
  timeClock+=0.02;

  printf("%9.2f %f %f %f %f %f\n", timeClock,ranges[0], ranges[1], ranges[3], ranges[5], ranges[6]);
}

int keyLoop()
{
  char c;
  bool dirty=false;

  // get the console in raw mode                                                              
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file                         
  raw.c_cc[VEOL] = 1;//1
  raw.c_cc[VEOF] = 2;//2
  tcsetattr(kfd, TCSANOW, &raw);

  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use arrow keys to move the turtle.");


  for(;;)
  {
    // get the next event from the keyboard  
    if(::read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }

    // ROS_DEBUG("value: 0x%02X\n", c);
  
    switch(c)
    {
      case KEYCODE_L:
        std::cout << "LEFT\n";//<< std::endl;
        break;
      case KEYCODE_R:
        // ROS_DEBUG("RIGHT");
        std::cout << "RIGHT\n";// << std::endl;
        break;
      case KEYCODE_U:
        // ROS_DEBUG("UP");
        std::cout << "UP\n";// << std::endl;
        break;
      case KEYCODE_D:
        // ROS_DEBUG("DOWN");
        std::cout << "DOWN\n";// << std::endl;
        break;
    }

  }
  return 0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_listen_node");
  ros::NodeHandle n;
  ros::Subscriber sub =
      n.subscribe<sensor_msgs::LaserScan>("scan", 1, LaserCallback);
  // ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。

  ros::Timer timer1 = n.createTimer(ros::Duration(0.02), TimerCallback);

  keyLoop();

  ros::spin();
  return 0;
}
