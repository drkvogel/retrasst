#ifndef EXCEPTIONALDATAHANDLERIMPLH
#define EXCEPTIONALDATAHANDLERIMPLH

#include "ExceptionalDataHandler.h"

struct lua_State;

namespace paulst
{
    class LoggingService;
}

namespace valc
{

class UserAdvisor;

class ExceptionalDataHandlerImpl : public ExceptionalDataHandler
{
public:
    /*
        Script is expected to have the following functions:

        Name:                        
            canProvideProjectIDFor
        Args:
            barcode
        Returns:
            boolean

        Name:                        
            getProjectIDFor
        Args:
            barcode
        Returns:
            integer

        Name:
            notifyBuddyDatabaseEntryIgnored
        Args:
            none
        Returns:
            2 boolean values: 
                The first: 
                    whether or not should bother doing more processing
                The second: 
                    whether or not the user should be advised

        Name:
            notifyCannotAllocateResultToWorklistEntry
        Args:
            none
        Returns:
            2 boolean values: 
                The first: 
                    whether or not should bother doing more processing
                The second: 
                    whether or not the user should be advised

        Name:
            notifyWorklistEntryIgnored
        Args:
            none
        Returns:
            2 boolean values: 
                The first: 
                    whether or not should bother doing more processing
                The second: 
                    whether or not the user should be advised
    */
    ExceptionalDataHandlerImpl( const std::string& luaScript, UserAdvisor* userAdvisor, paulst::LoggingService* log );
    ~ExceptionalDataHandlerImpl();
    bool canProvideProjectIDFor( const std::string& barcode );
    int  getProjectIDFor       ( const std::string& barcode );
    bool notifyBuddyDatabaseEntryIgnored            ( int buddySampleID, const std::string& reason );
    bool notifyCannotAllocateResultToWorklistEntry  ( int resultID,      const std::string& reason );
    bool notifyWorklistEntryIgnored                 ( int recordNo,      const std::string& reason );
private:
    lua_State* L;
    UserAdvisor*            m_userAdvisor;
    paulst::LoggingService* m_log;

    bool runNotifyMethod( const std::string& methodName, const std::string& failure, const std::string& reason );
};

}

#endif

