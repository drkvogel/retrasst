//---------------------------------------------------------------------------

#ifndef BoxSummaryH
#define BoxSummaryH

#include <string>
#include <vector>
#include <set>
#include <map>
#include "WorkerThread.h"
#include "ValueFields.h"

class LQuery;
class LPDbBoxName;

//---------------------------------------------------------------------------

class BoxSummary {

	struct Cryovials {
		ValueFields filter;
		Cryovials( int aliquotTypeID );
		void addBarcode( const std::string & cryovial );
		short checkAnalyses( LQuery & pq, const std::set< int > & testProfileIDs );
	};

	struct Results {
		int boxID;
		short status, cryovials, analyses;
		Results( const LPDbBoxName & box );
		void checkContents();
	};

	class BoxCheckService {
		paulst::WorkerThread * workerThread;
	public:
		BoxCheckService();
		~BoxCheckService();
		void queue( Results * summary );
	};

	class BoxCheckTask : public paulst::WorkerThreadTask {
		Results * results;
	public:
		BoxCheckTask( Results * summary );
		~BoxCheckTask();
		virtual bool execute();
	};

	BoxCheckService * checker;
	std::map< std::string, Results * > checked;

public:

	enum {
		BOX_NOT_FOUND = -1,
		BOX_TYPE_NOT_FOUND = -2,
		NO_ANALYSES_REQUIRED = -3,
		CONTENTS_NOT_FOUND = -4,
		NOT_YET_CHECKED = -5,
		SPECIMEN_CHECKED = -6,
		READY_FOR_STORAGE = -7
	};

	BoxSummary();
	~BoxSummary();
	int check( const LPDbBoxName & box, short expected );
	void clear();
	void restart();
};

//---------------------------------------------------------------------------
#endif
