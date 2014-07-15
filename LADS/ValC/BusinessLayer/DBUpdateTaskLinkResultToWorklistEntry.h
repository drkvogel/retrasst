#ifndef DBUPDATETASKLINKRESULTTOWORKLISTENTRYH
#define DBUPDATETASKLINKRESULTTOWORKLISTENTRYH

#include "Task.h"

namespace valc
{

class DBTransactionResources;

class DBUpdateTaskLinkResultToWorklistEntry : public stef::Task
{
public:
    DBUpdateTaskLinkResultToWorklistEntry(  int resultID, 
                                            int worklistEntry,
                                            const DBTransactionResources* dbtr  );
protected:
    void doStuff();
private:
    const int m_resultID, m_worklistEntryID;
    const DBTransactionResources* const m_dbTransactionResources;
};

};

#endif

