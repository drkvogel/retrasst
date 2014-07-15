#ifndef QCVDISPLAYMODEUNKNOWNH
#define QCVDISPLAYMODEUNKNOWNH

#include "API.h"
#include "QCViewController.h"
#include "QCViewData.h"

namespace valcui
{

class Model;

class QCVDisplayModeUnknown : public QCViewController::QCVDisplayMode
{
public:
    QCVDisplayModeUnknown( QCViewController* c, valc::SnapshotPtr s, valcui::Model* m );
    void doUpdate();

private:
    QCViewController*           const m_controller;
    const valc::SnapshotPtr     m_snapshot;
    valcui::Model*              const m_model;
    const QCViewData            m_data;

    void addNodesForControllingQCs( const valc::QCControls& controls );
    void addNodesForRules( int controlResultID );
    std::string describeControl( const valc::QCControl& c ) const;
    std::string describeControlResult( const valc::QCControl& c ) const;
    std::string describeTestResult( const valc::TestResult* r ) const;
};

}

#endif

