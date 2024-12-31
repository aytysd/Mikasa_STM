/*
 * CallbackBase.hpp
 *
 *  Created on: Sep 4, 2022
 *      Author: ayato
 */

#ifndef INC_CALLBACKBASE_HPP_
#define INC_CALLBACKBASE_HPP_


#pragma once

#include <vector>

namespace NITNC
{

template < typename Callback_t, typename Activator_t >
class CallbackBase
{

private:

    std::vector<Callback_t> m_Callbacks;
    std::vector<Activator_t> m_Activators;
    std::vector<bool> m_Enabled;
    Activator_t m_Deactivator;

    void operator += ( const Callback_t& Func )
    {
        m_Callbacks.push_back( Func );
    };

    void operator += ( const Activator_t Activator )
    {
        m_Activators.push_back( Activator );
    };


public:

    CallbackBase(){};
    CallbackBase( Activator_t deactivator ) : m_Deactivator( deactivator ){};
    ~CallbackBase(){};

    void ResetCallbacks( void )
    {
        m_Callbacks.clear();
        m_Activators.clear();
        m_Enabled.clear();
    };

    size_t GetCallbackSize( void ){ return m_Callbacks.size(); };

    Callback_t& GetCallback( size_t index ){ return m_Callbacks[ index ]; };
    Activator_t& GetActivator( size_t index )
    {
        if( m_Enabled[ index ] )
        {
            return m_Activators[ index ];
        }
        else
        {
            return m_Deactivator;
        }
    };

    void RemoveCallback( size_t index )
    {
        m_Enabled[ index ] = false;
    };


    void SetCallback( const Activator_t Activator, const Callback_t& Function )
    {
        *this += Function;
        *this += Activator;
        m_Enabled.push_back( true );
    };

    virtual void ActivateCallback(){};

};


};



#endif /* INC_CALLBACKBASE_HPP_ */
