package uk.ac.ox.ctsu.syngo.client;

import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.SocketException;

/*
 * Simple class to make sure only one java cron job type is running at any one time.
 * 
 * It works by opening up a port on startup, if it fails to open the port, we know it is already open.
 * The porn numbers them selfs may have other things running on. So may need changing at some point.
 */
public class singleAppInstance
{
	static final int CMDQUERY 				= 48001;
	static final int CMDDECRYPT 			= 48002;
	static final int CMDLETTERS 			= 48003;
	static final int CMDINCIDENT 			= 48004;
	static final int CMDQUIZ 				= 48005;
	static final int CMDDOWNLOAD			= 48006;
	static final int CMDQUERYQUESTIONNAIRE	= 48007;
	
	static private ServerSocket ss = null;
	
/***********************************************************************************
 *                                       PUBLIC FUNCTIONS
 ***********************************************************************************/	
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
