#ifndef CURSORADOH
#define CURSORADOH

#include "Cursor.h"
#include "ADODB_TLB.h"

namespace paulstdb
{

class CursorADO : public Cursor
{
public:
    CursorADO( Adodb_tlb::_Recordset* r );
    void close();
    bool endOfRecordSet() const;
    bool isNull( int col );
    void next();
    void read( int col, bool&        outVal );
    void read( int col, char&        outVal );
    void read( int col, float&       outVal );
    void read( int col, double&      outVal );
    void read( int col, int&         outVal );
    void read( int col, std::string& outVal );
    void read( int col, TDateTime&   outVal );
private:
    Adodb_tlb::RecordsetPtr m_recordSet;
    VARIANT getValue( int col );
};

};

#endif

