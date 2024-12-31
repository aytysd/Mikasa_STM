#pragma once

#pragma once

#include "spi.h"
#include "usart.h"
#include <stdio.h>

#include <vector>
#include <functional>

#define CHIP_PER_ONE_SEG 7


namespace NITNC
{


class TapeLED
{
private:

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t index = 0;

	void SetRed( uint8_t r_ ){ r = r_; };
	void SetBlue( uint8_t b_ ){ b = b_; };
	void SetGreen( uint8_t g_ ){ g = g_; };
	void SetIndex( uint8_t index_ ){ index = index_; };

	uint8_t GetRed( void ){ return r; };
	uint8_t GetGreen( void ){ return g; };
	uint8_t GetBlue( void ){ return b; };
	uint8_t GetIndex( void ){ return index; };

public:
    static void ExtractRGBData( uint8_t data, uint8_t& r, uint8_t& g, uint8_t& b );
    void Drive( void );

    void CANCallback( uint32_t CANID, uint8_t* pData );
};



};

