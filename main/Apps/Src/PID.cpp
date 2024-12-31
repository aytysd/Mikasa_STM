/*
 * PID.cpp
 *
 *  Created on: Apr 19, 2022
 *      Author: ayato
 */



#include "PID.hpp"

#include <math.h>
#include "utils.hpp"


double PID::compute( double current_vector, double target_vector )
{
    return ( abs( max_thresh_ ) < abs( target_vector - current_vector ) ) ?
        P( current_vector, target_vector ) +
        I( current_vector, target_vector ) +
        D( current_vector, target_vector ) : 0;


}


double PID::P( double current_vector, double target_vector )
{
    double P = Kp_ * ( target_vector - current_vector );

    if( abs( P ) > p_clamp_max_ )
    {
        if( P < 0 )
            P = ( -1 ) * p_clamp_max_;
        else
            P = p_clamp_max_;
    }

    return P;
}

double PID::I( double current_vector, double target_vector )
{
	uint32_t time_dev = time_calc();

    static double integral;
    integral += ( target_vector - current_vector ) * ( ( double )time_dev / ( double )1000.0 );

    static bool Reverted = false;
    static bool PrevSignedFlag = false;
    bool SignedFlag = SIGNED( target_vector - current_vector );

    if( SignedFlag != PrevSignedFlag )
        integral = 0;

    PrevSignedFlag = SignedFlag;

    double I = Ki_ * integral;

    if( abs( integral ) > i_clamp_max_ )
    {
        if( integral < 0 )
            integral = ( -1 ) * i_clamp_max_;
        else
            integral = i_clamp_max_;
    }


    return I;

}

double PID::D( double current_vector, double target_vector )
{
    static double prev_dev;
    double current_dev = target_vector - current_vector;

    double D = Kd_ * ( current_dev - prev_dev );

    if( abs( D ) > d_clamp_max_ )
    {
        if( D < 0 )
            D = ( -1 ) * d_clamp_max_;
        else
            D = d_clamp_max_;
    }

    prev_dev = current_dev;

    return D;
}


