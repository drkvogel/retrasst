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
 Issue 1/08/2005

 This file contains the definitions required to use AES in C. See aesopt.h
 for optimisation details.
*/

#ifndef _AES_H
#define _AES_H

#include <stdlib.h>

/*  This include is used to find 8 & 32 bit unsigned integer types  */
/* #include "tdefs.h" */

#if defined(__cplusplus)
extern "C"
{
#endif

#include <limits.h>

#if UCHAR_MAX == 0xff
  typedef unsigned char     uint_8t;
#else
# error Please define uint_8t as an 8-bit unsigned integer type in tdefs.h
#endif


#if USHRT_MAX == 0xffff
  typedef   unsigned short  uint_16t;
#else
# error Please define uint_16t as a 16-bit unsigned short type in tdefs.h
#endif

#if UINT_MAX == 0xffffffff
  typedef   unsigned int    uint_32t;
#elif ULONG_MAX == 0xfffffffful
  typedef   unsigned long   uint_32t;
#else
# error Please define uint_32t as a 32-bit unsigned integer type in tdefs.h
#endif

# define li_64(h)    0x##h##ull
#if defined( _MSC_VER )
# if _MSC_VER < 1310
    typedef unsigned __int64    uint_64t;
#   undef  li_64
#   define li_64(h) 0x##h##ui64
# else
    typedef unsigned long long  uint_64t;
# endif
#elif defined( __sun ) && defined(ULONG_MAX) && ULONG_MAX == 0xfffffffful
  typedef unsigned long long    uint_64t;
/* modified by Erica, 26 May 2006 because Borland C++ doesn't
 * support this expression
 * #elif defined( ULONG_LONG_MAX ) && ULONG_LONG_MAX == 0xffffffffffffffffull
 */
#elif defined( ULONG_LONG_MAX ) && ULONG_LONG_MAX == 0xfffffffffffffffful
  typedef unsigned long long    uint_64t;
/* modified by Erica, 26 May 2006 because Borland C++ doesn't
 * support this expression
 * #elif defined( ULONG_LONG_MAX ) && ULONG_LONG_MAX == 0xffffffffffffffffull
 */
  #elif defined( ULLONG_MAX ) && ULLONG_MAX == 0xfffffffffffffffful
  typedef unsigned long long    uint_64t;
#elif defined( ULONG_MAX ) && ULONG_MAX == 0xfffffffffffffffful
  typedef unsigned long         uint_64t;
#elif defined( UINT_MAX ) && UINT_MAX == 0xffffffffffffffff
  typedef unsigned int          uint_64t;
#else
/*
 * added by Erica because Borland C++ doens't fit into any of the previous macro
 * # error Please define uint_64t as an unsigned 64 bit type in tdefs.h
 */
  typedef unsigned long uint_64t;
#endif

#if defined( BUILD_DLL )
#  if defined( _MSC_VER ) || defined ( __INTEL_COMPILER )
#    define void_ret    __declspec( dllexport ) void __stdcall
#    define int_ret     __declspec( dllexport ) int  __stdcall
#  elif defined( __GNUC__ )
#    define void_ret    __declspec( __dllexport__ ) void
#    define int_ret     __declspec( __dllexport__ ) int
#  else
#    error The DLL build is only available on the MSVC, Intel and GCC compilers
#  endif
#elif defined( USE_DLL) 
#  if defined( _MSC_VER ) || defined ( __INTEL_COMPILER )
#    define void_ret    __declspec( dllimport ) void __stdcall
#    define int_ret     __declspec( dllimport ) int  __stdcall
#  elif defined( __GNUC__ )
#    define void_ret    __declspec( __dllimport__ ) void
#    define int_ret     __declspec( __dllimport__ ) int
#  else
#    error Use of the DLL is only available on the MSVC, Intel and GCC compilers
#  endif
#else
#  define void_ret  void
#  define int_ret   int
#endif

#define AES_128     /* define if AES with 128 bit keys is needed    */
#define AES_192     /* define if AES with 192 bit keys is needed    */
#define AES_256     /* define if AES with 256 bit keys is needed    */
#define AES_VAR     /* define if a variable key size is needed      */
#define AES_MODES   /* define if support is needed for modes        */

/* The following must also be set in assembler files if being used  */

#define AES_ENCRYPT /* if support for encryption is needed          */
#define AES_DECRYPT /* if support for decryption is needed          */
#define AES_ERR_CHK /* for parameter checks & error return codes    */
#define AES_REV_DKS /* define to reverse decryption key schedule    */

#define AES_BLOCK_SIZE  16  /* the AES block size in bytes          */
#define N_COLS           4  /* the number of columns in the state   */

/* The key schedule length is 11, 13 or 15 16-byte blocks for 128,  */
/* 192 or 256-bit keys respectively. That is 176, 208 or 240 bytes  */
/* or 44, 52 or 60 32-bit words.                                    */

#if defined( AES_VAR ) || defined( AES_256 )
#define KS_LENGTH       60
#elif defined( AES_192 )
#define KS_LENGTH       52
#else
#define KS_LENGTH       44
#endif

#if defined( AES_ERR_CHK )
#define aes_rval     int_ret
#else
#define aes_rval     void_ret
#endif

/* the character array 'inf' in the following structures is used    */
/* to hold AES context information. This AES code uses cx->inf.b[0] */
/* to hold the number of rounds multiplied by 16. The other three   */
/* elements can be used by code that implements additional modes    */

typedef union
{   uint_32t l;
    uint_8t b[4];
} aes_inf;

typedef struct
{   uint_32t ks[KS_LENGTH];
    aes_inf inf;
} aes_encrypt_ctx;

typedef struct
{   uint_32t ks[KS_LENGTH];
    aes_inf inf;
} aes_decrypt_ctx;

/* This routine must be called before first use if non-static       */
/* tables are being used                                            */

aes_rval gen_tabs(void);

/* Key lengths in the range 16 <= key_len <= 32 are given in bytes, */
/* those in the range 128 <= key_len <= 256 are given in bits       */

#if defined( AES_ENCRYPT )

#if defined(AES_128) || defined(AES_VAR)
aes_rval aes_encrypt_key128(const unsigned char *key, aes_encrypt_ctx cx[1]);
#endif

#if defined(AES_192) || defined(AES_VAR)
aes_rval aes_encrypt_key192(const unsigned char *key, aes_encrypt_ctx cx[1]);
#endif

#if defined(AES_256) || defined(AES_VAR)
aes_rval aes_encrypt_key256(const unsigned char *key, aes_encrypt_ctx cx[1]);
#endif

#if defined(AES_VAR)
aes_rval aes_encrypt_key(const unsigned char *key, int key_len, aes_encrypt_ctx cx[1]);
#endif

aes_rval aes_encrypt(const unsigned char *in, unsigned char *out, const aes_encrypt_ctx cx[1]);

#endif

#if defined( AES_DECRYPT )

#if defined(AES_128) || defined(AES_VAR)
aes_rval aes_decrypt_key128(const unsigned char *key, aes_decrypt_ctx cx[1]);
#endif

#if defined(AES_192) || defined(AES_VAR)
aes_rval aes_decrypt_key192(const unsigned char *key, aes_decrypt_ctx cx[1]);
#endif

#if defined(AES_256) || defined(AES_VAR)
aes_rval aes_decrypt_key256(const unsigned char *key, aes_decrypt_ctx cx[1]);
#endif

#if defined(AES_VAR)
aes_rval aes_decrypt_key(const unsigned char *key, int key_len, aes_decrypt_ctx cx[1]);
#endif

aes_rval aes_decrypt(const unsigned char *in, unsigned char *out, const aes_decrypt_ctx cx[1]);

#endif

#if defined(AES_MODES)

aes_rval aes_ecb_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, const aes_encrypt_ctx cx[1]);

aes_rval aes_ecb_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, const aes_decrypt_ctx cx[1]);

aes_rval aes_cbc_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, const aes_encrypt_ctx cx[1]);

aes_rval aes_cbc_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, const aes_decrypt_ctx cx[1]);

aes_rval aes_mode_reset(aes_encrypt_ctx cx[1]);

aes_rval aes_cfb_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, aes_encrypt_ctx cx[1]);

aes_rval aes_cfb_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, aes_encrypt_ctx cx[1]);

#define aes_ofb_encrypt aes_ofb_crypt
#define aes_ofb_decrypt aes_ofb_crypt

aes_rval aes_ofb_crypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, aes_encrypt_ctx cx[1]);

typedef void cbuf_inc(unsigned char *cbuf);

#define aes_ctr_encrypt aes_ctr_crypt
#define aes_ctr_decrypt aes_ctr_crypt

aes_rval aes_ctr_crypt(const unsigned char *ibuf, unsigned char *obuf,
            int len, unsigned char *cbuf, cbuf_inc ctr_inc, aes_encrypt_ctx cx[1]);

#endif

#if defined(__cplusplus)
}
#endif

#endif
