//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("tmessage.cpp", fm_tmessage);
USEFORM("result.cpp", fm_result);
USEFORM("tTrailBoard.cpp", TrailBoard);
USEFORM("tSymbolDigitBoard.cpp", symbolDigitBoard);
USEFORM("board.cpp", fm_board);
USEFORM("main.cpp", Form1);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(Tfm_result), &fm_result);
		Application->CreateForm(__classid(Tfm_board), &fm_board);
		Application->CreateForm(__classid(TTrailBoard), &TrailBoard);
		Application->CreateForm(__classid(Tfm_tmessage), &fm_tmessage);
		Application->CreateForm(__classid(TsymbolDigitBoard), &symbolDigitBoard);
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
