//---------------------------------------------------------------------------

#ifndef page3H
#define page3H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page3 : public View
{
	bool showErrors() const;

protected:

	virtual void sendHeader() const;
	virtual void sendQuestions() const;

	virtual void deployJS() const;

	void exAddRadio(std::string vectorvalue, const std::string name, int value, bool selected, bool required, bool other ) const;
public:

	page3( const Model &data ) : View( data ) {}
};

#endif
