//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Objects.hpp>
#include <FMX.TreeView.hpp>

#include <string>

#include "Config.h"
#include "DirUtil.h"
#include "DataManager.h"
#include "GUImanager.h"
#include "LogManager.h"


//---------------------------------------------------------------------------

/** The main form for the entire program. */
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *MainToolBar;
	TStatusBar *MainStatusBar;
	TPanel *ResultsPanel;
	TPanel *WorkPanel;
	TStyleBook *ValCStyleBook;
	TSpeedButton *ForceReloadButton;
	TSplitter *MainSplitter;
	TLabel *StatusLabel;
	TImage *ToolbarBackground;
	TPanel *ResultsInnerPanel;
	TPanel *WorkInnerPanel;
	TScrollBox *WorkScrollBox;
	TScrollBox *ResultsScrollBox;
	void __fastcall ForceReloadButtonClick(TObject *Sender);
	void __fastcall MainSplitterMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations

	valc::SnapshotPtr snapshot;  // the current snapshot
	// (lifetime of snapshot managed by business layer)
    paulst::Dir appDataDir;

	void clearAll();
	void forceReload();

public:
	/** the (singleton) object that manages most GUI operations, apart from the main form itself */
	GUImanager *guiManager;

	/** the (singleton) object that manages all the data operations,
	  * especially extracting the data from the current snapshot */
	DataManager *dataManager;

	/** Manages the operations logging activities of ValC, including a log file
	  * and (optionally) a logging window.   */
	LogManager *logManager;

	/** Holds the top-level configuration values. Currently from the
	  * config file  config-top.txt     */
	paulst::Config config;




	__fastcall TMainForm(TComponent* Owner);
	__fastcall ~TMainForm();


};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
