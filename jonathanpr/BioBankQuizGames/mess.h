#ifndef messH
#define messH
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
class MESS
{
private:
	static	std::string	staff_name;
	static	std::string	person_name;
	static	std::string	logfile_name;
	static	bool	noise_enabled;
public:
	static	void	initLogfile( const std::string directory,
			const std::string prefix );
	static	int	amsg( const std::string txt, const std::string title,
			const int but_code );
	static	void	bug( const std::string txt );
	static	void	bleep( const bool forced );
	static	void	aside( const char *txt, ...  );
	static	int 	error(   const std::string txt, const int but_code );
	static	int 	warning( const std::string txt, const int but_code );
	static	int 	check(   const std::string txt, const int but_code );
	static	int 	decide(  const std::string txt, const int but_code );
	static	int 	info(    const std::string txt, const int but_code );
	static	int 	action(  const std::string txt, const int but_code );
	static	void	personaliseText( std::string *txt );
};
//---------------------------------------------------------------------------
#endif

