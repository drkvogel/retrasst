#ifndef XAES_H_INCLUDED
#define XAES_H_INCLUDED 1
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string>
#include "xerror.h"
#include "xbinob.h"
//---------------------------------------------------------------------------
class XAES : public XERROR
{
private:
	void	*ctx_e_buffer; 		// ENCRYPTION BUFFER
	void	*ctx_d_buffer;		// DECRYPTION BUFFER
	static	bool	first;
	static	unsigned long idum;
	static	unsigned char	*iv;
	void	fillrand( unsigned char *buf );
protected:
	void InitializeRijndael( const std::string hex_key );
public:
	XAES( const std::string hex_key );
	~XAES( void );
	bool	setInitVect( const std::string hex_iv );
	bool 	encrypt( unsigned char **enc, int *enc_len,
			unsigned char const *clr, const int len_clr );
	bool 	encrypt( XBINOB *enc, const XBINOB *clr );
	bool	decrypt( unsigned char **clr, int *len_clr,
			unsigned const char *enc, const int len_enc);
	bool	decrypt( XBINOB *clr, const XBINOB *enc );
	bool 	encryptValidate( unsigned char **enc, int *len_enc,
			const unsigned char *clr, const int len_clr );
	bool 	encryptSz( unsigned char **enc, int *len_enc, const char *sz );
	char 	*encryptBuf2Hex( const char *buf, const int len_buf );
	char 	*encryptSz2Hex( const char *sz );
	bool 	decryptHex( unsigned char **clr, int *len_clr,
			const char *hex );
	char 	*encryptBuf2Base64( const char *buf, const int len_buf );
	char 	*encryptSz2Base64( const char *sz );
	bool 	decryptBase64( unsigned char **clr, int *len_clr,
			const char *b64 );
	bool    encryptFile( FILE *fin, FILE *fout );
	bool    decryptFile( FILE *fin, FILE *fout );
	static	std::string 	bin2HexKey( const unsigned char *bin,
		const int len );
	static	std::string 	string2HexKey( const std::string text );
};
//---------------------------------------------------------------------------
#endif
