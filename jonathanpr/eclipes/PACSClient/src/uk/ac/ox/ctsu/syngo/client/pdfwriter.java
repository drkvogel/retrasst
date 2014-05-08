package uk.ac.ox.ctsu.syngo.client;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.pdfbox.cos.COSArray;
import org.apache.pdfbox.cos.COSDictionary;
import org.apache.pdfbox.cos.COSFloat;
import org.apache.pdfbox.cos.COSName;
import org.apache.pdfbox.cos.COSString;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.edit.PDPageContentStream;
import org.apache.pdfbox.pdmodel.font.PDFont;
import org.apache.pdfbox.pdmodel.font.PDType1Font;
import org.apache.pdfbox.pdmodel.graphics.xobject.PDJpeg;
import org.apache.pdfbox.pdmodel.graphics.xobject.PDXObjectImage;
import org.apache.pdfbox.pdmodel.interactive.form.PDAcroForm;
import org.apache.pdfbox.pdmodel.interactive.form.PDTextbox;

// writes PDFs for the uploadClinical program.

public class pdfwriter
{
	PDFont font;
	PDDocument document;
	PDPageContentStream contentStream;
	
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	************************************************************************************/
	public pdfwriter() 
	{
	}

	// initialises the pdf
	public void Open(String ParticipentID,String DicomParticipantID, String FirstName) throws Exception
	{
		font = PDType1Font.HELVETICA_BOLD;		
		document = new PDDocument();
		PDPage page = new PDPage();
		page.setMediaBox(PDPage.PAGE_SIZE_A4); 
		document.addPage( page );
		PDPage page2 = new PDPage();
		page2.setMediaBox(PDPage.PAGE_SIZE_A4); 
		document.addPage( page2 );
				
		addBioBankLogo(185, 750, page);
		addBioBankLogo(185, 750, page2);
		
		PDAcroForm acroForm = new PDAcroForm(document);
		document.getDocumentCatalog().setAcroForm(acroForm);
		COSDictionary cosDict = new COSDictionary();
		
		COSArray rect = new COSArray();
		rect.add(new COSFloat(page.getArtBox().getLowerLeftX()+10.0f)); // lower x boundary
		rect.add(new COSFloat(page.getArtBox().getLowerLeftY()+10.0f)); // lower y boundary
		rect.add(new COSFloat(page.getArtBox().getUpperRightX()-10.0f)); // upper x boundary
		rect.add(new COSFloat(page.getArtBox().getUpperRightY()-140.0f)); // upper y boundary
		
		cosDict.setItem(COSName.RECT, rect);
		cosDict.setItem(COSName.FT, COSName.getPDFName("Tx")); // Field Type
		cosDict.setItem(COSName.TYPE, COSName.ANNOT);
		cosDict.setItem(COSName.SUBTYPE, COSName.getPDFName("Widget"));
		cosDict.setItem(COSName.T, new COSString("yourFieldName"));
		cosDict.setString(COSName.DA, "/sylfaen 0 Tf 0 g");
		
		PDTextbox textField = new PDTextbox(acroForm, cosDict);
		textField.setMultiline(true);
		
		try
		{
			acroForm.getFields().add(textField);
			page2.getAnnotations().add(textField.getWidget());
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		// Start a new content stream which will "hold" the to be created content
		contentStream = new PDPageContentStream(document, page2,true, false);
		Paragraph p = new Paragraph(10, 720, "Clinical report for participant:" +DicomParticipantID + " " + FirstName);
		write(p);
		contentStream.close();
	    //now write page 1 when ready
		contentStream = new PDPageContentStream(document, page,true, false);
		
		p = new Paragraph(50, 50, "If necessary, provide a brief description of your findings and whether futher imaging is required on the following page, save and upload this PDF to the participant within 'REPORTED' on syngo.share.");
		p.color = 15;
		p.fontSize = 9;
		write(p);		
	}

	public void SaveClose(String FileName) throws Exception
	{
		contentStream.close();

		// Save the results and ensure that the document is properly closed:
		document.save( FileName);
		document.close();	
	}
	
	// called when in the middle of writing stuff to the PDF
	public void write(Paragraph paragraph) throws IOException 
	{
		contentStream.beginText();
		contentStream.appendRawCommands(paragraph.getFontHeight() + " TL\n");
		contentStream.setFont(paragraph.getFont(), paragraph.getFontSize());
		contentStream.moveTextPositionByAmount(paragraph.getX(), paragraph.getY());
		contentStream.setStrokingColor(paragraph.getColor());

	    List<String> lines = paragraph.getLines();  // does line breaks
	      
	    for (Iterator<String> i = lines.iterator(); i.hasNext(); ) 
	    {
	    	String ss = i.next().trim();
	       	contentStream.drawString(ss);
	        if (i.hasNext()) 
	        	contentStream.appendRawCommands("T*\n");   // newline
	    }
	    contentStream.endText();
	}
	
	/************************************************************************************
	*                                       PRIVATE FUNCTIONS							*
	************************************************************************************/
	 private void addBioBankLogo(int X, int Y,PDPage page) throws IOException
	 {
        PDXObjectImage ximage = null;
        ximage = new PDJpeg(document, new FileInputStream( "biobank.jpg" ) );
        PDPageContentStream contentStream = new PDPageContentStream(document, page, true, true);
        contentStream.drawImage( ximage, X,  Y );
        contentStream.close();
	 }

	

	/************************************************************************************
	*                                       INNER CLASS									*
	************************************************************************************/
	//A class to organise text into paragraphs and to help lay them out, by wrapping the words at paragraph 'width'.
	public class Paragraph 
	{
	    private float x;
	    private float y;

	    /** width of this paragraph */
	    private int width = 500;
	    private String text;

	    private PDType1Font font = PDType1Font.HELVETICA;
	    private int fontSize = 10;
	    private int color = 0;

		/************************************************************************************
		*                                       PUBLIC FUNCTIONS							*
		************************************************************************************/
	    public Paragraph(float x, float y, String text) 
	    {
	        this.x = x;
	        this.y = y;
	        this.text = text;
	    }

	    public List<String> getLines() throws IOException 
	    {
	        List<String> result = new ArrayList<String>();

	        String[] split = text.split("(?<=\\W)");
	        int[] possibleWrapPoints = new int[split.length];
	        possibleWrapPoints[0] = split[0].length();
	        for ( int i = 1 ; i < split.length ; i++ ) 
	        {
	            possibleWrapPoints[i] = possibleWrapPoints[i-1] + split[i].length();
	        }

	        int start = 0;
	        int end = 0;
	        for ( int i : possibleWrapPoints )
	        {
	            float width = font.getStringWidth(text.substring(start,i)) / 1000 * fontSize;
	            if ( start < end && width > this.width ) 
	            {
	            	String s = text.substring(start,end);
	                result.add(s);
	                start = end;
	            }
	            end = i;
	        }
	        // Last piece of text
	        result.add(text.substring(start));
	        return result;
	    }

		/***********************************  Get/Set functions  ***********************************/    
	    public float getFontHeight() 
	    {
	        return font.getFontDescriptor().getFontBoundingBox().getHeight() / 1000 * fontSize;
	    }

	    public Paragraph withWidth(int width) 
	    {
	        this.width = width;
	        return this;
	    }

	    public Paragraph withFont(PDType1Font font, int fontSize) 
	    {
	        this.font = font;
	        this.fontSize = fontSize;
	        return this;
	    }

	    public Paragraph withColor(int color) 
	    {
	        this.color = color;
	        return this;
	    }

	    public int getColor() 
	    {
	        return color;
	    }

	    public float getX() 
	    {
	        return x;
	    }

	    public float getY() 
	    {
	        return y;
	    }

	    public int getWidth() 
	    {
	        return width;
	    }

	    public String getText()
	    {
	        return text;
	    }

	    public PDType1Font getFont() 
	    {
	        return font;
	    }

	    public int getFontSize() 
	    {
	        return fontSize;
	    }
 	}
}




