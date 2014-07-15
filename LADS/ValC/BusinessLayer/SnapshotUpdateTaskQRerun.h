#ifndef SNAPSHOTUPDATETASKQRERUNH
#define SNAPSHOTUPDATETASKQRERUNH

#include "API.h"
#include <string>
#include "SnapshotUpdateTask.h"

namespace valc
{

class DBTransactionHandler;

class SnapshotUpdateTaskQRerun : public SnapshotUpdateTask
{
public:

    SnapshotUpdateTaskQRerun(   int worklistID, 
                                const IDToken& sampleRunID, 
                                const std::string& sampleDescriptor, 
                                const std::string& barcode, 
                                int user,
                                DBTransactionHandler* dbth );

protected:

    void updateDatabase();
    void updateSnapshot();
    void notifyObserver();

private:
    int                                     m_newWorklistID;
    const    int                            m_worklistID;
    const    int                            m_user;
    const    IDToken                        m_sampleRunID;
    const    std::string                    m_sampleDescriptor;
    const    std::string                    m_barcode;
    DBTransactionHandler* const             m_dbTransactionHandler;
};

}

#endif

