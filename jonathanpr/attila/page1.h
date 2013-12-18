//---------------------------------------------------------------------------

#ifndef page1H
#define page1H
//---------------------------------------------------------------------------

#include <string>
#include "view.h"

//---------------------------------------------------------------------------

class page1 : public View
{
	void askCentre(bool showErrors) const;
	void askConsultant(bool showErrors) const;
	void askOptionalDetails(bool showErrors) const;
	void askSocialServicesPro(bool showErrors) const;
	void askPatientDetils(bool showErrors) const;
	bool showErrors() const;
	void askLogin(bool ShowErrors) const;
	void askRandomiser(bool ShowErrors) const;
protected:

	virtual void sendHeader() const;
	virtual void sendQuestions() const;

public:

	page1( const Model &data ) : View( data ) {}
};

#endif
