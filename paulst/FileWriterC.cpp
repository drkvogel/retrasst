#include "FileWriterC.h"
#include "Require.h"

namespace paulst
{

FileWriterC::FileWriterC( const std::string& filename )
{
    m_fp = fopen( filename.c_str(), "a" );
    throwUnless( m_fp );

}

FileWriterC::~FileWriterC()
{
    fclose( m_fp );
}

void FileWriterC::write( const std::string& msg )
{
    fputs( msg.c_str(), m_fp );
    fputc( '\n', m_fp );
    fflush( m_fp );
}

}

