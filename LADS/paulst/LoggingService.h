#ifndef LOGGINGSERVICEH
#define LOGGINGSERVICEH

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

std::string relative( const std::string& filePath );

#define LOG(msg) { if(m_log) { std::ostringstream _s;  _s << relative(__FILE__) << "," << __LINE__ << ": " << msg; m_log->log( _s.str() ); } }

namespace paulst
{

class WorkerThread;

class Writer
{
public:
    virtual ~Writer() {}
    virtual void write( const std::string& msg ) = 0;
};

class LoggingService
{
public:
    // LoggingService assumes ownership of the Writer instance.
    // i.e. will delete it on destruction.
    LoggingService( Writer* w );
    ~LoggingService();
    void log( const std::string& msg );
    bool hasPending();
private:
    std::auto_ptr<paulst::WorkerThread> m_workerThread;
    Writer* m_writer;
};

class FileWriter : public Writer
{
public:
    FileWriter( const std::string& filename );
    void write( const std::string& msg );
private:
    std::ofstream m_file;
};

};

#endif

