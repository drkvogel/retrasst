package cardiffOxfordDataTransfer;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;


public class SaxInputStream extends InputStream
{
	private final byte[] buffer = new byte[10 * 1024]; //10k buffer. If any line if over 10k, something is very WRONG.
	HashMap m_lines = new HashMap();
	private final InputStream wrapped;
	private int previous;
	private int length;
	private int lineNumber;
	private int firstLineNumber;

	/**
	 * @param wrapped
	 */
	public SaxInputStream(InputStream wrapped)
	{
		this.wrapped = wrapped;
		lineNumber = 0;
		firstLineNumber = 0;
	}

	public int read() throws IOException
	{
		int current = wrapped.read();

		addToBuffer(current);
		return current;
	}

	private void addToBuffer(int current) throws IOException
	{
		if ('\r' == current) 
			newLine();
		else if ('\n' == current) 
		{
	        if ('\r' != previous) 
	        	newLine();
		} 
		else 
		    add(current);
		previous = current;
	}
	
	private void newLine() 
	{
		lineNumber++;
		m_lines.put(new Integer(lineNumber), getBuffer());
		length = 0;
    }
	
	public String getLines(int from, int too)
	{
/*		System.out.println(m_lines.size());
		
		Iterator itr = m_lines.keySet().iterator();
		while (itr.hasNext())
		{
			Integer key = (Integer) itr.next();
			String Data = (String) m_lines.get(key);
			System.out.println(key+":"+Data);
		}
*/		
		

		StringBuffer sbuffer = new StringBuffer();
		for (int i=from;i <= too;i++)
			sbuffer.append(m_lines.get(new Integer(i)));     //m_lines.keySet().toArray()[i]));
			
		//now delete all which are less then.
		for (int i=too-1;i>=firstLineNumber;i--)
			m_lines.remove(new Integer(i));
//			m_lines.remove(m_lines.keySet().toArray()[i]);
		
		firstLineNumber = too;
		return sbuffer.toString();
	}

	public int read(byte[] b, int off, int len) throws IOException
	{
		int current = wrapped.read(b, off, len);
		if (current <= 0) //eof or nothing to read!?!?!
			return current;
						
		add(b,off,current);
		return current;
	}

	public int read(byte[] b) throws IOException
	{
		int current = wrapped.read(b);
		add(b,0, current); 
		return current;
	}

	private String getBuffer()
	{
		byte[] line = new byte[length];
		System.arraycopy(buffer, 0, line, 0, length);
		String value = new String(line);
		return value;
	}
	
	private void add(byte[]b,int off, int ilength) throws IOException
	{
		for (int i=off;i<ilength;i++)
			addToBuffer(b[i]);
	}
	
	private void add(int current) throws IOException
	{
		if (length < buffer.length && current != -1)
			buffer[length++] = (byte) current;
		else
			throw new IOException("Buffer too small");
	}
}
