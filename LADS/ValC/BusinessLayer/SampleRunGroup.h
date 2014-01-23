#ifndef SAMPLERUNGROUPH
#define SAMPLERUNGROUPH

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
    bool isQC() const;
    void listRunIDs( std::vector< std::string >& out ) const;
    void push_back( const std::string& sampleRunID );
    void reviseGroupID( int newGroupID );
private:
    const bool m_isQC;
    int m_id;
    bool m_canAcceptIDRevision;
    std::vector< std::string > m_members;

    SampleRunGroup( const SampleRunGroup& );
    SampleRunGroup& operator=( const SampleRunGroup& );
};

}

#endif

