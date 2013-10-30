package cardiffOxfordDataTransfer;

import java.util.HashSet;

/**
 * @author jonathanp
 *
 * Simple class to check for duplicate strings
 */
public class duplicateCheckVar
{
	HashSet dupTester = new HashSet();
	
	/**
	 * Keeps a list of strings, returns false if the new string 'varname' is already in the list 
	 * @param VarName	the string we want to check for duplicates with
	 * @return	false if can't add to set, i.e. is a duplicate
	 */
	//returns 
	public boolean isNotDuplicate(String VarName) 
	{
		return (dupTester.add(VarName));
	}
	
	/**
	 * used to check if this set had any content
	 * @return	TRUE if we have things in the list
	 */
	//
	public boolean sectionHasData()
	{
		return (dupTester.size() > 0);
	}
	
	/**
	 * @return
	 */
	public int size()
	{
		return (dupTester.size());
	}
	
	/**
	 * Empty the list.
	 */
	public void ClearForNewSet()
	{
		dupTester.clear();
	}
	
}
