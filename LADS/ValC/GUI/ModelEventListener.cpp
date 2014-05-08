#include "ModelEventListener.h"

namespace valcui
{

ModelEventListener::ModelEventListener()
{
}

ModelEventListener::~ModelEventListener()
{
}

void ModelEventListener::onForceReload( valc::SnapshotPtr& )
{
}

void ModelEventListener::onWarningAlarmOn()
{
}

void ModelEventListener::onWarningAlarmOff()
{
}

void ModelEventListener::onWorklistEntrySelected( int worklistEntryID )
{
}

}

