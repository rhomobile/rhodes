import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import java.util.*;

public class SplashScreenCanvas extends Canvas 
{
    MIDlet midlet;
    Image  coverImage;
    Timer  coverTimer = new Timer();
    boolean done;
    int     bgcolor;
    ShowCoverTask showCoverTask;
    Object      showCoverMonitor = new Object();
    Displayable main;
    boolean     started;
    Font        font;

    static final int SHOW_COVER_DELAY = 5*1000;
    static final int BLACK = 0x000000;
    static final int WHITE = 0xFFFFFF;
    static final int RED   = 0xFF0000;

    SplashScreenCanvas(MIDlet midlet, Displayable main) { 
        setFullScreenMode (true);
        this.midlet = midlet;
        this.main = main;
        this.bgcolor = WHITE;
        try { 
            coverImage = Image.createImage("/basketball.png");
        } catch (Exception e) { 
            System.out.println("Failed to show cover: " + e);
            Display.getDisplay(midlet).setCurrent(main);
            return;
        }
    }

    protected void paint(Graphics g) {
        if (!started) { 
            showCoverTask = new ShowCoverTask();
            coverTimer.schedule(showCoverTask, SHOW_COVER_DELAY);
            started = true;
        }
        g.setColor(bgcolor);
        g.fillRect(g.getClipX(), g.getClipY(), g.getClipWidth(), g.getClipHeight());
        g.drawImage(coverImage, 
                    (getWidth() - coverImage.getWidth())/2, 
                    (getHeight() - coverImage.getHeight())/2, 
                    Graphics.TOP|Graphics.LEFT);
    }

    public void keyReleased(int keyCode) {
        synchronized (showCoverMonitor) { 
            if (!done) { 
                done = true;
                showCoverTask.cancel();
            } else { 
                return;
            }
        }
        Display.getDisplay(midlet).setCurrent(main);
    }

    class ShowCoverTask extends TimerTask { 
        public void run() { 
            synchronized (showCoverMonitor) { 
                if (!done) { 
                    done = true;
                } else { 
                    return;
                }
            }
            Display.getDisplay(midlet).setCurrent(main);
        }
    }
}
