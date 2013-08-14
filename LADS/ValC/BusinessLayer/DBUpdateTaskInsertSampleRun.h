#ifndef DBUPDATETASKINSERTSAMPLERUNH
#define DBUPDATETASKINSERTSAMPLERUNH

#include "DBUpdateTask.h"

namespace valc
{

class DBUpdateTaskInsertSampleRun : public DBUpdateTask
{
public:
    DBUpdateTaskInsertSampleRun( const std::string& candidateNewSampleRunID, int sequencePosition );
protected:
    std::string describeUpdate() const;
    void        updateDatabase();
private:
    std::string m_candidateNewSampleRunID;
    int m_sequencePosition;
};

};

#endif

