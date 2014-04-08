/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package uk.ac.ox.ctsu.javarosetta;

import java.text.ParseException;
import java.text.SimpleDateFormat;


class Deserializer
{
	private final String input;
	private int next, last;

	Deserializer(String serial_in)
	{
		if (serial_in == null || !serial_in.startsWith("[") || !serial_in.endsWith("]"))
		{
			throw new IllegalArgumentException();
		}
		else
		{
			input = serial_in;
			next = 1;
			last = serial_in.length() - 1;
		}
	}

	private Deserializer(String outer, int start, int end)
	{
		input = outer;
		next = start + 1;
		last = end - 1;
	}
	
	public int unhex( String hx ) 
	{       
		char c;
		int	val = 0;
		for (int i = 0; i < 2; i++ )
		{
			c = hx.charAt(i);
			if ( '0' <= c && c <= '9' )
				c -= '0';				
			else if ( 'a' <= c && c <= 'f' )
				c -= ( 'a' - 10 );				
			else if ( 'A' <= c && c <= 'F' )
				c -= ( 'A' - 10 );				
			else
				return( (int) -1 );
				
			val += ( i < 1 ) ? ( 16 * c ) : c;
		}
		return( val );
	}
	
	public String decodeURL(String stringToDecode)
	{
		StringBuffer out = new StringBuffer();
		
		for (int i=0;i<stringToDecode.length();i++)
		{
			char c = stringToDecode.charAt(i); 
			if (c == '+')
			{
				out.append(' ');
			}
			else if (c == '%')
			{
				int code = unhex(stringToDecode.substring(i+1,i+3));
				if ( code > 8 )
				{
					out.append((char)code);
				}
				else
				{ //not sure about this code...
				}
				i += 2;				
			}
			else 
			{
				out.append(c);
			}
		}		
		return out.toString();
	}

	void copyFields(Rosetta r)
	{
		int expected = Integer.valueOf(serializeInSplit('&'));
		for (int i = 0; i < expected; ++i)
		{
			char type = serializeInSplit('#').charAt(0);
			String name = serializeInSplit('=');
			Object value;
			
			if (type == 'i')
			{
				value = new Integer(serializeInSplit('&'));			
			}
			else if (type == 'r')
			{
				value = new Double(serializeInSplit('&'));
			}
			else if (type == 'b')
			{
				String dataString=serializeInSplit('&');
				if (dataString.compareTo("T")==0)
					value = new Boolean(true);
				else
					value = new Boolean(false);
			}
			else if (type == 's')
			{
				value = decodeURL(serializeInSplit('&'));				
			}
			else if (type == 'R')
			{
				value = parseInnerRosetta();
			}
			else if (type == 't')
			{
				try
				{
					value = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss").parse(serializeInSplit('&'));
				}
				catch (ParseException e)
				{ //TRY OTHERS HERE OR CHANGE FORMAT
					value = "ERROR";
				}				
			}
			else if (type == 'd')
			{
				try
				{
					value = new SimpleDateFormat("yyyy-MM-dd").parse(serializeInSplit('&'));
				}
				catch (ParseException e)
				{ //TRY OTHERS HERE OR CHANGE FORMAT
					value = "ERROR";
				}				
			}
			else if (type == 'o')
			{
				//IMPLMENT IF REQUIRED
				value = serializeInSplit('&');	
			}
			else
			{
				throw new IllegalArgumentException();				
			}
			
/*		
			switch (type)
			{
			case "i":
				value = new Integer(serializeInSplit('&'));
			break;
			case "r":
				value = new Double(serializeInSplit('&'));
			break;
			case "b":
				String dataString=serializeInSplit('&');
				if (dataString.compareTo("T")==0)
					value = new Boolean(true);
				else
					value = new Boolean(false);
			break;
			case "s":
				value = serializeInSplit('&');
			break;
			case "R":
				value = parseInnerRosetta();
			break;
			case "t":
				try
				{
					value = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss").parse(serializeInSplit('&'));
				}
				catch (ParseException e)
				{ //TRY OTHERS HERE OR CHANGE FORMAT
					value = "ERROR";
				}
			break;			
			default:
				throw new IllegalArgumentException();
			}
*/
			r.addObject(name, value);
		}
	}


	private String serializeInSplit(char c)
	{
		if (next < 1)
		{
			throw new IndexOutOfBoundsException();
		}
		int end = input.indexOf(c, next);
		String value;
		if (end > 0 && end < last)
		{
			value = input.substring(next, end);
			next = end + 1;
		}
		else
		{
			value = input.substring(next, last);
			next = -1;
		}
		return value;
	}

	private Rosetta parseInnerRosetta()
	{	
		if (next < 1 || input.charAt(next) != '[')
		{
			throw new IndexOutOfBoundsException();
		}
		int pos = next + 1;
		for (int level = 1; level > 0; ++pos)
		{
			char ch = input.charAt(pos);
			if (ch == '[')
			{
				level++;
			}
			else if (ch == ']')
			{
				level--;
			}
		}
		if (pos > last)
		{
			throw new IndexOutOfBoundsException();
		}
	
//		System.out.println(input.substring(next, pos)); 
/*		if (input.substring(next, pos).startsWith("[9&i#cnt=1&t#dtc=2013-10-28T15:05:48&t#ts=2013-10-28T15:04"))
		{
			int iiii = 0;
			iiii = 1;
		}
	*/	
		Rosetta r = new Rosetta();
		new Deserializer(input, next, pos).copyFields(r);
		if (pos == last)
		{
			next = -1;
		}
		else if (input.charAt(pos) != '&')
		{
			throw new IllegalArgumentException();
		}
		else
		{
			next = pos + 1;
		}
		return r;
	}
	
}
