//---------------------------------------------------------------------------

#ifndef page0H
#define page0H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page0 : public View
{
	bool showErrors() const;
	void askCentre(bool showErrors) const;
protected:

	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:

	page0( const Model &data ) : View( data ) {}
};

#endif
//---------------------------------------------------------------------------
