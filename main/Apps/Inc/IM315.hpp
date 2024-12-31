#pragma once

#ifndef INC_IM315_HPP_
#define INC_IM315_HPP_

#include "main.h"
#include "gpio.h"


#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>

#include "utils.hpp"


//#define CONV_SIGNED( x ) ( ( x >= 128 )? )

#define TRANS_MODE "TXDT"
#define BRANK ""
#define COMMA ','
#define END "\r\n"
#define COL ":"
#define WAKEUP "?"

#define CHANGE_CHANNEL "CHCH"

#define GroupRegisteringTimeOut ( 10 * 1000 )

typedef std::function<void( uint8_t, uint8_t*, uint8_t )> IM315CallbackFunc;

typedef struct StructIM315CallbackFuncs_t
{
	std::vector<IM315CallbackFunc> CallbackFuncs;
	std::vector<uint8_t> IDs;
} IM315CallbackFuncs_t;


class IM315
{
public:

	IM315
    (
			uint8_t MyNodeID,
			UART_HandleTypeDef* huart,
			GPIO_TypeDef* STSPort,
			uint16_t STSPin
	):
	huart( huart ),
	STSPort_( STSPort ),
	STSPin_( STSPin )
	{
		MyNodeID_ = Dec2HexString( MyNodeID, 4 );


		if( MyNodeID_ == "0001" )
		{
			Parent_ = true;
		}
	};

	void init();
	void ClearAll( void );

	void Transmit( uint8_t DestinationID, uint8_t* pData, uint8_t Size );


	void ReadMyNode( void );
	void ReadMyID( void );
	void ReadMyGroupNum( void );
	void ReadMyChannel( void );
	void ReadRSSI( void );
	void ReadMyVersion( void );
	void ReadReceiveID( void );

	void EraseReceiveID( void );

	void SetReceiveID( std::string ID );

	void SetMyChannel( bool NotifyPartner, uint8_t Channel, uint8_t DestinationID = -1 );

	void RegisterGroup( void );

	void SoftReset( void );

	void ReceiveData( bool print = false );

	size_t CarrierSense( void );

	std::string Rxdata;
	static char Rxdata_buff;

	UART_HandleTypeDef* huart;

	bool CANIDJudgeFunc( uint32_t CANID );
	void CANCallbackFunc( uint32_t CANID, uint8_t* Data );

private:

	void SetCallbackFunc( IM315CallbackFunc CallbackFunc, uint8_t ID );

	void SetMyNode( void );

	void EnableFlash( void );
	void DisableFlash( void );

	inline bool GetIsBusy( void );


	std::string MyNodeID_;
	bool Parent_;

	bool IsSensing_;

	GPIO_TypeDef* STSPort_;
	uint16_t STSPin_;

	IM315CallbackFuncs_t CallbackFuncs_;

	bool ChannelChangeRequiredFlag_ = false;
	bool IsInitialized_ = false;

	int8_t RSSIValue_;
	std::map< int8_t, uint8_t > RSSIValues_;

	uint8_t SensingTarget_ = 0;

};




inline bool IM315::GetIsBusy( void )
{
	if( HAL_GPIO_ReadPin( STSPort_, STSPin_ ) == GPIO_PIN_SET )
	{
		str_output( "wait\r\n" );
//		 HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_SET );
		return false;
	}
	else
	{
//		  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5, GPIO_PIN_RESET );
		return true;
	}
}

extern IM315 im315;


#endif /* INC_IM315_HPP_ */