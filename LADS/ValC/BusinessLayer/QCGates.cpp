#include <boost/bind.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBQueryTask.h"
#include "QCGates.h"
#include "ThreadPool.h"
#include "UncontrolledResult.h"
#include <utility>

namespace valc
{

void addGate( paulstdb::Cursor* c, QCGates* qcGates )
{
    const std::string   barcode     = paulstdb::read<std::string>( *c, 0 );
    const double        mean        = paulstdb::read<double>     ( *c, 1 );
    const double        stdDev      = paulstdb::read<double>     ( *c, 2 );
    const int           testID      = paulstdb::read<int>        ( *c, 3 );
    const int           gateID      = paulstdb::read<int>        ( *c, 4 );
    const int           source      = paulstdb::read<int>        ( *c, 5 );
    const int           qcLotStatus = paulstdb::read<int>        ( *c, 6 );

    Gate gate( mean, stdDev, gateID, source );

    qcGates->add( gate, barcode, testID );
}

QCGates::QCGates( 
        stef::ThreadPool* dbQueryRunner,
        const paulst::Config* config,
        paulstdb::AbstractConnectionFactory* connectionFactory,
        paulst::LoggingService* log,
        int localMachineID )
{
    dbQueryRunner->addTask( new DBQueryTask( "QCGates", connectionFactory, config, boost::bind( addGate, _1, this ) ) );
}

QCGates::~QCGates()
{
    for ( GateMap::iterator i = m_gates.begin(); i != m_gates.end(); ++i )
    {
        delete i->second;
    }
}

void QCGates::add( const Gate& gate, const std::string& QCLotBarcodePattern, int testID )
{
    QCGates::Key key = makeKey( QCLotBarcodePattern, testID );

    if ( 0 == m_gates.count( key ) )
    {
        Gate* copy = new Gate(gate);
        m_gates.insert( std::make_pair( key, copy ) );
    }
}

const Gate* QCGates::getActiveGateFor( const UncontrolledResult& r )
{
    QCGates::Key key = makeKey( r.barcode, r.testID );

    GateMap::const_iterator i = m_gates.find( key );

    return i == m_gates.end() ? NULL : i->second;
}

QCGates::Key QCGates::makeKey( const std::string& barcode, int testID ) const
{
    std::string copy(barcode);
    copy.resize(8);
    return std::make_pair( copy, testID );
}

}

