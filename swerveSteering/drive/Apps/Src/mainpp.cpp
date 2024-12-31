/*
 * mainpp.cpp
 *
 *  Created on: 2022/05/25
 *      Author: aoi
 */

#include "drive.hpp"
#include "tim.h"
#include "mainpp.h"
#include <stdio.h>
#include "Instances.hpp"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	ControlPeriodCallback( htim );
}


void ControlPeriodCallback( TIM_HandleTypeDef *htim )
{
	using namespace NITNC;

	velocity.Implement();
	drive.Forward( velocity.GetCurrentVelocity() );

}




void mainpp()
{


	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim2,  TIM_CHANNEL_ALL);

	NITNC::can.init();
	NITNC::velocity.Init( &htim2 );

	HAL_TIM_Base_Start_IT(&htim6);



	while(1)
	{

	}

}
