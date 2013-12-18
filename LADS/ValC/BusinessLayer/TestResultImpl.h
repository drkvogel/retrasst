#ifndef TESTRESULTIMPLH
#define TESTRESULTIMPLH

#include "API.h"

namespace valc
{

class TestResultImpl : public TestResult
{
public:
    TestResultImpl(
        char actionFlag,
        const std::string& sampleDescriptor,
        const TDateTime& dateAnalysed,
        int machineID,
        int resultID,
        const std::string& sampleRunID,
        int testID,
        float resultValue,
        const std::string& resultText );
    ~TestResultImpl();
    char        getActionFlag       () const;
    TDateTime   getDateAnalysed     () const;
    int         getMachineID        () const;
    int         getID               () const;
    std::string getSampleDescriptor () const;
    std::string getSampleRunID      () const;
    std::string getResultText       () const;
    float       getResultValue      () const;
    int         getTestID           () const;
private:
    const char          m_actionFlag;
    const std::string   m_sampleDescriptor;
    const TDateTime     m_dateAnalysed;
    const int           m_machineID;
    const int           m_resultID;
    const std::string   m_sampleRunID;
    const int           m_testID;
    const float         m_resultValue;
    const std::string   m_resultText;

};

};

#endif

