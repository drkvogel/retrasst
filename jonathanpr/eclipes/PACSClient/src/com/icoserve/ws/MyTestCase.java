package com.icoserve.ws;

import java.util.Vector;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;


/**
 * A sample test case, testing <code>java.util.Vector</code>.
 *
 */
public class MyTestCase extends TestCase {
	protected Vector fEmpty;
	protected Vector fFull;

	public static void main (String[] args) {
		junit.textui.TestRunner.run (suite());
	}
	@SuppressWarnings("rawtypes")
	protected void setUp() {
		fEmpty= new Vector();
		fFull= new Vector();
		fFull.addElement(new Integer(1));
		fFull.addElement(new Integer(2));
		fFull.addElement(new Integer(3));
	}
	public static Test suite() {
		return new TestSuite(MyTestCase.class);
	}
	public void testClone() {
		Vector clone= (Vector)fFull.clone(); 
		assertTrue(clone.size() == fFull.size());
		assertTrue(clone.contains(new Integer(1)));
	}
	public void testContains() {
		assertTrue(fFull.contains(new Integer(1)));  
		assertTrue(!fEmpty.contains(new Integer(1)));
	}
	public void testElementAt() {
		Integer i= (Integer)fFull.elementAt(0);
		assertTrue(i.intValue() == 1);

		try { 
			fFull.elementAt(fFull.size());
		} catch (ArrayIndexOutOfBoundsException e) {
			return;
		}
		fail("Should raise an ArrayIndexOutOfBoundsException");
	}
	public void testRemoveAll() {
		fFull.removeAllElements();
		fEmpty.removeAllElements();
		assertTrue(fFull.isEmpty());
		assertTrue(fEmpty.isEmpty()); 
	}
	public void testRemoveElement() {
		fFull.removeElement(new Integer(3));
		assertTrue(!fFull.contains(new Integer(3)) ); 
	}
	public void testSizeIsThree()
	{
		assertTrue(fFull.size() == 3);
		 Vector v = new Vector();
		 for (int i=0; i<3; i++)
		   v.addElement(new Object());
		 assertTrue(v.size() == fFull.size());
	}
	public void testValues()
	{
		Integer[] expected = new Integer[3];
		for (int e = 0; e < expected.length; e++) {
			expected[e] = new Integer(e+1);
		}
		Integer[] actual = (Integer[]) fFull.toArray(new Integer[fFull.size()]);
	}
	public void testCapacity() {
		int size= fFull.size(); 
		for (int i= 0; i < 100; i++)
			fFull.addElement(new Integer(i));
		assertTrue(fFull.size() == 100+size);
	}
	
}