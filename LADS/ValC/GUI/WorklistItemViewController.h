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
	WorklistItemViewController( TWorklistItemViewFrame* widgetContainer, Model* m );
    void __fastcall familyTreeClickHandler( TObject* sender );
	void notify( int modelEvent );
    void __fastcall update();
private:
	TWorklistItemViewFrame* const                           m_widgetContainer;
	ModelEventListenerAdapter<WorklistItemViewController>   m_eventListener;
    Model* const                                            m_model;

    void addTreeNodeForWorklistEntry( const valc::SnapshotPtr& s, const valc::WorklistRelative& wr, TTreeViewItem* parent = NULL );
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

