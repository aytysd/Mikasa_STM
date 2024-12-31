/*
 * drive.hpp
 *
 *  Created on: 2022/06/12
 *      Author: aoi
 */

#ifndef INC_DRIVE_HPP_
#define INC_DRIVE_HPP_
#include "mycan.hpp"
#include "gpio.h"
#include "tim.h"
#include "math.h"

class drive
{
public:
	void turn(float target_angle,float angle_speed);
	float pid_sample(float angle,float angleobjective);

private:
	int pulas=1;

};

extern MyCAN can;

extern uint16_t received_target_angle;
extern uint16_t target_velo;

extern bool data_check;


#endif /* INC_DRIVE_HPP_ */
