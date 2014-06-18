#ifndef LOCALRUNIMPLH
#define LOCALRUNIMPLH

#include "API.h"
#include "CritSec.h"
#include "IDTokenSequence.h"
#include <vector>
#include <string>

namespace valc
{

class SampleRunGroupModel;

class LocalRun::Impl
{
public:
    Impl();
    void closeOff  ( const IDToken& sampleRunID );
    int  getGroupID( const IDToken& sampleRunID );
    void introduce ( LocalRun& lr, bool isOpen );
    bool isOpen    ( const IDToken& sampleRunID ) const;
    void setSampleRunGroupModel( SampleRunGroupModel* m );
private:
    paulst::CritSec m_cs;
    IDTokenSequence m_openRuns;
    SampleRunGroupModel*    m_sampleRunGroupModel;
};

}

#endif

