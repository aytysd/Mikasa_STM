#include "main.h"
#include "tim.h"


#include "stdio.h"
#include "math.h"

#include "Instances.hpp"
#include "Odometry.hpp"
#include "utils.hpp"
#include "Float2IEEE.hpp"


#define SINGLE ( false )

double yaw = 0;

void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
	using namespace NITNC;
	odom.PubSelfPos();

//	xprint( TIM1 -> CNT, "tim1:" );
//	xprint( TIM2 -> CNT, "tim2:" );
//	xprint( TIM3 -> CNT, "tim3:" );
//	xprint( TIM4 -> CNT, "tim4:" );

}


namespace NITNC
{
void Odometry::CalcOperation( TIM_TypeDef* x, TIM_TypeDef* y, double AttitudeAngle, Vector2_t& SelfPos, bool x_Inverted, bool y_Inverted )
{
	volatile double d1 = ODM_CIR * ( EncoderRead( x, x_Inverted ) / ODM_PPR );
	volatile double d2 = ODM_CIR * ( EncoderRead( y, y_Inverted ) / ODM_PPR );


	while( AttitudeAngle > 2 * M_PI )
	{
		AttitudeAngle -= 2 * M_PI;
	}

	SelfPos.x += d1 * cos( AttitudeAngle ) - d2 * sin( AttitudeAngle ); //X_coordinate
	SelfPos.y += d1 * sin( AttitudeAngle ) + d2 * cos( AttitudeAngle ); //Y_coordinate


}

void Odometry::GetAverage( Vector2_t& SelfPos1, Vector2_t& SelfPos2, Vector2_t& AveragedSelfPos )
{
	double x = ( SelfPos1.x + SelfPos2.x ) / static_cast<double>( 2 );
	double y = ( SelfPos1.y + SelfPos2.y ) / static_cast<double>( 2 );

	AveragedSelfPos.x = x;
	AveragedSelfPos.y = y;
}

void Odometry::PubSelfPos(void)
{

	Vector2_t SelfPos1;
#if !SINGLE
	Vector2_t SelfPos2;
#endif

#if !SINGLE
	SelfPos1.x = SelfPos2.x = SelfPosX;
	SelfPos1.y = SelfPos2.y = SelfPosY;
#else
	SelfPos1.x = SelfPosX;
	SelfPos1.y = SelfPosY;
#endif

	CalcOperation( TIM1, TIM2, m_AttitudeAngle, SelfPos1, true, false );
#if !SINGLE
	CalcOperation( TIM4, TIM3, m_AttitudeAngle, SelfPos2, true, false );

#endif

#if !SINGLE
	Vector2_t AveragedSelfPos;

	GetAverage( SelfPos1, SelfPos2, AveragedSelfPos );

	SelfPosX = AveragedSelfPos.x;
	SelfPosY = AveragedSelfPos.y;

	yaw = m_AttitudeAngle;

#else
	SelfPosX = SelfPos1.x;
	SelfPosY = SelfPos1.y;
#endif

	uint8_t msg[ 9 ] = { 0 };

	uint8_t IsDrivePowerOn = 0;

	if( drive_power_watcher.CheckGPIOControl() == POWERED_ON )
	{
		if( drive_power_watcher.CheckFinalPower() == POWERED_ON )
		{
			IsDrivePowerOn |= 0b10000000;
		}
		else
		{
			IsDrivePowerOn;
		}

		IsDrivePowerOn |= 0b00000001;
	}
	else
	{
		IsDrivePowerOn;
	}


	uint32_t AttitudeAngleDataInIEEE = Float2IEEE( m_AttitudeAngle );


	msg[ 0 ] = ( ( int16_t )round( SelfPosX ) & 0xFF00 ) >> 8;
	msg[ 1 ] = ( ( int16_t )round( SelfPosX ) &	 0x00FF );
	msg[ 2 ] = ( ( int16_t )round( SelfPosY ) & 0xFF00 ) >> 8;
	msg[ 3 ] = ( ( int16_t )round( SelfPosY ) & 0x00FF );
	msg[ 4 ] = ( static_cast<uint8_t>( AttitudeAngleDataInIEEE >> 24 ) );
	msg[ 5 ] = ( static_cast<uint8_t>( AttitudeAngleDataInIEEE >> 16 ) );
	msg[ 6 ] = ( static_cast<uint8_t>( AttitudeAngleDataInIEEE >> 8 ) );
	msg[ 7 ] = ( static_cast<uint8_t>( AttitudeAngleDataInIEEE >> 0 ) );
	msg[ 8 ] = IsDrivePowerOn;

	if( m_f_Start )
	{
		PublishToPC( msg, 9 );
	}

//		xprint( SelfPosX, "x:" );
//		xprint( SelfPosY, "y:" );




}


double Odometry::EncoderRead( TIM_TypeDef* TIM, bool Inverted )
{
	uint32_t EncoderValue = TIM -> CNT;

	double SignedEncoderValue = EncoderValue;
	if( EncoderValue >= TIM -> ARR / 2 )
	{
		SignedEncoderValue -= TIM -> ARR;
	}

	TIM -> CNT = 0;

	if( Inverted )
	{
		SignedEncoderValue *= ( -1 );
	}

//	if( TIM == TIM4 )
//	{
//		SignedEncoderValue *= ( -1 );
//		return SignedEncoderValue;
//	}
//	else
//	{
//		return SignedEncoderValue;
//	}

	return SignedEncoderValue;

}

void Odometry::Init( void )
{
	HAL_TIM_Encoder_Start( &htim1, TIM_CHANNEL_ALL );
    HAL_TIM_Encoder_Start( &htim2, TIM_CHANNEL_ALL );
    HAL_TIM_Encoder_Start( &htim3, TIM_CHANNEL_ALL );
	HAL_TIM_Encoder_Start( &htim4, TIM_CHANNEL_ALL );

//    HAL_TIM_Base_Start_IT( &htim6 );

	return;
}

bool Odometry::CANIDJudge( uint32_t CANID )
{
	if( ( CANID >> 5 ) == 1 )
	{
		if( ( CANID & 0b11111 ) == 31 )
		{
			return true;
		}
	}
	return false;
}

void Odometry::OdomReset( uint32_t CANID, uint8_t* pData )
{
	uint8_t check = pData[ 7 ];
	if( check != 127 )
	{
		int16_t t_SelfPosX = pData[ 0 ] << 8;
		t_SelfPosX |= pData[ 1 ];

		int16_t t_SelfPosY = pData[ 2 ] << 8;
		t_SelfPosY |= pData[ 3 ];

		m_SelfPosXOnInitialPoint = SelfPosX = t_SelfPosX;
		m_SelfPosYOnInitialPoint = SelfPosY = t_SelfPosY;

		m_f_Start = true;

//		bno055.SetIsResetNeeded( true );

	}

};

};

