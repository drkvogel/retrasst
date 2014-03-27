#include "ComUtil.h"
#include "CursorADO.h"
#include "Require.h"

namespace paulstdb
{

CursorADO::CursorADO( Adodb_tlb::_Recordset* r )
    : m_recordSet( r )
{
}

CursorADO::~CursorADO()
{
    close();
}

void CursorADO::close()
{
    if ( m_recordSet )
    {
        m_recordSet->Close();
        m_recordSet.Reset();
    }
}


bool CursorADO::endOfRecordSet() const
{
    return VARIANT_TRUE == m_recordSet->get_EOF_();
}

VARIANT CursorADO::getValue( int col )
{
    Adodb_tlb::FieldsPtr fields = m_recordSet->get_Fields();

    Variant index(col);

    Adodb_tlb::FieldPtr field = fields->get_Item( index );

    return field->get_Value();
}

bool CursorADO::isNull( int col )
{
    Variant value( getValue(col) );

    return ( value.Type() == VT_NULL);
}

void CursorADO::next()
{
    require( ! endOfRecordSet() );
    throwUnlessOK( m_recordSet->MoveNext() );
}

void CursorADO::read( int col, bool& outVal )
{
    Variant value( getValue( col ) );

	int i = value;
	outVal = i != 0;
}

void CursorADO::read( int col, char& outVal )
{
    std::string s;

    read( col, s );

    outVal = s.size() ? s[0] : '\0';
}

void CursorADO::read( int col, float& outVal )
{
    Variant value( getValue( col ) );

    outVal = value;
}

void CursorADO::read( int col, double& outVal )
{
    Variant value( getValue( col ) );

    outVal = value;
}

void CursorADO::read( int col, int& outVal )
{
    Variant value( getValue( col ) );

    outVal = value;
}

void CursorADO::read( int col, std::string& outVal )
{
    Variant value( getValue( col ) );

    AnsiString s = value;

    outVal = s.c_str();
}

void CursorADO::read( int col, TDateTime& outVal )
{
    Variant value( getValue( col ) );

    outVal = (double)value;
}

}

