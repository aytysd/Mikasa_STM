/*
 * PID.hpp
 *
 *  Created on: Apr 19, 2022
 *      Author: ayato
 */

#ifndef INC_PID_HPP_
#define INC_PID_HPP_

#pragma once

#define SIGNED( x ) ( ( x < 0 )? true : false )

class PID
{
protected:

    double Kp_;
    double Ki_;
    double Kd_;

    double p_clamp_max_;
    double i_clamp_max_;
    double d_clamp_max_;

    double max_thresh_;

    virtual double P( double current_vector, double target_vector );
    virtual double I( double current_vector, double target_vector );
    virtual double D( double current_vector, double target_vector );


public:
    PID
    (
        double Kp,
        double Ki,
        double Kd,
        double p_clamp_max,
        double i_clamp_max,
        double d_clamp_max,
        double max_thresh
    ):
        Kp_( Kp ),
        Ki_( Ki ),
        Kd_( Kd ),
        p_clamp_max_( p_clamp_max ),
        i_clamp_max_( i_clamp_max ),
        d_clamp_max_( d_clamp_max ),
        max_thresh_( max_thresh )
    {

    };

    virtual double compute( double current_vector, double target_vector );

    inline void set_gain( double p, double i, double d );



};



inline void PID::set_gain( double p, double i, double d )
{
    Kp_ = p;
    Ki_ = i;
    Kd_ = d;
}





#endif /* INC_PID_HPP_ */
