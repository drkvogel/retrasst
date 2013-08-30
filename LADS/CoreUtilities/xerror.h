#ifndef xerrorH
#define xerrorH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#ifdef __BORLANDC__
#include <windows.h>
#endif
//---------------------------------------------------------------------------
typedef	void (*XERROR_CALLBACK)
	( const std::string object, const std::string error_txt );
//---------------------------------------------------------------------------
class XERROR
{
private:
	std::string	creator;
	static	bool 	use_exceptions;
	mutable	bool	is_valid;
	mutable	std::vector<std::string>	error_msgs;
	mutable	std::vector<std::string>	caveat_msgs;
protected:
	XERROR( void );
	XERROR( const std::string owner_label );
	virtual	~XERROR( void );
public:
	std::string	getCreatorName( void ) const;
	void 	resetXError( void );
	virtual	bool	isValid( void ) const;
	virtual	bool 	hasCaveats( void ) const;
	virtual	int 	getNErrors(  void ) const;
	virtual	int 	getNCaveats( void ) const;
	virtual	std::string	getLastError(  void ) const;
	virtual	std::string	getLastCaveat( void ) const;
	const	std::vector<std::string>	*getErrorsPtr( void ) const;
	const	std::vector<std::string>	*getCaveatsPtr( void ) const;
	static	XERROR_CALLBACK	errorCallBack;
	static	void 	setErrorCallBack( XERROR_CALLBACK ec );
	virtual	void 	setInvalid( const std::string emsg ) const;
	virtual	void 	setCaveat( const std::string cmsg ) const;
	virtual	void 	setValid( void );
	static	void	useExceptions( const bool ue );
	static	bool 	usingExceptions( void );
#ifdef __BORLANDC__
	static	std::string 	translateMSError( const DWORD last_error );
#endif
};
//---------------------------------------------------------------------------
#endif

