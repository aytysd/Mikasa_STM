/*
 * Velocity.hpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#ifndef INC_VELOCITY_HPP_
#define INC_VELOCITY_HPP_


#pragma once

#include "math.h"

#include "tim.h"

namespace NITNC
{

class Velocity
{
private:

	const double tire_radius = 30.0;
	const double dt = 0.01;
	const double ppr = 8192.0;

	double m_CurrentVelocity;

	TIM_HandleTypeDef* m_pEncoder;

public:

	Velocity(){}
	~Velocity(){}

	void Init( TIM_HandleTypeDef *htim );
	void Implement( void );

	double GetCurrentVelocity( void ){ return m_CurrentVelocity; }

};
};


#endif /* INC_VELOCITY_HPP_ */
