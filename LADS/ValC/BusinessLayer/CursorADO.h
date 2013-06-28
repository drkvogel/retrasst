#ifndef CURSORADOH
#define CURSORADOH


#include "API.h"
#include "ADODB_TLB.h"

namespace valc
{

class CursorADO : public Cursor
{
public:
    CursorADO( Adodb_tlb::_Recordset* r );
    ~CursorADO();
    void close();
    bool endOfRecordSet() const;
    bool isNull( int col );
    void next();
    void read( int col, bool&        outVal );
    void read( int col, char&        outVal );
    void read( int col, float&       outVal );
    void read( int col, int&         outVal );
    void read( int col, std::string& outVal );
    void read( int col, TDateTime&   outVal );
private:
    Adodb_tlb::RecordsetPtr m_recordSet;
    VARIANT getValue( int col );
};

};

#endif

