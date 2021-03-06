#ifndef CONTROLMODELH
#define CONTROLMODELH

#include "API.h"
#include "CritSec.h"
#include "QCControlCache.h"
#include "RuleEngine.h"
#include "UncontrolledResult.h"

namespace valc
{

class SampleRunGroupModel;

class ControlModel
{
public:
    ControlModel();
    virtual ~ControlModel();

    virtual void            clear                      ()                                                     = 0;
    virtual ControlStatus   getControlStatus           ( int testID, const IDToken& sampleRunID ) const       = 0;
    virtual void            notifyQCEvaluationStarted  ( const UncontrolledResult& qcResult, const IDToken& runID ) = 0;
    virtual void            notifyQCEvaluationCompleted( const RuleResults& evaluation, int QCResultID )      = 0;
    virtual void            setSampleRunGroupModel     ( SampleRunGroupModel* m )                             = 0;

    class RuleResultPublisherAdapter : public RuleResultPublisher
    {
    public:
        RuleResultPublisherAdapter( ControlModel* cm );
        void publish( const RuleResults& results, int forResult );
    private:
        ControlModel* m_model;
    };

    class RuleEngineQueueListenerAdapter : public RuleEngineQueueListener
    {
    public:
        RuleEngineQueueListenerAdapter( ControlModel* cm );
        void notifyQueued( const UncontrolledResult& r, const IDToken& runID );
    private:
        ControlModel* m_model;
    };

private:
    ControlModel( const ControlModel& );
    ControlModel& operator=( const ControlModel& );
};

class ControlModelImpl : public ControlModel
{
public:

    ControlModelImpl();
    void            clear                       ();
    ControlStatus   getControlStatus            ( int testID, const IDToken& sampleRunID ) const;
    void            notifyQCEvaluationStarted   ( const UncontrolledResult& qcResult, const IDToken& runID );
    void            notifyQCEvaluationCompleted ( const RuleResults& evaluation, int QCResultID );
    void            setSampleRunGroupModel      ( SampleRunGroupModel* m );
private:
    paulst::CritSec                                         m_cs;
    SampleRunGroupModel*                                    m_sampleRunGroupModel;
    std::map< int, std::pair<UncontrolledResult,IDToken> >  m_pending;
    QCControlCache                                          m_cache;
};

}

#endif

