//---------------------------------------------------------------------------

#ifndef dvSelectorH
#define dvSelectorH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ValueFields.h"

//---------------------------------------------------------------------------

class TselectorFrame : public TFrame
{
__published:	// IDE-managed Components

	TButton *btnClear;
	TComboBox *cbDescrip;
	TListBox *lbDValues;
	TListBox *lbDVSelected;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label4;
	
	void __fastcall cbDescripChange(TObject *Sender);
	void __fastcall cbDescripDropDown(TObject *Sender);
	void __fastcall lbDValuesClick(TObject *Sender);
	void __fastcall lbDVSelectedClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);

private:	// User declarations

	ValueFields selected;
	std::set< int > valid, available;
	bool showAll, modified, finished;

	void listValues( const LPDbDescriptor * descrip );
	int nextSharedDID();

public:		// User declarations

	__fastcall TselectorFrame(TComponent* Owner);

	void prepare( bool allDescriptors );
	void allow( const LPDbDescriptor & descriptor );
	bool setDescriptor( const std::string & name );
	void remove( const LPDbDescriptor & descriptor );
	void updateDisplay();

	const LPDbDescriptor * getDescriptor() const;
	std::string getValueName() const;
	
	void select( const ValueField & dvPair );
	const ValueFields & getSelected();

	bool isFinished() const { return finished; }
	void forceUpdate() { modified = true; }
	bool isUpdated() const { return modified; }
};

//---------------------------------------------------------------------------

extern PACKAGE TselectorFrame *selectorFrame;

//---------------------------------------------------------------------------
#endif
