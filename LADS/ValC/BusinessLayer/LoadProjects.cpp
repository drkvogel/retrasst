#include "Cursor.h"
#include "DBConnection.h"
#include <boost/lexical_cast.hpp>
#include "LoadProjects.h"
#include "LoggingService.h"
#include "Projects.h"
#include "Trace.h"


namespace valc
{

LoadProjects::LoadProjects( Projects* projects, paulst::LoggingService* log, paulstdb::DBConnection* con )
    :
    m_log(log),
    m_con(con),
    m_projects(projects)
{
    trace( "LoadProjects constructor" );
}

LoadProjects::~LoadProjects()
{
    trace( "LoadProjects destructor" );
}

void LoadProjects::execute()
{
    std::string sql = "select project_cid, lower(external_name), lower(db_name) from c_project";

    for ( std::auto_ptr<paulstdb::Cursor> cursor( m_con->executeQuery( sql ) ); ! cursor->endOfRecordSet(); cursor->next() )
    {
        int projectID = 0;
        std::string externalName, dbName;

        cursor->read( 0, projectID );
        cursor->read( 1, externalName );
        cursor->read( 2, dbName );

        m_projects->add( projectID, externalName, dbName  );
    }

    m_log->log( std::string("Loaded ") + boost::lexical_cast<std::string>( m_projects->size() ) + " projects." );
}

}

