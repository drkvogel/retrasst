//---------------------------------------------------------------------------


#include <fmx.h>

#pragma hdrstop

#include <tchar.h>

//---------------------------------------------------------------------------

USEFORM("TWaitDlg.cpp", WaitDlg);

USEFORM("TSnapshotFrame.cpp", SnapshotFrame); /* TFrame: File Type */

USEFORM("TWorklistItemViewFrame.cpp", WorklistItemViewFrame); /* TFrame: File Type */

USEFORM("TLogFrame.cpp", LogFrame); /* TFrame: File Type */

USEFORM("TQCViewFrame.cpp", QCViewFrame); /* TFrame: File Type */

USEFORM("TMainForm.cpp", MainForm);

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

