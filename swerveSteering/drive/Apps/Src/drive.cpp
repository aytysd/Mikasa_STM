/*
 * drive.cpp
 *
 *  Created on: 2022/06/12
 *      Author: aoi
 */


#include <drive.hpp>
#include "Instances.hpp"
#include "xprint.hpp"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef*hcan)
{
	NITNC::can.Receive();
}


namespace NITNC
{

void Drive::CANCallback( uint32_t CANID, uint8_t* p_Data )
{
	m_TargetSpeed = ( (p_Data[ 1 ] & 0b00111111) << 8)|(( p_Data[0] & 0b11111111) << 0);

	if( ( p_Data[ 1 ] & 0b01000000 ) != 0 )
	{
		m_TargetSpeed *= ( -1 );
	}

}

bool Drive::CANIDJudge( uint32_t CANID )
{
	if( can._rx_id == CANID )
	{
		return true;
	}
	else
	{
		return false;
	}
}



double g_TargetSpeed;
double g_speed;
double g_pid;
double g_pwm;

void Drive::Forward( double speed )
{

	double pid = 0;
	double pwm = m_CurrentPWM;
	double TargetSpeed = m_TargetSpeed;
//	TargetSpeed = -500;
//	xprint( speed, "speed:" );

	pwm = VELOCITY2PWM( abs( TargetSpeed ) ) * PlusMinus( TargetSpeed );
	pid = PIDForward(speed, TargetSpeed);
	pwm += pid;

	if( pwm > 800 )
	{
		pwm = 800;
	}
	if( pwm < -800 )
	{
		pwm = -800;
	}


	g_pwm = m_CurrentPWM = pwm;
	g_pid = m_pid = pid;
	g_speed = speed;
	g_TargetSpeed = TargetSpeed;

	pwm=-300;
	TargetSpeed=1;

	if( pwm > 0 )
	{
		CW( pwm );
	}
	else if( pwm < 0 )
	{
		CCW( pwm );
	}
	else if( TargetSpeed == 0 || pwm == 0 )
	{
		STOP();
	}


}

}
