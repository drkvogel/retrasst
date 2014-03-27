//---------------------------------------------------------------------------

#ifndef viewH
#define viewH
//---------------------------------------------------------------------------
#include "model.h"

#define REQUIRED "required"
#define REQUIREDbool true
#define RADIO_REQUIRED true

class View
{
	Model model;
	bool m_ShowNoButtons;

protected:
	View( const Model &data ) : model( data ), m_ShowNoButtons(false) {}
	Model &getData() { return model; }

	virtual void sendHeader() const = 0;
	virtual void sendHistory() const {}
	virtual void sendQuestions() {}
	virtual void sendFooter();
	virtual void deployJS() const;

	void sendTitle( ) const;

	struct RADIO_OPTIONS
	{
		std::string text;
		std::string varname;
		int selected; //-1 for not selected
	};
	void addOption( int id, const std::string & name, int selected ) const;
	void addOption(const std::string id, const std::string & name, const std::string selected  ) const;
	void addRadio( std::vector<std::string> &vectorvalues, const std::string & name, int selected, bool required = true ) const;
	void addRadio(std::string vectorvalue, const std::string name, int value, bool selected, bool required = true) const;
	void addRadioReadOnly(std::string vectorvalue, const std::string name, int value, bool selected ) const;

	static std::string formatNumber(unsigned int number, int miniumSize=2);
public:

	virtual ~View() {}
	void sendHTML( );
	void setShowNoButtons() {m_ShowNoButtons = true;}
};

//---------------------------------------------------------------------------

#endif
