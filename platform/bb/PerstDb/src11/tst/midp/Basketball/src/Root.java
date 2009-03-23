import org.garret.perst.*;

// In Perst Lite all object in the storage should be accessible though
// single root object. So this object should contains collections which
// keeps references to all application top-level objects.
// In out case it contains index of players by last name and two indices of teams: 
// by team ID and name
public class Root extends Persistent { 
    Index teamId;
    Index teamName;
    Index playerLName;

    // Deserialize the object
    public void readObject(IInputStream in) { 
        teamId = (Index)in.readObject();
        teamName = (Index)in.readObject();
        playerLName = (Index)in.readObject();
    }

    // Serialize the object
    public void writeObject(IOutputStream out) { 
        out.writeObject(teamId);
        out.writeObject(teamName);
        out.writeObject(playerLName);
    }

    // Default constructor (constructor without parameters) should not be used for object initialization since it is
    // used to instantiate object each time when it is loaded from the database.
    // So class should either not have constructor at all (in this case it
    // will be generated automatically by compiler), either provide
    // empty default constructor and constructor used for object
    // initialization (usually it is passed reference to the Storage
    // since it is need to create Perst collections).
    public Root() {}

    // This constructor is called once while database initialization.
    // It initialize root object and creates indices to access teams
    // and players.
    public Root(Storage db) { 
        super(db);
        // Create unique index for team identifiers
        teamId = db.createIndex(Types.Int, true);
        // Create unique index for team names
        teamName = db.createIndex(Types.String, true);
        // Create non-unique index for last name of the players
        playerLName = db.createIndex(Types.String, false);
    }
}