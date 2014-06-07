#ifndef BusinessLayerH
#define BusinessLayerH

#include "API.h"
#include <memory>
#include <string>
#include <System.Classes.hpp>

namespace paulst
{
	class LoggingService;
}


namespace stef
{
	class ThreadPool;
}


namespace valcui
{

/** Encapsulates any non-trivial interaction with the business layer.
  * It presents a synchronous interface to operations that might,
  * in fact, be asynchronous (eg queuing a rerun).
  *
  * It has its own thread on which to interact with the business layer.
  * This ensures that the UI thread doesn't get stuck, for example it can
  * feel free to time-out whilst waiting for a database lock.
  */
class BusinessLayer
{
public:
	BusinessLayer(
		int machineID,
		int userID,
		const std::string& config,
		paulst::LoggingService* log,
		valc::UserAdvisor* warningsListener
		);
	~BusinessLayer();
    void borrowSnapshot( TThreadMethod callback );
    bool close();
    void forceReload();
    valc::SnapshotPtr& getSnapshot();

private:
	stef::ThreadPool* m_threadPool;
	valc::SnapshotPtr m_snapshot;
    std::unique_ptr<valc::UserAdvisor> m_warningsListener;

	BusinessLayer( const BusinessLayer& );
	BusinessLayer& operator=( const BusinessLayer& );

	static const int THREAD_POOL_SHUTDOWN_WAIT_MILLIS = 1000;
};

}
#endif

