/*
 * 7SegmentLED.cpp
 *
 *  Created on: Aug 3, 2022
 *      Author: ayato
 */

#include "7SegmentLED.hpp"

namespace NITNC
{

void SegmentLED::Init
(
	GPIO_TypeDef* PortA,
	uint16_t PinA,
	GPIO_TypeDef* PortB,
	uint16_t PinB,
	GPIO_TypeDef* PortC,
	uint16_t PinC,
	GPIO_TypeDef* PortD,
	uint16_t PinD,
	GPIO_TypeDef* PortE,
	uint16_t PinE,
	GPIO_TypeDef* PortF,
	uint16_t PinF,
	GPIO_TypeDef* PortG,
	uint16_t PinG
)
{
	m_p_PortA = PortA;
	m_PinA = PinA;
	m_p_PortB = PortB;
	m_PinB = PinB;
	m_p_PortC = PortC;
	m_PinC = PinC;
	m_p_PortD = PortD;
	m_PinD = PinD;
	m_p_PortE = PortE;
	m_PinE = PinE;
	m_p_PortF = PortF;
	m_PinF = PinF;
	m_p_PortG = PortG;
	m_PinG = PinG;

	MakeLampLamda( LEDs[ A ], m_p_PortA, m_PinA );
	MakeLampLamda( LEDs[ B ], m_p_PortB, m_PinB );
	MakeLampLamda( LEDs[ C ], m_p_PortC, m_PinC );
	MakeLampLamda( LEDs[ D ], m_p_PortD, m_PinD );
	MakeLampLamda( LEDs[ E ], m_p_PortE, m_PinE );
	MakeLampLamda( LEDs[ F ], m_p_PortF, m_PinF );
	MakeLampLamda( LEDs[ G ], m_p_PortG, m_PinG );

}

void SegmentLED::Forward( uint8_t num )
{

}

void SegmentLED::MakeLampLamda( Lamp& lamp, GPIO_TypeDef* Port, uint16_t Pin )
{
	lamp = [ Port, Pin ]( bool On )
	{
		HAL_GPIO_WritePin( Port, Pin, static_cast<GPIO_PinState>( On ) );
	};
}

void SegmentLED::One( void )
{
	LEDs[ B ]( true );
	LEDs[ C ]( true );

	LEDs[ A ]( false );
	LEDs[ D ]( false );
	LEDs[ E ]( false );
	LEDs[ F ]( false );
	LEDs[ G ]( false );

}

void SegmentLED::Two( void )
{
	LEDs[ A ]( true );
	LEDs[ B ]( true );
	LEDs[ G ]( true );
	LEDs[ E ]( true );
	LEDs[ D ]( true );

	LEDs[ F ]( false );
	LEDs[ C ]( false );
}

};


