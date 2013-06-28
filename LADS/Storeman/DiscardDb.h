#ifndef DiscardDbH
#define DiscardDbH

#include <DBTables.hpp>
#include "LCdbProject.h"
#include "LCdbJob.h"
#include "LCdbOperator.h"

#include "DiscardUtil.h"
#include "DiscardLogic.h"


namespace Discard
{

typedef std::vector<Sample> SampleVec;

class Db
{
public:
    Db( );
	Db( LIMSDatabase * cdb );
	void setCdb( LIMSDatabase * cdb );
	LIMSDatabase * getCdb( ) const;
	void setPdb( LIMSDatabase * pdb );
	LIMSDatabase * getPdb( ) const;
    void setProjectName( const std::string & name );
    const std::string & getProjectName( ) const;
    const LCDbProject * getPproj( ) const;
    std::string getStudyCode( ) const;
    std::string getCdbname( ) const;
    std::string getPdbname( ) const;
    void setCq( LQuery * cq );
    LQuery * getCq( ) const;
    void setPq( LQuery * pq );
    LQuery * getPq( ) const;
    std::string calcPersonFname( ) const;
    void addJobnos( IntVec * jobnos, const int dbcrstatus ) const;
	const LCDbCryoJob * getJob( const int jobno ) const;
    bool isJob( const int jobno ) const;
    const LCDbCryoJob * makeJob( const std::string & prefix,
		const std::string & description, const std::string & reason = "" ) const;
    const LCDbOperator * getUser( const int userid ) const;
    void addSamples( SampleVec * samples, const Cryovial & cryovial ) const;
    void addSamples( SampleVec * samples, const Tube & tube ) const;
    void addSamples( SampleVec * samples, const Box & box ) const;
    void addSamples( SampleVec * samples, const Person & person ) const;
    void addSamples( SampleVec * samples, const Job & job ) const;
    void addSamples( SampleVec * samples ) const;
    void setNotesForSamples( SampleVec * samples ) const;
    std::string updateSamples( const std::map<int,IntSet> & jobCsids,
        const int dbcrstatus,
        const std::string & jobName, const std::string & jobDescription,
        const IntToStringMap & sampleNote ) const;
    std::string updateSamplesStatus( const std::map<int,IntSet> & jobCsids,
        const int dbcrstatus,
        const std::string & jobName, const std::string & jobDescription ) const;
    std::string resetSamples( const std::map<int,IntSet> & jobCsids ) const;
    std::string resetSamplesStatus(
        const std::map<int,IntSet> & jobCsids ) const;
    std::string calcJobCounts(
        const int jobno, std::map<int,int> * counts ) const;
    std::string closeJob( const int jobno ) const;
    std::string updateSamplesNote( const IntToStringMap & sampleNote ) const;
    std::string getJobOwner( const int jobno ) const;
    bool canAbort( const int jobno ) const;
    std::string getJobName( const int jobno ) const;
    std::string getJobDescription( const int jobno ) const;
    time_t getJobCreationUtime( const int jobno ) const;
    const LCDbOperator * getCurrentUser( ) const;
    std::string getCurrentUserName( ) const;
    StringVec getProjectNames( ) const;
    int allocCids( const size_t count = 1 ) const;
    bool addAuditEntry( const std::string & message ) const;
protected:
private:
	LIMSDatabase * m_cdb;
    LIMSDatabase * m_pdb;
    std::string m_projectName;
    const LCDbProject * m_pproj;
    LQuery * m_cq;
    LQuery * m_pq;
};

class AliquotInfo
	: public OrderedNameTable
{
public:
	AliquotInfo( );
	virtual void populate( );
protected:
private:
};

class CrstatusInfo
	: public OrderedNumberTable
{
public:
	CrstatusInfo( );
	virtual void populate( );
protected:
private:
};

} // Discard


#endif
