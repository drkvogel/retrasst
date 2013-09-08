//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("main.cpp", Form1);
USEFORM("..\LADS\Storeman\ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("..\LADS\Storeman\RetrievalAssistant.cpp", frmRetrievalAssistant);
USEFORM("..\LADS\Storeman\RetrievalAssistantAutoChunk.cpp", frmAutoChunk);
USEFORM("..\LADS\Storeman\RetrievalAssistantBoxes.cpp", frmBoxes);
USEFORM("..\LADS\Storeman\RetrievalAssistantProcess.cpp", frmProcess);
USEFORM("..\LADS\Storeman\RetrievalAssistantSamples.cpp", frmSamples);
USEFORM("..\LADS\Storeman\Temp.cpp", frmTest);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         SetApplicationMainFormOnTaskBar(Application, true);
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TfrmReferredBoxesSummary), &frmReferredBoxesSummary);
         Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
         Application->CreateForm(__classid(TfrmAutoChunk), &frmAutoChunk);
         Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
         Application->CreateForm(__classid(TfrmProcess), &frmProcess);
         Application->CreateForm(__classid(TfrmSamples), &frmSamples);
         Application->CreateForm(__classid(TfrmTest), &frmTest);
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
