#ifndef STRINGBACKEDCURSORH
#define STRINGBACKEDCURSORH

#include "API.h"
#include "CSVIterator.h"
#include <vector>

namespace valc
{

class StringBackedCursor : public Cursor
{
public:
    StringBackedCursor( const std::string& s = "" );
    void close();
    bool isNull( int col );
    bool endOfRecordSet() const;
    void next();
    void read( int col, bool&        outVal );
    void read( int col, char&        outVal );
    void read( int col, float&       outVal );
    void read( int col, int&         outVal );
    void read( int col, std::string& outVal );
    void read( int col, TDateTime&   outVal );
private:
    paulst::CSVIterator<'\n'> m_currentRecord;
    paulst::CSVIterator<'\n'> m_endOfRecords;
    std::vector< std::string > m_rowValues;

    std::string colValue(int col) const;
    void parseCurrentRow();
};

};

#endif

