package ctsu.pacs.test;

public class encryptionKey
{
	static Long next = 1L;
	
	public int myrand() 
	{
	    next = next * 1103515245 + 12345;
	    return((int)(next/65536) % 32768);
	}

	public void mysrand(Long seed)
	{
	    next = seed;
	}

	public String getDBEncKey()
	{
		return gen(8355544L);
	}

	private String gen(Long seed)
	{
		mysrand(seed);
		StringBuilder ss = new StringBuilder();
		for (int i=0;i<256;i++)
		{
			char a = (char) ((myrand() % 70) + 48);
			ss.append(a);
		}

		String finalkey = toHex(ss.toString());		

		StringBuilder ss2 = new StringBuilder();
		for (int i=0;i<128;i++)
		{
			char a = (char) ((myrand() % 70) + 40);
			ss2.append(a);
		}
		return finalkey +=  toHex(ss2.toString());		
	}

	private String toHex(String s)
	{
		StringBuilder hexString = new StringBuilder();
		for (int i=0;i<s.length();i++) 
		{
			String hex = Integer.toHexString(0xFF & s.charAt(i));
			if(hex.length()==1) 
				hexString.append('0');
			hexString.append(hex);
		}
		return hexString.toString();
	}	
}
