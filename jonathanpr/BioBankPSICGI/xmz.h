#ifndef XMZ_H_INCLUDED
#define XMZ_H_INCLUDED
/*===========================================================================*/
#include <string>
#include <vector>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include "xerror.h"
#include "xrand.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
class	XDB;
class	XMZ;
/*===========================================================================*/

class XMZ_VAR : public XERROR
{
friend class XMZ;
private:
	int	role;
	const	std::string	field;   // NAME OF DATABASE FIELD HOLDING VARIABLE
	const	std::string	label;
	const	int	nvals;		 // NUMBER OF VALUES TAKEN IN COL
	std::vector<int>	past;
protected:
	XMZ_VAR( const std::string title, const int nvalues,
		const std::string db_field );
	~XMZ_VAR( void );
public:
	int	getNVals( void ) const;
	bool 	setRole( const int r );
	int	getRole( void ) const;
	bool 	addPast( const int past_value );
	std::string	getTitle( void ) const;
	std::string 	getFieldName( void ) const;
	static	const	int	Out = 0; // OUTPUT COLUMN HOLDING TREATMENT
	static	const	int	Dat = 1; // INPUT RANDOMIZATION VARIABLES
	static	const	int	Ign = 2; // COLUMNS TO BE IGNORED
};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
class XMZ_TREAT : public XERROR
{
private:
public:
	XMZ_TREAT( void ){};
	~XMZ_TREAT( void ){};
	int	count;			/* RAW COUNT OF MATCHES		*/
	int	index;			/* INDEX OF TREATMENT		*/
	double	metric;			/* `GOODNESS' OF TREATMENT	*/
};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
class XMZ : public XERROR
{
private:
	const	std::string	label;			/* IDENTIFIER FOR DATA		*/
	int	npast;		   	/* NUMBER OF PREVIOUS ALLOCATIONS */
	int	treat_nvals;		/* NUMBER OF TREATMENT VALUES	*/
	int	ncols;
	int	debug;
	std::vector<XMZ_VAR *>	col;
	XMZ_TREAT	*trt;
	XRAND	rand;
	static	int 	sort_cmp_TREAT( const void *a, const void *b );
	void 	clearResults( void );
public:
	XMZ( const std::string title );
	~XMZ( void );

	void 	setDebug( const int td );
	XMZ_VAR *createVar( const std::string title, const int nvalues,
		const std::string db_field );
	XMZ_VAR *pointerVar( const int indx );
	XMZ_VAR *pointerVar( const std::string title );
	void 	countMatch( const int nrows, std::vector<int> *past_val,
		std::vector<int> *past_trt, const int match, XMZ_TREAT *tcount);
	void 	dump( XMZ_VAR *ctreat );
	bool	eval( const std::vector<int> *current,
			const std::vector<int> *weight );
	int 	select( const int rank );
	int 	countVal( const int column, const int val );
	bool 	addPast( const std::vector<int> *val );
	int 	getNPast( void ) const;
	int 	addFromDB( XDB *d, const std::string table,
		const std::string where );
};
/*===========================================================================*/
#endif
