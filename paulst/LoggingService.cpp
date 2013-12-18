#include "AcquireCriticalSection.h"
#include <boost/lexical_cast.hpp>
#include <cstdarg>
#include <cstdio>
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
		try
		{
			m_writer->write( m_msg );
		}
		catch( ... )
		{
		}
		return true;
	}

private:
	Writer* m_writer;
	std::string m_msg;
};

class StopThreadTask : public paulst::WorkerThreadTask
{
public:

	bool execute()
	{
		return false;
	}
};


LoggingService::LoggingService( Writer* w )
    :
    m_workerThread(new paulst::WorkerThread()), m_writer( w )
{
}

LoggingService::~LoggingService()
{
	m_workerThread->queueTask( new StopThreadTask() );
	m_workerThread->waitFor();
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

void LoggingService::logFormatted( const char* msgFormat, ... )
{
    char buffer[2048];
    va_list args;
    va_start(args, msgFormat );
    std::vsprintf( buffer, msgFormat, args );
    va_end(args);
    log( buffer ); 
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

