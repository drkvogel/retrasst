#ifndef QCVDISPLAYMODEQCH
#define QCVDISPLAYMODEQCH

#include "QCViewController.h"

namespace valcui
{

class QCVDisplayModeQC : public QCViewController::QCVDisplayMode
{
public:
    QCVDisplayModeQC( QCViewController* c, valc::SnapshotPtr s, valcui::Model* m );
    void doUpdate();
private:
    QCViewController*           const m_controller;
    const valc::SnapshotPtr     m_snapshot;
    valcui::Model*              const m_model;
    const valc::WorklistEntry*  const m_worklistEntry;
};

}

#endif

