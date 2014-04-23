#ifndef MD5_INCLUDED_H
#define MD5_INCLUDED_H
/*===========================================================================*/
#define	MD5_KEYLENGTH	33	/* (2*16)+1	*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef unsigned char md5_byte_t; /* 8-bit byte */
typedef unsigned int  md5_word_t; /* 32-bit word */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef struct
	{
	md5_word_t count[2];	/* message length in bits, lsw first */
	md5_word_t abcd[4];		/* digest buffer */
	md5_byte_t buf[64];		/* accumulate block */
	}
	md5_state_t;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	md5_byte_t 	digest[16];
	char		hex[MD5_KEYLENGTH];
	}
	XMD5_DIGEST;
/*===========================================================================*/
#include <string>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
class XMD5
{
private:
	int	nchunks;
	md5_state_t lump_state;
	static	void	init( md5_state_t *pms );
	static	void 	process( md5_state_t *pms, const md5_byte_t *data );
	static	void	append( md5_state_t *pms, const md5_byte_t *data, int nbytes );
	static	void	finish( md5_state_t *pms, md5_byte_t digest[16] );
	static	bool 	digest2hex( XMD5_DIGEST *m );
public:
	XMD5( void );
	~XMD5( void );
	void	reset( void );
	bool	addChunk( const unsigned char *buf, const int len );
	std::string	getResultHex( void );
	static	bool 	calc( XMD5_DIGEST *m, const char *buf, const int len );
	static	bool 	calcSz( XMD5_DIGEST *m, const char *txt );
	static	std::string 	calcHex( const char *txt );
	static	std::string 	calcHex( const unsigned char *buf, const int len);
	static	std::string 	calcFileHex( const std::string filename );
};
/*===========================================================================*/
#endif 								// XMD5.H
