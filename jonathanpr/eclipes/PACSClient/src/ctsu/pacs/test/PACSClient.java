package ctsu.pacs.test;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.DicomImportResult;
import com.icoserve.www.va20_documentmanipulationservice.VA20_DocumentManipulationServiceStub.GenericImportResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomImage;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.DicomSeries;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.GenericDocumentFuzzySearchResult;
import com.icoserve.www.va20_queryservice.VA20_QueryServiceStub.Patient;


class PACSClient extends JPanel implements ActionListener
{
	JTextField m_textField_PatientId;
	JTextField m_textField_FirstName;
	JTextField m_textField_LastName;

	Patient[] m_Patients;
	DefaultListModel m_SearchPatientlist;
	JList m_SearchPatientlist_JList;

	DefaultListModel m_SearchStudylist;
	JList m_SearchStudylist_JList;
	DicomDocumentFuzzySearchResult[] m_DicomStudyResults;

	DefaultListModel m_SearchSerieslist;
	JList m_SearchSerieslist_JList;	
	DicomSeries[] m_DicomSeriesResults;
	
	DefaultListModel m_SearchDicomlist;
	JList m_SearchDicomlist_JList;	
	DicomImage[] m_DicomResults;
	
	JScrollPane scrollpane2;
	
	JTextField m_localFile;
	String m_DirectoryAndFileName;
	
	JTextField m_DownLoadPatientID;
	
	JTextField m_localNonDicomFile;
	String m_DirectoryAndFileNameNonDICOM;
	JTextField m_PatientIDTextField;
	JTextField m_PatientFirstNameTextField;
	JTextField m_PatientLastNameTextField;
	JTextField m_ArchiveFileNameTextField;
	JTextField m_FileDescriptionTextField;
	
	GenericDocumentFuzzySearchResult[] m_NonDicomSearchResults;
	
	public PACSClient()
	{
		try
		{
			SwingUtilities.invokeAndWait(new Runnable()
			{
				@Override
				public void run()
				{
					setLayout(null);
					makeSearchGUI();
					makeUpLoadDicomGUI();
					makeUploadFileGUI();
					makeDownloadAll();
				}
			});
		}
		catch (Exception exc)
		{
			System.out.println("Can't create because of " + exc);
		}
	}

	private void makeUploadFileGUI()
	{		
		JPanel SearchPane = new JPanel();
		SearchPane.setBorder(BorderFactory.createLineBorder(Color.black));
		SearchPane.setBounds(502, 122, 500, 320);
		SearchPane.setLayout(null);
		add(SearchPane);
		
		JLabel label = new JLabel("Upload NON-DICOM File To PACS");
		label.setBounds(200, 10, 200, 20);
		SearchPane.add(label);
		
		JLabel label2 = new JLabel("Select a NON-DICOM FILE:");
		label2.setBounds(20, 30, 200, 20);
		SearchPane.add(label2);
		
		m_localNonDicomFile = new JTextField();
		m_localNonDicomFile.setBounds(40+200, 30, 150, 20);
		SearchPane.add(m_localNonDicomFile);
		
		JButton jb = new JButton("...");
		jb.setActionCommand("FindLocalNonDicomFile");
		jb.setBounds(20, 60, 100, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);
		
		jb = new JButton("Upload");
		jb.setActionCommand("UploadNonDicomFile");
		jb.setBounds(160, 260, 100, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);	
		
		label2 = new JLabel("Patient ID:");
		label2.setBounds(20, 100, 200, 20);
		SearchPane.add(label2);
		
		m_PatientIDTextField = new JTextField();
		m_PatientIDTextField.setBounds(40+200, 100, 150, 20);
		SearchPane.add(m_PatientIDTextField);

		label2 = new JLabel("First Name:");
		label2.setBounds(20, 120, 200, 20);
		SearchPane.add(label2);
		
		m_PatientFirstNameTextField = new JTextField();
		m_PatientFirstNameTextField.setBounds(40+200, 120, 150, 20);
		SearchPane.add(m_PatientFirstNameTextField);

		label2 = new JLabel("Last Name:");
		label2.setBounds(20, 140, 200, 20);
		SearchPane.add(label2);
		
		m_PatientLastNameTextField = new JTextField();
		m_PatientLastNameTextField.setBounds(40+200, 140, 150, 20);
		SearchPane.add(m_PatientLastNameTextField);

		label2 = new JLabel("File Description:");
		label2.setBounds(20, 190, 200, 20);
		SearchPane.add(label2);
		
		m_FileDescriptionTextField = new JTextField();
		m_FileDescriptionTextField.setBounds(40+200, 190, 150, 20);
		SearchPane.add(m_FileDescriptionTextField);
		
		label2 = new JLabel("Archive file name:");
		label2.setBounds(20, 210, 200, 20);
		SearchPane.add(label2);
		
		m_ArchiveFileNameTextField = new JTextField();
		m_ArchiveFileNameTextField.setBounds(40+200, 210, 150, 20);
		SearchPane.add(m_ArchiveFileNameTextField);		
	}
	
	private void makeDownloadAll()
	{
		JPanel SearchPane = new JPanel();
		SearchPane.setBorder(BorderFactory.createLineBorder(Color.black));
		SearchPane.setBounds(502, 442, 500, 110);
		SearchPane.setLayout(null);
		add(SearchPane);
		
		JLabel label = new JLabel("Download from PACS");
		label.setBounds(200, 10, 200, 20);
		SearchPane.add(label);
		
		JLabel label2 = new JLabel("Enter PatientID");
		label2.setBounds(20, 30, 200, 20);
		SearchPane.add(label2);
		
		m_DownLoadPatientID = new JTextField();
		m_DownLoadPatientID.setBounds(40+200, 30, 150, 20);
		SearchPane.add(m_DownLoadPatientID);
		
		JButton jb = new JButton("Download");
		jb.setActionCommand("DownLoadAll");
		jb.setBounds(160, 70, 100, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);			
	}

	private void makeUpLoadDicomGUI()
	{		
		JPanel SearchPane = new JPanel();
		SearchPane.setBorder(BorderFactory.createLineBorder(Color.black));
		SearchPane.setBounds(502, 2, 500, 120);
		SearchPane.setLayout(null);
		add(SearchPane);
		
		JLabel label = new JLabel("Upload DICOM To PACS");
		label.setBounds(200, 10, 200, 20);
		SearchPane.add(label);
		
		JLabel label2 = new JLabel("Select a Dicom FILE:");
		label2.setBounds(20, 30, 200, 20);
		SearchPane.add(label2);
		
		m_localFile = new JTextField();
		m_localFile.setBounds(40+200, 30, 150, 20);
		SearchPane.add(m_localFile);
		
		JButton jb = new JButton("...");
		jb.setActionCommand("FindLocalFile");
		jb.setBounds(20, 60, 100, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);
		
		jb = new JButton("Upload");
		jb.setActionCommand("UploadDicomFile");
		jb.setBounds(160, 60, 100, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);	
	}
		
	private void makeSearchGUI()
	{		
		JPanel SearchPane = new JPanel();
		SearchPane.setBorder(BorderFactory.createLineBorder(Color.black));
		SearchPane.setBounds(2, 2, 500, 550);
		SearchPane.setLayout(null);
		add(SearchPane);
		
		JLabel label = new JLabel("Search for Patient  (use % for wildcard)");
		label.setBounds(100, 10, 250, 20);
		SearchPane.add(label);
		
		JLabel label2 = new JLabel("Patient ID");
		label2.setBounds(20, 30, 200, 20);
		SearchPane.add(label2);
		
		m_textField_PatientId = new JTextField();
		m_textField_PatientId.setBounds(40+200, 30, 150, 20);
		SearchPane.add(m_textField_PatientId);
		
		JLabel label3 = new JLabel("or");
		label3.setBounds(40, 50, 200, 20);
		SearchPane.add(label3);
		
		JLabel label5 = new JLabel("First Name");
		label5.setBounds(20, 70, 200, 20);
		SearchPane.add(label5);
		
		m_textField_FirstName = new JTextField();
		m_textField_FirstName.setBounds(40+200, 70, 150, 20);
		SearchPane.add(m_textField_FirstName);

		JLabel label4 = new JLabel("Last Name");
		label4.setBounds(20, 90, 200, 20);
		SearchPane.add(label4);
		
		m_textField_LastName = new JTextField();
		m_textField_LastName.setBounds(40+200, 90, 150, 20);
		SearchPane.add(m_textField_LastName);
	
		JButton jb = new JButton("Search");
		jb.setActionCommand("SearchPatient");
		jb.setBounds(20, 110, 180, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);
		
		jb = new JButton("Get File List");
		jb.setActionCommand("GetPatientFileList");
		jb.setBounds(150+150, 110, 180, 20);
		jb.addActionListener(this);
		SearchPane.add(jb);
		
		m_SearchPatientlist = new DefaultListModel();
		m_SearchPatientlist_JList = new JList(m_SearchPatientlist);
		m_SearchPatientlist_JList.setBounds(5, 140, 480, 60);
		m_SearchPatientlist_JList.setLayoutOrientation(JList.VERTICAL);
//		SearchPane.add(m_SearchPatientlist_JList);
		
		JScrollPane listScroller = new JScrollPane(m_SearchPatientlist_JList);	
		listScroller.setBounds(5, 140, 480, 60);
		SearchPane.add(listScroller, BorderLayout.CENTER);
						
		JButton jb2 = new JButton("Get Dicom Studies");
		jb2.setActionCommand("GetPatientDicomList");
		jb2.setBounds(20, 160+45, 180, 20);
		jb2.addActionListener(this);
		SearchPane.add(jb2);
				
		jb2 = new JButton("Save File");
		jb2.setActionCommand("SaveNonDicomFile");
		jb2.setBounds(150+150, 160+45, 180, 20);
		jb2.addActionListener(this);
		SearchPane.add(jb2);
		
		m_SearchStudylist = new DefaultListModel();
		m_SearchStudylist_JList = new JList(m_SearchStudylist);
		m_SearchStudylist_JList.setBounds(5, 230, 480, 60);
		m_SearchStudylist_JList.setLayoutOrientation(JList.VERTICAL);
		SearchPane.add(m_SearchStudylist_JList);

		listScroller = new JScrollPane(m_SearchStudylist_JList);	
		listScroller.setBounds(5, 230, 480, 60);
		SearchPane.add(listScroller, BorderLayout.CENTER);
				
		jb2 = new JButton("Get Series from Study");
		jb2.setActionCommand("GetSeriesList");
		jb2.setBounds(20, 295, 180, 20);
		jb2.addActionListener(this);
		SearchPane.add(jb2);
			
		m_SearchSerieslist = new DefaultListModel();
		m_SearchSerieslist_JList = new JList(m_SearchSerieslist);
		m_SearchSerieslist_JList.setBounds(5, 320, 480, 60);
		m_SearchSerieslist_JList.setLayoutOrientation(JList.VERTICAL);
		SearchPane.add(m_SearchSerieslist_JList);	
		
		listScroller = new JScrollPane(m_SearchSerieslist_JList);	
		listScroller.setBounds(5, 320, 480, 60);
		SearchPane.add(listScroller, BorderLayout.CENTER);
		
		jb2 = new JButton("Get Dicom from Series");
		jb2.setActionCommand("GetDicomList");
		jb2.setBounds(20, 385, 180, 20);
		jb2.addActionListener(this);
		SearchPane.add(jb2);
		
		m_SearchDicomlist = new DefaultListModel();
		m_SearchDicomlist_JList = new JList(m_SearchDicomlist);
		m_SearchDicomlist_JList.setBounds(5, 410, 480, 60);
		m_SearchDicomlist_JList.setLayoutOrientation(JList.VERTICAL);
		SearchPane.add(m_SearchDicomlist_JList);
		
		listScroller = new JScrollPane(m_SearchDicomlist_JList);	
		listScroller.setBounds(5, 410, 480, 60);
		SearchPane.add(listScroller, BorderLayout.CENTER);
		
		jb2 = new JButton("Save File");
		jb2.setActionCommand("SaveDicom");
		jb2.setBounds(20, 475, 180, 20);
		jb2.addActionListener(this);
		SearchPane.add(jb2);
	}

	@Override
	public void actionPerformed(ActionEvent ae)
	{	
		try
		{
			PACSComms PC = new PACSComms();
			if (ae.getActionCommand() == "DownLoadAll")
			{
				PC.downLoadAll(m_DownLoadPatientID.getText());
				JOptionPane.showMessageDialog(this,"OK" );
			}
			else if (ae.getActionCommand() == "SearchPatient")
			{		
				if ((m_textField_FirstName.getText().isEmpty()) && (m_textField_LastName.getText().isEmpty()) && (m_textField_PatientId.getText().isEmpty()))
				{
					JOptionPane.showMessageDialog(this, "Need something to search with!"); 	
					return;
				}
				
				m_SearchPatientlist.clear(); m_Patients = null;m_NonDicomSearchResults = null;
				m_SearchStudylist.clear(); m_DicomStudyResults = null; 
				m_SearchSerieslist.clear(); m_DicomSeriesResults = null;
				m_SearchDicomlist.clear(); m_DicomResults = null;
				
				m_Patients = PC.searchForPatients(m_textField_FirstName.getText(),m_textField_LastName.getText(),m_textField_PatientId.getText());
				for (int pats = 0; pats < m_Patients.length; pats++)
				{
					m_SearchPatientlist.addElement(m_Patients[pats].getPatientId() + " " +m_Patients[pats].getFirstName() + " " + m_Patients[pats].getLastName() + " " + m_Patients[pats].getNumberOfDicomStudies() + " dicom studies" + " "  + m_Patients[pats].getNumberOfGenericContainers() + " files");
					System.out.println(m_Patients[pats].getPatientId() + " " + m_Patients[pats].getFirstName() + " " + m_Patients[pats].getLastName() + " " + m_Patients[pats].getNumberOfDicomStudies() + " dicom studies" + " "  + m_Patients[pats].getNumberOfGenericContainers()+ " files");
				}
			}
			else if (ae.getActionCommand() == "GetPatientDicomList")
			{
				if (m_Patients == null)
				{
					JOptionPane.showMessageDialog(this, "You need to 'Search' for a patient"); 	
					return;
				}

				int Index = m_SearchPatientlist_JList.getSelectedIndex();
				if (Index == -1)
				{
					JOptionPane.showMessageDialog(this, "Error: Nothing selected"); 	
					return;
				}
								
				m_NonDicomSearchResults = null;
				m_SearchStudylist.clear(); m_DicomStudyResults = null; 
				m_SearchSerieslist.clear(); m_DicomSeriesResults = null;
				m_SearchDicomlist.clear(); m_DicomResults = null;

//				m_DicomStudyResults = PC.searchForPatientStudies(m_Patients[Index].getPatientId(),"BIOBANK");
				if (m_DicomStudyResults == null)
					return;
				
				for (int i=0;i<m_DicomStudyResults.length;i++)
					m_SearchStudylist.addElement("StudyInstanceUID: " + m_DicomStudyResults[i].getDicomStudy().getStudyInstanceUid()+" Study Modality: "+ m_DicomStudyResults[i].getDicomStudy().getAllModalities()+" Series in Study: " + m_DicomStudyResults[i].getDicomStudy().getNumSeries());	
			}
			else if (ae.getActionCommand() == "GetPatientFileList")
			{
				m_DicomStudyResults = null;
				if ((m_textField_FirstName.getText().isEmpty()) && (m_textField_LastName.getText().isEmpty()) && (m_textField_PatientId.getText().isEmpty()))
				{
					JOptionPane.showMessageDialog(this, "Need something to search with!"); 	
					return;
				}

				m_SearchPatientlist.clear(); m_Patients = null;m_NonDicomSearchResults = null;
				m_SearchStudylist.clear(); m_DicomStudyResults = null; 
				m_SearchSerieslist.clear(); m_DicomSeriesResults = null;
				m_SearchDicomlist.clear(); m_DicomResults = null;
				
				m_NonDicomSearchResults = PC.searchForDocument(m_textField_FirstName.getText(),m_textField_LastName.getText(),m_textField_PatientId.getText());
				if (m_NonDicomSearchResults == null)
					return;
					
				for (int pats = 0; pats < m_NonDicomSearchResults.length; pats++)				
					m_SearchPatientlist.addElement("File " + pats + ": " + m_NonDicomSearchResults[pats].getGenericContainer().getArchiveContainerName() +" Disc: "+ m_NonDicomSearchResults[pats].getDocument().getDescription());		
			}
			else if (ae.getActionCommand() == "GetSeriesList")
			{
				if (m_DicomStudyResults==null)
				{
					JOptionPane.showMessageDialog(this, "Need to find some Dicom Studies!"); 	
					return;
				}
				
				int selectedIndex = m_SearchStudylist_JList.getSelectedIndex();
				if (selectedIndex == -1)
				{
					JOptionPane.showMessageDialog(this, "Error: Nothing selected"); 	
					return;
				}
				
				m_NonDicomSearchResults = null;
				m_SearchSerieslist.clear(); m_DicomSeriesResults = null;
				m_SearchDicomlist.clear(); m_DicomResults = null;
				m_DicomSeriesResults = PC.searchForPatientSeries(m_DicomStudyResults[selectedIndex].getDicomStudy().getId());				
				for (int i=0;i<m_DicomStudyResults.length;i++)
					m_SearchSerieslist.addElement("SeriesInstanceUID: " + m_DicomSeriesResults[i].getSeriesInstanceUid());
					
			}
			else if (ae.getActionCommand() == "GetDicomList")
			{
				if (m_DicomSeriesResults==null)
				{
					JOptionPane.showMessageDialog(this, "Need to find some Dicom Series!"); 	
					return;
				}
				
				int selectedIndex = m_SearchSerieslist_JList.getSelectedIndex();
				if (selectedIndex == -1)
				{
					JOptionPane.showMessageDialog(this, "Error: Nothing selected"); 	
					return;
				}
				
				m_SearchDicomlist.clear();
				m_DicomResults = PC.searchForDicomInSeries(m_DicomSeriesResults[selectedIndex].getId());				
				for (int i=0;i<m_DicomResults.length;i++)
					m_SearchDicomlist.addElement("DicomSopInstanceUID: " + m_DicomResults[i].getSopInstanceUid());
					
			}
			else if (ae.getActionCommand() == "SaveDicom")
			{
				if (m_DicomSeriesResults==null)
				{
					JOptionPane.showMessageDialog(this, "Need to find some Dicom before you can export!"); 	
					return;
				}
				
				int selectedIndex = m_SearchDicomlist_JList.getSelectedIndex();
				if (selectedIndex == -1)
				{
					JOptionPane.showMessageDialog(this, "Error: Nothing selected"); 	
					return;
				}
				
				PC.ExportDicomImage(m_DicomResults[selectedIndex].getId());
				JOptionPane.showMessageDialog(this, "File exorted, check c:\\temp\\export\\"); 
			}	
			else if (ae.getActionCommand() == "SaveNonDicomFile")
			{
				if (m_NonDicomSearchResults==null)
				{
					JOptionPane.showMessageDialog(this, "Need to find some Non-Dicom before you can export!"); 	
					return;
				}				
				int selectedIndex = m_SearchPatientlist_JList.getSelectedIndex();
				if (selectedIndex == -1)
				{
					JOptionPane.showMessageDialog(this, "Error: Nothing selected"); 	
					return;
				}
				
				PC.ExportFile(m_NonDicomSearchResults[selectedIndex].getDocument().getReferencePointer());
				JOptionPane.showMessageDialog(this, "File exorted, check c:\\temp\\export\\"); 
			}
			else if (ae.getActionCommand() == "FindLocalFile")
			{
			     //Create a file chooser        
				JFileChooser fc = new JFileChooser();
		        int returnVal = fc.showOpenDialog(this);
		        if (returnVal == JFileChooser.APPROVE_OPTION) 
		        {                
		        	File file = fc.getSelectedFile();   
		        	m_DirectoryAndFileName = file.getAbsolutePath();
		        	m_localFile.setText(file.getName());
		        } 
		        else
		        {                
		        	//cancel. do nothing	 				
		        }
			}
			else if (ae.getActionCommand() == "FindLocalNonDicomFile")
			{
			     //Create a file chooser        
				JFileChooser fc = new JFileChooser();
		        int returnVal = fc.showOpenDialog(this);
		        if (returnVal == JFileChooser.APPROVE_OPTION) 
		        {                
		        	File file = fc.getSelectedFile();   
		        	m_DirectoryAndFileNameNonDICOM = file.getAbsolutePath();
		        	m_localNonDicomFile.setText(file.getName());
		        } 
		        else
		        {                
		        	//cancel. do nothing	 				
		        }				
			}
			else if (ae.getActionCommand() == "UploadDicomFile")
			{
				if (m_DirectoryAndFileName.isEmpty())
				{
					JOptionPane.showMessageDialog(this, "Need something to upload!"); 
					return;
				}
				DicomImportResult result = PC.importDicomToPatient(m_DirectoryAndFileName);
				if (result != null)
					JOptionPane.showMessageDialog(this,"Successfully imported new dicom file. New Reference pointer is: " + result.getReferencePointer()); 
				else
					JOptionPane.showMessageDialog(this, "Failed to upload file, check it is DICOM"); 					
			}
			else if (ae.getActionCommand() == "UploadNonDicomFile")
			{
				if (m_DirectoryAndFileNameNonDICOM.isEmpty())
				{
					JOptionPane.showMessageDialog(this, "Need something to upload!"); 
					return;
				}
				
				if (m_PatientIDTextField.getText().isEmpty())
				{
					JOptionPane.showMessageDialog(this, "Need a patient ID"); 
					return;
				}
				
				if (m_ArchiveFileNameTextField.getText().isEmpty())
				{
					JOptionPane.showMessageDialog(this, "Need an archive file name"); 
					return;
				}

				GenericImportResult result = null;//PC.importFile(m_DirectoryAndFileNameNonDICOM, m_PatientIDTextField.getText(),/* m_PatientFirstNameTextField.getText(),m_PatientLastNameTextField.getText(),*/m_FileDescriptionTextField.getText(), m_ArchiveFileNameTextField.getText());
				if (result != null)
					JOptionPane.showMessageDialog(this,"Successfully imported file. New Reference pointer is: " + result.getReferencePointer()); 
				else
					JOptionPane.showMessageDialog(this, "Failed to upload file"); 											
			}
		}
		catch (Exception e)
		{
			JOptionPane.showMessageDialog(this, e); 
		}
	}
}
