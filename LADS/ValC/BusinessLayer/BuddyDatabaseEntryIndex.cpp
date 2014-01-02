#include "API.h"
#include "BuddyDatabaseEntryIndex.h"
#include "Require.h"

namespace valc
{

BuddyDatabaseEntryIndex::Visitor::Visitor()
{
}

BuddyDatabaseEntryIndex::Visitor::~Visitor()
{
}

BuddyDatabaseEntryIndex::BuddyDatabaseEntryIndex()
{
}

BuddyDatabaseEntryIndex::~BuddyDatabaseEntryIndex()
{
    for ( BDEMap::iterator i = m_entries.begin(); i != m_entries.end(); ++i )
    {
        const BuddyDatabaseEntry* bde = i->second;
        delete bde;
    }    
}

void BuddyDatabaseEntryIndex::add( int buddySampleID, int alphaSampleID, const std::string& barcode, const std::string& databaseName,
    const TDateTime& dateAnalysed )
{
    if ( 0 == m_entries.count( buddySampleID ) )
    {
        m_entries.insert( std::make_pair( buddySampleID, 
            new BuddyDatabaseEntry( buddySampleID, alphaSampleID, barcode, databaseName, dateAnalysed ) ) );
    }
}

const BuddyDatabaseEntry* BuddyDatabaseEntryIndex::get( int buddySampleID ) const
{
    BDEMap::const_iterator i = m_entries.find( buddySampleID );
    require( i != m_entries.end() );
    return i->second;
}


int BuddyDatabaseEntryIndex::lookupBuddySampleIDForResult( int resultID ) const
{
    EntryIDKeyedOnResultID::const_iterator i = m_entryIDs.find( resultID );
    return i == m_entryIDs.end() ? 0 : i->second; 
}

void BuddyDatabaseEntryIndex::accept( Visitor* v ) const
{
    const TestIDs emptySet;

    for( BDEMap::const_iterator i = m_entries.begin(); i != m_entries.end(); ++i )
    {
        const BuddyDatabaseEntry* bde = i->second;
        const int entryID = i->first;
        EntryTestMap::const_iterator i2 = m_entryTests.find( entryID );
        v->visit( bde, i2 == m_entryTests.end() ? emptySet : i2->second );
    }
}

void BuddyDatabaseEntryIndex::supplementEntryWithResultInfo( int buddySampleID, int resID, int resultTestID )
{
    require( m_entries.count( buddySampleID ) );
    m_entryIDs.insert( std::make_pair( resID, buddySampleID ) );

    // WON'T insert, if already exists
    std::pair<EntryTestMap::iterator,bool> i = m_entryTests.insert( std::make_pair( buddySampleID, TestIDs() ) ); 

    i.first->second.insert( resultTestID );
}

}

