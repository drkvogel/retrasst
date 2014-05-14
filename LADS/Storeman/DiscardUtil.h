#ifndef DiscardUtilH
#define DiscardUtilH

#include <string>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <time.h>
#include <stdexcept>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define HERE __FILE__ ":" TOSTRING(__LINE__)

typedef std::set<int> IntSet;
typedef std::vector<int> IntVec;

typedef std::set<std::string> StringSet;
typedef std::vector<std::string> StringVec;

typedef std::pair<std::string,std::string> StringPair;
typedef std::pair<int,int> IntPair;

typedef std::set<StringPair> StringPairSet;
typedef std::vector<StringPair> StringPairVec;

typedef std::set<IntPair> IntPairSet;
typedef std::vector<IntPair> IntPairVec;

typedef std::map<int,int> IntToIntMap;
typedef std::map<int,std::string> IntToStringMap;
typedef std::map<std::string,int> StringToIntMap;
typedef std::map<std::string,std::string> StringToStringMap;

namespace Discard
{

class MyRosetta
{
public:
    MyRosetta( );
    int getInt( const std::string & name ) const;
    void setInt( const std::string & name, const int value );
    const std::string & getString( const std::string & name ) const;
    void setString( const std::string & name, const std::string & value );
    static const std::string NO_SVALUE;
    static const int NO_IVALUE;
protected:
    StringToStringMap m_svalues;
    StringToIntMap m_ivalues;
private:
};

class ANamedThing
    : public MyRosetta
{
public:
	ANamedThing( );
	ANamedThing( const std::string & name );
	std::string getName( ) const;
};

class ANumberedThing
    : public MyRosetta
{
public:
	ANumberedThing( );
	ANumberedThing( int number );
	int getNumber( ) const;
};

class NameTable
{
public:
	NameTable( );
	void init( );
	virtual void clear( );
	virtual void populate( );
	virtual void close( );
	bool set( const int id, const std::string & name );
	std::string getName( const int id ) const;
	int getId( const std::string & name ) const;
protected:
	StringToIntMap m_id;
	IntToStringMap m_name;
private:
};

class NumberTable
{
public:
	NumberTable( );
	void init( );
	virtual void clear( );
	virtual void populate( );
	virtual void close( );
	bool set( const int id, const int number );
	int getNumber( const int id ) const;
	int getId( const int number ) const;
protected:
	IntToIntMap m_id;
	IntToIntMap m_number;
private:
};

class OrderedNameTable
	: public NameTable
{
public:
	OrderedNameTable( );
	virtual void close( );
	IntVec getIds( ) const;
	StringVec getNames( ) const;
protected:
	IntVec m_ids;
	StringVec m_names;
	IntToIntMap m_index;
	int m_nids;
private:
};
/*
class OrderedNumberTable
	: public NumberTable
{
public:
	OrderedNumberTable( );
	virtual void close( );
	IntVec getIds( ) const;
	IntVec getNumbers( ) const;
protected:
	IntVec m_ids;
	IntVec m_numbers;
	IntToIntMap m_index;
	int m_nids;
private:
};
*/
class MyException
    : public std::runtime_error
{
public:
    MyException( const std::string & text );
};

namespace Util
{
	std::string quote( const std::string & s,
		const std::string & q1 = "'", const std::string & q2 = "" );
    std::string bracket( const std::string & s );
    StringVec split( const std::string & sep, const std::string & in );
	std::string asString( const int n );
	std::string substitute( const std::string & in,
		const std::string & from, const std::string & to );
	std::string trim( const std::string & text );
	std::string ltrim( const std::string & text );
	std::string rtrim( const std::string & text );
    std::string dtg( time_t utime = -1 );
    std::string ddmmyyyy( time_t utime = -1 );
    int sigma( const int n );
    int at( const IntToIntMap & map, const int name,
        const int defaultValue = -1 );
    int at( const IntVec & vec, const int index,
        const int defaultValue = -1 );
    std::string at( const IntToStringMap & map, const int name,
        const std::string & defaultValue = "" );
    std::string at( const StringVec & vec, const int index,
        const std::string & defaultValue = "" );
    unsigned int blend( const unsigned int i1, const size_t w1,
        const unsigned int i2, const size_t w2 );
    std::string pad( const std::string & text, const size_t width,
        const char padding = ' ' );
    bool splat( const std::string & path, const std::string & gob,
        const std::string mode = "w" );

} // Util

namespace Util
{

template<typename T>
std::string
join( const std::string & d, const T & begin, const T & end )
{
	std::string text = "";
	bool first = true;
	for (T it = begin; it != end; it++)
	{
		if (first)
		{
			text += *it;
			first = false;
		}
		else
		{
			text += d + *it;
		}
	}

	return text;
}

} // Util


} // Discard

#endif
