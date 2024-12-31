#include "usart.h"
#include "main.h"

#include <string>

#include "SBDBT.hpp"
#include "myuart.hpp"
#include "utils.hpp"

SBDBT sbdbt( &huart3 );

uint8_t SBDBT::RxdataBuff_ = 0;

void SBDBTCallback( UART_HandleTypeDef* huart )
{
	if( sbdbt.huart_ == huart )
	{
		sbdbt.ReceiveData();
	}
}

void SBDBT::Init( void )
{
	HAL_UART_Receive_IT( huart_, ( uint8_t* )&SBDBT::RxdataBuff_, sizeof( sbdbt.RxdataBuff_ ) );

	while( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_8 ) ){}
}

void SBDBT::Transmit( uint8_t* pData, uint8_t Size )
{
	for( int i = 0; i < Size; i++ )
	{
		HAL_UART_Transmit( huart_, ( uint8_t* )&pData[ i ], 1, 100 );
	}

	uint8_t EndOfFrame = EoF;
	HAL_UART_Transmit(huart_, ( uint8_t* )&EndOfFrame, 1, 100 );

}


void SBDBT::ReceiveData( void )
{
	HAL_UART_Receive_IT(huart_, ( uint8_t* )&SBDBT::RxdataBuff_, sizeof( sbdbt.RxdataBuff_ ) );

	if( RxdataBuff_ != EoF )
	{
		Rxdata_.push_back( RxdataBuff_ );
	}
	else
	{
		uint8_t ID = Rxdata_[ 0 ];
		Rxdata_.erase( Rxdata_.begin() );
		uint8_t* Data = Rxdata_.data();
		uint8_t Size = Rxdata_.size();

		xprint( ID, "ID:" );
		xprint( Data[ 0 ], "data:" );

		for( size_t i = 0; i < CallbackFuncs_.IDs.size(); i++ )
		{
			if( ID == CallbackFuncs_.IDs[ i ] )
			{
				SBDBTCallbackFunc CallbackFunc = CallbackFuncs_.CallbackFuncs[ i ];
				CallbackFunc( ID, Data, Size );
			}
		}

	}
}

void SBDBT::SetCallbackFunc( SBDBTCallbackFunc CallbackFunc, uint8_t ID )
{
	CallbackFuncs_.CallbackFuncs.push_back( CallbackFunc );
	CallbackFuncs_.IDs.push_back( ID );
}
