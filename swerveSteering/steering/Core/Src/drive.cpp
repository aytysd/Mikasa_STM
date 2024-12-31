/*
 * drive.cpp
 *
 *  Created on: 2022/06/12
 *      Author: aoi
 */


#include "drive.hpp"
#include "tim.h"
#include "can.h"
MyCAN can(&hcan,31);

uint16_t received_target_angle;
uint16_t target_velo;

int pwm;

bool data_check=false;
int tuen_count;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef*hcan)
{

		can.receive();
//		data = can.getData();//can get target_angle
//		received_target_angle = ((data[1] & 0b1) << 8)|((data[0] & 0b11111111) << 0);
//
//		target_velo=(data[2] << 7)|((data[1] & 0b11111110) >> 1);
//		data_check=true;
}

float p, i, d;
float diff_1=1;
float diff_2=0;
float integral=0;

#define DELTA_T 0.002
#define KP 0.8 //定数を代入
#define KI 0
#define KD 0

#define SIGNED( x ) ( ( x < 0 ) ? true : false )

int power_on=1;
bool iscalmdown = false;
int past_pwm;

bool power_check=false;
float drive::pid_sample(float angle,float angleobjective)
{

	static bool Signed = false;

	diff_1=diff_2;
	diff_2=angleobjective-angle;//偏差を取
	integral += ( diff_2 + diff_1 ) / 2.0 * DELTA_T;

//	if( SIGNED( diff_2 ) != Signed )
//	{
//		integral = 0;
//	}

	Signed = SIGNED( diff_2 );

	p = KP * diff_2;
	i = KI * integral;
	d = 0;

	if( abs( diff_2 ) < 5 )
	{
		d = KD * (diff_2-diff_1) /  ( 2 * DELTA_T );
	}

	return (p + i - d);
}
double faute;
double target_angle;
void drive::turn(float angle,float angle_speed)
{

//	received_target_angle = 45;

	double AngleCandidates[ 8 ];// = { 0 };

	AngleCandidates[ 0 ] = ( double )received_target_angle;
	AngleCandidates[ 1 ] = ( double )received_target_angle + 180;
	AngleCandidates[ 2 ] = ( double )received_target_angle + 180 + 180;
	AngleCandidates[ 3 ] = ( double )received_target_angle + 180 + 180 + 180;
	AngleCandidates[ 4 ] = ( double )received_target_angle - 180;
	AngleCandidates[ 5 ] = ( double )received_target_angle - 180 - 180;
	AngleCandidates[ 6 ] = ( double )received_target_angle - 180 - 180 - 180;
	AngleCandidates[ 7 ] = ( double )received_target_angle - 180 - 180 - 180 - 180;

	if( AngleCandidates[ 3 ] > 720 )
	{
		AngleCandidates[ 3 ] -= 1440;
	}

	if( AngleCandidates[ 7 ] < -720 )
	{
		AngleCandidates[ 7 ] += 1440;
	}


	uint8_t transmit_velo[2];

	float pid_angle;
	uint8_t invert_velocity = 0;

	//角度の最短距離

	target_angle = AngleCandidates[ 0 ];

//	target_angle=45;

	for( int i = 1; i < 8; i++ )
	{
		if
		(
			abs( target_angle - angle ) > 
			abs( AngleCandidates[ i ] - angle )
		)
		{
			target_angle = AngleCandidates[ i ];
		}
	}

	if
	(
		static_cast<int>( abs( target_angle - received_target_angle ) ) == 360 ||
		static_cast<int>( abs( target_angle - received_target_angle ) ) == 720 ||
		static_cast<int>( abs( target_angle - received_target_angle ) ) == 0
	)
	{
		invert_velocity = 1;
	}
	else
	{
		invert_velocity = 0;
	}

	if((past_pwm>0)&&(pwm<0))
	{
		integral=0;
	}
	else if ((past_pwm<0)&&(pwm>0))
	{
		integral=0;
	}
	pid_angle = pid_sample(angle, (float)target_angle);
	pwm=pid_angle*44;

	faute=(double)target_angle-angle;

	double AttitudeAngleTolerance = 0.2;

	if( target_velo == 0 )
	{
		AttitudeAngleTolerance = 5;
	}

	if(fabs(faute)<AttitudeAngleTolerance)
	{
		pwm=0;
		integral=0;
		iscalmdown = true;//収束した
	}
	else
	{
		iscalmdown = false;//しゅうそくしてない
	}

//	if(target_velo==0)
//	{
//		if(fabs(faute)<0.2)
//		{
//			pwm=0;
//			integral=0;
//			iscalmdown = true;//収束した
//		}
//		else
//		{
//			iscalmdown = false;//しゅうそくしてない
//		}
//	}

	if( angle_speed == 0 )
	{
		if( iscalmdown == true )
		{
			pwm = 0;
		}
		else
		{
			if( target_angle-angle > 0 )
			{
				pwm = 150;
			}
			else if(target_angle-angle < 0 )
			{
				pwm=-150;
			}
		}
		diff_1=1;
		diff_2=0;
		integral=0;
	}

//
//	pwm = 200;

	if(tuen_count>10)
	{
		pwm=0;
	}

	if(pwm>0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,abs(pwm));
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	}

	else if(pwm < 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,abs(pwm));
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
	}

	else
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	}

	if(data_check==true)
	{
		invert_velocity*=-1;
		transmit_velo[0]=(target_velo & 0xFF)<<0;
		transmit_velo[1]=((invert_velocity & 0b1)<<6)|((target_velo & 0x1F00)>>8);
		can.transmit(transmit_velo,sizeof(transmit_velo));
		data_check=false;
	}

	past_pwm=pwm;
	
}
