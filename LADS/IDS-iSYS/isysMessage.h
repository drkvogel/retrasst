//---------------------------------------------------------------------------

#ifndef isysMessageH
#define isysMessageH

#include "CommsProtocol.h"
//---------------------------------------------------------------------------

class IsysMessage : public CommsMessage, public CommsRecord
{
public:
	String record;

	IsysMessage(const String & rec);
	IsysMessage(const unsigned char * data, unsigned n);

	// patient block - these should really be defined commonly for IsysMessage and IsysResult
	int			intPSequenceNo;
	String 		strPLabPatientID;
	int			intPAnalysisID;
	String  	strPPatientFamilyName;
	String  	strPPatientGivenName;
	String  	strPPatientDOB;
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

	static IsysMessage * createHeader();
    static IsysMessage * createQRecord();
	static IsysMessage * createPRecord();
	static IsysMessage * createORecord(String container, String sample, String manufacturerCode, char chaPriority, char chaOAction, char chaOSampleType, char chaOReportType);
    static IsysMessage * createRRecord();
	static IsysMessage * createTrailer();

	// implementations of superclass virtual functions
	String getField(int num) const;
	String toString() const { return record; }

	// other funcs
	TDateTime getDate(int field) const;
	char getRecordType() const;
	String getPart(int field, int num) const;
};

//---------------------------------------------------------------------------
#endif
