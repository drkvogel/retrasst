//---------------------------------------------------------------------------

#ifndef tTrailBoardH
#define tTrailBoardH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "trailgame.h"
#include "xtime.h"
#include "rosetta.h"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
#define	PLAY_BACKGROUND	((TColor) 0xFFFFFF)
#define	PLAY_BACKGROUND_ERROR	((TColor) 0x0000FF)
#define	PLAY_BACKGROUND_DONE	((TColor) 0x004000)
//---------------------------------------------------------------------------
class TTrailBoard : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_play;
	TImage *image_play;
	TPanel *panel_abandon;
	TTimer *bgFlashTimer;
	TButton *NextButton;
	TSaveTextFileDialog *SaveTextFileDialog;
	void __fastcall image_playMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,int X, int Y);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall panel_abandonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bgFlashTimerTimer(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);
private:	// User declarations
	TRAIL_GAME	*game;
	bool	in_game;
	int		score;
	int		level;
	int		margin;

	void 	abandonGame( void );
	void 	clearBoard( void );
	void	extractResults( void );
   std::string TTrailBoard::toSTD(String in);
public:		// User declarations
	void 	resetBoard( void );
   	void doFlashBackGround();
	__fastcall TTrailBoard(TComponent* Owner);

	bool	is_valid;
	bool	abandon, bailout;
	bool 	playGame( TForm *caller, int gameid  );
	static	ROSETTA	*results;

	System::TDateTime m_startTime;
};
//---------------------------------------------------------------------------
extern PACKAGE TTrailBoard *TrailBoard;
//---------------------------------------------------------------------------
#endif
