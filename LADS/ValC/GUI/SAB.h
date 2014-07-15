#ifndef SABH
#define SABH

#include "API.h"
#include "Nullable.h"

namespace valcui
{

/*
    Simple API for Batches

    Inspired (?) by Simple API for XML.

    A callback interface for notification of a sequence of parser events (see SABParser.h)
    which constitute a batch-oriented view of a sequence of LocalEntry instances (see API.h).
*/
class SAB
{
public:
    SAB();
    virtual ~SAB();
    virtual void eof      () = 0;
    virtual void group    (   bool QC, int groupID ) = 0;
    virtual void run      (   const valc::IDToken& runID, const std::string& barcode, bool isOpen ) = 0;
    virtual void result   (   const valc::TestResult* r,
                              const paulst::Nullable<valc::RuleResults>& rr, 
                              const valc::WorklistEntry* associatedWorklistEntry ) = 0;
    virtual void pending  (   const valc::WorklistEntry* ) = 0;
};

}

#endif

