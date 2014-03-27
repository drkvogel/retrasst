//---------------------------------------------------------------------------

#ifndef page0H
#define page0H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page0 : public View
{
	bool showErrors();

protected:

	virtual void sendHeader() const;
	virtual void sendQuestions();

public:

	page0( const Model &data ) : View( data ) {}
};

#endif
//---------------------------------------------------------------------------
