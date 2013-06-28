#include <vcl.h>
#include <map>
#include <vector>
#include <exception>
#include <sysutils.hpp>
#include <StrUtils.hpp>

#include "leaseIDs.h"
#include "LQuery.h"
#include "LCDbProject.h"
#include "LPDbBoxes.h"
#include "LDbBoxType.h"
#include "LCDbBoxStore.h"
#include "LCDbAuditTrail.h"
#include "LPDbDescriptor.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"
#include "LDbStage.h"
#include "LPDbProfileMap.h"
#include "LPDbProfile.h"
#include "LDbSource.h"
#include "TfrmAboutBox.h"
#include "fmResolve.h"
#include "fmMain.h"
#include "fmLogin.h"
#include "fmReport.h"
#include "rijndael.h"
#include "devnote.h"
#include "LCDbBoxCourier.h"
#include "LIMSParams.h"
#include "LCDbAnalyser.h"
#include "LCDbRack.h"

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
TForm * TfrmLoginBase::mainForm = NULL;

RIJNDAEL	*rijndael = NULL;
bool use_rijndael = true;
char *keyFileName = "key.dat";
String keyFileLocation;
char * rijndael_key;
const int LCDbProjects::leaseID = LEASEE_BOXSYNC; //xxx
DEVICE_NOTIFY *dev;
bool debug_on = false;

const char * TfrmAboutBox::author = "Chris Bird";
const char * TfrmAboutBox::extension = "x3511";
const char * TfrmAboutBox::compiled = __DATE__;
const char * LCDbAuditTrail::default_email = "chris.bird@ctsu.ox.ac.uk"; // XXX

tdvecLBoxExpectedRecord     vecExpected;
tdvecLBoxExpectedRecord     vecWriteBuffer;
tdvecBoxArrivalRecord       vecTransferred;
tdvecBoxArrivalRecord       vecArrivals;
tdvecBoxArrivalRecord       vecSuccessful;
tdvecBoxArrivalRecord       vecErrorsToday;
tdvecBoxArrivalRecord       vecErrorsTotal;
tdvecBoxArrivalRecord       vecChanged;

bool diskArrive(const long lp) {
    return (frmMain->diskArrival(lp));
}

bool diskRemove(const long lp) {
    return (frmMain->diskRemoval(lp));
}

__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
	TfrmLoginBase::mainForm = this;
	logVisible = true;
    Height = Height - memoLog->Height;
    logVisible = false;
    formWidthNoLog = Width;
    formHeightNoLog = Height;
    formWidthWithLog = Width;
    formHeightWithLog = Height+100;
}

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
    initialise();
}

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    delete dev;
}

void __fastcall TfrmMain::FormCanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{
    Resize = logVisible;
}

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
    LCDbProjects::records().releaseDatabases();
}

void __fastcall TfrmMain::btnSyncClick(TObject *Sender) {
	doSync();
}

void __fastcall TfrmMain::Timer1Timer(TObject *Sender) {
	doSync();
}

void __fastcall TfrmMain::menuItemAboutClick(TObject *Sender) {
    frmAboutBox->ShowModal();
}

void __fastcall TfrmMain::btnResolveClick(TObject *Sender) {
    frmResolve->ShowModal();
}

void __fastcall TfrmMain::btnExitClick(TObject *Sender) {
    Close();
}

void TfrmMain::stripNewlines(String & message) {
    message = AnsiReplaceStr(message, "\n", "\\n");
    message = AnsiReplaceStr(message, "\r", "");
}

String TfrmMain::addToLog(String message, String messageTag = "other") {
	//xxxLCDbAuditTrail::getCurrent().addRecord(message);
	XMLFile::logError(messageTag.c_str(), message.c_str()); //xxx?
    stripNewlines(message);
    String temp;
	DateTimeToString(temp, "ddddd hh:mm:ss: ", Now());
    memoLog->Lines->Add( temp + message );
    return message;
}

void TfrmMain::queryExec(LQuery q) {
        q.execSQL();
}

void TfrmMain::updateStatus(String major, String minor) {
    if (!major.IsEmpty()) labelMajor->Caption = major;
    labelMinor->Caption = minor;
	addToLog(labelMajor->Caption + " | " + labelMinor->Caption);
    Application->ProcessMessages();
}

void TfrmMain::fatalError(String message, String where = "unknown") {
    errorThrown = true;
    minorError(message, where);
    Timer1->Enabled = false;
    Application->MessageBox(message.c_str(), "Error", MB_ICONEXCLAMATION);
    Timer1->Enabled = true;
	updateStatus("", "Error: " + message);
	LCDbProjects::records().releaseDatabases();
}

void TfrmMain::minorError(String message, String where = "unknown") {
    panelTime->Color = clRed;
    addToLog(strTemp.sprintf("Error: '%s' at %s", message, where), "error");
}

void __fastcall TfrmMain::OnDeviceChange(TMessage &Msg)
{
    if (NULL != dev) {
        dev->processMessage(Msg); // DETECT USB CHANGES
    }
}

bool TfrmMain::diskArrival(const long lp) {
    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lp;
    if (DBT_DEVTYP_VOLUME == pHdr->dbch_devicetype) {
        findKey();
    }
    return true;
}

bool TfrmMain::diskRemoval(const long lp) {
    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lp;
    if (DBT_DEVTYP_VOLUME == pHdr->dbch_devicetype) {
        findKey();
    }
    return true;
}

void TfrmMain::initialise() {

	addToLog("initialising application");
	labelTime->Caption  = "Not synced"; panelTime->Color = clRed;
	updateStatus("Ready.", "");

	DbFamily system;
	Caption = Caption + " *** " + system.getName() + " *** ";
	system.changeDbName(tdbLaptop, system.getCentralDb());
    addToLog(Caption);

    findKey();

    if (NULL == dev) {
        dev = new DEVICE_NOTIFY(); // DETECT ONLY AFTER LOGIN
        dev->setCallbackArrive(DBT_DEVTYP_VOLUME, diskArrive);
        dev->setCallbackRemove(DBT_DEVTYP_VOLUME, diskRemove);
    }
    errorThrown = false;
    Timer1->Enabled = true;

    TrayIcon1->BalloonHint = "Ready";
    TrayIcon1->ShowBalloonHint();

}

String TfrmMain::getSysErrorMessage() {
    char szBuf[256];
    int lastError = GetLastError();
    if (FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        lastError, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) szBuf, sizeof (szBuf), NULL)) {
        return String(szBuf);
    } else {
        return String("Error formatting error message");
    }
}

void TfrmMain::getEncryptionKey() {
    TStringList *tslKeyDat = new TStringList;
    try {
        if (0 == USBvol)
            throw Exception("USB key is not inserted - please insert");
        tslKeyDat->LoadFromFile(keyFileLocation);
        rijndael_key = new char[16];
        strcpy(rijndael_key, tslKeyDat->Strings[0].c_str());
        if (4 > strlen(rijndael_key) || 128 < strlen(rijndael_key)) //xxx
            throw Exception("Invalid encryption key.");
        rijndael = new RIJNDAEL(rijndael_key);
    } __finally {
        delete tslKeyDat;
    }
}

void TfrmMain::findKey() {
    int driveType;
    CHAR strDrive [4];
    USBvol = 0;
    DWORD dwDrives = GetLogicalDrives ();
    try {
        DWORD dwMask = 4096; // L
        for (int i=12; i>=3 && USBvol == 0; i--) { // check drives backwards, ending at D
            if (dwDrives & dwMask) { // it's a drive
                // create root path
                strDrive[0] = 'A'+i;
                strDrive[1] = ':';
                strDrive[2] = '\\';
                strDrive[3] = '\0';

                driveType = GetDriveType(strDrive);
                addToLog("Found drive "+String(strDrive)+":, type "+String(driveType));
                switch (driveType) {
                    case DRIVE_FIXED:
                    case DRIVE_REMOVABLE:
                        keyFileLocation.sprintf("%c:\\%s", strDrive[0], keyFileName);
                        if (FileExists(keyFileLocation)) {
                            USBvol = strDrive[0];
                            panelUSBIndicator->Color = clGreen;
                            panelUSBIndicator->Caption = "In";
                            break;
                        }
                    default:
                        ;
                }
            }
            dwMask >>= 1; // Next bit to the right
        }
    } catch (Exception &e) {
        minorError(e.Message);
    }

    if (!USBvol) {
        panelUSBIndicator->Color = clRed;
        panelUSBIndicator->Caption = "Out";
    }
}

void TfrmMain::checkKey() {
    if (debug_on) return;
    if (!USBvol) {
        throw Exception("Please insert key\n\nIf key is already plugged in, remove (after using 'Safely Remove Hardware' tray tool) and re-insert");
    }
    getEncryptionKey();
}

void TfrmMain::doSync()
{
    Timer1->Enabled = false;
    errorThrown = false;
    checkKey();

    //testing();

    try {
        memoLog->Lines->Clear();
        panelTransferred->Color = clBtnFace; panelSuccessful->Color = clBtnFace; panelErrorsToday->Color = clBtnFace; panelErrorsOlder->Color = clBtnFace; panelExpected->Color = clBtnFace;
        DateTimeToString(strTemp, "yyyy-mm-dd_hh_mm_ss", Now());
        LogFileName = LIMSParams::instance().getLogFolder() + Application->Title + " " + strTemp + "_log.txt";
        addToLog("Starting Storage Sync");
        currentProject = NULL;
        TDatabase * dbCentral = Session->FindDatabase(qCentral->DatabaseName);
		String leaseError = LCDbProjects::records().claimLease(dbCentral, 0);
        if (leaseError.IsEmpty()) {
            addToLog("Claimed lease on central database");
        } else {
            const String message = "Error claiming lease on central database: \n\n"+leaseError;
            addToLog(message);
            throw Exception(message);
        }
		LCDbAuditTrail::getCurrent().addRecord("Starting Storage Sync", LCDbAuditTrail::INFORMATION );

		updateStatus("Reading central database", "");
        progressMajor->Position = 0; progressMinor->Position = 0; Application->ProcessMessages();
        LCDbObjects::records().read(qCentral, true);
        LCDbBoxSizes::records().read(qCentral, true);
        LCDbProjects::records().read(qCentral, false);
		short nproj = 0; for (Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr) { nproj++; };
        progressMajor->Max = nproj + 6;
        vecExpected.clear(); vecSuccessful.clear(); vecArrivals.clear(); vecErrorsToday.clear(); vecErrorsTotal.clear(); refreshTally();

        copyAuditTrail();
        syncArrivals();
        getExpected();
        syncCOperator();
        copyTables();
        getCIDs();
        updateSyncTime();

		LCDbAuditTrail::getCurrent().addRecord("Storage sync finished successfully", LCDbAuditTrail::COMPLETED );
    } catch (Exception &e) {
        fatalError(e.Message, __FUNC__);
    } catch (std::exception &e) {
        fatalError(e.what(), __FUNC__);
    } catch (char * str) {
        fatalError(String(str), __FUNC__);
    } catch (Exception * e) {
        fatalError(e->Message, __FUNC__);
    } catch (...) {
        fatalError("Unknown exception", __FUNC__);
    }
	LCDbProjects::records().releaseDatabases();
    memoLog->Lines->SaveToFile(LogFileName);

    if (errorThrown) {
        Application->MessageBox("There were errors. Please view the log file.", "Errors encountered", MB_ICONEXCLAMATION);
    } else {
        panelTime->Color = clGreen;
    }
    Timer1->Enabled = true;

    delete rijndael;
}

void TfrmMain::updateSyncTime() {
	updateStatus("Sync with LIMS finished.", "");
    strTemp = "Last synced at " + TDateTime::CurrentDateTime().DateTimeString();
    labelTime->Caption = strTemp;
    TrayIcon1->BalloonHint = strTemp;
    TrayIcon1->ShowBalloonHint();

    static char * EVENT_CODE = "StorageSync";
    const LCDbObject * event = LCDbObjects::records().find(EVENT_CODE, LCDbObject::SIS_EVENT);
    insertEvent(event->getID());

//    LQuery qc(qCentral);
//    LCDbID myLCDbID;
//    myLCDbID.claimNextID(qc);
//    qc.setSQL(strTemp.sprintf(
//        "INSERT INTO c_event_log (event_cid, event_code, machine_cid, test_cid, related_id, operator_cid, event_date, process_cid)"
//        " VALUES (%d, -197500, %d, 0, 0, %d, :now, %d)",
//        myLCDbID.getID(),
//        LCDbAnalysers::getCurrentID(),
//        LCDbOperators::getCurrentID(),
//        LCDbAuditTrail::getProcessID()
//        ));
//    qc.setParam("now", Now());
//    qc.execSQL();
}

void TfrmMain::insertEvent(int event_code) {
    LQuery qc(qCentral);
    LCDbID myLCDbID;
    myLCDbID.claimNextID(qc);
    qc.setSQL(strTemp.sprintf(
        "INSERT INTO c_event_log (event_cid, event_code, machine_cid, test_cid, related_id, operator_cid, event_date, process_cid)"
        " VALUES (%d, %d, %d, 0, 0, %d, :now, %d)",
        myLCDbID.getID(),
        event_code,
        LCDbAnalysers::getCurrentID(),
        LCDbOperators::getCurrentID(),
        LCDbAuditTrail::getProcessID()
        ));
    qc.setParam("now", Now());
    qc.execSQL();
}

// Get a batch of CIDs for Andrew's program (Box Reception) to use
void TfrmMain::getCIDs()
{
    LQuery qc(qCentral), ql(qLaptop);
	updateStatus("Getting CIDs", "");

	LCDbID myLCDbID;
	myLCDbID.claimNextID(qc, Config::numIDs);
	int nextid = myLCDbID.getID();
	LDbNextID::clearAll();

	ql.setSQL("DELETE FROM c_next_cid");
	queryExec(ql);
	ql.setSQL(strTemp.sprintf("INSERT INTO c_next_cid (next_cid, available_range) VALUES (%d, %d)", nextid, Config::numIDs));
	queryExec(ql);

	progressMajor->StepIt();
	Application->ProcessMessages();
}

void TfrmMain::getExpected()
{
	updateStatus("Getting expected boxes", "");
	LQuery q(qLaptop);
	q.setSQL("DELETE FROM l_box_expected");
	queryExec(q);

	String status;
	for (Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr) {
		try {
			if( pr->isActive() ) {
				switchProject((*pr).getID());
				status = "Getting expected boxes from project '" + currentProject.getName() + "'";
				updateStatus(status, "");
				int count = currentProject.hasBoxes()
						? getExpectedKadoorieType(currentProject)
						: getExpectedThriveType(currentProject);
				status = String( count ) + " boxes from project '" + currentProject.getName() + "'";
				updateStatus(status, "");
				LCDbAuditTrail::getCurrent().addRecord( status, LCDbAuditTrail::COMPLETED );
			}
		} catch (Exception & e) {
			minorError(e.Message, __FUNC__); // xxx catch project-wide errors, move on to next project
		}
		progressMajor->StepIt(); Application->ProcessMessages();
	}
    progressMinor->Position = progressMinor->Max - 1; //xxx
    updateStatus("", "Finished getting expected boxes");
}

// Get records from specimen using descrip
int TfrmMain::getExpectedKadoorieType(LCDbProject pr) // eg. t_ldb17
{
    LQuery q(qProject);
    tdmapFields mapFields = getSpecimenFieldNames();
    strTemp.sprintf(
        "SELECT sample_id, %s, status, %s, %s, %s, %s FROM specimen"
        " WHERE status = 0", // xxx status 0 is a condition of LDbStage::EXPECTED
        mapFields["box_label"],
        mapFields["first_barcode"],
        mapFields["first_position"],
        mapFields["last_barcode"],
        mapFields["last_position"]);
    q.setSQL(strTemp);
	int nrows = q.open(); if (0 == nrows) return 0;
    progressMinor->Max = nrows; progressMinor->Position = 0;
    updateStatus("", "Getting records");

    vecWriteBuffer.clear();
	while (!q.eof()) {
        LBoxExpectedRecord NewRecord(
            q.readInt("sample_id"),                     // box_expected_cid
            pr.getID(),                                 // project_cid
            q.readString(mapFields["box_label"]),       // box_name
            q.readString(mapFields["first_barcode"]),
            q.readString(mapFields["first_position"]).ToIntDef(-1), // should give -1 for '.' e.g. for mexico project
            q.readString(mapFields["last_barcode"]),
            q.readString(mapFields["last_position"] ).ToIntDef(-1)  // should give -1 for '.' e.g. for mexico project
        );
        vecWriteBuffer.push_back(NewRecord);
        q.next();
        progressMinor->StepIt(); Application->ProcessMessages();
	}
	writeToLBoxExpected();
	return nrows;
}

// get records from box_name, cryovial_store and cryovial
int TfrmMain::getExpectedThriveType(LCDbProject pr)   // eg. t_ldb20
{
	LQuery q(qProject);
	q.setSQL(strTemp.sprintf("SELECT box_cid, external_name FROM box_name WHERE status IN (%d)AND box_capacity = 0", LPDbBoxName::CONFIRMED));
	int nrows = q.open(); if (0 == nrows) return 0;
	progressMinor->Max = nrows; progressMinor->Position = 0;
	updateStatus("", "Getting records");

	vecWriteBuffer.clear();
	while (!q.eof()) {
		LBoxExpectedRecord box(q.readInt("box_cid"), pr.getID(), q.readString("external_name"));
		vecWriteBuffer.push_back(box);
		q.next(); refreshTally();
		progressMinor->StepIt(); Application->ProcessMessages();
	}

	// get first_position and last_position
	progressMinor->Position = 0;
    updateStatus("", "Getting positions");
	tdvecLBoxExpectedRecord::iterator boxIt;
	for (boxIt = vecWriteBuffer.begin(); boxIt != vecWriteBuffer.end(); boxIt++) {
		getThriveTypeCryovialSlotPositions(*boxIt, q);
		progressMinor->StepIt(); Application->ProcessMessages();
	}

	// get first_barcode and last_barcode
	progressMinor->Position = 0;
    updateStatus("", "Getting barcodes");
	for (boxIt = vecWriteBuffer.begin(); boxIt != vecWriteBuffer.end(); boxIt++) {
		getThriveTypeCryovialBarcodes(*boxIt, q);
		progressMinor->StepIt(); Application->ProcessMessages();
	}
	writeToLBoxExpected();
	return nrows;
}

String TfrmMain::encryptBarcode(String input) {
    unsigned char *encrypted;
    unsigned char unencrypted[32];

    strcpy(unencrypted, input.c_str()); // avoid using c_str() result directly

    if (0 == strlen(unencrypted) || unencrypted[0] == '.') {
        encrypted = ".";
    } else {
        encrypted = rijndael->encryptSz2Base64(unencrypted);
    }

    String result = String((char *)encrypted);
    if (NULL != encrypted && encrypted[0] != '.')
        free(encrypted);

    return result;
}

String TfrmMain::decryptBarcode(String input) {
	String result;
	int dummy_len;
    char *out = NULL;

    if (input == ".") {
        return input;
    }

	if (rijndael->decryptBase64((unsigned char **)&out, &dummy_len, input.c_str())) {
		result = String(out);
		free( out );
	} else {
		//xxxbarcode_unencrypted = true; // should have been encrypted
		result = input;
		minorError("Unencryption failed (source not encrypted?): "+input); // xxx?
	}
	return result;
}

void TfrmMain::writeToLBoxExpected()
{
    progressMinor->Max = vecWriteBuffer.size(); progressMinor->Position = 0;
    updateStatus("", "Writing records");
    LQuery q(qLaptop);
    tdvecLBoxExpectedRecord::iterator nr;

    for (nr = vecWriteBuffer.begin(); nr != vecWriteBuffer.end(); nr++) {
        strTemp.sprintf(
            "INSERT INTO l_box_expected"
            " (box_expected_id, project_cid, box_name,"
            " first_barcode, first_position,"
            " last_barcode, last_position)"
            " VALUES"
            " (%d, %d, '%s', '%s', %d, '%s', %d)",
            (*nr).box_expected_id, (*nr).project_cid, (*nr).box_name,
            encryptBarcode((*nr).first_barcode), (*nr).first_position,
            encryptBarcode((*nr).last_barcode),  (*nr).last_position);
        q.setSQL(strTemp); queryExec(q);
        vecExpected.push_back(*nr);
        addToLog("NewRecord: "+(*nr).print());
        progressMinor->StepIt(); refreshTally(); Application->ProcessMessages();
    }
    vecWriteBuffer.clear();
}

void TfrmMain::switchProject(int cid) {
    const LCDbProject * proj = LCDbProjects::records().findByID(cid);
    if (NULL == proj) {
        throw SSMinorException(BoxRecord::INVALID_PROJECT, "Invalid project ID: "+String(cid));
    }
    currentProject = *proj; //xxx currentProject should be a LCDbProject * ?
	String prefix;
	switch (DbFamily().getDbSystem()) {
		 default:
			prefix = "t_";
			break;
		case DbFamily::TEST_DATA:
			prefix = "test_";
	}
	if (prefix != LeftStr(currentProject.getDbName(), prefix.Length())) {
		throw Exception("Tried to connect to database '"+currentProject.getDbName()+"' ('"+currentProject.getName()
				+"') when testing status is "+String(DbFamily().getDbSystem()));
	}
	TDatabase * dbProj      = Session->FindDatabase(qProject->DatabaseName);
	TDatabase * dbCentral   = Session->FindDatabase(qCentral->DatabaseName);
	proj->claim(dbCentral, dbProj);
	LCDbAuditTrail::getCurrent().addRecord(addToLog("Changing to: "+currentProject.getName()), LCDbAuditTrail::DB_CHANGE );

    LPDbBoxTypes::records().read(qProject, true);
    LPDbDescriptors::records().read(qProject, true);
    LPDbProfiles::records().read(qProject, true);
    LPDbProfileMaps::records().read(qProject, true);
}

void TfrmMain::useCentralDB() {
    TDatabase * owner = Session->FindDatabase(qCentral->DatabaseName);
	owner->Close();
	owner->Params->Values["DATABASE NAME"] = DbFamily().getCentralDb();
}

int TfrmMain::getKadoorieBoxType(BoxArrivalRecord rec) {
    LQuery qp(qProject), qc(qCentral);

    tdmapFields mapFields = getSpecimenFieldNames();
    const LPDbProfile * profile = NULL;
    const LPDbSource * sample = LPDbSources::records().readRecord(qp, rec.box_arrival_id);
    if (sample != NULL && sample->getStage() != LDbStage::DELETED) {
		const LPDbProfileMap * pm = sample->findProfileMap();
		if( pm != NULL )
			profile = LPDbProfiles::records().findByID(pm -> getProfileID());
    }
    if (profile != NULL) {
        Range< LPDbProfile::BoxType > bt = profile->getBoxTypes();
        if (bt.size() == 1)
            return bt -> getID();
    }
    throw SSMinorException(BoxRecord::CONFIG_ERROR, "Cannot identify box content for box ID " + String(rec.box_arrival_id)+" in "+currentProject.getDbName());
}

String TfrmMain::getSite(int tank_cid) {
    static std::map<int, String> tankToSite;
    if (tankToSite.find(tank_cid) == tankToSite.end()) {
        LQuery qc(qCentral);
        qc.setSQL("SELECT storage_cid FROM c_tank_map WHERE tank_cid = :tkid AND STATUS != 99");
        qc.setParam("tkid", tank_cid);
        if (1 != qc.open())
            throw SSMinorException(BoxRecord::UNKNOWN_ERROR, strTemp.sprintf("!1 rows for tank_cid %d found in c_tank_map", tank_cid));
        int stid = qc.readInt("storage_cid");
        qc.setSQL("SELECT location_cid FROM c_storage_details WHERE storage_cid = :stid AND STATUS != 99");
        qc.setParam("stid", stid);
        if (1 != qc.open())
            throw SSMinorException(BoxRecord::UNKNOWN_ERROR, strTemp.sprintf("!1 rows for storage_cid %d found in c_storage_details", stid));
        int obid = qc.readInt("location_cid");
        qc.setSQL("SELECT external_name FROM c_object_name WHERE object_cid = :obid AND STATUS != 99");
        qc.setParam("obid", obid);
        if (1 != qc.open())
            throw SSMinorException(BoxRecord::UNKNOWN_ERROR, strTemp.sprintf("!1 rows for object_cid %d found in c_object_name", obid));;
        tankToSite[tank_cid] = qc.readString("external_name");
    }
    return (tankToSite[tank_cid]);
}

tdmapFields TfrmMain::getSpecimenFieldNames()
{
    // retrieve field names from descrip table
    const int NUM_FIELDS_DESCRIP = 7;
    String strFields[] = {  "box_label", "first_barcode", "first_position",
                            "last_barcode", "last_position",
                            "aliquot_type", "box_project"};
    tdmapFields mapFields;
    LQuery q(qProject);
    for (int i=0; i<NUM_FIELDS_DESCRIP; i++) {
        const LPDbDescriptor * descrip =
            LPDbDescriptors::records().findByName(strFields[i]);
        if (descrip != NULL) {
            mapFields[strFields[i]] = descrip->getSpecimenField();
        } else {
            throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "No specimen field for "+strFields[i]+" descriptor");
        }
    }
    return mapFields;
}

void TfrmMain::syncArrivals() {
	getArrivalsFromLaptop();
	processArrivals();
    updateArrivalsToLaptop();
    if (debug_on) {
        throw Exception("Syncing arrivals only for debug");
    }
}

// sync labs and laptop versions of l_box_arrival
void TfrmMain::getArrivalsFromLaptop()
{
    // nothing should go wrong here, but if it does, allow to bomb out and abort sync
    vecTransferred.clear();
    progressMinor->Position = 0; progressMajor->StepIt();
    updateStatus("Getting new arrivals", "");

    LQuery ql(qLaptop), qp(qProject), qc(qCentral);

    // get the last laptop sync time
    static char * EVENT_CODE = "StorageSync";
    const LCDbObject * event = LCDbObjects::records().find(EVENT_CODE, LCDbObject::SIS_EVENT);

    qc.setSQL(strTemp.sprintf(
        "SELECT MAX(event_date) FROM c_event_log"
        " WHERE event_code = %d AND machine_cid = %d",
        event->getID(), LCDbAnalysers::getCurrentID()));
    int count = qc.open();
    TDateTime lastLapSync = qc.readDateTime(0);

    // pull over new records from laptop
    ql.setSQL(strTemp.sprintf(
		"SELECT * FROM l_box_arrival WHERE time_stamp > :lastLapSync ORDER BY project_cid, swipe_time"));
	ql.setParam("lastLapSync", lastLapSync);
	count = ql.open();
	updateStatus("", strTemp.sprintf("Reading %d arrivals", count));
	progressMinor->Max = count; progressMinor->Position = 0; Application->ProcessMessages();
	while (!ql.eof()) {
        BoxArrivalRecord NewRecord(
            ql.readInt("laptop_cid"),
            ql.readInt("process_cid"),
            ql.readInt("box_arrival_id"),
            ql.readInt("project_cid"),
            ql.readDateTime("swipe_time"),
            ql.readString("box_name"),
            ql.readInt("status"), //xxx 5 = allocated, 6 = slot confirmed, 7 = referred
			decryptBarcode(ql.readString("first_barcode")), ql.readInt("first_position"),
            decryptBarcode(ql.readString("last_barcode")),  ql.readInt("last_position"),
            ql.readInt("tank_cid"), ql.readString("rack_number"), ql.readInt("slot_position"),
            ql.readDateTime("time_stamp")
            //, key_hash?
		);

        qc.setSQL(strTemp.sprintf(
			"SELECT * FROM l_box_arrival WHERE project_cid = %d AND box_arrival_id = %d ",
            NewRecord.project_cid, NewRecord.box_arrival_id));
        count = qc.open();
        if (0 != count) {
            //minorError("Box arrival already in LIMS: "+NewRecord.print());
            qc.setSQL(strTemp.sprintf(
				"UPDATE l_box_arrival"
				" SET laptop_cid = %d, process_cid = %d,"
				" swipe_time = :st, status = %d,"
				" first_barcode = :fb, first_position = %d,"
                " last_barcode = :lb, last_position = %d,"
				" tank_cid = %d, rack_number = '%s', slot_position = %d, time_stamp = :ts"
                " WHERE box_arrival_id = %d AND project_cid = %d",
				NewRecord.laptop_cid, NewRecord.process_cid,
                NewRecord.status,
				NewRecord.first_position,
				NewRecord.last_position,
                NewRecord.tank_cid, NewRecord.rack_number, NewRecord.slot_position,
                NewRecord.box_arrival_id, NewRecord.project_cid));
			qc.setParam("st", NewRecord.swipe_time);
            qc.setParam("fb", NewRecord.first_barcode);
            qc.setParam("lb", NewRecord.last_barcode);
            qc.setParam("ts", Now());
            addToLog("LIMS <-- laptop : Updated existing box : "+strTemp);
        } else {
            qc.setSQL(strTemp.sprintf(
				"INSERT INTO l_box_arrival"
				" (laptop_cid, process_cid, box_arrival_id, project_cid, "
				" swipe_time, box_name, status,"
				" first_barcode, first_position, last_barcode, last_position,"
				" tank_cid, rack_number, slot_position, time_stamp)"
                " VALUES (%d, %d, %d, %d,"
                " :st, '%s', %d,"
                " '%s', %d, '%s', %d,"
				" %d, '%s', %d, :ts)",
				NewRecord.laptop_cid, NewRecord.process_cid,
                NewRecord.box_arrival_id, NewRecord.project_cid,
                NewRecord.box_name, NewRecord.status,
				NewRecord.first_barcode, NewRecord.first_position,
				NewRecord.last_barcode, NewRecord.last_position,
                NewRecord.tank_cid, NewRecord.rack_number, NewRecord.slot_position));
			qc.setParam("st", NewRecord.swipe_time);
            qc.setParam("ts", Now());
            addToLog("LIMS <-- laptop : Inserted new box : "+strTemp);
        }
        queryExec(qc);
        vecTransferred.push_back(NewRecord);
        refreshTally();
        progressMinor->StepIt(); Application->ProcessMessages();
        ql.next();
	}

    // copy l_box_arrival_event_history
	copyTable(qLaptop, qCentral, "l_box_arrival_event_history", "l_box_arrival_event_history", false);
    ql.setSQL("DELETE FROM l_box_arrival_event_history");
	ql.execSQL();
}

void TfrmMain::processArrivals() {
    LQuery qc(qCentral), qp(qProject), ql(qLaptop);

    // now get the whole list of unprocessed arrivals from the labs l_box_arrival
    vecArrivals.clear();
    qc.setSQL(strTemp.sprintf(
                "SELECT laptop_cid, process_cid, box_arrival_id, project_cid,"
                " swipe_time, box_name, status,"
                " first_barcode, first_position,"
                " last_barcode, last_position,"
                " tank_cid, rack_number, slot_position, time_stamp"
				" FROM l_box_arrival"
				" WHERE status != %d"
                " ORDER BY project_cid, time_stamp DESC",
                LPDbBoxName::IN_TANK ));
    int count = qc.open();
    updateStatus("", strTemp.sprintf("Reading %d arrivals", count));
    progressMinor->Max = count; progressMinor->Position = 0; Application->ProcessMessages();
    while(!qc.eof()) {
        BoxArrivalRecord NewRecord(
            qc.readInt("laptop_cid"), qc.readInt("process_cid"), qc.readInt("box_arrival_id"),
            qc.readInt("project_cid"),  qc.readDateTime("swipe_time"), qc.readString("box_name"),
            qc.readInt("status"), //xxx 5 = allocated, 6 = slot confirmed, 7 = referred
            qc.readString("first_barcode"), qc.readInt("first_position"),
            qc.readString("last_barcode"), qc.readInt("last_position"),
            qc.readInt("tank_cid"), qc.readString("rack_number"),
            qc.readInt("slot_position"), qc.readDateTime("time_stamp")
        );
        vecArrivals.push_back(NewRecord); qc.next();
        refreshTally();
        progressMinor->StepIt(); Application->ProcessMessages();
    }
    progressMinor->Max = vecArrivals.size(); progressMinor->Position = 0;
    updateStatus("", "Processing arrivals");
    tdvecBoxArrivalRecord::const_iterator arIt;
	currentProject.setID(LCDbProject::NONE_SELECTED);
    for (arIt = vecArrivals.begin(); arIt != vecArrivals.end(); arIt++) {
        BoxArrivalRecord rec = *arIt;
        addToLog("Box arrival: "+rec.print());
        try {
			if (LCDbProject::NONE_SELECTED == currentProject.getID() || currentProject.getID() != rec.project_cid) {
                switchProject(rec.project_cid);
            }
            if (rec.box_arrival_id < 0) { // an unexpected box
                throw SSMinorException(BoxRecord::UNEXPECTED, "Box " + rec.box_name + " id "+ String(rec.box_arrival_id) + " is unexpected"); }
			if (rec.status != LCDbBoxStore::SLOT_CONFIRMED) {
                throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "Box " + rec.box_name + " id "+ String(rec.box_arrival_id) + " is not confirmed"); }
            if (currentProject.hasBoxes()) {
                processKadoorieType(rec);
            } else {
                processThriveType(rec);
            }

//            // copy box event history into project db
//			qc.setSQL(strTemp.sprintf("SELECT * FROM l_box_arrival_event_history"
//					" WHERE box_arrival_id = %d AND project_cid = %d", rec.box_arrival_id, rec.project_cid));
//			for( qc.open(); !qc.eof(); qc.next() ) {
//				qp.setSQL(strTemp.sprintf(
//					"INSERT INTO box_event_history (box_id, event_cid, operator_cid, event_date, process_cid)"
//					" VALUES (%d, %d, %d, :event_date, %d)",
//					rec.box_arrival_id, qc.readInt("event_cid"), qc.readInt("operator_cid"), qc.readInt("process_cid")));
//				qp.setParam("event_date", qc.readDateTime("event_date"));
//				queryExec(qp);
//			}
            addToLog("Stored arrival to project "+currentProject.getDbName()+" : "+rec.print());
            vecSuccessful.push_back(rec);
        } catch (Exception & e) {
            rec.errorMessage = e.Message;
            strTemp.sprintf("'%s' [%s]", e.Message, rec.print());
            minorError(strTemp, __FUNC__);
            if (rec.time_stamp > TDateTime::CurrentDate()) { // 00:00 AM today
                vecErrorsToday.push_back(rec);
            }
            vecErrorsTotal.push_back(rec);
            addToLog("Error storing arrival to project "+currentProject.getDbName()+" : "+rec.print());
        }

        // 'finally'
        qc.setSQL(strTemp.sprintf("SELECT status FROM l_box_arrival" // get current status
            " WHERE box_arrival_id = %d AND project_cid = %d", rec.box_arrival_id, rec.project_cid));
        if (1 != qc.open()) throw Exception("status not found: "+strTemp);
        int currentStatus = qc.readInt("status");
        if (rec.status != currentStatus) {
            qc.setSQL(strTemp.sprintf(
                "UPDATE l_box_arrival SET status = %d"
                " WHERE box_arrival_id = %d AND project_cid = %d",
                rec.status, rec.box_arrival_id, rec.project_cid));
            queryExec(qc);
        } else {
            addToLog(strTemp.sprintf("box %d project %d status %d unchanged", rec.box_arrival_id, rec.project_cid, rec.status)); 
        }
        refreshTally();
        progressMinor->StepIt(); Application->ProcessMessages();
    }
    progressMajor->StepIt(); Application->ProcessMessages();
}

// update arrivals on laptop if they have changed in LIMS
// insert if they are not already there (e.g. fresh laptop database)
void TfrmMain::updateArrivalsToLaptop()
{
    LQuery ql(qLaptop), qp(qProject), qc(qCentral);

    //static char * EVENT_CODE = "StorageSync";
    const LCDbObject * event = LCDbObjects::records().find("StorageSync", LCDbObject::SIS_EVENT);
    int event_sync_id = event->getID();

    event = LCDbObjects::records().find("NewLaptop", LCDbObject::SIS_EVENT);
    int event_newlaptop_id = event->getID();

    qc.setSQL(strTemp.sprintf(
        "SELECT event_code, log_date FROM c_event_log"
        " WHERE machine_cid = %d ORDER BY log_date DESC", LCDbAnalysers::getCurrentID()));
    int count = qc.open();
    TDateTime lastLabsSync = (0 == count ? NULL : qc.readDateTime("log_date"));

    String query;
    query = "SELECT laptop_cid, process_cid, box_arrival_id, project_cid,"
            " swipe_time, box_name, status,"
            " first_barcode, first_position, last_barcode,"
            " last_position, tank_cid, rack_number, slot_position, time_stamp"
            " FROM l_box_arrival";

    if (0 == count) {   // A new laptop, insert a 'NewLaptop' event
        insertEvent(event_newlaptop_id);
        addToLog("Unrecognized machine - added 'NewLaptop' event to c_event_log");
        query += " ORDER BY project_cid";
        qc.setSQL(query);
    } else if (qc.readInt("event_code") == event_sync_id) {
        query += " WHERE time_stamp > :lastLabSync ORDER BY project_cid";
        qc.setSQL(query);
        qc.setParam("lastLabSync", lastLabsSync);
    } else if (qc.readInt("event_code") != event_newlaptop_id) {
        throw Exception(strTemp.sprintf("Unknown event_code: %d", qc.readInt("event_code")));
    } else { // get all boxes
        query += " ORDER BY project_cid";
        qc.setSQL(query);
    }

    // copy changed records from labs

    count = qc.open();
    updateStatus("Getting "+String(count)+" updated arrivals from labs", "");
    vecChanged.clear();
    progressMinor->Max = count; progressMinor->Position = 0; Application->ProcessMessages();
    while (!qc.eof()) {
        BoxArrivalRecord NewRecord(
            qc.readInt("laptop_cid"),
            qc.readInt("process_cid"),
            qc.readInt("box_arrival_id"),
            qc.readInt("project_cid"),
            qc.readDateTime("swipe_time"),
            qc.readString("box_name"),
            qc.readInt("status"), //xxx 5 = allocated, 6 = slot confirmed, 7 = referred
            qc.readString("first_barcode"), qc.readInt("first_position"),
            qc.readString("last_barcode"),  qc.readInt("last_position"),
            qc.readInt("tank_cid"), qc.readString("rack_number"), qc.readInt("slot_position"),
			qc.readDateTime("time_stamp")
            //, key_hash?
        );
        vecChanged.push_back(NewRecord);
        refreshTally();
        progressMinor->StepIt(); Application->ProcessMessages();
        qc.next();
    }

    // insert into laptop l_box_arrival
    updateStatus("Update laptop arrival list", "");
    progressMinor->Max = vecChanged.size(); progressMinor->Position = 0;
    updateStatus("", "Updating arrivals");
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecChanged.begin(); arIt != vecChanged.end(); arIt++) {
        BoxArrivalRecord rec = *arIt;

        // is it in the table already?
        ql.setSQL(strTemp.sprintf(
			"SELECT * FROM l_box_arrival"
			" WHERE project_cid = %d AND box_arrival_id = %d AND process_cid = %d",
			rec.project_cid, rec.box_arrival_id, rec.process_cid));
        count = ql.open();
		if (0 != count) { // yes, update it
            ql.setSQL(strTemp.sprintf(
				"UPDATE l_box_arrival SET status = %d, time_stamp = :ts"
				" WHERE project_cid = %d AND box_arrival_id = %d AND process_cid = %d",
				rec.status, rec.project_cid, rec.box_arrival_id, rec.process_cid));
			ql.setParam("ts", Now()); // laptop local time //xxx
			ql.execSQL();
            addToLog("LIMS --> laptop : Updated existing box : "+strTemp);
		} else { // no, insert it
			ql.setSQL(strTemp.sprintf(
				"INSERT INTO l_box_arrival"
				" (laptop_cid, process_cid, box_arrival_id, project_cid, "
				" swipe_time, box_name, status,"
				" first_barcode, first_position, last_barcode, last_position,"
				" tank_cid, rack_number, slot_position, time_stamp)"
				" VALUES (%d, %d, %d, %d,"
				" :st, '%s', %d,"
				" '%s', %d, '%s', %d,"
				" %d, '%s', %d, :ts)",
				rec.laptop_cid, rec.process_cid, rec.box_arrival_id, rec.project_cid,
				rec.box_name, rec.status,
				encryptBarcode(rec.first_barcode), rec.first_position,
				encryptBarcode(rec.last_barcode),  rec.last_position,
				rec.tank_cid, rec.rack_number, rec.slot_position));
			ql.setParam("st", rec.swipe_time);
			ql.setParam("ts", Now()); // laptop local time //xxx
			ql.execSQL();
            addToLog("LIMS --> laptop : Inserted new box : "+strTemp);
        }
        progressMinor->StepIt(); Application->ProcessMessages();
    }
	refreshTally();
}

void TfrmMain::processKadoorieType(BoxArrivalRecord & rec) {

    if (rec.box_arrival_id < 0) { // an unexpected box
        throw SSMinorException(BoxRecord::UNEXPECTED, "Box " + rec.box_name + " id "+ String(rec.box_arrival_id) + " is unexpected");
    }

	if (rec.status != LCDbBoxStore::SLOT_CONFIRMED) {
        throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "Box " + rec.box_name + " id "+ String(rec.box_arrival_id) + " is not confirmed");
    }

	// site, tank, rack, slot
	LCDbRack store( getSite(rec.tank_cid), rec.tank_cid, rec.rack_number, rec.slot_position);

	int box_type_id = getKadoorieBoxType(rec);
	useCentralDB(); //xxx???

	// set up rack; check if slot already used
	if( !store.findRack(qCentral) )
//		store.findSlotRecord( qProject );
//	else
	{	store.setTypeID(box_type_id);
		store.createRack(qCentral);
	}

	LPDbBoxName box( rec.box_name, box_type_id );
	const LPDbBoxName * pBox = LPDbBoxNames().readRecord(qProject, rec.box_name);

	// add records to box_name
	if (pBox != NULL) {                 // already in box_name - unlikely and/or erroneous
		box = *pBox;
//		if (box.getID() == store.getBoxID()) {     // ok
			//box.setStatus(LPDbBoxName::IN_TANK );    // should already be set
			//rec.status = LPDbBoxName::IN_TANK;
//		} else if(store.getBoxID() != 0) {
//			throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "Box has already been stored");
//		}
	} else {    // most likely scenario
//	NG, 27/05/09 - box.create() makes an entry to a new name
//		box.create(box_type_id, qProject);
		box.saveRecord( qProject );
	}
	//rec.slot_cid = box.getSlotID(); //xxx??

	LQuery qp( qProject );
	tdmapFields mapFields = getSpecimenFieldNames(); // xxx inefficient
	qp.setSQL(strTemp.sprintf(
		"SELECT %s, %s, %s, %s, %s FROM specimen"
		" WHERE sample_id = %d", // xxx status 0 is a condition of LDbStage::EXPECTED
		mapFields["box_label"],
		mapFields["first_barcode"], mapFields["first_position"],
		mapFields["last_barcode"], mapFields["last_position"],
		rec.box_arrival_id));
	if (1 != qp.open()) {
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR,
			"Box " + rec.box_name + ", ID "+rec.box_arrival_id+" has no entry in specimen");
	}

	// check name
	if (0 != rec.box_name.AnsiCompareIC(qp.readString(mapFields["box_label"]))) {
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR,
			"Box " + rec.box_name + ", ID "+rec.box_arrival_id+
			" name does not match specimen entry ("+qp.readString(mapFields["box_label"])+")");
	}

	// DON'T create cryovial/cryovial store entries but update specimen
	const LPDbSource * pSource = LPDbSources::records().findByID(rec.box_arrival_id);
	if (NULL == pSource) {
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "Box " + rec.box_name + ", ID "+rec.box_arrival_id+" not found");
	}
	LPDbSource existing = *pSource;
	BoxArrivalRecord existingRec;

	// check positions
	String position;
	position = qp.readString(mapFields["first_position"]);
	if (position == ".") { // 'unknown' value in project db
		existing.insert(ValueField().descriptor("first_position").value(rec.first_position)); // insert the known value
	} else if (rec.first_position != position.ToInt()) { // check the expected value is the same as the actual value
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "first position mismatch: "+rec.print());
	}
	position = qp.readString(mapFields["last_position"]);
	if (position == ".") { // 'unknown' value in project db
		existing.insert(ValueField().descriptor("last_position").value(rec.last_position)); // insert the known value
	} else if (rec.last_position != position.ToInt()) { // check the expected value is the same as the actual value
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "last position mismatch: "+rec.print());
	}

	// check barcodes
	String barcode;
	barcode = qp.readString(mapFields["first_barcode"]);
	if (barcode == ".") { // 'unknown' value in project db
		existing.insert(ValueField().descriptor("first_barcode").value(rec.first_barcode)); // insert the known value
	} else if (rec.first_barcode != barcode) {
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "first barcode mismatch: "+rec.print());
	}

	barcode = qp.readString(mapFields["last_barcode"]);
	if (barcode == ".") { // 'unknown' value in project db
		existing.insert(ValueField().descriptor("last_barcode").value(rec.last_barcode)); // insert the known value
	} else if (rec.last_barcode != barcode) {
		throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "last barcode mismatch: "+rec.print());
	}

	existing.setArrived(rec.swipe_time, rec.process_cid);		// NG, 7/5/09 : was existing.setStage(LDbStage::IN_TANK);
	existing.saveRecord(qProject);

	rec.status = LPDbBoxName::IN_TANK;
	box.setStatus( LPDbBoxName::IN_TANK );
	box.saveRecord(qProject);

	LDbBoxArrival lbar( LCDbProjects::getCurrentID(), existingRec.box_arrival_id );
	for( Range< LDbBoxArrivalEvent > bah = lbar.readBoxHistory(qCentral); bah.isValid(); ++ bah)
	{
		LPDbBoxEvent event( *bah );
		event.saveRecord( qProject );
	}

	LCDbBoxStore link(0, rec.tank_cid, rec.rack_number, rec.slot_position);
	link.setBox(LCDbProjects::getCurrentID(), box.getID());
	link.saveRecord(qProject);
}

void TfrmMain::processThriveType(BoxArrivalRecord & rec) {
	LQuery qp(qProject);

	const LPDbBoxName * pBox = LPDbBoxNames().readRecord(qp, rec.box_name);
	LPDbBoxName box;

	LCDbRack store(getSite(rec.tank_cid), rec.tank_cid, rec.rack_number, rec.slot_position);

	if (pBox != NULL) {
		box = *pBox;
		useCentralDB(); //XXX???

		// set up rack; check if slot already used
		if( !store.findRack(qCentral) )
//			store.findSlotRecord( qProject );
  //		else
		{	store.setTypeID(box.getTypeID());
			store.createRack(qCentral);
		}

/*		// already in box_name
		if (box.getID() == store.getBoxID()) {   // slot already allocated
			// xxx we've matched the box name and slot ID - now check slot positions
			addToLog(strTemp.sprintf("box %d project %d has slot id %d", box.getID(), rec.project_cid, store.getID()));
		} else if(store.getBoxID() == 0) {
			addToLog(strTemp.sprintf("box %d project %d in slot id %d", box.getID(), rec.project_cid, store.getID()));
		} else {
			throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "Box " + rec.box_name + " has already been stored");
		}
*/
		LBoxExpectedRecord existingRecord(rec.box_arrival_id, rec.project_cid, rec.box_name);
		getThriveTypeCryovialSlotPositions(existingRecord, LQuery(qProject));
		getThriveTypeCryovialBarcodes(existingRecord, LQuery(qProject));

		if (    existingRecord.first_position   == rec.first_position
			&&  existingRecord.first_barcode    == rec.first_barcode
			&&  existingRecord.last_position    == rec.last_position
			&&  existingRecord.last_barcode     == rec.last_barcode) {
			addToLog(strTemp.sprintf("positions matched for box id %d project %d '%s'", rec.box_arrival_id, rec.project_cid, rec.box_name));
		} else {
			strTemp.sprintf(
				"Box '%s' id %d project %d positions not matched: existing: %d, '%s', %d, '%s' record:   %d, '%s', %d, '%s'",
				rec.box_name, rec.box_arrival_id, rec.project_cid,
				existingRecord.first_position, existingRecord.first_barcode, existingRecord.last_position, existingRecord.last_barcode,
				rec.first_position, rec.first_barcode, rec.last_position, rec.last_barcode);
			throw SSMinorException(BoxRecord::BOX_NOT_MATCHED, strTemp);
		}
	} else {
        // (for thrive type databases), the box should already be in box_name
        throw SSMinorException(BoxRecord::BOX_NOT_MATCHED, "Box " + rec.box_name + " id "+ rec.box_arrival_id + " name not matched");
	}

	//rec.slot_cid = box.getSlotID(); //xxx??
	rec.status = LPDbBoxName::IN_TANK;
	box.setStatus( LPDbBoxName::IN_TANK );
	box.saveRecord(qProject);

	LDbBoxArrival lbar( LCDbProjects::getCurrentID(), rec.box_arrival_id );
	for( Range< LDbBoxArrivalEvent > bah = lbar.readBoxHistory(qCentral); bah.isValid(); ++ bah)
	{
		LPDbBoxEvent event( *bah );
		event.saveRecord( qProject );
	}

	LCDbBoxStore link( 0, rec.tank_cid, rec.rack_number, rec.slot_position);
	link.setBox(LCDbProjects::getCurrentID(), box.getID());
	link.saveRecord(qProject);
}

void TfrmMain::getThriveTypeCryovialSlotPositions(LBoxExpectedRecord & box, LQuery q) {
	q.setSQL(strTemp.sprintf("SELECT MIN(cryovial_position) FROM cryovial_store"
							 " WHERE box_cid = %d", box.box_expected_id));
	if (1 != q.open()) { //
		throw Exception("no cryovial_position in cryovial store for box: "+box.print());
	}
	box.first_position = q.readInt(0);
	q.setSQL(strTemp.sprintf("SELECT MAX(cryovial_position) FROM cryovial_store"
							" where box_cid = %d", box.box_expected_id)); q.open();
	box.last_position = q.readInt(0);
}

void TfrmMain::getThriveTypeCryovialBarcodes(LBoxExpectedRecord & box, LQuery q) {
	q.setSQL(strTemp.sprintf(
			 "SELECT cr.cryovial_barcode , cs.cryovial_position"
			 " FROM cryovial cr, cryovial_store cs"
             " WHERE cr.cryovial_id = cs.cryovial_id AND cs.box_cid = %d"
             " AND cs.cryovial_position IN (%d, %d)"
             " ORDER BY cs.cryovial_position",
             box.box_expected_id, box.first_position, box.last_position));
    int count = q.open();
    if (count != 2)
        throw SSMinorException(BoxRecord::UNKNOWN_ERROR, String(count)+" row/s (expected 2) on SELECT cr.cryovial_barcode, box: "+box.print()+":\n"+strTemp);
    box.first_barcode = q.readString(0); q.next(); box.last_barcode = q.readString(0);
}

void __fastcall TfrmMain::btnLogToggleClick(TObject *Sender)
{
    if (logVisible) {
        formHeightWithLog = Height;
        formWidthWithLog  = Width;
        Height = formHeightNoLog;
        Width  = formWidthNoLog;
        btnLogToggle->Caption = "Show Log >>";
        logVisible = false;
    } else {
        logVisible = true;
        Height = formHeightWithLog;
        Width  = formWidthWithLog;
        btnLogToggle->Caption = "<< Hide Log";
    }
    Paint();
}

void __fastcall TfrmMain::btnOpenLogClick(TObject *Sender)
{
    ShellExecute(Handle, "open", LogFileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void TfrmMain::refreshTally() {
    panelTransferred->Caption = vecTransferred.size();
    panelSuccessful->Caption  = vecSuccessful.size();
    panelErrorsToday->Caption = vecErrorsToday.size();
    panelErrorsOlder->Caption = vecErrorsTotal.size();
    panelExpected->Caption = vecExpected.size();
    Application->ProcessMessages();
}


void __fastcall TfrmMain::btnTransferredClick(TObject *Sender)
{
    if (0 == vecTransferred.size()) return;

    TCursor	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;

    frmReport->Caption = "Boxes transferred";
    frmReport->memoReport->Text = "Boxes transferred from Laptop to LIMS:";
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecTransferred.begin(); arIt != vecTransferred.end(); arIt++) {
        BoxArrivalRecord box = *arIt;
        frmReport->memoReport->Lines->Add(box.box_name);
    }
    Screen->Cursor = initial_cursor;
    frmReport->ShowModal();
}

void __fastcall TfrmMain::btnSuccessfulClick(TObject *Sender)
{
    if (0 == vecSuccessful.size()) return;

    TCursor	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;

    frmReport->Caption = "Successfully stored boxes";
    frmReport->memoReport->Text = "Boxes successfully stored in LIMS:";
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecSuccessful.begin(); arIt != vecSuccessful.end(); arIt++) {
        BoxArrivalRecord box = *arIt;
        frmReport->memoReport->Lines->Add(box.box_name);
    }
    Screen->Cursor = initial_cursor;
    frmReport->ShowModal();
}

void __fastcall TfrmMain::btnErrorsTodayClick(TObject *Sender)
{
//    BoxArrivalRecord NewRecord(
//            -1,
//            -1,
//            -1,
//            -1, //project_cid
//            Now(), // swipe_time
//            "debug678901234567890123456789012", // box_name
//            -1, //xxx 5 = allocated, 6 = slot confirmed, 7 = referred
//			"first_barcode", -1,
//            "last_barcode",  -1,
//            -1, "rack_number", -1,
//            Now()
//            //, key_hash?
//		);
//
//    vecErrorsToday.push_back(NewRecord);

    if (0 == vecErrorsToday.size()) return;

    TCursor	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;
    frmReport->Caption = "Conflicts today";
    frmReport->memoReport->Text = strTemp.sprintf("Conflicts today (%s): %d", Now().DateTimeString(), vecErrorsToday.size());
    frmReport->memoReport->Lines->Add("");
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecErrorsToday.begin(); arIt != vecErrorsToday.end(); arIt++) {
        BoxArrivalRecord box = *arIt;
        TVarRec args[4] = { box.time_stamp.DateTimeString(), box.box_name, box.errorMessage, box.print() };
        strTemp = AnsiString::Format("[%19.19s] %-32.32s '%s'   [%s]", args, 4);
        frmReport->memoReport->Lines->Add(strTemp);
    }
    Screen->Cursor = initial_cursor;
    frmReport->ShowModal();
}

void __fastcall TfrmMain::btnErrorsTotalClick(TObject *Sender)
{

    if (0 == vecErrorsTotal.size()) return;

    TCursor	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;

    frmReport->Caption = "Total conflicts";
    frmReport->memoReport->Text = strTemp.sprintf("Total number of outstanding conflicts at %s: %d", Now().DateTimeString(), vecErrorsTotal.size());
    frmReport->memoReport->Lines->Add("");
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecErrorsTotal.begin(); arIt != vecErrorsTotal.end(); arIt++) {
        BoxArrivalRecord box = *arIt;
        //frmReport->memoReport->Lines->Add(box.time_stamp.DateTimeString()+": "+box.box_name + "\t\t" + box.errorMessage + "\t[" + box.print()+"]");
        TVarRec args[4] = { box.time_stamp.DateTimeString(), box.box_name, box.errorMessage, box.print() };
        strTemp = AnsiString::Format("[%19.19s] %-32.32s '%s'   [%s]", args, 4);
        frmReport->memoReport->Lines->Add(strTemp);
    }
    Screen->Cursor = initial_cursor;
    frmReport->ShowModal();


//    if (0 == vecErrors.size()) return;

//    TStringList *tslToday = new TStringList;
//    TStringList *tslOther = new TStringList;
//
//    tdvecBoxArrivalRecord::const_iterator arIt;
//    for (arIt = vecErrors.begin(); arIt != vecErrors.end(); arIt++) {
//        BoxArrivalRecord box = *arIt;
//        if (box.time_stamp > TDateTime::CurrentDate()) { // today
//            tslToday->Add(box.box_name + "\t\t" + box.errorMessage + "\t[" + box.print()+"]");
//        } else {
//            tslOther->Add(box.box_name + "\t\t" + box.errorMessage + "\t[" + box.print()+"]");
//        }
//    }
//    frmReport->Caption = "Conflicts";
//    frmReport->memoReport->Text = strTemp.sprintf("Conflicts today: %d", tslToday->Count);
//    frmReport->memoReport->Lines->Add("");
//    if (tslToday->Count) {
//        frmReport->memoReport->Lines->AddStrings(tslToday);
//        frmReport->memoReport->Lines->Add("");
//    }
//    frmReport->memoReport->Lines->Add(strTemp.sprintf("Conflicts before today: %d", tslOther->Count));
//    frmReport->memoReport->Lines->Add("");
//    frmReport->memoReport->Lines->AddStrings(tslOther);
//    frmReport->memoReport->Lines->Add("");
//
//    frmReport->ShowModal();
//    delete tslToday; delete tslOther;
}

void __fastcall TfrmMain::btnExpectedClick(TObject *Sender)
{
    if (0 == vecExpected.size()) return;

    TCursor	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;

    LCDbProject proj;
     int count = 0;
    TStringList *tslTemp  = new TStringList;
    TStringList *tslTemp2 = new TStringList;

    frmReport->Caption = "Expected boxes";
    frmReport->memoReport->Text = String(vecExpected.size())+" expected boxes updated to Laptop.";
    frmReport->memoReport->Lines->Add("");
    frmReport->memoReport->Lines->Add("");
    frmReport->memoReport->Lines->Add("Summary:");
    frmReport->memoReport->Lines->Add("--------");
    frmReport->memoReport->Lines->Add("");

    //proj.setID;
    tdvecLBoxExpectedRecord::const_iterator arIt;
    // set the project to that of the first box
	for (Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr) {
        if ((*pr).getID() == (*(vecExpected.begin())).project_cid) {
            proj = *pr; break; }}

    class _local {
    public:
        static void addSummary(TStringList *tslTemp, TStringList *tslTemp2, TStrings *tsLines, int projID, String projName, int count) {
            strTemp = String(projID);
            TVarRec args[4] = {strTemp.c_str(), projName, count, (count == 1 ? "" : "es")};
            strTemp = AnsiString::Format("[%8.8s] %-20.20s %d box%s", args, 4);
            tsLines->Add(strTemp); // output summary at top of page
            tslTemp->Add("");
            tslTemp->Add(strTemp); // and add summary for each project in listing
            tslTemp->Add("");
            tslTemp->AddStrings(tslTemp2);
            tslTemp2->Clear();
        }
    };

    for (arIt = vecExpected.begin(); ; arIt++) {
        if (arIt != vecExpected.end()) {
            LBoxExpectedRecord box = *arIt;
            if (proj.getID() != box.project_cid) {  // we've changed project
                // save summary info for previous project
                _local::addSummary(tslTemp, tslTemp2, frmReport->memoReport->Lines, proj.getID(), proj.getName(), count);

                // then set correct project
                for (Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr) { if ((*pr).getID() == box.project_cid) { proj = *pr; }}
                if (proj.getID() != box.project_cid) {
                    // project ID not found
                    proj = LCDbProject(box.project_cid); proj.setName("Unknown");
                    Application->MessageBox(strTemp.sprintf("Unknown project cid: %d", box.project_cid).c_str(), "Error", MB_OK);
                }
                count = 0;
            }
            // add boxes to a temp list which we will add on later
            // box_expected_id, project_cid, box_name, first_barcode, first_position, last_barcode, last_position
            tslTemp2->Add(box.box_name); count++;
        } else {
            // finish up
            _local::addSummary(tslTemp, tslTemp2, frmReport->memoReport->Lines, proj.getID(), proj.getName(), count);
            break;
        }
    }

    frmReport->memoReport->Lines->Add("");
    frmReport->memoReport->Lines->Add("Detail:");
    frmReport->memoReport->Lines->Add("-------");
    frmReport->memoReport->Lines->AddStrings(tslTemp);
    Screen->Cursor = initial_cursor;
    frmReport->ShowModal();

    delete tslTemp; delete tslTemp2;
}

void TfrmMain::copyAuditTrail() {
    // copy audit trail from laptop
    // exception should bomb out to main loop and cause abort
//    copyTable(qLaptop, qCentral, "c_audit_trail", "c_audit_trail");
    LQuery ql(qLaptop), qc(qCentral);

    updateStatus("", "Copying c_audit_trail");
    ql.setSQL(  "SELECT message_date, process_cid, operator_cid, message_type, machine_cid, database_name, program_name, details"
                " FROM c_audit_trail");
    int nrows = ql.open();
    progressMinor->Max = nrows;
    progressMinor->Position = 0;
    while (!ql.eof()) {
        qc.setSQL(strTemp.sprintf(
            "INSERT INTO c_audit_trail (message_date, process_cid, operator_cid, message_type, machine_cid, database_name, program_name, details)"
            " VALUES (:msgd, %d, %d, %d, %d, :dbname, :progname, :details)",
            ql.readInt("process_cid"), ql.readInt("operator_cid"), ql.readInt("message_type"), ql.readInt("machine_cid")));
        qc.setParam("msgd", ql.readDateTime("message_date"));
        qc.setParam("dbname", ql.readString("database_name"));
        qc.setParam("progname", ql.readString("program_name"));
        qc.setParam("details", ql.readString("details"));
        qc.execSQL(); ql.next();
        progressMinor->StepIt(); Paint(); Application->ProcessMessages();
    }
    ql.setSQL("DELETE FROM c_audit_trail");
    ql.execSQL();
    progressMajor->StepIt(); Application->ProcessMessages();
}

void TfrmMain::copyTables()
{
    updateStatus("Copying central tables", "");

    LQuery qc(qCentral), ql(qLaptop); int nrows;

// this didn't work:
//    BatchMove1->Source      = qCentral;
//    BatchMove1->Destination = TableLaptop;
//    BatchMove1->Execute();

    // batch copy other tables
    progressMinor->Max = Config::numTables; progressMinor->Position = 0;

    for (int i=0; i<Config::numTables; i++) {
        try {
            updateStatus("", "Copying "+Config::otherTables[i]);
            copyCentralTableToLaptop(Config::otherTables[i]);
        } catch (Exception & e) {
            minorError("Error copying "+Config::otherTables[i]+": "+e.Message, __FUNC__);
        }
        progressMinor->StepIt(); Paint(); Application->ProcessMessages();
    }

    ql.setSQL("SELECT MIN(rack_cid) FROM c_rack_number");
    ql.open();
    int minId = ql.readInt(0);
    qc.setSQL(strTemp.sprintf("SELECT rack_cid, tank_cid, position, external_name, rack_type_cid, default_box_type, project_cid FROM c_rack_number WHERE rack_cid < %d", minId));
    nrows = qc.open();
    while (!qc.eof()) {
        ql.setSQL(strTemp.sprintf(
            "INSERT INTO c_rack_number (rack_cid, tank_cid, position, external_name, rack_type_cid, default_box_type, project_cid)"
            " VALUES (%d, %d, %d, :en, %d, %d, %d)",
            qc.readInt("rack_cid"), qc.readInt("tank_cid"),
            qc.readInt("position"), qc.readInt("rack_type_cid"), qc.readInt("default_box_type"), qc.readInt("project_cid")));
        ql.setParam("en", qc.readString("external_name"));
        ql.execSQL();
        qc.next();
    }

    progressMajor->StepIt(); Application->ProcessMessages();
}

void TfrmMain::copyCentralTableToLaptop(String tableName)
{
    updateStatus("", "Copying " + tableName);
//    TableCentral->TableName = tableName;
//    TableLaptop->TableName = tableName;
//    BatchMove1->Execute();
    copyTable(qCentral, qLaptop, tableName, tableName, true);

    // n.b. use EmptyTable() and batAppend mode when copying
    // between different database types (at least between MySQL and Ingres)
}

/*
ftString	Character or string field
ftSmallint	16-bit integer field
ftInteger	32-bit integer field
ftWord	16-bit unsigned integer field
ftBoolean	Boolean field
ftFloat	Floating-point numeric field
ftDate	Date field
ftTime	Time field
ftDateTime	Date and time field
ftAutoInc	Auto-incrementing 32-bit integer counter field
ftBlob	Binary Large OBject field
ftFixedChar	Fixed character field
ftWideString	Wide string field
ftLargeint	Large integer field
ftVariant	Data of unknown or undetermined type
ftGuid	globally unique identifier (GUID) values
ftUnknown	Unknown or undetermined
*/

void TfrmMain::copyTable(TQuery *qSource, TQuery *qDest, String sourceTable, String destTable, bool deleteDest = false)
{
    LQuery qs(qSource), qd(qDest);
    String query, param, strFields, strValues;

    strTemp.sprintf(
        "Copying %s::%s to %s::%s, %s",
        qSource->Name, sourceTable, qDest->Name, destTable,
        (deleteDest? "deleting dest" : "not deleting dest"));
    addToLog(strTemp);

    qs.setSQL(query.sprintf("SELECT * FROM %s", sourceTable));
    int count = qs.open();
    if (0 == count) {
        return;
    }

    // build field list
    for(int i = 0; i < qSource->FieldCount; i++) {
        if (0 != i) { strFields += ", "; }
        strFields += qSource->Fields->Fields[i]->FieldName;
    }

    // build param string
    for(int i = 0; i < qSource->FieldCount; i++) {
        if (0 != i) { strValues += ", "; }
        strValues += param.sprintf(":param%d", i);
    }

    if (deleteDest) {
        qd.setSQL("DELETE FROM "+destTable);
        qd.execSQL();
    }

    qs.setSQL(query.sprintf("SELECT %s FROM %s", strFields, sourceTable));
    qs.open();

    qd.setSQL(query.sprintf("INSERT INTO %s (%s) VALUES (%s)", destTable, strFields, strValues));

    while(!qs.eof()) {
        // set params
        for(int i = 0; i < qSource->FieldCount; i++) {
            param.sprintf("param%d", i);
            switch (qSource->Fields->Fields[i]->DataType) {
                case ftString:
                    qd.setParam(param, qs.readString(qSource->Fields->Fields[i]->FieldName));

                    break;
                case ftSmallint:
                case ftInteger:
                case ftBytes:
                    qd.setParam(param, qs.readInt(qSource->Fields->Fields[i]->FieldName));
                    break;
                //case ftFloat:
                case ftDate:
                case ftTime:
                case ftDateTime:
                    qd.setParam(param, qs.readDateTime(qSource->Fields->Fields[i]->FieldName));
                    break;
                default:
                    throw Exception(query.sprintf("Field type %d unhandled in %s", qSource->Fields->Fields[i]->DataType, __FUNC__));
            }
        }
        qd.execSQL();
        qs.next();
    }
}

void TfrmMain::syncCOperator()
{
    updateStatus("Syncing user information", "");

    LQuery ql(qLaptop);
    ql.setSQL("SELECT operator_cid, status, valid_to, password_md5 FROM c_operator WHERE status IN (0, 1)");
    progressMinor->Max = ql.open();
    progressMinor->Position = 0;
    updateStatus("", "Getting records");

    tdvecOperator vecOperators;
    while (!ql.eof()) {
        Operator op;
        op.cid          = ql.readInt("operator_cid");
        op.status       = ql.readInt("status");
        op.valid_to     = ql.readDateTime("valid_to");
        op.password     = ql.readString("password_md5");
        vecOperators.push_back(op);
        ql.next();
        progressMinor->StepIt(); Application->ProcessMessages();
    }

    LQuery qc(qCentral);
    tdvecOperator::const_iterator opIt;
    for (opIt = vecOperators.begin(); opIt != vecOperators.end(); opIt++) {
        qc.setSQL("UPDATE c_operator SET password_md5 = :pass, status = :status, valid_to = :valid WHERE operator_cid = :cid AND valid_to < :valid");
        qc.setParam("pass", (*opIt).password);
        qc.setParam("cid", (*opIt).cid);
        qc.setParam("status", (*opIt).status);
        qc.setParam("valid", (*opIt).valid_to);
        queryExec(qc);
    }

    //copyCentralTableToLaptop("archive_c_operator"); //XXX???
    progressMajor->StepIt();
}

void TfrmMain::testing() {
    // test
//    try {
//        throw SSMinorException(BoxRecord::UNKNOWN_ERROR, "SSMinorException");
//    } catch (SSMinorException & e) {
//        // A recognised error - there is something wrong with the box. Refer it.
//        throw Exception(e.Message);
////        rec.errorMessage = e.Message;
////        strTemp.sprintf("'%s' [%s]", e.Message, rec.print());
////        minorError(strTemp, __FUNC__);
////        rec.status = LPDbBoxName::REFERRED;
////        vecErrors.push_back(rec);
//    } catch (Exception & e) {
//        // Some other problem not to do with the box. Leave it unchanged in c_box_arrival to be dealt with on a later run.
//        Application->MessageBox("Error", e.Message.c_str(), MB_OK);
//        Application->Terminate();
////        rec.errorMessage = e.Message;
////        strTemp.sprintf("'%s' [%s]", e.Message, rec.print());
////        minorError(strTemp, __FUNC__);
////        vecErrors.push_back(rec);
//    }

    //tfield = qCentral->FieldByName(field.name);
//    LQuery qd(qLaptop);
//    //qd.setSQL("INSERT INTO c_permission (permission_cid) VALUES (:param0)");
//    qd.setSQL("SELECT * FROM c_permission");
//    qd.open();
//    while (!qd.eof()) {
//        ;
//        qd.next();
//    }
//
//    qd.setSQL("INSERT INTO c_permission (permission_cid) VALUES (:param0)");
//    strTemp = qd.getSQL();
//    qd.setParam("param0", 111);

	//copyTable(qCentral, "c_permission", qLaptop, "c_permission", "permission_cid", "operator_cid", "program_cid", "project_cid", "page_number", "status", 0);
    //copyTable(qCentral, qLaptop, "c_permission");

    //use_rijndael = false;
//    String strEncrypted;
//    strTemp = "1234567890123456123456789012345612345678901234561234567890123456";
//    strEncrypted = rijndael->encryptSz2Base64(strTemp.c_str());
//    addToLog(strEncrypted);
//    return;

    // new expected boxes

    // box_expected_id,project_cid, box_name, first_barcode, first_position, last_barcode, last_position

//     // some thrive boxes
//    LBoxExpectedRecord ebox1(1000000, -149657, "thrivetest1e", "bar1", 1, "bar2", 2); vecExpected.push_back(ebox1);
//    LBoxExpectedRecord ebox2(1000001, -149657, "thrivetest2e", "bar1", 1, "bar2", 2); vecExpected.push_back(ebox2);
//
//    // duff box
//    LBoxExpectedRecord ebox7(1000004, -7567777, "bolloxtest1e", "bar1", 1, "bar2", 2); vecExpected.push_back(ebox7);
//
//    // some mexico boxes
//    LBoxExpectedRecord ebox3(1000002, -204080, "mextest1e", "1stbar", 1, "lastbar", 2); vecExpected.push_back(ebox3);
//    LBoxExpectedRecord ebox4(1000003, -204080, "mextest2e", "stuff", 1, "stuff", 2); vecExpected.push_back(ebox4);
//
//    // some kadoorie boxes
//    LBoxExpectedRecord ebox5(1000004, -77777, "kadtest1e", "bar1", 1, "bar2", 2); vecExpected.push_back(ebox5);
//    LBoxExpectedRecord ebox6(1000005, -77777, "kadtest2e", "bar1", 1, "bar2", 2); vecExpected.push_back(ebox6);
//
//
//    //throw Exception
//    Application->MessageBox("inserted some test rows into vecExpected, now try the report", "", MB_OK);
//
//    if (TESTING_STATUS != LCDbProjects::MIRROR_SYSTEM) return;
//
//    return;
//    // insert stuff into databases for testing

 /* int         laptop_cid;
    int         process_cid;
    int         box_arrival_id;
    int         project_cid;
    TDateTime   swipe_time;
    String      box_name;
    int         status;         // only in c_box_arrival
    String      first_barcode;
    int         first_position;
    String      last_barcode;
    int         last_position;
    int         tank_cid;                                                                                                             int         tank_cid;
    String      rack_number;                                                                                                          String      rack_number;
    int         slot_position;                                                                                                        int         slot_position;
    int         slot_cid;                                                                                                             int         slot_cid;
    // extra to database record:
    String      errorMessage;*/

    tdvecBoxArrivalRecord       vecTestArrivals;

    // some kadoorie boxes
	BoxArrivalRecord box5(-1, -1, 20018, -77767, Now(), "KS-B-460502", (int)LCDbBoxStore::SLOT_CONFIRMED, "0105278431", 1, "0105279421", 100, -142589, "A1", 1, -1); vecTestArrivals.push_back(box5);
	BoxArrivalRecord box6(-1, -1, 19766, -77767, Now(), "KS-A-460502", (int)LCDbBoxStore::SLOT_CONFIRMED, "0105278431", 1, "0105279421", 100, -142589, "A1", 2, -1); vecTestArrivals.push_back(box6);

    // some thrive boxes
	BoxArrivalRecord box1(-1, -1, 880488, -149647, Now(), "HPS2-THRIVE Urine(UK) 880488", (int)LCDbBoxStore::SLOT_CONFIRMED, "112100158", 1, "112100371", 100, -142589, "A1", 3, -1); vecTestArrivals.push_back(box1);
    BoxArrivalRecord box2(-1, -1, 900139, -149647, Now(), "HPS2-THRIVE LiHep(UK) 900139", (int)LCDbBoxStore::SLOT_CONFIRMED, "112100462", 1, "112100580", 100, -142589, "A1", 4, -1); vecTestArrivals.push_back(box2);

    // some mexico boxes
//    BoxArrivalRecord box3(-1, -1, 1000002, -204123, Now(), "mextest1", (int)LPDbBoxName::SLOT_CONFIRMED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box3);
//    BoxArrivalRecord box4(-1, -1, 1000003, -204123, Now(), "mextest2", (int)LPDbBoxName::SLOT_CONFIRMED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box4);

    // duff boxes
//    BoxArrivalRecord box7(-1, -1, 1000004, -7567777, Now(), "kadtest1", (int)LPDbBoxName::SLOT_CONFIRMED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box7);
//    BoxArrivalRecord box8(-1, -1, 1000005, -7774577, Now(), "kadtest2", (int)LPDbBoxName::SLOT_CONFIRMED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box8);
//    BoxArrivalRecord box9(-1, -1, 1000005, -77777, Now(), "kadtest3", (int)LPDbBoxName::REFERRED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box9);
//    BoxArrivalRecord box10(-1, -1, 1000005, -77777, Now(), "kadtest4", (int)LPDbBoxName::ALLOCATED, "1234567890", 1, "1234567890", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box10);
//    BoxArrivalRecord box11(-1, -1, 1000005, -77777, Now(), "kadtest2", (int)LPDbBoxName::SLOT_CONFIRMED, "bar1", 1, "bar2", 2, 1, 2, 3, 4); vecTestArrivals.push_back(box11);

    LQuery qc(qCentral), ql(qLaptop);
    LCDbID myLCDbID;

    String first_barcode, last_barcode;
    tdvecBoxArrivalRecord::const_iterator arIt;
    for (arIt = vecTestArrivals.begin(); arIt != vecTestArrivals.end(); arIt++) {
        BoxArrivalRecord box = *arIt;
        myLCDbID.claimNextID(qc); int nextid = myLCDbID.getID(); //xxx?
        if (use_rijndael) {
            first_barcode = rijndael->encryptSz2Base64(box.first_barcode.c_str());
            last_barcode  = rijndael->encryptSz2Base64(box.last_barcode.c_str());
        } else {
            first_barcode = box.first_barcode;
            last_barcode  = box.last_barcode;
        }
        ql.setSQL(strTemp.sprintf(
            "INSERT INTO l_box_arrival (laptop_cid, process_cid, box_arrival_id, project_cid, swipe_time, box_name, status, first_barcode, first_position, last_barcode, last_position, tank_cid, rack_number, slot_position, time_stamp)"
            " VALUES (%d, %d, %d, %d, :st, '%s', %d,"
            " '%s', %d, '%s', %d,"
            " %d, '%s', %d, :ts)",
            box.laptop_cid, box.process_cid, box.box_arrival_id, box.project_cid,
            box.box_name, box.status,
            first_barcode, box.first_position, last_barcode, box.last_position,
			box.tank_cid, box.rack_number, box.slot_position));
        ql.setParam("st", box.swipe_time);
        ql.setParam("ts", box.time_stamp);
        queryExec(ql);
    }
}


//--------- scratch ------------------------------------------------------------------

/*
        if ( DBT_DEVICEARRIVAL == Msg.WParam || DBT_DEVICEREMOVECOMPLETE == Msg.WParam ) {
            PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)Msg.LParam;
            if (DBT_DEVTYP_VOLUME == pHdr->dbch_devicetype) {
                USBvol = dev->getDriveLetter(Msg.LParam);
                if (USBvol) {
                    //Application->MessageBox(strTemp.sprintf("The USB thingy drive letter is '%c'.", vol).c_str(), "p", MB_ICONEXCLAMATION);

                } else {
                    throw Exception(strTemp.sprintf("USBvol is %d", USBvol));
                }
            }
        }*/

//String TfrmMain::getDescriptor(const String & descripName)
//{
//	const LPDbDescriptor * descrip = LPDbDescriptors::records().findByName(descripName);
//	if (descrip != NULL)
//	{
//		String field = descrip->getSpecimenField();
//		if (!field.IsEmpty() && field != ".")
//			return field;
//	}
//	throw Exception("No specimen field for " + descripName + " descriptor");
//}

//        if (TESTING_STATUS == MIRROR_NONDESTRUCTIVE) { //xxx
//            LQuery qc(qCentral); qc.setSQL("DELETE FROM c_lease WHERE task_desc = 'P93/Storage Sync'");
//            qc.execSQL();
//        }

//    // String buffer for holding the path.
//    TCHAR strPath[MAX_PATH];
//    LPTSTR pszPath[MAX_PATH];
//
//    // Get the special folder path.
////    SHGetSpecialFolderPath(
////        0,       // Hwnd
////        strPath, // String buffer.
////        CSIDL_DESKTOPDIRECTORY, // CSLID of folder
////        FALSE ); // Create if doesn't exists?
//
////#include <SHFolder.h>
//
//    //PIDLIST_ABSOLUTE pidlist;
//
//    //if(SHGetFolderLocation(
//    SHGetSpecialFolderPath(
//        0,
//        CSIDL_DESKTOPDIRECTORY,//CLSID_ShellDesktop,//CSIDL_DESKTOPDIRECTORY,
//        0,//HANDLE hToken,
//        0,//DWORD dwFlags,
//        strPath
//    );

//    char szPath[_MAX_PATH];
//
//    PIDLIST_ABSOLUTE pidl;
//    HRESULT hr = SHGetFolderLocation(0, CSIDL_DESKTOP, 0, 0, &pidl);
//    if (SUCCEEDED(hr))
//    {
//		if (SHGetPathFromIDList(pidl, szPath))
//		{
//            ShowMessage(szPath);
//        }
//    }
//    ILFree(pidl);

//xxx#include <shlobj.h>

// #include "Hidclass.h"

//void TfrmMain::findUSBvol() {
//
//
////    LPGUID guid;
////    HidD_GetHidGuid(guid);
//
////    HDEVINFO hDevInfo;
//
////    hDevInfo = SetupDiGetClassDevs(guid,NULL,NULL,DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
//
////-------------------------------------------
//
//// or
//    DWORD dwMask = 1; // Least significant bit is A: flag
//    DWORD dwDrives = GetLogicalDrives ();
//    CHAR strDrive [4];
//    int driveType;
//    HANDLE hDevice;
//    PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
//    String volumeName;
//
//    for (int i=3; i<26; i++) { // 26 letters in [A..Z] range... start at D
//        if (dwDrives & dwMask) {
//            // create root path
//            strDrive[0] = 'A'+i;
//            strDrive[1] = ':';
//            strDrive[2] = '\\';
//            strDrive[3] = '\0';
//
//            driveType = GetDriveType(strDrive);
//            addToLog("Found drive "+String(strDrive)+":, type "+String(driveType));
//            switch (driveType) {
//            case DRIVE_FIXED:
//            case DRIVE_REMOVABLE:
//                // sometimes flash drives are reported as being fixed, examine the bus type to find USB
//                volumeName.sprintf("\\\\.\\%c:", strDrive[0]); //"\\\\.\\"
//                hDevice = CreateFile(   volumeName.c_str(),
//                                        GENERIC_READ,
//                                        FILE_SHARE_READ | FILE_SHARE_WRITE,
//                                        NULL,
//                                        OPEN_EXISTING,
//                                        FILE_FLAG_OVERLAPPED, //NULL,
//                                        NULL);
//                if(hDevice == INVALID_HANDLE_VALUE) //Error opening drive
//                {
//                    //int lastError = GetLastError();
//                    addToLog("Error opening drive "+volumeName+", error: "+getSysErrorMessage());
//                } else {
//                    if(GetDisksProperty(hDevice, pDevDesc)) {
//                        if(pDevDesc->BusType == BusTypeUsb) // This is the ‘Check Point’!!! ;-)
//                        {
//                            addToLog("It's a USB drive");
//                            // We store the drive letter here
//    //                        szMoveDiskName[k] = chFirstDriveFromMask(temp);
//    //                        szMoveDiskName[0]=k; k++;
//                        } else {
//                            addToLog("It's not a USB drive");
//                        }
//                    } else {
//                        strTemp.sprintf("Can't get disk %s properties, error: %s", strDrive, getSysErrorMessage());
//                        addToLog(strTemp);
//                    }
//                }
//                CloseHandle(hDevice);
//            default:
//                ;
//            }
//            //wsprintf (strDrive, "%c:", 'A'+i);
//        }
//        dwMask <<= 1; // Next bit to the left
//    }
//}

//#include "Ntddstor.h"
//#include "winioctl.h"
//
//bool GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc) {
//    STORAGE_PROPERTY_QUERY Query; // input param for query
//    DWORD dwOutBytes; // IOCTL output length
//    BOOL bResult; // IOCTL return val
//
//    // specify the query type
//    Query.PropertyId = StorageDeviceProperty;
//    Query.QueryType = PropertyStandardQuery;
//    Query.AdditionalParameters[0] = '\0';
//
//    // Query using IOCTL_STORAGE_QUERY_PROPERTY
//    bResult = DeviceIoControl(
//        hDevice, // device handle
//        IOCTL_STORAGE_QUERY_PROPERTY, // info of device property
//        &Query, sizeof(STORAGE_PROPERTY_QUERY), // input data buffer
//        pDevDesc,
//        //480,
//        sizeof(PSTORAGE_DEVICE_DESCRIPTOR),
//        //pDevDesc->Size, // output data buffer
//        &dwOutBytes, // out's length
//        (LPOVERLAPPED)NULL);
//
//    return bResult;
//}

// another usb detect routine
/*
   BOOLEAN success;
   ULONG nBytes;
   PUSB_DEVICE_DESCRIPTOR pDeviceDiscriptor = new USB_DEVICE_DESCRIPTOR();
   if( _hEzUsb == INVALID_HANDLE_VALUE )
   {
      ReportError( "Driver not open." );
   }
   else
   {
      success = DeviceIoControl( _hEzUsb, IOCTL_Ezusb_GET_DEVICE_DESCRIPTOR, NULL,
         0, pDeviceDiscriptor, sizeof(USB_DEVICE_DESCRIPTOR), &nBytes, NULL );
      if( success == FALSE )
      {
         CheckWin32Error();
      }
   }
   UsbDeviceDescriptor* DeviceDiscriptor = new UsbDeviceDescriptor();
   DeviceDiscriptor = (UsbDeviceDescriptor*)Marshal::PtrToStructure(
      pDeviceDiscriptor, DeviceDiscriptor->GetType() );
   delete pDeviceDiscriptor;
   return( DeviceDiscriptor );
*/


void __fastcall TfrmMain::panelExpectedDblClick(TObject *Sender)
{
    debug_on = !debug_on;
    GroupBox2->Color = debug_on ? clRed : clSkyBlue;
    Timer1->Enabled = debug_on ? false : true;

    rijndael_key = "1234";
    rijndael = new RIJNDAEL(rijndael_key);
    USBvol = 'G';
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::TrayIcon1DblClick(TObject *Sender)
{
    Show();
    SetFocus();
    BringToFront();
    Application->Restore();
    Application->BringToFront();
    WindowState = wsNormal;
}
//---------------------------------------------------------------------------

//void __fastcall TfrmMain::FormHide(TObject *Sender)
//{
//    WindowState = wsMinimized;
//    Application->Minimize();
//}
////---------------------------------------------------------------------------
//
//
//void __fastcall TfrmMain::FormDeactivate(TObject *Sender)
//{
//    Application->Minimize();
//}
////---------------------------------------------------------------------------


