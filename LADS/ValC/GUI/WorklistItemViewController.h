#ifndef WorklistItemViewControllerH
#define WorklistItemViewControllerH

#include "ModelEventListenerAdapter.h"

class TWorklistItemViewFrame;

namespace valc
{
    class SnapshotPtr;
    class TestResult;
    class WorklistRelative;
}

namespace valcui
{

class Model;


class WorklistItemViewController
{
public:
	WorklistItemViewController();
    void __fastcall     familyTreeClickHandler( TObject* sender );
    void                init();
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
	void                notify( int modelEvent, const EventData& eventData );
    void                onIdle();
    void                onResize();
    void                setModel( Model* m );
    void                setView( TWorklistItemViewFrame* view );
    void __fastcall     update();
private:
	TWorklistItemViewFrame*                                 m_view;
    IdleServiceUserAdapter<WorklistItemViewController>      m_idleServiceUser;
	ModelEventListenerAdapter<WorklistItemViewController>   m_eventListener;
    Model*                                                  m_model;

    void addTreeNodeForWorklistEntry( int idOfSelectedWorklistEntry, const valc::SnapshotPtr& s, const valc::WorklistRelative& wr, TTreeViewItem* parent = NULL );
    void clear();
    std::string describe( const valc::SnapshotPtr& s, const valc::WorklistRelative& wr ) const;
    void describeFamilyTree( const valc::SnapshotPtr& snapshot, const valc::WorklistEntry* w );
	void describeResult( const valc::TestResult* r );
    UnicodeString describeTest( const valc::SnapshotPtr& s, int testID ) const;
    const valc::WorklistEntry* searchLocalRunSequenceForWorklistEntry( const valc::SnapshotPtr& snapshot, int worklistEntryID ) const;
    const valc::WorklistEntry* searchQueueForWorklistEntry           ( const valc::SnapshotPtr& snapshot, int worklistEntryID ) const;
};

}

#endif

