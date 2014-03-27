#include "ldap_unx.h"
#include <stdio.h>
	/* DERIVED FROM CODE SUPPLIED BY PETE HARDING */
	//const char *LDAP_UNX::server_address = "ldap://10.1.6.215";
//=============================================================================
LDAP_UNX::LDAP_UNX( void )
	:
	server_address( "10.1.6.7" ),
	ou( "OU=_allusers" ),
	dc( "DC=ctsu,DC=ox,DC=ac,DC=uk" ),
	ldap_pw( "t0m,hp1,hpkk0mt" )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
LDAP_UNX::~LDAP_UNX()
{
}
//-----------------------------------------------------------------------------
std::string LDAP_UNX::initialize( LDAP **ld )
{  			// (should use ldaps but PH couldn't get that to work)
	const	std::string  server = std::string( "ldap://" ) + server_address;
	const	int	iinit = ldap_initialize( ld, server.c_str() );
	if ( iinit != LDAP_SUCCESS )
		{sprintf( ebuf, "ldap_initialize=%d", iinit );
		return( std::string(ebuf) );
		}
	int	iver = LDAP_VERSION3;
	const	int	iopt = ldap_set_option( *ld, LDAP_OPT_PROTOCOL_VERSION,
		&iver );
	if ( iopt != LDAP_SUCCESS )
		{sprintf( ebuf, "ldap_set_option=%d", iopt );
		return( std::string(ebuf) );
		}
	return( "" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string LDAP_UNX::getDistinguishedName( const std::string userID,
	std::string *dname )
{
	*dname = "";
	LDAP *ldap = NULL;	// INITIALISE AND CONNECT TO LDAP SERVER
	std::string	e = initialize( &ldap );
	if ( ! e.empty() )
		{return( e );
		}
	std::string	base = ou + "," + dc;
	std::string	bparam = std::string("CN=LDAPbind,") + base;
	const	int 	ibind = ldap_simple_bind_s( ldap, bparam.c_str(),
		ldap_pw.c_str() );
	if ( ibind != LDAP_SUCCESS )
		{sprintf( ebuf, "ldap_simple_bind_s=%d", ibind );
		return( std::string(ebuf) );
		}
/* SEARCH FOR ENTRY WITH sAMAccountName AS GIVEN BY USER AND GET
 distinguishedName IF IT EXISTS (IF FOUND, THEN USERNAME EXISTS) */
	LDAPMessage *res = ( LDAPMessage * )NULL;
	std::string filter = "(sAMAccountName=" + userID + ")";
	const	int	isearch = ldap_search_s( ldap, base.c_str(),
		LDAP_SCOPE_ONELEVEL, filter.c_str(), ( char ** )NULL, 0, &res );
	if ( isearch != LDAP_SUCCESS )
		{sprintf( ebuf, "ldap_search_s=%d", isearch );
		return( std::string(ebuf) );
		}
	const	int	nentries = ldap_count_entries( ldap, res );
	if ( nentries <= 0 )
		{    // FAILED TO FIND distinguishedName from sAMAccountName
		ldap_msgfree( res );
		ldap_unbind_s( ldap );
		return( "Username not found in LDAP-OU" );
		}
					    // EXTRACT distinguishedNAme
	char 	*dn = ldap_get_dn( ldap, ldap_first_entry( ldap, res ) );
	*dname = std::string( dn );
	ldap_memfree( dn );			    		// TIDY UP
	ldap_msgfree( res );
	ldap_unbind_s( ldap );   			// DROP ASSOCIATION
	return( "" );
 }
 //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string LDAP_UNX::existsUser( const std::string userID )
{
	std::string	dist_name;
	return( getDistinguishedName( userID, &dist_name ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string LDAP_UNX::authenticate( const std::string userID,
	const std::string password )
{
	std::string	dist_name;
	std::string	e = getDistinguishedName( userID, &dist_name );
	if ( ! e.empty() )
		{return( e );
		}
	LDAP *ldap = NULL;	// INITIALISE AND CONNECT TO LDAP SERVER
	e = initialize( &ldap );
	if ( ! e.empty() )
		{return( e );
		}
		// ATTEMPT TO BIND distinguishedName WITH PASSWORD GIVEN
	const	int	ibind = ldap_simple_bind_s( ldap, dist_name.c_str(),
		password.c_str() );
	ldap_unbind_s( ldap );
	return( ( LDAP_SUCCESS == ibind )
		?  "" : "Username/password mis-match" );
}
//=============================================================================
