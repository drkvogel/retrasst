/*===========================================================================*/
/*			RANDOM NUMBER GENERATOR			*/
/*===========================================================================*/
#include <time.h>
#include <math.h>
#include "xrand.h"
/*===========================================================================*/
bool	XRAND::seeded = false;
long	XRAND::seed = -1;
/*---------------------------------------------------------------------------*/
XRAND::XRAND( void )
{
	if ( ! seeded)	// SHARE SINGLE SEED BETWEEN ALL COPIES OF OBJECT
		{seed = (long) time( NULL );
		seeded = true;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XRAND::~XRAND( void )
{
}
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
/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
				/* RANDOM ALGORITHM #2 FROM NUMERICAL RECIPES */
double XRAND::nrRan2( long *idum )
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
/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
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
double XRAND::getRandom01( void )
{
	return( nrRan2( &seed ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XRAND::getRandom0N( const int n )
{
	const	double	r = getRandom01();
	return( (int) floor( r * (double) n ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XRAND::getBiasedRandom0N( const int n, const std::vector<int> *bias )
{			// bias=[1,2,3] returns 3 times more "2"s than "0"s
	if ( n < 2 || NULL == bias || n != (int) bias->size() )
		{return( -1 );
		}
	int	i;
	int	tot = 0;
	for ( i = 0; i < n; i++ )
		{if ( (*bias)[i] < 1 || (*bias)[i] > 10000 )
			{return( -2 );
			}
		tot += (*bias)[i];
		}
	const	double	r = getRandom01();
	const	int	nr = (int) floor( r * (double) tot );
	int	cum = 0;
	for ( i = 0; i < (n-1); i++ )
		{cum += (*bias)[i];
		if ( nr < cum )
			{return( i );
			}
		}
	return( n-1 );
}
/*===========================================================================*/

