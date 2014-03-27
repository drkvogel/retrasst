//---------------------------------------------------------------------------

#ifndef page3H
#define page3H
//---------------------------------------------------------------------------

#include <string>
#include "view.h"
//---------------------------------------------------------------------------

class page3 : public View
{
protected:
	virtual void sendHeader() const;
	virtual void sendQuestions();
	virtual void deployJS() const;
public:
	page3( const Model &data ) : View( data )
	{
		this->setShowNoButtons();
	}
};

#endif
