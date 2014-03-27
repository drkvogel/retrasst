//---------------------------------------------------------------------------

#ifndef page2H
#define page2H
//---------------------------------------------------------------------------

#include <string>
#include "view.h"
//---------------------------------------------------------------------------

class page2 : public View
{
protected:
	virtual void sendHeader() const;
	virtual void sendQuestions();

public:
	page2( const Model &data ) : View( data )
	{
		this->setShowNoButtons();
	}
};

#endif
