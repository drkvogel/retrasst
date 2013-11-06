//---------------------------------------------------------------------------
#ifndef blowfishH
#define blowfishH
//---------------------------------------------------------------------------
/*********************blowfish.h********************/

/* $Id: blowfish.h,v 1.1 2013/09/20 15:45:44 jprice Exp $*/

#define MAXKEYBYTES 56		/* 448 bits */
#define bf_N             16
#define noErr            0
#define DATAERROR         -1
#define KEYBYTES         8
#define subkeyfilename   "Blowfish.dat"

/* ORIGINAL VALUES
#define UWORD_32bits  unsigned long
#define UWORD_16bits  unsigned short
#define UBYTE_08bits  unsigned char
*/

/* choose a byte order for your hardware */
#ifdef __BORLANDC__
#define	ORDER_DCBA	// AY
#define UWORD_32bits  unsigned __int32
#define UWORD_16bits  unsigned __int16
#define UBYTE_08bits  unsigned char
#endif


/* ABCD - big endian - motorola */
#ifdef ORDER_ABCD
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte0:8;
    unsigned int byte1:8;
    unsigned int byte2:8;
    unsigned int byte3:8;
  } w;
};
#endif	/* ORDER_ABCD */

/* DCBA - little endian - intel */
#ifdef ORDER_DCBA
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte3:8;
    unsigned int byte2:8;
    unsigned int byte1:8;
    unsigned int byte0:8;
  } w;
};
#endif	/* ORDER_DCBA */

/* BADC - vax */
#ifdef ORDER_BADC
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte1:8;
    unsigned int byte0:8;
    unsigned int byte3:8;
    unsigned int byte2:8;
  } w;
};
#endif	/* ORDER_BADC */


/*
short	opensubkeyfile(void);
UWORD_32bits F( UWORD_32bits x );
*/
//---------------------------------------------------------------------------
class BLOWFISH
{
private:
	static	char 	hexval[17];
	static 	UWORD_32bits 	bf_P_init[bf_N + 2];
	static 	UWORD_32bits 	bf_S_init[4][256];
		UWORD_32bits 	bf_P[bf_N + 2];
		UWORD_32bits 	bf_S[4][256];
	int 	hex_value( unsigned char c );
	bool   	asc_to_hex( char *out, const char *in, int length );
	bool 	hex_to_asc( char *out, int *length, const char *in );
protected:
	void 	init_local_sboxes( void );
	short	InitializeBlowfish( unsigned char *key, short keybytes );
	void	Blowfish_encipher( UWORD_32bits *xl, UWORD_32bits *xr );
	void	Blowfish_decipher( UWORD_32bits *xl, UWORD_32bits *xr );
public:
	BLOWFISH( unsigned char *key, int keybytes );
	BLOWFISH( char *key_text );
	~BLOWFISH( void );
	void 	encrypt( char **out, int *new_len, const char *in );
	void 	decrypt( char *out, int len );
	bool 	encrypt_validate( char **out, int *new_len, const char *in );
	char 	*hex_encrypt( const char *in );
	char 	*hex_decrypt( const char *in );
	char 	*base64_encrypt( const char *in );
	char 	*base64_decrypt( const char *in );
};
//---------------------------------------------------------------------------
#endif
