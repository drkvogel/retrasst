// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// $Rev: 52393 $
// File generated on 16/09/2013 09:13:27 from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Program Files (x86)\Common Files\System\ado\msado15.dll (1)
// LIBID: {B691E011-1797-432E-907A-4D8C69339129}
// LCID: 0
// Helpfile: C:\windows\HELP\ado270.chm 
// HelpString: Microsoft ActiveX Data Objects 6.1 Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\system32\stdole2.tlb)
// SYS_KIND: SYS_WIN32
// Errors:
//   Hint: Symbol 'EOF' renamed to 'EOF_'
//   Hint: Symbol 'Update' renamed to '_Update'
//   Error creating palette bitmap of (TConnection) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TRecord) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TStream) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TCommand) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TRecordset) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TParameter) : Server C:\Program Files\Common Files\System\ado\msado15.dll contains no icons
// Cmdline:
//   "C:\Program Files (x86)\Embarcadero\RAD Studio\11.0\bin64\tlibimp"  -C -D"J:\cvs\paulst\db" "C:\Program Files (x86)\Common Files\System\ado\msado15.dll"
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include "ADODB_TLB.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Adodb_tlb
{


// *********************************************************************//
// GUIDS declared in the TypeLibrary                                      
// *********************************************************************//
const GUID LIBID_ADODB = {0xB691E011, 0x1797, 0x432E,{ 0x90, 0x7A, 0x4D,0x8C, 0x69, 0x33,0x91, 0x29} };
const GUID IID__Collection = {0x00000512, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__DynaCollection = {0x00000513, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__ADO = {0x00000534, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Properties = {0x00000504, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Property = {0x00000503, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Error = {0x00000500, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Errors = {0x00000501, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Command15 = {0x00001508, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Connection = {0x00001550, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Connection15 = {0x00001515, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Recordset = {0x00001556, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset21 = {0x00001555, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset20 = {0x0000154F, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset15 = {0x0000150E, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields = {0x00001564, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields20 = {0x0000154D, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields15 = {0x00001506, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field = {0x00001569, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field20 = {0x0000154C, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Parameter = {0x0000150C, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Parameters = {0x0000150D, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Command25 = {0x0000154E, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Command = {0x986761E8, 0x7269, 0x4890,{ 0xAA, 0x65, 0xAD,0x7C, 0x03, 0x69,0x7A, 0x6D} };
const GUID IID_ConnectionEventsVt = {0x00001402, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_RecordsetEventsVt = {0x00001403, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID DIID_ConnectionEvents = {0x00001400, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID DIID_RecordsetEvents = {0x00001266, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADOConnectionConstruction15 = {0x00000516, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADOConnectionConstruction = {0x00000551, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Connection = {0x00000514, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Record = {0x00001562, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Record = {0x00000560, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Stream = {0x00001565, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Stream = {0x00000566, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADORecordConstruction = {0x00000567, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADOStreamConstruction = {0x00000568, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADOCommandConstruction = {0x00000517, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Command = {0x00000507, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Recordset = {0x00000535, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ADORecordsetConstruction = {0x00000283, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field15 = {0x00001505, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID CLSID_Parameter = {0x0000050B, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_ConnectionEventsVt_Deprecated = {0x00000402, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Connection_Deprecated = {0x00000550, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Connection15_Deprecated = {0x00000515, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Recordset_Deprecated = {0x00000556, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset21_Deprecated = {0x00000555, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset20_Deprecated = {0x0000054F, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Recordset15_Deprecated = {0x0000050E, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields_Deprecated = {0x00000564, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields20_Deprecated = {0x0000054D, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Fields15_Deprecated = {0x00000506, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field_Deprecated = {0x00000569, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field20_Deprecated = {0x0000054C, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Command_Deprecated = {0xB08400BD, 0xF9D1, 0x4D02,{ 0xB8, 0x56, 0x71,0xD5, 0xDB, 0xA1,0x23, 0xE9} };
const GUID IID_Command25_Deprecated = {0x0000054E, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Command15_Deprecated = {0x00000508, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Parameter_Deprecated = {0x0000050C, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Parameters_Deprecated = {0x0000050D, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_RecordsetEventsVt_Deprecated = {0x00000403, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID DIID_ConnectionEvents_Deprecated = {0x00000400, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID DIID_RecordsetEvents_Deprecated = {0x00000266, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Record_Deprecated = {0x00000562, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID__Stream_Deprecated = {0x00000565, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };
const GUID IID_Field15_Deprecated = {0x00000505, 0x0000, 0x0010,{ 0x80, 0x00, 0x00,0xAA, 0x00, 0x6D,0x2E, 0xA4} };

};     // namespace Adodb_tlb
