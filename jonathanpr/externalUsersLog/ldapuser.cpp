#include "LdapUser.h"
#include <adserr.h>
#include <adshlp.h>

LdapUser::LdapUser()
{

}

LdapUser::~LdapUser()
{
	::CoUninitialize();
}

//Initialize COM library
std::wstring  LdapUser::winitialize( const std::wstring serverURL )
{
	std::wstring result = L"";
	// Initialize COM.
	hr = ::CoInitialize(0);
	if (hr != S_OK && hr != S_FALSE ) {
		result = L"Error initializing COM";
		return result;
	}

	ldapServerURL = serverURL;
	wtrim(ldapServerURL);
	if (ldapServerURL.length() == 0 ) {
		ldapServerURL = L"LDAP://rootDSE";
	}

	return result;
}

std::string  LdapUser::initialize( const std::string serverURL )
{
	std::string result = "";
	std::wstring url;

	string2wstring( url, serverURL );
	std::wstring wresult = winitialize( url );
	wstring2string( result, wresult );

	return result;
}

//Open a connection to LDAP server with passed userid and password
std::wstring  LdapUser::wauthenticate( const std::wstring userID, const std::wstring password )
{
	IADs *pObject = NULL;

	//Check if the passed parameters are non-blank.
	std::wstring wuid = userID;
	std::wstring wpwd = password;

	wtrim( wuid );
	if ( wuid.length() == 0 )
	{
		error = L"User ID is blank";
		return error;
	}

	wtrim( wpwd );
	if ( wpwd.length() == 0 )
	{
		error = L"Password is blank";
		return error;
	}

	hr = S_OK;
	hr = ADsOpenObject(ldapServerURL.c_str(),
					   wuid.c_str(),
					   wpwd.c_str(),
					   ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
					   IID_IADs,
					   (void**)&pObject);

	getLastError();
	if (pObject)
		pObject->Release();

	if ( error.length() == 0 )
	{//Authentication successful
		this->userID = wuid;
	}
	return error;
}

std::string  LdapUser::authenticate( const std::string userID, const std::string password )
{
	std::string result = "";
	std::wstring wuid, wpwd;

	string2wstring( wuid, userID );
	string2wstring( wpwd, password );
	std::wstring wresult = wauthenticate( wuid, wpwd );
	wstring2string( result, wresult );

	return result;
}

//Open a connection to LDAP server with password only. Use cached userid.
std::wstring  LdapUser::wauthenticate( const std::wstring password )
{
	//Check if the passed parameters are non-blank.
	if ( this->userID.length() == 0 )
	{
		error = L"Cannot authenticate without userID";
		return error;
	}

	return wauthenticate( this->userID, password );
}

std::string  LdapUser::authenticate( const std::string password )
{
	std::string result = "";
	std::wstring wpwd;

	string2wstring( wpwd, password );
	std::wstring wresult = wauthenticate( wpwd );
	wstring2string( result, wresult );

	return result;
}

std::wstring LdapUser::wgetUserID()
{
	return this->userID;
}

std::string  LdapUser::getUserID()
{
	std::string uid;
	wstring2string( uid, this->userID );
	return uid;
}

std::wstring LdapUser::wgetServerURL()
{
	return this->ldapServerURL;
}

std::string  LdapUser::getServerURL()
{
	std::string url;
	wstring2string( url, this->ldapServerURL );
	return url;
}

void LdapUser::wtrim(std::wstring& str)
{
	int pos = str.length();
	int i;
	//right trim
	for (i = pos - 1; i >= 0; i--)
	{
		if (str.at(i) != L' ' )
			break;
	}

	str.erase(i+1, str.length());

	//left trim
	for (i = 0; i < (int)str.length(); i++)
	{
		if (str.at(i) != L' ' )
			break;
	}

	str.erase(0, i );
}

void LdapUser::getLastError()
{
	error = L"";
	if (SUCCEEDED(hr))
		return;

	switch( hr )
	{
		case E_ADS_BAD_PATHNAME:
			error = L"An invalid directory pathname was passed";
			break;

		case E_ADS_INVALID_DOMAIN_OBJECT:
			error = L"An unknown directory domain object was requested";
			break;

		case E_ADS_INVALID_USER_OBJECT:
			error = L"An unknown directory user object was requested";
			break;

		case E_ADS_UNKNOWN_OBJECT:
			error = L"An unknown directory object was requested";
			break;

		case E_ADS_PROPERTY_NOT_SET:
			error = L"The specified directory property was not set";
			break;

		case E_ADS_PROPERTY_NOT_SUPPORTED:
			error = L"The specified directory property is not supported";
			break;

		case E_ADS_PROPERTY_INVALID:
			error = L"The specified directory property is invalid";
			break;

		case E_ADS_BAD_PARAMETER:
			error = L"One or more input parameters are invalid";
			break;

		case E_ADS_OBJECT_UNBOUND:
			error = L"The specified directory object is not bound to a remote resource";
			break;

		case E_ADS_PROPERTY_NOT_MODIFIED:
			error = L"The specified directory object has not been modified";
			break;

		case E_ADS_PROPERTY_MODIFIED:
			error = L"The specified directory object has been modified";
			break;

		case E_ADS_CANT_CONVERT_DATATYPE:
			error = L"The directory datatype cannot be converted to/from a native DS datatype";
			break;

		case E_ADS_PROPERTY_NOT_FOUND:
			error = L"The directory property cannot be found in the cache";
			break;

		case E_ADS_OBJECT_EXISTS:
			error = L"The directory object exists";
			break;

		case E_ADS_SCHEMA_VIOLATION:
			error = L"The attempted action violates the DS schema rules";
			break;

		case S_ADS_ERRORSOCCURRED:
			error = L"One or more errors occurred";
			break;

		case E_ADS_INVALID_FILTER:
			error = L"The search filter specified is invalid";
			break;

		default:
			error = L"Authentication Failed";
			break;
	}
}

std::wstring LdapUser::wexistsUser( const std::wstring knownUserID, const std::wstring knownPassword, const std::wstring userID )
{
	IADs *pObject = NULL;
	IDirectorySearch *pDS = NULL;
	//Check if the passed parameters are non-blank.
	std::wstring wknownUID = knownUserID;
	std::wstring wknownPWD = knownPassword;
	std::wstring wuid = userID;

	wtrim( wknownUID );
	if ( wknownUID.length() == 0 )
	{
		error = L"Known User ID is blank";
		return error;
	}

	wtrim( wknownPWD );
	if ( wknownPWD.length() == 0 )
	{
		error = L"Known Password is blank";
		return error;
	}

	wtrim( wuid );
	if ( wuid.length() == 0 )
	{
		error = L"User ID is blank";
		return error;
	}
	hr = S_OK;
	//if ldap url is not specified as "LDAP://rootDSE" then pObject->Get( L"defaultNamingContext", &var ) below fails
	hr = ADsOpenObject(L"LDAP://rootDSE",
					   wknownUID.c_str(),  //Use credentials of existing user
					   wknownPWD.c_str(),
					   ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
					   IID_IADs,
					   (void**)&pObject);

	if ( FAILED(hr) )
	{
		getLastError();
		if ( pObject )
			pObject->Release();
		return error;
	}

	//Get proper url of ldap server. LDAP://rootDSE doesn't allow binding to IID_IDirectorySearch
	wchar_t szPath[MAX_PATH] ;
	VARIANT var;
	VariantInit( &var ) ;

	hr = pObject->Get( L"defaultNamingContext", &var );
	if ( pObject )
	{
		pObject->Release();
		pObject = NULL;
	}
	if ( FAILED(hr) )
	{
		getLastError();
		return error;
	}

	wcscpy( szPath, L"LDAP://" ) ;
	wcscat( szPath, var.bstrVal ) ;
	szPath[MAX_PATH-1] = 0 ;
	VariantClear(&var);
//-----------------------------------------------------------------------------------
	//Bind to the root of the current domain.
	hr = ADsOpenObject(szPath,
					   wknownUID.c_str(),
					   wknownPWD.c_str(),
					   ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
					   IID_IDirectorySearch,
					   (void**)&pDS);
	if ( FAILED(hr) )
	{
		getLastError();
		if ( pDS )
			pDS->Release();
		return error;
	}

	wchar_t pszSearchFilter[MAX_PATH] ;
	ADS_SEARCHPREF_INFO searchPrefs ;
	ADS_SEARCH_HANDLE hSearch ;
	LPWSTR pszAttribute[1] = { L"ADsPath"} ;

	// build our search filter. The username (sAMAccountName) is unique throughout the domain
	wcscpy(pszSearchFilter, L"(&(objectClass=person)(sAMAccountName=" );
	wcscat(pszSearchFilter, wuid.c_str() );
	wcscat(pszSearchFilter, L"))" );

	ZeroMemory(&searchPrefs, sizeof( searchPrefs ) ) ;
	searchPrefs.dwSearchPref = ADS_SEARCHPREF_SEARCH_SCOPE ;
	searchPrefs.vValue.dwType = ADSTYPE_INTEGER ;
	searchPrefs.vValue.Integer = ADS_SCOPE_SUBTREE ;

	hr = pDS->SetSearchPreference( &searchPrefs, 1 ) ;
	if ( FAILED(hr) )
	{
		getLastError();
		if ( pDS )
			pDS->Release();
		return error;
	}

	hr = pDS->ExecuteSearch( pszSearchFilter, pszAttribute, 1, &hSearch ) ;
	if ( FAILED(hr) )
	{
		getLastError();
		if ( pDS )
			pDS->Release();
		return error;
	}

	if ( S_ADS_NOMORE_ROWS != pDS->GetNextRow( hSearch ) )
	{//User found
		pDS->CloseSearchHandle( hSearch ) ;
	}
	else
	{
		pDS->CloseSearchHandle( hSearch ) ;
		error = L"User not found";
		return error;
	}

	if ( pDS )
		pDS->Release();

	return error;
}

std::string LdapUser::existsUser( const std::string knownUserID, const std::string knownPassword, const std::string userID )
{
	std::string result = "";
	std::wstring wknownUserID;
	std::wstring wknownPassword;
	std::wstring wuid;

	string2wstring( wknownUserID, knownUserID );
	string2wstring( wknownPassword, knownPassword );
	string2wstring( wuid, userID );
	std::wstring wresult = wexistsUser( wknownUserID, wknownPassword, wuid );
	wstring2string( result, wresult );

	return result;
}

void LdapUser::string2wstring(std::wstring& dest, const std::string source)
{
	char temp[MAX_PATH];
	wchar_t wtemp[MAX_PATH];

	strcpy(temp, source.c_str());
	mbstowcs(wtemp, temp, strlen(temp) + 1 );
	dest = wtemp;
}

void LdapUser::wstring2string(std::string& dest, const std::wstring source)
{
	char temp[MAX_PATH];
	wchar_t wtemp[MAX_PATH];

	wcscpy(wtemp, source.c_str());
	wcstombs(temp, wtemp, wcslen(wtemp) + 1 );
	dest = temp;
}


