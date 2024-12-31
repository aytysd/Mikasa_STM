/*
 * mainpp.cpp
 *
 *  Created on: 2022/05/25
 *      Author: aoi
 */

#include "tim.h"
#include "can.h"

#include "mainpp.h"
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

//	can.Increment();

}




void mainpp()
{

	using namespace NITNC;

	drive.Init();
	can.init( &hcan );
	velocity.Init( &htim2 );

	HAL_TIM_Base_Start_IT( &htim6 );


	while(1)
	{
//		HAL_Delay( 10000 );
//
//		MX_CAN_Init();
//		can.init( &hcan );

	}

}
