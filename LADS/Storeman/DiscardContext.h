#ifndef DiscardContextH
#define DiscardContextH

#include "DiscardUtil.h"
#include "DiscardLogic.h"
#include "DiscardDb.h"

namespace Discard
{

class SamplePile;

class SampleHandler
{
public:
	SampleHandler( );
	virtual int operator()( SamplePile * samples, const int sampleno ) const;
protected:
    virtual bool isMarkable( SamplePile * samples, const int sampleno ) const;
};

class MarkSampleHandler
	: public SampleHandler
{
public:
	MarkSampleHandler( );
	virtual int operator()( SamplePile * samples, const int sampleno ) const;
};

class UnmarkSampleHandler
	: public SampleHandler
{
public:
	UnmarkSampleHandler( );
	virtual int operator()( SamplePile * samples, const int sampleno ) const;
};

class ToggleMarkSampleHandler
	: public SampleHandler
{
public:
	ToggleMarkSampleHandler( );
	virtual int operator()( SamplePile * samples, const int sampleno ) const;
};

class NoteSampleHandler
	: public SampleHandler
{
public:
	NoteSampleHandler( );
	virtual int operator()( SamplePile * samples, const int sampleno ) const;
};

class SCComparator
{
public:
    enum { HIDDEN = 0, RESULTNO, SBARCODE, CBARCODE, PERSONID, ALIQUOT0, };
    SCComparator( const int sortcolno = 1 );
    bool operator( ) ( const int & left,
        const int & right ) const;
    void insertSortcolno( const int abssortcolno, const int direction = 0 );
    int getSortPosition( const int abssortcolno ) const;
    void setScalidToSampleno( const std::map<IntPair,int> & scalidToSampleno );
    void setAlnoToAlid( const IntToIntMap & alnoToAlid );
    void setSamples( const SamplePile * samples );
    const Sample * getSample( const int scid, const int alid = 0 ) const;
protected:
private:
    std::list<int> m_sortcolnos;
    std::map<IntPair,int> m_scalidToSampleno;
    IntToIntMap m_alnoToAlid;
    const SamplePile * m_samples;
};

class Context
{
public:
    enum { MARK = 0, NOTE, };
    Context( Db * db );
    void setProjectName( const std::string & name );
    const std::string & getProjectName( ) const;
    std::string getStudyCode( ) const;
    void configCrstatus( const int index );
    int getNextCrstatus( ) const;
    int getCurrentCrstatus( ) const;
    int getNextDbCrstatus( ) const;
    int getCurrentDbCrstatus( ) const;
    std::string getNextCrstatusName( ) const;
    std::string getCurrentCrstatusName( ) const;
    bool isCreateJobStage( ) const;
    bool isSelectJobStage( ) const;
    void setReason( const std::string & reason );
    const std::string & getReason( ) const;
    void setDescription( const std::string & description );
    const std::string & getDescription( ) const;
    void setMethod( const std::string & method );
    const std::string & getMethod( ) const;
    void setNote( const std::string & note );
    const std::string & getNote( ) const;
    static const MarkSampleHandler & getMarker( );
    static const UnmarkSampleHandler & getUnmarker( );
    static const ToggleMarkSampleHandler & getToggleMarker( );
    static const NoteSampleHandler & getNoter( );
    IntVec getAliquotIds( ) const;
    std::string getAliquotName( const int aliquotId ) const;
    void configPersonFname( );
    std::string getPersonFname( ) const;
    IntVec getJobnos( ) const;
    std::string calcJobPrompt( int jobno = 0 ) const;
    std::string calcJobDescription( int jobno = 0 ) const;
    std::string calcJobStatus( int jobno = 0 ) const;
    bool canAbort( int jobno = 0 ) const;
    std::string getJobOwner( int jobno = 0 ) const;
    std::string getJobDescription( int jobno = 0 ) const;
    std::string getJobReason( int jobno = 0 ) const;
    time_t getJobCreationUtime( int jobno = 0 ) const;
    void setSearchType( const std::string & searchType );
    const std::string & getSearchType( ) const;
    void setSearchSource( const std::string & searchSource );
    const std::string & getSearchSource( ) const;
    void configSearchTexts( const std::string & searchText );
    const StringSet & getSearchTexts( ) const;
    int calcDbCrstatus( const int crstatus ) const;
    void configDbCrstatusMap( );
    int calcCrstatus( const int dbCrstatus ) const;
    void configCrstatusMap( );
    Db * getDb( );
    bool isAddMarked( ) const;
    void setAddMarked( const bool isAddMarked );
    void setSelectMode( const int mode );
    int getSelectMode( ) const;
    bool isMarking( ) const;
    bool isNoting( ) const;
    void setSaved( const bool isSaved );
    bool isSaved( ) const;
    int allocCids( const size_t count = 1 ) const;
    std::string getCurrentUserName( ) const;
    bool addAuditEntry( const std::string & message ) const;
    void setJobno( const int jobno );
    int getJobno( ) const;
protected:
private:
    std::string m_reason;
    std::string m_description;
    std::string m_method;
    std::string m_note;
    CryovialStatus m_crstatus;
    static MarkSampleHandler s_marker;
    static UnmarkSampleHandler s_unmarker;
    static ToggleMarkSampleHandler s_toggleMarker;
    static NoteSampleHandler s_noter;
    AliquotInfo m_alinfo;
    std::string m_personFname;
    std::string m_searchType;
    std::string m_searchSource;
    StringSet m_searchTexts;
    IntToIntMap m_dbCrstatusMap;
    IntToIntMap m_crstatusMap;
    Db * m_db;
    int m_selectMode;
    bool m_isSaved;
    int m_jobno;
};

class SamplePile
{
public:
	SamplePile( );
	void init( Discard::Context * context );
	IntPair add( const Tube & tube );
	IntPair add( const Cryovial & cryovial );
	IntPair add( const Box & box );
	IntPair add( const Person & person );
	IntPair add( const Job & job );
	void add( const Sample & sample );
	void clear( );
	int count( ) const;
	std::string asString( ) const;
	const Sample * getSample( const int sampleno ) const;
	std::string update( const int dbcrstatus,
        const std::string & description, const std::string & reason );
	std::string reset( );
    int getNCryovialsRemaining( const int jobno ) const;
    bool isSampleMarkable( const Sample & sample ) const;
	int setMarked( const int sampleno );
	int clearMarked( const int sampleno );
	int toggleMarked( const int sampleno );
	int isMarked( const int sampleno );
	int getNMarked( ) const;
	IntSet::const_iterator getMarkedBegin( ) const;
	IntSet::const_iterator getMarkedEnd( ) const;
	int setNote( const int sampleno, const std::string & note );
	int getNNoted( ) const;
	int setNoteFromContext( const int sampleno );
	std::string getNote( const int sampleno ) const;
    IntVec getAliquotIds( ) const;
    SCComparator * getSCComparator( );
    std::string getAliquotName( const int alid ) const;
protected:
private:
	std::vector<Sample> m_samples;
    IntSet m_crids;
    IntSet m_jobnos;
    Context * m_context;
	IntSet m_ismarked;
    SCComparator m_scc;
    IntToStringMap m_note;
};

class GridStuff
{
public:
    GridStuff( const Discard::SamplePile & samples,
        const std::string & utext = "", const int usampleno = -1 );
    void init( );
    void analyze( Discard::SCComparator * comparator );
    void setup( );
    int getNcols( ) const;
    int getNrows( ) const;
    const std::string & getText( const Discard::Cell & cell );
    void setText( const Discard::Cell & cell, const std::string & text );
    std::map<Discard::Cell,std::string>::const_iterator getTextBegin( ) const;
    std::map<Discard::Cell,std::string>::const_iterator getTextEnd( ) const;
    int getFirstSampleColno( ) const;
    int getFirstSampleRowno( ) const;
    bool isHeader( const Discard::Cell & cell ) const;
    void setHeader( const Discard::Cell & cell );
    std::set<Discard::Cell>::const_iterator getHeaderBegin( ) const;
    std::set<Discard::Cell>::const_iterator getHeaderEnd( ) const;
    const int getSampleno( const Discard::Cell & cell );
    void setSampleno( const Discard::Cell & cell, const int sampleno );
    std::map<Discard::Cell,int>::const_iterator getCellSamplenoBegin( ) const;
    std::map<Discard::Cell,int>::const_iterator getCellSamplenoEnd( ) const;
protected:
private:
    void specifyGridColumns( );
    void specifyGridRows( );
    void specifyGridCells( );
    static int scnoToRowno( const int scno );
    static int alnoToColno( const int alno );

    int m_nrows;
    int m_ncols;
    int m_firstSampleColno;
    int m_firstSampleRowno;
    const Discard::SamplePile & m_samples;
    std::string m_utext;
    int m_usampleno;

    std::map<Discard::Cell,std::string> m_text;
    std::set<Discard::Cell> m_header;
    std::map<Discard::Cell,int> m_sampleno;

    IntToIntMap m_scidToScno;
    IntToIntMap m_alidToAlno;
    IntToIntMap m_alnoToAlid;
    std::map<IntPair,int> m_scalidToSampleno;
    IntToIntMap m_samplenoToScid;
};

} // Discard


#endif
