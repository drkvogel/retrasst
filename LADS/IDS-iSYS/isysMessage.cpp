//---------------------------------------------------------------------------

#include <vcl.h>
#include "CommsProtocol.h"
#include "isysMessage.h"

#pragma hdrstop
#pragma package(smart_init)


//---------------------------------------------------------------------------

IsysMessage::IsysMessage(const String & rec)
 : record(rec)
{}

//---------------------------------------------------------------------------

IsysMessage::IsysMessage(const unsigned char * data, unsigned n)
 : record((char *) data, n)
{}

IsysMessage * IsysMessage::createHeader()
{
	// eg. H|\^_||||||||||P|2|20050110162817
	// 14 fields
	String msg;
    TDateTime time;
    time.FormatString("yyyymmddhhnnss");
    time = Now();

	//msg = "H|\\^_||||||||||P|2|" + time.FormatString("yyyymmddhhnnss") + "\r";
    msg = "H|\\^_||||||||||P|2|" + time.FormatString("yyyymmddhhnnss") + "\r";
	return new IsysMessage(msg);
}

IsysMessage * IsysMessage::createQRecord()
{
	return new IsysMessage("");
}

IsysMessage * IsysMessage::createPRecord()
{
	return new IsysMessage("");
}

/* Test Order Block

# ASTM Name 					Value 		Description and use in IDS-iSYS system
1 Record Type ID 				O 			Fixed value O for indicating an Order block.
2 Sequence Number 				1 			Number of the order within the Patient block starts at 1 and is increased for each Order block in the message.
3 Container and Specimen ID 	01^100      This field consists of 2 components:
											• The type of container used for the sample (2 digits).
											• 01 PT_11_4.5 mL.
											• 02 PT_13_6 mL.
											• 03 ST_13_5 mL H.
											• 05 Cup 500 ?L.
											• 06 Cup 2 mL.
											• 07 PT_16_10 ml.
											PT for Primary Tube; ST for Secondary Tube
											For correct functioning of the apparatus, it is important to allocate the correct type of container used for the sample.
											• The SID of the profile concerned by the request.
4 Instrument Specimen ID 					Not used.
5 Universal Test ID 			^^^123 		This ASTM field consists of 4 components:
											• Universal test ID.
											• Universal test ID Name.
											• Universal test ID Type.
											• Manufacturer’s Code.
											Only the last component is used and will be entered with the unique IDS-iSYS identifier of the analyte.
											In one single order it is possible to specify several tests by adding a repeat delimiter and by then adding one or several UTI thus obtaining a chain of the type ^^^123\^^^124.
6 Priority 						S 			This field may contain the values:
											• S = STAT.
											• A = ASAP.
											• R = Routine.
											• C = Callback.
											• P = Preoperative.
											For the device, only S has any influence, the passage of the associated profile into Urgent, and the other values are ignored.
7 Requested/Ordered	Date and Time			Not used.
8 Specimen Collection Date and Time			Not used.
9 Collection End Time 						Not used.
10 Collection Volume 						Not used.
11 Collector ID 							Not used.
12 Action Code 					A 			Action to be carried out on the block test or tests.
											This field may contain the values:
											• A for Add.
											• C for Cancel.
											• N for New.
											• P for Pending.
											• Q for QC.
											For the device, A and N are managed in the same way (new request or new rerun), C (cancellation of a request) can be managed if the test is not in progress. P and Q are not managed.
13 Danger Code 								Not used.
14 Relevant Clinical Information 			Not used.
15 Date and Time Specimen Received			Not used.
16 Specimen Descriptor 			S^ 			This field consists of two sections:
											• The sample type which can take for the IDS-iSYS the following values:
												• U = Urine.
												• S = Serum.
												• O = Other.
											• The sample source (place where the sample was taken) is not used.
17 Ordering Physician 						Doctor.
18 Physician Telephone Number 				Not used.
19 User Field 1 							Not used.
20 User Field 2 							Not used.
21 Laboratory Field 1 						Not used.
22 Laboratory Field 2 						Not used.
23 Date and Time Result Reported or Last Modified Not used.
24 Instrument Charge to Computer System   	Not used.
25 Instrument Section ID 					Not used.
26 Report Type 					O 			This field may take the values:
											• O = Order.
											• C = Correction.
											• P = Preliminary results.
											• F = Final results.
											• X = Cancel.
											• I = Instrument pending (in response to a request).
											• Y = No order on record for this test (in response to a request).
											• Z = No record for this patient (in response to a request).
											• Q = Response to a request.
											In the device C will not be managed.
27 Reserved Field 							Not used.
28 Location or Ward of Specimen Collection	Not used.
29 Nosocomial Infection Flag 				Not used.
30 Specimen Service 						Not used.
31 Specimen Institution 					Not used.

       seq
         container^sampleID
                 test ID: ^^^manufacturer's code
                        priority
                               action
                                   specimen descriptor: U, S, O
                                      doctor
                                               report type (O=order)
e.g. O|1|01^100||^^^123|S||||||||||S^||||||||||O|||||
*/

IsysMessage * IsysMessage::createORecord(
    String container = String("01"),  // this gets overridden anyway
    String sample = "",
	String codeList = "^^^25DRU",
    char chaPriority = 'R',     // routine
    char chaOAction = 'A',      // add
    char chaOSampleType = 'S',  // must be S to avoid bug in analyser software?
    char chaOReportType = 'O')
{
	String msg;
/*
                  seq
                    container^sampleID
                           test ID: ^^^manufacturer's code
                                 priority
                                        action
                                              specimen descriptor: U, S, O
                                                  doctor
                                                           report type (O=order)
*/
	msg.printf("O|1|%s^%s||%s|%c||||||%c||||%c^||||||||||%c|||||\r",
                    container.c_str(),
                       sample.c_str(),
							  codeList.c_str(),
                                 chaPriority,
                                        chaOAction,
                                              chaOSampleType,
														   chaOReportType);
	return new IsysMessage(msg);
}

IsysMessage * IsysMessage::createRRecord()
{
	return new IsysMessage("\r");
}

IsysMessage * IsysMessage::createTrailer()
{
	return new IsysMessage("L|1|N\r");
}

char IsysMessage::getRecordType() const
{
	return record.IsEmpty() ? '\0' : record[1];
}

//---------------------------------------------------------------------------
//	Return the given field from the current record; may be empty
//---------------------------------------------------------------------------
TDateTime IsysMessage::getDate(int field) const
{
	String isoDate = getField(field);
	if (isoDate.IsEmpty())
		return Now();

	unsigned short year = short(isoDate.SubString(1, 4).ToInt());
	unsigned short month = short(isoDate.SubString(5, 2).ToInt());
	unsigned short day = short(isoDate.SubString(7, 2).ToInt());
	unsigned short hour = short(isoDate.SubString(9, 2).ToIntDef(0));
	unsigned short min = short(isoDate.SubString(11, 2).ToIntDef(0));
	unsigned short sec = short(isoDate.SubString(13, 2).ToIntDef(0));
	unsigned short msec = short(isoDate.SubString(15, 2).ToIntDef(0));
	return TDateTime(year,  month, day, hour, min, sec, msec);
}

// return given field of message - assumes one-based indexing
String IsysMessage::getField(int num) const
{
	// TODO should read delimiter from header message ideally
	return split(record, num, "|" );
}

// return given part from given field of message
String IsysMessage::getPart( int field, int num ) const
{
	// TODO should read delimiter from header message ideally
	return split(getField(field), num, "^" );
}
