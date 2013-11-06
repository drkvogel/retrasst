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
#include <ctype.h>
#include <time.h>
#ifdef __BORLANDC__
#include <mem.h>
#endif
#include "rijndael.h"
#include "xencode.h"

#if ( __VMS )
      #include <starlet.h> /* FOR VMS SPECIFIC ROUTINES */
#endif

#define BLOCK_LEN   16

	/* ENCAPSULATE DIRECT AES REFERENCES ENTIRELY WITHIN THIS SOURCE FILE */
#include "aes.h"
#define	ctx_e 	((aes_encrypt_ctx *) ctx_e_buffer)
#define ctx_d 	((aes_decrypt_ctx *) ctx_d_buffer)

//===========================================================================

// ... PRIVATE ROUTINES HERE
//  A Pseudo Random Number Generator (PRNG) used for the
//  Initialisation Vector. The PRNG is George Marsaglia's
//  Multiply-With-Carry (MWC) PRNG that concatenates two
//  16-bit MWC generators:
//      x(n)=36969 * x(n-1) + carry mod 2^16
//      y(n)=18000 * y(n-1) + carry mod 2^16
//  to produce a combined PRNG with a period of about 2^60.
//  The Pentium cycle counter is used to initialise it. This
//  is crude but the IV does not really need to be secret.

/* COMMENTED OUT BY ERICA, THE FOLLOWING IS THE ORIGINAL ROUTINE FOR
 * GETTING PENTIUM CYCLE COUNTER TO INITIALISE THE RANDOM NUMBER GENERATOR (RNG)
 * FOR PLATFORM INDEPENDENCY, THIS HAS BEEN CHANGED TO USE SYSTEM TIME TO
 * INITIALISE THE RNG.
 
  __inline volatile unsigned long long get_rdtsc(void)
{   unsigned long lo, hi;

#if defined( _MSC_VER )
    __asm
    {   cpuid
        rdtsc
        mov     [lo],eax
        mov     [hi],edx
    }
#else
    asm volatile("cpuid; rdtsc" : "=a" (lo), "=d" (hi));
#endif
    return (((unsigned long long)hi) << 32) | lo;
}

#define RAND(a,b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + \
                    (b = 18000 * (b & 65535) + (b >> 16))  )
*/

//---------------------------------------------------------------------------
				/* FILL duf WITH LENGTH len TIME RELATED RANDOM DATA */
static void fillrand(unsigned char *buf, const int len)
{   static unsigned long idum;
    int                  i;
    static      bool        first = true;

/*   if(mt) { mt = 0; *(unsigned long long *)a = get_rdtsc(); } */
    if( first )/* USE SYSTEM TIME TO SEED THE RANDOM NUMBER GENERATOR */
        {first = false;
        idum = ( unsigned long ) time( NULL );
        }
    for( i = 0; i < len; ++i )
            {
               idum = ( 1664525L * idum ) + 1013904223L;
	       buf[i] = (unsigned char) ( idum % 256 );
            }
}

void RIJNDAEL::InitializeRijndael( const char *original_key, const int keybytes )
{
    char            *cp, ch, key[32];
    int             i, by = 0, key_len;

    cp = (char*)original_key;   // this is a pointer to the hexadecimal key digits
    i = 0;          // this is a count for the input digits processed

    if( NULL == original_key )
        {last_error = "Null key";
        valid = false;
        return;
        }
    if( keybytes != 16 && keybytes != 24 && keybytes != 32 )
        {last_error = "The key length must be 16, 24, or 32 bytes";
        valid = false;
        return;
        }
    else if ( keybytes > 32 )
        {last_error = "The key length must be less than or equal to 32 bytes";
        valid = false;
        return;
        }

/* INITIALISE THE KEY */
    while(i < 64 && *cp)        // the maximum key length is 32 bytes and
        {                           // hence at most 64 hexadecimal digits
		ch = (char) toupper(*cp++);    // process a hexadecimal digit
                if(ch >= '0' && ch <= '9')
                    {by = (by << 4) + ch - '0';
                    }
                else if(ch >= 'A' && ch <= 'F')
                    {by = (by << 4) + ch - 'A' + 10;
                    }
                else                    // error if not hexadecimal
                    {last_error = "key must be in hexadecimal notation";
                    valid = false;
                    break;
                    }
                // store a key byte for each pair of hexadecimal digits
                if(i++ & 1)
		    {key[i / 2 - 1] = (char) ( by & 0xff );
                    }
        }
/*
    if(*cp)
        {last_error = "The key value is too long";
        valid = false;
        }
    else if(i < 32 || (i & 15))
        {last_error = "The key length must be 32, 48 or 64 hexadecimal digits ";
        valid = false;
        }
*/

    key_len = i / 2;

/* PREPARE THE ENCRYPTION AND DECRYPTION CONTEXT */
       ctx_e_buffer = (aes_encrypt_ctx *) malloc( sizeof(aes_encrypt_ctx) );
       ctx_d_buffer = (aes_decrypt_ctx *) malloc( sizeof(aes_decrypt_ctx) );

       if ( NULL == ctx_e_buffer || NULL == ctx_d_buffer )
		{last_error = "malloc failure";
		valid = false;
		return;
		}

       aes_encrypt_key((unsigned char*)key, key_len, ctx_e );
       aes_decrypt_key((unsigned char*)key, key_len, ctx_d );
}

const std::string RIJNDAEL::getLastError( void )
  {
	return( last_error );
  }

//===========================================================================
					/* GENERAL CONSTRUCTOR */
RIJNDAEL::RIJNDAEL( const char *original_key, const int keybytes )
	:
	ctx_e_buffer( NULL ),
	ctx_d_buffer( NULL ),
	valid( true )
{
        InitializeRijndael( original_key, keybytes );
}

//---------------------------------------------------------------------------
					/* TEXT-KEY (WEAKER) CONSTRUCTOR */
RIJNDAEL::RIJNDAEL( const char *key_text )
	:
	ctx_e_buffer( NULL ),
	ctx_d_buffer( NULL ),
	valid( true )
{	int	        i = 0;
//        char            key_copy[32];
        char            key_copy[33], *hex_key;
        int             len_hex;

        if ( NULL == key_text )
        {last_error = "NULL key";
        valid = false;
        return;
        }

        XENCODE::binToHex( ( unsigned char * ) key_text, ( int ) strlen( key_text ), &hex_key, &len_hex );
        strncpy( key_copy, hex_key, 32 );
        free( hex_key );
        if(  32 > len_hex ) // PAD THE key_text WITH '0' AND EXTEND ITS LENGTH TO 32 BYTES
                {for ( i = len_hex ; i < 32 ; i++ )
                        {key_copy[i] = '0';
                        }
                }
        key_copy[32] = '\0';
        len_hex = 32;
        InitializeRijndael( key_copy, len_hex );
}
//---------------------------------------------------------------------------
RIJNDAEL::~RIJNDAEL( void )
{
	if ( NULL != ctx_e_buffer )
		{free( ctx_e_buffer );
		}
	if ( NULL != ctx_d_buffer )
		{free( ctx_d_buffer );
		}
}
//---------------------------------------------------------------------------
bool RIJNDAEL::isValid( void ) const
{
	return( valid );
}
//---------------------------------------------------------------------------
				/* ENCRYPT ARRAY - RETURN IN NEW ARRAY */
bool 	RIJNDAEL::encrypt( unsigned char **enc_in, int *enc_len,
                        unsigned char const *clr, const int clr_len)
{
	if ( ! valid )
		{last_error = "RIJNDAEL::encrypt, object not valid";
		return( false );
		}
    if( NULL == clr )
	{last_error = "RIJNDAEL::encrypt, cleartext is NULL";
	return false;
	}
    if ( NULL == enc_len )
	{last_error = "RIJNDAEL::encrypt, enc_len is NULL";
	return( false );
	}

    unsigned char       *enc = (unsigned char *) malloc( clr_len + BLOCK_LEN );
    if ( NULL == enc )
	    {last_error = "RIJNDAEL::encrypt, malloc of enc failed";
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

    fillrand(dbuf, BLOCK_LEN);

    // read the first file block
//   len = (unsigned long) fread((char*)dbuf + BLOCK_LEN, 1, BLOCK_LEN, fin);
/*  CHANGE TO READ CLEARTEXT FROM clr */
    len = clr_len;

    if(len < BLOCK_LEN)
        {/* IF THE LENGTH IS LESS THAN ONE BLOCK */
        // read the first block from the cleartext stream, but len is less than
        // BLOCK_LEN
        memcpy((unsigned char*)dbuf + BLOCK_LEN, clr, len);
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
        memcpy((unsigned char *)enc, dbuf, len);
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
         memcpy(enc, dbuf, BLOCK_LEN);
         *enc_len += BLOCK_LEN;

        // read the first block from the cleartext stream
        memcpy((unsigned char*)dbuf + BLOCK_LEN, clr, BLOCK_LEN);
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
                memcpy((unsigned char*) b3, clr+clen, len);
                clen += BLOCK_LEN;
                }
            else
                {/* NOT ENOUGH CLEARTEXT LEFT, I.E. HAVE TO USE CIPHERTEXT STEALING */
                len = clr_len - clen;
                if (len != 0)
                        {memcpy((unsigned char*) b3, clr+clen, len);
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
                memcpy((unsigned char*)enc+*enc_len, (char*) b3, BLOCK_LEN);
                *enc_len += BLOCK_LEN;
                }

/*            // write the encrypted block
            if(fwrite((char*)b1, 1, wlen, fout) != wlen)
                return WRITE_ERROR; */
/* WRITE TO enc */
            memcpy((unsigned char*)enc+*enc_len, (char*) b1, wlen);
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
				/* DECRYPT ARRAY - RETURN IN NEW ARRAY */
bool	RIJNDAEL::decrypt( unsigned char **clr_out, int *clr_len,
                        const unsigned char *enc, const int enc_len)
{
	if ( ! valid )
		{last_error = "RIJNDAEL::decrypt, object not valid";
		return( false );
		}
    if( NULL == enc )
	{last_error = "RIJNDAEL::decrypt, encrypted stream is NULL";
	return( false );
	}
    if( enc_len < BLOCK_LEN )
	{last_error = "RIJNDAEL::decrypt, encrypted stream is too short";
	return( false );
	}
    unsigned char *clr = (unsigned char *) malloc( enc_len + BLOCK_LEN );
    if ( NULL == clr )
	{last_error = "RIJNDAEL::decrypt, malloc failed";
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
        {memcpy((unsigned char*) dbuf, enc, enc_len);
        len = enc_len;
        elen = enc_len;
        }
    else
        {memcpy((unsigned char *) dbuf, enc, 2 * BLOCK_LEN);
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
        memcpy((unsigned char*) clr, (unsigned char*)dbuf, len);
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
                memcpy((unsigned char *) b3, enc+elen, BLOCK_LEN);
                len = BLOCK_LEN;
                elen += BLOCK_LEN;
                }
            else
                {/* CIPHER STEALING IS REQUIRED */
                 memcpy((unsigned char *) b3, enc+elen, enc_len - elen);
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
                memcpy((unsigned char*) clr+(*clr_len), (char*)b3, BLOCK_LEN);
                *clr_len += BLOCK_LEN;
                }

/*            // write the decrypted plaintext block
            if(fwrite((char*)buf, 1, wlen, fout) != wlen)
                return WRITE_ERROR; */
/* WRITE THE LAST REMAINING DECRYPTED BLOCK TO clr */
                memcpy((unsigned char*) clr+(*clr_len), (char*)buf, wlen);
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
bool RIJNDAEL::encryptValidate( unsigned char **enc, int *len_enc,
	const unsigned char *clr, const int len_clr )
{
	int	len_test;
	bool	match = true;
	unsigned	char	*test = NULL;
	if ( ! encrypt( enc, len_enc, clr, len_clr ) )
		{valid = false;				// NOTE FAILURE
		return( false );
		}
	if ( ! decrypt( &test, &len_test, *enc, *len_enc )
			|| len_test != len_clr)
		{free( *enc );
		*enc = NULL;
		if ( NULL != test )
			{free( test );
			}
		valid = false;
		return( false );
		}
	match = ( 0 == memcmp( clr, test, len_clr ) );
	free( test );
	if ( ! match )
		{free( *enc );
		*enc = NULL;
		valid = false;
		}
	return( match );
}

//---------------------------------------------------------------------------
				/* ENCRYPT 0-TERMINATED STRING */
bool RIJNDAEL::encryptSz( unsigned char **enc, int *len_enc, const char *sz )
{
	*enc = NULL;
	return( ( NULL != sz )
		? encrypt( enc, len_enc, (const unsigned char *) sz, strlen(sz) + 1 )
		: false );
}
//---------------------------------------------------------------------------
		/* ENCRYPT BINARY BUFFER AND RETURN RESULTS AS A HEX STRING */
char *RIJNDAEL::encryptBuf2Hex( const char *buf, const int len_buf )
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
char *RIJNDAEL::encryptSz2Hex( const char *sz )
{
	return( ( NULL != sz ) ? encryptBuf2Hex( sz, strlen(sz) + 1 ) : NULL );
}
//---------------------------------------------------------------------------
			/* DECRYPT HEX STRING AND RETURN RESULT */
	/* (SHOULD ONLY BE USED FOR DATA PROCESSED VIA encryptHex) */
bool RIJNDAEL::decryptHex( unsigned char **clr, int *len_clr, const char *hex )
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
char *RIJNDAEL::encryptBuf2Base64( const char *buf, const int len_buf )
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
char *RIJNDAEL::encryptSz2Base64( const char *sz )
{
	return( ( NULL != sz ) ? encryptBuf2Base64( sz, strlen( sz ) + 1 ) : NULL );
}
//---------------------------------------------------------------------------
				/* DECRYPT B64 STRING AND RETURN RESULT */
	/* (SHOULD ONLY BE USED FOR DATA PROCESSED VIA encryptBase64) */
bool RIJNDAEL::decryptBase64( unsigned char **clr, int *len_clr,
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
bool RIJNDAEL::encryptFile(FILE *fin, FILE *fout)
{
    if( NULL == fin || NULL == fout )
    {last_error = "input or output file is NULL";
    valid = false;
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

    fillrand(dbuf, BLOCK_LEN);

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
bool RIJNDAEL::decryptFile(FILE *fin, FILE *fout)
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


