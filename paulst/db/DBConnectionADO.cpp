#include "AcquireCriticalSection.h"
#include "DBConnectionADO.h"
#include "ComUtil.h"
#include "CursorADO.h"
#include "Require.h"


namespace paulstdb
{

DBConnectionADO::DBConnectionADO( const std::string& conString, const std::string& sessionReadLockSettingStr )
	: m_open(false)
{
	m_con = CoConnection::Create();
	require( m_con );
	WideString connectionString( conString.c_str() );// = L"dsn=paulst_test_on_brat;db=paulst_test";
	WideString username, password;
	HRESULT hr = m_con->Open( connectionString.c_bstr(), username.c_bstr(), password.c_bstr(), 0 );
	if ( ! SUCCEEDED( hr )   )
	{
		UnicodeString info( L"Failed to open a connection using '" );
		info += connectionString.c_bstr();
		info += L"'";
		reportErrors( info );
	}

    m_open = true;

    WideString sessionReadLockSetting( sessionReadLockSettingStr.c_str() );

    if ( ! sessionReadLockSetting.IsEmpty() )
    {
        try
        {
            m_con->Execute( sessionReadLockSetting.c_bstr(), NULL,
				static_cast<long>(Adodb_tlb::ExecuteOptionEnum::adExecuteNoRecords) );
        }
        catch( ... )
        {
            close();
            throw;
        }
    }
}

DBConnectionADO::~DBConnectionADO()
{
    close();
}

void DBConnectionADO::close()
{
    if ( m_open )
    {
        m_open = false;
        m_con->Close();
    }
}

void DBConnectionADO::reportErrors( const UnicodeString& supplementaryInfo )
{
	Adodb_tlb::ErrorsPtr errors = m_con->get_Errors();

	int numErrors = errors->get_Count();

	if ( numErrors )
	{
		Adodb_tlb::Error* error = NULL;

		UnicodeString messageBuffer;

		for ( int i = 0; i < numErrors; ++i )
		{
			BSTR message;
			error = errors->get_Item( Variant(i) );
			error->get_Description(&message);
			messageBuffer += message;
			messageBuffer += L"\n";
			SysFreeString( message );
			error->Release();
		}

		messageBuffer += supplementaryInfo;

		throw Exception( messageBuffer );
	}
}

Cursor* DBConnectionADO::executeQuery( const std::string& sql )
{
	paulst::AcquireCriticalSection a(m_critSec);

	{
		WideString sqlWide(sql.c_str());
		Adodb_tlb::_Recordset* recordSet = m_con->Execute( sqlWide.c_bstr(), NULL, 0 );
		reportErrors( sqlWide.c_bstr());
        return new CursorADO( recordSet );
    }
}

void DBConnectionADO::executeStmt( const std::string& sql )
{
    paulst::AcquireCriticalSection a(m_critSec);

    {
        WideString sqlWide(sql.c_str());
        m_con->Execute( sqlWide.c_bstr(), NULL,
            static_cast<long>(Adodb_tlb::ExecuteOptionEnum::adExecuteNoRecords) );
    }
}

}

