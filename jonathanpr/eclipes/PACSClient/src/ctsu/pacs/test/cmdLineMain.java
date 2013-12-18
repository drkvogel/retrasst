package ctsu.pacs.test;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.rmi.RemoteException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.xml.stream.XMLStreamException;

import com.icoserve.ws.client.types.DicomDocumentFuzzySearchResult;
import com.icoserve.ws.client.types.DicomImage;
import com.icoserve.ws.client.types.DicomImportResult;
import com.icoserve.ws.client.types.DicomSeries;
import com.icoserve.ws.client.types.DicomStudy;
import com.icoserve.ws.client.types.GenericImportResult;

//command line - download from pacs.

public class cmdLineMain
{
	public static void main(String[] args)
	{
		if (args.length <= 1)
		{
			printUsage();
			return;
		}

		if (args[0].compareTo("-qpid") == 0)
		{
			if (args.length != 2)
			{
				printUsage();
				return;
			}

			String patientID = args[1];

			PACSComms PC = new PACSComms();
			try
			{
				DicomDocumentFuzzySearchResult[] DicomStudyResults = PC
						.searchForPatientStudies(patientID);

				if ((DicomStudyResults == null)
						|| (DicomStudyResults.length == 0))
				{
					System.out.println("No studies found.");
					return;
				}

				for (int i = 0; i < DicomStudyResults.length; i++)
					System.out.println("[StudyID:"
							+ DicomStudyResults[i].getDicomStudy().getId()
							+ "][Study Modality:"
							+ DicomStudyResults[i].getDicomStudy()
									.getAllModalities() + "]");

			}
			catch (FileNotFoundException e)
			{
				e.printStackTrace();
			}
			catch (RemoteException e)
			{
				e.printStackTrace();
			}
			catch (XMLStreamException e)
			{
				e.printStackTrace();
			}
		}
		else if (args[0].compareTo("-dstudy") == 0)
		{
			if (args.length != 4)
			{
				printUsage();
				return;
			}
			String patientID = args[1]; // PATIENT ID
			int study = Integer.parseInt(args[2]); // pacs dicom study int, not
													// dicom studyID
			String destination = args[3];

			PACSComms PC = new PACSComms();
			downloadStudy(study, destination + "/" + patientID + "/", PC);
		}
		else if (args[0].compareTo("-dall") == 0)
		{
			if (args.length != 3)
			{
				printUsage();
				return;
			}
			String patientID = args[1]; // PATIENT ID
			String destination = args[2];

			PACSComms PC = new PACSComms();
			// System.out.println("Download everything from patient " +
			// patientID + " to " + destination);

			try
			{
				DicomDocumentFuzzySearchResult[] DicomStudyResults = PC
						.searchForPatientStudies(patientID);
				if ((DicomStudyResults == null)
						|| (DicomStudyResults.length == 0))
				{
					System.out.println("No studies found.");
					return;
				}
				for (int i = 0; i < DicomStudyResults.length; i++)
				{
					// System.out.println("StudyInstanceUID: " +
					// DicomStudyResults[i].getDicomStudy().getStudyInstanceUid()+" Study Modality: "+
					// DicomStudyResults[i].getDicomStudy().getAllModalities()+" Series in Study: "
					// + DicomStudyResults[i].getDicomStudy().getNumSeries());

					// save each study
					downloadStudy(DicomStudyResults[i].getDicomStudy().getId(),
							destination + "/" + patientID + "/", PC);
				}
			}
			catch (FileNotFoundException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (RemoteException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (XMLStreamException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		else
		{
			printUsage();
			return;
		}
	}

	private static void downloadStudy(final long study,
			final String destination, PACSComms PC)
	{
		// System.out.println("Download study " + study + " to " + destination);

		DicomSeries[] DicomSeriesResults = null;
		DicomStudy StudyDetails = null;
		try
		{
			StudyDetails = PC.getDicomStudy(study);
			DicomSeriesResults = PC.searchForPatientSeries(study);
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
		}
		catch (RemoteException e)
		{
			e.printStackTrace();
		}
		catch (XMLStreamException e)
		{
			e.printStackTrace();
		}

		for (int i = 0; i < DicomSeriesResults.length; i++)
		{
			// System.out.println("SeriesInstanceUID: " +
			// DicomSeriesResults[i].getSeriesInstanceUid());
			DicomImage[] DicomResults = null;
			try
			{
				DicomResults = PC.searchForDicomInSeries(DicomSeriesResults[i]
						.getId());
			}
			catch (FileNotFoundException e)
			{
				e.printStackTrace();
			}
			catch (RemoteException e)
			{
				e.printStackTrace();
			}
			catch (XMLStreamException e)
			{
				e.printStackTrace();
			}
			for (int j = 0; j < DicomResults.length; j++)
			{
				// System.out.println("DicomSopInstanceUID: " +
				// DicomResults[j].getSopInstanceUid());
				try
				{
					String ModelName = DicomSeriesResults[i]
							.getManufacturerModelName();
					if (ModelName == null)
						ModelName = "Undefined";
					else if (ModelName.compareTo("null") == 0)
						ModelName = "Undefined";
					PC.ExportDicomImage(
							DicomResults[j].getId(),
							destination
									+ ModelName
									+ "/"
									+ StudyDetails.getStudyInstanceUid()
									+ "/"
									+ DicomSeriesResults[i]
											.getSeriesInstanceUid() + "/");
				}
				catch (XMLStreamException e)
				{
					e.printStackTrace();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		}
	}

	private static void printUsage()
	{
		System.out.println("CTSU PACS TOOL");
		System.out.println("");
		System.out.println("query with paitientid - returns study ids");
		System.out.println("pacs.java -qpid patientid");
		System.out.println("");
		System.out.println("download - download a single study of a patient");
		System.out.println("pacs.java -dstudy patientid studynum destination");
		System.out.println("");
		System.out.println("download - download all studies of a patient");
		System.out.println("pacs.java -dall patientid destination");
		System.out.println("");
	}
}