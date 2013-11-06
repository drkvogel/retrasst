#ifndef rosigH
#define rosigH
#include "rosebase.h"
#include "rosetta.h"
#include <string>
#include <vector>
//---------------------------------------------------------------------------
class ROSIG : public ROSETTA
{
private:
	bool	is_valid;
protected:
	std::vector<ROSE_BASE *> pairs_filter; // POINTERS TO CHECKED ELEMENTS
	const	ROSETTA	*target;
	ROSETTA_FILTER	filter_mask;
	bool identifyRos( const ROSETTA *r );
	bool identify( void );
public:
	ROSIG( const ROSETTA *data, ROSETTA_FILTER filter );
	~ROSIG( void );
	bool 	isValid( void ) const;
	std::string 	getSignature( void ) const;
	bool	verifySignature( const std::string sig ) const;
				/* FILTER FUNCTIONS FOR EXTERNAL USE */
	static	bool 	filter_null( const ROSE_BASE *b );
	static 	bool 	filter_underscore( const ROSE_BASE *b );
};
//---------------------------------------------------------------------------
#endif

