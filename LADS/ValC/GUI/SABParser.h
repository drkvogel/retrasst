#ifndef SABPARSERH
#define SABPARSERH

#include "API.h"

namespace valcui
{

class SAB;

/*
    Re. 'SAB', refer to SAB.h
*/
class SABParser
{
public:
    SABParser( valc::SnapshotPtr s );
    void parse( SAB* out ); 
private:
    valc::SnapshotPtr m_snapshot;
};

}

#endif

