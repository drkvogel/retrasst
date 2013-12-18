#ifndef GATESH
#define GATESH

namespace valc
{

class Gate;
class UncontrolledResult;

class Gates
{
public:
    Gates();
    virtual ~Gates();
    /*
        Returns NULL if no gate configured/known for the specific result.
    */
    virtual const Gate* getActiveGateFor( const UncontrolledResult& r ) = 0;
};

}

#endif

