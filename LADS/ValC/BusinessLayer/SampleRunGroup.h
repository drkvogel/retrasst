#ifndef SAMPLERUNGROUPH
#define SAMPLERUNGROUPH

#include "API.h"
#include "IDTokenSequence.h"
#include <string>
#include <vector>

namespace valc
{

class SampleRunGroup
{
public:
    SampleRunGroup( bool isQC, int groupID, bool canAcceptIDRevision );
    bool canAcceptIDRevision( int newID ) const;
    int  getID() const;
    bool includes( const IDToken& runID ) const;
    bool isQC() const;
    void listRunIDs( IDTokenSequence& out ) const;
    void push_back( const IDToken& sampleRunID );
    void reviseGroupID( int newGroupID );
private:
    const bool m_isQC;
    int m_id;
    bool m_canAcceptIDRevision;
    IDTokenSequence m_members;

    SampleRunGroup( const SampleRunGroup& );
    SampleRunGroup& operator=( const SampleRunGroup& );
};

}

#endif

