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

// $Rev: 39763 $
// File generated on 07/03/2013 15:07:05 from Type Library described below.

// ************************************************************************  //
// Type Lib: C:/Program Files (x86)/Common Files/System/ado/msado60.tlb (1)
// LIBID: {B691E011-1797-432E-907A-4D8C69339129}
// LCID: 0
// Helpfile: C:\windows\HELP\ado270.chm
// HelpString: Microsoft ActiveX Data Objects 6.0 Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\SysWOW64\stdole2.tlb)
// Errors:
//   Hint: Symbol 'EOF' renamed to 'EOF_'
//   Hint: Symbol 'Update' renamed to '_Update'
//   Error creating palette bitmap of (TConnection) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TRecord) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TStream) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TCommand) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TRecordset) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
//   Error creating palette bitmap of (TParameter) : Server C:\Program Files (x86)\Common Files\System\ado\msado15.dll contains no icons
// Cmdline:
//   "C:\Program Files (x86)\Embarcadero\RAD Studio\9.0\bin\tlibimp.exe" -C "C:/Program Files (x86)/Common Files/System/ado/msado60.tlb"
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#pragma option -w-8122
#include "ADODB_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Adodb_tlb
{

_ConnectionPtr& TConnection::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TConnection::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TConnection::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TConnection::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TConnection::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TConnection::ConnectTo(_ConnectionPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TConnection::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Connection;
  sd.IntfIID = __uuidof(_Connection);
  sd.EventIID= __uuidof(ConnectionEvents);
  ServerData = &sd;
}

void __fastcall TConnection::InvokeEvent(int id, Vcl::Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 0: {
      if (OnInfoMessage) {
        (OnInfoMessage)(this, (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[0].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 1: {
      if (OnBeginTransComplete) {
        (OnBeginTransComplete)(this, params[0], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[1].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 3: {
      if (OnCommitTransComplete) {
        (OnCommitTransComplete)(this, (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[0].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 2: {
      if (OnRollbackTransComplete) {
        (OnRollbackTransComplete)(this, (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[0].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 4: {
      if (OnWillExecute) {
        (OnWillExecute)(this, params[0].pbstrVal, (Adodb_tlb::CursorTypeEnum*)(int*)(params[1].plVal), (Adodb_tlb::LockTypeEnum*)(int*)(params[2].plVal), params[3].plVal, (Adodb_tlb::EventStatusEnum*)(int*)(params[4].plVal), (Adodb_tlb::_CommandPtr)(LPDISPATCH)(params[5].pdispVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[6].pdispVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[7].pdispVal));
      }
      break;
      }
    case 5: {
      if (OnExecuteComplete) {
        (OnExecuteComplete)(this, params[0], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[1].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_CommandPtr)(LPDISPATCH)(params[3].pdispVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[4].pdispVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[5].pdispVal));
      }
      break;
      }
    case 6: {
      if (OnWillConnect) {
        (OnWillConnect)(this, params[0].pbstrVal, params[1].pbstrVal, params[2].pbstrVal, params[3].plVal, (Adodb_tlb::EventStatusEnum*)(int*)(params[4].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[5].pdispVal));
      }
      break;
      }
    case 7: {
      if (OnConnectComplete) {
        (OnConnectComplete)(this, (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[0].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 8: {
      if (OnDisconnect) {
        (OnDisconnect)(this, (Adodb_tlb::EventStatusEnum*)(int*)(params[0].plVal), (Adodb_tlb::_ConnectionPtr)(LPDISPATCH)(params[1].pdispVal));
      }
      break;
      }
    default:
      break;
  }
}

void __fastcall TConnection::Close(void)
{
  GetDefaultInterface()->Close();
}

Adodb_tlb::_Recordset* __fastcall TConnection::Execute(BSTR CommandText/*[in]*/, 
                                                       VARIANT* RecordsAffected/*[out,opt]*/, 
                                                       long Options/*[in,def,opt]*/)
{
  Adodb_tlb::_Recordset* ppiRset = 0;
  OLECHECK(GetDefaultInterface()->Execute(CommandText, RecordsAffected, Options, (Adodb_tlb::_Recordset**)&ppiRset));
  return ppiRset;
}

long __fastcall TConnection::BeginTrans(void)
{
  long TransactionLevel;
  OLECHECK(GetDefaultInterface()->BeginTrans((long*)&TransactionLevel));
  return TransactionLevel;
}

void __fastcall TConnection::CommitTrans(void)
{
  GetDefaultInterface()->CommitTrans();
}

void __fastcall TConnection::RollbackTrans(void)
{
  GetDefaultInterface()->RollbackTrans();
}

void __fastcall TConnection::Open(BSTR ConnectionString/*[in,def,opt]*/, BSTR UserID/*[in,def,opt]*/, 
                                  BSTR Password/*[in,def,opt]*/, long Options/*[in,def,opt]*/)
{
  GetDefaultInterface()->Open(ConnectionString/*[in,def,opt]*/, UserID/*[in,def,opt]*/, 
                              Password/*[in,def,opt]*/, Options/*[in,def,opt]*/);
}

Adodb_tlb::_Recordset* __fastcall TConnection::OpenSchema(Adodb_tlb::SchemaEnum Schema/*[in]*/, 
                                                          VARIANT Restrictions/*[in,opt]*/, 
                                                          VARIANT SchemaID/*[in,opt]*/)
{
  Adodb_tlb::_Recordset* pprset = 0;
  OLECHECK(GetDefaultInterface()->OpenSchema(Schema, Restrictions, SchemaID, (Adodb_tlb::_Recordset**)&pprset));
  return pprset;
}

void __fastcall TConnection::Cancel(void)
{
  GetDefaultInterface()->Cancel();
}

BSTR __fastcall TConnection::get_ConnectionString(void)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->get_ConnectionString((BSTR*)&pbstr));
  return pbstr;
}

void __fastcall TConnection::set_ConnectionString(BSTR pbstr/*[in]*/)
{
  GetDefaultInterface()->set_ConnectionString(pbstr/*[in]*/);
}

long __fastcall TConnection::get_CommandTimeout(void)
{
  long plTimeout;
  OLECHECK(GetDefaultInterface()->get_CommandTimeout((long*)&plTimeout));
  return plTimeout;
}

void __fastcall TConnection::set_CommandTimeout(long plTimeout/*[in]*/)
{
  GetDefaultInterface()->set_CommandTimeout(plTimeout/*[in]*/);
}

long __fastcall TConnection::get_ConnectionTimeout(void)
{
  long plTimeout;
  OLECHECK(GetDefaultInterface()->get_ConnectionTimeout((long*)&plTimeout));
  return plTimeout;
}

void __fastcall TConnection::set_ConnectionTimeout(long plTimeout/*[in]*/)
{
  GetDefaultInterface()->set_ConnectionTimeout(plTimeout/*[in]*/);
}

BSTR __fastcall TConnection::get_Version(void)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->get_Version((BSTR*)&pbstr));
  return pbstr;
}

Adodb_tlb::ErrorsPtr __fastcall TConnection::get_Errors(void)
{
  Adodb_tlb::ErrorsPtr ppvObject;
  OLECHECK(GetDefaultInterface()->get_Errors(&ppvObject));
  return ppvObject;
}

BSTR __fastcall TConnection::get_DefaultDatabase(void)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->get_DefaultDatabase((BSTR*)&pbstr));
  return pbstr;
}

void __fastcall TConnection::set_DefaultDatabase(BSTR pbstr/*[in]*/)
{
  GetDefaultInterface()->set_DefaultDatabase(pbstr/*[in]*/);
}

Adodb_tlb::IsolationLevelEnum __fastcall TConnection::get_IsolationLevel(void)
{
  Adodb_tlb::IsolationLevelEnum Level;
  OLECHECK(GetDefaultInterface()->get_IsolationLevel((Adodb_tlb::IsolationLevelEnum*)&Level));
  return Level;
}

void __fastcall TConnection::set_IsolationLevel(Adodb_tlb::IsolationLevelEnum Level/*[in]*/)
{
  GetDefaultInterface()->set_IsolationLevel(Level/*[in]*/);
}

long __fastcall TConnection::get_Attributes(void)
{
  long plAttr;
  OLECHECK(GetDefaultInterface()->get_Attributes((long*)&plAttr));
  return plAttr;
}

void __fastcall TConnection::set_Attributes(long plAttr/*[in]*/)
{
  GetDefaultInterface()->set_Attributes(plAttr/*[in]*/);
}

Adodb_tlb::CursorLocationEnum __fastcall TConnection::get_CursorLocation(void)
{
  Adodb_tlb::CursorLocationEnum plCursorLoc;
  OLECHECK(GetDefaultInterface()->get_CursorLocation((Adodb_tlb::CursorLocationEnum*)&plCursorLoc));
  return plCursorLoc;
}

void __fastcall TConnection::set_CursorLocation(Adodb_tlb::CursorLocationEnum plCursorLoc/*[in]*/)
{
  GetDefaultInterface()->set_CursorLocation(plCursorLoc/*[in]*/);
}

Adodb_tlb::ConnectModeEnum __fastcall TConnection::get_Mode(void)
{
  Adodb_tlb::ConnectModeEnum plMode;
  OLECHECK(GetDefaultInterface()->get_Mode((Adodb_tlb::ConnectModeEnum*)&plMode));
  return plMode;
}

void __fastcall TConnection::set_Mode(Adodb_tlb::ConnectModeEnum plMode/*[in]*/)
{
  GetDefaultInterface()->set_Mode(plMode/*[in]*/);
}

BSTR __fastcall TConnection::get_Provider(void)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->get_Provider((BSTR*)&pbstr));
  return pbstr;
}

void __fastcall TConnection::set_Provider(BSTR pbstr/*[in]*/)
{
  GetDefaultInterface()->set_Provider(pbstr/*[in]*/);
}

long __fastcall TConnection::get_State(void)
{
  long plObjState;
  OLECHECK(GetDefaultInterface()->get_State((long*)&plObjState));
  return plObjState;
}

_RecordPtr& TRecord::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TRecord::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TRecord::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TRecord::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TRecord::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TRecord::ConnectTo(_RecordPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TRecord::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Record;
  sd.IntfIID = __uuidof(_Record);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

BSTR __fastcall TRecord::MoveRecord(BSTR Source/*[in,def,opt]*/, BSTR Destination/*[in,def,opt]*/, 
                                    BSTR UserName/*[in,def,opt]*/, BSTR Password/*[in,def,opt]*/, 
                                    Adodb_tlb::MoveRecordOptionsEnum Options/*[in,def,opt]*/, 
                                    VARIANT_BOOL Async/*[in,def,opt]*/)
{
  BSTR pbstrNewURL = 0;
  OLECHECK(GetDefaultInterface()->MoveRecord(Source, Destination, UserName, Password, Options, Async, (BSTR*)&pbstrNewURL));
  return pbstrNewURL;
}

BSTR __fastcall TRecord::CopyRecord(BSTR Source/*[in,def,opt]*/, BSTR Destination/*[in,def,opt]*/, 
                                    BSTR UserName/*[in,def,opt]*/, BSTR Password/*[in,def,opt]*/, 
                                    Adodb_tlb::CopyRecordOptionsEnum Options/*[in,def,opt]*/, 
                                    VARIANT_BOOL Async/*[in,def,opt]*/)
{
  BSTR pbstrNewURL = 0;
  OLECHECK(GetDefaultInterface()->CopyRecord(Source, Destination, UserName, Password, Options, Async, (BSTR*)&pbstrNewURL));
  return pbstrNewURL;
}

void __fastcall TRecord::DeleteRecord(BSTR Source/*[in,def,opt]*/, 
                                      VARIANT_BOOL Async/*[in,def,opt]*/)
{
  GetDefaultInterface()->DeleteRecord(Source/*[in,def,opt]*/, Async/*[in,def,opt]*/);
}

void __fastcall TRecord::Open(VARIANT Source/*[in,opt]*/, VARIANT ActiveConnection/*[in,opt]*/, 
                              Adodb_tlb::ConnectModeEnum Mode/*[in,def,opt]*/, 
                              Adodb_tlb::RecordCreateOptionsEnum CreateOptions/*[in,def,opt]*/, 
                              Adodb_tlb::RecordOpenOptionsEnum Options/*[in,def,opt]*/, 
                              BSTR UserName/*[in,def,opt]*/, BSTR Password/*[in,def,opt]*/)
{
  GetDefaultInterface()->Open(Source/*[in,opt]*/, ActiveConnection/*[in,opt]*/, Mode/*[in,def,opt]*/, 
                              CreateOptions/*[in,def,opt]*/, Options/*[in,def,opt]*/, 
                              UserName/*[in,def,opt]*/, Password/*[in,def,opt]*/);
}

void __fastcall TRecord::Close(void)
{
  GetDefaultInterface()->Close();
}

Adodb_tlb::_Recordset* __fastcall TRecord::GetChildren(void)
{
  Adodb_tlb::_Recordset* pprset = 0;
  OLECHECK(GetDefaultInterface()->GetChildren((Adodb_tlb::_Recordset**)&pprset));
  return pprset;
}

void __fastcall TRecord::Cancel(void)
{
  GetDefaultInterface()->Cancel();
}

VARIANT __fastcall TRecord::get_ActiveConnection(void)
{
  VARIANT pvar;
  OLECHECK(GetDefaultInterface()->get_ActiveConnection((VARIANT*)&pvar));
  return pvar;
}

void __fastcall TRecord::set_ActiveConnection(BSTR pvar/*[in]*/)
{
  GetDefaultInterface()->set_ActiveConnection(pvar/*[in]*/);
}

void __fastcall TRecord::_set_ActiveConnection(Adodb_tlb::_Connection* pvar/*[in]*/)
{
  GetDefaultInterface()->_set_ActiveConnection(pvar/*[in]*/);
}

Adodb_tlb::ObjectStateEnum __fastcall TRecord::get_State(void)
{
  Adodb_tlb::ObjectStateEnum pState;
  OLECHECK(GetDefaultInterface()->get_State((Adodb_tlb::ObjectStateEnum*)&pState));
  return pState;
}

VARIANT __fastcall TRecord::get_Source(void)
{
  VARIANT pvar;
  OLECHECK(GetDefaultInterface()->get_Source((VARIANT*)&pvar));
  return pvar;
}

void __fastcall TRecord::set_Source(BSTR pvar/*[in]*/)
{
  GetDefaultInterface()->set_Source(pvar/*[in]*/);
}

void __fastcall TRecord::_set_Source(LPDISPATCH pvar/*[in]*/)
{
  GetDefaultInterface()->_set_Source(pvar/*[in]*/);
}

Adodb_tlb::ConnectModeEnum __fastcall TRecord::get_Mode(void)
{
  Adodb_tlb::ConnectModeEnum pMode;
  OLECHECK(GetDefaultInterface()->get_Mode((Adodb_tlb::ConnectModeEnum*)&pMode));
  return pMode;
}

void __fastcall TRecord::set_Mode(Adodb_tlb::ConnectModeEnum pMode/*[in]*/)
{
  GetDefaultInterface()->set_Mode(pMode/*[in]*/);
}

BSTR __fastcall TRecord::get_ParentURL(void)
{
  BSTR pbstrParentURL = 0;
  OLECHECK(GetDefaultInterface()->get_ParentURL((BSTR*)&pbstrParentURL));
  return pbstrParentURL;
}

Adodb_tlb::FieldsPtr __fastcall TRecord::get_Fields(void)
{
  Adodb_tlb::FieldsPtr ppFlds;
  OLECHECK(GetDefaultInterface()->get_Fields(&ppFlds));
  return ppFlds;
}

Adodb_tlb::RecordTypeEnum __fastcall TRecord::get_RecordType(void)
{
  Adodb_tlb::RecordTypeEnum ptype;
  OLECHECK(GetDefaultInterface()->get_RecordType((Adodb_tlb::RecordTypeEnum*)&ptype));
  return ptype;
}

_StreamPtr& TStream::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TStream::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TStream::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TStream::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TStream::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TStream::ConnectTo(_StreamPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TStream::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Stream;
  sd.IntfIID = __uuidof(_Stream);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

VARIANT __fastcall TStream::Read(long NumBytes/*[in,def,opt]*/)
{
  VARIANT pval;
  OLECHECK(GetDefaultInterface()->Read(NumBytes, (VARIANT*)&pval));
  return pval;
}

void __fastcall TStream::Open(VARIANT Source/*[in,opt]*/, 
                              Adodb_tlb::ConnectModeEnum Mode/*[in,def,opt]*/, 
                              Adodb_tlb::StreamOpenOptionsEnum Options/*[in,def,opt]*/, 
                              BSTR UserName/*[in,def,opt]*/, BSTR Password/*[in,def,opt]*/)
{
  GetDefaultInterface()->Open(Source/*[in,opt]*/, Mode/*[in,def,opt]*/, Options/*[in,def,opt]*/, 
                              UserName/*[in,def,opt]*/, Password/*[in,def,opt]*/);
}

void __fastcall TStream::Close(void)
{
  GetDefaultInterface()->Close();
}

void __fastcall TStream::SkipLine(void)
{
  GetDefaultInterface()->SkipLine();
}

void __fastcall TStream::Write(VARIANT Buffer/*[in]*/)
{
  GetDefaultInterface()->Write(Buffer/*[in]*/);
}

void __fastcall TStream::SetEOS(void)
{
  GetDefaultInterface()->SetEOS();
}

void __fastcall TStream::CopyTo(Adodb_tlb::_Stream* DestStream/*[in]*/, 
                                Adodb_tlb::ADO_LONGPTR CharNumber/*[in,def,opt]*/)
{
  GetDefaultInterface()->CopyTo(DestStream/*[in]*/, CharNumber/*[in,def,opt]*/);
}

void __fastcall TStream::Flush(void)
{
  GetDefaultInterface()->Flush();
}

void __fastcall TStream::SaveToFile(BSTR FileName/*[in]*/, 
                                    Adodb_tlb::SaveOptionsEnum Options/*[in,def,opt]*/)
{
  GetDefaultInterface()->SaveToFile(FileName/*[in]*/, Options/*[in,def,opt]*/);
}

void __fastcall TStream::LoadFromFile(BSTR FileName/*[in]*/)
{
  GetDefaultInterface()->LoadFromFile(FileName/*[in]*/);
}

BSTR __fastcall TStream::ReadText(long NumChars/*[in,def,opt]*/)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->ReadText(NumChars, (BSTR*)&pbstr));
  return pbstr;
}

void __fastcall TStream::WriteText(BSTR Data/*[in]*/, 
                                   Adodb_tlb::StreamWriteEnum Options/*[in,def,opt]*/)
{
  GetDefaultInterface()->WriteText(Data/*[in]*/, Options/*[in,def,opt]*/);
}

void __fastcall TStream::Cancel(void)
{
  GetDefaultInterface()->Cancel();
}

Adodb_tlb::ADO_LONGPTR __fastcall TStream::get_Size(void)
{
  Adodb_tlb::ADO_LONGPTR pSize;
  OLECHECK(GetDefaultInterface()->get_Size((Adodb_tlb::ADO_LONGPTR*)&pSize));
  return pSize;
}

VARIANT_BOOL __fastcall TStream::get_EOS(void)
{
  VARIANT_BOOL pEOS;
  OLECHECK(GetDefaultInterface()->get_EOS((VARIANT_BOOL*)&pEOS));
  return pEOS;
}

Adodb_tlb::ADO_LONGPTR __fastcall TStream::get_Position(void)
{
  Adodb_tlb::ADO_LONGPTR pPos;
  OLECHECK(GetDefaultInterface()->get_Position((Adodb_tlb::ADO_LONGPTR*)&pPos));
  return pPos;
}

void __fastcall TStream::set_Position(Adodb_tlb::ADO_LONGPTR pPos/*[in]*/)
{
  GetDefaultInterface()->set_Position(pPos/*[in]*/);
}

Adodb_tlb::StreamTypeEnum __fastcall TStream::get_Type(void)
{
  Adodb_tlb::StreamTypeEnum ptype;
  OLECHECK(GetDefaultInterface()->get_Type((Adodb_tlb::StreamTypeEnum*)&ptype));
  return ptype;
}

void __fastcall TStream::set_Type(Adodb_tlb::StreamTypeEnum ptype/*[in]*/)
{
  GetDefaultInterface()->set_Type(ptype/*[in]*/);
}

Adodb_tlb::LineSeparatorEnum __fastcall TStream::get_LineSeparator(void)
{
  Adodb_tlb::LineSeparatorEnum pLS;
  OLECHECK(GetDefaultInterface()->get_LineSeparator((Adodb_tlb::LineSeparatorEnum*)&pLS));
  return pLS;
}

void __fastcall TStream::set_LineSeparator(Adodb_tlb::LineSeparatorEnum pLS/*[in]*/)
{
  GetDefaultInterface()->set_LineSeparator(pLS/*[in]*/);
}

Adodb_tlb::ObjectStateEnum __fastcall TStream::get_State(void)
{
  Adodb_tlb::ObjectStateEnum pState;
  OLECHECK(GetDefaultInterface()->get_State((Adodb_tlb::ObjectStateEnum*)&pState));
  return pState;
}

Adodb_tlb::ConnectModeEnum __fastcall TStream::get_Mode(void)
{
  Adodb_tlb::ConnectModeEnum pMode;
  OLECHECK(GetDefaultInterface()->get_Mode((Adodb_tlb::ConnectModeEnum*)&pMode));
  return pMode;
}

void __fastcall TStream::set_Mode(Adodb_tlb::ConnectModeEnum pMode/*[in]*/)
{
  GetDefaultInterface()->set_Mode(pMode/*[in]*/);
}

BSTR __fastcall TStream::get_Charset(void)
{
  BSTR pbstrCharset = 0;
  OLECHECK(GetDefaultInterface()->get_Charset((BSTR*)&pbstrCharset));
  return pbstrCharset;
}

void __fastcall TStream::set_Charset(BSTR pbstrCharset/*[in]*/)
{
  GetDefaultInterface()->set_Charset(pbstrCharset/*[in]*/);
}

_CommandPtr& TCommand::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TCommand::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TCommand::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TCommand::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TCommand::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TCommand::ConnectTo(_CommandPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TCommand::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Command;
  sd.IntfIID = __uuidof(_Command);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

void __fastcall TCommand::_set_CommandStream(LPUNKNOWN pvStream/*[in]*/)
{
  GetDefaultInterface()->_set_CommandStream(pvStream/*[in]*/);
}

VARIANT __fastcall TCommand::get_CommandStream(void)
{
  VARIANT pvStream;
  OLECHECK(GetDefaultInterface()->get_CommandStream((VARIANT*)&pvStream));
  return pvStream;
}

void __fastcall TCommand::set_Dialect(BSTR pbstrDialect/*[in]*/)
{
  GetDefaultInterface()->set_Dialect(pbstrDialect/*[in]*/);
}

BSTR __fastcall TCommand::get_Dialect(void)
{
  BSTR pbstrDialect = 0;
  OLECHECK(GetDefaultInterface()->get_Dialect((BSTR*)&pbstrDialect));
  return pbstrDialect;
}

void __fastcall TCommand::set_NamedParameters(VARIANT_BOOL pfNamedParameters/*[in]*/)
{
  GetDefaultInterface()->set_NamedParameters(pfNamedParameters/*[in]*/);
}

VARIANT_BOOL __fastcall TCommand::get_NamedParameters(void)
{
  VARIANT_BOOL pfNamedParameters;
  OLECHECK(GetDefaultInterface()->get_NamedParameters((VARIANT_BOOL*)&pfNamedParameters));
  return pfNamedParameters;
}

_RecordsetPtr& TRecordset::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TRecordset::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TRecordset::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TRecordset::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TRecordset::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TRecordset::ConnectTo(_RecordsetPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TRecordset::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Recordset;
  sd.IntfIID = __uuidof(_Recordset);
  sd.EventIID= __uuidof(RecordsetEvents);
  ServerData = &sd;
}

void __fastcall TRecordset::InvokeEvent(int id, Vcl::Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 9: {
      if (OnWillChangeField) {
        (OnWillChangeField)(this, params[0], params[1], (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 10: {
      if (OnFieldChangeComplete) {
        (OnFieldChangeComplete)(this, params[0], params[1], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[2].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[3].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[4].pdispVal));
      }
      break;
      }
    case 11: {
      if (OnWillChangeRecord) {
        (OnWillChangeRecord)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], params[1], (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 12: {
      if (OnRecordChangeComplete) {
        (OnRecordChangeComplete)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], params[1], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[2].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[3].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[4].pdispVal));
      }
      break;
      }
    case 13: {
      if (OnWillChangeRecordset) {
        (OnWillChangeRecordset)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 14: {
      if (OnRecordsetChangeComplete) {
        (OnRecordsetChangeComplete)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[1].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 15: {
      if (OnWillMove) {
        (OnWillMove)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 16: {
      if (OnMoveComplete) {
        (OnMoveComplete)(this, (Adodb_tlb::EventReasonEnum)(int)params[0], (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[1].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 17: {
      if (OnEndOfRecordset) {
        (OnEndOfRecordset)(this, params[0].pboolVal, (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    case 18: {
      if (OnFetchProgress) {
        (OnFetchProgress)(this, params[0], params[1], (Adodb_tlb::EventStatusEnum*)(int*)(params[2].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[3].pdispVal));
      }
      break;
      }
    case 19: {
      if (OnFetchComplete) {
        (OnFetchComplete)(this, (Adodb_tlb::ErrorPtr)(LPDISPATCH)(params[0].pdispVal), (Adodb_tlb::EventStatusEnum*)(int*)(params[1].plVal), (Adodb_tlb::_RecordsetPtr)(LPDISPATCH)(params[2].pdispVal));
      }
      break;
      }
    default:
      break;
  }
}

void __fastcall TRecordset::Save(VARIANT Destination/*[in,opt]*/, 
                                 Adodb_tlb::PersistFormatEnum PersistFormat/*[in,def,opt]*/)
{
  GetDefaultInterface()->Save(Destination/*[in,opt]*/, PersistFormat/*[in,def,opt]*/);
}

_ParameterPtr& TParameter::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TParameter::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TParameter::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TParameter::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TParameter::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TParameter::ConnectTo(_ParameterPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TParameter::InitServerData()
{
  static Vcl::Oleserver::TServerData sd;
  sd.ClassID = CLSID_Parameter;
  sd.IntfIID = __uuidof(_Parameter);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

void __fastcall TParameter::AppendChunk(VARIANT Val/*[in]*/)
{
  GetDefaultInterface()->AppendChunk(Val/*[in]*/);
}

BSTR __fastcall TParameter::get_Name(void)
{
  BSTR pbstr = 0;
  OLECHECK(GetDefaultInterface()->get_Name((BSTR*)&pbstr));
  return pbstr;
}

void __fastcall TParameter::set_Name(BSTR pbstr/*[in]*/)
{
  GetDefaultInterface()->set_Name(pbstr/*[in]*/);
}

VARIANT __fastcall TParameter::get_Value(void)
{
  VARIANT pvar;
  OLECHECK(GetDefaultInterface()->get_Value((VARIANT*)&pvar));
  return pvar;
}

void __fastcall TParameter::set_Value(VARIANT pvar/*[in]*/)
{
  GetDefaultInterface()->set_Value(pvar/*[in]*/);
}

Adodb_tlb::DataTypeEnum __fastcall TParameter::get_Type(void)
{
  Adodb_tlb::DataTypeEnum psDataType;
  OLECHECK(GetDefaultInterface()->get_Type((Adodb_tlb::DataTypeEnum*)&psDataType));
  return psDataType;
}

void __fastcall TParameter::set_Type(Adodb_tlb::DataTypeEnum psDataType/*[in]*/)
{
  GetDefaultInterface()->set_Type(psDataType/*[in]*/);
}

void __fastcall TParameter::set_Direction(Adodb_tlb::ParameterDirectionEnum plParmDirection/*[in]*/)
{
  GetDefaultInterface()->set_Direction(plParmDirection/*[in]*/);
}

Adodb_tlb::ParameterDirectionEnum __fastcall TParameter::get_Direction(void)
{
  Adodb_tlb::ParameterDirectionEnum plParmDirection;
  OLECHECK(GetDefaultInterface()->get_Direction((Adodb_tlb::ParameterDirectionEnum*)&plParmDirection));
  return plParmDirection;
}

void __fastcall TParameter::set_Precision(unsigned_char pbPrecision/*[in]*/)
{
  GetDefaultInterface()->set_Precision(pbPrecision/*[in]*/);
}

unsigned_char __fastcall TParameter::get_Precision(void)
{
  unsigned_char pbPrecision;
  OLECHECK(GetDefaultInterface()->get_Precision((unsigned_char*)&pbPrecision));
  return pbPrecision;
}

void __fastcall TParameter::set_NumericScale(unsigned_char pbScale/*[in]*/)
{
  GetDefaultInterface()->set_NumericScale(pbScale/*[in]*/);
}

unsigned_char __fastcall TParameter::get_NumericScale(void)
{
  unsigned_char pbScale;
  OLECHECK(GetDefaultInterface()->get_NumericScale((unsigned_char*)&pbScale));
  return pbScale;
}

void __fastcall TParameter::set_Size(Adodb_tlb::ADO_LONGPTR pl/*[in]*/)
{
  GetDefaultInterface()->set_Size(pl/*[in]*/);
}

Adodb_tlb::ADO_LONGPTR __fastcall TParameter::get_Size(void)
{
  Adodb_tlb::ADO_LONGPTR pl;
  OLECHECK(GetDefaultInterface()->get_Size((Adodb_tlb::ADO_LONGPTR*)&pl));
  return pl;
}

long __fastcall TParameter::get_Attributes(void)
{
  long plParmAttribs;
  OLECHECK(GetDefaultInterface()->get_Attributes((long*)&plParmAttribs));
  return plParmAttribs;
}

void __fastcall TParameter::set_Attributes(long plParmAttribs/*[in]*/)
{
  GetDefaultInterface()->set_Attributes(plParmAttribs/*[in]*/);
}


};     // namespace Adodb_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Adodb_ocx
{

void __fastcall PACKAGE Register()
{
  // [6]
  System::Classes::TComponentClass cls_svr[] = {
                              __classid(Adodb_tlb::TConnection), 
                              __classid(Adodb_tlb::TRecord), 
                              __classid(Adodb_tlb::TStream), 
                              __classid(Adodb_tlb::TCommand), 
                              __classid(Adodb_tlb::TRecordset), 
                              __classid(Adodb_tlb::TParameter)
                           };
  System::Classes::RegisterComponents("Servers", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Adodb_ocx
