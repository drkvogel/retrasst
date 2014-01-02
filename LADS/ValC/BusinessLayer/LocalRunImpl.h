#ifndef LOCALRUNIMPLH
#define LOCALRUNIMPLH

#include "API.h"
#include "CritSec.h"
#include <set>
#include <string>

namespace valc
{

class LocalRun::Impl
{
public:
    void closeOff( const std::string& sampleRunID );
    void introduce( LocalRun& lr, bool isOpen );
    bool isOpen  ( const std::string& sampleRunID ) const;
private:
    paulst::CritSec m_cs;
    std::set< std::string > m_openRuns;
};

}

#endif

