import javax.microedition.lcdui.*;

public class ResultForm extends Form implements CommandListener { 
    TestIndex main;

    ResultForm(TestIndex main, String name, int nIterations, long time) { 
        super(name);
        this.main = main;
        append(new StringItem("Elapsed time for " + nIterations + " objects", 
                              Long.toString(time) + " ms"));
        if (nIterations != 0 && time != 0) { 
            append(new StringItem("Objects per second", 
                                  Long.toString(nIterations*1000/time)));
        }
        addCommand(TestIndex.BACK_CMD);
        setCommandListener(this);
        Display.getDisplay(main).setCurrent(this);
    }
 
    public void commandAction(Command c, Displayable d) 
    {
        Display.getDisplay(main).setCurrent(main.menu);
    }
}