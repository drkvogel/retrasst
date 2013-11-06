package cardiffOxfordDataTransfer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.charset.Charset;
import java.text.ParseException;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Vector;

/**
 * A class to validate the XML data values.
 * 
 * 1) It checks the type and is in the bounds specified. 2) It checks the
 * constrains of the database are being kept too. 3) It checks variables keep
 * the following propities: only lower case, digits and underscores. always
 * starts with a letter.
 */
public class varValidation
{
	// SOME DATABASE CONTRANTS
	private final int MAX_VAR_NAME_LENGTH = 80; // Names of any Var can't be greater
										// then 80
	private final int MAX_STRING_LENGTH = 200; // If the Var is a string, it can't be
										// longer then 200
	private final int MAX_STRING_TYPE_LENGTH = 15; // Name of the String type can't be
											// longer then 15. (string,
											// char(10), varchar(100)

	private HashMap m_VarData;  //<String, Vector<Vector<String>>>
	
	private String m_SectionStrings[][] =
	{
			{}, //section 0 which doesn't exist, adding to make array indexing simpler
			{}, //section 1 which has nothing to check
			{}, //section 2 which has nothing to check
			{
				"buttonf_","buttonn_","elapsed_","leftimg_","rightimg_","selected_"
				//"buttonf_","buttonn_","elapsed_","leftimg_","rightimg_","selectted_"
			},
			{
				"elapsed_","expected_","qname_","selected_","trialtype_"
				//"elapsed_","expected_","qname_","selected_","trialtype_"
			},
			{
				//
				"elapsed_","expected_","match_","part_","selected_"
			},
			{
				//"elapsed_","expected_","rsi_","selected_","symbol_","trialtype_","valid_"
				"valid_", "trialtype_", "symbol_", "selected_", "rsi_", "expected_", "elapsed_"
			},
			{
//					"firstcolumn_","firstimg_", "firstrow_", "secondcolumn_", "secondelap_", "secondimg_", "secondrow_"
					"firstcolumn_","firstelap_","firstimg_","firstrow_","secondcolumn_","secondelap_","secondimg_","secondrow_"			
			},
			{
//					"enterelap_", "input_", "inputelap_", "match_", "pauseelap_", "showelap_", "stimulus_", "timedout_"
					"blankelap_","digitstream_","enterelap_","input_","inputelap_","match_","pauseelap_","showelap_","stimulus_","timedout_"
			}
	};
	
			
	public varValidation()
	{
		// maps from section types to section data
		m_VarData = new HashMap();
	}

	/**
	 * Loads in the validation file. The format is:
	 * 
	 * Comment line - free text Section number;variable name;variable type;Min
	 * value; Max value; Comment; Min value can also have the special case
	 * 'enum' where the Max value contains the allowable values in a list.
	 * 
	 * @param filename
	 *            The path/filename of the validation file.
	 * @throws XMLParseException
	 * @throws IOException
	 */
	public void LoadValidationFile(String filename) throws XMLParseException, IOException
	{
		StringBuilder StringBuild = new StringBuilder();
		BufferedReader in = null;
		boolean firstLine = true;
		String line = null;
		int lineCounter = 0;
		try
		{
			in = new BufferedReader(new FileReader(filename));
			
			while ((line = in.readLine()) != null)
			{
				if (firstLine) // skip the human reable stuff.
				{
					firstLine = false;
					continue;
				}
				// System.out.println(line);
				String[] output = line.split(";");
				if ((output.length < 5) || (output.length > 6))
					throw new XMLParseException("Error in validation data file: " + filename + " -@ line: " + lineCounter);

				lineCounter++;

				Vector data = new Vector();
				data.add(output[1].trim()); // REGEX EXPRESSION TO MATCH AGAINST
				data.add(output[2].trim()); // TYPE (INT, FLOAT, etc)
				data.add(output[3].trim()); // MIN VALUE or ENUM
				data.add(output[4].trim()); // MAX VALUE RANGE ALLOWED [1,4][a,d]

				// check for copy or reference? we obviously don't want a copy..
				Vector old = (Vector) m_VarData.get(output[0]);
				if (old == null)
				{
					Vector newSet = new Vector();
					newSet.add(data);
					m_VarData.put(output[0], newSet);
				}
				else
				{
					old.add(data);
					// m_VarData.put(output[0],old);
				}
			}
		}
		catch (IOException e)
		{
		}
		finally
		{
			if (in != null)
				in.close();
		}
		
/*		
		String []s = new String[1];
		s[0] = filename;
		Path path = Paths.get(Dir,s);
		Charset charset = Charset.forName("US-ASCII");
		BufferedReader reader = Files.newBufferedReader(path, charset);
		boolean firstLine = true;
		String line = null;
		int lineCounter = 0;
		while ((line = reader.readLine()) != null)
		{
			if (firstLine) // skip the human reable stuff.
			{
				firstLine = false;
				continue;
			}
			// System.out.println(line);
			String[] output = line.split(";");
			if ((output.length < 5) || (output.length > 6))
				throw new XMLParseException("Error in validation data file: " + filename + " -@ line: " + lineCounter);

			lineCounter++;

			Vector data = new Vector();
			data.add(output[1].trim()); // REGEX EXPRESSION TO MATCH AGAINST
			data.add(output[2].trim()); // TYPE (INT, FLOAT, etc)
			data.add(output[3].trim()); // MIN VALUE or ENUM
			data.add(output[4].trim()); // MAX VALUE RANGE ALLOWED [1,4][a,d]

			// check for copy or reference? we obviously don't want a copy..
			Vector old = (Vector) m_VarData.get(output[0]);
			if (old == null)
			{
				Vector newSet = new Vector();
				newSet.add(data);
				m_VarData.put(output[0], newSet);
			}
			else
			{
				old.add(data);
				// m_VarData.put(output[0],old);
			}
		}
		*/
	}

	private boolean validateVariableName(String varName)
	{
		// From the spec.
		// Values of qid are strings composed only of lowercase letters, digits
		// and underscores, always beginning with a letter.
		// MAX LENGTH is also checked here (as a bonus)
		String RegEx = "\\b^[a-z][a-z0-9_]{0," + (MAX_VAR_NAME_LENGTH - 1) + "}\\b";

		return varName.matches(RegEx);
	}

	/**
	 * @param SectionName
	 *            the section to query
	 * @return the number of variables in that section
	 * @throws XMLParseException
	 */
	public int getNumberInSection(String SectionName) throws XMLParseException
	{
		SectionName = String.valueOf(Integer.parseInt(SectionName));
		Vector VarInfoArrays = (Vector) m_VarData.get(SectionName);
		if (VarInfoArrays == null)
			throw new XMLParseException("No such SectionName exsists:" + SectionName);
		return VarInfoArrays.size();
	}

	/**
	 * @param varName
	 *            The name of the variable
	 * @param SectionName
	 *            The name of the section which the variable is a member of
	 * @return the type of the variable as define in the varvalidation.txt file
	 *         (int, date, float..etc)
	 * @throws XMLParseException
	 */
	public String getTypeString(String varName, String SectionName) throws XMLParseException
	{
		SectionName = String.valueOf(Integer.parseInt(SectionName));
		Vector VarInfoArrays = (Vector) m_VarData.get(SectionName);
		if (VarInfoArrays == null)
			throw new XMLParseException("No such SectionName exsists:" + SectionName);

		// see if we get a match
		Iterator it = VarInfoArrays.iterator();

		while (it.hasNext())
		{
			Vector boundryInfo = (Vector) it.next();

			String RegEx = (String) boundryInfo.get(0);
			if (varName.matches(RegEx))
				return (String) boundryInfo.get(1);
		}
		throw new XMLParseException("Unknown var:" + varName);
	}

	/**
	 * returns the type of the variable.
	 * 
	 * @param varName
	 *            The name of the variable to find
	 * @param SectionName
	 *            The section the variable is in
	 * @return The object type of that variable.
	 * @throws XMLParseException
	 */
	public Object getVarType(String varName, String SectionName) throws XMLParseException
	{
		SectionName = String.valueOf(Integer.parseInt(SectionName));

		Vector VarInfoArrays = (Vector) m_VarData.get(SectionName);
		if (VarInfoArrays == null)
			throw new XMLParseException("No such SectionName exsists:" + SectionName);

		// see if we get a match
		Iterator it = VarInfoArrays.iterator();

		while (it.hasNext())
		{
			Vector boundryInfo = (Vector) it.next();

			String RegEx = (String) boundryInfo.get(0);
			if (varName.matches(RegEx))
			{
				String type = (String) boundryInfo.get(1);
				if (type.compareTo("int") == 0)
					return new Integer(0);
				else if (type.compareTo("float") == 0)
					return new Float(1.1f);
				else if (type.compareTo("isodate") == 0)
					return new Date();
				else if (type.compareTo("boolean") == 0)
					return new Boolean(true);
				else if (type.startsWith("varchar("))
					return "string";
				else if (type.startsWith("char("))
					return "string";
				else if (type.compareTo("string") == 0)
					return "string";
				throw new XMLParseException("Unknown type:" + type);
			}
		}
		throw new XMLParseException("Unknown var:" + varName);
	}

	/**
	 * Gets the Variable String, could be a Regular expression, from the
	 * varvalidation.txt file, which corresponds to varName.
	 * 
	 * @param varName
	 *            name of the variable to check.
	 * @param SectionName
	 *            name of the section the variable belongs
	 * @return returns the expression to match to the variable
	 * @throws XMLParseException
	 */
	public String getVarRegExCheckString(String varName, String SectionName) throws XMLParseException
	{
		SectionName = String.valueOf(Integer.parseInt(SectionName));
		Vector VarInfoArrays = (Vector) m_VarData.get(SectionName);
		if (VarInfoArrays == null)
			throw new XMLParseException("No such SectionName exsists:" + SectionName);

		// see if we get a match
		Iterator it = VarInfoArrays.iterator();

		while (it.hasNext())
		{
			Vector boundryInfo = (Vector) it.next();

			String RegEx = (String) boundryInfo.get(0);
			if (varName.matches(RegEx))
			{
				return RegEx;
			}
		}
		throw new XMLParseException("Unknown var:" + varName);
	}

	private void updateMinMaxTotals(int num, String keyValue, int[][] Mins, int[][] Maxs, int[] Totals, String NumberList[][])
	{
		Totals[num]++;
		String[] split = keyValue.split("_");
		
		int Pos0 = Integer.parseInt(split[1]);
		int Pos1 = Integer.parseInt(split[2]);
		int Pos2 = Integer.parseInt(split[3]);
	
		if (Mins[num][0] > Pos0)
			Mins[num][0] = Pos0;
		if (Maxs[num][0] < Pos0)
			Maxs[num][0] = Pos0;

		if (Mins[num][1] > Pos1)
			Mins[num][1] = Pos1;
		if (Maxs[num][1] < Pos1)
			Maxs[num][1] = Pos1;

		if (Mins[num][2] > Pos2)
			Mins[num][2] = Pos2;
		if (Maxs[num][2] < Pos2)
			Maxs[num][2] = Pos2;

		if (NumberList[num][0] == null)
			NumberList[num][0] = new String();
		if (NumberList[num][1] == null)
			NumberList[num][1] = new String();
		if (NumberList[num][2] == null)
			NumberList[num][2] = new String();
		NumberList[num][0] += split[1] + ",";
		NumberList[num][1] += split[2] + ",";
		NumberList[num][2] += split[3] + ",";
	}

	/**
	 * @param sectionID
	 * @param SectionData
	 * @return
	 * @throws XMLParseException
	 */
	public String validateSectionData(String sectionID, TreeMap SectionData) throws XMLParseException
	{
		int isectionID = Integer.parseInt(sectionID);
	
		if (m_SectionStrings[isectionID].length == 0) //quit if no strings to check
			return "OK";
		
		// fill in min/max
		int Mins[][] = new int[m_SectionStrings[isectionID].length][3]; 
		int Maxs[][] = new int[m_SectionStrings[isectionID].length][3];
		for (int s=0;s<m_SectionStrings[isectionID].length;s++)
		{
			Mins[s][0] = Integer.MAX_VALUE;
			Maxs[s][0] = Integer.MIN_VALUE;
			Mins[s][1] = Integer.MAX_VALUE;
			Maxs[s][1] = Integer.MIN_VALUE;
			Mins[s][2] = Integer.MAX_VALUE;
			Maxs[s][2] = Integer.MIN_VALUE;
		}
		
		int Totals[] = new int[m_SectionStrings[isectionID].length];
		String NumberList[][] = new String[m_SectionStrings[isectionID].length][3];

		Iterator keyIt = SectionData.keySet().iterator();
		while (keyIt.hasNext())
		{
			String keyValue = (String) keyIt.next();
			for (int i = 0; i < m_SectionStrings[isectionID].length; i++)
			{
				if (keyValue.startsWith(m_SectionStrings[isectionID][i]))
					updateMinMaxTotals(i, keyValue, Mins, Maxs, Totals, NumberList);
			}
		}

		int TotalsTotal = Totals[0]; //counter to see if we have any user imput present. I have found data where there is none.
		
		// check we have the same number of Var types..
		for (int i = 1; i < m_SectionStrings[isectionID].length; i++)
		{
			TotalsTotal+=Totals[i]; 
			if (Totals[0] != Totals[i])
			{ // TOTALS DON'T MATCH
				String Err = "";
				for (int j = 0; j < m_SectionStrings[isectionID].length; j++)
				{	
					Err += "[" + m_SectionStrings[isectionID][j] + ":" + Totals[j] + "]";
				}
				return "Array totals don't match" + Err;
			}
		}
		
		if (TotalsTotal == 0)
			return "OK"; //no user data, but that is OK. User was asleep..

		// check we run from min to max with each var
		for (int i = 0; i < m_SectionStrings[isectionID].length; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				String[] Nums = NumberList[i][j].split(",");
		
				int min = Mins[i][j];
				int max = Maxs[i][j];

				boolean found = false;
				for (int toCheck = min; toCheck <= max; toCheck++)
				{
					found = false;
					String ToFind = String.valueOf(toCheck);
					for (int k = 0; k < Nums.length; k++)
					{
						if (ToFind.compareTo(Nums[k]) == 0)
						{
							found = true;
							break;
						}
					}
					if (!found) // WE DIDN'T FIND WHAT WE WERE LOOKING FOR.
								// SOMETHING IS MISSING
						return "Missing part of an array[" + m_SectionStrings[isectionID][i] + NumberList[i][j]+"]" ;
				}
			}
		}
		return "OK";

	}

	/**
	 * Takes a variable and its section and then validates it's value against
	 * what it expects
	 * 
	 * @param varName
	 *            The name of the variable.
	 * @param DataHeld
	 *            The data which this variable contains.
	 * @param SectionName
	 *            The section the variable is from.
	 * @return returns False if the Data isn't within the expected boundary
	 *         defined.
	 * @throws XMLParseException
	 */
	public boolean validateVar(String varName, String DataHeld, String SectionName) throws XMLParseException
	{
		if (varName.length() > MAX_VAR_NAME_LENGTH)
			return false; // Variables can't be longer then MAX_VAR_NAME_LENGTH
							// or they will break the constraints of the
							// database.

		if (!validateVariableName(varName))
			return false;

		// turn section id's with 07 to 7 to get match
		SectionName = String.valueOf(Integer.parseInt(SectionName));
		Vector VarInfoArrays = (Vector) m_VarData.get(SectionName);
		if (VarInfoArrays == null)
			throw new XMLParseException("No such SectionName exsists:" + SectionName);

		// see if we get a match
		Iterator it = VarInfoArrays.iterator();

		String RegEx;
		while (it.hasNext())
		{
			Vector boundryInfo = (Vector) it.next();

			RegEx = (String) boundryInfo.get(0);
			if (varName.matches(RegEx))
				return CheckVaribleBounds(DataHeld, boundryInfo);
		}
		return false;
	}

	private boolean CheckVaribleBounds(String DataHeld, Vector boundryInfo) throws XMLParseException
	{
		// found the var
		String type = (String) boundryInfo.get(1);
		if (type.compareTo("int") == 0)
		{
			int min = Integer.parseInt((String) boundryInfo.get(2));
			int max = Integer.parseInt((String) boundryInfo.get(3));
			int var = 0;
			try
			{
				var = Integer.parseInt(DataHeld);
			}
			catch (NumberFormatException e)
			{
				// obviously it LIES! it is no INT!
				return false;
			}

			if (var < min)
				return false;
			if (var > max)
				return false;
			return true;
		}
		else if (type.compareTo("isodate") == 0)
		{
			try
			{
				Utils.ISODateStringToDate(DataHeld);
			}
			catch (ParseException e)
			{
				return false;
			}
			return true;
		}
		else if (type.compareTo("float") == 0)
		{
			float min = Float.parseFloat((String) boundryInfo.get(2));
			float max = Float.parseFloat((String) boundryInfo.get(3));
			float var = 0.0f;
			try
			{
				var = Float.parseFloat(DataHeld);
			}
			catch (NumberFormatException e)
			{
				// obviously it LIES! it is no Float!
				return false;
			}

			if (var < min)
				return false;
			if (var > max)
				return false;
			return true;
		}
		else if (type.compareTo("boolean") == 0)
		{
			String enumString = (((String) boundryInfo.get(3)).substring(2, ((String) boundryInfo.get(3)).length() - 2));
			String[] myenums = enumString.split(",");

			for (int i = 0; i < myenums.length; i++)
			{
				if (DataHeld.compareTo(myenums[i]) == 0)
					return true;
			}
			return false;
		}
		else if (type.compareTo("varchar(10)") == 0)
		{
			if (DataHeld.length() > 10)
				return false;
			return true;
		}
		else if (type.compareTo("string") == 0)
		{
			if (DataHeld.length() > MAX_STRING_LENGTH)
				return false;
			if (DataHeld.length() == 0)
				return false;
			return true;
		}
		else if (type.startsWith("char("))
		{
			// looking at char(x) and a possible enum
			int EndBracketPos = type.indexOf(')');
			int theNumOfChars = Integer.parseInt(type.substring(5, EndBracketPos));

			if (type.length() > MAX_STRING_TYPE_LENGTH)
				return false; // Sanity check. no string type can be larger then
								// MAX_STRING_TYPE_LENGTH or it won't fit in
								// database.

			if (DataHeld.length() > MAX_STRING_LENGTH)
				return false;

			if (DataHeld.length() > theNumOfChars)
				return false;

			if (((String) boundryInfo.get(2)).compareTo("enum") == 0)
			{
				// strip out { }
				String enumString = (((String) boundryInfo.get(3)).substring(2, ((String) boundryInfo.get(3)).length() - 2));
				String[] myenums = enumString.split(",");

				for (int i = 0; i < myenums.length; i++)
				{
					if (DataHeld.compareTo(myenums[i]) == 0)
						return true;
				}
				return false;
			}

			// it's a number stored as a char
			int min = Integer.parseInt((String) boundryInfo.get(2));
			int max = Integer.parseInt((String) boundryInfo.get(3));
			int var = 0;
			try
			{
				var = Integer.parseInt(DataHeld);
			}
			catch (NumberFormatException e)
			{
				// obviously it LIES! it is no INT!
				return false;
			}
			if (var < min)
				return false;
			if (var > max)
				return false;
			return true;
		}
		throw new XMLParseException("Unknown type:" + type);
	}
}
