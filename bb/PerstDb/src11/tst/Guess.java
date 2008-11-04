import org.garret.perst.*;
import java.io.*;

public class Guess extends Persistent {
    public Guess  yes;
    public Guess  no;
    public String question; 

    static byte[] inputBuffer = new byte[256];

    public void writeObject(IOutputStream out) {
        out.writeObject(yes);
        out.writeObject(no);
        out.writeString(question);
    }
    
    public void readObject(IInputStream in) {
        yes = (Guess)in.readObject();
        no = (Guess)in.readObject();
        question = in.readString();
    }

    public Guess(Guess no, String question, Guess yes) { 
        this.yes = yes;
        this.question = question;
        this.no = no;
    }

    public Guess() {} 

    static String input(String prompt) {
        while (true) { 
            try { 
                System.out.print(prompt);
                int len = System.in.read(inputBuffer);
                String answer = new String(inputBuffer, 0, len).trim();
                if (answer.length() != 0) {
                    return answer;
                }
            } catch (IOException x) {}
        }
    }

    static boolean askQuestion(String question) { 
        String answer = input(question);
        return answer.equalsIgnoreCase("y") || answer.equalsIgnoreCase("yes");
    }

    static Guess whoIsIt(Guess parent) { 
        String animal = input("What is it ? ");
        String difference = input("What is a difference from other ? ");
        return new Guess(parent, difference, new Guess(null, animal, null));
    }

    Guess dialog() {
        if (askQuestion("May be, " + question + " (y/n) ? ")) {
            if (yes == null) { 
                System.out.println("It was very simple question for me...");
            } else { 
                Guess clarify = yes.dialog();
                if (clarify != null) { 
                    yes = clarify;
                    store();
                }
            }
        } else { 
            if (no == null) { 
                if (yes == null) { 
                    return whoIsIt(this);
                } else {
                    no = whoIsIt(null);
                    store();
                } 
            } else { 
                Guess clarify = no.dialog();
                if (clarify != null) { 
                    no = clarify;
                    store();
                }
            }
        }
        return null; 
    }
    
    static public void main(String[] args) {    
        Runtime rt = Runtime.getRuntime();
        Storage db = StorageFactory.getInstance().createStorage();

        boolean multiclient = args.length > 0 && args[0].startsWith("multi");
        if (multiclient) { 
            db.setProperty("perst.multiclient.support", Boolean.TRUE);
        }

        db.open("guess.dbs");
        
        while (askQuestion("Think of an animal. Ready (y/n) ? ")) { 
            if (multiclient) { 
                db.beginThreadTransaction(Storage.READ_WRITE_TRANSACTION);
            }
            Guess root = (Guess)db.getRoot();
            if (root == null) { 
                root = whoIsIt(null);
                db.setRoot(root);
            } else { 
                root.dialog();
            }
            if (multiclient) { 
                db.endThreadTransaction();
            } else { 
                db.commit();
            }
        }
        
        System.out.println("End of the game");
        db.close();
    }
}
