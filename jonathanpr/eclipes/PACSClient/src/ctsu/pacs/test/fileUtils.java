package ctsu.pacs.test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public final class fileUtils
{
    public static int zipDir(String directory, String zipName) throws IOException 
    {
    	int size = 0;
        ZipOutputStream zos = new ZipOutputStream(new FileOutputStream(zipName));
        String path = "";
        size = zipDir(directory, zos, path);
        zos.close();
        return size;
    }
    
    public static long getFileSize(String file)
    {
    	File f = new File(file);
    	if (!f.isFile())
    		return -1;
    	return f.length();
    }

    public static int zipDir(String directory, ZipOutputStream zos, String path) throws IOException 
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
    
    public static void deleteFileRecursive(File file) throws IOException
    {
    	if(file.isDirectory())
    	{
    		//directory is empty, then delete it
    		if(file.list().length==0)
    		{
    		   file.delete();
    		   System.out.println("Directory is deleted : " + file.getAbsolutePath());
    		}
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
}

   
