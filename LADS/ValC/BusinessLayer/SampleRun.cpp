#include "SampleRun.h"
#include "StringBuilder.h"
#include <SysUtils.hpp>

namespace valc
{

SampleRun::SampleRun()
    :
    m_isOpen(false),
    m_sequencePosition(0.0),
    m_isQC(false)
{
}

SampleRun::SampleRun( 
    const IDToken& runID, 
    const std::string& sampleDescriptor, 
    bool isOpen, 
    const TDateTime& created, 
    const TDateTime& closed, 
    float sequencePosition, 
    const paulst::Nullable<int>& groupID, 
    bool isQC )
    :
    m_runID( runID ),
    m_sampleDescriptor( sampleDescriptor ),
    m_isOpen( isOpen ),
    m_created( created ),
    m_closed( isOpen ? TDateTime() : closed ),
    m_sequencePosition( sequencePosition ),
    m_groupID ( groupID ),
    m_isQC( isQC )
{
}

SampleRun::SampleRun( const SampleRun& o )
    :
    m_runID             (   o.m_runID           ),
    m_sampleDescriptor  (   o.m_sampleDescriptor),
    m_isOpen            (   o.m_isOpen          ),
    m_created           (   o.m_created         ),
    m_closed            (   o.m_closed          ),
    m_sequencePosition  (   o.m_sequencePosition),
    m_groupID           (   o.m_groupID         ),
    m_isQC              (   o.m_isQC            )
{
}

SampleRun& SampleRun::operator=( const SampleRun& o )
{
    m_runID             = o.m_runID;
    m_sampleDescriptor  = o.m_sampleDescriptor;
    m_isOpen            = o.m_isOpen;
    m_created           = o.m_created;
    m_closed            = o.m_closed;
    m_sequencePosition  = o.m_sequencePosition;
    m_groupID           = o.m_groupID;
    m_isQC              = o.m_isQC;
    return *this;
}

paulst::Nullable<int> SampleRun::getGroupID() const
{
    return m_groupID;
}

IDToken SampleRun::getID() const
{
    return m_runID;
}

std::string SampleRun::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

float SampleRun::getSequencePosition() const
{
    return m_sequencePosition;
}

bool SampleRun::isOpen() const
{
    return m_isOpen;
}

bool SampleRun::isQC() const
{
    return m_isQC;
}

}

