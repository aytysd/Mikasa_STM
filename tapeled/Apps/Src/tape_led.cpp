#include "tape_led.hpp"


namespace NITNC
{

void TapeLED::Drive( void )
{

	uint8_t index = GetIndex();
	uint8_t r = GetRed();
	uint8_t g = GetGreen();
	uint8_t b = GetBlue();

//	uint8_t index = 10;
//	uint8_t r = 3;
//	uint8_t g = 2;
//	uint8_t b = 0;

	uint8_t data[12*index];

	for (uint8_t i = 0; i < 4; i++)
	{
		data[i]   = (g >> i*2) & 0b11;
		data[i+4] = (r >> i*2) & 0b11;
		data[i+8] = (b >> i*2) & 0b11;
	}

	for (uint8_t i = 0; i < 12; i++)
	{
		switch (data[i])
		{
			case 0b11:
				data[i] = 0xEE;
				break;
			case 0b10:
				data[i] = 0xE8;
				break;
			case 0b01:
				data[i] = 0x8E;
				break;
			case 0b00:
				data[i] = 0x88;
				break;
			default:
				break;
		}
	}

	for (uint8_t i = 0; i < index; i++)
	{
		for (uint8_t j = 0; j < 12; j++)    data[j+12*i] = data[j];
	}

	HAL_SPI_Transmit(&hspi1, data, sizeof(data), 300);


}

void TapeLED::ExtractRGBData( uint8_t data, uint8_t& r, uint8_t& g, uint8_t& b )
{

}

void TapeLED::CANCallback( uint32_t CANID, uint8_t* pData )
{
	SetIndex( pData[ 0 ] );
	SetRed( pData[ 1 ] );
	SetGreen( pData[ 2 ] );
	SetBlue( pData[ 3 ] );

}

};



