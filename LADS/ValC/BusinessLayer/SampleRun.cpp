#include "SampleRun.h"
#include "StringBuilder.h"
#include <SysUtils.hpp>

namespace valc
{

SampleRun::SampleRun()
    :
    m_isOpen(false),
    m_sequencePosition(0.0)
{
}

SampleRun::SampleRun( const std::string& runID, const std::string& sampleDescriptor, bool isOpen, const TDateTime& created, const TDateTime& closed, float sequencePosition )
    :
    m_runID( runID ),
    m_sampleDescriptor( sampleDescriptor ),
    m_isOpen( isOpen ),
    m_created( created ),
    m_closed( isOpen ? TDateTime() : closed ),
    m_sequencePosition( sequencePosition )
{
}

SampleRun::SampleRun( const SampleRun& o )
    :
    m_runID             (   o.m_runID           ),
    m_sampleDescriptor  (   o.m_sampleDescriptor),
    m_isOpen            (   o.m_isOpen          ),
    m_created           (   o.m_created         ),
    m_closed            (   o.m_closed          ),
    m_sequencePosition  (   o.m_sequencePosition)
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
    return *this;
}

std::string SampleRun::getID() const
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

}

