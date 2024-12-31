/*
 * CommunicationWatchDog.hpp
 *
 *  Created on: Nov 1, 2022
 *      Author: ayato
 */

#ifndef INC_COMMUNICATIONWATCHDOG_HPP_
#define INC_COMMUNICATIONWATCHDOG_HPP_

#include "main.h"

namespace NITNC
{



class CommunicationWatchDog
{
private:

	const uint32_t ExpectedPollingRate = 10;

	const double ControlRate = 100.0;

	const uint32_t TorleranceInMsec = 5000;

	const uint32_t TorleranceCount = static_cast<double>( ExpectedPollingRate ) * ( static_cast<double>( TorleranceInMsec ) / 1000.0 );

	uint32_t m_WatchDogCounter = 0;
	uint32_t m_ExpectedCommunicationCounter = 0;

	void reset( void )
	{
		HAL_NVIC_SystemReset();
	}


public:

	CommunicationWatchDog()
	{

	}


	void Init( void )
	{
		m_WatchDogCounter = 0;
		m_ExpectedCommunicationCounter = 0;

	}

	void Increment( void )
	{
		m_WatchDogCounter++;

		if( m_WatchDogCounter >= ( ControlRate / static_cast<double>( ExpectedPollingRate ) ) + 1 )
		{
			m_WatchDogCounter = 0;
			m_ExpectedCommunicationCounter++;
		}

		if( m_ExpectedCommunicationCounter >= TorleranceCount )
		{
			reset();
		}
	}

	void IsReceived( void )
	{
		if( m_ExpectedCommunicationCounter >= 1 )
		{
			m_ExpectedCommunicationCounter--;
		}
	}

};
}



#endif /* INC_COMMUNICATIONWATCHDOG_HPP_ */
