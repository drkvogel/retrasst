//---------------------------------------------------------------------------

#ifndef viewH
#define viewH
//---------------------------------------------------------------------------
#include "model.h"

//---------------------------------------------------------------------------

//#define REQUIRED ""
//#define RADIO_REQUIRED false
//#define REQUIREDbool false

#define REQUIRED "required"
#define REQUIREDbool true
#define RADIO_REQUIRED true

class View
{
	const Model model;

protected:
	bool m_onlyShowNextButton;
	View( const Model &data ) : model( data ), m_onlyShowNextButton(false) {}
	const Model & getData() const { return model; }

	virtual void sendHeader() const = 0;
	virtual void sendHistory() const {}
	virtual void sendQuestions() const {}
	virtual void sendFooter() const;
	virtual void deployJS() const;
/*
	void sendLogin() const;
	void sendCentre() const;
	void sendConsultant() const;
	void sendParticipant() const;
	void sendChecklist() const;
	void sendTreatment() const;
*/
	void sendTitle( ) const;

	struct RADIO_OPTIONS
	{
		std::string text;
		std::string varname;
		int selected; //-1 for not selected
	};
	void addOption( int id, const std::string & name, int selected ) const;
	void addRadio( std::vector<std::string> &vectorvalues, const std::string & name, int selected, bool required = true ) const;
	void addRadio(std::string vectorvalue, const std::string name, int value, bool selected, bool required = true) const;
	void addRadioReadOnly(std::string vectorvalue, const std::string name, int value, bool selected ) const;

public:

	virtual ~View() {}
	void sendHTML( ) const ;
	void setOnlyShowNextButton() {m_onlyShowNextButton = true;}
};

//---------------------------------------------------------------------------

#endif
