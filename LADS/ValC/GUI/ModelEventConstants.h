#ifndef MODELEVENTCONSTANTSH
#define MODELEVENTCONSTANTSH

namespace valcui
{

namespace MODEL_EVENT
{

    static const int FORCE_RELOAD                       = 1;
    static const int NEW_WORKLIST_ENTRY                 = 2;
    static const int RERUN_QUEUED                       = 3;
    static const int PROCESSED_PENDING_UPDATES          = 4;
    static const int SAMPLE_ADDED_TO_QUEUE              = 5;
    static const int SAMPLE_RUN_CLOSED_OFF              = 6;
    static const int UPDATE_FAILED                      = 7;
    static const int WARNING_ALARM_ON                   = 8;
    static const int WARNING_ALARM_OFF                  = 9;
    static const int WORKLIST_ENTRY_CHANGED             = 10;
    static const int WORKLIST_ENTRY_SELECTION_CHANGE    = 11;
    static const int APPLICATION_CONTEXT_DELETED        = 12;
    static const int APPLICATION_INITIALISED            = 13;
    static const int ASYNC_TASK_ERROR                   = 14;
}

}

#endif

