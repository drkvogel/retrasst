#ifndef TESTRESULTIMPLH
#define TESTRESULTIMPLH

#include "API.h"

namespace valc
{

class ControlModel;

class TestResultImpl : public TestResult
{
public:
    TestResultImpl(
        char actionFlag,
        const std::string& sampleDescriptor,
        const TDateTime& dateAnalysed,
        int machineID,
        int resultID,
        const IDToken& sampleRunID,
        int testID,
        float resultValue,
        const std::string& resultText,
        const ControlModel* controlModel // can be NULL, signifiying 'uncontrolled'
        );
    ~TestResultImpl();
    char          getActionFlag       () const;
    ControlStatus getControlStatus    () const;
    TDateTime     getDateAnalysed     () const;
    int           getMachineID        () const;
    int           getID               () const;
    std::string   getSampleDescriptor () const;
    IDToken       getSampleRunID      () const;
    std::string   getResultText       () const;
    float         getResultValue      () const;
    int           getTestID           () const;
private:
    const char          		m_actionFlag;
    const std::string   		m_sampleDescriptor;
    const TDateTime     		m_dateAnalysed;
    const int           	    m_machineID;
    const int                   m_resultID;
    const IDToken               m_sampleRunID;
    const int                   m_testID;
    const float                 m_resultValue;
    const std::string           m_resultText;
    const ControlModel* const   m_controlModel;

    static const ControlStatus  UNCONTROLLED;
};

};

#endif

