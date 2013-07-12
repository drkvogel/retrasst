#include "DBConnectionADO.h"
#include "ComUtil.h"
#include "CursorADO.h"


namespace valc
{

DBConnectionADO::DBConnectionADO( const Properties& p )
    : m_open(false)
{
    m_con = CoConnection::Create();
    WideString connectionString( p.getProperty( "ConnectionString" ).c_str() );// = L"dsn=paulst_test_on_brat;db=paulst_test";
    WideString username, password;
    throwUnlessOK( m_con->Open( connectionString.c_bstr(), username.c_bstr(), password.c_bstr(), 0 ) );
    m_open = true;

    WideString sessionReadLockSetting( p.getProperty( "SessionReadLockSetting" ).c_str() );

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

Cursor* DBConnectionADO::executeQuery( const std::string& sql )
{
    WideString sqlWide(sql.c_str());
    Adodb_tlb::_Recordset* recordSet = m_con->Execute( sqlWide.c_bstr(), NULL, 0 );
    return new CursorADO( recordSet );
}

void DBConnectionADO::executeStmt( const std::string& sql )
{
    WideString sqlWide(sql.c_str());
	m_con->Execute( sqlWide.c_bstr(), NULL,
		static_cast<long>(Adodb_tlb::ExecuteOptionEnum::adExecuteNoRecords) );
}

}

