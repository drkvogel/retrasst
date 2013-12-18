//---------------------------------------------------------------------------

#ifndef RetrievalListGridUtilsH
#define RetrievalListGridUtilsH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

class RetrivalListGridUtils
{
public:
	enum RichFont {FONT_H1,FONT_NORMAL,FONT_HI_LIGHT};

	RetrivalListGridUtils() {};
	~RetrivalListGridUtils() {};

	static void reSizeGrid(TForm *Dlg,TStringGrid *StringGrid,bool hideLast);
	static void SortStringGrid(TStringGrid *GenStrGrid, int col);

	static void setRichTextType(RichFont type,TRichEdit *RichEdit);
};
#endif
