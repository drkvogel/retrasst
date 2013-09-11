#ifndef DBUPDATETASKLINKRESULTTOWORKLISTENTRYH
#define DBUPDATETASKLINKRESULTTOWORKLISTENTRYH

#include "DBUpdateTask.h"

namespace valc
{

class DBUpdateTaskLinkResultToWorklistEntry : public DBUpdateTask
{
public:
    DBUpdateTaskLinkResultToWorklistEntry( int resultID, int worklistEntry );
protected:
    std::string describeUpdate() const;
    void        updateDatabase();
private:
    int m_resultID, m_worklistEntryID;
};

};

#endif

