#ifndef DBUPDATETASKUPDATESAMPLERUNIDH
#define DBUPDATETASKUPDATESAMPLERUNIDH

#include "DBUpdateTask.h"

namespace valc
{

class DBUpdateTaskUpdateSampleRunID : public DBUpdateTask
{
public:
    DBUpdateTaskUpdateSampleRunID( const std::string& candidateNewSampleRunID, int buddySampleID );
protected:
    std::string describeUpdate() const;
    void        updateDatabase();
private:
    std::string m_candidateNewSampleRunID;
    int         m_buddySampleID;
};

};

#endif

