//---------------------------------------------------------------------------

#ifndef OptionH
#define OptionH

#include <cstdlib>
#include <string>

//---------------------------------------------------------------------------
class option
{
protected:
	int id;
	std::string name;

public:
	const int getID() const { return id; }
	std::string getName() const { return name; }
};

//---------------------------------------------------------------------------
#endif

