//---------------------------------------------------------------------------

#ifndef tSymbolDigitBoardH
#define tSymbolDigitBoardH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "rosetta.h"
#include "symbolDigitGame.h"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TsymbolDigitBoard : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_abandon;
	TPanel *panel_play;
	TImage *image_play;
	TTimer *gameTimer;
	TTimer *flashTimer;
	TButton *NextButton;
	TSaveTextFileDialog *SaveTextFileDialog;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall panel_abandonClick(TObject *Sender);
	void __fastcall image_playMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall gameTimerTimer(TObject *Sender);
	void __fastcall flashTimerTimer(TObject *Sender);
	void __fastcall NextButtonClick(TObject *Sender);

private:	// User declarations
	bool	in_game;
	int	score;
	int	level;
	int	margin;
	SYMBOLDIGIT_GAME	*game;
	void 	resetBoard( void );
	void 	abandonGame( void );
	void	extractResults();
	std::string toSTD(String in);

public:		// User declarations
	void setGameTimeOut(bool start);
	void setFlashTimeOut(bool start, int timeout);
	bool	is_valid;
	bool	abandon, bailout;
	static	ROSETTA	*results;
public:		// User declarations
	__fastcall TsymbolDigitBoard(TComponent* Owner);
	bool playGame( TForm *caller, int gameid );
};
//---------------------------------------------------------------------------
extern PACKAGE TsymbolDigitBoard *symbolDigitBoard;
//---------------------------------------------------------------------------
#endif
