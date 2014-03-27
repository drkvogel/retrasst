package ctsu.pacs.test;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.SocketException;

public class singleAppInstance
{
	static final int CMDQUERY 				= 48001;
	static final int CMDDECRYPT 			= 48002;
	static final int CMDLETTERS 			= 48003;
	static final int CMDINCIDENT 			= 48004;
	static final int CMDQUIZ 				= 48005;
	static final int CMDDOWNLOAD			= 48006;
	static final int CMDQUERYQUESTIONNAIRE	= 48007;
	
	static ServerSocket ss = null;
	
	public static void instance(int port)
	{
		try
		{
			ss = new ServerSocket();
			ss.bind(new InetSocketAddress(port));
			System.out.println("Application started");
		}
		catch (SocketException e)
		{
			System.out.println("Application already running");
			System.exit(1);
		}
		catch(Exception e)
		{
			System.out.println("Application encountered some problem.");
			System.exit(1);
		}		
	}
}
