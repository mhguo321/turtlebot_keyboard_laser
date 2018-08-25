#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
//自定义msg产生的头文件
#include <sensor_msgs/LaserScan.h>
#include <keyboard/Key.h>
//ROS标准msg头文件
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>

#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include <geometry_msgs/Twist.h>
#include <math.h>

enum KeyCmdOption
{
	Forward = 1,
	Backward,
  Right,
  Left,
  QuickStop,
  SlowStop
};
int cmd_Flag;

float ranges[10];
bool isManual = true;
bool isAuto = false;
bool isPressed = false;
uint16_t Code;
#define KEY_0 48
#define KEY_1 49
#define KEY_up 273
#define KEY_down 274
#define KEY_left 276
#define KEY_right 275
#define KEY_space 32

double target_speed;
double target_turn;
double control_speed = 0.0;
double control_turn = 0.0;

/**
 * This tutorial demonstrates the use of timer callbacks.
 */
void KeyLisenerCallback(const keyboard::Key::ConstPtr &msg)
{  
    //puts("Key Listener\n");
    isPressed = msg->isPressed;
    Code = msg->code;
    // listen for Manual or Auto mode:
    if(isPressed)
    {
          switch(Code)
        {
          case KEY_0:
            isManual = true;
            isAuto = false;
            break;
          case KEY_1:
            isManual = false;
            isAuto = true;
            break;

          case KEY_up:
            cmd_Flag = Forward;
            target_speed = 2.0;
            target_turn = 0.0;
            break;
          case KEY_down:
            cmd_Flag = Backward;
            target_speed = -2.0;
            target_turn = 0.0;
            break;
          case KEY_left:
            cmd_Flag = Left;
            target_speed = 0.0;
            target_turn = 2.0;
            break;
          case KEY_right:
            cmd_Flag = Right;
            target_speed = 0.0;
            target_turn = -2.0;
            break;
          case KEY_space:
            cmd_Flag = QuickStop;
            target_speed = 0.0;
            target_turn = 0.0;           
            control_speed = 0.0;
            control_turn = 0.0;
            break;
          default:
            cmd_Flag = SlowStop;
            target_speed = 0.0;
            target_turn = 0.0;
            break;        
        }
    }
    else
    {
          cmd_Flag = SlowStop;
          target_speed = 0.0;
          target_turn = 0.0;
    }
    
}
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
  printf("%f %f %f %f %f\n", ranges[0], ranges[1], ranges[3], ranges[5], ranges[6]);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "pub_twist_node");

  geometry_msgs::Twist vel;
  vel.angular.z = 0.0;
  vel.linear.x = 0.0;

  //实例化句柄，初始化node
  ros::NodeHandle nh;
  //创建publisher
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
  //ros::Publisher pub = nh.advertise<lg_project::key>("key_info", 1);
  

  // 初始化，创建节点
  ros::init(argc, argv, "listener_node");
  ros::NodeHandle n;

  // 定时器
  //ros::Timer timer1 = n.createTimer(ros::Duration(0.1), TimerCallback);
  //ros::Timer timer1 = n.createTimer(ros::Duration(0.1),boost::bind(&TimerCallback, this));


  ros::Subscriber sub1 = n.subscribe("/keyboard_node/keydown", 10, KeyLisenerCallback);
  ros::Subscriber sub2 = n.subscribe("scan", 10, LaserCallback);
  //ros::Subscriber sub2 =  n.subscribe<sensor_msgs::LaserScan>("scan", 1, LaserCallback);
	//ros::MultiThreadedSpinner s(1);
  //ros::spin(s);
  ros::spinOnce();

  static double count = 0;
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
      count+=0.1;
      printf("time: %5.1fs,",count);
      if(isManual)
      {
        std::cout<<"Manual, ";

        if(target_speed > control_speed)
        {
          control_speed = std::min( target_speed, control_speed + 0.2);
        }
        else if(target_speed < control_speed)
        {
          control_speed = std::max( target_speed, control_speed - 0.2);
        }
        else
        {
          control_speed = target_speed;
        }


        if(target_turn > control_turn)
        {
          control_turn = std::min( target_turn, control_turn + 0.2);
        }
        else if(target_turn < control_turn)
        {
          control_turn = std::max( target_turn, control_turn - 0.2);
        }
        else
        {
          control_turn = target_turn;
        }


        printf("v: %1.1fs, w: %1.1f\n",control_speed,control_turn);
      }

      if(isAuto)
      {
         std::cout<<"  Auto, ";
        control_speed = 0.0;
        control_turn = 0.0;
         printf("v: %1.1fs, w: %1.1f\n",control_speed,control_turn);
      }

      vel.angular.z = control_turn;
      vel.linear.x = control_speed;
      pub.publish(vel);

    	ros::spinOnce();
      loop_rate.sleep();
  }
	return 0;
}
