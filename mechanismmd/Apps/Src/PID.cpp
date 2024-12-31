#include "PID.hpp"


namespace NITNC
{

int PID::PIDForward( double CurrentSpeed, double TargetSpeed )
{
	double p, i, d;

	diff[ 0 ] = diff[ 1 ];
	diff[ 1 ]= TargetSpeed - CurrentSpeed;//偏差を取
	integral += ( diff[ 1 ] + diff[ 0 ] ) / 2.0 * DeltaT;

	p = Kp * diff[ 1 ];
	i = Ki * integral;
	d = Kd * ( diff[ 1 ] - diff[ 0 ] ) / DeltaT;

	if( abs( p ) >= P_MAX )
	{
		p = P_MAX * PlusMinus( p );
	}

	if( abs( i ) >= I_MAX )
	{
		i = I_MAX * PlusMinus( i );
	}

	if( abs( d ) >= D_MAX )
	{
		d = D_MAX * PlusMinus( d );
	}

	return (p + i + d);

}
}
