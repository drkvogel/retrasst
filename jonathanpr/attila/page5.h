//---------------------------------------------------------------------------

#ifndef page5H
#define page5H

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page5 : public View
{
	void renderRow(const std::string &lable,const int &data,int count) const;
	void renderRow(const std::string &lable,const std::string &data,int count) const;
protected:

	void askLogin(bool ShowErrors) const;
	bool showErrors() const;

	virtual void sendHeader() const;
	virtual void sendQuestions() const;
public:

	page5( const Model &data ) : View( data ) {}
};

#endif
//---------------------------------------------------------------------------

