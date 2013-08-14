#ifndef BUDDYDATABASEENTRYINDEXH
#define BUDDYDATABASEENTRYINDEXH

#include <map>
#include <string>

namespace valc
{

class BuddyDatabaseEntry;

class BuddyDatabaseEntryIndex
{
public:
    BuddyDatabaseEntryIndex();
    ~BuddyDatabaseEntryIndex();
    void add( int buddySampleID, int alphaSampleID, const std::string& barcode, const std::string& databaseName, const TDateTime& dateAnalysed );
    const BuddyDatabaseEntry* get( int buddySampleID ) const;
private:
    typedef std::map< int, const BuddyDatabaseEntry* > BDEMap;
    BDEMap m_entries;

    BuddyDatabaseEntryIndex( const BuddyDatabaseEntryIndex& );
    BuddyDatabaseEntryIndex& operator=( const BuddyDatabaseEntryIndex& );
};

};

#endif

