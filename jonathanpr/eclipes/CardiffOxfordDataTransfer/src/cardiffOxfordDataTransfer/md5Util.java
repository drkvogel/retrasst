package cardiffOxfordDataTransfer;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

/**
 * A basic MD5 help class to help check/generate MD5 values.
 */

public class md5Util
{
	/**
	 * Returns true if we generate the same MD5 checksum as you supply.
	 * 
	 * @param MD5Checksum	The MD5 string to be validated against
	 * @param Data			The data structure which we generate the string to MD5 
	 * @return Returns true of the Data generates the same md5 as MD5Checksum			
	 * @throws NoSuchAlgorithmException
	 */
	static public boolean checkMD5(String MD5Checksum, TreeMap Data) throws NoSuchAlgorithmException
	{
		String inputString = buildUpTheTextStringToMD5(Data);
		String newMD5 = generateMD5(inputString);
		
		if (newMD5.compareTo(MD5Checksum)!=0)
			System.out.println(newMD5 + ":" + MD5Checksum + ":" + inputString);
		return (newMD5.compareTo(MD5Checksum)==0);
	}

	
	/**
	 * Returns an MD5 string in the same format as in the specification document.
	 * 
	 * @param Data			The data structure which we generate the string to MD5 
	 * @return	The MD5 string
	 * @throws NoSuchAlgorithmException
	 */
	static public String generateMD5(TreeMap Data) throws NoSuchAlgorithmException
	{
		String inputString = buildUpTheTextStringToMD5(Data);
		return generateMD5(inputString);
	}
	
	static private String buildUpTheTextStringToMD5(TreeMap Data)
	{
		String returnString = "#"; 
		Iterator it = Data.entrySet().iterator();
		while (it.hasNext())
		{
			Map.Entry pairs = (Map.Entry) it.next();
			returnString += pairs.getKey() + "=" + pairs.getValue() + "#";
		}
//		System.out.println("test string:"+returnString);
		return returnString;
	}
	
	/**
	 * Output conforms to the ROSIG.getSignature()
	 * @param toMD5 the string to get an MD5 from
	 * @return MD5 as a HEX string
	 * @throws NoSuchAlgorithmException
	 */
	static public String generateMD5(String toMD5) throws NoSuchAlgorithmException
	{
		java.security.MessageDigest algorithm = MessageDigest.getInstance("MD5");
		
		algorithm.reset();
		algorithm.update(toMD5.getBytes());
		byte messageDigest[] = algorithm.digest();

		StringBuffer hexString = new StringBuffer();
		for (int i=0;i<messageDigest.length;i++) 
		{
			String hex = Integer.toHexString(0xFF & messageDigest[i]);
			if(hex.length()==1) 
				hexString.append('0');
			hexString.append(hex);
		}		
		return hexString.toString();
	}
	
	/**
	 * @param Data  The variables to create the MD5 signature out of 
	 * @return MD5 signature which conforms to the ROSETTA 
	 * @throws NoSuchAlgorithmException
	 */
	static public String getSignature(TreeMap Data) throws NoSuchAlgorithmException
	{
		String inputString = buildUpTheTextStringToMD5(Data);
		return "MD5:" + generateMD5(inputString);
	}
}
