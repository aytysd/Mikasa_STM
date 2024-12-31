/*
 * PWM.cpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#include "PWM.hpp"


namespace NITNC
{

void PWM::CW( double pwm )
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(pwm));//test
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);

}

void PWM::CCW( double pwm )
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,abs(pwm));//test
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);


}

void PWM::STOP( void )
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);

}

}


