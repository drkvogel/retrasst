package uk.ac.ox.ctsu.paulst;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.net.Authenticator;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.TimeUnit;

public class App 
{
    public static void main( String[] args )
    {
        try
        {
            if ( args.length < 2 )
            {
                throw new RuntimeException("Must supply url filename and thread pool size as args");
            }
            Authenticator.setDefault( new Auth() );
            String urlFile = args[0];
            int threads = Integer.parseInt( args[1] );
            BufferedReader urlReader = new BufferedReader( new FileReader( new File( urlFile ) ) );
            ExecutorService threadPool = Executors.newFixedThreadPool(threads);
            for ( String url = urlReader.readLine(); url != null; url = urlReader.readLine() )
            {
                threadPool.submit( new FetchTask( url ) );
            }
            threadPool.shutdown();
            threadPool.awaitTermination( 30, TimeUnit.SECONDS );
        }
        catch( Exception e )
        {
            System.err.println( e.getMessage() );
        }
    }
}

