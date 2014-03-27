#include "xerror.h"
//===========================================================================
bool XERROR::use_exceptions = false;
XERROR_CALLBACK	XERROR::errorCallBack = NULL;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XERROR::XERROR( void )
	:
	creator( "XERROR" )
{
	setValid();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XERROR::XERROR( const std::string owner_label )
	:
	creator( owner_label )
{
	setValid();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XERROR::~XERROR( void )
{
}
//---------------------------------------------------------------------------
void XERROR::useExceptions( const bool ue )
{	use_exceptions = ue;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XERROR::usingExceptions( void )
{	return( use_exceptions );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XERROR::setErrorCallBack( XERROR_CALLBACK ec )
{
	errorCallBack = ec;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XERROR::setInvalid( const std::string emsg ) const
{
	is_valid = false;
	error_msgs.push_back( emsg );
	if ( NULL != errorCallBack )
		{try
			{errorCallBack( creator, emsg );
			}
		catch( ... )
			{throw( std::string( "Failed XERROR::errorCallBack" ) );
			}
		}
	if ( usingExceptions() )
		{std::string e = creator + std::string(" exception: ") + emsg;
		throw( e );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XERROR::setCaveat( const std::string cmsg ) const
{
	caveat_msgs.push_back( cmsg );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XERROR::setValid( void )
{
	is_valid = true;		// DO NOT CLEAR ERROR HISTORY
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XERROR::isValid( void ) const
{
	return( is_valid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XERROR::hasCaveats( void ) const
{
	return( caveat_msgs.size() > 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XERROR::getNErrors( void ) const
{
	return( error_msgs.size() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XERROR::getNCaveats( void ) const
{
	return( caveat_msgs.size() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XERROR::getLastError( void ) const
{
	int	ne = getNErrors();
	return( ( ne > 0 ) ? error_msgs[ne-1] : std::string("") );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XERROR::getLastCaveat( void ) const
{
	int	nc = getNCaveats();
	return( ( nc > 0 ) ? caveat_msgs[nc-1] : std::string("") );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::vector<std::string> *XERROR::getErrorsPtr( void ) const
{
	return( &error_msgs );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::vector<std::string> *XERROR::getCaveatsPtr( void ) const
{
	return( &caveat_msgs );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XERROR::getCreatorName( void ) const
{
	return( creator );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XERROR::resetXError( void )
{
	setValid();
	error_msgs.clear();
	caveat_msgs.clear();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef __BORLANDC__
#include <strsafe.h>
std::string XERROR::translateMSError( const DWORD last_error )
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		last_error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	std::string	msg = std::string( (const char *) lpMsgBuf );
	LocalFree(lpMsgBuf);
	return( msg );

}
#endif
//===========================================================================



