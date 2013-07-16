#ifndef TRACETIMEH
#define TRACETIMEH

namespace paulst
{

class TraceTime
{
public:
    TraceTime(const char* file, int line );
    ~TraceTime();
private:
    long m_start;
    std::string m_file;
    int m_line;

    TraceTime( const TraceTime& );
    TraceTime& operator=( const TraceTime& );
};

};

#endif

