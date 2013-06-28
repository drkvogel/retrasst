//---------------------------------------------------------------------------

#ifndef isysResultH
#define isysResultH

#include "LIMSResult.h"
#include "isysMessage.h"

//---------------------------------------------------------------------------
//#include "isysMainForm.h"
//isysMainForm->((IsysDebugComms *)input)->printMessage(debugMsg);

class IsysResult : public CommsResults
{
public:
    enum enumResultStage {WAITING, HEADER, INFO_REQUESTED, PATIENT_DATA, TEST_ORDER, RESULT_DATA};
    static char * strResultStage(int stage) {
        char * strings[] = {"WAITING", "HEADER", "INFO_REQUESTED", "PATIENT_DATA", "TEST_ORDER", "RESULT_DATA"};
        return strings[stage];
    }
	IsysResult();
	~IsysResult();

	enumResultStage getResultStage() const { return resultStage; }
	String summary();	// counts cleared after they're reported

	// implementations of superclass virtual functions
	bool add(const CommsMessage & received, LQuery data, LQuery worklist);

private:
    enumResultStage resultStage;

//	int analysisID, testID;
	TStrings *noteList;
	String buff;

	int testID;

	// header block
	String 		strHDelimiters;
	String 		strHSenderName;
	String 		strHProcessingID;
	String 		strHVersionNo;
	TDateTime 	dtmHDateTime;

	// patient block
	int			intPSequenceNo;
	String 		strPLabPatientID;
	int			intPAnalysisID;
	String  	strPPatientFamilyName;
	String  	strPPatientGivenName;
	TDateTime 	dtmPPatientDOB;
	String  	strPPatientSex;
	String  	strPDoctorName;
	String  	strPPatientDiag;
	String  	strPHospDept;

	// test order block
	int			intOSequenceNo;
	int			intOContainerType;
	String  	strOSpecimenID;
	String  	strOManufacturerCode;
	String		strOPriority;
	String  	strOActionCode;
	String  	strOSpecimenDescriptor;
	String		strOReportType;

	// result block
	int			intRSequenceNumber;
	String  	strRManufacturerCode;
	String  	strRDataValue;
	String  	strRUnits;
	String  	strRRange;
	String  	strRAbnormalFlags;
	String  	strRAbnormality;
	String  	strRResultStatus;
	TDateTime  	dtmRDateStarted;
	TDateTime  	dtmRDateCompleted;
	String  	strRInstrumentID;

	// Request Information Block
	int			intQSequenceNumber;
//	int			intQPID;
//	int			intQSID;
	String 		strQPID;
	String		strQSID;
	String  	strQUniversalTestID;
	char		chaQStatusCode;

	// comment block
	int			intCSequenceNumber;
	char		chaCCommentSource;
	String  	strCCommentText;
	char		chaCCommentType;

	// message terminator
	char		chaLTerminationCode;

	short saved, notes;
	String  history;

	bool parseMessageHeader(const IsysMessage & msg);
	bool parsePatientInformation(const IsysMessage & msg);
	bool parseTestOrderRecord(const IsysMessage & msg);
	bool parseResultRecord(const IsysMessage & msg);
	bool parseInformationRequest(const IsysMessage & msg);
	bool parseCommentRecord(const IsysMessage & msg);
	bool parseMessageTerminator(const IsysMessage & msg);

	void checkValue(const String & result);
	void checkFlags(const String & flags);

	bool saveResult(LQuery & data, LQuery & worklist);
	bool updateAnalysis(LIMSResult & current, LQuery & qData, LQuery & qWorklist);
	bool addResultData(LIMSResult & current, LQuery & qData, LQuery & qWorklist);
	void updateSummary();
    void setResultStage(enumResultStage stage);
};

//---------------------------------------------------------------------------
#endif
