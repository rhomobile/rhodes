import javax.microedition.lcdui.*;
import org.garret.perst.Iterator;

class PlayerForm extends Form implements CommandListener { 
    UniAR    uar;
    Iterator iterator;
    Displayable parent;

    PlayerForm(UniAR uar, Iterator iterator, Displayable parent) { 
        this(uar, (Player)iterator.next(), parent);
        this.iterator = iterator;
        if (iterator.hasNext()) {             
            addCommand(UniAR.NEXT_CMD);
        }
    }

    PlayerForm(UniAR uar, Player player, Displayable parent) { 
        super("Player");
        this.uar = uar;
        this.parent = parent;
        append(new StringItem("Team", player.team.name)); 
        append(new StringItem("First name", player.fname)); 
        append(new StringItem("Last name",  player.lname)); 
        append(new StringItem("Jersey", Integer.toString(player.jersey))); 
        append(new StringItem("Address", player.address)); 
        append(new StringItem("City", player.city)); 
        append(new StringItem("State", player.state)); 
        append(new StringItem("Zip", player.zip)); 
        append(new StringItem("School", player.school)); 
        append(new StringItem("Grade year", Integer.toString(player.gradyear))); 
        append(new StringItem("Height", Integer.toString(player.height))); 
        addCommand(UniAR.BACK_CMD);
        setCommandListener(this);
        Display.getDisplay(uar).setCurrent(this);
    }

    public void commandAction(Command c, Displayable d) 
    {
        if (c == UniAR.NEXT_CMD) { 
            new PlayerForm(uar, iterator, parent);
        } else { 
            Display.getDisplay(uar).setCurrent(parent);
        }
    }
}