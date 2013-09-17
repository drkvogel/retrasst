#ifndef RIJNDAEL_H_INCLUDED
#define RIJNDAEL_H_INCLUDED 1
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string>
//---------------------------------------------------------------------------
class RIJNDAEL
{
private:
        std::string     last_error;
	bool	        valid;
	void	*ctx_e_buffer; 		// ENCRYPTION BUFFER
	void	*ctx_d_buffer;		// DECRYPTION BUFFER
protected:
        void InitializeRijndael( const char *original_key, const int keybytes );
public:
	RIJNDAEL( const char *key, const int keybytes);
	RIJNDAEL( const char *key_text);
	~RIJNDAEL( void );

	bool 	isValid( void ) const;
        bool 	encrypt( unsigned char **enc, int *enc_len,
			unsigned char const *clr, const int len_clr );
	bool	decrypt( unsigned char **clr, int *len_clr,
			unsigned const char *enc, const int len_enc);
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
        const std::string getLastError( void );
};
//---------------------------------------------------------------------------
#endif
