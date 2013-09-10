#include <algorithm>
#include <cctype>
#include "Projects.h"
#include "Require.h"
#include "Trace.h"

namespace valc
{

Project::Project( int id, const std::string& externalName, const std::string& dbName )
    :
    m_id(id), m_externalName( externalName ), m_dbName( dbName )
{
}

Project::~Project()
{
}

int Project::getID() const
{
    return m_id;
}

std::string Project::getExternalName() const
{
    return m_externalName;
}

std::string Project::getDbName() const
{
    return m_dbName;
}

Projects::Projects()
{
}

void Projects::add( int id, const std::string& externalName, const std::string& dbName )
{
    ProjectPtr p( new Project( id, externalName, dbName ) );
    m_projects.push_back( p );
}



char toLower( const char& c )
{
    return std::tolower( c );
}

class MatchOnDatabaseName
{
private:
    std::string m_databaseName;
public:
    MatchOnDatabaseName( const std::string& dbName ) 
        : m_databaseName( dbName )
    {
        std::transform( m_databaseName.begin(), m_databaseName.end(), m_databaseName.begin(), toLower );
    }

    bool operator()( const ProjectPtr& p )
    {
        return ( p->getExternalName() == m_databaseName ) || ( p->getDbName() == m_databaseName );
    }
};

bool Projects::canFindProjectIDForDatabase( const std::string& databaseName ) const
{
    ProjectList::const_iterator i = std::find_if( m_projects.begin(), m_projects.end(), MatchOnDatabaseName( databaseName ) );

    return ( i != m_projects.end() );
}

int Projects::findProjectIDForDatabase( const std::string& databaseName ) const
{
    ProjectList::const_iterator i = std::find_if( m_projects.begin(), m_projects.end(), MatchOnDatabaseName( databaseName ) );

    require ( i != m_projects.end() );

    return (*i)->getID();
}

int Projects::size() const
{
    return m_projects.size();
}

}

