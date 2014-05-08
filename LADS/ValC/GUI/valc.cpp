//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("TSnapshotFrame.cpp", SnapshotFrame); /* TFrame: File Type */
USEFORM("TWaitDlg.cpp", WaitDlg);
USEFORM("TLogFrame.cpp", LogFrame); /* TFrame: File Type */
USEFORM("TMainForm.cpp", MainForm);
USEFORM("TQCViewFrame.cpp", QCViewFrame); /* TFrame: File Type */
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
