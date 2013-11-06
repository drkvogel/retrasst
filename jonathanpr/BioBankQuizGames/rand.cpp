#include "rand.h"
#include <math.h>
/*---------------------------------------------------------------------------*/
namespace RANDOM
{
/*---------------------------------------------------------------------------*/
#define RAN2_IM1        2147483563
#define RAN2_IM2        2147483399
#define RAN2_AM         ( 1.0 / RAN2_IM1 )
#define RAN2_IMM1       ( RAN2_IM1 - 1 )
#define RAN2_IA1        40014
#define RAN2_IA2        40692
#define RAN2_IQ1        53668
#define RAN2_IQ2        52774
#define RAN2_IR1        12211
#define RAN2_IR2        3791
#define RAN2_NTAB       32
#define RAN2_NDIV       ( 1 + RAN2_IMM1 / RAN2_NTAB )
#define RAN2_EPS        1.2e-7
#define RAN2_RNMX       ( 1.0 - RAN2_EPS )
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
long	rand_seed = -1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RANDOM ALGORITHM #2  */
static double randNumRec2( long *idum )
{       long    j, k;
	static  long    iy = 0;
	static  long    idum2 = 123456789;
	static  long    iv[RAN2_NTAB];
	double  temp;
	if ( *idum <= 0 )
		{*idum = ( *idum > -1 ) ? 1 : -(*idum);
		idum2 = *idum;
		for ( j = RAN2_NTAB + 7; j >= 0; j-- )
			{k = (*idum ) / RAN2_IQ1;
			*idum = RAN2_IA1 * ( *idum - k * RAN2_IQ1 )
				- k * RAN2_IR1;
			if ( *idum < 0 )
				{*idum += RAN2_IM1;
				}
			if ( j < RAN2_NTAB )
				{iv[j] = *idum;
				}
			}
		iy = iv[0];
		}
	k = (*idum) / RAN2_IQ1;
	*idum = RAN2_IA1 * ( *idum - k * RAN2_IQ1 ) - k * RAN2_IR1;
	if ( *idum < 0 )
		{*idum += RAN2_IM1;
		}
	k = idum2 / RAN2_IQ2;
	idum2 = RAN2_IA2 * ( idum2 - k * RAN2_IQ2 ) - k * RAN2_IR2;
	if ( idum2 < 0 )
		{idum2 += RAN2_IM2;
		}
	j = iy / RAN2_NDIV;
	iy = iv[j] - idum2;
	iv[j] = *idum;
	if ( iy < 1 )
		{iy += RAN2_IMM1;
		}
	return( ( ( temp = RAN2_AM * iy ) > RAN2_RNMX ) ? RAN2_RNMX : temp );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef  RAN2_IM1
#undef  RAN2_IM2
#undef  RAN2_AM
#undef  RAN2_IMM1
#undef  RAN2_IA1
#undef  RAN2_IA2
#undef  RAN2_IQ1
#undef  RAN2_IQ2
#undef  RAN2_IR1
#undef  RAN2_IR2
#undef  RAN2_NTAB
#undef  RAN2_NDIV
#undef  RAN2_EPS
#undef  RAN2_RNMX
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void randSetSeed( const long seed )
{
	rand_seed = ( 0 == seed ) ? -1
		:
		( seed < 0 ) ? seed : (-seed);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
long getRandSeed( void )
{
	return( rand_seed );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* RETURN UNIFORM RANDOM DEVIATE ON [0,1) INTERVAL */
double getRand01( void )
{
	return( randNumRec2( &rand_seed ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* RETURN UNIFORM RANDOM INTEGER ON [0,n) INTERVAL */
int getRand0n( const int n )
{
	double	r = getRand01() * ((double) n);
	int	rn = (int) floor( r );
	while ( rn < 0 )
		{rn += n;
		}
	return( rn % n );
}
//---------------------------------------------------------------------------
};							// END OF NAMESPACE
//---------------------------------------------------------------------------

