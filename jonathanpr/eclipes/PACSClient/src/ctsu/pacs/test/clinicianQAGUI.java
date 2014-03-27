package ctsu.pacs.test;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.util.Calendar;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

import javarosetta.Rosetta;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;

class clinicianQAGUI extends JPanel implements ActionListener
{
	private static final Map<String, String> FAIL_TEMPLATES_MAP = createFailMap();
	private static final Map<String, String> IF_TEMPLATES_MAP = createIFMap();

	private static final String qaFailedTitleTag = "Optional tag list";
	
    private static Map<String, String> createFailMap() {
        Map<String, String> result = new HashMap<String, String>();
        result.put(qaFailedTitleTag, "none");
        result.put("Patient moved", "The patient moved while the scan was being preformed.");
        result.put("Image Defects", "The image has artifacts, and such may be not fit for purpose.");
        result.put("Minor issue", "The image has minor issues, but can be used with some work.");
        return Collections.unmodifiableMap(result);
    }	

    private static Map<String, String> createIFMap() {
        Map<String, String> result = new HashMap<String, String>();
        result.put(qaFailedTitleTag, "none");
        result.put("No Bain", "The subject may be homer simpson.");
        result.put("No eyes", "The subject may be blind.");
        result.put("Big ears", "The subject may have super human ears, and thus should be checked out for super powers.");
        return Collections.unmodifiableMap(result);
    }
	
   	
	private static final long serialVersionUID = 1L;
	
	final String FAIL_PROMPT_STRING = "To report a failed QA, select a reason for the fail from the drop down. Once selected you can generate the reason for the fail using the button Genrate. This can be edit before reporting back if needed. If no tag is approperate, just leave blank and write the reason below. Onace complete, press the confirm button.";
	final String INCIDENT_PROMPT_STRING = "To report a possible inicental finding....";
	final boolean DATABASE_TEST_MODE = false;
	final boolean WORKING_FROM_HOME_MODE = true;

	JButton m_refreshButton;
	JFrame m_parent;
	PACSComms m_PACSComms;
	database m_db;
	boolean m_ActionReady;
	JComboBox<String> m_patientComboBox; 
	JComboBox<String> m_studyComboBox; 
	JLabel m_StudyDate;
	jTextBoxPlaceHolder m_fault_report;
	JButton m_PassButton;
	JButton m_FailButton;
	JButton m_IncidentButton;
	JButton m_ConfirmButton;
	JButton m_CancelButton;
	JPanel m_SearchPane;
	JPanel m_SearchPaneFeedback;
//	JLabel m_findingslabel;	
	JComboBox<String> m_qaFailComboBox; 
	JButton m_qaFialTemplateButton;

//SIGN IN PANE
	JPanel m_SignPane;
	JButton m_SignInButton;
	JTextField m_Username;
	JPasswordField m_Password;	
	TreeMap<String,TreeMap<String,DicomDocumentFuzzySearchResult> > m_DicomDocuments;
		
	public clinicianQAGUI(JFrame f)
	{
		m_parent = f;
		try
		{
			SwingUtilities.invokeAndWait(new Runnable()
			{
				@Override
				public void run()
				{
					m_PACSComms = new PACSComms();
					try
					{
						if (DATABASE_TEST_MODE || !WORKING_FROM_HOME_MODE)
							m_db = database.instancebb5_extract_pacs_live(false);
						setLayout(null);
						makeSignIn();
						makeDownloadAll();
					}
					catch (Exception e)
					{
						e.printStackTrace();
						return;
					}
				}
			});
		}
		catch (Exception exc)
		{
			System.out.println("Can't create because of " + exc);
		}
	}
		
	
	private void showFullSize(boolean full)
	{
		if (!full)
		{
			m_qaFailComboBox.setVisible(false);
			m_qaFialTemplateButton.setVisible(false);
			m_SearchPaneFeedback.setVisible(false);
			m_parent.setSize(470, 135);
		}
		else
		{
			m_qaFailComboBox.setVisible(true);
			m_qaFialTemplateButton.setVisible(true);
			m_SearchPaneFeedback.setVisible(true);
			m_parent.setSize(640, 400);
			m_fault_report.focusLost(null);
		}
		m_PassButton.setEnabled(!full);
		m_FailButton.setEnabled(!full);
		m_IncidentButton.setEnabled(!full);		
		m_patientComboBox.setEnabled(!full);
		m_studyComboBox.setEnabled(!full);
		m_refreshButton.setEnabled(!full);
		
		m_ConfirmButton.setEnabled(full);
		m_CancelButton.setEnabled(full);
	}
	
	private void fillTemplateCombo(boolean failCombo)
	{
		m_qaFialTemplateButton.setEnabled(false);
		m_qaFailComboBox.removeAllItems();
		if (failCombo)
		{ 
			Iterator<String> it = FAIL_TEMPLATES_MAP.keySet().iterator();
			while (it.hasNext())
				m_qaFailComboBox.addItem(it.next());
		}
		else
		{ 
			Iterator<String> it = IF_TEMPLATES_MAP.keySet().iterator();
			while (it.hasNext())
				m_qaFailComboBox.addItem(it.next());
		}	
	}
	
	private String getTemplateText(boolean failCombo)
	{
		if (failCombo)
		{ 
			return FAIL_TEMPLATES_MAP.get(m_qaFailComboBox.getSelectedItem());	
		}
		return IF_TEMPLATES_MAP.get(m_qaFailComboBox.getSelectedItem());
	}
	
	private void makeSignIn()
	{
		m_SignPane = new JPanel();
		m_SignPane.setBorder(BorderFactory.createLineBorder(Color.black));
		m_SignPane.setBounds(0, 0, 470, 125);
		m_SignPane.setLayout(null);
		m_SignPane.setVisible(true);
		add(m_SignPane);
			
		JLabel label = new JLabel("Username");
		label.setBounds(100-20, 15, 190, 20);
		m_SignPane.add(label);
		
		m_Username = new JTextField();
		m_Username.setBounds(200-20, 15, 200, 20);
		m_SignPane.add(m_Username);
		
		label = new JLabel("Password");
		label.setBounds(100-20, 40, 190, 20);
		m_SignPane.add(label);

		m_Password = new JPasswordField();
		m_Password.setBounds(200-20, 40, 200, 20);
		m_SignPane.add(m_Password);
	
		m_SignInButton = new JButton("Sign In");
		m_SignInButton.setActionCommand("signin");
		m_SignInButton.setBounds(270-50, 70, 100, 30);
		m_SignInButton.addActionListener(this);
		m_SignPane.add(m_SignInButton);		
	}
	
	private void makeDownloadAll()
	{
		m_ActionReady = false;
		m_DicomDocuments = new TreeMap<String,TreeMap<String,DicomDocumentFuzzySearchResult> >();
	
		m_SearchPane = new JPanel();
//		m_SearchPane.setBorder(BorderFactory.createLineBorder(Color.black));
		m_SearchPane.setBounds(0, 0, 470, 100);
		m_SearchPane.setLayout(null);
		m_SearchPane.setVisible(false);
		add(m_SearchPane);
		
		JLabel label = new JLabel("Patient");
		label.setBounds(20, 5, 190, 20);
		m_SearchPane.add(label);
		
		m_patientComboBox = new JComboBox<String>();
		m_patientComboBox.setBounds(65, 5, 150, 20);
		m_patientComboBox.setActionCommand("PatientComboChanged");
		m_patientComboBox.addActionListener(this);
		m_SearchPane.add(m_patientComboBox);	

		label = new JLabel("Study");
		label.setBounds(20,27, 190, 20);
		m_SearchPane.add(label);
		
		m_studyComboBox = new JComboBox<String>();
		m_studyComboBox.setBounds(65, 27, 380, 20);
		m_studyComboBox.setActionCommand("StudyComboChanged");
		m_studyComboBox.addActionListener(this);
		m_SearchPane.add(m_studyComboBox);	

		m_refreshButton = new JButton("Refresh");
		m_refreshButton.setActionCommand("refresh");
		m_refreshButton.setBounds(280, 5, 100, 20);
		m_refreshButton.addActionListener(this);
		m_SearchPane.add(m_refreshButton);		
		
		m_StudyDate = new JLabel("");
		m_StudyDate.setBounds(20, 45, 250, 20);
		m_SearchPane.add(m_StudyDate);

		m_PassButton = new JButton("QA Passed");
		m_PassButton.setActionCommand("pass");
		m_PassButton.setBounds(20, 65, 135, 30);
		m_PassButton.addActionListener(this);
		m_SearchPane.add(m_PassButton);			

		m_FailButton = new JButton("QA Fail");
		m_FailButton.setActionCommand("fail");
		m_FailButton.setBounds(160, 65, 135, 30);
		m_FailButton.addActionListener(this);
		m_SearchPane.add(m_FailButton);			

		m_IncidentButton = new JButton("Possible incident");
		m_IncidentButton.setActionCommand("incident");
		m_IncidentButton.setBounds(300, 65, 135, 30);
		m_IncidentButton.addActionListener(this);
		m_SearchPane.add(m_IncidentButton);		
		
//		m_findingslabel = new JLabel("Findings:");
//		m_findingslabel.setBounds(20, 100, 490, 20);
//		m_SearchPane.add(m_findingslabel);
		
		
		m_SearchPaneFeedback = new JPanel();
		m_SearchPaneFeedback.setBorder(BorderFactory.createLineBorder(Color.LIGHT_GRAY));
		m_SearchPaneFeedback.setBounds(1, 100, 638, 400-127);
		m_SearchPaneFeedback.setLayout(null);
		m_SearchPaneFeedback.setVisible(false);
		add(m_SearchPaneFeedback);
		
		m_qaFailComboBox = new JComboBox<String>();
		m_qaFailComboBox.setBounds(20, 5, 220, 20);
		m_qaFailComboBox.setActionCommand("qaFailComboChanged");
		m_qaFailComboBox.addActionListener(this);
		m_SearchPaneFeedback.add(m_qaFailComboBox);	
		
		m_qaFialTemplateButton = new JButton("Generate template text");
		m_qaFialTemplateButton.setActionCommand("qaTemplate");
		m_qaFialTemplateButton.setBounds(270, 5, 150, 25);
		m_qaFialTemplateButton.setEnabled(false);
		m_qaFialTemplateButton.addActionListener(this);
		m_SearchPaneFeedback.add(m_qaFialTemplateButton);	
		
		m_fault_report = new jTextBoxPlaceHolder();
		m_fault_report.setBounds(20, 30, 600,200);
		m_fault_report.setLineWrap(true);
		m_fault_report.setWrapStyleWord(true);
		JScrollPane scrollingResult = new JScrollPane(m_fault_report);
		scrollingResult.setBounds(20, 30, 600,200);		
		m_SearchPaneFeedback.add(m_fault_report);
		
		m_ConfirmButton = new JButton("Confirm");
		m_ConfirmButton.setActionCommand("confirm");
		m_ConfirmButton.setBounds(210, 235, 135, 30);
		m_ConfirmButton.addActionListener(this);
		m_SearchPaneFeedback.add(m_ConfirmButton);	
		
		m_CancelButton = new JButton("Cancel");
		m_CancelButton.setActionCommand("cancel");
		m_CancelButton.setBounds(350,235, 135, 30);
		m_CancelButton.addActionListener(this);
		m_SearchPaneFeedback.add(m_CancelButton);	
		showFullSize(false);
		m_ActionReady = true;		
	}
	
	@Override
	public void actionPerformed(ActionEvent ae)
	{	
		if (!m_ActionReady)
			return;
		try
		{
			if (WORKING_FROM_HOME_MODE)
			{//THIS CAN BE SAFELY REMOVED WHEN BACK AT WORK		
				if (ae.getActionCommand() == "signin")
				{
					m_SearchPane.setVisible(true);
					m_SearchPaneFeedback.setVisible(false);
					m_SignPane.setVisible(false);
					m_PassButton.setEnabled(true);
					m_FailButton.setEnabled(true);
					m_IncidentButton.setEnabled(true);
					m_fault_report.setEnabled(true);
					populateStudyComboBox();
					m_ActionReady = true;
					return;
				}			
				else if (ae.getActionCommand() == "fail")
				{
	//				m_findingslabel.setText("Failed QA details for '??' - '???'");
					m_ConfirmButton.setActionCommand("confirmfail");
					m_fault_report.setPlaceHolder(FAIL_PROMPT_STRING);
					fillTemplateCombo(true);
					showFullSize(true);
				}
				else if (ae.getActionCommand() == "confirmfail")
				{
					if (m_fault_report.getText().length() == 0)
					{
						if (JOptionPane.showConfirmDialog(this,"No 'failed QA' details has been supplied, do you want to continue?","Warning", JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION)
							return;
					}
					m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
					JOptionPane.showMessageDialog(this, "What do we do on the QA Fail"); 	
					m_fault_report.removeAll();
					populatePatientComboBox();
					m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
					showFullSize(false);
					JOptionPane.showMessageDialog(this,"'??' - '??' is now set as QA Failed");
				}
				else if (ae.getActionCommand() == "incident")
				{
					m_ConfirmButton.setActionCommand("confirmIncident");
					m_fault_report.setPlaceHolder(INCIDENT_PROMPT_STRING);
					fillTemplateCombo(false);
					showFullSize(true);
				}
				else if (ae.getActionCommand() == "qaTemplate")
				{
					m_fault_report.setTextEx(getTemplateText(m_ConfirmButton.getActionCommand() == "confirmfail"));
				}
				else if (ae.getActionCommand() == "confirmIncident")
				{
					if (m_fault_report.getText().length() == 0)
					{
						if (JOptionPane.showConfirmDialog(this,"No incident details has been supplied, do you want to continue?","Warning", JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION)
							return;
					}
					
					m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
					showFullSize(false);
					JOptionPane.showMessageDialog(this,"'???' - '???' will be checked.");
				}
	
				else if (ae.getActionCommand() == "cancel")
				{
					m_fault_report.setText("");
					showFullSize(false);
				}
				else if (ae.getActionCommand() == "qaFailComboChanged")
				{
					if (m_qaFailComboBox.getSelectedItem() == qaFailedTitleTag)
						m_qaFialTemplateButton.setEnabled(false);
					else
						m_qaFialTemplateButton.setEnabled(true);
				}
				return;
			}
		
			if (ae.getActionCommand() == "signin")
			{				
				if (!validateUsernamePassword())
					return;
				
				m_ActionReady = true;

				try
				{
					Rosetta r = new Rosetta();
					r.setString("name", m_Username.getText());
					r.setString("password", m_Password.getText());
					
					Rosetta rContiner = new Rosetta();
					rContiner.setRosetta("signin", r);
							
					String seralisedRosetta = rContiner.serializeOut();		
					
					//ENCRYPT IT HERE - NO NEED, USING HTTPS
					
					//HEX IT
					StringBuffer hexString = new StringBuffer();
					for (int i=0;i<seralisedRosetta.length();i++) 
					{
						String hex = Integer.toHexString(0xFF & seralisedRosetta.charAt(i));
						if(hex.length()==1) 
							hexString.append('0');
						hexString.append(hex);
					}		
					seralisedRosetta = "rosetta="+hexString;
					
					//now send to CGI
					String hexresponse = httpPost.sendGetRequest(httpPost.CGIURL, seralisedRosetta);

					Rosetta in = decodeCGIResponse(hexresponse);
				  
					if ((in.isRosetta("ERROR") || (!in.isRosetta("OK"))))
					{
						  //Badness...
						throw new Exception("garbage");
					}

					Rosetta pacsLogin = in.getRosetta("OK");
					String pacsuser = pacsLogin.getString("pacsuser");
					String pacspassword = pacsLogin.getString("pacspassword"); 
					if (!m_PACSComms.signin(pacsuser,  pacspassword))
						throw new Exception("Error");

					populatePatientComboBox();
				}
				catch (Exception e)
				{
					JOptionPane.showMessageDialog(this, "Username and password not found" , "Error", JOptionPane.INFORMATION_MESSAGE);
					return;
				}
				finally
				{
					m_ActionReady = true;						
				}
				
				m_SearchPane.setVisible(true);
				m_SearchPaneFeedback.setVisible(false);
				m_SignPane.setVisible(false);
				m_PassButton.setEnabled(true);
				m_FailButton.setEnabled(true);
				m_IncidentButton.setEnabled(true);
				m_fault_report.setEnabled(true);
				populateStudyComboBox();
				m_ActionReady = true;

				
				m_SearchPaneFeedback.setVisible(false);
				m_SignPane.setVisible(false);
				return;
			}
			
			if (ae.getActionCommand() == "refresh")
			{
				populatePatientComboBox();
				return;
			}
			
			if (ae.getActionCommand() == "PatientComboChanged")
			{
				populateStudyComboBox();
				updateStudyDateLabel();		
				return;
			}
			else if (ae.getActionCommand() == "StudyComboChanged")
			{
				updateStudyDateLabel();		
				return;
			}
			else if (ae.getActionCommand() == "qaFailComboChanged")
			{
				if (m_qaFailComboBox.getSelectedItem() == qaFailedTitleTag)
					m_qaFialTemplateButton.setEnabled(false);
				else
					m_qaFialTemplateButton.setEnabled(true);
				return;
			}
			else if (ae.getActionCommand() == "qaTemplate")
			{
				m_fault_report.setTextEx(getTemplateText(m_ConfirmButton.getActionCommand() == "confirmfail"));
				return;
			}

			
			String patient = (String) m_patientComboBox.getSelectedItem();
			String study = (String) m_studyComboBox.getSelectedItem();
			DicomDocumentFuzzySearchResult studies = m_DicomDocuments.get(patient).get(study);
			
			String PatientID = studies.getPatient().getPatientId();
			String StudyInstance_UID = studies.getDicomStudy().getStudyInstanceUid();
			Calendar createWhen = studies.getDocument().getDocumentCreatedWhen();
			
			dicomRecord dicomInfo = dicomRecord.instance(PatientID, StudyInstance_UID,createWhen);
			if (DATABASE_TEST_MODE)
				dicomInfo = dicomRecord.instance(m_db,PatientID, StudyInstance_UID,createWhen);
			
			if (ae.getActionCommand() == "pass")
			{
				if (JOptionPane.showConfirmDialog(this,"You are about to QA pass '" + PatientID+ "' - '" + StudyInstance_UID + "', do you want to continue?","Confirm", JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION)
					return;
				
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
				dicomInfo.updateFindingsStatus_QAPass();				
				//move to it BIOBANK
				long studykey = studies.getDicomStudy().getDocumentPk();
				if (!m_PACSComms.changeOrganizationalUnit(studykey, "BIOBANK"))
				{
					m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
					JOptionPane.showMessageDialog(this, "ERROR WITH COMMUNICATING WITH THE PACS, PLEASE TRY AGAIN"); 	
					return;
				}
				m_fault_report.removeAll();
				populatePatientComboBox();
				showFullSize(false);
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
				JOptionPane.showMessageDialog(this,"'" + PatientID+ "' - '" + StudyInstance_UID + "' has now passed QA");
			}
			else if (ae.getActionCommand() == "fail")
			{
	//			m_findingslabel.setText("Failed QA details for '" + PatientID+ "' - '" + StudyInstance_UID + "'");
				m_fault_report.setPlaceHolder(FAIL_PROMPT_STRING);
				m_ConfirmButton.setActionCommand("confirmfail");
				fillTemplateCombo(true);
				showFullSize(true);
			}
			else if (ae.getActionCommand() == "confirmfail")
			{
				if (m_fault_report.getText().length() == 0)
				{
					if (JOptionPane.showConfirmDialog(this,"No 'failed QA' details has been supplied, do you want to continue?","Warning", JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION)
						return;
				}
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
				dicomInfo.updateFindingsStatus_QAFail();
				JOptionPane.showMessageDialog(this, "What do we do on the QA Fail"); 	
				m_fault_report.removeAll();
				populatePatientComboBox();
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
				showFullSize(false);
				JOptionPane.showMessageDialog(this,"'" + PatientID+ "' - '" + StudyInstance_UID + "' is now set as QA Failed");
			}
			else if (ae.getActionCommand() == "incident")
			{
	//			m_findingslabel.setText("Incident details for '" + PatientID + "' - '" + StudyInstance_UID + "'");
				m_fault_report.setPlaceHolder(INCIDENT_PROMPT_STRING);
				m_ConfirmButton.setActionCommand("confirmIncident");
				fillTemplateCombo(false);
				showFullSize(true);
			}
			else if (ae.getActionCommand() == "confirmIncident")
			{
				if (m_fault_report.getText().length() == 0)
				{
					if (JOptionPane.showConfirmDialog(this,"No incident details has been supplied, do you want to continue?","Warning", JOptionPane.YES_NO_OPTION) == JOptionPane.NO_OPTION)
						return;
				}
				
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
				dicomInfo.setPSIAlert(m_fault_report.getText());				
				//move to it BIOBANK
				long studykey = studies.getDicomStudy().getDocumentPk();
				if (!m_PACSComms.copyToOrganizationalUnit(studykey, "BIOBANK"))
				{
					m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
					JOptionPane.showMessageDialog(this, "ERROR COPYING"); 	
					return;
				}
				if (!m_PACSComms.changeOrganizationalUnit(studykey, "BIOBANK_PSI"))
					JOptionPane.showMessageDialog(this, "ERROR MOVING"); 	

				m_fault_report.removeAll();
				populatePatientComboBox();
				m_parent.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
				showFullSize(false);
				JOptionPane.showMessageDialog(this,"'" + PatientID+ "' - '" + StudyInstance_UID + "' will be checked.");
			}
			else if (ae.getActionCommand() == "cancel")
			{
				m_fault_report.setText("");
				showFullSize(false);
			}
			else if (ae.getActionCommand() == "qaFailComboChanged")
			{
				if (m_qaFailComboBox.getSelectedItem() == qaFailedTitleTag)
					m_qaFialTemplateButton.setEnabled(false);
				else
					m_qaFialTemplateButton.setEnabled(true);
			}
		}
		catch (Exception e)
		{
			JOptionPane.showMessageDialog(this, e); 
		}
	}

	private Rosetta decodeCGIResponse(String hexresponse) throws Exception 
	{
		if (!(hexresponse.startsWith("<HTML><BODY>") && hexresponse.endsWith("</BODY></HTML>")))
		{
			//grabage back from CGI call... 
			throw new Exception("garbage");
		}

		//get the data between body> and </body
		hexresponse = hexresponse.substring(12, hexresponse.length() - 14);
							
		//UNHEX response
		StringBuilder sb = new StringBuilder();
		StringBuilder temp = new StringBuilder();
		 
		for( int i=0; i<hexresponse.length()-1; i+=2 )
		{			 
		  //grab the hex in pairs
			String output = hexresponse.substring(i, (i + 2));
		  //convert hex to decimal
			int decimal = Integer.parseInt(output, 16);
		  //convert the decimal to character
			sb.append((char)decimal);
			temp.append(decimal);
		}
		
		String res = sb.toString();
				
		Rosetta in = new Rosetta();
		in.serializeIn(res);
		return in;
	}

	private boolean validateUsernamePassword() 
	{
		boolean usernamenotpresent = m_Username.getText().isEmpty();
		boolean passwordnotpresent = m_Password.getText().isEmpty();
		if ((usernamenotpresent) || (passwordnotpresent))
		{
			String msg = "";
			if (usernamenotpresent)
			{
				msg = "username ";
				if (passwordnotpresent)
					msg += "and ";
			}
			if (passwordnotpresent)
				msg += "password ";
			msg += "is required";
				
			JOptionPane.showMessageDialog(this, msg , "Error", JOptionPane.INFORMATION_MESSAGE);
			return false;
		}
		return true;
	}
	
	private void updateStudyDateLabel()
	{
		String patient = (String) m_patientComboBox.getSelectedItem();
		String study = (String) m_studyComboBox.getSelectedItem();
		String studyDate =  m_DicomDocuments.get(patient).get(study).getDocument().getInsertionDate().getTime().toString();		
		m_StudyDate.setText(studyDate);			
	}
	
	private void populateStudyComboBox()
	{		
		try
		{
			m_ActionReady = false;
			m_studyComboBox.removeAllItems();
			m_StudyDate.setText("");
			m_fault_report.setText("");

			String patient = (String) m_patientComboBox.getSelectedItem();

			if ((patient == null) || (patient.isEmpty()))
				return;

			TreeMap<String,DicomDocumentFuzzySearchResult> studies = m_DicomDocuments.get(patient);
			String key = studies.firstKey();
			while (key != null)
			{			
				String studyID = studies.get(key).getDicomStudy().getStudyInstanceUid();					
				m_studyComboBox.addItem(studyID);
				key = studies.higherKey(key);
			}
			updateStudyDateLabel();
		}
		finally
		{
			m_ActionReady = true;
		}
	}
	
	private void populatePatientComboBox() throws Exception
	{		
		try
		{
			m_ActionReady = false;
			m_patientComboBox.removeAllItems();
			m_StudyDate.setText("");
			m_fault_report.setText("");

			m_DicomDocuments.clear();
			DicomDocumentFuzzySearchResult[] DicomStudyResults = m_PACSComms.searchForScans("BIOBANK_QA"); //("BIOBANK_PSI");
			
			for (DicomDocumentFuzzySearchResult Result : DicomStudyResults)
			{
				String Patientid = Result.getPatient().getPatientId();
				String StudyID = Result.getDicomStudy().getStudyInstanceUid();
				
				if (m_DicomDocuments.containsKey(Patientid))
					m_DicomDocuments.get(Patientid).put(StudyID,Result);
				else
				{
					TreeMap<String,DicomDocumentFuzzySearchResult> v = new TreeMap<String,DicomDocumentFuzzySearchResult>();
					v.put(StudyID, Result);
					m_DicomDocuments.put(Patientid, v);
					m_patientComboBox.addItem(Patientid);
				}
			}
			
			if (m_DicomDocuments.isEmpty())
			{
				m_PassButton.setEnabled(false);
				m_FailButton.setEnabled(false);
				m_IncidentButton.setEnabled(false);
				m_fault_report.setEnabled(false);
				return;
			}
			m_PassButton.setEnabled(m_DicomDocuments.size() > 0);
			m_FailButton.setEnabled(m_DicomDocuments.size() > 0);
			m_IncidentButton.setEnabled(m_DicomDocuments.size() > 0);
			m_fault_report.setEnabled(m_DicomDocuments.size() > 0);
			populateStudyComboBox();
		}
		finally
		{
			m_ActionReady = true;
		}
	}
}
