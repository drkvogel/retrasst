#include "AcquireCriticalSection.h"
#include <boost/lexical_cast.hpp>
#include "LoggingService.h"
#include "Require.h"
#include "WorkerThread.h"

std::string relative( const std::string& filePath )
{
    std::size_t lastSep = filePath.find_last_of( '\\' );

    std::string relativeName = ( lastSep == std::string::npos ) ? filePath : filePath.substr( lastSep + 1 );

    std::size_t lastDot = relativeName.find_last_of( '.' );

    if ( lastDot != std::string::npos )
    {
        relativeName.resize( lastDot );
    }

    return relativeName;
}

namespace paulst
{


class LogWritingTask : public paulst::WorkerThreadTask
{
public:
    LogWritingTask( Writer* w, const std::string& msg )
        : m_writer(w), m_msg(msg)
    {
    }

    bool execute()
    {
        if ( m_msg == "stop" )
        {
            return false;
        }

        m_writer->write( m_msg );
        return true;
    }

private:
    Writer* m_writer;
    std::string m_msg;
};

LoggingService::LoggingService( Writer* w )
    :
    m_workerThread(new paulst::WorkerThread()), m_writer( w )
{
}

LoggingService::~LoggingService()
{
    while ( hasPending() )
    {
        Sleep(40);
    }

    delete m_writer;
}

bool LoggingService::hasPending()
{
    return m_workerThread->queueSize();
}

void LoggingService::log( const std::string& msg )
{
    m_workerThread->queueTask( new LogWritingTask( m_writer, msg ) );
}

FileWriter::FileWriter( const std::string& filename )
    : m_file( filename.c_str(), std::ios_base::out | std::ios_base::app  )
{
    throwUnless( m_file );
}

void FileWriter::write( const std::string& msg )
{
    if ( m_file )
    {
        m_file << msg << "\n";
    }

    if ( m_file )
    {
        m_file.flush();
    }
}

}

