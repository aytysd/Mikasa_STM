/*
 * drive.hpp
 *
 *  Created on: 2022/06/12
 *      Author: aoi
 */

#ifndef INC_DRIVE_HPP_
#define INC_DRIVE_HPP_
#include "gpio.h"
#include "tim.h"
#include "math.h"

#include "PID.hpp"
#include "mycan.hpp"
#include "PWM.hpp"

namespace NITNC
{

#define VELOCITY2PWM( x ) ( ( static_cast<double>( x ) / 8.5 ) + 24 )

class Drive : public PID, public PWM
{
private:

	double m_TargetSpeed = 0;

	double m_pid = 0;
	double m_CurrentPWM = 0;

public:

	double GetCurrentPWM( void ){ return m_CurrentPWM; }

	void Forward( double speed );

	void CANCallback( uint32_t CANID, uint8_t* p_Data );
	bool CANIDJudge( uint32_t CANID );

};

};


#endif /* INC_DRIVE_HPP_ */
