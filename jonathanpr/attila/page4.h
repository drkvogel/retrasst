//---------------------------------------------------------------------------

#ifndef page4H
#define page4H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page4 : public View
{
protected:

	bool showErrors() const;
	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:

	page4( const Model &data ) : View( data ) {}
};

#endif
//---------------------------------------------------------------------------

