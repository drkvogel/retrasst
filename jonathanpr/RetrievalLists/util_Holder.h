#ifndef __util_Holder.h
#define __util_Holder.h

#include <vector>
#include <map>

/*
	Something to hold values which are derived from TObject

	For use in Comboboxes

	There must be a better way then this.
*/

class util_iHolder :public TObject
{
 public:
	util_iHolder(int i)
	{
		value = i;
	}

	int value;
 };

class util_sHolder :public TObject
{
 public:
	util_sHolder(std::vector<String> &mystrings)
	{
		m_strings = mystrings;
	}

	std::vector<String> m_strings;
 };

 class util_mapHolder : public TObject
 {
 public:
	util_mapHolder(std::map<String,String> &myMap)
	{
		m_map = myMap;
	}
	std::map<String,String> m_map;
 };

 #endif