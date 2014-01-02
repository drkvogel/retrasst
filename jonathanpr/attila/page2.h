//---------------------------------------------------------------------------

#ifndef page2H
#define page2H
//---------------------------------------------------------------------------

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page2 : public View
{
	void askEligibility() const;
   	void showErrors() const;

protected:

	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:

	page2( const Model &data ) : View( data ) {}
};

#endif
