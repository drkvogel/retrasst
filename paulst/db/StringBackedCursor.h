#ifndef STRINGBACKEDCURSORH
#define STRINGBACKEDCURSORH

#include "Cursor.h"
#include "CSVIterator.h"
#include <vector>

namespace paulstdb
{

typedef std::vector< std::string > RowValues;

typedef void (*FieldParsingStrategy)( const std::string& record, RowValues& out );

void parseCSV( const std::string& record, RowValues& out );


class StringBackedCursor : public Cursor
{
public:
    StringBackedCursor( const std::string& s, FieldParsingStrategy fps = parseCSV );
    void close();
    bool isNull( int col );
    bool endOfRecordSet() const;
    void next();
    void read( int col, bool&        outVal );
    void read( int col, char&        outVal );
    void read( int col, float&       outVal );
    void read( int col, double&      outVal );
    void read( int col, int&         outVal );
    void read( int col, std::string& outVal );
    void read( int col, TDateTime&   outVal );
private:
    paulst::CSVIterator<'\n'> m_currentRecord;
    paulst::CSVIterator<'\n'> m_endOfRecords;
    std::vector< std::string > m_rowValues;
    FieldParsingStrategy      m_fieldParsingStrategy;

    StringBackedCursor( const StringBackedCursor& );
    StringBackedCursor& operator=( const StringBackedCursor& );
    std::string colValue(int col) const;
    void parseCurrentRow();
};


};

#endif

