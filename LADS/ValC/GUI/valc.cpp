//---------------------------------------------------------------------------


#include <fmx.h>

#pragma hdrstop

#include <tchar.h>

//---------------------------------------------------------------------------

USEFORM("TBatchNavFrame.cpp", BatchNavFrame); /* TFrame: File Type */
USEFORM("TBatchViewFrame.cpp", BatchViewFrame); /* TFrame: File Type */
USEFORM("TQCViewFrame.cpp", QCViewFrame); /* TFrame: File Type */
USEFORM("TRuleFrame.cpp", RuleFrame); /* TFrame: File Type */
USEFORM("TLogFrame.cpp", LogFrame); /* TFrame: File Type */
USEFORM("TMainForm.cpp", MainForm);
USEFORM("TWaitDlg.cpp", WaitDlg);
USEFORM("TWorklistItemViewFrame.cpp", WorklistItemViewFrame); /* TFrame: File Type */
USEFORM("TSampleRunFrame.cpp", SampleRunFrame); /* TFrame: File Type */
USEFORM("TSnapshotFrame.cpp", SnapshotFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern "C" int FMXmain()

{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();
	}

	catch (Exception &exception)

	{

		Application->ShowException(&exception);

	}

	catch (...)

	{

		try

		{

			throw Exception("");

		}

		catch (Exception &exception)

		{

			Application->ShowException(&exception);

		}

	}

	return 0;

}

//---------------------------------------------------------------------------

