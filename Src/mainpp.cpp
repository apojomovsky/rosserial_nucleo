/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include <main.h>
#include <mainpp.h>
#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <string.h>
#include <I2Cdev.h>
#include <MPU6050.h>

ros::NodeHandle nh;

//TIM_HandleTypeDef htim2;
//UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;

int16_t ax, ay, az;
int16_t gx, gy, gz;

char buff[50];

sensor_msgs::Imu imu_msg;
std_msgs::String string_msg;
ros::Publisher chatter("alexis", &imu_msg);
//ros::Publisher chatter("chatter", &string_msg);



void ToQuaternion(geometry_msgs::Quaternion* quat, int16_t yaw, int16_t pitch, int16_t roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    quat->w = cy * cp * cr + sy * sp * sr;
    quat->x = cy * cp * sr - sy * sp * cr;
    quat->y = sy * cp * sr + cy * sp * cr;
    quat->z = sy * cp * cr - cy * sp * sr;
}

// Only set once
//imu_msg.linear_acceleration_covariance[0] = -1;
//imu_msg.orientation_covariance[0] = -1;
//imu_msg.angular_velocity_covariance[0] = -1;

#ifdef __cplusplus
 extern "C" {
#endif

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}

void setup(void)
{
  nh.initNode();
  nh.advertise(chatter);
  I2Cdev_init(&hi2c1);
  MPU6050_setAddress(0x68);
  MPU6050_initialize();
}

void loop(void)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
//  MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Updated on every step
//  imu_msg.angular_velocity.x = 0;
//  imu_msg.angular_velocity.y = 0;
//  imu_msg.angular_velocity.z = 0;
//  imu_msg.linear_acceleration.x = ax;
//  imu_msg.linear_acceleration.y = ay;
//  imu_msg.linear_acceleration.z = az;
//  ToQuaternion(&imu_msg.orientation, gz, gy, gx);
  chatter.publish(&imu_msg);
//  string_msg.data = "Hola!";
//  chatter.publish(&string_msg);
  nh.spinOnce();
  HAL_Delay(200);
}

#ifdef __cplusplus
}
#endif
