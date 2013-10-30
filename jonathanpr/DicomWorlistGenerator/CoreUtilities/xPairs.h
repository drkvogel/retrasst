#ifndef xpairsH
#define xpairsH
//---------------------------------------------------------------------------
class XPAIR_VAL
{
private:
public:
	XPAIR_VAL( String id, int ival );
	XPAIR_VAL( String id, String sval );
	~XPAIR_VAL( void );
	String	name;
	int	type;
	int	ivalue;
	String	svalue;
};
//---------------------------------------------------------------------------
class XPAIRS
{
private:
public:
	XPAIRS( void );
	~XPAIRS( void );
	int	npairs;
	XPAIR_VAL	**pair;
	void 	extend( void );
	bool	set_int( String id, int ival );
	bool	set_string( String id, String sval );
	XPAIR_VAL	*get_value( String id );
};
//---------------------------------------------------------------------------
#endif

