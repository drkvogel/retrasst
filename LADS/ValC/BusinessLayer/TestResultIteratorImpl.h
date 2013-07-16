#ifndef TESTRESULTITERATORIMPLH
#define TESTRESULTITERATORIMPLH

#include "ResultIndex.h"

namespace valc
{

typedef const TestResult* ConstTestResultPtr;

class TestResultIteratorImpl
{
public:
    TestResultIteratorImpl();
	TestResultIteratorImpl(
		const typename ResultIndex::const_iterator& pos,
		const typename ResultIndex::const_iterator& endOfSequence,
        const ResultIndex* resultIndex );
    TestResultIteratorImpl( const TestResultIteratorImpl& );
	~TestResultIteratorImpl();
    TestResultIteratorImpl& operator=( const TestResultIteratorImpl& other );
    bool operator==( const TestResultIteratorImpl& other ) const;
    void increment();
    const TestResult*& dereference() const;
private:
    ConstTestResultPtr* m_referencedValue;
    ResultIndex::const_iterator m_pos, m_endOfSequence;
    const ResultIndex* m_resultIndex;

    void updateReferencedValue();
    
};


};

#endif

