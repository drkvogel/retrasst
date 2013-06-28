//---------------------------------------------------------------------------

#ifndef LDbStageH
#define LDbStageH

#include "StrUtils.hpp"

//---------------------------------------------------------------------------

class LDbStage
{
	short stage;

public:

	enum Stage
	{
		NEW_ENTRY,		// incomplete, e.g. no worklist entry
		EXPECTED,		// matched to specimen entry
		ARRIVED,		// vacutainer swiped into system
		NOTIFIED,		// sample arrived but not expected
		ALLOCATED,		// cryovial IDs allocated
		ALIQUOTED,		// contents split into cryovials
		TRIGGERED,		// 2nd run - new test(s) triggered
		RE_RUN,			// 2nd run - first result out of range
		WIPED,			// previous results cleared from analyser
		PROGRAMMED,		// result expected from analyser
		ANALYSED,		// available to be validated
		L0_HELD,		// wating for more results
		L1_VALIDATED,	// ready for 2nd level validation
		L1_MARKED,		// marked for attention at level 2
		L2_VALIDATED,	// result to be sent to project
		INVALIDATED,	// result not to be sent to project
		TRANSMITTED,	// result(s) have been sent to project
		COLLECTED,		// results and cryovials accepted by project
		IN_TANK,        // specimen (box) has been stored
		DELETED			// record no longer required
	};

	LDbStage( Stage initial = NEW_ENTRY ) : stage( initial ) {}

	Stage getStage() const { return Stage( stage ); }
	void setStage( Stage newStage ) { stage = newStage; }

	String getStatus() const;
};

//---------------------------------------------------------------------------

#endif

