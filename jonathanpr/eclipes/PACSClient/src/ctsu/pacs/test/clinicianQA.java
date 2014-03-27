package ctsu.pacs.test;

import javax.swing.JFrame;

public class clinicianQA
{
	public static void main(String[] args)
	{
		JFrame f = new JFrame();
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	
		f.setAlwaysOnTop(true);
		f.setSize(470, 320);
		f.setResizable(false);		
		f.add(new clinicianQAGUI(f));

		f.setVisible(true);
	}
}
