import javax.microedition.lcdui.*;
import org.garret.perst.Iterator;

class TeamSearchForm extends Form implements CommandListener { 
    UniAR     uar;
    TextField pattern;

    TeamSearchForm(UniAR uar) { 
        super("Team search");
        this.uar = uar;
        pattern = new TextField("Team name", null, 80, TextField.ANY);        
        append(pattern);
        addCommand(UniAR.BACK_CMD);
        addCommand(UniAR.SEARCH_CMD);
        setCommandListener(this);
        Display.getDisplay(uar).setCurrent(this);
    }

    public void commandAction(Command c, Displayable d) 
    {
        if (c == UniAR.SEARCH_CMD) { 
            /// Get storage root object
            Root root = (Root)uar.db.getRoot();
            // Perform prefix search
            Iterator iterator = root.teamName.prefixIterator(pattern.getString().toLowerCase());
            if (iterator.hasNext()) {
                // pass iterator through all located objects to the TeramForm constructor
                new TeamForm(uar, iterator, uar.menu);
            } else { 
                uar.message("No results");
            }
        } else { 
            Display.getDisplay(uar).setCurrent(uar.menu);
        }
    }
}