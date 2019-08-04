/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include <main.h>
#include <mainpp.h>
#include <ros.h>
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

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

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
  MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  sprintf(buff, "%d, %d, %d, %d, %d, %d\n\r", ax, ay, az, gx, gy, gz);
  str_msg.data = buff;
  chatter.publish(&str_msg);
  nh.spinOnce();
  HAL_Delay(1000);
}

#ifdef __cplusplus
}
#endif
