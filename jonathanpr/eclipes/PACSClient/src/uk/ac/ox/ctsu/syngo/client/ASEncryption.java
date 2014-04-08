package uk.ac.ox.ctsu.syngo.client;

import java.security.spec.AlgorithmParameterSpec;
import java.security.spec.KeySpec;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Hex;

/*
 * Simple class to perform ASE encryption on strings and to reverse the operation.
 */
public class ASEncryption 
{
	//The salt which we are using. No significance to this value, also not a secret. 
    private static final byte[] SALT = 
    {
        (byte) 0xA9, (byte) 0x9B, (byte) 0x28, (byte) 0x32,
        (byte) 0x56, (byte) 0x25, (byte) 0xE3, (byte) 0x23
    };

    private static final int ITERATION_COUNT = 65536;
    private static final int KEY_LENGTH = 128;
    private Cipher ecipher;
    private Cipher dcipher;
    
    private byte[] iv;
    private  SecretKey secret;

	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/	
    
//The hex of the IV used to initialise the first block, store in the database against the row encrpyted
    public String getIV()
    {
    	return new String(new Hex().encode(iv));
    }
    
//Initialise the class, pass phase should be retrieved from encryptionkey class    
    public ASEncryption(String passPhrase) throws Exception 
    {
        SecretKeyFactory factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
        KeySpec spec = new PBEKeySpec(passPhrase.toCharArray(), SALT, ITERATION_COUNT, KEY_LENGTH);
        SecretKey tmp = factory.generateSecret(spec);
        secret = new SecretKeySpec(tmp.getEncoded(), "AES");
    }
    
//ready the encryption for a new row, generating a new IV.
    public void setEncrypt() throws Exception
    {
        ecipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        ecipher.init(Cipher.ENCRYPT_MODE, secret);
		iv = ecipher.getParameters().getParameterSpec(IvParameterSpec.class).getIV();
   }
    
//ready the decryption, pass in the iv of what you want to decrypt
    public void setDecrypt(String iv) throws Exception
    {
    	byte[] decodevi = (byte[]) new Hex().decode(iv);
    	
		AlgorithmParameterSpec paramSpec = new IvParameterSpec(decodevi);
        dcipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        dcipher.init(Cipher.DECRYPT_MODE, secret, paramSpec);    	
    }

//performs the encrpytion on the passed in string, returns a HEX's version of the encrpyted string
    public String encrypt(String encrypt) throws Exception 
    {
    	if (encrypt.isEmpty())
    		return "";
        byte[] bytes = encrypt.getBytes();
        byte[] encrypted = ecipher.doFinal(bytes);
        return new String(new Hex().encode(encrypted));
    }

//reverse the encrpyt(), takes an encrpyted HEX string and outputs the plan text string.
    public String decrypt(String encrypt) throws Exception 
    {
        byte[] bytes = new Hex().decode(encrypt.getBytes());
        byte[] decrypted = dcipher.doFinal(bytes);
        return new String(decrypted);
    }
    
/*********************************** TEST ANY OF THIS WORKS    ********************************************/    
    public static void main(String[] args) throws Exception 
    {
        String message = "SN0. 390";
        String password = new encryptionKey().getDBEncKey();

        ASEncryption encrypter = new ASEncryption(password);
        encrypter.setEncrypt();
        String vi = encrypter.getIV();
        String encrypted = encrypter.encrypt(message);
        encrypter.setDecrypt(vi);
        String decrypted = encrypter.decrypt(encrypted);

		String encrypted2 = encrypter.encrypt("23");
		String vi2 = encrypter.getIV();
		String encrypted3 = encrypter.encrypt("223");
		String vi3 = encrypter.getIV();
		String encrypted4 = encrypter.encrypt("2333");
		String vi4 = encrypter.getIV();
		String decrypted3 = encrypter.decrypt(encrypted2);
		String decrypted4 = encrypter.decrypt(encrypted3);
		String decrypted5 = encrypter.decrypt(encrypted4);
    
        encrypter.setDecrypt("e08bd667d1287be04f0cea8f1f032253");
        String decddddrypted = encrypter.decrypt("b289f650b89a43090d4d7166c41b23d3");      
        
        System.out.println("Encrypt(\"" + message + "\", \"" + password + "\") = \"" + encrypted + "\"");
        System.out.println("Decrypt(\"" + encrypted + "\", \"" + password + "\") = \"" + decrypted + "\"");
    }
}
