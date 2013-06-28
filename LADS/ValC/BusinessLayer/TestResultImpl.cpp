#include "TestResultImpl.h"
#include "Trace.h"

namespace valc
{

TestResultImpl::TestResultImpl(
        char actionFlag,
        const std::string& sampleDescriptor,
        TDateTime& dateAnalysed,
        int machineID,
        int resultID,
        const SampleRunID& sampleRunID,
        int testID,
        float resultValue )
    : 
    m_actionFlag(actionFlag),
    m_sampleDescriptor(sampleDescriptor),
    m_dateAnalysed(dateAnalysed),
    m_machineID(machineID),
    m_resultID(resultID),
    m_sampleRunID(sampleRunID),
    m_testID(testID),
    m_resultValue(resultValue)
{
}

TestResultImpl::~TestResultImpl()
{
}

char TestResultImpl::getActionFlag() const
{
    return m_actionFlag;
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

float TestResultImpl::getResultValue() const
{
    return m_resultValue;
}

std::string TestResultImpl::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

std::string TestResultImpl::getSampleRunID() const
{
    return m_sampleRunID.toString();
}

int TestResultImpl::getTestID() const
{
    return m_testID;
}

}

