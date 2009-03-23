import javax.microedition.lcdui.*;

public class RunTestForm extends Form implements CommandListener { 
    TestIndex main;
    Gauge progress;

    RunTestForm(TestIndex main, String name, int nIterations) { 
        super(name);
        this.main = main;
        if (nIterations != 0) { 
            progress = new Gauge("Running...", false, nIterations, 0);
            append(progress);
        }
        addCommand(TestIndex.STOP_CMD);
        setCommandListener(this);
        Display.getDisplay(main).setCurrent(this);
    }
 
    public void commandAction(Command c, Displayable d) 
    {
        if (c == TestIndex.STOP_CMD) { 
            main.stopped = true;
        }
    }
}