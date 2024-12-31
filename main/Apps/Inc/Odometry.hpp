#pragma once

#include "main.h"
#include "math.h"

namespace NITNC
{
#define ODM_RADIUS ( 50.8 / 2 )
#define ODM_PPR 8192.0f
#define ODM_CIR ( 2.0 * ODM_RADIUS * M_PI )

typedef struct Vector2
{
	double x = 0;
	double y = 0;
} Vector2_t;

class Odometry
{
public:
	void PubSelfPos(void);

	void Init( void );

	Odometry()
	{}

	bool CANIDJudge( uint32_t CANID );
	void OdomReset( uint32_t CANID, uint8_t* pData );
	void OdomResetOnSW( void )
	{
		SelfPosX = m_SelfPosXOnInitialPoint;
		SelfPosY = m_SelfPosYOnInitialPoint;
	};

	void SetAttitudeAngle( double AttitudeAngle ) { m_AttitudeAngle = AttitudeAngle; };

private:

	void GetAverage( Vector2_t& SelfPos1, Vector2_t& SelfPos2, Vector2_t& AveragedSelfPos );

	void CalcOperation( TIM_TypeDef* x, TIM_TypeDef* y, double AttitudeAngle, Vector2_t& SelfPos, bool x_Inverted = false, bool y_Inverted = false );

	double EncoderRead( TIM_TypeDef* TIM, bool Inverted = false );

//	int16_t SelfPosX = 9565;
	volatile double SelfPosX = 840;
	volatile double SelfPosY = 840;

	double m_SelfPosXOnInitialPoint = SelfPosX;
	double m_SelfPosYOnInitialPoint = SelfPosY;

	double m_AttitudeAngle = 0;

	bool m_f_Start = false;

};

};



