#pragma once

// 头文件
// #include <fmt/core.h>
// #include <fmt/color.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "opencv4/opencv2/opencv.hpp"

// #define DEBUG

namespace network_com {
struct message_pack {
  int num;
  float age;
  char buffer_char[1024];
  message_pack() {
    num = 0;
    age = 0.f;
    memset(buffer_char, 0, sizeof(buffer_char));
  }
};

// 数据传输协议
// The color of our team
enum Color {
  ALL,
  RED,
  BLUE,
};

// Description of operation mode information
enum RunMode {
  DEFAULT_MODE,
  // Self-Scanning Mode
  SUP_SHOOT,
  // Talisman Mode
  ENERGY_AGENCY,
  // Hitting the sentry Mode
  SENTRY_STRIKE_MODE,
  // Little top mode
  TOP_MODE,
  // Record video Mode
  RECORD_MODE,
  // Plane Mode
  PLANE_MODE,
  // Sentrys autonomous mode
  SENTINEL_AUTONOMOUS_MODE,
  // Radar Mode
  RADAR_MODE,
};
// Describe the current robot ID information
enum RobotID {
  HERO = 1,
  UAV = 6,
  ENGINEERING,
  INFANTRY,
  SENTRY,
};

struct Receive_Data {
  int my_color;
  int now_run_mode;
  int my_robot_id;
  int bullet_velocity;

  // Description of the yaw axis angle of the gyroscope (signed)
  union Receive_Yaw_Angle_Information {
    float yaw_angle;
    uint8_t arr_yaw_angle[4] = {0};
  } Receive_Yaw_Angle_Info;

  union Receive_Yaw_Velocity_Information {
    float yaw_veloctiy;
    uint8_t arr_yaw_velocity[4] = {0};
  } Receive_Yaw_Velocity_Info;

  // Description of the pitch axis angle of the gyroscope (signed)
  union Receive_Pitch_Angle_Information {
    float pitch_angle;
    uint8_t arr_pitch_angle[4] = {0};
  } Receive_Pitch_Angle_Info;

  union Receive_Pitch_Velocity_Information {
    float pitch_veloctiy;
    uint8_t arr_pitch_velocity[4] = {0};
  } Receive_Pitch_Velocity_Info;

  Receive_Data() {
    my_color = ALL;
    now_run_mode = SUP_SHOOT;
    my_robot_id = INFANTRY;
    bullet_velocity = 30;
    Receive_Yaw_Angle_Info.yaw_angle = 0.f;
    Receive_Yaw_Velocity_Info.yaw_veloctiy = 0.f;
    Receive_Pitch_Angle_Info.pitch_angle = 0.f;
    Receive_Pitch_Velocity_Info.pitch_veloctiy = 0.f;
  }
};

// Serial port message sending structure
struct Write_Data {
  int symbol_yaw;
  int symbol_pitch;
  int depth;

  // Whether the robot shold shoot, 1 for shoot for 1 ball, otherwises 0
  int is_shooting;

  // Whether the target is found, 1 for found, otherwise 0
  int data_type;

  float yaw_angle;
  float pitch_angle;

  Write_Data() {
    symbol_yaw = 0;
    symbol_pitch = 0;
    depth = 0;
    is_shooting = 0;
    data_type = 0;
    yaw_angle = 0.f;
    pitch_angle = 0.f;
  }
};

// 服务端类
class tcp_com {
 public:
  // 构造函数
  tcp_com() = default;
  explicit tcp_com(const int _port, const int _argc);
  explicit tcp_com(char** _argv, const int _argc,std::string _tcp_com_config);
  explicit tcp_com(std::string _tcp_com_config);
  // 析构函数
  ~tcp_com();

  // 初始化服务端
  int initServer();

  /**
   * @brief  接收函数
   * @return int 0:  break
   *             -1: continue
   */
  int recvData();

  // 发送函数
  int sendData();
  void sendData(const int&     _yaw,
                           const int16_t& yaw,
                           const int&     _pitch,
                           const int16_t& pitch,
                           const int16_t& depth,
                           const int&     data_type,
                           const int&     is_shooting);

  // reset 函数
  void resetEventfdBox();

 private:
  /* 初始化和配置参数 */
  // argc的大小
  int argc_size = 0;
  // 端口值
  int port = 0;
  // 服务端套接字
  int server_sock;

  // setsockopt相关
  int opt = 1;
  unsigned int opt_len = sizeof(opt);
  // 服务端地址族信息
  struct sockaddr_in server_addr;

  // 读事件的集合，包括监听socket和客户端连接上来的socket。
  fd_set readfdset;
  // readfdset中socket的最大值。
  int maxfd;
  // 临时事件集合
  fd_set tmpfdset;

  // select 返回值
  int infds;

  /* 连接部分 */
  // 客户端地址族信息
  struct sockaddr_in client_addr;
  // 客户端地址族信息长度
  socklen_t client_len = sizeof(client_addr);
  // 客户端套接字
  int client_sock;

  /* 发送和接收部分 */
  // 接受的返回值
  ssize_t isize;
  // 收发的结构体
  message_pack pack;
  Receive_Data receive_data_;
  Write_Data   write_data_;

  // 发送所需的文件描述符
  int eventfd_copy;
  std::vector<int> eventfd_box;
};

// 初始化与配置
// 默认模式和输入端口模式
// 接受数据
// 发送数据

};  // namespace network_com