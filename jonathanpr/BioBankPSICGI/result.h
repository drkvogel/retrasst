//---------------------------------------------------------------------------

#ifndef resultH
#define resultH

#include <cstdlib>
#include <string>
#include <stdlib.h>
#include "xtime.h"
//---------------------------------------------------------------------------

class Result
{
public:
	Result()
	{
		isError = false;
		text = "";
		id = -1;
	}
	~Result(){}

	bool isError;
	std::string text;
	int id;

	void setText(const std::string &newtext)
	{
		isError = false;
		text = newtext;
	}

	void setMessage(const std::string &newtext)
	{
		isError = true;
		text = newtext;
	}

	std::string getError() const
	{
		if (isError)
			return text;
		else
			return std::string("");
	}

	std::string getHTMLError() const
	{
		if (isError)
			return "<p>" + text + "</p>";
		return std::string("");
	}

	std::string getHTMLErrorWarning() const
	{
		if (!this->getError().empty())
			return "<span class ='red'>*</span>\n";
		return std::string("");
	}

	std::string getValueText() const
	{
		if ((!isError) && (!text.empty()))
			return std::string("value = \"") + text + std::string("\"");
		return std::string("");
	}

	std::string getText() const
	{
		if ((!isError) && (!text.empty()))
			return text;
		return std::string("");
	}

	int igetText() const
	{
		if (isError || text.empty())
			return -1;
		if (text == "0")
			return 0;
		int value = atoi(text.c_str());
		if (value == 0)
			return -1;
		return value;
	}
};


#endif

