//---------------------------------------------------------------------------

#ifndef page1H
#define page1H
//---------------------------------------------------------------------------

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page1 : public View
{
	void sendStatusInfo();
	void sendTabControls();
protected:
	virtual void sendHeader() const;
	virtual void sendQuestions();

public:
	page1( const Model &data ) : View( data )
	{
		this->setShowNoButtons();
	}
};

#endif
