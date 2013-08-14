#include "MockConnection.h"
#include "MockConnectionFactory.h"

namespace valc
{

MockConnectionFactory::MockConnectionFactory()
{
}

MockConnection* MockConnectionFactory::createConnection()
{
    MockConnection* mc = new MockConnection();
    mc->setBuddyDB  ( m_buddyDB   );
    mc->setClusters ( m_clusters  );
    mc->setProjects ( m_projects  );
    mc->setTestNames( m_testNames );
    mc->setWorklist ( m_worklist  );
    return mc;
}

void MockConnectionFactory::setBuddyDB( const std::string& buddyDB )
{
    m_buddyDB = buddyDB;
}

void MockConnectionFactory::setClusters( const std::string& clusters )
{
    m_clusters = clusters;
}

void MockConnectionFactory::setProjects( const std::string& projects )
{
    m_projects = projects;
}

void MockConnectionFactory::setTestNames( const std::string& testNames )
{
    m_testNames = testNames;
}

void MockConnectionFactory::setWorklist( const std::string& worklist )
{
    m_worklist = worklist;
}

}

