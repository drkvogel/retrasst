#include "API.h"
#include "BuddyDatabaseEntryIndex.h"
#include "Require.h"

namespace valc
{

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

}

