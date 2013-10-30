/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javarosetta;

import java.text.ParseException;
import java.text.SimpleDateFormat;


class Deserializer
{
/*
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

	void copyFields(Rosetta r)
	{
		int expected = Integer.valueOf(serializeInSplit('&'));
		for (int i = 0; i < expected; ++i)
		{
			String type = serializeInSplit('#');
			String name = serializeInSplit('=');
			Object value;
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
	*/
}
