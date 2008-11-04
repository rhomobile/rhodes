import org.garret.perst.*;

public class TreeNode extends Persistent { 
    public String   question;
    public TreeNode yes;    
    public TreeNode no;
    
    public void writeObject(IOutputStream out) { 
        out.writeString(question);
        out.writeObject(yes);        
        out.writeObject(no);
    }

    
    public void readObject(IInputStream in) { 
        question = in.readString();
        yes = (TreeNode)in.readObject();        
        no = (TreeNode)in.readObject();        
    }

    public TreeNode(TreeNode no, String question, TreeNode yes) { 
        this.yes = yes;
        this.question = question;
        this.no = no;
    }

    public TreeNode() {}
}
