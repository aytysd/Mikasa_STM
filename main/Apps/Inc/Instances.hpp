/*
 * Instances.hpp
 *
 *  Created on: Sep 16, 2022
 *      Author: ayato
 */

#ifndef INC_INSTANCES_HPP_
#define INC_INSTANCES_HPP_

#include "DrivePowerWatcher.hpp"
#include "mycan.hpp"
#include "Odometry.hpp"
#include "IM920.hpp"
#include "bno055.hpp"
#include "bno055_uart.hpp"

namespace NITNC
{
extern DrivePowerWatcher drive_power_watcher;

extern MyCAN can1;
extern MyCAN can2;

extern Odometry odom;

extern IM920 im920;

extern BNO055 bno055;

extern BNO055UART bno055_uart3;
extern BNO055UART bno055_uart2;

extern BNO055UART* bno055_uart;

};


#endif /* INC_INSTANCES_HPP_ */
