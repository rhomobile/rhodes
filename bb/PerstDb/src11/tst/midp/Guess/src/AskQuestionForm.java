import javax.microedition.lcdui.*;

public class AskQuestionForm extends Form implements CommandListener { 
    Answer callback;
    
    AskQuestionForm(Guess midlet, String question, Answer callback) { 
        super("Question");
        this.callback = callback;
        append(new StringItem(null, question + '?'));
        addCommand(Guess.YES_CMD);
        addCommand(Guess.NO_CMD);
        setCommandListener(this);
        Display.getDisplay(midlet).setCurrent(this);
    }
 
    public void commandAction(Command c, Displayable d) 
    {
        callback.answer(c == Guess.YES_CMD);
    }
}
    