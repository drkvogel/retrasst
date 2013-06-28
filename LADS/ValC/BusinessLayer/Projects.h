#ifndef PROJECTSH
#define PROJECTSH

#include <boost/shared_ptr.hpp>
#include <string>
#include <SysUtils.hpp>
#include <vector>

namespace valc
{

class Project
{
public:
    Project( int id, const std::string& externalName, const std::string& dbName );
    ~Project();
    int         getID()             const;
    std::string getExternalName()   const;
    std::string getDbName()         const;
private:
    const int m_id;
    const std::string m_externalName;
    const std::string m_dbName;

    Project( const Project& p );
    Project& operator=( const Project& p );
};

typedef boost::shared_ptr<Project> ProjectPtr;

class ProjectNotFoundException : public Exception
{
public:
    __fastcall ProjectNotFoundException( const std::string& dbName );
};

class Projects
{
public:
    Projects();
    void add( int id, const std::string& externalName, const std::string& dbName );
    int findProjectIDForDatabase( const std::string& databaseName ) const;
    int size() const;
private:
    typedef std::vector< ProjectPtr > ProjectList;
    ProjectList m_projects;

    Projects( const Projects& );
    Projects& operator=( const Projects& );
};

};

#endif

