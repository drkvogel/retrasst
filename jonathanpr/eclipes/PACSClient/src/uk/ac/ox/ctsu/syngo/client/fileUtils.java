package uk.ac.ox.ctsu.syngo.client;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/*
 * File based utility functions. 
 */
public final class fileUtils
{	
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/	
	//Zips everything in a directory, including sub directories
    public static int zipDir(String directory, String zipName) throws IOException 
    {
    	int size = 0;
        ZipOutputStream zos = new ZipOutputStream(new FileOutputStream(zipName));
        String path = "";
        size = zipDir(directory, zos, path);
        zos.close();
        return size;
    }
    
    //Returns the filesize of a file
    public static long getFileSize(String file)
    {
    	File f = new File(file);
    	if (!f.isFile())
    		return -1;
    	return f.length();
    }
    
    //Deletes all files and directories
    public static void deleteFileRecursive(File file) throws IOException
    {
    	if(file.isDirectory())
    	{
    		//directory is empty, then delete it
    		if(file.list().length==0)
    		   file.delete();
    		else
    		{
    		   //list all the directory contents
        	   String files[] = file.list(); 
        	   for (String temp : files) 
        	   {
        	      File fileDelete = new File(file, temp);
        	      deleteFileRecursive(fileDelete);
        	   }
 
        	   //check the directory again, if empty then delete it
        	   if(file.list().length==0)
        	   {
        		   if (!file.delete())
        			   throw new IOException("Failed to delete directory: " + file.getName());
        	   }       			   
    		}
    	}
    	else
    	{
    		//if file, then delete it
    		if (!file.delete())
    			throw new IOException("Failed to delete directory: " + file.getName());
    	}
    }
       
    //Takes an MD5 of a file, returns a HEX string of the result.
    public static String checksumFile(String filename) throws NoSuchAlgorithmException, IOException
    {
    	InputStream fis =  new FileInputStream(filename);

		byte[] buffer = new byte[1024];
		MessageDigest complete = MessageDigest.getInstance("MD5");
		int numRead;
		do 
		{
			numRead = fis.read(buffer);
			if (numRead > 0) 
				complete.update(buffer, 0, numRead);
		} 
		while (numRead != -1);
		
		fis.close();
		byte messageDigest[] = complete.digest();
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

	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	*************************************************************************************/
    private static int zipDir(String directory, ZipOutputStream zos, String path) throws IOException 
    {
    	int size = 0;
        File zipDir = new File(directory);
        String[] dirList = zipDir.list();
        byte[] readBuffer = new byte[2156];
        int bytesIn = 0;

        for (int i = 0; i < dirList.length; i++) 
        {
            File f = new File(zipDir, dirList[i]);
            if (f.isDirectory()) 
            {
                String filePath = f.getPath();
                size += zipDir(filePath, zos, path + f.getName() + "/");
                continue;
            }
            FileInputStream fis = new FileInputStream(f);
            try 
            {
                ZipEntry anEntry = new ZipEntry(path + f.getName());
                zos.putNextEntry(anEntry);
                bytesIn = fis.read(readBuffer);
                while (bytesIn != -1) 
                {
                	size += bytesIn;
                    zos.write(readBuffer, 0, bytesIn);
                    bytesIn = fis.read(readBuffer);
                }
            } 
            finally
            {
                fis.close();
            }
        }
        return size;
    }   
}

/*
 * THIS IS NO LONGER REQUIRED! IT WORKS, hiding it out the way
 *	
public int testGetDistanceMatch(String rawPatientID, String rawFirstName, String dbPatientID, String dbFirstName)
{
	if (dbPatientID.compareToIgnoreCase(rawPatientID) == 0)
		return 0;
	if (dbFirstName.compareToIgnoreCase(rawFirstName) == 0)
		return 0;
	
	int pidDist = stringUtils.getLevenshteinDistance(rawPatientID,dbPatientID); 
	int fnDist = stringUtils.getLevenshteinDistance(rawFirstName,dbFirstName); 
	
	if ((pidDist > 2) && (fnDist > 2))
		return -1;
	
	int dist = pidDist + fnDist;				
	return dist;
}


public boolean tryExtensiveMatchOnPatientID(String[] PatientID, String[] FirstName, Calendar dicomDate) throws SQLException
{ 
	if (((PatientID[0] == null) || (PatientID[0].isEmpty())) && ((FirstName[0] == null) || (FirstName[0].isEmpty())))  
		return false;
	
	
	//get all firstnames and patient IDS due to be scanned for that day.
	//make both upper, if same.. YEAH!
	
	//see if any of them are close..both have to have NO MORE THEN 1 or two miss typed letters MAX!
	
	Vector<String> patientids = new Vector<String>(); 
	Vector<String> firstnames = new Vector<String>(); 
	Vector<Integer> piddistance = new Vector<Integer>(); 
	Vector<Integer> fnddistance = new Vector<Integer>(); 
															//2014-01-28
	SimpleDateFormat dateformatBuilding = new SimpleDateFormat("yyyy-MM-dd");
	StringBuilder dateString = new StringBuilder( dateformatBuilding.format( dicomDate.getTime() ) );
	
	String sql = "SELECT patientid,firstname,char(scandate) AS scandate FROM patientquiz WHERE char(scandate) LIKE '" + dateString.toString() + "%'";
	ResultSet result = null;
	try
	{
		result = m_stmt.executeQuery(sql);
		while (result.next())
		{	
			String pid = result.getString("patientid");
			String fn = result.getString("firstname");
			
			if (pid.compareToIgnoreCase(PatientID[0]) == 0)
			{ //we have a match!
				PatientID[1] = pid;
				FirstName[1] = fn;
				return true;
			}
			if (fn.compareToIgnoreCase(FirstName[0]) == 0)
			{ //we have a match!
				PatientID[1] = pid;
				FirstName[1] = fn;
				return true;
			}
			
			int pidDist = stringUtils.getLevenshteinDistance(PatientID[0],pid); 
			int fnDist = stringUtils.getLevenshteinDistance(FirstName[0],fn); 
			
			if ((pidDist > 2) && (fnDist > 2))
				continue; //more then 2 changes.. too many..

			patientids.add(pid);
			firstnames.add(fn); 
			piddistance.add(pidDist);
			fnddistance.add(fnDist);				
		}
	}
	finally
	{
		if (result!=null)
			result.close();
	}
	
	if (patientids.size() == 0)
		return false;
	
	if (patientids.size() == 1)
	{
		PatientID[1] = patientids.firstElement();
		FirstName[1] = firstnames.firstElement();
	}
	//if two have the same score, then give up.. as there is no way of knowing
	int lowestscore = Integer.MAX_VALUE;
	boolean duplicate = false;
	int lowestIndex = -1;
	for (int i=0;i<patientids.size();i++)
	{
		int totalscore = piddistance.elementAt(i) + fnddistance.elementAt(i); 
		if (lowestscore == totalscore)
			duplicate = true;
		else if (lowestscore > totalscore)
		{
			lowestIndex = i;
			duplicate = false;
			lowestscore = totalscore;
		}
	}
	
	if (duplicate)
		return false;
	
	PatientID[1] = patientids.get(lowestIndex);
	FirstName[1] = firstnames.get(lowestIndex);
	return true;
}
*/
