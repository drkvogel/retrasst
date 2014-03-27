#include "usc_ldap.h"
#include "ldapuser.h"
//---------------------------------------------------------------------------
USECURE_LDAP::USECURE_LDAP( void )
	:
	USECURE(),
	ldu( NULL )
{
	ldu = new LdapUser();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
USECURE_LDAP::~USECURE_LDAP( void )
{
	delete	ldu;
}
//---------------------------------------------------------------------------
std::string USECURE_LDAP::initialize( void )
{
	return( ldu->initialize( "LDAP://rootDSE" ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string USECURE_LDAP::authenticate( const std::string userID,
	const std::string password )
{
	std::string	s = ldu->authenticate( userID, password );
	if ( s.empty() )
		{last_success = userID;
		}
	return( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string USECURE_LDAP::authenticate( const std::string password )
{
	return( ldu->authenticate( password ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string  USECURE_LDAP::existsUser( const std::string knownUserID,
	const std::string knownPassword, const std::string userID )
{
	return( ldu->existsUser( knownUserID, knownPassword, userID ) );
}
//---------------------------------------------------------------------------

