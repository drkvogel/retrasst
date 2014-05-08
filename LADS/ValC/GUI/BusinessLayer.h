#ifndef BusinessLayerH
#define BusinessLayerH

#include "API.h"
#include <memory>
#include <string>

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

