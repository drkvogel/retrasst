package ctsu.pacs.test;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;
import java.security.spec.KeySpec;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Hex;

public class ASEncryption 
{
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

    public String getIV()
    {
    	return new String(new Hex().encode(iv));
    }
    
    ASEncryption(String passPhrase) throws Exception 
    {
        SecretKeyFactory factory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
        KeySpec spec = new PBEKeySpec(passPhrase.toCharArray(), SALT, ITERATION_COUNT, KEY_LENGTH);
        SecretKey tmp = factory.generateSecret(spec);
        secret = new SecretKeySpec(tmp.getEncoded(), "AES");
    }
    
    public void setEncrypt() throws Exception
    {
        ecipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        ecipher.init(Cipher.ENCRYPT_MODE, secret);
		iv = ecipher.getParameters().getParameterSpec(IvParameterSpec.class).getIV();
   }
    
    public void setDecrypt(String iv) throws Exception
    {
    	byte[] decodevi = (byte[]) new Hex().decode(iv);
    	
		AlgorithmParameterSpec paramSpec = new IvParameterSpec(decodevi);
        dcipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        dcipher.init(Cipher.DECRYPT_MODE, secret, paramSpec);    	
    }

    public String encrypt(String encrypt) throws Exception 
    {
    	if (encrypt.isEmpty())
    		return "";
        byte[] bytes = encrypt.getBytes();
        byte[] encrypted = encrypt(bytes);
        return new String(new Hex().encode(encrypted));
    }

    public byte[] encrypt(byte[] plain) throws Exception 
    {
        return ecipher.doFinal(plain);
    }

    public String decrypt(String encrypt) throws Exception 
    {
        byte[] bytes = new Hex().decode(encrypt.getBytes());
        byte[] decrypted = decrypt(bytes);
        return new String(decrypted);
    }

    public byte[] decrypt(byte[] encrypt) throws Exception 
    {
        return dcipher.doFinal(encrypt);
    }

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

