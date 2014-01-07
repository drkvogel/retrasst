#ifndef LOCALRUNIMPLH
#define LOCALRUNIMPLH

#include "API.h"
#include "CritSec.h"
#include <set>
#include <string>

namespace valc
{

class SampleRunGroupModel;

class LocalRun::Impl
{
public:
    Impl();
    void closeOff  ( const std::string& sampleRunID );
    int  getGroupID( const std::string& sampleRunID );
    void introduce ( LocalRun& lr, bool isOpen );
    bool isOpen    ( const std::string& sampleRunID ) const;
    void setSampleRunGroupModel( SampleRunGroupModel* m );
private:
    paulst::CritSec m_cs;
    std::set< std::string > m_openRuns;
    SampleRunGroupModel*    m_sampleRunGroupModel;
};

}

#endif

