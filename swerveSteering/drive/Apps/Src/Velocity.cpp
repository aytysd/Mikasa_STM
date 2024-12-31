/*
 * Velocity.cpp
 *
 *  Created on: Jul 17, 2022
 *      Author: ayato
 */

#include "Velocity.hpp"

namespace NITNC
{

void Velocity::Init( TIM_HandleTypeDef *htim )
{
	m_pEncoder = htim;
}
void Velocity::Implement( void )
{
	double velo = 0;
	double count = 0;

	if( m_pEncoder -> Instance -> CNT > 65535 / 2)
	{
		count = ( double )( 65535.0 - m_pEncoder -> Instance -> CNT ) * -1;
	}
	else
	{
		count = ( double )m_pEncoder -> Instance -> CNT;
	}

	m_pEncoder -> Instance -> CNT = 0;


	double diameter = 2.0 * M_PI * tire_radius;

	velo = diameter * ( count / ppr );
	velo /= dt;

	velo = CONVERT( velo );

	velo *= ( -1 );

	m_CurrentVelocity = velo;

}
}



