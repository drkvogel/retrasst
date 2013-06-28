//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Wizard.res");
USEUNIT("QCMaterialClass.cpp");
USEUNIT("QCLotClass.cpp");
USEUNIT("QCTestClass.cpp");
USEUNIT("QCLevelClass.cpp");
USEUNIT("rexec_ob.cpp");
USEFORM("Login1.cpp", Login);
USEFORM("WizardLogin1.cpp", WizardLogin);
USEFORM("AddValue1.cpp", AddValue);
USEFORM("Machine1.cpp", Machine);
USEFORM("AddMachine1.cpp", AddMachine);
USEFORM("Profile1.cpp", Profile);
USEFORM("AddProfile1.cpp", AddProfile);
USEFORM("AddTestToProfile1.cpp", AddTestToProfile);
USEFORM("ProfileMap1.cpp", ProfileMap);
USEFORM("ChooseProfile1.cpp", ChooseProfile);
USEFORM("EditCentralDb1.cpp", EditCentralDb);
USEFORM("EditProjectDb1.cpp", EditProjectDb);
USEFORM("AddProfileMapNew1.cpp", AddProfileMapNew);
USEFORM("RackSize1.cpp", RackSize);
USEFORM("AddTestCalculation1.cpp", AddTestCalculation);
USEFORM("AddRackSize1.cpp", AddRackSize);
USEFORM("BoxContent1.cpp", BoxContent);
USEFORM("AddTankMapping1.cpp", AddTankMapping);
USEFORM("ProfileReport1.cpp", ProfileReport);
USEFORM("MachineCReport1.cpp", MachineCReport);
USEFORM("ChooseItem1.cpp", ChooseItem);
USEFORM("Operator1.cpp", Operator);
USEFORM("AddOperator1.cpp", AddOperator);
USEFORM("AddQCTest1.cpp", AddQCTest);
USEFORM("CentralTest1.cpp", CentralTest);
USEFORM("CentralTestMachine1.cpp", CentralTestMachine);
USEFORM("AddCentralTest1.cpp", AddCentralTest);
USEFORM("AddTestMachine1.cpp", AddTestMachine);
USEFORM("AddObjectName1.cpp", AddObjectName);
USEFORM("ObjectName1.cpp", ObjectName);
USEFORM("AddProject1.cpp", AddProject);
USEFORM("Descriptor1.cpp", Descriptor);
USEFORM("QCMaterial1.cpp", QCMaterial);
USEFORM("AddQCMaterial1.cpp", AddQCMaterial);
USEFORM("AddQCLevel1.cpp", AddQCLevel);
USEFORM("AddQCLot1.cpp", AddQCLot);
USEFORM("Project1.cpp", Project);
USEFORM("AddDescriptor1.cpp", AddDescriptor);
USEFORM("CreateProjectDatabase1.cpp", CreateProjectDatabase);
USEFORM("AddProjectNote1.cpp", AddProjectNote);
USEUNIT("..\CoreUtilities\LQuery.cpp");
USEUNIT("..\LabsDAO\LCDbAuditTrail.cpp");
USEUNIT("..\CoreUtilities\LogFile.cpp");
USEUNIT("..\CoreUtilities\xmd5.cpp");
USEUNIT("..\LabsLeasing\lease.cpp");
USEUNIT("..\LabsDAO\LCDbAnalyser.cpp");
USEUNIT("..\CoreUtilities\xb36Code.cpp");
USEUNIT("..\LabsDAO\LCDbProject.cpp");
USEUNIT("..\CoreUtilities\XMLFile.cpp");
USEUNIT("..\LabsDAO\LCDbOperator.cpp");
USEUNIT("..\LabsDAO\LDbIdClasses.cpp");
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEUNIT("..\LabsDAO\LCDbObject.cpp");
USEUNIT("..\LabsDAO\LCDbTest.cpp");
USEUNIT("..\LabsDAO\LPDbProfile.cpp");
USEUNIT("..\LabsDAO\LPDbDescriptor.cpp");
USEUNIT("..\LabsObjects\LIMSProfileMap.cpp");
USEUNIT("..\LabsDAO\LDbNotes.cpp");
USEUNIT("..\LabsObjects\LIMSParams.cpp");
USEUNIT("..\LabsDAO\LPDbProfileMap.cpp");
USEFORM("BoxSize1.cpp", BoxSize);
USEUNIT("..\LabsDAO\LCDbBoxStore.cpp");
USEFORM("AddBoxSize1.cpp", AddBoxSize);
USEFORM("AddBoxContent1.cpp", AddBoxContent);
USEUNIT("..\LabsDAO\LDbBoxSize.cpp");
USEUNIT("..\LabsDAO\LDbBoxType.cpp");
USEUNIT("..\StringGrid\ColourStringGrid.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Labs Wizard";
		Application->CreateForm(__classid(TWizardLogin), &WizardLogin);
		Application->CreateForm(__classid(TLogin), &Login);
		Application->CreateForm(__classid(TAddProject), &AddProject);
		Application->CreateForm(__classid(TDescriptor), &Descriptor);
		Application->CreateForm(__classid(TAddValue), &AddValue);
		Application->CreateForm(__classid(TMachine), &Machine);
		Application->CreateForm(__classid(TAddMachine), &AddMachine);
		Application->CreateForm(__classid(TProfile), &Profile);
		Application->CreateForm(__classid(TAddProfile), &AddProfile);
		Application->CreateForm(__classid(TAddTestToProfile), &AddTestToProfile);
		Application->CreateForm(__classid(TProfileMap), &ProfileMap);
		Application->CreateForm(__classid(TChooseProfile), &ChooseProfile);
		Application->CreateForm(__classid(TEditCentralDb), &EditCentralDb);
		Application->CreateForm(__classid(TEditProjectDb), &EditProjectDb);
		Application->CreateForm(__classid(TProfileReport), &ProfileReport);
		Application->CreateForm(__classid(TMachineCReport), &MachineCReport);
		Application->CreateForm(__classid(TChooseItem), &ChooseItem);
		Application->CreateForm(__classid(TOperator), &Operator);
		Application->CreateForm(__classid(TAddOperator), &AddOperator);
		Application->CreateForm(__classid(TAddQCTest), &AddQCTest);
		Application->CreateForm(__classid(TQCMaterial), &QCMaterial);
		Application->CreateForm(__classid(TAddQCMaterial), &AddQCMaterial);
		Application->CreateForm(__classid(TAddQCLevel), &AddQCLevel);
		Application->CreateForm(__classid(TAddQCLot), &AddQCLot);
		Application->CreateForm(__classid(TProject), &Project);
		Application->CreateForm(__classid(TAddDescriptor), &AddDescriptor);
		Application->CreateForm(__classid(TCreateProjectDatabase), &CreateProjectDatabase);
		Application->CreateForm(__classid(TAddProjectNote), &AddProjectNote);
		Application->CreateForm(__classid(TAddTestCalculation), &AddTestCalculation);
		Application->CreateForm(__classid(TAddProfileMapNew), &AddProfileMapNew);
		Application->CreateForm(__classid(TRackSize), &RackSize);
		Application->CreateForm(__classid(TAddRackSize), &AddRackSize);
		Application->CreateForm(__classid(TBoxContent), &BoxContent);
		Application->CreateForm(__classid(TAddTankMapping), &AddTankMapping);
		Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
		Application->CreateForm(__classid(TCentralTest), &CentralTest);
		Application->CreateForm(__classid(TCentralTestMachine), &CentralTestMachine);
		Application->CreateForm(__classid(TAddCentralTest), &AddCentralTest);
		Application->CreateForm(__classid(TAddTestMachine), &AddTestMachine);
		Application->CreateForm(__classid(TAddObjectName), &AddObjectName);
		Application->CreateForm(__classid(TObjectName), &ObjectName);
		Application->CreateForm(__classid(TBoxSize), &BoxSize);
		Application->CreateForm(__classid(TAddBoxSize), &AddBoxSize);
		Application->CreateForm(__classid(TAddBoxContent), &AddBoxContent);
		Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
