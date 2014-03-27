package ctsu.pacs.test;

import javax.swing.JFrame;


public class main
{
	public static void main(String[] a) 
	{
		JFrame f = new JFrame();
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	
		f.add(new PACSClient());
		f.setSize(1010, 582);
		f.setResizable(false);
		f.setVisible(true);
	}
}


