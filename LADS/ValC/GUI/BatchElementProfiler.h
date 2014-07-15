#ifndef BATCHELEMENTPROFILERH
#define BATCHELEMENTPROFILERH

#include "API.h"
#include "Nullable.h"

namespace valcui
{

class BatchElementProfiler
{
public:
	BatchElementProfiler();
	virtual ~BatchElementProfiler();
	virtual std::string characterizeGroup	(   bool QC ) = 0;
	virtual std::string characterizeRun     (   bool QC, bool isOpen ) = 0;
	virtual std::string characterizeResult  (   const valc::TestResult* r,
												const paulst::Nullable<valc::RuleResults>& rr,
												const valc::WorklistEntry* associatedWorklistEntry ) = 0;
	virtual std::string characterizePending (   const valc::WorklistEntry* ) = 0;
};

}

#endif

