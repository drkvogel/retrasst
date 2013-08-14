//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include <cstdlib>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}

/* The function we'll call from the lua script */
static int getTestResult(lua_State *L)
{
	/* get number of arguments
	int n = lua_gettop(L);
    */
    int testID = lua_tonumber(L, 1);
    lua_pushnumber(L, testID + 1);
    return 1; // number of results
}

//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
    int isValid;
	lua_State* L;

	/* initialize Lua */
	L = lua_open();

	/* load Lua base libraries */
	luaL_openlibs(L);

    lua_register(L, "getTestResult", getTestResult);
	/* load the script */
	AnsiString scriptTextA( script->Text.c_str() );
	luaL_dostring(L, scriptTextA.c_str());
	
	/* the function name */
	lua_getglobal(L, "validateTestResult");

	/* the first argument */
	lua_pushnumber(L, testResult->Text.ToInt() );

	/* call the function with 1
	   argument, return 1 result */
	lua_call(L, 1, 1);

	/* get the result */
	isValid = (int)lua_toboolean(L, -1);
    result->Text = isValid ? "Valid" : "Not valid";
	lua_pop(L, 1);

	/* cleanup Lua */
	lua_close(L);

}
//---------------------------------------------------------------------------


