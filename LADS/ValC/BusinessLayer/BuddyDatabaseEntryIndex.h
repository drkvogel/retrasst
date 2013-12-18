#ifndef BUDDYDATABASEENTRYINDEXH
#define BUDDYDATABASEENTRYINDEXH

#include <map>
#include <set>
#include <string>
#include <System.hpp>

namespace valc
{

class BuddyDatabaseEntry;

class BuddyDatabaseEntryIndex
{
public:
    typedef std::set<int> TestIDs;

    class Visitor
    {
    public:
        Visitor();
        virtual ~Visitor();
        virtual void visit( const BuddyDatabaseEntry* bde, const TestIDs& testsForWhichResults ) = 0;
    };

    BuddyDatabaseEntryIndex();
    ~BuddyDatabaseEntryIndex();
    void add( int buddySampleID, int alphaSampleID, const std::string& barcode, const std::string& databaseName, const TDateTime& dateAnalysed );
    const BuddyDatabaseEntry* get( int buddySampleID ) const;
    /*
        lookupBuddySampleIDForResult: returns zero if not found
    */
    int lookupBuddySampleIDForResult( int resultID ) const;
    void accept( Visitor* v ) const;
    /*
        Before calling this 'supplement' method, an entry must already have been added for the same buddySampleID.
    */
    void supplementEntryWithResultInfo( int buddySampleID, int resID, int resultTestID );
private:
    typedef std::map< int, const BuddyDatabaseEntry* > BDEMap;
    BDEMap m_entries;
    typedef std::map< int, TestIDs > EntryTestMap;
    EntryTestMap m_entryTests;
    typedef std::map< int, int > EntryIDKeyedOnResultID;
    EntryIDKeyedOnResultID m_entryIDs;

    BuddyDatabaseEntryIndex( const BuddyDatabaseEntryIndex& );
    BuddyDatabaseEntryIndex& operator=( const BuddyDatabaseEntryIndex& );
};

};

#endif

