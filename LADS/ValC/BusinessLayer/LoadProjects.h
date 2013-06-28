#ifndef LOADPROJECTSH
#define LOADPROJECTSH

namespace paulst
{
    class LoggingService;
};

namespace valc
{

class DBConnection;
class Projects;

class LoadProjects
{
public:
    LoadProjects( Projects* projects, paulst::LoggingService* log, DBConnection* con );
    ~LoadProjects();
    void execute();
    int releaseReturnValue();
private:
    paulst::LoggingService* m_log;
    DBConnection* m_con;
    Projects* m_projects;

    LoadProjects( const LoadProjects& );
    LoadProjects& operator=( const LoadProjects& );
};

};

#endif

