/*
 * drive.cpp
 *
 *  Created on: 2022/06/12
 *      Author: aoi
 */


#include <Drive.hpp>
#include "Instances.hpp"
#include "xprint.hpp"


namespace NITNC
{

void Drive::CANCallback( uint32_t CANID, uint8_t* p_Data )
{
	m_TargetSpeed = ( (p_Data[ 0 ] & 0b01111111) << 8)|(( p_Data[ 1 ] & 0b11111111) << 0);

	if( ( p_Data[ 0 ] & 0b10000000 ) != 0 )
	{
		m_TargetSpeed *= ( -1 );
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
//	TargetSpeed = 15000;
//	xprint( speed, "speed:" );

	pid = PIDForward(speed, TargetSpeed);
	pwm = pid;


	if( pwm > PWM_MAX )
	{
		pwm = PWM_MAX;
	}
	else if( pwm < PWM_MAX * ( -1 ) )
	{
		pwm = PWM_MAX * ( -1 );
	}


	g_pwm = m_CurrentPWM = pwm;
	g_pid = m_pid = pid;
	g_speed = speed;
	g_TargetSpeed = TargetSpeed;
//
//	pwm=-200;
//	TargetSpeed=1;

	if( TargetSpeed == 0 )
	{
		CW( 0 );
		g_pwm = m_CurrentPWM = 0;
		g_pid = m_pid = 0;

	}
	else
	{
		if( pwm > 0 )
		{
			CW( pwm );
		}
		else if( pwm < 0 )
		{
			CCW( pwm );
		}
		else if( pwm == 0 )
		{
			CW( 0 );
		}

	}



}

}
