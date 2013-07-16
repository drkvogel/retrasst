#ifndef WORKLISTENTRYITERATORIMPLH
#define WORKLISTENTRYITERATORIMPLH

#include "WorklistEntries.h"

namespace valc
{

typedef const WorklistEntry* ConstWorklistEntryPtr;

class WorklistEntryIteratorImpl
{
public:
    WorklistEntryIteratorImpl();
    WorklistEntryIteratorImpl(
		const typename WorklistEntries::const_iterator& pos,
		const typename WorklistEntries::const_iterator& endOfSequence );
    WorklistEntryIteratorImpl( const WorklistEntryIteratorImpl& );
    ~WorklistEntryIteratorImpl();
    WorklistEntryIteratorImpl& operator=( const WorklistEntryIteratorImpl& other );
    bool operator==( const WorklistEntryIteratorImpl& other ) const;
    void increment();
    const WorklistEntry*& dereference() const;
private:
    ConstWorklistEntryPtr* m_referencedValue;
    WorklistEntries::const_iterator m_pos, m_endOfSequence;

    void updateReferencedValue();
    
};


};

#endif

