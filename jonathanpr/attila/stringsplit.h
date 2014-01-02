#ifndef stringsplith
#define stringsplith

// Split string into parts.
//stringsplit split("hello,world", ",");


class stringsplit : public std::vector<std::string>
{
public:
	stringsplit(const std::string& str, char* delimList)
	{
		size_t lastPos = 0;
		size_t pos = str.find_first_of(delimList);

		while (pos != std::string::npos)
		{
			if (pos != lastPos)
				push_back(str.substr(lastPos, pos-lastPos));
			lastPos = pos + 1;
			pos = str.find_first_of(delimList, lastPos);
		}
		if (lastPos < str.length())
			push_back(str.substr(lastPos, pos-lastPos));
	}
};

#endif
