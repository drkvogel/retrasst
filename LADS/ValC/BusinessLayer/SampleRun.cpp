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

SampleRun::SampleRun( const std::string& sampleDescriptor, int buddySampleID )
    :
    m_id(sampleDescriptor),
    m_sampleDescriptor( sampleDescriptor ),
    m_isOpen( true ),
    m_created( Now() ),
    m_closed( TDateTime() ),
    m_sequencePosition( buddySampleID )
{
}

SampleRun::SampleRun( int dbID, const std::string& sampleDescriptor, bool isOpen, const TDateTime& created, const TDateTime& closed, float sequencePosition )
    :
    m_databaseID( std::string() << dbID ),
    m_id( std::string() << dbID),
    m_sampleDescriptor( sampleDescriptor ),
    m_isOpen( isOpen ),
    m_created( created ),
    m_closed( isOpen ? TDateTime() : closed ),
    m_sequencePosition( sequencePosition )
{
}

SampleRun::SampleRun( const SampleRun& o )
    :
    m_databaseID        (   o.m_databaseID      ),
    m_id                (   o.m_id              ),
    m_sampleDescriptor  (   o.m_sampleDescriptor),
    m_isOpen            (   o.m_isOpen          ),
    m_created           (   o.m_created         ),
    m_closed            (   o.m_closed          ),
    m_sequencePosition  (   o.m_sequencePosition)
{
}

SampleRun& SampleRun::operator=( const SampleRun& o )
{
    m_databaseID        = o.m_databaseID;
    m_id                = o.m_id;
    m_sampleDescriptor  = o.m_sampleDescriptor;
    m_isOpen            = o.m_isOpen;
    m_created           = o.m_created;
    m_closed            = o.m_closed;
    m_sequencePosition  = o.m_sequencePosition;
    return *this;
}

std::string SampleRun::getDatabaseID() const
{
    return m_databaseID;
}

std::string SampleRun::getID() const
{
    return m_id;
}

std::string SampleRun::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}

float SampleRun::getSequencePosition() const
{
    return m_sequencePosition;
}

bool SampleRun::hasDatabaseID() const
{
    return m_databaseID.size();
}

bool SampleRun::isOpen() const
{
    return m_isOpen;
}

/*
bool SampleRun::operator==( const SampleRun& o ) const
{
    return  ( 
                ( hasDatabaseID() && o.hasDatabaseID() )
                &&
                ( getDatabaseID() == o.getDatabaseID() )
            )
            ||
            (
                ( getSampleDescriptor() == o.getSampleDescriptor() )
                &&
                ( 
                    ( ( ! hasDatabaseID() ) && ( ! o.hasDatabaseID() ) )
                    ||
                    ( ( !   hasDatabaseID() ) && o.hasDatabaseID() && o.isOpen() )
                    ||
                    ( ( ! o.hasDatabaseID() ) &&   hasDatabaseID() &&   isOpen() )
                )
            );
}
*/

}

