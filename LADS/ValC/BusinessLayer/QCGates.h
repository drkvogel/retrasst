#ifndef QCGATESH
#define QCGATESH

#include "Gate.h"
#include "Gates.h"

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace paulstdb
{
    class AbstractConnectionFactory;
}

namespace stef
{
    class ThreadPool;
}

namespace valc
{

class QCGates : public Gates
{
public:

    QCGates( 
        stef::ThreadPool* dbQueryRunner,
        const paulst::Config* config,
        paulstdb::AbstractConnectionFactory* connectionFactory,
        paulst::LoggingService* log,
        int localMachineID );
    ~QCGates();
    void add( const Gate& gate, const std::string& QCLotBarcodePattern, int testID );
    /*
        Returns NULL if no matching gate found.
    */
    const Gate* getActiveGateFor( const UncontrolledResult& r );
private:
    typedef std::pair< std::string, int > Key;
    typedef std::map< Key, const Gate* > GateMap;

    GateMap m_gates;

    QCGates( const QCGates& );
    QCGates& operator=( const QCGates& );
    QCGates::Key makeKey( const std::string& barcode, int testID ) const;
};

}

#endif

