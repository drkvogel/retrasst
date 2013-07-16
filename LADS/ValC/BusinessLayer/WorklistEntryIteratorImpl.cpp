#include "Require.h"
#include "WorklistEntryIteratorImpl.h"

namespace valc
{

WorklistEntryIteratorImpl::WorklistEntryIteratorImpl()
    : m_referencedValue( new ConstWorklistEntryPtr() )
{
    *m_referencedValue = 0;
}

WorklistEntryIteratorImpl::WorklistEntryIteratorImpl( 
    const WorklistEntries::const_iterator& pos, 
    const WorklistEntries::const_iterator& endOfSequence )
    : 
    m_pos( pos ), 
    m_endOfSequence( endOfSequence ),
    m_referencedValue( new ConstWorklistEntryPtr() )
{
    updateReferencedValue();
}

WorklistEntryIteratorImpl::WorklistEntryIteratorImpl( const WorklistEntryIteratorImpl& other )
    :
    m_pos( other.m_pos ),
    m_endOfSequence( other.m_endOfSequence ),
    m_referencedValue( new ConstWorklistEntryPtr() )
{
    updateReferencedValue();
}

WorklistEntryIteratorImpl::~WorklistEntryIteratorImpl()
{
    delete m_referencedValue;
}

WorklistEntryIteratorImpl& WorklistEntryIteratorImpl::operator=( const WorklistEntryIteratorImpl& other )
{
    m_pos           = other.m_pos;
    m_endOfSequence = other.m_endOfSequence;
    updateReferencedValue();
    return *this;
}

bool WorklistEntryIteratorImpl::operator==( const WorklistEntryIteratorImpl& other ) const
{
    return m_pos == other.m_pos;
}

void WorklistEntryIteratorImpl::increment()
{
    require( m_pos != m_endOfSequence );
    
    ++m_pos;

    updateReferencedValue();
}

const WorklistEntry*& WorklistEntryIteratorImpl::dereference() const
{
    return *m_referencedValue;
}

void WorklistEntryIteratorImpl::updateReferencedValue()
{
    *m_referencedValue = m_pos == m_endOfSequence ? 0 : m_pos->second.get();
}

}

