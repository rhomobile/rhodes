import javax.microedition.lcdui.*;
import org.garret.perst.Iterator;

class PlayerSearchForm extends Form implements CommandListener { 
    UniAR     uar;
    TextField pattern;

    PlayerSearchForm(UniAR uar) { 
        super("Player search");
        this.uar = uar;
        pattern = new TextField("Player last name", null, 80, TextField.ANY);        
        append(pattern);
        addCommand(UniAR.BACK_CMD);
        addCommand(UniAR.SEARCH_CMD);
        setCommandListener(this);
        Display.getDisplay(uar).setCurrent(this);
    }

    public void commandAction(Command c, Displayable d) 
    {
        if (c == UniAR.SEARCH_CMD) { 
            Root root = (Root)uar.db.getRoot();
            Iterator iterator = root.playerLName.prefixIterator(pattern.getString().toLowerCase());
            if (iterator.hasNext()) { 
                new PlayerForm(uar, iterator, uar.menu);
            } else { 
                uar.message("No results");
            }
        } else { 
            Display.getDisplay(uar).setCurrent(uar.menu);
        }
    }
}