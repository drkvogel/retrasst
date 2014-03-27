#ifndef usc_ldapH
#define usc_ldapH
//---------------------------------------------------------------------------
#include "usecure.h"
//---------------------------------------------------------------------------
class	LdapUser;
//---------------------------------------------------------------------------
class USECURE_LDAP : public USECURE
{			// THIN WRAPPER TO DISGUISE LDAP FROM NON-PC CODE
private:
	LdapUser	*ldu;
public:
	USECURE_LDAP( void );
	virtual	~USECURE_LDAP( void );
	virtual	std::string	initialize( void );
	virtual	std::string	authenticate( const std::string userID,
		const std::string password );
	virtual	std::string	authenticate( const std::string password );
	virtual	std::string  	existsUser( const std::string knownUserID,
		const std::string knownPassword, const std::string userID );
};
//---------------------------------------------------------------------------
#endif

