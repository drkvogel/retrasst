//---------------------------------------------------------------------------

#pragma hdrstop

#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <memory>
#include <vcl.h>


void Utils::reSizeGrid(TForm *Dlg,TStringGrid *StringGrid, bool  hideList)
{
	int TotalSize = 0;
	int MaxLenRow = 0;
	int ColCount = StringGrid->ColCount;
	if (hideList)
	{
		ColCount-=1;
		StringGrid->ColWidths[ColCount] = 0;
	}

//SPREAD EACH COL SO IT DISPLAYS ITS INFORMATION
	for (int c = 0 ; c < ColCount; c++)
	{
		MaxLenRow = 0;
		for (int r = 0 ; r < StringGrid->RowCount; r++)
		{
			int currentLen = StringGrid->Canvas->TextWidth(Trim(StringGrid->Cells[c][r]));
			if (MaxLenRow < currentLen + 10)
				MaxLenRow = currentLen + 10;
		}
		StringGrid->ColWidths[c] = MaxLenRow;
		TotalSize += MaxLenRow;
	}

	int CW = StringGrid->ClientWidth;

	int size1 = GetSystemMetrics(SM_CXVSCROLL);
	int currentsize = 0;
	if (TotalSize < (StringGrid->ClientWidth))
	{
//SPREAD THE GRID IF THE GRID IS SMALLER THEN THE DIALOG
		int SpareSpace = ((StringGrid->ClientWidth)-TotalSize)/(ColCount);
		for (int c = 0 ; c < ColCount-1; c++)
			StringGrid->ColWidths[c] += SpareSpace;

		for (int c = 0 ; c < ColCount; c++)
			currentsize += StringGrid->ColWidths[c];

//HAVE THE LAST COL COVER ANYTHING EXTRA WHICH IS LEFT.
		int DIFF = CW - currentsize;
		StringGrid->ColWidths[ColCount-1] += DIFF-size1;
	}
}


void Utils::SortStringGrid(TStringGrid *GenStrGrid, int ThatCol)
{
	String TheSeparator = "@";
	int CountItem, ThePosition;

	std::auto_ptr<TStringList> MyList = std::auto_ptr<TStringList>( new TStringList() );
	MyList->Sorted = false;

	String MyString, TempString;
	CountItem = GenStrGrid->RowCount;

	for (int i = 1;i< CountItem;i++)
	{
		MyList->Add(GenStrGrid->Cells[ThatCol][i] + TheSeparator + GenStrGrid->Rows[i]->Text);
		TempString = GenStrGrid->Rows[i]->Text;
	}
 	MyList->Sort();

	for (int i = 0;i<MyList->Count;i++)
	{
		MyString = MyList->Strings[i];
		ThePosition = Pos(TheSeparator, MyString);

		TempString = MyString.SubString(ThePosition + 1,MyString.Length() - ThePosition);
		MyList->Strings[i] = TempString;
	}
	for (int i = 1;i<CountItem;i++)
		GenStrGrid->Rows[i]->Text = MyList->Strings[(i - 1)];
}


void Utils::setRichTextType(RichFont type,TRichEdit *RichEdit)
{
	switch (type)
	{
	case FONT_H1:
		RichEdit->SelAttributes->Size = 13;
		if(!RichEdit->SelAttributes->Style.Contains(fsBold))
			RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style << fsBold;
		RichEdit->SelAttributes->Color = clRed;
	break;
	case FONT_NORMAL:
		RichEdit->SelAttributes->Size = 8;
		if(RichEdit->SelAttributes->Style.Contains(fsBold))
			RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style >> fsBold;
		RichEdit->SelAttributes->Color = clWindowText;
	break;
	case FONT_HI_LIGHT:
		RichEdit->SelAttributes->Size = 8;
		if(!RichEdit->SelAttributes->Style.Contains(fsBold))
			RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style << fsBold;
		RichEdit->SelAttributes->Color = clWindowText;
	break;
	}
}


