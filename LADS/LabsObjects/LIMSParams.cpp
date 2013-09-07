/*---------------------------------------------------------------------------
 *
 *  Routines for labs software to access the registry and .ini files
 *
 *		20 May 2004, NG:	Created to replace SharedClasses/InitParams
 *  	18 June, NG/AM:     Checked in to CVS/GeneralClasses/CoreUtilities
 *		31 March 2005:		Allow caller to update buddy and cluster IDs
 *		7 April '05, NG:	Added method to suggest folder for log files
 *		4 August 2005:		Replace getValue() with findValue() [no Exception]
 *		10 November, NG:	Check registry before using the .ini file
 *		11 November, NG:	Update registry even when read from .ini file
 *		19 Feb. 2007, NG:	Always use program name as part of key name
 *		30 May 08, NG:		Remove static functions to ensure mutex checked
 *		6 June 2008, NG:	Remove option to ignore second program instance
 *		20 June 08, NG:		Check buddy ID against Windows machine name
 *		20 January 2009:	Don't terminate application (caller should)
 *      21 May 2009, NG:	Removed support for .ini file; added copyValues()
 *      12 Oct 09, NG:	    Add local/central worklist migration switch
 *
 *--------------------------------------------------------------------------*/


#include <vcl.h>
#include <stdio.h>
#include <sstream>
#include <memory>
#include "LIMSParams.h"
#include "LCDbAnalyser.h"

#pragma hdrstop

#pragma package(smart_init)

//==============================================================================

LIMSParams & LIMSParams::instance()
{
	static LIMSParams singleton;
	return singleton;
}

//---------------------------------------------------------------------------

LIMSParams::LIMSParams()
 : mutexHandle( NULL ), regKey( NULL ), inRegistry( false )
{
	regKey = new TRegistry;
	openSection( "", false );
	buddyID = findValue( "Buddy ID", 0 );
}

//---------------------------------------------------------------------------

LIMSParams::~LIMSParams()
{
	if( mutexHandle != NULL )
		CloseHandle( mutexHandle );
	delete regKey;
}

//==============================================================================
//	Test for multiple instances, using the application title as a mutex
//==============================================================================

bool LIMSParams::checkUnique()
{
	String title = Application -> Title;
	for( ;; )
	{
		mutexHandle = CreateMutex( NULL, false, title.c_str() );
		if( mutexHandle != NULL )
		{
			switch( WaitForSingleObject( mutexHandle, TIMEOUT ) )
			{
				case WAIT_OBJECT_0:
					return true;             	// got mutex - only instance

				case WAIT_ABANDONED:
					continue;                   // timed out - try again
			}
		}

		int response = Application -> MessageBox( L"Program is already running.",
										NULL, MB_RETRYCANCEL | MB_ICONWARNING );
		switch( response )
		{
			case IDCANCEL:
				return false;					// caller should abort

			default:
				continue;                       // otherwise try again
		}
	}
}

//==============================================================================
//	Identify machine; warn if unidentifed, e.g. name does not match ID
//==============================================================================

bool LIMSParams::checkMachine()
{
	std::string computer, userName, error;
	wchar_t buffer[ 100 ];
	DWORD size = sizeof( buffer );
	if( GetUserName( buffer, &size ) )
		userName = AnsiString( buffer, size-1 ).c_str();

	size = sizeof( buffer );
	if( GetComputerName( buffer, &size ) )
	{
		wchar_t * dot = wcschr( buffer, '.' );
		if( dot == NULL )
			computer = AnsiString( buffer, size ).c_str();
		else
			computer = AnsiString( buffer, dot - buffer ).c_str();
	}

	LCDbAnalysers & machines = LCDbAnalysers::records();
	const LCDbAnalyser * named = machines.findByName( computer );
	if( named == NULL )
		named = machines.findByName( userName );

	if( named == NULL )
		error = "No database record for " + computer + '/' + userName;

	else if( buddyID != 0 && named -> getID() != buddyID )
		error = "Machine/user name does not match buddy ID";

	const LCDbAnalyser * buddy = machines.findByID( buddyID );
	if( buddy != NULL )
		machines.setCurrent( *buddy );
	else if( named != NULL )
		machines.setCurrent( *named );

	if( error.empty() ) {
		return true;
	}
	String message = error.c_str();
	return Application -> MessageBox( message.c_str(), NULL, MB_OKCANCEL | MB_ICONWARNING ) == IDOK;
}

//==============================================================================
//	Create/modify buddy and cluster IDs in registry for current user
//==============================================================================

void LIMSParams::setBuddyID( int newID )
{
	openSection( "", true );
	setValue( "Buddy ID", newID );
	buddyID = newID;
}

//==============================================================================
//	Remember the current window size and position for next program run
// ============================================================================

void LIMSParams::saveSize( TForm * display )
{
	if( openSection( "window", true ) )
	{
		setValue( "width", display -> Width );
		setValue( "height", display -> Height );
		setValue( "top", display -> Top );
		setValue( "left", display -> Left );
		closeSection();
	}
}

// ============================================================================
//	Re-size or move the given form to match the settings in the .ini file
//==============================================================================

void LIMSParams::restoreSize( TForm * display )
{
	if( openSection( "window", false ) )
	{
		int top = findValue( "top", display -> Top );
		if( top > 10 && top + 50 < Screen -> Height )
			display -> Top = top;

		int left = findValue( "left", display -> Left );
		if( left > 10 && left + 50 < Screen -> Width )
			display -> Left = left;

		int height = findValue( "height", display -> Height );
		if( height > 50 && display -> Top + height < Screen -> Height )
			display -> Height = height;

		int width = findValue( "width", display -> Width );
		if( width > 50 && display -> Left + width < Screen -> Width )
			display -> Width = width;
	}
}

//==============================================================================
//	Open a key in the registry or find a section in the .ini file
//	create == false (default) => open read only, return false if not found
//	create == true => allow read/write, create new section if not found
//==============================================================================

bool LIMSParams::openSection( const std::string & name, bool create )
{
	static String root = "\\Software\\CTSU_CP\\Labs System";
	static String prog = ExtractFileName( Application -> ExeName );

	String path = root + '\\' + prog;
	if( !name.empty() )
		path = path + '\\' + name.c_str();

	regKey -> RootKey = HKEY_CURRENT_USER;
	if( create )
	{
		regKey -> Access = KEY_WRITE;
		inRegistry = regKey -> OpenKey( path, true );
	}
	else if( regKey -> OpenKeyReadOnly( path ) )
		inRegistry = true;
	else
	{	regKey -> RootKey = HKEY_LOCAL_MACHINE;
		inRegistry = regKey -> OpenKeyReadOnly( path );
	}

	section = name;
	return inRegistry;
}

//---------------------------------------------------------------------------

void LIMSParams::closeSection()
{
	section = "";
	regKey -> CloseKey();
}

//---------------------------------------------------------------------------

void LIMSParams::copyValues( TStrings * valueList ) const
{
	if( inRegistry )
		regKey -> GetValueNames( valueList );
	else
		valueList -> Clear();
}

//==============================================================================
//	Read a value from the INI file; return given value if not found
//==============================================================================

int LIMSParams::findValue( const std::string & name, int value ) const
{
	String bcn = name.c_str();
	if( inRegistry && regKey -> ValueExists( bcn ) && regKey -> GetDataType( bcn ) == rdInteger )
		value = regKey -> ReadInteger( bcn );
	return value;
}

//---------------------------------------------------------------------------

std::string LIMSParams::findValue( const std::string & name, std::string value ) const
{
	String bcn = name.c_str();
	if( inRegistry && regKey -> ValueExists( bcn ) && regKey -> GetDataType( bcn ) == rdString )
		value = AnsiString( regKey -> ReadString( bcn ) ).c_str();
	return value;
}

//==============================================================================
//	Add to registry; throws Exception if key not open with write access
//==============================================================================

void LIMSParams::setValue( const std::string & name, int value )
{
	regKey -> WriteInteger( name.c_str(), value );
}

//---------------------------------------------------------------------------

void LIMSParams::setValue( const std::string & name, const std::string & value )
{
	regKey -> WriteString( name.c_str(), value.c_str() );
}

//---------------------------------------------------------------------------

void LIMSParams::removeValue( const std::string & name )
{
	regKey -> DeleteValue( name.c_str() );
}

//==============================================================================
//	Get version number added by C++Builder from the program's .EXE file
//==============================================================================

std::string LIMSParams::getProgVersion() const
{
	String progName = Application -> ExeName;
	char version[ 40 ];
	version[ 0 ] = '\0';
	DWORD zero, size = GetFileVersionInfoSize( progName.c_str(), &zero );
	if( size > 0 )
	{
		char * buffer = new char[ size + 2 ];
		if( GetFileVersionInfo( progName.c_str(), NULL, size, buffer ) )
		{
			LPVOID fileInfo;
			UINT length;
			if( VerQueryValue( buffer, L"\\", &fileInfo, &length ) )
			{
				DWORD msdw = ((VS_FIXEDFILEINFO *) fileInfo) -> dwFileVersionMS;
				int major = msdw >> 16;
				int minor = msdw & 0xFFFF;
				DWORD lsdw = ((VS_FIXEDFILEINFO *) fileInfo) -> dwFileVersionLS;
				int release = lsdw >> 16;
				int build = lsdw & 0xFFFF;
				std::sprintf( version, "%d.%d.%d.%d", major, minor, release, build );
			}
		}
		delete[] buffer;
	}
	return version;
}

//==============================================================================
//	Ask Windows where to put the log files (from the user's profile)
//==============================================================================

std::string LIMSParams::getLogFolder() const
{
	String appsData = getenv( "APPDATA" );
	String progName = ExtractFileName( Application -> ExeName );
	progName.SetLength( progName.Pos( "." ) - 1 );
	AnsiString path = appsData + "\\CTSU_CP\\" + progName + "\\";
	if( !ForceDirectories( path ) )
		throw Exception( "Unable to create log file folder" );
	return path.c_str();
}

//==============================================================================
//	Check if local worklist is used; default to true for compatibility
//==============================================================================

bool LIMSParams::useLocalWorklist() 
{
	return openSection( "worklist", false ) && findValue( "local", 0 ) != 0;
}

//==============================================================================

void LIMSParams::setLocalWorklist( bool useBoth )
{
	openSection( "worklist", true );
	setValue( "local", useBoth );
}

//==============================================================================

