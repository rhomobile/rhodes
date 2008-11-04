import javax.microedition.lcdui.*;
import org.garret.perst.Iterator;

class TeamForm extends Form implements CommandListener { 
    UniAR       uar;
    Iterator    iterator;
    Team        team;
    Displayable parent;
    ChoiceGroup list;

    TeamForm(UniAR uar, Iterator iterator, Displayable parent) { 
        super("Team");
        this.uar = uar;
        this.parent = parent;
        this.iterator = iterator;
        Team team = (Team)iterator.next();
        this.team = team;
        append(new StringItem("Name", team.name)); 
        append(new StringItem("Age", Integer.toString(team.age))); 
        append(new StringItem("Bracket",  team.bracket)); 
        append(new StringItem("Chapter",  team.chapter)); 
        append(new StringItem("Color",  team.ucolor)); 
        append(new StringItem("Reverse",  team.reverse)); 
        append(new StringItem("First Name",  team.cfname)); 
        append(new StringItem("Last Name",  team.clname)); 
        append(new StringItem("Address",  team.caddress)); 
        append(new StringItem("City",  team.ccity)); 
        append(new StringItem("State",  team.cstate)); 
        append(new StringItem("Zip",  team.czip)); 
        append(new StringItem("Home phone",  team.chphone)); 
        append(new StringItem("Work phone",  team.cwphone)); 
        append(new StringItem("Cellar phone",  team.ccphone)); 
        append(new StringItem("E-mail",  team.cemail));
        list = new ChoiceGroup("Players", Choice.POPUP);
        int nPlayers = team.players.size();
        for (int i = 0; i < nPlayers; i++) {
            Player p = (Player)team.players.get(i);
            list.append(p.fname + ' ' + p.lname, null);
        }
        append(list);
        addCommand(UniAR.BACK_CMD);
        addCommand(UniAR.PLAYER_CMD);
        if (iterator != null && iterator.hasNext()) { 
            addCommand(UniAR.NEXT_CMD);
        }
        setCommandListener(this);
        Display.getDisplay(uar).setCurrent(this);
    }

    public void commandAction(Command c, Displayable d) 
    {
        if (c == UniAR.NEXT_CMD) { 
            new TeamForm(uar, iterator, parent);
        } else if (c == UniAR.PLAYER_CMD) { 
            int i = list.getSelectedIndex();
            if (i >= 0) { 
                new PlayerForm(uar, (Player)team.players.get(i), this);
            }
        } else { 
            Display.getDisplay(uar).setCurrent(parent);
        }
    }
}