
# Unable to open file 'DBRTL.LIB'

Mysteriously started happening, preventing build.

Should be prevented from being included by following in `xdb.h`

    #if defined(__BORLANDC__) && defined(XSQL_BDE)
    #define X_BDE 1
    #define X_ING 0
    #else
    #define X_BDE 0
    #define X_ING 1
    #endif
    //===========================================================================
    #if X_BDE
    #include <Db.hpp>
    #include <DBTables.hpp>
    #include <DBGrids.hpp>
    #elif X_ING
    extern "C" {
    #include <iiapi.h>  // REQUIRES PATH ii_system/files
      }
    #endif

Should prevent `DBTables.hpp` from being included, which lives in:

    C:\Program Files (x86)\Embarcadero\RAD Studio\11.0\include\windows\rtl

### DBTables.hpp

Just does this:

    #include <Bde.DBTables.hpp>

### Bde.DBTables.hpp

    /* automatically link to dbrtl and vcldb as well */
    #ifdef USEPACKAGES
    #pragma link "dbrtl.bpi"
    #pragma link "vcldb.bpi"
    #pragma link "vcl.bpi"
    #else
    #pragma link "dbrtl.lib"
    #pragma link "vcldb.lib"
    #pragma link "vcl.lib"
    #endif

### dbrtl.bpi/lib

Where?

    C:\Program Files (x86)\Embarcadero\RAD Studio\11.0\lib

or in Cygwin:

    /cygdrive/c/Program Files (x86)/Embarcadero/RAD Studio/11.0/lib

    2014-05-08 19:51:28 cbird@Q108 /cygdrive/c/Program Files (x86)/Embarcadero/RAD Studio/11.0/lib
    $ find . -iname dbrtl.*

    ./win32/debug/dbrtl.dcp
    ./win32/debug/dbrtl.lib
    ./win32/release/dbrtl.bpi
    ./win32/release/dbrtl.dcp
    ./win32/release/dbrtl.lib
    ./win64/debug/dbrtl.dcp
    ./win64/release/dbrtl.a
    ./win64/release/dbrtl.bpi
    ./win64/release/dbrtl.dcp

Is something new including `dbtables.hpp` without checking for `X_BDE`?

Not in the Storeman main folder:

    2014-05-08 19:33:09 cbird@Q108 ~/Projects/retrieval-assistant/LADS/Storeman
    $ grep -i dbtables *
    2014-05-08 19:55:14 cbird@Q108 ~/Projects/retrieval-assistant/LADS/Storeman
    $

Anywhere else? Er, yep...

    2014-05-08 19:57:47 cbird@Q108 ~/Projects/retrieval-assistant
    $ grep -ir dbtables.hpp {LADS,paulst,jonathanpr}
    LADS/AAAComms/AAACForm.h:#include <DBTables.hpp>
    LADS/AddSpecimens/FillUnit.h:#include <DBTables.hpp>
    LADS/AliquotStorage/AliquotList.h:#include <DBTables.hpp>
    LADS/AliquotStorage/AllocMain.h:#include <DBTables.hpp>
    LADS/AliquotStorage/BoxConfirm.h:#include <DBTables.hpp>
    LADS/AliquotStorage/VialListEdit.h:#include <DBTables.hpp>
    LADS/BeckmanComms/bcfMainForm.h:#include <DBTables.hpp>
    LADS/BeckmanComms/bec_serial.h:#include <DBTables.hpp>
    LADS/BoxBridge/fmLogin.h:#include <DBTables.hpp>
    LADS/BoxBridge/fmMain.h:#include <DBTables.hpp>
    LADS/BoxBridge/frmCommon.h:#include <DBTables.hpp>
    LADS/BoxCreation/MainForm.h:#include <DBTables.hpp>
    LADS/BoxCreation/Unit1.h:#include <DBTables.hpp>
    LADS/BoxReception/fBoxReception.h:#include <DBTables.hpp>
    LADS/BoxReception/fBRDEF.h:#include <DBTables.hpp>
    LADS/BoxReception/TfrmBRLogin.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xdb.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xexec.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xquery.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xsql.h:#include <DBTables.hpp>
    LADS/CoulterComms/ccMainForm.h:#include <DBTables.hpp>
    LADS/GenericComms/frmMain.h:#include <DBTables.hpp>
    LADS/GenericComms/frmManage.h:#include <DBTables.hpp>
    LADS/HPLCComms/FormCommon.h:#include <DBTables.hpp>
    LADS/HPLCComms/FormUtils.h:#include <DBTables.hpp>
    LADS/IDS-iSYS/isysMainForm.h:#include <DBTables.hpp>
    LADS/LabsDAO/LPDbBoxWork.h:#include <DBTables.hpp>
    LADS/LabsForms/BuddyLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmCommsConfig.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmCommsMain.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmLoginA.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSerialConfig.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSocketComms.h:#include <DBTables.hpp>
    LADS/LabsLeasing/lease.h:#include <DBTables.hpp>
    LADS/MenariniComms/mcProtocol.h:#include <DBTables.hpp>
    LADS/MenariniComms/mnMainForm.h:#include <DBTables.hpp>
    LADS/MenariniComms/MNSaveCmd.h:#include <DBTables.hpp>
    LADS/Nephelometer/ControlResults.h:#include <dbtables.hpp>
    LADS/Nephelometer/JobListReply.h:#include <DBTables.hpp>
    LADS/Nephelometer/JobListRequest.h:#include <dbtables.hpp>
    LADS/Nephelometer/Message.h:#include <dbtables.hpp>
    LADS/Nephelometer/NephCommsMain.h:#include <DBTables.hpp>
    LADS/Nephelometer/NephPersistImpl.h:#include <DBTables.hpp>
    LADS/NightShift/TfrmHistory.h:#include <DBTables.hpp>
    LADS/NightShift/TfrmNightShift.h:#include <DBTables.hpp>
    LADS/NightShift/TfrmNSLogin.h:#include <DBTables.hpp>
    LADS/NightShift/TfrmProjects.h:#include <DBTables.hpp>
    LADS/OlympusComms/BOCForm.h:#include <DBTables.hpp>
    LADS/PlotQC/DataMod1.h:#include <DBTables.hpp>
    LADS/PlotQC/DateHandling.cpp:#include <Dbtables.hpp>
    LADS/PlotQC/DateMod1.cpp:#include <Dbtables.hpp>
    LADS/PlotQC/EventLogClass.cpp:#include <dbtables.hpp>
    LADS/PlotQC/LeaseMod1.h:#include <DBTables.hpp>
    LADS/PlotQC/Login1.h:#include <DBTables.hpp>
    LADS/PlotQC/LogMod1.cpp:#include <Dbtables.hpp>
    LADS/PlotQC/LogMod1.h:#include <DbTables.hpp>
    LADS/PlotQC/MachineClass.cpp:#include <dbtables.hpp>
    LADS/PlotQC/mainfm.h:#include <DBTables.hpp>
    LADS/PlotQC/ObjectNameClass.cpp:#include <dbtables.hpp>
    LADS/PlotQC/OperatorClass.cpp:#include <dbtables.hpp>
    LADS/PlotQC/ProjectClass.cpp:#include <dbtables.hpp>
    LADS/PlotQC/TestClass.cpp:#include <dbtables.hpp>
    LADS/ProfileMapTest/mapTestMain.h:#include <DBTables.hpp>
    LADS/QCSetup/ModifiedLogin.h:#include <DBTables.hpp>
    LADS/QCSetup/QCSLogin.h:#include <DBTables.hpp>
    LADS/QCSetup/Unit1.h:#include <DBTables.hpp>
    LADS/QCSetup/Unit3.h:#include <DBTables.hpp>
    LADS/RackCheck/checkMain.h:#include <DBTables.hpp>
    LADS/RegisterSetup/Unit1.h:#include <DBTables.hpp>
    LADS/SampleReception/DataModule.h:#include <DBTables.hpp>
    LADS/SampleReception/DbCBuddyWork.cpp:#include <dbtables.hpp>
    LADS/SampleReception/fAssigner.h:#include <DBTables.hpp>
    LADS/SampleReception/fByHandProfiles.h:#include <DBTables.hpp>
    LADS/SampleReception/fFindSample.h:#include <DBTables.hpp>
    LADS/SampleReception/fLogin.h:#include <DBTables.hpp>
    LADS/SampleReception/frmSampleList.h:#include <DBTables.hpp>
    LADS/SampleReception/incs.txt:#include <DBTables.hpp>
    LADS/SampleReception/incs.txt:#include <dbtables.hpp>
    LADS/SampleReception/incs.txt:#include <DBTables.hpp>
    LADS/SampleReception/incs.txt:#include <DBTables.hpp>
    LADS/SampleReception/incs.txt:#include <dbtables.hpp>
    LADS/SampleReception/Reception.h:#include <DBTables.hpp>
    LADS/SampleStatus/BoxCheck.h:#include <DBTables.hpp>
    LADS/SpecimenWipeout/fSpecimenWipeout.h:#include <DBTables.hpp>
    LADS/Storeman/.#SMLogin.h.1.3:#include <DBTables.hpp>
    LADS/Storeman/.#Storemain.h.1.12:#include <DBTables.hpp>
    Binary file LADS/Storeman/Win32/Debug/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Debug/Storemain.obj matches
    Binary file LADS/Storeman/Win32/Release/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Release/Storemain.obj matches
    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \
    LADS/Storeman/__history/SMLogin.h.~1~:#include <DBTables.hpp>
    LADS/Storeman/__history/Storemain.h.~1~:#include <DBTables.hpp>
    LADS/TestComms/ModifiedLogin.h:#include <DBTables.hpp>
    LADS/TestComms/TCLogin.h:#include <DBTables.hpp>
    LADS/TestComms/testMain.h:#include <DBTables.hpp>
    LADS/Validation/FinalScore.h:#include <DBTables.hpp>
    LADS/Validation/xexec.h:#include <DBTables.hpp>
    LADS/Validation/xquery.h:#include <DBTables.hpp>
    LADS/Validation/xsql.h:#include <DBTables.hpp>
    LADS/Wizard/AddBoxContent1.h:#include <DBTables.hpp>
    LADS/Wizard/AddBoxSize1.h:#include <DBTables.hpp>
    LADS/Wizard/AddCentralTest1.h:#include <DBTables.hpp>
    LADS/Wizard/AddDescriptor1.h:#include <DBTables.hpp>
    LADS/Wizard/AddMachine1.h:#include <DBTables.hpp>
    LADS/Wizard/AddObjectName1.h:#include <DBTables.hpp>
    LADS/Wizard/AddOperator1.h:#include <DBTables.hpp>
    LADS/Wizard/AddProfile1.h:#include <DBTables.hpp>
    LADS/Wizard/AddProfileMap1.h:#include <DBTables.hpp>
    LADS/Wizard/AddProfileMapNew1.h:#include <DBTables.hpp>
    LADS/Wizard/AddProject1.h:#include <DBTables.hpp>
    LADS/Wizard/AddProjectNote1.h:#include <DBTables.hpp>
    LADS/Wizard/AddQCLevel1.h:#include <DBTables.hpp>
    LADS/Wizard/AddQCLot1.h:#include <DBTables.hpp>
    LADS/Wizard/AddQCMaterial1.h:#include <DBTables.hpp>
    LADS/Wizard/AddQCTest1.h:#include <DBTables.hpp>
    LADS/Wizard/AddRackSize1.h:#include <DBTables.hpp>
    LADS/Wizard/AddTankMapping1.h:#include <DBTables.hpp>
    LADS/Wizard/AddTestCalculation1.h:#include <DBTables.hpp>
    LADS/Wizard/AddTestMachine1.h:#include <DBTables.hpp>
    LADS/Wizard/AddTestToProfile1.h:#include <DBTables.hpp>
    LADS/Wizard/AddValue1.h:#include <DBTables.hpp>
    LADS/Wizard/BoxContent1.h:#include <DBTables.hpp>
    LADS/Wizard/BoxContent1.h:#include <DBTables.hpp>
    LADS/Wizard/BoxSize1.h:#include <DBTables.hpp>
    LADS/Wizard/CalculationElementClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/CentralTest1.h:#include <DBTables.hpp>
    LADS/Wizard/CentralTest1.h:#include <DBTables.hpp>
    LADS/Wizard/CentralTestMachine1.h:#include <DBTables.hpp>
    LADS/Wizard/CentralTestMachineClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/DataMod1.h:#include <DBTables.hpp>
    LADS/Wizard/DateMod1.cpp:#include <Dbtables.hpp>
    LADS/Wizard/Descriptor1.h:#include <DBTables.hpp>
    LADS/Wizard/DescriptorClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/LeaseMod1.h:#include <DBTables.hpp>
    LADS/Wizard/Login1.h:#include <DBTables.hpp>
    LADS/Wizard/LogMod1.cpp:#include <Dbtables.hpp>
    LADS/Wizard/LogMod1.h:#include <DbTables.hpp>
    LADS/Wizard/Machine1.h:#include <DBTables.hpp>
    LADS/Wizard/Machine1.h:#include <DBTables.hpp>
    LADS/Wizard/MachineClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/MachineCReport1.h:#include <DBTables.hpp>
    LADS/Wizard/MachineIncludeClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/ObjectName1.h:#include <DBTables.hpp>
    LADS/Wizard/ObjectNameClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/Operator1.h:#include <DBTables.hpp>
    LADS/Wizard/Operator1.h:#include <DBTables.hpp>
    LADS/Wizard/OperatorClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/Profile1.h:#include <DBTables.hpp>
    LADS/Wizard/ProfileClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/ProfileMap1.h:#include <DBTables.hpp>
    LADS/Wizard/ProfileMapClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/ProfileReport1.h:#include <DBTables.hpp>
    LADS/Wizard/Project1.h:#include <DBTables.hpp>
    LADS/Wizard/Project1.h:#include <DBTables.hpp>
    LADS/Wizard/Project1.h:#include <DBTables.hpp>
    LADS/Wizard/ProjectClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/ProjectNoteClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/QCLevelClass.h:#include <DBTables.hpp>
    LADS/Wizard/QCLotClass.h:#include "dbtables.hpp"
    LADS/Wizard/QCMaterial1.h:#include <DBTables.hpp>
    LADS/Wizard/QCMaterialClass.h:#include "dbtables.hpp"
    LADS/Wizard/QCTestClass.h:#include <DBTables.hpp>
    LADS/Wizard/RackSize1.h:#include <DBTables.hpp>
    LADS/Wizard/RackSize1.h:#include <DBTables.hpp>
    LADS/Wizard/TestClass.cpp:#include <dbtables.hpp>
    LADS/Wizard/WizardLogin1.h:#include <DBTables.hpp>
    jonathanpr/attila/xdb.h:#include <DBTables.hpp>
    jonathanpr/attila/xexec.h:#include <DBTables.hpp>
    jonathanpr/attila/xquery.h:#include <DBTables.hpp>
    jonathanpr/attila/xsql.h:#include <DBTables.hpp>
    jonathanpr/BioBankPACStatusCGI/xdb.h:#include <DBTables.hpp>
    jonathanpr/BioBankPACStatusCGI/xexec.h:#include <DBTables.hpp>
    jonathanpr/BioBankPACStatusCGI/xquery.h:#include <DBTables.hpp>
    jonathanpr/BioBankPACStatusCGI/xsql.h:#include <DBTables.hpp>
    jonathanpr/BioBankPSICGI/xdb.h:#include <DBTables.hpp>
    jonathanpr/BioBankPSICGI/xexec.h:#include <DBTables.hpp>
    jonathanpr/BioBankPSICGI/xquery.h:#include <DBTables.hpp>
    jonathanpr/BioBankPSICGI/xsql.h:#include <DBTables.hpp>
    jonathanpr/DicomWorlistGenerator/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/DicomWorlistGenerator/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/DicomWorlistGenerator/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/DicomWorlistGenerator/CoreUtilities/xsql.h:#include <DBTables.hpp>
    jonathanpr/externalUsersLog/xdb.h:#include <DBTables.hpp>
    jonathanpr/externalUsersLog/xexec.h:#include <DBTables.hpp>
    jonathanpr/externalUsersLog/xquery.h:#include <DBTables.hpp>
    jonathanpr/externalUsersLog/xsql.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xsql.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xsql.h:#include <DBTables.hpp>
    2014-05-08 20:00:32 cbird@Q108 ~/Projects/retrieval-assistant
    $

Let's narrow that down a bit:

    LADS/CoreUtilities/xdb.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xexec.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xquery.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xsql.h:#include <DBTables.hpp>
    LADS/LabsDAO/LPDbBoxWork.h:#include <DBTables.hpp>
    LADS/LabsForms/BuddyLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmCommsConfig.h:#include <DBTables.hpp>
      LADS/LabsForms/TfrmCommsMain.h:#include <DBTables.hpp>
      LADS/LabsForms/TfrmLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmLoginA.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSerialConfig.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSocketComms.h:#include <DBTables.hpp>
      LADS/LabsLeasing/lease.h:#include <DBTables.hpp>
    LADS/Storeman/.#SMLogin.h.1.3:#include <DBTables.hpp>
    LADS/Storeman/.#Storemain.h.1.12:#include <DBTables.hpp>
    Binary file LADS/Storeman/Win32/Debug/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Debug/Storemain.obj matches
    Binary file LADS/Storeman/Win32/Release/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Release/Storemain.obj matches
    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \
    LADS/Storeman/__history/SMLogin.h.~1~:#include <DBTables.hpp>
    LADS/Storeman/__history/Storemain.h.~1~:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xsql.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xsql.h:#include <DBTables.hpp>

`Storemain.cpp` includes `LADS/LabsForms/TfrmLogin.h` which includes `<DBTables.hpp>`.

And further:

These are all guarded by X_BDE:

    LADS/CoreUtilities/xdb.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xexec.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xquery.h:#include <DBTables.hpp>
    LADS/CoreUtilities/xsql.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RationaliseTanks/CoreUtilities/xsql.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xdb.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xexec.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xquery.h:#include <DBTables.hpp>
    jonathanpr/RetrievalLists/CoreUtilities/xsql.h:#include <DBTables.hpp>

?

    LADS/LabsDAO/LPDbBoxWork.h:#include <DBTables.hpp>

Suspicious:

    LADS/LabsForms/BuddyLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmCommsConfig.h:#include <DBTables.hpp>
      LADS/LabsForms/TfrmCommsMain.h:#include <DBTables.hpp>
      LADS/LabsForms/TfrmLogin.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmLoginA.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSerialConfig.h:#include <DBTables.hpp>
    LADS/LabsForms/TfrmSocketComms.h:#include <DBTables.hpp>
      LADS/LabsLeasing/lease.h:#include <DBTables.hpp>

Backup files:

    LADS/Storeman/.#SMLogin.h.1.3:#include <DBTables.hpp>
    LADS/Storeman/.#Storemain.h.1.12:#include <DBTables.hpp>
    LADS/Storeman/__history/SMLogin.h.~1~:#include <DBTables.hpp>
    LADS/Storeman/__history/Storemain.h.~1~:#include <DBTables.hpp>

Binary files:

    Binary file LADS/Storeman/Win32/Debug/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Debug/Storemain.obj matches
    Binary file LADS/Storeman/Win32/Release/LPDbBoxes.obj matches
    Binary file LADS/Storeman/Win32/Release/Storemain.obj matches

Curious, these are generated, but why?

    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/LPDbBoxes.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\DBTables.hpp \
    LADS/Storeman/Win64/Debug/Storemain.d:  c:\program\ files\ (x86)\embarcadero\rad\ studio\11.0\include\windows\rtl\Bde.DBTables.hpp \

## commenting out #include <DBTables.hpp>

Commented out:

    //#include <DBTables.hpp>

in `TfrmLogin.h`, `TfrmLoginA.h` (not used as far as I can see), `TfrmCommsMain.h` (not used?), `lease.h`, no difference.

Then in `LPDbBoxWork.h`. Now get different error. Was it this last one, or all/some of the previous?

`TfrmLogin.h` - caused error
`TfrmLoginA.h` - no difference
`TfrmCommsMain.h` - no difference
`lease.h` - no difference
`LPDbBoxWork.h`.

And it's just DBTables.hpp, not DB.hpp, that causes the problem. So:

In `TfrmLogin.h` and `LPDbBoxWork.h`, comment out `#include <DBTables.hpp>`.

## project file problems

I think I must have merged StoreMan.cpp from CVS with the old form names before I renamed them, this caused lots of errors. Also the fact that Logfile.h had moved from CoreUtilities to LabsObjects.

## email to corprog

Hi All,

The latest Storeman that I pulled out of CVS gave me a linker error on 64-bit Debug build:

    [ilink64 Error] Fatal: Unable to open file 'DBRTL.LIB'

I tracked it down to a few files #including DBTables.hpp, which throws a wobbly on 64-bit. 

In the main database modules, that include is #if'd out with: 

    #if X_BDE

but there are a few files hanging around that include DBTables.hpp without any such checks. I found just commenting out the include in TfrmLogin.h and LPDbBoxWork.h got rid of the error, for now.

Why this has happened all of a sudden I don't know, and it may peculiar to my setup, but if you have the same problem, try commenting out that include wherever you can find it (out side of the X libraries) or wrapping it in #if X_BDE. 

Chris

## Full error message

    Checking project dependencies...
    Compiling Storeman.cbproj (Debug, Win64)
    bcc64 command line for "RetrAsstPlanSamples.cpp"
      c:\program files (x86)\embarcadero\rad studio\11.0\bin\bcc64.exe -cc1 -D _DEBUG -I C:\Users\cbird\Projects\retrieval-assistant\LADS\Storeman -I 
      "c:\program files (x86)\embarcadero\rad studio\11.0\include\windows\vcl" -I RetrievalLists -I RationaliseTanks -I "C:\Program 
      Files\Actian\IngresIJ\ingres\files" -I ..\LabsLeasing -I ..\..\paulst -I ..\LabsDAO -I ..\LabsObjects -I ..\CoreUtilities -I ..\LabsForms -isystem 
      "C:\Program Files (x86)\Embarcadero\RAD Studio\11.0\include\boost_1_50" -isystem "c:\program files (x86)\embarcadero\rad studio\11.0\include" 
      -isystem "c:\program files (x86)\embarcadero\rad studio\11.0\include\dinkumware64" -isystem "c:\program files (x86)\embarcadero\rad 
      studio\11.0\include\windows\crtl" -isystem "c:\program files (x86)\embarcadero\rad studio\11.0\include\windows\sdk" -isystem "c:\program files 
      (x86)\embarcadero\rad studio\11.0\include\windows\rtl" -isystem "c:\program files (x86)\embarcadero\rad studio\11.0\include\windows\vcl" -isystem 
      "c:\program files (x86)\embarcadero\rad studio\11.0\include\windows\fmx" -isystem "C:\Program Files (x86)\FastReports\LibD18x64" -isystem "C:\Program 
      Files (x86)\Raize\CS5\Lib\RS-XE4\Win64" -g -fno-limit-debug-info -fborland-extensions -nobuiltininc -nostdsysteminc -triple=x86_64-pc-win32-elf 
      -emit-obj -fexceptions -fcxx-exceptions -fseh -munwind-tables -mstackrealign -fno-spell-checking -fno-use-cxa-atexit -main-file-name 
      RetrAsstPlanSamples.cpp -x c++ -std=c++11 -O0 -tM -tU -tW -o .\Win64\Debug\RetrAsstPlanSamples.o -dependency-file .\Win64\Debug\RetrAsstPlanSamples.d 
      -MT .\Win64\Debug\RetrAsstPlanSamples.o -sys-header-deps RetrAsstPlanSamples.cpp 
    RetrAsstPlanSamples.cpp(1): RetrAsstPlanSamples.cpp
    RetrAsstPlanSamples.cpp
    ilink64 command line
      c:\program files (x86)\embarcadero\rad studio\11.0\bin\ilink64.exe -L.\Win64\Debug;"c:\program files (x86)\embarcadero\rad 
      studio\11.0\lib\Win64\debug";RetrievalLists;RationaliseTanks;"C:\Program Files\Actian\IngresIJ\ingres\files";..\LabsLeasing;..\..\paulst;..\LabsDAO;
      ..\LabsObjects;..\CoreUtilities;..\LabsForms;"c:\program files (x86)\embarcadero\rad studio\11.0\lib\win64\release";"c:\program files 
      (x86)\embarcadero\rad studio\11.0\lib\win64\release\psdk";"C:\Program Files (x86)\FastReports\LibD18x64";"C:\Program Files 
      (x86)\Raize\CS5\Lib\RS-XE4\Win64";"C:\Users\Public\Documents\RAD Studio\11.0\DCP\WIN64" -j.\Win64\Debug;"c:\program files (x86)\embarcadero\rad 
      studio\11.0\lib\Win64\debug";RetrievalLists;RationaliseTanks;"C:\Program Files\Actian\IngresIJ\ingres\files";..\LabsLeasing;..\..\paulst;..\LabsDAO;
      ..\LabsObjects;..\CoreUtilities;..\LabsForms;"c:\program files (x86)\embarcadero\rad studio\11.0\lib\win64\release";"c:\program files 
      (x86)\embarcadero\rad studio\11.0\lib\win64\release\psdk";"C:\Program Files (x86)\FastReports\LibD18x64";"C:\Program Files 
      (x86)\Raize\CS5\Lib\RS-XE4\Win64";"C:\Users\Public\Documents\RAD Studio\11.0\DCP\WIN64" -l.\Win64\Debug -v -Gn 
      -GA"C:\Users\cbird\AppData\Local\Temp\vfsDFB9.tmp"="C:\Users\cbird\Projects\retrieval-assistant\LADS\Storeman\RetrAsstPlanSamples.dfm" 
      -GA"C:\Users\cbird\AppData\Local\Temp\vfsDFBA.tmp"="C:\Users\cbird\Projects\retrieval-assistant\LADS\LabsForms\TfrmLoginBase.dfm" -aa -V5.0 -Tpe  
      c0w64w memmgr.a sysinit.o .\Win64\Debug\rosebase.o .\Win64\Debug\rosetta.o .\Win64\Debug\xB36Code.o .\Win64\Debug\xbinob.o .\Win64\Debug\xdb.o 
      .\Win64\Debug\xencode.o .\Win64\Debug\xerror.o .\Win64\Debug\xexec.o .\Win64\Debug\xMD5.o .\Win64\Debug\xquery.o .\Win64\Debug\xsql.o 
      .\Win64\Debug\xtime.o .\Win64\Debug\LCDbAnalyser.o .\Win64\Debug\LCDbAuditTrail.o .\Win64\Debug\LCDbJob.o .\Win64\Debug\LCDbObject.o 
      .\Win64\Debug\LCDbOperator.o .\Win64\Debug\LCDbProject.o .\Win64\Debug\LCDbRack.o .\Win64\Debug\LCDbRetrieval.o .\Win64\Debug\LCDbStoreDetail.o 
      .\Win64\Debug\LCDbTankLayout.o .\Win64\Debug\LCDbTankMap.o .\Win64\Debug\LDbBoxSize.o .\Win64\Debug\LDbBoxStore.o .\Win64\Debug\LDbBoxType.o 
      .\Win64\Debug\LDbIdClasses.o .\Win64\Debug\LDbNameBase.o .\Win64\Debug\LDbNotes.o .\Win64\Debug\LDbResult.o .\Win64\Debug\LDbSource.o 
      .\Win64\Debug\LDbStage.o .\Win64\Debug\LDbValid.o .\Win64\Debug\LPDbBoxes.o .\Win64\Debug\LPDbCryovial.o .\Win64\Debug\LPDbCryovialStore.o 
      .\Win64\Debug\LPDbDescriptor.o .\Win64\Debug\LPDbProfile.o .\Win64\Debug\LPDbProfileMap.o .\Win64\Debug\dvSelector.o .\Win64\Debug\TfrmAboutBox.o 
      .\Win64\Debug\TfrmConfirm.o .\Win64\Debug\TfrmLoginBase.o .\Win64\Debug\TfrmPassword.o .\Win64\Debug\LIMSDatabase.o .\Win64\Debug\LIMSParams.o 
      .\Win64\Debug\LQuery.o .\Win64\Debug\TestRange.o .\Win64\Debug\ValueFields.o .\Win64\Debug\AddSpecimens.o .\Win64\Debug\AnalyseSamples.o 
      .\Win64\Debug\BoxSummary.o .\Win64\Debug\BoxTransfer.o .\Win64\Debug\BrowseSample.o .\Win64\Debug\Configure.o .\Win64\Debug\DiscardContext.o 
      .\Win64\Debug\DiscardDb.o .\Win64\Debug\DiscardLogic.o .\Win64\Debug\DiscardMethod.o .\Win64\Debug\DiscardNote.o .\Win64\Debug\DiscardPrologue.o 
      .\Win64\Debug\DiscardReason.o .\Win64\Debug\DiscardSamples.o .\Win64\Debug\DiscardSearch.o .\Win64\Debug\DiscardSef.o .\Win64\Debug\DiscardUtil.o 
      .\Win64\Debug\FindSample.o .\Win64\Debug\Inventory.o .\Win64\Debug\MoveJobs.o .\Win64\Debug\NewBoxType.o .\Win64\Debug\NewExercise.o 
      .\Win64\Debug\NewJob.o .\Win64\Debug\NewSection.o .\Win64\Debug\NewSite.o .\Win64\Debug\NewTank.o .\Win64\Debug\NewType.o 
      .\Win64\Debug\RatTanksBoxSelection.o .\Win64\Debug\RatTanksBoxType.o .\Win64\Debug\RatTanksCreateExercise.o 
      .\Win64\Debug\RatTanksCreateRetrievalJob.o .\Win64\Debug\RatTanksCtrl.o .\Win64\Debug\RatTanksDatabase.o .\Win64\Debug\RatTanksdbSelection.o 
      .\Win64\Debug\RatTanksMainDialog.o .\Win64\Debug\RatTanksOperatorSignedIn.o .\Win64\Debug\RatTanksPleaseWait.o .\Win64\Debug\RatTanksSignIn.o 
      .\Win64\Debug\ReferredBoxes.o .\Win64\Debug\ReferredBoxesSummary.o .\Win64\Debug\RetrAsstCollectBoxes.o .\Win64\Debug\RetrAsstCollectEmpties.o 
      .\Win64\Debug\RetrAsstCollectSamples.o .\Win64\Debug\RetrAsstMain.o .\Win64\Debug\RetrAsstPlanBoxes.o .\Win64\Debug\RetrAsstPlanSamples.o 
      .\Win64\Debug\RetrievalListCryovialHistoryDialog.o .\Win64\Debug\RetrievalListCtrl.o .\Win64\Debug\RetrievalListDatabase.o 
      .\Win64\Debug\RetrievalListGridUtils.o .\Win64\Debug\retrievalListMainDialog.o .\Win64\Debug\RetrievalListMainListDialog.o 
      .\Win64\Debug\RetrievalListResultNotesDialog.o .\Win64\Debug\RetrievalListResultsDialog.o .\Win64\Debug\RetrieveSamples.o .\Win64\Debug\SampleEntry.o 
      .\Win64\Debug\SampleList.o .\Win64\Debug\SampleMove.o .\Win64\Debug\SelectSamples.o .\Win64\Debug\showsamples.o .\Win64\Debug\SMLogin.o 
      .\Win64\Debug\StoreDAO.o .\Win64\Debug\Storemain.o .\Win64\Debug\StoreMan.o .\Win64\Debug\StoreUtil.o .\Win64\Debug\AcquireCriticalSection.o 
      .\Win64\Debug\CritSec.o .\Win64\Debug\Event.o .\Win64\Debug\ExceptionUtil.o .\Win64\Debug\Properties.o .\Win64\Debug\StrBuilder.o 
      .\Win64\Debug\StringDB.o .\Win64\Debug\StrUtil.o .\Win64\Debug\Thread.o .\Win64\Debug\WorkerThread.o , .\Win64\Debug\Storeman.exe , 
      .\Win64\Debug\Storeman.map , vcl.a rtl.a vclimg.a IndyCore.a IndySystem.a IndyProtocols.a vclx.a iiapi_w64.a import64.a cp64mt.a , , Storeman.res 
    [ilink64 Error] Fatal: Unable to open file 'DBRTL.LIB'
    Failed
    Elapsed time: 00:00:14.8