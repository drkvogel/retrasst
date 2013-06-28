//---------------------------------------------------------------------------

#error	This is an old version of xb36 - do not use

#ifndef xb36Code1H
#define xb36Code1H
#include <map>
typedef std::map<char,int> CharIntMap;
typedef std::map<int,char> IntCharMap;
//---------------------------------------------------------------------------
class xb36Code : public TObject
{
private:	// User declarations
    static CharIntMap mapEncode;
    static IntCharMap mapDecode;
public:		// User declarations
    xb36Code();
    virtual __fastcall ~xb36Code();
    static void fillMap();
    static unsigned __int64 xb36Encode( AnsiString alpha );
    static AnsiString xb36Decode( unsigned __int64 code, int len );
};
//---------------------------------------------------------------------------
#endif
 