#ifndef LOADPROJECTSH
#define LOADPROJECTSH

namespace paulst
{
    class LoggingService;
};

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class DBConnection;
class Projects;

class LoadProjects
{
public:
    LoadProjects( Projects* projects, paulst::LoggingService* log, paulstdb::DBConnection* con );
    ~LoadProjects();
    void execute();
private:
    paulst::LoggingService* m_log;
    paulstdb::DBConnection* m_con;
    Projects* m_projects;

    LoadProjects( const LoadProjects& );
    LoadProjects& operator=( const LoadProjects& );
};

};

#endif

