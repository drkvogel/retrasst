#ifndef UTILS_H_
#define UTILS_H_


#include <vcl.h>
#include <Vcl.Grids.hpp>
#include <memory>
#include <vector>
#include <map>
#include "xexec.h"
#include "xdb.h"
#include "xquery.h"

class utils
{
public:
	static void resizeMemo(TMemo *Memo, float MinimalSize)
	{
		int lineHight = -Memo->Font->Height + 4;
		float noLines = Memo->Lines->Count+1.5;
		if (noLines < MinimalSize)
			noLines = MinimalSize;

		int newSize = (int) (noLines * lineHight);
		if (Memo->Height != newSize)
		{
			Memo->Height = newSize;
		}
	}

 	static void reSizeGrid(TForm *Dlg,TStringGrid *StringGrid)
	{
		int TotalSize = 0;
		int MaxLenRow = 0;
		int ColCount = StringGrid->ColCount;

	//SPREAD EACH COL SO IT DISPLAYS ITS INFORMATION
		for (int c = 1 ; c < ColCount; c++)
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
			for (int c = 1 ; c < ColCount-1; c++)
				StringGrid->ColWidths[c] += SpareSpace;

			for (int c = 1 ; c < ColCount; c++)
				currentsize += StringGrid->ColWidths[c];

	//HAVE THE LAST COL COVER ANYTHING EXTRA WHICH IS LEFT.
			int DIFF = CW - currentsize;
			StringGrid->ColWidths[ColCount-1] += DIFF-size1;
		}
	}

	static void SortStringGrid(TStringGrid *GenStrGrid, int ThatCol)
	{
		String TheSeparator = "@";
		int CountItem, ThePosition;

		std::unique_ptr<TStringList> MyList = std::unique_ptr<TStringList>( new TStringList() );
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
			ThePosition = MyString.Pos(TheSeparator);

			TempString = MyString.SubString(ThePosition + 1,MyString.Length() - ThePosition);
			MyList->Strings[i] = TempString;
		}
		for (int i = 1;i<CountItem;i++)
			GenStrGrid->Rows[i]->Text = MyList->Strings[(i - 1)];
	}

	static int getNextValueFor(XDB *db, const std::string &seqName)
	{
		int UID = -1;
		std::string nextUID = std::string("select NEXT VALUE for ") + seqName;
		XQUERY query(db,nextUID);
		if (query.open())
		{
			if (query.fetch())    //any data?
			{
				UID = query.result.getLint(0);
			}
			query.close();
		}
		if (UID == -1)
			throw (new std::string("Error accessing database"));
		return UID;
	}
};



// Split string into parts.
//stringsplit split("hello,world", ",");


class stringsplit : public std::vector<std::string>
{
public:
	stringsplit(const std::string& str, char* delimList)
	{
		size_t lastPos = 0;
		size_t pos = str.find_first_of(delimList);

		while (pos != std::string::npos)
		{
			if (pos != lastPos)
				push_back(str.substr(lastPos, pos-lastPos));
			lastPos = pos + 1;
			pos = str.find_first_of(delimList, lastPos);
		}
		if (lastPos < str.length())
			push_back(str.substr(lastPos, pos-lastPos));
	}
};


class util_iHolder :public TObject
{
 public:
	util_iHolder(int i)
	{
		value = i;
	}

	int value;
 };

class util_sHolder :public TObject
{
 public:
	util_sHolder(std::vector<String> &mystrings)
	{
		m_strings = mystrings;
	}

	std::vector<String> m_strings;
 };

 class util_mapHolder : public TObject
 {
 public:
	util_mapHolder(std::map<String,String> &myMap)
	{
		m_map = myMap;
	}
	std::map<String,String> m_map;
 };

 class util_stdStringmapHolder : public TObject
 {
 public:
	util_stdStringmapHolder(std::map<std::string,std::string> &myMap)
	{
		m_map = myMap;
	}
	std::map<std::string,std::string> m_map;
 };

#endif
