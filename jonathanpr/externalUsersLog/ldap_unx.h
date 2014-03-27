#ifndef LDAP_UNX_H_INCLUDED
#define LDAP_UNX_H_INCLUDED	1
//=============================================================================
#include <string>
#define LDAP_DEPRECATED 1
#include <ldap.h>
//-----------------------------------------------------------------------------
class LDAP_UNX
{
private:
	char	ebuf[100];
	std::string	ldap_pw;
	std::string 	initialize( LDAP **ld );
public:
	LDAP_UNX( void );
	~LDAP_UNX( void );
	std::string 	server_address;
	std::string	ou;
	std::string	dc;
	std::string 	getDistinguishedName( const std::string userID,
		std::string *dname );
	std::string	existsUser( const std::string userID );
	std::string	authenticate( const std::string userID,
		const std::string password );
};
//=============================================================================
#endif



