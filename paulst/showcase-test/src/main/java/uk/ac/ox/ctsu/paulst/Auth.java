package uk.ac.ox.ctsu.paulst;

import java.net.Authenticator;
import java.net.PasswordAuthentication;
import org.apache.log4j.Logger;

public class Auth extends Authenticator
{
    PasswordAuthentication m_pa;
    
    static Logger logger = Logger.getLogger(Auth.class.getName());

    public Auth()
    {
        char pw[] = { 'y', 'o', 'u', 'n', 'g' };

        m_pa = new PasswordAuthentication( "alan", pw );
    }

    protected PasswordAuthentication getPasswordAuthentication()
    {
        logger.debug("getPasswordAuthentication");
        return m_pa;
    }
}


