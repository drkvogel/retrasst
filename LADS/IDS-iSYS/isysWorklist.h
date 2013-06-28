 //---------------------------------------------------------------------------

#ifndef isysWorklistH
#define isysWorklistH

#include <set>
#include "LIMSANalysis.h"
#include "CommsProtocol.h"
#include "isysMessage.h"

//---------------------------------------------------------------------------

class IsysWorklist : public CommsWorklist
{
public:
	enum enumWorklistState { WAITING, SENDING, PARTICIPANT };
	static char * strWorklistState(int state) {
		char * strings[] = { "WAITING", "SENDING", "PARTICIPANT" };
		return strings[state];
    }

//    enum enumWorklistState {WAITING, INFO_REQUESTED, INFO_H_SENT, INFO_P_SENT, INFO_O_SENT};
//    static char * strWorklistState(int state) {
//        char * strings[] = { "WAITING", "INFO_REQUESTED", "INFO_H_SENT", "INFO_P_SENT", "INFO_O_SENT"};
//        return strings[state];
//    }

//    String manufacturerCode;

	IsysWorklist() : worklistState(WAITING)
	{}
	enumWorklistState getState() { return worklistState; }
    bool setState(enumWorklistState state) { worklistState = state; return true; }

	void addPendingSamples(LQuery qCluster, LQuery qCentral);

	bool hasSummary() const;
	String summary();

	// implement virtual
	CommsMessage * nextRequest(LQuery qCluster, LQuery qCentral);

private:
    enumWorklistState worklistState;
	short patient;
	LIMSAnalysis current;
	std::set< String > programmed, cleared;

//    LIMSAnalysis next();
	IsysMessage * msgPRecord(LQuery & qCluster, LQuery & qCentral);
	IsysMessage * msgORecord(LQuery & qCluster, LQuery & qCentral);
};

//---------------------------------------------------------------------------

#endif
