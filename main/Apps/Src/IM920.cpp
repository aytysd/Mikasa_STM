	/*
 * IM920.cpp
 *
 *  Created on: Dec 12, 2021
 *      Author: ayato
 */
#include "usart.h"
#include "main.h"
#include "gpio.h"


#include <string>
#include <cstring>
#include <algorithm>

#include "IM920.hpp"
#include "myuart.hpp"
#include "utils.hpp"
#include "Instances.hpp"


namespace NITNC
{
char IM920::Rxdata_buff = '0';

void IM920::init( void )
{
	HAL_UART_Receive_IT( huart, ( uint8_t* )&IM920::Rxdata_buff, sizeof( IM920::Rxdata_buff ) );
	SetMyNode();
	SetMyChannel( false, 1 );

}

void IM920::wakeup( void )
{
	HAL_UART_Transmit(  huart, ( uint8_t* )WAKEUP, sizeof( WAKEUP ), 100 );
}

void IM920::ClearAll( void )
{
	std::string command = "PCLR";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::Transmit( uint8_t DestinationID, uint8_t* pData, uint8_t Size )
{
	L2( 1 );

	std::string DataInString;

	for( size_t i = 0; i < Size; i++ )
	{
		DataInString += Dec2HexString( pData[ i ], 2 );
	}

	std::string DestinationIDInString = Dec2HexString( DestinationID, 4 );

	std::string command = TRANS_MODE;
	command += DestinationIDInString;
	command += COMMA;
	command += DataInString;
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(  huart, ( uint8_t* )command.c_str(), command.size(), 100 );

	L2( 0 );
}



void IM920::ReadMyID( void )
{

	std::string command = "RDID";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::ReadMyNode( void )
{

	std::string command = "RDNN";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(  huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::ReadMyGroupNum( void )
{

	std::string command = "RDGN";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );


}


void IM920::SetMyNode( void )
{
	std::string command = "STNN";

	if( MyNodeID_ == "0001" )
	{
		EnableFlash();
	}

	command += MyNodeID_;
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(huart, ( uint8_t* )command.c_str(), command.size(), 100 );


	if( MyNodeID_ == "0001" )
	{
		DisableFlash();
	}


}

void IM920::SetRetryTimes( uint8_t RetryTimes )
{
	std::string command = "STTR";
	command += Dec2HexString( RetryTimes, 2 );
	command += END;

	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );
}

void IM920::ReadRSSI( void )
{
	std::string command = "RDRS";
	command += END;

	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );
}

size_t IM920::CarrierSense( void )
{
	for( int i = 1; i < 45; i++ )
	{
		if( i != 30 )
		{
			std::pair< int8_t, uint8_t > Carrier;
			Carrier.first = i;

			IsSensing_ = true;
			SensingTarget_ = i;

			ReadRSSI();

			while( IsSensing_ == true ){}

			Carrier.second = RSSIValue_;

			RSSIValues_.insert( Carrier );

			SetMyChannel( false, SensingTarget_ );

			HAL_Delay( 100 );
		}
	}

	uint8_t BestChannel = max_element( RSSIValues_.begin(), RSSIValues_.end(), compare )-> first;

	SetMyChannel( false, 1 );

	return BestChannel;
}

void IM920::ReadRetryTimes( void )
{
	std::string command = "RDTR";
	command += END;

	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::EnableFlash( void )
{
	std::string command = "ENWR";
	command += END;

	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit(huart, ( uint8_t* )command.c_str() ,command.size() , 100 );

}

void IM920::DisableFlash( void )
{
	std::string command = "DSWR";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(  huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}


void IM920::EstablishGroupRegistration( void )
{
	std::string command = "STGN";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}


void IM920::DisableSniffer( void )
{
	std::string command = "DSNF";
	command += END;

	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::SoftReset( void )
{
	std::string command = "SRST";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(  huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}


void IM920::ReadMyChannel( void )
{
	std::string command = "RDCH";
	command += END;

	while( !( IM920::GetIsBusy() ) );
	HAL_UART_Transmit(  huart, ( uint8_t* )command.c_str(), command.size(), 100 );

}

void IM920::ReadTN( void )
{
	std::string command = "RDTN";
	command += END;

	while( !( IM920::GetIsBusy() ) );


	HAL_UART_Transmit(huart, ( uint8_t* )command.c_str(), sizeof( command.c_str() ), 100);

}




void IM920::RegisterGroup( void )
{
	EnableFlash();
	DisableSniffer();

	EstablishGroupRegistration();

	if( MyNodeID_ != "0001" )
	{
		while( Rxdata != "GRNOREGD" )
		{
			if( HAL_GetTick() > GroupRegisteringTimeOut )
			{
				break;
			}
		};
	}
	else
	{
		while( HAL_GetTick() < GroupRegisteringTimeOut ){};
	}

	DisableFlash();
	SoftReset();

}

void IM920::SetMyChannel( bool NotifyPartner, uint8_t Channel, uint8_t DestinationID )
{

	if( NotifyPartner == true && DestinationID != -1 )
	{
		uint8_t msg[ 3 ] = { 255, 255, Channel };
		Transmit( DestinationID, msg, 3 );
	}

	std::string ChannelInString = Dec2HexString( Channel, 2 );

	std::string command = "STCH";
	command += ChannelInString;
	command += END;


	while( !( IM920::GetIsBusy() ) );

	HAL_UART_Transmit( huart, ( uint8_t* )command.c_str() , command.size(), 100 );


}






void IM920::ReceiveData( bool print )
{
    L2( 1 );
	HAL_UART_Receive_IT( huart, ( uint8_t* )&IM920::Rxdata_buff, sizeof( IM920::Rxdata_buff ) );


	if( Rxdata_buff != '\n' && Rxdata_buff != '\r' )
	{
		std::string s( 1, Rxdata_buff );
		Rxdata += s;
	}
	else
	{

		if( Rxdata.size() > 10 )
		{
			std::string Dummy = Rxdata.substr( 0, 2 );
			std::string Node = Rxdata.substr( 3, 4 );
			std::string RSSI = Rxdata.substr( 8, 2 );
			std::string Data = Rxdata.substr( 11 );

			while( Data.find( ',' ) != std::string::npos )
			{
				Data.erase( Data.find( ',' ), 1 );
			}

			std::vector<uint8_t> DataInUint;
			HexString2Dec( Data, DataInUint );

			std::vector<uint8_t> NodeInUint;
			HexString2Dec( Node, NodeInUint );

			std::vector<uint8_t> RSSIInUint;
			HexString2Dec( RSSI, RSSIInUint );

			int8_t RSSIInInt = ( int8_t )RSSIInUint[ 0 ];

			if( DataInUint[ 0 ] == 255 && DataInUint[ 1 ] == 255 )
			{
				SetMyChannel( false, DataInUint[ 2 ] );

				return;
			}

			if( Data.find( '\n' ) != std::string::npos )
				Data.erase( '\n' );

			for( size_t i = 0; i < CallbackFuncs_.IDs.size(); i++ )
			{
				if( NodeInUint[ 1 ] == CallbackFuncs_.IDs[ i ] )
				{
					IM920CallbackFunc CallbackFunc = CallbackFuncs_.CallbackFuncs[ i ];
					CallbackFunc( NodeInUint[ 1 ], DataInUint.data(), DataInUint.size() );
				}
			}

		}


		if
		(
			IsSensing_ == true &&
			Rxdata.find( "OK" ) == std::string::npos &&
			Rxdata.find( "NG" ) == std::string::npos
		)
		{
			std::vector< uint8_t > RSSI;
			HexString2Dec( Rxdata, RSSI );

			if( ( int8_t )RSSI[ 0 ] != 0 )
				RSSIValue_ = ( int8_t )RSSI[ 0 ];
			else
				RSSIValue_ = -120;

			xprint( RSSIValue_, "RSSI:" );
			xprint( SensingTarget_, "SensingTarget:" );

			IsSensing_ = false;

		}

		Rxdata += "\r\n";
		str_output( Rxdata );
		Rxdata = "";

	}

	L2( 0 );

}

void IM920::SetCallbackFunc( IM920CallbackFunc CallbackFunc, uint8_t ID )
{
	CallbackFuncs_.CallbackFuncs.push_back( CallbackFunc );
	CallbackFuncs_.IDs.push_back( ID );
}

void IM920Callback( UART_HandleTypeDef* huart )
{
	using namespace NITNC;
	if( huart == im920.huart )
	{
		im920.ReceiveData();
	}
}

void IM920::CANCallbackFunc( uint32_t CANID, uint8_t* Data )
{
	return;
}

bool IM920::CANIDJudgeFunc( uint32_t CANID )
{
	return true;
}

}

