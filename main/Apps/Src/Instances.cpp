/*
 * Instances.cpp
 *
 *  Created on: Sep 16, 2022
 *      Author: ayato
 */


#include "Instances.hpp"

namespace NITNC
{
DrivePowerWatcher drive_power_watcher;

MyCAN can1( &hcan1 );
MyCAN can2( &hcan2 );

Odometry odom;

IM920 im920( 1, &huart2, GPIOD, GPIO_PIN_14 );

BNO055 bno055( &hi2c1 );

BNO055UART bno055_uart3( &huart3, GPIOE, GPIO_PIN_12 );
BNO055UART bno055_uart2( &huart2, GPIOC, GPIO_PIN_9 );

BNO055UART* bno055_uart = &bno055_uart3;
};
