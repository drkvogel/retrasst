package cardiffOxfordDataTransfer;

import java.util.Iterator;
import java.util.Vector;

public class IgnoreList
{
	static Vector m_ignoreList = new Vector();
	
	static
	{
		m_ignoreList.add("xmlError_hm[0-9]+");
	}
	
	public IgnoreList()
	{
		
	}
	
	static boolean shouldIgnore(String var)
	{
		Iterator it = m_ignoreList.iterator();
		while(it.hasNext())
		{
			if (var.matches((String) it.next()))
				return true;
		}
		return false;	
	}

}
