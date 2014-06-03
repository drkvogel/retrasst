//---------------------------------------------------------------------------

#ifndef TSnapshotFrameH
#define TSnapshotFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------

class TSnapshotFrame;

/** An interface to which widget changes (eg button clicks) are broadcast. */
class SnapshotFrameObserver
{
public:
	SnapshotFrameObserver();
	virtual ~SnapshotFrameObserver();
	virtual void notifyAssociatedWith(TSnapshotFrame* tsf) = 0;
	virtual void notifyDestroyed(TSnapshotFrame* tsf) = 0;
	virtual void notifyForceReloadButtonClick(TSnapshotFrame* tsf) = 0;
	virtual void notifyMainSplitterMouseUp(TSnapshotFrame* tsf) = 0;
	virtual void notifySelected(int worklistEntryID) = 0;
};

class TSnapshotFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *snapshotFramePanel;
	TToolBar *MainToolBar;
	TSpeedButton *ForceReloadButton;
	TStatusBar *MainStatusBar;
	TLabel *StatusLabel;
	TPanel *ResultsPanel;
	TScrollBox *ResultsScrollBox;
	TPanel *ResultsInnerPanel;
	TPanel *WorkPanel;
	TScrollBox *WorkScrollBox;
	TPanel *WorkInnerPanel;
	TStyleBook *ValCStyleBook;
	TSplitter *MainSplitter;
	TImage *ToolbarBackground;
	void __fastcall ForceReloadButtonClick(TObject *Sender);
	void __fastcall MainSplitterMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, float X, float Y);

private:	// User declarations
	SnapshotFrameObserver* m_observer;

public:		// User declarations
	__fastcall TSnapshotFrame(TComponent* Owner);
	__fastcall ~TSnapshotFrame();
	void setObserver( SnapshotFrameObserver* obs );
};
//---------------------------------------------------------------------------
extern PACKAGE TSnapshotFrame *SnapshotFrame;
//---------------------------------------------------------------------------
#endif
