#ifndef XENCODE_H_DEFINED
#define XENCODE_H_DEFINED	1
/*===========================================================================*/
/*			BASE-64 + HEX ENCODE AND DECODE			*/
/*===========================================================================*/
#include <string>
//---------------------------------------------------------------------------/
class XENCODE
{
private:
	static 	const 	char 	trans[];
	static 	bool	b64_chunking;
	static	std::string	last_error;
public:
	static	std::string getLastError( void );
	static 	void 	b64ChunkingSet( const bool do_chunking );
	static 	bool 	binToB64( const	unsigned char *dat, const int len_dat,
				char **b64, int *len_b64 );
	static	bool 	binToB64( const unsigned char *dat, const int len_dat,
				std::string *b64 );
	static	bool	szToB64( const std::string sz, std::string *b64 );
	static	bool 	b64ToBin( const char *b64, const int len_b64,
				unsigned char **dat, int *len_dat );
	static	bool 	b64ToBin( const char *b64,
				unsigned char **dat, int *len_dat );
	static	bool 	b64ToBin( const std::string b64,
				unsigned char **dat, int *len_dat );
	static	bool 	b64ToSz( const std::string b64, std::string *sz );
	static	bool 	b2h( const unsigned char b, char *h2 );
	static	bool	binToHex( const unsigned char *dat, const int len_dat,
				char **hex, int *len_hex );
	static	bool	binToHex( const unsigned char *dat, const int len_dat,
				std::string *hex );
	static	bool 	szToHex( const std::string sz, std::string *hex );
	static	bool 	h2b( const char *h2, unsigned char *bin );
	static	bool 	hexToBin( const char *hex, const int len_hex,
				unsigned char **dat, int *len_dat );
	static	bool 	hexToBin( const char *hex,
				unsigned char **dat, int *len_dat );
	static	bool 	hexToBin( const std::string &hex,
				unsigned char **dat, int *len_dat );
	static	bool 	hexToSz( const std::string hex, std::string *sz );
	static	bool	binToUrl( const char *dat, const int len_dat,
		std::string *url );
	static	bool 	isHex( const std::string ihx );
};
/*===========================================================================*/
#endif

