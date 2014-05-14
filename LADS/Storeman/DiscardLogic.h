#ifndef DiscardLogicH
#define DiscardLogicH

#include "DiscardUtil.h"

namespace Discard
{

class Cell
{
public:
    Cell( const int colno, const int rowno );
    std::string asString( ) const;
    bool operator== ( const Cell & that ) const;
    bool operator< ( const Cell & that ) const;
    int getColno( ) const;
    int getRowno( ) const;
protected:
private:
    int m_colno;
    int m_rowno;
};

class Tube
	: public ANamedThing
{
public:
	Tube( const std::string & barcode );
};

class Cryovial
	: public ANamedThing
{
public:
	Cryovial( const std::string & barcode );
    enum { STORED, MARKED, REMOVED, DESTROYED, NINETYNINED };
};

class Box
	: public ANamedThing
{
public:
	Box( const std::string & name );
};

class Context;

class Person
	: public ANamedThing
{
public:
	Person( const std::string & id );
    static void setFname( const std::string & fname );
    static const std::string & getFname( );
    static bool canSearch( );
private:
    static std::string s_fname;
};

class Job
	: public ANumberedThing
{
public:
	Job( const int id );
};

class Sample
	: public ANumberedThing
{
public:
	Sample( const int id );
	int getId( ) const;
	void setBarcode( const std::string & barcode );
	std::string getBarcode( ) const;
	void setCryovialBarcode( const std::string & barcode );
	std::string getCryovialBarcode( ) const;
	void setPersonId( const std::string & id );
	std::string getPersonId( ) const;
	void setAliquotId( const int id );
	int getAliquotId( ) const;
	void setCryovialId( const int id );
	int getCryovialId( ) const;
	void setCryovialStoreStatus( const int status );
	int getCryovialStoreStatus( ) const;
	void setJobno( const int status );
	int getJobno( ) const;
	void setNoteFlag( const int count );
	int getNoteFlag( ) const;
	void setCryovialStoreId( const int id );
	int getCryovialStoreId( ) const;
    void setNote( const std:: string & noteText );
    std::string getNote( ) const;
};

class CryovialStatus
	: public OrderedNameTable
{
public:
	CryovialStatus( );
	virtual void populate( );
	virtual void close( );
	int nextId( const int id );
	int getCurrentId( ) const;
	bool setCurrentId( const int id );
	bool setCurrentId( const std::string & name );
	int getNextId( ) const;
	bool setNextId( const int id );
	bool setNextId( const std::string & name );
protected:
	int m_currentIndex;
	int m_nextIndex;
private:
};

class Cells
{
public:
	Cells( );
	void reset( );
	int setHeader( const Cell & cell );
	int clearHeader( const Cell & cell );
	int toggleHeader( const Cell & cell );
	int isHeader( const Cell & cell );
	int getNHeader( );
	std::set<Cell>::const_iterator getHeaderBegin( ) const;
	std::set<Cell>::const_iterator getHeaderEnd( ) const;
	void setSampleno( const Cell & cell, const int sampleno );
	int getSampleno( const Cell & cell ) const;
private:
	std::set<Cell> m_isheader;
	std::map<Cell,int> m_sampleno;
};

} // Discard

#endif
