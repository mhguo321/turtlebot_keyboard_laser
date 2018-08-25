#include <ros/ros.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <boost/thread/thread.hpp>
//自定义msg产生的头文件
#include <lg_project/key.h>

#define KEYCODE_R 0x43 
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71
#define KEYCODE_0 0x30
#define KEYCODE_1 0x31


int kfd = 0;
struct termios cooked, raw;

lg_project::key msg;
///////////////////////////////////////////////////////
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
  if(c==' ')
  {
    msg.keyState = "Null";
  }
  //puts("---------------------------");
  //puts("Use arrow keys to move the turtle.");

  std::cout<<c<<std::endl;
  //for(;;)
  //{
    // get the next event from the keyboard  
    if(::read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }
    puts("test1\n");
    // ROS_DEBUG("value: 0x%02X\n", c);
    switch(c)
    {
      case KEYCODE_0:
        msg.isManual = true;
        msg.isAuto = false;
        msg.keyState = "manual";
        break;
      case KEYCODE_1:
        msg.isManual = false;
        msg.isAuto = true;
        msg.keyState = "auto";
        break;
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
  //}
  puts("test2\n");
  return 0;
}


int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "keyloop_node");

  //实例化句柄，初始化node
  ros::NodeHandle nh;

  msg.isManual = true;
  msg.isAuto = false;
  msg.keyState = "Null";
  

  //创建publisher
  ros::Publisher pub = nh.advertise<lg_project::key>("key_info", 1);

  //ros::Timer timer1 = nh.createTimer(ros::Duration(0.01), keyLoop);

  pub.publish(msg);
  //定义发布的频率 
  ros::Rate loop_rate(100.0);
  //循环发布msg
  while (ros::ok())
  {
    keyLoop();
    pub.publish(msg);
    loop_rate.sleep();
  }
  
  return 0;
}
