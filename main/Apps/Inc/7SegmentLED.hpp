/*
 * 7SegmentLED.hpp
 *
 *  Created on: Aug 3, 2022
 *      Author: ayato
 */

#ifndef INC_7SEGMENTLED_HPP_
#define INC_7SEGMENTLED_HPP_


#include "gpio.h"

#include <functional>
#include <vector>

namespace NITNC
{

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6

typedef std::function<void( bool )> Lamp;

class SegmentLED
{
private:

	GPIO_TypeDef* m_p_PortA;
	uint16_t m_PinA;
	GPIO_TypeDef* m_p_PortB;
	uint16_t m_PinB;
	GPIO_TypeDef* m_p_PortC;
	uint16_t m_PinC;
	GPIO_TypeDef* m_p_PortD;
	uint16_t m_PinD;
	GPIO_TypeDef* m_p_PortE;
	uint16_t m_PinE;
	GPIO_TypeDef* m_p_PortF;
	uint16_t m_PinF;
	GPIO_TypeDef* m_p_PortG;
	uint16_t m_PinG;

	std::vector<Lamp> LEDs;

	void One( void );
	void Two( void );
	void Three( void );
	void Four( void );
	void Five( void );
	void Six( void );
	void Seven( void );
	void Eight( void );
	void Nine( void );

public:
	SegmentLED(){};
	~SegmentLED(){};

	void Init
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
	);

	void MakeLampLamda( Lamp& lamp, GPIO_TypeDef* Port, uint16_t Pin );

	void Forward( uint8_t num );

};
};



#endif /* INC_7SEGMENTLED_HPP_ */
