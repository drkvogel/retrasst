#ifndef WORKLISTDIRECTORYH
#define WORKLISTDIRECTORYH

#include <string>

namespace valc
{

class WorklistEntry;

class WorklistDirectory
{
public:
    class Func
    {
    public:
        virtual ~Func();
        virtual void execute( const WorklistEntry* wle ) = 0;
    };

    virtual ~WorklistDirectory();
    virtual const WorklistEntry* get( int id ) const = 0;
    /*
        f gets invoked for every WorklistEntry with the specified sampleDescriptor, or, if sampleDescriptor is empty,
        for every WorklistEntry in the collection.

        Note that, in the latter scenario, callback invocations are grouped by sampleDescriptor. Thus, if there 
        is more than one worklist entry with a given sample descriptor, then the invocations of the callback routine 
        for these entries are bound to be consecutive.
    */
    virtual void forEach( Func& f, const std::string& sampleDescriptor = "" ) const = 0;
};

};

#endif

