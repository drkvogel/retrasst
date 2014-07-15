#ifndef DBTRANSACTIONRERUNH
#define DBTRANSACTIONRERUNH

#include "API.h"

namespace paulst
{
    class Config;
}

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class DBTransactionResources;

class DBTransactionRerun
{
public:
    DBTransactionRerun( int worklistID,
                        int userID,
                        IDToken sampleRunID,
                        const DBTransactionResources* dbTransactionResources );
    DBTransactionRerun( const DBTransactionRerun& );
    DBTransactionRerun& operator=( const DBTransactionRerun& );
    void operator()( int& newWorklistID );

private:

    int                             m_worklistID;
    int                             m_userID;
    IDToken                         m_sampleRunID;
    const DBTransactionResources*   m_dbTransactionResources;

    void confirmOKToCloseSampleRun(         paulstdb::DBConnection* c, 
                                            const paulst::Config* config );
    void reserveIDValueForNewWorklistEntry( paulstdb::DBConnection* c, 
                                            const paulst::Config* config,
                                            int& newWorklistID );
};

}

#endif

