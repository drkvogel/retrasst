#ifndef MODELEVENTLISTENERH
#define MODELEVENTLISTENERH

namespace valcui
{

class ModelEventListener
{
public:
    ModelEventListener();
    virtual ~ModelEventListener();
    virtual void notify( int eventID ) = 0;
};

}

#endif

