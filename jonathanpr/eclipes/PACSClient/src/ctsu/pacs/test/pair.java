package ctsu.pacs.test;

public class pair<L,R> 
{
    final L left;
    final R right;

    public pair(L left, R right) 
    {
      this.left = left;
      this.right = right;
    }

    static <L,R> pair<L,R> of(L left, R right)
    {
        return new pair<L,R>(left, right);
    }
}