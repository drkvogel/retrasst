//---------------------------------------------------------------------------

#ifndef page6H
#define page6H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page6 : public View
{
protected:

	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:

	page6( const Model &data ) : View( data )
	{
		this->setOnlyShowNextButton();
	}
};

#endif
//---------------------------------------------------------------------------
