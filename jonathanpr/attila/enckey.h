//---------------------------------------------------------------------------

#ifndef enckeyH
#define enckeyH
//---------------------------------------------------------------------------
#include <string>

class enckey
{
public:
	static std::string getDBEncKey();
	static std::string getROSSETTAEncKey();
private:
	static std::string gen(int seed);
};

#endif
