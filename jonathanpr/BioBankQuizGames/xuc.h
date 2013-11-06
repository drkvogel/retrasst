#ifndef xucH
#define xucH
//---------------------------------------------------------------------------
//#ifdef _BORLANDC_
#include <vcl.h>
//#endif
#include <string>
//---------------------------------------------------------------------------
class XUC
{
private:
	XUC( void );
	~XUC( void );
public:
#ifdef __BORLANDC__
	static	String stds2cbs( const std::string in );
	static	std::string cbs2stds( const String in );
	static	wchar_t *char2wchar( const char *source );
	static	int	amsg( const std::string msg, const std::string title,
		const int opts );
#endif
};
//---------------------------------------------------------------------------
#define	AMSG	XUC::amsg
#endif

