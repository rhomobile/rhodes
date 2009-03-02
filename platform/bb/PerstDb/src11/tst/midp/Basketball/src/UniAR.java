import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import java.io.*;
import org.garret.perst.StorageFactory;
import org.garret.perst.CompressedFile;
import org.garret.perst.JZlibEncryptedCompressor;
import org.garret.perst.Storage;
import org.garret.perst.Key;
import org.garret.perst.Index;

public class UniAR extends MIDlet implements CommandListener 
{
    static Command QUIT_CMD = new Command("Quit", Command.EXIT, 1);
    static Command PLAYER_CMD = new Command("Player", Command.ITEM, 1);
    static Command SEARCH_CMD = new Command("Search", Command.OK, 1);
    static Command BACK_CMD = new Command("Back", Command.BACK, 1);
    static Command NEXT_CMD = new Command("Next", Command.OK, 1);

    static final String PLAYERS_LIST = "players.lst";
    static final String TEAMS_LIST = "teams.lst";
 
    static final int TRANSACTION_PERIOD = 100;
    static final int PAGE_POOL_SIZE = 64*1024;

    private String[] readCSVLine(int nColumns, InputStream in) throws IOException 
    { 
        StringBuffer buf = new StringBuffer();
        String[] row = new String[nColumns];
        int ch;
        int column = 0;
        while ((ch = in.read()) != -1) {
            if (ch == '\t' || ch == '\n' || ch == '\r') {
                row[column++] = buf.toString();
                buf.delete(0, buf.length());
                if (ch == '\n') { 
                    return row;
                } else if (ch == '\r') { 
                    in.read();
                    return row;
                }
            } else { 
                buf.append((char)ch);
            }
        } 
        return null;       
    }

    void importData() 
    {
        String[] row;
        Gauge gauge = new Gauge("Building Database...", false, 10000, 0);
        Form form = new Form("Please wait...", new Item[] {gauge});
        Display.getDisplay(this).setCurrent(form);
        Root root = (Root)db.getRoot();

        try { 

            InputStream in = UniAR.class.getResourceAsStream("/"+TEAMS_LIST);
            gauge.setLabel("Loading Teams...");
            int nRows = 0;
            int maxRowsToLoad = 162;
            gauge.setMaxValue(maxRowsToLoad);
            
            while ((row = readCSVLine(17, in)) != null) { 
                // Create new persistent capable object 
                Team team = new Team(db, row);
                // Put created team object in team ID index
                root.teamId.put(new Key(team.id), team);
                // Put created team object in team name index
                root.teamName.put(team.name.toLowerCase(), team);
                gauge.setValue(++nRows);
                if (nRows % TRANSACTION_PERIOD == 0) {
                    // Since commit of transaction is expensive operation, commit of transaction after
                    // each insertion will cause significant performance degradation. But we have to periodicall 
                    // perform commits to avoid memory overflow since all modified and created persistent 
                    // objects are hold in memory.
                    db.commit();
                    // We need to refetch root object since commit of transaction perform cleanup of 
                    // object cache, so any refrence to persistent objects may not be valid any more
                    root = (Root)db.getRoot();
                }
                if (nRows == maxRowsToLoad) { 
                    break;
                }
            } 
            
            in = UniAR.class.getResourceAsStream("/"+PLAYERS_LIST);
            gauge.setLabel("Loading Players...");
            nRows = 0;
            maxRowsToLoad = 500;
            gauge.setMaxValue(maxRowsToLoad);
            while ((row = readCSVLine(13, in)) != null) { 
                int teamId = Integer.parseInt(row[1]);
                Team team = (Team)root.teamId.get(new Key(teamId));
                if (team == null) { 
                    System.out.println("Failed to find team " + teamId);
                } else { 
                    // Create new player
                    Player player = new Player(db, row);
                    // Set reference to the other persistemt object: to the team to which player belongs.
                    player.team = team;
                    // Put player in players last name index
                    root.playerLName.put(player.lname.toLowerCase(), player);
                    // Add player to the list of team players
                    team.players.add(player);
                }
                gauge.setValue(++nRows);
                if (nRows % TRANSACTION_PERIOD == 0) { 
                    // Periodically commit the current trasnaction to prevent memory overflow.
                    db.commit();
                    // We need to refetch root object since commit of transaction perform cleanup of 
                    // object cache, so any refrence to persistent objects may not be valid any more
                    root = (Root)db.getRoot();
                }
                if (nRows == maxRowsToLoad) { 
                    break;
                }
            } 
            db.commit();
        } catch (Exception x) { 
            x.printStackTrace();
        }
    }

    public void startApp() 
    {
        // Get instance of Perst storage
        db = StorageFactory.getInstance().createStorage();
        // Key for database encryption. To provide real data protection it should
        // not be hardcoded in the application, but asked from user.
        String cipherKey = "welcome"; 
        // Open database using compression and encruption
        db.open(new CompressedFile("uniar.dbz", new JZlibEncryptedCompressor(cipherKey)), PAGE_POOL_SIZE);
        // Open the database with given database name and specified page pool (database cache) size
        //db.open("uniar.dbs", PAGE_POOL_SIZE);

        // There is one root object in the database. 
        Root root = (Root)db.getRoot();
        if (root == null) { // if root object was not specified, then storage is not yet initialized
            // Perform initialization:
            // ... create root object
            root = new Root(db);
            // ... register new root object
            db.setRoot(root); 
            // ... and import data from resource files
            importData();
        }
        Image[] images = new Image[2];
        try { 
            images[0] = Image.createImage("/player.png");
            images[1] = Image.createImage("/team.png");
        } catch (Exception e) { 
            images = null;
        }           
        menu = new List("Find", List.IMPLICIT, new String[]{"Player", "Team"}, images);
        menu.addCommand(QUIT_CMD);
        menu.setCommandListener(this);
        Display.getDisplay(this).setCurrent(new SplashScreenCanvas(this, menu));
        //Display.getDisplay(this).setCurrent(menu);
    }

    public void commandAction(Command c, Displayable d) 
    {
        if (c == QUIT_CMD) { 
            quit();
        } else { 
            switch (menu.getSelectedIndex()) { 
            case 0:
                new PlayerSearchForm(this);
                break;
            case 1:
                new TeamSearchForm(this);
                break;
            }
        }
    }

    protected  void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
        // When application is terminated, do not forget to close the database
        db.close();
    }    

    void quit() { 
        destroyApp(true);
        notifyDestroyed();
    }

    void message(String msg) {
        Alert alert = new Alert("Warning", 
                                msg, 
                                null, 
                                AlertType.INFO);
        alert.setTimeout(Alert.FOREVER);
        Display.getDisplay(this).setCurrent(alert, menu);
    }

    Storage db;
    List    menu;
}