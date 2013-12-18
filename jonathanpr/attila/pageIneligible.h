//---------------------------------------------------------------------------

#ifndef pageIneligibleH
#define pageIneligibleH
//---------------------------------------------------------------------------

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class pageIneligible : public View
{
	void showNotEligibile() const;

protected:
	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:
	pageIneligible( const Model &data ) : View( data )
	{
		this->setOnlyShowNextButton();
	}
};

#endif


