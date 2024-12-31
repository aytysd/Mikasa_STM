/*
 * mainpp.cpp
 *
 *  Created on: 2022/05/25
 *      Author: aoi
 */
#include "drive.hpp"
#include "tim.h"
#include "mainpp.h"


float angle;
float angle_speed;
int32_t count;
int16_t past_count;

drive drive;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

//	if(TIM2->CNT>2147483647) count =(int32_t)(429497295-TIM2->CNT)*-1;
//		  else				  count=(int32_t)TIM2->CNT;
	count = TIM2 -> CNT * ( -1 );
	angle_speed=2.0 * 25.0 * 3.14 *static_cast<double>(past_count-count) / 0.01 / 8192.0;
	angle = static_cast<double>( count )/ 8192.0 * 43.0 * 360.0 / 105.0;
	drive.turn(angle,angle_speed);
	past_count=count;
}


void mainpp()
{
	MyCAN::init();
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);

	while(1)
	{

	}


}

