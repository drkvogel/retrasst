#include "ControlModel.h"
#include "TestResultImpl.h"
#include "Trace.h"

namespace valc
{

const ControlStatus TestResultImpl::UNCONTROLLED;

TestResultImpl::TestResultImpl(
        char actionFlag,
        const std::string& sampleDescriptor,
        const TDateTime& dateAnalysed,
        int machineID,
        int resultID,
        const IDToken& sampleRunID,
        int testID,
        float resultValue,
        const std::string& resultText,
        const ControlModel* controlModel )
    : 
    m_actionFlag(actionFlag),
    m_sampleDescriptor(sampleDescriptor),
    m_dateAnalysed(dateAnalysed),
    m_machineID(machineID),
    m_resultID(resultID),
    m_sampleRunID(sampleRunID),
    m_testID(testID),
    m_resultValue(resultValue),
    m_resultText(resultText),
    m_controlModel( controlModel )
{
}

TestResultImpl::~TestResultImpl()
{
}

char TestResultImpl::getActionFlag() const
{
    return m_actionFlag;
}

ControlStatus TestResultImpl::getControlStatus() const
{
    return m_controlModel ? m_controlModel->getControlStatus( m_testID, m_sampleRunID ) : UNCONTROLLED;
}

TDateTime TestResultImpl::getDateAnalysed() const
{
    return m_dateAnalysed;
}

int TestResultImpl::getMachineID() const
{
    return m_machineID;
}

int TestResultImpl::getID() const
{
    return m_resultID;
}

std::string TestResultImpl::getResultText() const
{
    return m_resultText;
}

float TestResultImpl::getResultValue() const
{
    return m_resultValue;
}

std::string TestResultImpl::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

IDToken TestResultImpl::getSampleRunID() const
{
    return m_sampleRunID;
}

int TestResultImpl::getTestID() const
{
    return m_testID;
}

}

