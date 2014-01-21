#ifndef MOCKSAMPLERUNGROUPIDGENERATORH
#define MOCKSAMPLERUNGROUPIDGENERATORH

#include <deque>
#include "Require.h"
#include "SampleRunGroupModel.h"

class MockSampleRunGroupIDGenerator : public valc::SampleRunGroupIDGenerator
{
private:
    std::deque<int>     m_sequence;

public:

    MockSampleRunGroupIDGenerator()
    {
    }

    void prime( int id )
    {
        m_sequence.push_back( id );
    }

    int nextID()
    {
        require( m_sequence.size() );
        int id = m_sequence.front();
        m_sequence.pop_front();
        return id;
    }
};


#endif

