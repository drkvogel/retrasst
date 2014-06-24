#ifndef MODELEVENTLISTENERH
#define MODELEVENTLISTENERH

#include "API.h"
#include <boost/variant.hpp>
#include <string>

namespace valcui
{

typedef boost::variant< int, std::string, valc::IDToken > EventData;

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
    virtual void notify( int eventID, const EventData& eventData ) = 0;
};

}

#endif

