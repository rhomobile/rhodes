import org.garret.perst.*;

// All persistent capable classes should be derived from ogr.garret.perst.Persistent base class
public class Player extends Persistent { 
    Team   team;
    String fname;
    String lname;
    int    jersey;
    String address;
    String city;
    String state;
    String zip;
    String phone;
    String school;
    int    gradyear;
    int    height;

    // Serialize the object
    public void writeObject(IOutputStream out) { 
        out.writeObject(team);
        out.writeString(fname);
        out.writeString(lname);
        out.writeInt(jersey);
        out.writeString(address);
        out.writeString(city);
        out.writeString(state);
        out.writeString(zip);
        out.writeString(phone);
        out.writeString(school);
        out.writeInt(gradyear);
        out.writeInt(height);
    }

    // Deserialize the object
    public void readObject(IInputStream in) { 
        team = (Team)in.readObject();
        fname = in.readString();
        lname = in.readString();
        jersey = in.readInt();
        address = in.readString();
        city = in.readString();
        state = in.readString();
        zip = in.readString();
        phone = in.readString();
        school = in.readString();
        gradyear = in.readInt();
        height = in.readInt();
    }

    public Player() {}
    
    Player(Storage db, String row[]) throws NumberFormatException {
        super(db);
        fname = row[2];
        lname = row[3];
        jersey = Integer.parseInt(row[4]);
        address = row[5];
        city = row[6];
        state = row[7];
        zip = row[8];
        phone = row[9];
        school = row[10];
        gradyear = Integer.parseInt(row[11]);
        height = Integer.parseInt(row[12]);
    }
}
    