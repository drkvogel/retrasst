#include "AsyncTaskRerun.h"
#include <boost/shared_ptr.hpp>
#include "ExceptionHandler.h"
#include "ModelEventConstants.h"
#include <sstream>

namespace valcui
{

void closeHandle( HANDLE h )
{
    CloseHandle(h);
}

void RerunFunc::operator()( int& )
{
    HANDLE h = snapshot->queueForRerun( worklistID, sampleRunID, sampleDescriptor );

    boost::shared_ptr<void> onBlockExit( h, closeHandle );

    const unsigned long waitResult = WaitForSingleObject( h, maxWaitMillis );

    if ( waitResult != WAIT_OBJECT_0 )
    {
        throwWaitException( waitResult, "Rerun" );
    }
}

AsyncTaskRerun::AsyncTaskRerun(
    valc::SnapshotPtr snapshot,
    int worklistID,
    const valc::IDToken& sampleRunID,
    const std::string& sampleDescriptor,
    const std::string& barcode,
    const std::string& testName,
    unsigned long maxWaitMillis )
    :
    m_barcode( barcode ),
    m_worklistID( worklistID ),
    m_testName( testName )
{        
    m_rerunFunc.worklistID          = worklistID;
    m_rerunFunc.sampleRunID         = sampleRunID;
    m_rerunFunc.sampleDescriptor    = sampleDescriptor;
    m_rerunFunc.snapshot            = snapshot;
    m_rerunFunc.maxWaitMillis       = maxWaitMillis;
}

std::string AsyncTaskRerun::getDesc() const
{
    std::ostringstream desc;
    desc << "Queueing rerun of test '" << m_testName << "' for barcode '" << m_barcode 
        << "' (Worklist ID:" << m_worklistID << ")";
    return desc.str();
}

HANDLE AsyncTaskRerun::queue( stef::ThreadPool* tp )
{
    m_taskSubmission.reset( new stef::Submission<int, RerunFunc>( m_rerunFunc, tp ) );

    return m_taskSubmission->completionSignal();
}

void AsyncTaskRerun::callback( ModelEventListener* eventSink )
{
    if ( succeeded( m_taskSubmission.get(), eventSink ) )
    {
        eventSink->notify( MODEL_EVENT::RERUN_QUEUED );
    }
}

}


