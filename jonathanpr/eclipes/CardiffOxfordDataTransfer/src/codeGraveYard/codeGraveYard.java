package codeGraveYard;



public class codeGraveYard
{
/*
	private void PutDataInToRosetta()
	{
		Iterator<String> it = m_RespData.keySet().iterator();
		while (it.hasNext())
		{
			final String key = it.next();
			final String val = m_RespData.get(key);
			addToRossetta(NewPart,SectionName, key,val );			
		}
		
		//add the control fields
		it = m_SectionData.keySet().iterator();
		while (it.hasNext())
		{
			final String key = it.next();
			final String val = m_SectionData.get(key);
			
			if (key.compareTo(DEFINE_DATECOMPLETED_NAME)==0)
			{	
				Date date = null;
				try
				{
					date = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss").parse(val);
				}
				catch (final ParseException e)
				{
					ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS				
				}
				NewPart.setDate(key, date);
			}
			else if(key.compareTo(DEFINE_PARTID_NAME)==0)
			{		
				NewPart.setInt(key, Integer.parseInt(val));
			}
			else if(key.compareTo(DEFINE_SECTION_ID_NAME)==0)
			{		
				NewPart.setInt(key, Integer.parseInt(val));
			}
			else if(key.compareTo(DEFINE_SESSIONID_NAME)==0)
			{		
				NewPart.setInt(key, Integer.parseInt(val));
			}
			else if(key.startsWith("dateStarted"))
			{		
				Date date = null;
				try
				{
					date = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss").parse(val);
				}
				catch (final ParseException e)
				{
					ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS				
				}
				NewPart.setDate(key, date);
			}		
		}
				
		Rosetta PersonRosetta = null;
		// 1 rosetta per person. a sub rosetta per part			
		if (!m_Rosetta.containsKey(PersonID))
		{//No person rosetta.. create one
			PersonRosetta = new Rosetta();
			m_Rosetta.put(PersonID, PersonRosetta);
		}
		else
		{
			PersonRosetta = m_Rosetta.get(PersonID);
		}	
		
		PersonRosetta.setRosetta(SectionName, NewPart);	
//			m_SectionData.clear();
//	}
	}
	
	private void addToRossetta(Rosetta NewPart,String SectionName, String key,String Data )
	{
		Object o = null;
		try
		{
			o = m_Validatior.getVarType(key,SectionName);
		}
		catch (final XMLParseException e1)
		{
			//no section exists. This would have been picked up in the first pass. Something has really gone wrong if this
			//ever happens	
			ErrorReporting.reportCritical(e1); //This needs fixing, alert CTUS
		}
		
		if (o instanceof Boolean)
		{
			if ((Data.toLowerCase().compareTo("true")==0) || (Data.compareTo("1")==0))
			{
				NewPart.setBoolean(key, true);
			}
			else if  ((Data.toLowerCase().compareTo("false")==0) || (Data.compareTo("0")==0))
			{
				NewPart.setBoolean(key, false);
			}
			else
			{
				final XMLParseException e = new XMLParseException("UNKNOWN BOOL TYPE");		
				ErrorReporting.reportCritical(e); //This needs fixing, alert CTUS
			}
		}
		else if (o instanceof Integer)
		{
			NewPart.setInt(key, Integer.parseInt(Data));
		}		
		else if (o instanceof Float)
		{
			NewPart.setReal(key, Float.parseFloat(Data));
		}	
		else if (o instanceof String)
		{
			NewPart.setString(key, Data);
		}
	}
	
	
	private void fillRosetta()
	{
		try
		{
			HashMap<String, Integer> creationList = AckXML.get_RosettaCreationList();
			Iterator<String> df =creationList.keySet().iterator();
			while (df.hasNext())
			{
				String PersonSessionKey = df.next();
				
				String[] PersonSession = PersonSessionKey.split(":");
				RosettaParser.parse(PersonSession[0],PersonSession[1]);
				
				Iterator<String> RosIt = theRosetta.keySet().iterator();
				while (RosIt.hasNext())
				{
					String PersonID = RosIt.next();
					System.out.println("-PERSON_ID: " + PersonID + ":");
					System.out.println(theRosetta.get(PersonID).serializeOut());
					System.out.println("------");
				}
				theRosetta.clear();
			}
		}
		catch (IOException | ParserConfigurationException | SAXException | XMLParseException e)
		{
			e.printStackTrace();
			ErrorReporting.reportCritical(e);
		}		
	}
	
	private void encrpytACKFIle()
	{
		//encrypt "c:\\temp\\carack.xml" and place in pick up location
		// decrypt to cardat.enc to cardat.xml
		input = new File(ack_FILENAME);
		output = new File(encryptedAck_FILENAME);
	
		try
		{
			FileOpperations.compressAndEncryptFile(input, output, THE_PASSWORD);
		}
		catch (InvalidKeyException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidParameterSpecException | IllegalBlockSizeException | BadPaddingException | IOException e)
		{
			//problem saving or create the encrpyted ACK...
			e.printStackTrace();
			ErrorReporting.reportCritical(e);			
		}
	}
	
	private void grabhttps()
	{
		final String encrptedXML_FILENAME = "cardat.enc";
		final String decrptedXML_FILENAME = "cardat.xml";
		final String ack_FILENAME = "carack.xml";
		final String encryptedAck_FILENAME = "carack.enc";
		final String THE_PASSWORD = "1234567890123456789012345678901234567890123456789012345678901234";
				
		final String saveFile = encrptedXML_FILENAME;
		//TRY AND GET HOLD OF THE ENCRYPTED FILE FROM CARDIFF.
		//IF NETWORKING PROBLEM, TRY IT A FEW TIMES BEFORE GIVING UP.
		
		int tryCounter = -1; 
		Exception lastException = null; //last Exception
		do
		{
			if (tryCounter!= -1) //TEMP CODE TO SKIP THIS
			{
		 		try
				{
					fetchCardat(saveFile);
					break;
				}
		 		catch (MalformedURLException e1)
				{   //Problem with URL... report this and exit
					e1.printStackTrace();
					ErrorReporting.reportCritical(e1);
					return;
				}
				catch (IOException e)
				{
					//java.net.ConnectException: Connection timed out: connect
					tryCounter--;
					
					lastException = e;
					//Something bad has happened.. report and quit
					e.printStackTrace();
					try
					{
						Thread.sleep(1000);//delay a second, something may start working
					}
					catch (InterruptedException e1)
					{}
				}
			}
		}
			while (tryCounter> 0);
			
		if (tryCounter == 0)
		{
			ErrorReporting.reportCritical(lastException);
			//just couldn't do it.. networking problems? server down?
			return;
		}
			
		// decrypt to cardat.enc to cardat.xml
		File input = new File(encrptedXML_FILENAME);
		File output = new File(decrptedXML_FILENAME);
	
		try
		{
			FileOpperations.decryptAndUncompressFile(input, output, THE_PASSWORD);
		}
		catch (InvalidKeyException | NoSuchAlgorithmException | InvalidKeySpecException | NoSuchPaddingException | InvalidAlgorithmParameterException | IllegalBlockSizeException | BadPaddingException | IOException e)
		{
			//something exciting happen. Better tell CTSU
			e.printStackTrace();
			ErrorReporting.reportCritical(e);
			return;
		}
	}
	
	//THIS IS NOT AT ALL TESTED OR EVEN TRIED
	static void fetchCardat(String SaveFile) throws IOException
	{
	    URL url = null;
	    URLConnection urlConn = null;

	    String user = "ctsu:T4y1ak01d!";		
		String encoded = javax.xml.bind.DatatypeConverter.printBase64Binary(user.getBytes()); 
		url = new URL("https://www.ukbiobanknm.cf.ac.uk/HMDB/subi/cardat.enc");
	    
		urlConn = url.openConnection();
		urlConn.setRequestProperty("Authorization", "Basic " + encoded);
		InputStream input = new DataInputStream(urlConn.getInputStream());
	    	    
	    BufferedInputStream Binput = new BufferedInputStream(input);
		FileOutputStream writer;
		writer = new FileOutputStream(SaveFile);

		final int BUFFER_SIZE = 1024; //Size this in testing
		
	    byte dataBuffer[] = new byte[BUFFER_SIZE];
		while (true)
		{
			int amounttoRead = Binput.available();
			if (amounttoRead > BUFFER_SIZE)
				amounttoRead = BUFFER_SIZE;
			
			int sizeRead = Binput.read(dataBuffer,0, amounttoRead);
			if (sizeRead == -1)
				break; //EOF!
			writer.write(dataBuffer,0,sizeRead);
		}		
		writer.close();
		Binput.close();
	}
	*/
}
