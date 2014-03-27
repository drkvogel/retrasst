//---------------------------------------------------------------------------

#pragma hdrstop


#include "enckey.h"
#include <stdlib.h>     /* srand, rand */
#include <sstream>
#include "xmd5.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)



static unsigned long next = 1;
int myrand(void)  /* RAND_MAX assumed to be 32767. */
{
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}


void mysrand(unsigned seed)
{
    next = seed;
}

std::string enckey::getDBEncKey()
{
	return gen(2348644);
}

std::string enckey::getROSSETTAEncKey()
{
	return gen(16651029);
}

//generate a series of repeatable strings, which we MD5 to get
//the 64 bit key
std::string enckey::gen(int seed)
{
	mysrand(seed);
	std::stringstream ss;
	for (int i=0;i<256;i++)
	{
		char a = (myrand() % 70) + 48;
		ss << a;
	}
	std::string s = ss.str();
	std::string finalkey = XMD5::calcHex((const unsigned char*)s.c_str(),s.length());

	std::stringstream ss2;
	for (int i=0;i<128;i++)
	{
		char a = (myrand() % 70) + 40;
		ss2 << a;
	}
	s = ss2.str();
	return finalkey += XMD5::calcHex((const unsigned char*)s.c_str(),s.length());
}





