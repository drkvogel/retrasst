/* THIS PROGRAM IS A MODIFIED VERSION OF THE aesxam.c PROGRAM IN
 * DR. BRIAN GLADMAN'S AES IMPLEMENTATION (25-08-2005 VERSION, SEE
 * http://fp.gladman.plus.com/cryptography_technology/rijndael/).
 * IT IMPLEMENTS AES FOR STRING/FILE ENCRYPTION USING CBC MODE WITH CIPHERTEXT
 * STEALING WHEN THE STRING/FILE LENGTH IS GREATER THAN ONE BLOCK (16 BYTES).
 * HERE IS A LIST OF THE CHANGES MADE:
 * 1. ADAPT THE ENCRYPTION AND DECRYPTION FUNCTIONS TO ACCEPT STRINGS
 * AS INPUTS. (aesxam.c ONLY TAKES FILES AS INPUT AND OUTPUTS TO FILES.)
 * 2. MODIFY THE get_rdtsc ROUTINE SO THAT IT IS NOT ARCHITECTURALLY DEPENDENT ON
 * INTEL PROCESSORS. THREE TYPES OF OPERATING SYSTEMS, WINDOWS, LINUX, AND VMS,
 * ARE SUPPORTED IN THE CURRENT PROGRAM.
 */

 /* this is the original text*/
/*
 ---------------------------------------------------------------------------
 Copyright (c) 2003, Dr Brian Gladman, Worcester, UK.   All rights reserved.

 LICENSE TERMS

 The free distribution and use of this software in both source and binary
 form is allowed (with or without changes) provided that:

   1. distributions of this source code include the above copyright
      notice, this list of conditions and the following disclaimer;

   2. distributions in binary form include the above copyright
      notice, this list of conditions and the following disclaimer
      in the documentation and/or other associated materials;

   3. the copyright holder's name is not used to endorse products
      built using this software without specific written permission.

 ALTERNATIVELY, provided that this notice is retained in full, this product
 may be distributed under the terms of the GNU General Public License (GPL),
 in which case the provisions of the GPL apply INSTEAD OF those given above.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue 01/08/2005
*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifdef __BORLANDC__
#include <mem.h>
#endif
#include "xaes.h"
#include "xencode.h"

#if ( __VMS )
      #include <starlet.h> /* FOR VMS SPECIFIC ROUTINES */
#endif

#define BLOCK_LEN   16

	/* ENCAPSULATE DIRECT AES REFERENCES ENTIRELY WITHIN THIS SOURCE FILE */
#pragma warn -8056	// ELIMINATE WARNINGS FROM SPECULATIVE #DEFINES
#include "aes.h"
#pragma warn +8056
#define	ctx_e 	((aes_encrypt_ctx *) ctx_e_buffer)
#define ctx_d 	((aes_decrypt_ctx *) ctx_d_buffer)

//===========================================================================
#define	BINARY_KEY_SIZE		32	// ONLY 256-BIT VERSION IMPLEMENTED
#define	HEX_KEY_SIZE		64
//---------------------------------------------------------------------------
bool	XAES::first = true;
unsigned long	XAES::idum = 0;
unsigned char	*XAES::iv = NULL;	// USED FOR TESTING
//---------------------------------------------------------------------------
				/* FILL duf WITH LENGTH len TIME RELATED RANDOM DATA */
void XAES::fillrand( unsigned char *buf )
{
    int                  i;

/*   if(mt) { mt = 0; *(unsigned long long *)a = get_rdtsc(); } */
    if( first )/* USE SYSTEM TIME TO SEED THE RANDOM NUMBER GENERATOR */
	{first = false;
	idum = ( unsigned long ) time( NULL );
	}
    if ( NULL != iv )
	{memcpy( buf, iv, BLOCK_LEN );
	}
    else
	{for( i = 0; i < BLOCK_LEN; ++i )
	    {
	       idum = ( 1664525L * idum ) + 1013904223L;
	       buf[i] = (unsigned char) ( idum % 256 );
	    }
	}
}
//---------------------------------------------------------------------------
bool XAES::setInitVect( const std::string hex_iv )
{		// SET INITIALISATION-VECTOR WHEN ATTEMPTING VERIFICATION
	if ( hex_iv.size() != BLOCK_LEN * 2 )
		{return( false );
		}
	int	len_iv = -1;
	if ( ! XENCODE::hexToBin( hex_iv.c_str(), &iv, &len_iv )
			|| len_iv != BLOCK_LEN )
		{return( false );
		}
	return( true );
}
//---------------------------------------------------------------------------

void XAES::InitializeRijndael( const std::string hex_key )
{
	if ( hex_key.size() != HEX_KEY_SIZE )
		{setInvalid( "Invalid size for hex key" );
		return;
		}                       	// CONSTRUCT BINARY KEY
	int	len_bkey = -1;
	unsigned char *bkey = NULL;
	if ( ! XENCODE::hexToBin( hex_key.c_str(), HEX_KEY_SIZE, &bkey,
			&len_bkey ) || NULL == bkey )
		{setInvalid( "Fail at XENCODE::hexToBin" );
		return;
		}
	if ( BINARY_KEY_SIZE != len_bkey )
		{setInvalid( "Binary key has invalid length" );
		return;
		}
				// PREPARE ENCRYPTION AND DECRYPTION CONTEXT
       ctx_e_buffer = (aes_encrypt_ctx *) malloc( sizeof(aes_encrypt_ctx) );
       ctx_d_buffer = (aes_decrypt_ctx *) malloc( sizeof(aes_decrypt_ctx) );
       if ( NULL == ctx_e_buffer || NULL == ctx_d_buffer )
		{setInvalid( "malloc failure" );
		return;
		}
       aes_encrypt_key((unsigned char*) bkey, BINARY_KEY_SIZE, ctx_e );
       aes_decrypt_key((unsigned char*) bkey, BINARY_KEY_SIZE, ctx_d );
       free( bkey );
}
//===========================================================================
XAES::XAES( const std::string hex_key )
	:
	XERROR( "XAES" ),
	ctx_e_buffer( NULL ),
	ctx_d_buffer( NULL )
{
	if ( hex_key.size() != HEX_KEY_SIZE )
		{setInvalid( "hex_key not 64 chars" );
		return;
		}
	InitializeRijndael( hex_key.c_str() );
}
//---------------------------------------------------------------------------
XAES::~XAES( void )
{
	if ( NULL != ctx_e_buffer )
		{free( ctx_e_buffer );
		}
	if ( NULL != ctx_d_buffer )
		{free( ctx_d_buffer );
		}
}
//---------------------------------------------------------------------------
				/* RETURN 64 HEX CHARS READY FOR USE AS A KEY */
std::string XAES::bin2HexKey( const unsigned char *bin, const int len )
{
	if ( len < 6 || len > 200 )
		{return( "" );			// REJECT `SILLY' INPUTS
		}
	int	len_hex;
	char	*hex_key = NULL;
	if ( ! XENCODE::binToHex( bin, len, &hex_key, &len_hex ) )
		{return( "" );			// SHOULD NEVER HAPPEN
		}
	char	key[65];
	memcpy( key, hex_key, 64 );
	free( hex_key );
	int	i;
	for ( i = len_hex ; i < 64 ; i++ )
		{key[i] = '0';// PAD THE key WITH '0' AND EXTEND LENGTH TO 32 BYTES
		}
	key[64] = 0;
	return( std::string(key) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* RETURN 64 HEX CHARS READY FOR USE AS A KEY */
std::string XAES::string2HexKey( const std::string text )
{
	return( bin2HexKey( (unsigned char *) text.c_str(), text.size() ) );
}
//---------------------------------------------------------------------------
				/* ENCRYPT ARRAY - RETURN IN NEW ARRAY */
bool XAES::encrypt( unsigned char **enc_in, int *enc_len,
			unsigned char const *clr, const int clr_len)
{
	if ( ! isValid() )
		{setCaveat( "encrypt, object not valid" );
		return( false );
		}
    if( NULL == clr )
	{setCaveat( "encrypt, cleartext is NULL" );
	return false;
	}
    if ( NULL == enc_len )
	{setCaveat( "encrypt, enc_len is NULL" );
	return( false );
	}

    unsigned char       *enc = (unsigned char *) malloc( clr_len + BLOCK_LEN );
    if ( NULL == enc )
	    {setCaveat( "encrypt, malloc of enc failed" );
	    return( false );
	    }

    *enc_in = enc;
    *enc_len = 0;
    unsigned char dbuf[3 * BLOCK_LEN];
    int i, len, wlen = BLOCK_LEN;
/* clen KEEPS TRACK OF THE CURRENT POINTER IN THE CLEARTEXT */
    unsigned int clen = 0;


    // When ciphertext stealing is used, we three ciphertext blocks so
    // we use a buffer that is three times the block length.  The buffer
    // pointers b1, b2 and b3 point to the buffer positions of three
    // ciphertext blocks, b3 being the most recent and b1 being the
    // oldest. We start with the IV in b1 and the block to be decrypted
    // in b2.

    // set a random IV

    fillrand(dbuf);

    // read the first file block
//   len = (unsigned long) fread((char*)dbuf + BLOCK_LEN, 1, BLOCK_LEN, fin);
/*  CHANGE TO READ CLEARTEXT FROM clr */
    len = clr_len;

    if(len < BLOCK_LEN)
        {/* IF THE LENGTH IS LESS THAN ONE BLOCK */
        // read the first block from the cleartext stream, but len is less than
        // BLOCK_LEN
	memcpy( dbuf + BLOCK_LEN, clr, len);
        clen = len;

        // xor the file bytes with the IV bytes
        for(i = 0; i < len; ++i)
                {dbuf[i + BLOCK_LEN] ^= dbuf[i];
                }

        // encrypt the top 16 bytes of the buffer
        aes_encrypt(dbuf + len, dbuf + len, ctx_e);

        len += BLOCK_LEN;

        // write the IV and the encrypted file bytes
/*        if(fwrite((char*)dbuf, 1, len, fout) != len)
            return WRITE_ERROR; */
/* WRITE THE IV AND ENCRYPTED BYTES TO enc */
	memcpy( enc, dbuf, len);
        *enc_len += len;

        return ( true );
        }
    else    // if the file length is more 16 bytes
        {unsigned char  *b1 = dbuf;
        unsigned char   *b2 = dbuf + BLOCK_LEN;
        unsigned char   *b3 = dbuf + ( 2 * BLOCK_LEN );
	unsigned char   *bt = NULL;

        // write the IV

        /*  if(fwrite((char*)dbuf, 1, BLOCK_LEN, fout) != BLOCK_LEN)
            return WRITE_ERROR; */
/* WRITE THE IV STRAIGHT TO enc */
	 memcpy( enc, dbuf, BLOCK_LEN);
         *enc_len += BLOCK_LEN;

        // read the first block from the cleartext stream
	memcpy( dbuf + BLOCK_LEN, clr, BLOCK_LEN);
        len = BLOCK_LEN;
        clen = BLOCK_LEN;

        for( ; ; )
            {
            /*  // read the next block to see if ciphertext stealing is needed
            len = (unsigned long)fread((char*)b3, 1, BLOCK_LEN, fin); */
/* READ A BLOCK FROM clr */
            if ((clr_len - clen) >= BLOCK_LEN)
                {/* HAVE ENOUGH CLEARTEXT LEFT FOR A BLOCK, I.E. NO CIPHERTEXT STEALING IS NEEDED */
                len = BLOCK_LEN;
		memcpy( b3, clr+clen, len);
                clen += BLOCK_LEN;
                }
            else
                {/* NOT ENOUGH CLEARTEXT LEFT, I.E. HAVE TO USE CIPHERTEXT STEALING */
                len = clr_len - clen;
                if (len != 0)
			{memcpy( b3, clr+clen, len);
                        }
		clen = clr_len;
                }

            // do CBC chaining prior to encryption for current block (in b2)
	    for(i = 0; i < BLOCK_LEN; ++i)
                {b1[i] ^= b2[i];
                }

            // encrypt the block (now in b1)
            aes_encrypt(b1, b1, ctx_e);

            if(len != 0 && len != BLOCK_LEN)    // use ciphertext stealing
                {// set the length of the last block
                wlen = len;

                // xor ciphertext into last block
                for(i = 0; i < len; ++i)
                    {b3[i] ^= b1[i];
                    }

                // move 'stolen' ciphertext into last block
                for(i = len; i < BLOCK_LEN; ++i)
                    {b3[i] = b1[i];
                    }

                // encrypt this block
                aes_encrypt(b3, b3, ctx_e);

/*                // and write it as the second to last encrypted block
                if(fwrite((char*)b3, 1, BLOCK_LEN, fout) != BLOCK_LEN)
                    return WRITE_ERROR;  */
/* WRITE IT TO enc */
		memcpy( enc + *enc_len, (char*) b3, BLOCK_LEN);
                *enc_len += BLOCK_LEN;
                }

/*            // write the encrypted block
	    if(fwrite((char*)b1, 1, wlen, fout) != wlen)
		return WRITE_ERROR; */
/* WRITE TO enc */
	    memcpy( enc + *enc_len, (char*) b1, wlen);
	    *enc_len += wlen;

	    if(len != BLOCK_LEN)
		{return( true );
		}

	    // advance the buffer pointers
/*            bt = b3, b3 = b2, b2 = b1, b1 = bt; */
	    bt = b3;
	    b3 = b2;
	    b2 = b1;
	    b1 = bt;
	}
    }
}
//---------------------------------------------------------------------------
bool XAES::encrypt( XBINOB *enc, const XBINOB *clr )
{
	int 	enc_len = -1;
	unsigned char *enc_data = NULL;
	bool	ok = encrypt( &enc_data, &enc_len, clr->data(), clr->size() );
	if ( ok )
		{enc->acquire( enc_len, enc_data );
		}
	else
		{if ( NULL != enc_data )
			{free( enc_data );
			}
		}
	return( ok );
}
//---------------------------------------------------------------------------
bool XAES::decrypt( XBINOB *clr, const XBINOB *enc )
{
	int	clr_len = -1;
	unsigned char *clr_data = NULL;
	bool	ok = decrypt( &clr_data, &clr_len, enc->data(), enc->size() );
	if ( ok )
		{clr->acquire( clr_len, clr_data );
		}
	else
		{if ( NULL != clr_data )
			{free( clr_data );
			}
		}
	return( ok );
}
//---------------------------------------------------------------------------
				/* DECRYPT ARRAY - RETURN IN NEW ARRAY */
bool XAES::decrypt( unsigned char **clr_out, int *clr_len,
	const unsigned char *enc, const int enc_len)
{
	if ( ! isValid() )
		{setCaveat( "decrypt, object not valid" );
		return( false );
		}
    if( NULL == enc )
	{setCaveat( "decrypt, encrypted stream is NULL" );
	return( false );
	}
    if( enc_len < BLOCK_LEN )
	{setCaveat( "decrypt, encrypted stream is too short" );
	return( false );
	}
    unsigned char *clr = (unsigned char *) malloc( enc_len + BLOCK_LEN );
    if ( NULL == clr )
	{setCaveat( "decrypt, malloc failed" );
	return( false );
	}
    *clr_out = clr;
    *clr_len = 0;
    unsigned char dbuf[3 * BLOCK_LEN], buf[BLOCK_LEN];
    unsigned long i, len, wlen = BLOCK_LEN;
    unsigned long elen = 0; // pointer to the ciphered stream enc

    // When ciphertext stealing is used, we three ciphertext blocks so
    // we use a buffer that is three times the block length.  The buffer
    // pointers b1, b2 and b3 point to the buffer positions of three
    // ciphertext blocks, b3 being the most recent and b1 being the
    // oldest. We start with the IV in b1 and the block to be decrypted
    // in b2.


/*    len = (unsigned long)fread((char*)dbuf, 1, 2 * BLOCK_LEN, fin); */
/* READ FROM enc */
    if (enc_len < 2 * BLOCK_LEN)
	{memcpy( dbuf, enc, enc_len);
        len = enc_len;
        elen = enc_len;
        }
    else
        {memcpy( dbuf, enc, 2 * BLOCK_LEN);
        len = 2 * BLOCK_LEN;
        elen = 2 * BLOCK_LEN;
        }

    if(len < 2 * BLOCK_LEN) // the original file is less than one block in length
        {len -= BLOCK_LEN;
        // decrypt from position len to position len + BLOCK_LEN
        aes_decrypt(dbuf + len, dbuf + len, ctx_d);

        // undo the CBC chaining
	for(i = 0; i < len; ++i)
            {dbuf[i] ^= dbuf[i + BLOCK_LEN];
            }

/*        // output the decrypted bytes
        if(fwrite((char*)dbuf, 1, len, fout) != len)
            return WRITE_ERROR; */
/* WRITE THE DECRYPTED BYTES TO clr */
	memcpy( clr, dbuf, len);
        *clr_len = len;

        return( true );
        }
    else // the original file is more than one block in length
        {unsigned char  *b1 = dbuf;
        unsigned char   *b2 = dbuf + BLOCK_LEN;
        unsigned char   *b3 = dbuf + (2 * BLOCK_LEN);
        unsigned char   *bt = NULL;

        for( ; ; )  // while some ciphertext remains, prepare to decrypt block b2
	    {
/*            // read in the next block to see if ciphertext stealing is needed
            len = fread((char*)b3, 1, BLOCK_LEN, fin); */
/* READ FROM enc */
            if ((enc_len - elen) >= BLOCK_LEN)
                {/* NO CIPHER STEALING IS REQUIRED */
                memcpy( b3, enc+elen, BLOCK_LEN);
                len = BLOCK_LEN;
                elen += BLOCK_LEN;
                }
            else
                {/* CIPHER STEALING IS REQUIRED */
		 memcpy( b3, enc+elen, enc_len - elen);
		 len = enc_len - elen;
		 elen = enc_len;
		 }

	    // decrypt the b2 block
	    aes_decrypt(b2, buf, ctx_d);

	    if(len == 0 || len == BLOCK_LEN)    // no ciphertext stealing
		{// unchain CBC using the previous ciphertext block in b1
			for(i = 0; i < BLOCK_LEN; ++i)
			    {buf[i] ^= b1[i];
			    }
		}
	    else    // partial last block - use ciphertext stealing
		{wlen = len;

		// produce last 'len' bytes of plaintext by xoring with
		// the lowest 'len' bytes of next block b3 - C[N-1]
		for(i = 0; i < len; ++i)
		    {buf[i] ^= b3[i];
		    }

		// reconstruct the C[N-1] block in b3 by adding in the
		// last (BLOCK_LEN - len) bytes of C[N-2] in b2
		for(i = len; i < BLOCK_LEN; ++i)
		    {b3[i] = buf[i];
		    }

		// decrypt the C[N-1] block in b3
		aes_decrypt(b3, b3, ctx_d);

		// produce the last but one plaintext block by xoring with
		// the last but two ciphertext block
		for(i = 0; i < BLOCK_LEN; ++i)
		    {b3[i] ^= b1[i];
		    }

/*                // write decrypted plaintext blocks
		if(fwrite((char*)b3, 1, BLOCK_LEN, fout) != BLOCK_LEN)
		    return WRITE_ERROR;  */
/* WRITE TO clr */
		memcpy( clr+(*clr_len), b3, BLOCK_LEN);
		*clr_len += BLOCK_LEN;
		}

/*            // write the decrypted plaintext block
	    if(fwrite((char*)buf, 1, wlen, fout) != wlen)
		return WRITE_ERROR; */
/* WRITE THE LAST REMAINING DECRYPTED BLOCK TO clr */
		memcpy( clr+(*clr_len), buf, wlen);
		*clr_len += wlen;

		if(len != BLOCK_LEN)
			{//strncpy((char *)clr+*clr_len, "\0", 1);
			return( true );
			}

		// advance the buffer pointers
/*                bt = b1, b1 = b2, b2 = b3, b3 = bt; */
		bt = b1;
		b1 = b2;
		b2 = b3;
		b3 = bt;
	}
    }
}
//---------------------------------------------------------------------------
		/* ENCRYPT ARRAY - CHECK DECRYPT - RETURN IN NEW ARRAY */
bool XAES::encryptValidate( unsigned char **enc, int *len_enc,
	const unsigned char *clr, const int len_clr )
{
	int	len_test;
	bool	match = true;
	unsigned	char	*test = NULL;
	if ( ! encrypt( enc, len_enc, clr, len_clr ) )
		{return( false );
		}
	if ( ! decrypt( &test, &len_test, *enc, *len_enc )
			|| len_test != len_clr)
		{free( *enc );
		*enc = NULL;
		if ( NULL != test )
			{free( test );
			}
		return( false );
		}
	match = ( 0 == memcmp( clr, test, len_clr ) );
	free( test );
	if ( ! match )
		{*len_enc = -1;
		free( *enc );
		*enc = NULL;
		}
	return( match );
}
//---------------------------------------------------------------------------
				/* ENCRYPT 0-TERMINATED STRING */
bool XAES::encryptSz( unsigned char **enc, int *len_enc, const char *sz )
{
	*enc = NULL;
	return( ( NULL != sz )
		? encrypt( enc, len_enc, (const unsigned char *) sz, strlen(sz) + 1 )
		: false );
}
//---------------------------------------------------------------------------
		/* ENCRYPT BINARY BUFFER AND RETURN RESULTS AS A HEX STRING */
char *XAES::encryptBuf2Hex( const char *buf, const int len_buf )
{
	int	len_bin, len_hex;
	unsigned	char	*bin = NULL;
	if ( ! encrypt( &bin, &len_bin, (const unsigned char *) buf, len_buf ) )
		{return( NULL );
		}
	char 	*hex = NULL;
	bool	status = XENCODE::binToHex( bin, len_bin, &hex, &len_hex );
	free( bin );
	return( status ? hex : NULL );
}
//---------------------------------------------------------------------------
	/* ENCRYPT 0-TERMINATED-STRING AND RETURN RESULT AS A HEX STRING */
char *XAES::encryptSz2Hex( const char *sz )
{
	return( ( NULL != sz ) ? encryptBuf2Hex( sz, strlen(sz) + 1 ) : NULL );
}
//---------------------------------------------------------------------------
			/* DECRYPT HEX STRING AND RETURN RESULT */
	/* (SHOULD ONLY BE USED FOR DATA PROCESSED VIA encryptHex) */
bool XAES::decryptHex( unsigned char **clr, int *len_clr, const char *hex )
{
	int	len_bin;
	unsigned char	*bin = NULL;
	if ( ! XENCODE::hexToBin( hex, &bin, &len_bin ) )
		{return( false );
		}
	bool	status = decrypt( clr, len_clr, bin, len_bin );
	free( bin );
	return( status );
}
//---------------------------------------------------------------------------
	/* ENCRYPT BINARY BUFFER AND RETURN RESULT AS A BASE64 STRING */
char *XAES::encryptBuf2Base64( const char *buf, const int len_buf )
{
	int	len_bin, len_b64;
	unsigned 	char	*bin = NULL;
	if ( ! encrypt( &bin, &len_bin, (const unsigned char *) buf, len_buf ) )
		{return( NULL );
		}
	char 	*b64 = NULL;
	bool	status = XENCODE::binToB64( bin, len_bin, &b64, &len_b64 );
	free( bin );
	return( status ? b64 : NULL );
}
//---------------------------------------------------------------------------
	/* ENCRYPT 0-TERMINATED-STRING AND RETURN RESULT AS A BASE64 STRING */
char *XAES::encryptSz2Base64( const char *sz )
{
	return( ( NULL != sz ) ? encryptBuf2Base64( sz, strlen( sz ) + 1 ) : NULL );
}
//---------------------------------------------------------------------------
				/* DECRYPT B64 STRING AND RETURN RESULT */
	/* (SHOULD ONLY BE USED FOR DATA PROCESSED VIA encryptBase64) */
bool XAES::decryptBase64( unsigned char **clr, int *len_clr,
	const char *b64 )
{
	int	len_bin;
	unsigned char	*bin = NULL;
	if ( ! XENCODE::b64ToBin( b64, &bin, &len_bin ) )
		{return( false );
		}
	bool	status = decrypt( clr, len_clr, bin, len_bin );
	free( bin );
	return( status );
}
//---------------------------------------------------------------------------
				/* ENCRYPT FILE */
bool XAES::encryptFile(FILE *fin, FILE *fout)
{
    if( NULL == fin || NULL == fout )
	    {setCaveat( "input or output file is NULL" );
	    return false;
	    }

    unsigned char dbuf[3 * BLOCK_LEN];
    unsigned long i, len, wlen = BLOCK_LEN;

    // When ciphertext stealing is used, we three ciphertext blocks so
    // we use a buffer that is three times the block length.  The buffer
    // pointers b1, b2 and b3 point to the buffer positions of three
    // ciphertext blocks, b3 being the most recent and b1 being the
    // oldest. We start with the IV in b1 and the block to be decrypted
    // in b2.

    // set a random IV

    fillrand( dbuf );

    // read the first file block
    len = (unsigned long) fread((char*)dbuf + BLOCK_LEN, 1, BLOCK_LEN, fin);

    if(len < BLOCK_LEN)
        {   // if the file length is less than one block

        // xor the file bytes with the IV bytes
        for(i = 0; i < len; ++i)
            {dbuf[i + BLOCK_LEN] ^= dbuf[i];
            }

        // encrypt the top 16 bytes of the buffer
        aes_encrypt(dbuf + len, dbuf + len, ctx_e);

        len += BLOCK_LEN;
        // write the IV and the encrypted file bytes
        if(fwrite((char*)dbuf, 1, len, fout) != len)
            {return( false );
            }

        return( true );
        }
    else    // if the file length is more 16 bytes
        {/*   unsigned char *b1 = dbuf, *b2 = b1 + BLOCK_LEN, *b3 = b2 + BLOCK_LEN, *bt; */
        unsigned char   *b1 = dbuf;
        unsigned char   *b2 = dbuf + BLOCK_LEN;
        unsigned char   *b3 = dbuf + ( 2 * BLOCK_LEN);
        unsigned char   *bt = NULL;

        // write the IV
        if(fwrite((char*)dbuf, 1, BLOCK_LEN, fout) != BLOCK_LEN)
            {return( false );
            }

        for( ; ; )
            {
            // read the next block to see if ciphertext stealing is needed
            len = (unsigned long)fread((char*)b3, 1, BLOCK_LEN, fin);

            // do CBC chaining prior to encryption for current block (in b2)
            for(i = 0; i < BLOCK_LEN; ++i)
                {b1[i] ^= b2[i];
                }

            // encrypt the block (now in b1)
            aes_encrypt(b1, b1, ctx_e);

            if(len != 0 && len != BLOCK_LEN)    // use ciphertext stealing
                {// set the length of the last block
                wlen = len;

                // xor ciphertext into last block
                for(i = 0; i < len; ++i)
                    {b3[i] ^= b1[i];
                    }

                // move 'stolen' ciphertext into last block
                for(i = len; i < BLOCK_LEN; ++i)
                    {b3[i] = b1[i];
                    }

                // encrypt this block
                aes_encrypt(b3, b3, ctx_e);

                // and write it as the second to last encrypted block
                if(fwrite((char*)b3, 1, BLOCK_LEN, fout) != BLOCK_LEN)
                    {return( false );
                    }
                }

            // write the encrypted block
            if(fwrite((char*)b1, 1, wlen, fout) != wlen)
                {return( false );
                }

            if(len != BLOCK_LEN)
                {return( true );
                }

            // advance the buffer pointers
/*            bt = b3, b3 = b2, b2 = b1, b1 = bt; */
                bt = b3;
                b3 = b2;
                b2 = b1;
                b1 = bt; 
        }
    }
}
//---------------------------------------------------------------------------
				/* DECRYPT FILE */
bool XAES::decryptFile(FILE *fin, FILE *fout)
{
    unsigned char dbuf[3 * BLOCK_LEN], buf[BLOCK_LEN];
    unsigned long i, len, wlen = BLOCK_LEN;

    // When ciphertext stealing is used, we three ciphertext blocks so
    // we use a buffer that is three times the block length.  The buffer
    // pointers b1, b2 and b3 point to the buffer positions of three
    // ciphertext blocks, b3 being the most recent and b1 being the
    // oldest. We start with the IV in b1 and the block to be decrypted
    // in b2.

    len = (unsigned long)fread((char*)dbuf, 1, 2 * BLOCK_LEN, fin);

    if(len < 2 * BLOCK_LEN) // the original file is less than one block in length
        {len -= BLOCK_LEN;
        // decrypt from position len to position len + BLOCK_LEN
        aes_decrypt(dbuf + len, dbuf + len, ctx_d);

        // undo the CBC chaining
        for(i = 0; i < len; ++i)
            {dbuf[i] ^= dbuf[i + BLOCK_LEN];
            }

        // output the decrypted bytes
        if(fwrite((char*)dbuf, 1, len, fout) != len)
            {return( false );
            }

        return( true );
        }
    else
        {/*unsigned char *b1 = dbuf, *b2 = b1 + BLOCK_LEN, *b3 = b2 + BLOCK_LEN, *bt;*/
        unsigned char   *b1 = dbuf;
        unsigned char   *b2 = dbuf + BLOCK_LEN;
        unsigned char   *b3 = dbuf + ( 2 * BLOCK_LEN);
        unsigned char   *bt = NULL;

        for( ; ; )  // while some ciphertext remains, prepare to decrypt block b2
            {// read in the next block to see if ciphertext stealing is needed
            len = fread((char*)b3, 1, BLOCK_LEN, fin);

            // decrypt the b2 block
            aes_decrypt(b2, buf, ctx_d);

            if(len == 0 || len == BLOCK_LEN)    // no ciphertext stealing
                {// unchain CBC using the previous ciphertext block in b1
                for(i = 0; i < BLOCK_LEN; ++i)
                    {buf[i] ^= b1[i];
                    }
                }
            else    // partial last block - use ciphertext stealing
                {wlen = len;

                // produce last 'len' bytes of plaintext by xoring with
                // the lowest 'len' bytes of next block b3 - C[N-1]
                for(i = 0; i < len; ++i)
                    {buf[i] ^= b3[i];
                    }

                // reconstruct the C[N-1] block in b3 by adding in the
                // last (BLOCK_LEN - len) bytes of C[N-2] in b2
                for(i = len; i < BLOCK_LEN; ++i)
                    {b3[i] = buf[i];
                    }

                // decrypt the C[N-1] block in b3
                aes_decrypt(b3, b3, ctx_d);

                // produce the last but one plaintext block by xoring with
                // the last but two ciphertext block
                for(i = 0; i < BLOCK_LEN; ++i)
                    {b3[i] ^= b1[i];
                    }

                // write decrypted plaintext blocks
                if(fwrite((char*)b3, 1, BLOCK_LEN, fout) != BLOCK_LEN)
                    {return( false );
                    }
                }

            // write the decrypted plaintext block
            if(fwrite((char*)buf, 1, wlen, fout) != wlen)
                {return( false );
                }

            if(len != BLOCK_LEN)
                {return( true );
                }

            // advance the buffer pointers
/*            bt = b1, b1 = b2, b2 = b3, b3 = bt; */
            bt = b1;
            b1 = b2;
            b2 = b3;
            b3 = bt; 
        }
    }
}

//===========================================================================


