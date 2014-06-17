#ifndef MODELEVENTLISTENERH
#define MODELEVENTLISTENERH

#include <string>

namespace valcui
{

/** An interface by means of which a UI component can keep informed of
  * changes made to the BusinessLayer.
  * (This promotes loose-coupling between views: views don't need to
  * keep each other informed of the changes they make to the business layer,
  * provided that they make those changes via the Model.
  */
class ModelEventListener
{
public:
    ModelEventListener();
    virtual ~ModelEventListener();
    virtual void notify( int eventID, const std::string& eventData ) = 0;
};

}

#endif

