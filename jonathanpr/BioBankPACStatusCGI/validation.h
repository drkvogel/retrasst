#ifndef validationH
#define validationH

#include <sstream>
#include "xtime.h"

class validation
{
public:

	static bool isNHSNumber(const std::string &number)
	{
		if (number.length() != 12)
			return false;

		stringsplit split(number, "-");
		if (split.size() != 3)
			return false;

		if (split[0].length() != 3)
			return false;
		if (split[1].length() != 3)
			return false;
		if (split[2].length() != 4)
			return false;

		for (int i=0;i<3;i++)
		{
			for (unsigned int j=0;j<split[i].length();j++)
			{
				if (split[i][j] < '0' || split[i][j] > '9')
					return false;
			}
		}
		return true;
	}

	static bool isNumeric(const std::string &number)
	{
		if (number.length() == 0)
			return false;
		for (unsigned int i=0;i<number.length();i++)
		{
			if (number[i] < '0' || number[i] > '9')
				return false;
		}
		return true;
	}

	static bool isPhoneNumber(const std::string &number)
	{
		int digitalCount = 0;
		for (unsigned int i=0;i<number.length();i++)
		{
			char c = number[i];
			if ((c < '0' || c > '9') && (c != '(' && c != ')' && c != ' '))
				return false;

			if (number[i] >= '0' && number[i] <= '9')
				digitalCount++;
		}
		if (digitalCount < 6)
			return false; //too short for any kind of number, even without the STD code.
		if (number.length() > 20)
			return false;  //too big for DB
		return true;
	}

	static bool isDOB(Result &date, int MinAge = 5, int MaxAge = 112)
	{
	//	(DD/MM/YYYY)
		if (date.getText().length() != 10)
		{
			date.setMessage("DOB format needs to be DD/MM/YYYY");
			return false;
		}

		stringsplit split(date.getText(), "//");
		if (split.size() != 3)
		{
			date.setMessage("DOB format needs to be DD/MM/YYYY");
			return false;
		}

		int day = atoi(split[0].c_str());
		int month = atoi(split[1].c_str());
		int year = atoi(split[2].c_str());

		if (day == 0 || month == 0 || year == 0)
		{
			date.setMessage("DOB format needs to be DD/MM/YYYY");
			return false;
		}

		XDATE DOB = XDATE(year, month, day );
		if (!DOB.isValid())
		{
			date.setMessage("DOB isn't a valid date");
			return false;
		}

		if ((DOB.getAge() <= MinAge) || (DOB.getAge() > MaxAge))//under 5 or over 112? see http://en.wikipedia.org/wiki/List_of_British_supercentenarians
		{
			std::stringstream ss;
			ss << "DOB gives an age of " << DOB.getAge() << " which is invalid. Acceptable ages are between " << MinAge << " and " << MaxAge << ".";
			date.setMessage(ss.str());
			return false;
		}
		return true;
	}


	static bool stringToXDATE(const std::string &datestring, XDATE &Date)
	{
	//	(DD/MM/YYYY)
		if (datestring.length() != 10)
			return false;

		stringsplit split(datestring, "//");
		if (split.size() != 3)
			return false;

		int day = atoi(split[0].c_str());
		int month = atoi(split[1].c_str());
		int year = atoi(split[2].c_str());

		if (day == 0 || month == 0 || year == 0)
			return false;

		Date = XDATE(year, month, day );
		return Date.isValid();
	}


	// return 0 for OK, -1 too small, 1 too big
	static int isVARCHAR(unsigned int MinSize,unsigned  int MaxSize,const std::string &toTest)
	{
		if (toTest.length() <= MinSize)
			return -1;
		if (toTest.length() > MaxSize)
			return 1;
		return 0;
	}

	static bool isINT(unsigned int MinSize,unsigned int MaxSize,const std::string &toTest)
	{
		if (!isNumeric(toTest))
			return false;

//special check for zero
		if (toTest == "0")
			return true;

		long int value;
		value = strtol(toTest.c_str(),0,10);

		if (value == 0)
			return false;
		if (value <= (long int)MinSize)
			return false;
		if (value > (long int)MaxSize)
			return false;
		return true;
	}
};

#endif
