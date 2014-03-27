#ifndef XRAND_H_INCLUDED
#define XRAND_H_INCLUDED
/*===========================================================================*/
#include <vector>
/*===========================================================================*/
class XRAND
{
private:
	static	bool	seeded;
	static	long	seed;
	double 	nrRan2( long *idum );
public:
	XRAND( void );
	~XRAND( void );
	double	getRandom01( void );
	int	getRandom0N( const int n );
	int 	getBiasedRandom0N( const int n, const std::vector<int> *bias );
};
/*===========================================================================*/
#endif
