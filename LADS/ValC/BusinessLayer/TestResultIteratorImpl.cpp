#include "Require.h"
#include "TestResultIteratorImpl.h"

namespace valc
{

TestResultIteratorImpl::TestResultIteratorImpl()
    : 
    m_referencedValue( new ConstTestResultPtr() ),
    m_resultIndex(0)
{
    *m_referencedValue = 0;
}

TestResultIteratorImpl::TestResultIteratorImpl( 
    const ResultIndex::const_iterator& pos, 
    const ResultIndex::const_iterator& endOfSequence,
    const ResultIndex* resultIndex )
    : 
    m_pos( pos ), 
    m_endOfSequence( endOfSequence ),
    m_referencedValue( new ConstTestResultPtr() ),
    m_resultIndex( resultIndex )
{
    updateReferencedValue();
}

TestResultIteratorImpl::TestResultIteratorImpl( const TestResultIteratorImpl& other )
    :
    m_pos            ( other.m_pos ),
    m_endOfSequence  ( other.m_endOfSequence ),
    m_resultIndex    ( other.m_resultIndex ),
    m_referencedValue( new ConstTestResultPtr() )
{
    updateReferencedValue();
}

TestResultIteratorImpl::~TestResultIteratorImpl()
{
    delete m_referencedValue;
}

TestResultIteratorImpl& TestResultIteratorImpl::operator=( const TestResultIteratorImpl& other )
{
    m_pos           = other.m_pos;
    m_endOfSequence = other.m_endOfSequence;
    m_resultIndex   = other.m_resultIndex;
    updateReferencedValue();
    return *this;
}

bool TestResultIteratorImpl::operator==( const TestResultIteratorImpl& other ) const
{
    return m_pos == other.m_pos;
}

void TestResultIteratorImpl::increment()
{
    require( m_pos != m_endOfSequence );
    
    ++m_pos;

    updateReferencedValue();
}

const TestResult*& TestResultIteratorImpl::dereference() const
{
    return *m_referencedValue;
}

void TestResultIteratorImpl::updateReferencedValue()
{
    if ( m_pos == m_endOfSequence )
    {
        *m_referencedValue = 0;
    }
    else
    {
        const int resultID = m_pos->second;
        *m_referencedValue = m_resultIndex->findResult( resultID );
    }
}

}

