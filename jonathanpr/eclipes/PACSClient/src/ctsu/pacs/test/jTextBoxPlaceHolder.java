package ctsu.pacs.test;

import java.awt.Font;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JTextArea;

public class jTextBoxPlaceHolder extends JTextArea implements FocusListener 
{
	private static final long serialVersionUID = 1L;

	private String placeholder;
	private Font normalFont;
	private Font placeholderFont;

	public jTextBoxPlaceHolder(String placeholder) 
	{
		super();
		this.addFocusListener(this);
		this.placeholder = placeholder;
		this.normalFont = this.getFont();
		this.placeholderFont = new Font(normalFont.getFontName(), Font.ITALIC, normalFont.getSize() - 2);
		this.focusLost(null);
	}
	
	public jTextBoxPlaceHolder() 
	{
		super();
		this.addFocusListener(this);
		this.normalFont = this.getFont();
		this.placeholderFont = new Font(normalFont.getFontName(), Font.ITALIC, normalFont.getSize() - 2);
	}

	@Override
	public void focusGained(FocusEvent e) 
	{
		if (this.getText().isEmpty()) 
		{
			this.setFont(normalFont);
			this.setText("");
		}
	}

	@Override
	public void focusLost(FocusEvent e) 
	{
		if (this.getText().isEmpty()) 
		{
			this.setFont(placeholderFont);
			this.setText(placeholder);
		}
	}


	public void setTextEx(String text)
	{
		this.setFont(normalFont);
		this.setText(text);	
	}
	
	@Override
	public String getText() 
	{
		String text = super.getText();
		return text.equals(placeholder) ? "" : text;
	}

	public void setPlaceHolder(String placeholder) 
	{
		this.placeholder = placeholder;
		this.focusLost(null);
	}

}