#ifndef _DEFINED_LDAP_USER_H
#define _DEFINED_LDAP_USER_H

#include <string>
#include <iads.h>

//Link "activeds.lib" from "<C++ Builder Installation Dir>\lib\PSDK"
class LdapUser
{
	public:
		//Constructor
		LdapUser();
		//Destructor
		virtual ~LdapUser();
		//winitialize and initialize: Initializes COM library
		//Parameter: serverURL - URL of Active Directory Server. If not supplied LDAP://rootDSE is assumed
		std::wstring  winitialize( const std::wstring serverURL );
		std::string   initialize( const std::string serverURL );
		//wauthenticate and authenticate: Authenticates a user
		//Parameter: userID - User ID of the user to be authenticated
		//           password - Password of the user to be authenticated
		//Returns: Result string containing error message if any. If the returned string has
		//         zero length (blank string) then the user authentication is successful.
		std::wstring  wauthenticate( const std::wstring userID, const std::wstring password );
		std::string   authenticate( const std::string userID, const std::string password );

		//wauthenticate and authenticate: Authenticates a user
		//Parameter: userID - This is not required. It is assumed that the user ID is the same
		//                    as that used in the above call. The class stores the user ID
		//                    supplied in the above call if authentication is successful and uses it in this call.
		//                    The user ID would be available as long as the instance of this class is alive.
		//           password - Password of the user to be authenticated. Password is not stored, should be
		//Returns: Result string containing error message if any. If the returned string has
		//         zero length (blank string) then the user authentication is successful.
		std::wstring  wauthenticate( const std::wstring password );
		std::string   authenticate( const std::string password );

		//wgetUserID and getUserID: Returns user ID stored in this object
		std::wstring wgetUserID();
		std::string  getUserID();

		//wgetServerURL and getServerURL: Returns LDAP Server URL stored in this object
		std::wstring wgetServerURL();
		std::string  getServerURL();

		//wexistsUser and existsUser: Checks if the passed user id exists in LDAP directory
		//Parameter: knownUserID - User ID of existing user
		//           knownPassword - Password of existing user
		//           userID - User ID of the user to be checked
		//Returns: Result string containing error message if any. If the returned string has
		//         zero length (blank string) then the user exists in LDAP directory
		std::wstring wexistsUser( const std::wstring knownUserID, const std::wstring knownPassword, const std::wstring userID );
		std::string  existsUser( const std::string knownUserID, const std::string knownPassword, const std::string userID );

		//helper methods for converting between std::wstring and std::string
		void string2wstring(std::wstring& dest, const std::string source);
		void wstring2string(std::string& dest, const std::wstring source);

	private:
		std::wstring ldapServerURL;
		std::wstring error;
		HRESULT hr;
		std::wstring userID;
		void getLastError();
		void wtrim(std::wstring& str);
};

#endif // _DEFINED_LDAP_USER_H
