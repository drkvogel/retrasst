/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javarosetta;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;

/**
 * 
 * @author nicholas
 */
public class Rosetta
{
	private HashMap<String, Object> contents = new HashMap<String, Object>();

	protected void addObject(String field, Object value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field 	The field to get
	 * @return			The boolean value
	 * @throws NoSuchFieldException
	 */
	public Boolean getBoolean(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof Boolean))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (Boolean) value;
		}
	}
	
	/**
	 * @param field 	The field to get
	 * @return			The date value
	 * @throws NoSuchFieldException
	 */
	public Date getDate(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof Date))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (Date) value;
		}
	}
	
	/**
	 * @param field 	The field to get
	 * @return			The Int value
	 * @throws NoSuchFieldException
	 */
	public Integer getInt(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof Integer))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (Integer) value;
		}
	}

	/**
	 * @param field 	The field to get
	 * @return			The real/double value
	 * @throws NoSuchFieldException
	 */
	public Double getReal(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof Double))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (Double) value;
		}
	}
	
	/**
	 * @param field 	The field to get
	 * @return			The Rosetta data structure
	 * @throws NoSuchFieldException
	 */
	public Rosetta getRosetta(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof Rosetta))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (Rosetta) value;
		}
	}

	/**
	 * @param field 	The field to get
	 * @return			The String value
	 * @throws NoSuchFieldException
	 */
	public String getString(String field) throws NoSuchFieldException
	{
		Object value = contents.get(field);
		if ((value == null) || !(value instanceof String))
		{
			throw new NoSuchFieldException();
		}
		else
		{
			return (String) value;
		}
	}

	/**
	 * @param field 	The field name to check against 
	 * @return TRUE 	is it is the type Boolean
	 */
	public boolean isBoolean(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof Boolean);
	}
	
	/**
	 * @param field The field name to check against 
	 * @return TRUE is it is the type Date
	 */
	public boolean isDate(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof Date);
	}

	/**
	 * @param field 	The field name to check against 
	 * @return TRUE 	is it is the type Int
	 */
	public boolean isInt(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof Integer);
	}

	/**
	 * @param field The field name to check against 
	 * @return TRUE is it is the type Real/double
	 */
	public boolean isReal(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof Double);
	}

	/**
	 * @param field 	The field name to check against 
	 * @return TRUE 	is it is the type is a Rosetta
	 */
	public boolean isRosetta(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof Rosetta);
	}

	/**
	 * @param field The field name to check against 
	 * @return TRUE is it is the type String
	 */
	public boolean isString(String field)
	{
		Object value = contents.get(field);
		return (value != null) && (value instanceof String);
	}

	/**
	 * @param serial_in
	 */
	
	public void serializeIn(String serial_in)
	{
		new Deserializer(serial_in).copyFields(this);
	}

	/**
	 * Generate a serialised Rosetta string, can be used to generate other Rosettas from.
	 * @return The Rosetta string.
	 */
	
	public String serializeOut()
	{
		StringBuilder sb = new StringBuilder();
		sb.append('[');
		sb.append(contents.size());
				
		for (String field : contents.keySet())
		{
			Object value = contents.get(field);
			char type;
			if (value instanceof Integer)
			{
				type = 'i';
			}
			else if (value instanceof Double)
			{
				type = 'r';
			}
			else if (value instanceof String)
			{
				type = 's';
			}
			else if (value instanceof Boolean)
			{
				type = 'b';
				if (((Boolean)value) == true)
					value = "T";
				else
					value = "F";
			}
			else if (value instanceof Rosetta)
			{
				type = 'R';
				value = ((Rosetta) value).serializeOut();
			}
			else if (value instanceof Date)
			{
				type = 't';				
				SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
				value = sdf.format(((Date) value));
			}
			else
			{
				throw new IllegalArgumentException();
			}
			if (type != 's')
				sb.append('&').append(type).append('#').append(field).append('=').append(value);
			else
				sb.append('&').append(type).append('#').append(field).append('=').append(encodeURL((String)value));
		}
		return sb.append(']').toString();
	}
	
	

	
	
	public String encodeURL(String stringToEncode)
	{
		String hex_char = "0123456789ABCDEFx";

		StringBuffer out = new StringBuffer();
		
		for (int i=0;i<stringToEncode.length();i++)
		{
			char c = stringToEncode.charAt(i); 
			if (c == ' ')
			{
				out.append('+');
			}
			else if (isPlainCharactor(c))
			{
				out.append(c);
			}
			else
			{
				out.append('%');
				int	ic = ( ( (int) (c) ) + 256 ) % 256;
				out.append(hex_char.charAt(ic/16));
				out.append(hex_char.charAt(ic%16));
			}
		}		
		return out.toString();
	}

	public boolean isPlainCharactor(char c)
	{
		if (Character.isLetterOrDigit(c))
			return true;
		return (c == '.' || c == ',' || c == '_' || c == '-' || c == ';' || c == '/' || c == '(' || c == ')');
	}

	
	/**
	 * @param field		The field name of the new or replacement value to store
	 * @param value		The boolean value to store
	 */
	public void setBoolean(String field, Boolean value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field		The field name of the new or replacement value to store
	 * @param value		The Date value to store
	 */
	public void setDate(String field, Date value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field		The field name of the new or replacement value to store
	 * @param value		The integer value to store
	 */
	public void setInt(String field, Integer value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field		The field name of the new or replacement value to store
	 * @param value		The real/double value to store
	 */
	public void setReal(String field, Double value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field		The field name of the new or replacement structure to store
	 * @param value		The Rosetta structure to store
	 */
	public void setRosetta(String field, Rosetta value)
	{
		contents.put(field, value);
	}

	/**
	 * @param field		The field name of the new or replacement value to store
	 * @param value		The String to store
	 */
	public void setString(String field, String value)
	{
		contents.put(field, value);
	}


	public String toString()
	{
		StringBuilder sb = new StringBuilder("{");
		boolean first = true;
		for (String field : contents.keySet())
		{
			if (first)
			{
				sb.append('{');
				first = false;
			}
			else
			{
				sb.append(", {");
			}
			sb.append(field).append(':').append(contents.get(field)).append('}');
		}
		return sb.append('}').toString();
	}
	
}
