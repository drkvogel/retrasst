#ifndef LIMSParamsH
#define LIMSParamsH

#include <inifiles.hpp>
#include <registry.hpp>
#include <FileCtrl.hpp>
#include <string>

//==============================================================================

class LIMSParams
{
	int buddyID;
	HANDLE mutexHandle;

//	TIniFile * iniFile;
	TRegistry * regKey;

	std::string section;
	bool inRegistry;

	static const short TIMEOUT = 2000;

	LIMSParams();							// constructor for singleton

	LIMSParams & operator=( LIMSParams & );			// not implemented
	LIMSParams( const LIMSParams & );				// not implemented

	void readRegistry();

public:

	static LIMSParams & instance();
	~LIMSParams();

	bool openSection( const std::string & name, bool create );
	void closeSection();

	void copyValues( TStrings * valueList ) const;
	int findValue( const std::string & name, int value ) const;
	std::string findValue( const std::string & name, std::string value = "" ) const;

	void setValue( const std::string & name, int value );
	void setValue( const std::string & name, const std::string & value );
	void removeValue( const std::string & name );

	void saveSize( TForm * display );
	void restoreSize( TForm * display );

	int getBuddyID() const { return buddyID; }
	void setBuddyID( int newID );
//	int getClusterID() const { return clusterID; }
//	void setClusterID( int newID );

	bool useLocalWorklist();
	void setLocalWorklist( bool useBoth );

	bool checkMachine();
	bool checkUnique();

	std::string getLogFolder() const;
	std::string getProgVersion() const;
};

//==============================================================================

#endif

