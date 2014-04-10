package uk.ac.ox.ctsu.syngo.client;

//Simple class to store pairs of data
public class pair<L,R> 
{
    final L left;
    final R right;
	/************************************************************************************
	*                                       PUBLIC FUNCTIONS							*
	*************************************************************************************/
    public pair(L left, R right) 
    {
      this.left = left;
      this.right = right;
    }

    public static <L,R> pair<L,R> of(L left, R right)
    {
        return new pair<L,R>(left, right);
    }
}