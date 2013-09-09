#ifndef EXCEPTIONALDATAHANDLERH
#define EXCEPTIONALDATAHANDLERH

#include <string>

namespace valc
{

class ExceptionalDataHandler
{
public:
    ExceptionalDataHandler();
    virtual ~ExceptionalDataHandler();
    virtual bool canProvideProjectIDFor( const std::string& barcode ) = 0;
    virtual int  getProjectIDFor       ( const std::string& barcode ) = 0;
    /*
        Return 'true' to indicate 'never mind, carry on'.
        Return 'false' to indicate not worth processing any more entries.
    */
    virtual bool notifyBuddyDatabaseEntryIgnored          ( int buddySampleID, const std::string& reason ) = 0;
    virtual bool notifyCannotAllocateResultToWorklistEntry( int resultID,      const std::string& reason ) = 0;
    virtual bool notifyWorklistEntryIgnored               ( int recordNo,      const std::string& reason ) = 0;
};

}

#endif

