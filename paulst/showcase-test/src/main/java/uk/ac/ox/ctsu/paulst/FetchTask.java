package uk.ac.ox.ctsu.paulst;

import java.net.URI;
import java.net.URL;
import java.net.URLConnection;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import org.apache.log4j.Logger;

public class FetchTask implements Runnable
{

    static Logger logger = Logger.getLogger(FetchTask.class.getName());

    String m_url;

    FetchTask( String url )
    {
        m_url = url;
    }

    public void run()
    {
        try
        {
            logger.info( "Start: " + m_url );
            long millisStart = System.currentTimeMillis();
            URI uri = new URI(m_url);
            URL url = uri.toURL();
            URLConnection con = url.openConnection();
            con.setUseCaches( false );
            BufferedReader in = new BufferedReader( new InputStreamReader( con.getInputStream() ) );  
            int charCount = 0;
            for ( String line = in.readLine(); line != null; line = in.readLine() )
            {
                charCount += line.length();
            }
            long millisEnd = System.currentTimeMillis();
            long duration = millisEnd - millisStart;
            logger.info( String.format( "Duration: %d; Characters: %d", duration, charCount ) );
        }
        catch( Exception e )
        {
            logger.info(e);
        }

    }
}

