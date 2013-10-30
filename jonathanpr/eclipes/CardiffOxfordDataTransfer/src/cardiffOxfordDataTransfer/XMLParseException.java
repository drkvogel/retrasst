package cardiffOxfordDataTransfer;

/**
 * 
 *
 */
public class XMLParseException extends Exception
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 2787425133918012663L;

	/**
	 * 
	 */
	public XMLParseException()
	{
	}

	/**
	 * @param message
	 */
	public XMLParseException(String message)
	{
		super(message);
	}

	/**
	 * @param cause
	 */
	public XMLParseException(Throwable cause)
	{
		super(cause);
	}

	/**
	 * @param message
	 * @param cause
	 */
	public XMLParseException(String message, Throwable cause)
	{
		super(message, cause);
	}

	/**
	 * @param message
	 * @param cause
	 * @param enableSuppression
	 * @param writableStackTrace
	 */
	public XMLParseException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace)
	{
		super(message, cause, enableSuppression, writableStackTrace);
	}
}
