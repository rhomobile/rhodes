import org.garret.perst.*;

// All persistent capable classes should be derived from ogr.garret.perst.Persistent base class
public class Team extends Persistent { 
    int    id;
    String name;
    int    age;
    String bracket;
    String chapter;
    String ucolor;
    String reverse;
    String cfname;
    String clname;
    String caddress;
    String ccity;
    String cstate;
    String czip;
    String chphone;
    String cwphone;
    String ccphone;
    String cemail;
    Link   players;

    // Serialize the object
    public void writeObject(IOutputStream out) { 
        out.writeInt(id);
        out.writeString(name);
        out.writeInt(age);
        out.writeString(bracket);
        out.writeString(chapter);
        out.writeString(ucolor);
        out.writeString(reverse);
        out.writeString(cfname);
        out.writeString(clname);
        out.writeString(caddress);
        out.writeString(ccity);
        out.writeString(cstate);
        out.writeString(czip);
        out.writeString(chphone);
        out.writeString(cwphone);
        out.writeString(ccphone);
        out.writeString(cemail);
        out.writeLink(players);
    }

    // Deserialize the object
    public void readObject(IInputStream in) { 
        id = in.readInt();
        name = in.readString();
        age = in.readInt();
        bracket = in.readString();
        chapter = in.readString();
        ucolor = in.readString();
        reverse = in.readString();
        cfname = in.readString();
        clname = in.readString();
        caddress = in.readString();
        ccity = in.readString();
        cstate = in.readString();
        czip = in.readString();
        chphone = in.readString();
        cwphone = in.readString();
        ccphone = in.readString();
        cemail = in.readString();
        players = in.readLink();
    }

    public Team() {}
    
    Team(Storage db, String row[]) throws NumberFormatException {
        super(db);
        id = Integer.parseInt(row[0]);
        name = row[1];
        age = Integer.parseInt(row[2]);
        bracket = row[3];
        chapter = row[4];
        ucolor = row[5];
        reverse = row[6];
        cfname = row[7];
        clname = row[8];
        caddress = row[9];
        ccity = row[10];
        cstate = row[11];
        czip = row[12];
        chphone = row[13];
        cwphone = row[14];
        ccphone = row[15];
        cemail = row[16];
        players = db.createLink();
    }

}
    