#include <boost/foreach.hpp>
#include "ResultDirectory.h"
#include "Trace.h"
#include "WorklistEntryImpl.h"

namespace valc
{

WorklistEntryImpl::WorklistEntryImpl(
        int                         recordNo,
        const std::string&          sampleDescriptor,
        int                         machineID,
        const std::string&          barcode,
        int                         testID,
        int                         groupID,
        int                         categoryID,
        int                         sampleID,
        int                         projectID,
        int                         profileID,
        const std::string&          profileName,
        TDateTime                   timeStamp,
        int                         tsSequence,
        char                        status,
        float                       diluent,
        int                         buddyResultID,
        const ResultDirectory*      resultDirectory )
    :
    m_recordNo      ( recordNo ),
    m_sampleDescriptor( sampleDescriptor ),
    m_machineID     ( machineID ),
    m_barcode       ( barcode ),
    m_testID        ( testID ),
    m_groupID       ( groupID ),
    m_categoryID    ( categoryID ),
    m_sampleID      ( sampleID ),
    m_projectID     ( projectID ),
    m_profileID     ( profileID ),
    m_profileName   ( profileName ),
    m_timeStamp     ( timeStamp ),
    m_tsSequence    ( tsSequence ),
    m_status        ( status ),
    m_diluent       ( diluent ),
    m_buddyResultID ( buddyResultID ),
    m_resultDirectory ( resultDirectory )
{
}

WorklistEntryImpl::~WorklistEntryImpl()
{
}

std::string WorklistEntryImpl::getBarcode() const
{
    return m_barcode;
}

int WorklistEntryImpl::getBuddyResultID() const
{
    return m_buddyResultID;
}

int WorklistEntryImpl::getCategoryID() const
{
    return m_categoryID;
}

float WorklistEntryImpl::getDiluent() const
{
    return m_diluent;
}

int WorklistEntryImpl::getGroupID() const
{
    return m_groupID;
}

int WorklistEntryImpl::getID() const
{
    return m_recordNo;
}

int WorklistEntryImpl::getMachineID() const
{
    return m_machineID;
}

int WorklistEntryImpl::getProfileID() const
{
    return m_profileID;
}

std::string WorklistEntryImpl::getProfileName() const
{
    return m_profileName;
}

int WorklistEntryImpl::getProjectID() const
{
    return m_projectID;
}

std::string WorklistEntryImpl::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

Range<TestResultIterator> WorklistEntryImpl::getTestResults() const
{
    return m_resultDirectory->equal_range( getID() );
}

int WorklistEntryImpl::getSampleID() const
{
    return m_sampleID;
}

int WorklistEntryImpl::getTestID() const
{
    return m_testID;
}

TDateTime WorklistEntryImpl::getTimeStamp() const
{
    return m_timeStamp;
}

int WorklistEntryImpl::getTSSequence() const
{
    return m_tsSequence;
}

char WorklistEntryImpl::getStatus() const
{
    return m_status;
}

}

