package uk.ac.ox.ctsu.syngo.client;

/*
 * A class to generate an encryption key
 * 
 * It returns a key to be used by the AES encrpytion in the database. Generates the key each time, rather then hardcoding a string with in the source.
 */
public class encryptionKey
{
	static Long next = 1L;

	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/	

	//Call this to get the key used in the database. 
	public String getDBEncKey()
	{
		return gen(8355544L);
	}

	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/	
//rand() has non-standard implementation, so differs in different c libraries, so to ensure we always get the same series of numbers returned, we have written our own.
	private int myrand() 
	{
	    next = next * 1103515245 + 12345;
	    return((int)(next/65536) % 32768);
	}

	private void mysrand(Long seed)
	{
	    next = seed;
	}
	
	//For a given seed, generate the key
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
    
	//Converts a string to Hex
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
