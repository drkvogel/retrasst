package cardiffOxfordDataTransfer;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.security.AlgorithmParameters;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.InvalidParameterSpecException;
import java.security.spec.KeySpec;
import java.util.HashMap;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * If you need greater then 128key length in java. You need "Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files". 
 * 
 * ULR:
 * http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html
 * http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html
 * 
 */

public class fileOperations
{
	private final static int SALT_LEN = 64;
	private byte[] mInitVec = null;
	private byte[] mSalt = null;
	private Cipher mEcipher = null;
	private Cipher mDecipher = null;
	private final int KEYLEN_BITS = 256; 
	private final int ITERATIONS = 65536;
	private final int MAX_FILE_BUF = 1024;
	private final static String APPENDEDFOOTER_STRING = "ƒî│ÉáPPEÑDèD";

	public fileOperations()
	{

	}
	
	/**
	 * @param filename
	 * @return The password from the file
	 * @throws IOException 
	 */
	//TO READ EMAIL SYSTEM PASSWORD and NOW ECRYPTION KEY PASSWORD IN CTSUCRYPT
	static public String ReadPWDFromFile(String filename) throws IOException
	{
		String line = "PWDNOTFOUND";
		BufferedReader in = null;
		try
		{
			in = new BufferedReader(new FileReader(filename));
			
			if ((line = in.readLine()) != null)
				return line;
		}
		catch (IOException e)
		{
		}
		finally
		{
			if (in!= null)
				in.close();
		}
	    return "PWDNOTFOUND";
		
/*		
		Path path = Paths.get(filename,null);
		Charset charset = Charset.forName("US-ASCII");
		BufferedReader reader = null;
		String line = null;
		try
		{
			reader = Files.newBufferedReader(path,charset);
			if ((line = reader.readLine()) != null)
			{
			    return line;
			}
		}
		catch (IOException e)
		{
		}
	    return "PWDNOTFOUND";
*/
	}
	
	/**
	 * @param filename
	 * @return the content of the file
	 * @throws IOException
	 */
	static public String ReadFileToString(String filename) throws IOException
	{
		StringBuilder StringBuild = new StringBuilder();
		String line = ""; 
		BufferedReader in = null;
		try
		{
			in = new BufferedReader(new FileReader(filename));
			
			while ((line = in.readLine()) != null)
				StringBuild.append(line);
		}
		catch (IOException e)
		{
		}
		finally
		{
			if (in != null)
				in.close();
		}
		return StringBuild.toString();
		
		
/*		
		StringBuilder StringBuild = new StringBuilder();
		Path path = Paths.get(filename,null);
		Charset charset = Charset.forName("US-ASCII");
		BufferedReader reader = null;
		String line = null;
		reader = Files.newBufferedReader(path,charset);
		while ((line = reader.readLine()) != null)
			StringBuild.append(line);

	    return StringBuild.toString();
*/
	}
	
	/**
	 * To compress a text file. Do this BEFORE Encrypting, else it won't compress it much if at all.
	 * This appends to any data already there. Should be header information.
	 * @param InFile 	The plan text file
	 * @param OutFile    The compressed file to create
	 * @throws IOException		FileNotFoundException, catch this especially
	 */
	public static void gzipFile(File InFile, File OutFile) throws IOException
	{
		FileInputStream fips = new FileInputStream(InFile);
		Reader r = new InputStreamReader(fips);

		FileOutputStream fops = new FileOutputStream(OutFile,true);
		Writer w = new OutputStreamWriter( new GZIPOutputStream(fops));

		int numRead;
		char []buf = new char[1024];
		while ( (numRead = r.read(buf) ) >= 0) 
		{
			w.write(buf, 0, numRead);
		}
		r.close();
		w.flush();		
		w.close();
	}
	
	/**
	 * To go from GZip back to Plain text
	 * @param InFile 	The GZiped file
	 * @param OutFile 	The destination uncompressed file
	 * @throws IOException		FileNotFoundException, catch this especially
	 */
	public static void ungipFile(File InFile, File OutFile) throws IOException
	{
		FileInputStream fips = new FileInputStream(InFile);
		Reader r = new InputStreamReader( new GZIPInputStream(fips));

		FileOutputStream fops = new FileOutputStream(OutFile);
		Writer w = new OutputStreamWriter(fops);

		int numRead;
		char []buf = new char[1024];
		while ( (numRead = r.read(buf) ) >= 0) 
		{
			w.write(buf, 0, numRead);
		}				
		r.close();
		w.flush();		
		w.close();
	}
	
	/**
	 * @return  	the max encrpytion key length java will allow.
	 * 			if it returns 128, and you want bigger, then you need to install "Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files"
	 */
	
	public static int getEncrpytionMaxKeyLength()
	{
		try
		{
			int iii = Cipher.getMaxAllowedKeyLength("AES") ;
			return iii;
		}
		catch (NoSuchAlgorithmException e2)
		{
			e2.printStackTrace();
		}
		return -1;
	}
	
	/**
	 * @return	the generated salt for this object
	 */
	public byte[] getSalt()
	{
		return mSalt;
	}

	/**
	 * @return 	the initialisation vector created from setupEncryption
	 */
	public byte[] getInitVec()
	{
		return mInitVec;
	}

	/**
	 * It creates a salt of SALT_LEN bytes and generates the salt bytes using secureRandom(). The encryption secret key is created along with the initialisation vectory. 
	 * @param password 
	 * 
	 * @throws NoSuchAlgorithmException
	 * @throws InvalidKeySpecException
	 * @throws NoSuchPaddingException
	 * @throws InvalidParameterSpecException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 * @throws UnsupportedEncodingException
	 * @throws InvalidKeyException
	 */
	public void setupEncrypt(String password) throws NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, InvalidParameterSpecException, IllegalBlockSizeException, BadPaddingException, UnsupportedEncodingException, InvalidKeyException
	{
		SecretKeyFactory factory = null;
		SecretKey tmp = null;

		// crate secureRandom salt and store as member var for later use
		mSalt = new byte[SALT_LEN];
		SecureRandom rnd = new SecureRandom();
		rnd.nextBytes(mSalt);

		factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");

		// Derive the key, given password and salt.		 
		KeySpec spec = new PBEKeySpec(password.toCharArray(), mSalt, ITERATIONS, KEYLEN_BITS);
		tmp = factory.generateSecret(spec);
		SecretKey secret = new SecretKeySpec(tmp.getEncoded(), "AES");

		// Create the Encryption cipher object and store as a member variable
		mEcipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		mEcipher.init(Cipher.ENCRYPT_MODE, secret);
		AlgorithmParameters params = mEcipher.getParameters();

		// get the initialisation vector and store as member var
		mInitVec = ((Cipher) params.getParameterSpec(IvParameterSpec.class)).getIV();
	}

	/**
	 * If a file is being decrypted, we need to know the password, the salt and the initialisation vector (iv). We have the password from initialising  the class. pass the iv and salt here which is obtained when encrypting  the file initially.
	 * @param password 
	 * 
	 * @param initvec
	 * @param salt
	 * @throws NoSuchAlgorithmException
	 * @throws InvalidKeySpecException
	 * @throws NoSuchPaddingException
	 * @throws InvalidKeyException
	 * @throws InvalidAlgorithmParameterException
	 */
	/*
	public void setupDecrypt(String password,String initvec, String salt) throws NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, InvalidKeyException, InvalidAlgorithmParameterException
	{
		SecretKeyFactory factory = null;
		SecretKey tmp = null;
		SecretKey secret = null;

		mSalt = javax.xml.bind.DatatypeConverter.parseBase64Binary(salt);
//		System.out.println("got salt " + javax.xml.bind.DatatypeConverter.printBase64Binary(mSalt));

		mInitVec = javax.xml.bind.DatatypeConverter.parseBase64Binary(initvec);		
//		System.out.println("got initvector :" + javax.xml.bind.DatatypeConverter.printBase64Binary(mInitVec));

		factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
		KeySpec spec = new PBEKeySpec(password.toCharArray(), mSalt, ITERATIONS, KEYLEN_BITS);

		tmp = factory.generateSecret(spec);
		secret = new SecretKeySpec(tmp.getEncoded(), "AES");

		mDecipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		mDecipher.init(Cipher.DECRYPT_MODE, secret, new IvParameterSpec(mInitVec));
	}
*/
	/**
	 * @param input 	- the cleartext file to be encrypted
	 * @param output 	- the encrypted data file
	 * @throws IOException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 */
	public void WriteEncryptedFile(File input, File output) throws IOException, IllegalBlockSizeException, BadPaddingException
	{
		FileInputStream fin;
		FileOutputStream fout;
		int nread = 0;
		byte[] inbuf = new byte[MAX_FILE_BUF];

		fout = new FileOutputStream(output,true); //APPENDING, AS WE SHOULD HAVE A HEADER
		fin = new FileInputStream(input);

		while ((nread = fin.read(inbuf)) > 0)
		{
			// create a buffer to write with the exact number of bytes read.
			// Otherwise a short read fills inbuf with 0x0 and results in full blocks of MAX_FILE_BUF being written.
			byte[] trimbuf = new byte[nread];
			for (int i = 0; i < nread; i++)
				trimbuf[i] = inbuf[i];

			// encrypt the buffer using the cipher obtained previosly
			byte[] tmp = mEcipher.update(trimbuf);

			// just in case..
			if (tmp != null)
				fout.write(tmp);
		}

		// finalize the encryption since we've done it in blocks of MAX_FILE_BUF
		byte[] finalbuf = mEcipher.doFinal();
		if (finalbuf != null)
			fout.write(finalbuf);

		fout.flush();
		fin.close();
		fout.close();
		fout.close();
	}

	/**
	 * Write the cleartext buffer back out to disk as (output) File.
	 * 
	 * @param input   - File object representing encrypted data on disk
	 * @param output  - File object of clear text data to write out after decrypting
	 * @param headersize 
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 * @throws IOException
	 */
	public void ReadEncryptedFile(File input, File output, long headersize) throws IllegalBlockSizeException, BadPaddingException, IOException
	{
		FileInputStream fin;
		FileOutputStream fout;
		CipherInputStream cin;
		int nread = 0;
		byte[] inbuf = new byte[MAX_FILE_BUF];

		fout = new FileOutputStream(output);
		fin = new FileInputStream(input);
		fin.getChannel().position(headersize); //skip header
		cin = new CipherInputStream(fin, mDecipher);

		while ((nread = cin.read(inbuf)) > 0)
		{
			// create a buffer to write with the exact number of bytes read.  Otherwise a short read fills inbuf with 0x0
			byte[] trimbuf = new byte[nread];
			for (int i = 0; i < nread; i++)
				trimbuf[i] = inbuf[i];

			fout.write(trimbuf);
		}

		fout.flush();
		cin.close();
		fin.close();
		fout.close();
	}
	
	/**
	 * Checks to see if the file has a header. If so, it reads out the Salt and the iv.
	 * @param Salt
	 * @param iv
	 * @return length of header, -1 if problem!
	 */
	
	private static HashMap readAESFileInfo(File Filename)
	{
		try
		{
			ObjectInputStream bw = null;

			FileInputStream fs = new FileInputStream(Filename);
			bw = new ObjectInputStream(fs);
			String s = bw.readUTF();
			if (s.compareTo(APPENDEDFOOTER_STRING)!=0)
			{
				bw.close();
				return null;
			}
			 
			HashMap myMap = new HashMap();
			myMap.put("salt",bw.readUTF()); 
			myMap.put("iv", bw.readUTF()); 
			myMap.put("pos", Long.toString(fs.getChannel().position()));
			bw.close();
			return myMap;
		}
		catch (IOException e)
		{
			// Not a valid header, or cruppted file
			System.out.println("Error: Input File looks like it has been currupted.");	
			System.out.println();	
			e.printStackTrace();
		}
		return null;
	}
	
	private static void writeAESFileInfo(File Filename,String Salt, String iv)
	{
		try
		{
			ObjectOutputStream bw = null;

			//append this to the file
			 bw = new ObjectOutputStream(new FileOutputStream(Filename));
			 bw.writeUTF(APPENDEDFOOTER_STRING);		 
			 bw.writeUTF(Salt);
			 bw.writeUTF(iv);
			 bw.flush();
			 bw.close();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * Deletes a file, ignores errors
	 * @param theFile
	 */
	
	private static void deleteFile(File theFile)
	{
		try
		{
			theFile.delete();
		}
		catch(Exception e)
		{}	
	}
	
	/**
	 * @param inputFile
	 * @param outputFile
	 * @param password
	 * @throws InvalidKeyException
	 * @throws NoSuchAlgorithmException
	 * @throws InvalidKeySpecException
	 * @throws NoSuchPaddingException
	 * @throws InvalidParameterSpecException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 * @throws IOException
	 */
	/*
	public static void compressAndEncryptFile(File inputFile, File outputFile, String password) throws InvalidKeyException, NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, InvalidParameterSpecException, IllegalBlockSizeException, BadPaddingException, IOException 
	{
		File tempFile = File.createTempFile("tempFile",".dat");
		tempFile.deleteOnExit();

		//try and clean up any old files from other runs. As we append.
		deleteFile(outputFile);
		
		fileOperations en = new fileOperations();
		en.setupEncrypt(password);			
		String iv = javax.xml.bind.DatatypeConverter.printBase64Binary(en.getInitVec());
		String salt = javax.xml.bind.DatatypeConverter.printBase64Binary(en.getSalt());
		//create the output file, add header, salt and iv
		writeAESFileInfo(outputFile,salt,  iv); //create outputfile with header
		fileOperations.gzipFile(inputFile, tempFile); //gzip file data to a temp file
		en.WriteEncryptedFile(tempFile, outputFile); //encrypt the temp file and append to header information.
	}
	*/
	/**
	 * @param inputFile
	 * @param outputFile
	 * @param password
	 * @throws InvalidKeyException
	 * @throws NoSuchAlgorithmException
	 * @throws InvalidKeySpecException
	 * @throws NoSuchPaddingException
	 * @throws InvalidAlgorithmParameterException
	 * @throws IOException
	 * @throws IllegalBlockSizeException
	 * @throws BadPaddingException
	 */
	/*
	public static void decryptAndUncompressFile(File inputFile, File outputFile, String password) throws InvalidKeyException, NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, InvalidAlgorithmParameterException, IOException, IllegalBlockSizeException, BadPaddingException
	{
		File tempFile = File.createTempFile("tempFile",".dat");
		tempFile.deleteOnExit();
		//try and clean up any old files from other runs. As we append. 
		deleteFile(outputFile);
		
		fileOperations en = new fileOperations();				
		HashMap info = readAESFileInfo(inputFile);	
		if (info==null)
			throw new IOException("Header information not found in " + inputFile.getAbsolutePath());
		
		String iv = (String) info.get("iv");
		String salt = (String) info.get("salt");	
		long headSize = Long.parseLong((String) info.get("pos"));	
			
		if (headSize == -1)
		{
			throw new IOException("Header information not found in " + inputFile.getAbsolutePath());
		}
		en.setupDecrypt(password,iv, salt);		
		en.ReadEncryptedFile(inputFile, tempFile,headSize);
		fileOperations.ungipFile(tempFile, outputFile);
	}*/
	
	private static void printUseageAndQuit()
	{
		System.out.println("CTSU CRYPT - AES256 encryption and compression utility");
		System.out.println();
		System.out.println("ctsucrypt [-e|-d] input_file output_file [hex_key|key_file]");
		System.out.println("");
		System.out.println("	-e				encrypt");
		System.out.println("	-d				decrypt");
		System.out.println("	input_file			the source file");
		System.out.println("	output_file			the resulting file");
		System.out.println("	hex_key				a 64-character hexadecimal string");
		System.out.println("	key_file			A text file containing a 64-character hexadecimal string");
		System.out.println("");
		if (fileOperations.getEncrpytionMaxKeyLength() < 256)
		{
			System.out.println("Note:");
			System.out.println("	- You You need \"Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files\" installed to get greater then 128 key length encrpytion in Java.");
			System.out.println("	- If you are running java 7 you can download it from - http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html");				
		}
		System.exit(0);
	}

	//-e c:/temp/carack.xml c:/temp/carack2.ase 1234567890123456789012345678901234567890123456789012345678901234
	//-d c:/temp/carack2.ase c:/temp/carack2.xml 1234567890123456789012345678901234567890123456789012345678901234
	/*
	public static void main(String[] args)
	{
		if (args.length != 4)
		{
			printUseageAndQuit();
		}
		
		if (fileOperations.getEncrpytionMaxKeyLength() < 256)
		{
			System.out.println();	
			System.out.println("Error:");
			System.out.println("	- You You need \"Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files\" installed to get greater then 128 key length encrpytion in Java. This is a matter of U.S. policy and U.S. export controls (not due to technical reasons).");
			System.out.println("	- If you are running java 7 you can download it from - http://www.oracle.com/technetwork/java/javase/downloads/jce-7-download-432124.html");				
			System.exit(0);
		}
	
		String Op = args[0];
		String input_file = args[1];
		String output_file = args[2];
		String password = args[3];

		//check these files
		File input = new File(input_file);
		if (!input.exists())
		{
			System.out.println();	
			System.out.println("Error: input_file not found.");	
			System.out.println();	
			printUseageAndQuit();
		}
		
		if ((!input.canRead()))
		{
			System.out.println();	
			System.out.println("Error: input_file can't be read.");	
			System.out.println();	
			printUseageAndQuit();
		}
		
		File output = new File(output_file);
		if (output.exists())
		{
			System.out.println();	
			System.out.println("Error: output_file already exsists.");	
			System.out.println();	
			printUseageAndQuit();
		}
		
		File pwd = new File(password);
		String encryptionPWD = null;
		if (pwd.exists())
		{
			if ((!pwd.canRead()))
			{
				System.out.println();	
				System.out.println("Error: password file can't be read.");	
				System.out.println();	
				printUseageAndQuit();
			}		
			encryptionPWD = ReadPWDFromFile(password);
		}
		else
			encryptionPWD = password;
						
		if (encryptionPWD.length() != 64)
		{
			System.out.println();	
			System.out.println("Error: password length isn't 64 bits. [" + encryptionPWD + "]");	
			System.out.println();	
			printUseageAndQuit();		
		}
		
		if (Op.compareTo("-e")==0)
		{//Encryption mode
			try
			{
				fileOperations.compressAndEncryptFile(input, output, encryptionPWD);
			}
			catch (InvalidKeyException | IllegalBlockSizeException | BadPaddingException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidParameterSpecException | IOException e)
			{
				System.out.println("Error: ");	
				e.printStackTrace();
			}			
		}
		else if (Op.compareTo("-d")==0)
		{//decryption mode
			try
			{
				fileOperations.decryptAndUncompressFile(input, output, encryptionPWD);
				System.out.println();	
				System.out.println("Success");
			}
			catch (ZipException | StreamCorruptedException e)
			{
				System.out.println("Error: Password was not valid, or input file has been currupted.");	
				System.out.println();	
				e.printStackTrace();
			}		
			catch (InvalidKeyException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidAlgorithmParameterException | IllegalBlockSizeException | BadPaddingException | IOException e)
			{
				e.printStackTrace();
			}
		}
		else
		{
			printUseageAndQuit();
		}

*/
		/* TEST PROGRAM
		File input = new File("c:\\temp\\carack.xml");
		File eoutput = new File("c:\\temp\\carack.aes");		
		File eoutputTemp = new File("c:\\temp\\carackTemp.aes");
		File doutput = new File("c:\\temp\\carack.txt");
		
		try
		{
			FileOpperations.compressAndEncryptFile(input,eoutputTemp, eoutput, "THEPASSWORD");
		}
		catch (InvalidKeyException | IllegalBlockSizeException | BadPaddingException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidParameterSpecException | IOException e)
		{
			e.printStackTrace();
		}
	
		try
		{
			FileOpperations.decryptAndUncompressFile(eoutput, eoutputTemp, doutput, "THEPASSWORD");
		}
		catch (InvalidKeyException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidAlgorithmParameterException | IllegalBlockSizeException | BadPaddingException | IOException e)
		{
			e.printStackTrace();
		}
		*/
	/*
	}
*/
}
