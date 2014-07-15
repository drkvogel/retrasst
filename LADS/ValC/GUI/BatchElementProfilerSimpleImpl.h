#ifndef BATCHELEMENTPROFILERSIMPLEIMPLH
#define BATCHELEMENTPROFILERSIMPLEIMPLH

#include "BatchElementProfiler.h"

namespace valcui
{

class BatchElementProfilerSimpleImpl : public BatchElementProfiler
{
public:
    BatchElementProfilerSimpleImpl();
	std::string characterizeGroup	(   bool QC );
	std::string characterizeRun     (   bool QC, bool isOpen );
	std::string characterizeResult  (   const valc::TestResult* r,
                                        const paulst::Nullable<valc::RuleResults>& rr,
                                        const valc::WorklistEntry* associatedWorklistEntry );
	std::string characterizePending (   const valc::WorklistEntry* );
};

}

#endif

