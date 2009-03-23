import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

import org.garret.perst.*;


public class Guess extends MIDlet
{ 
    static Command QUIT_CMD = new Command("Quit", Command.BACK, 1);
    static Command DONE_CMD = new Command("Done", Command.SCREEN, 1);
    static Command YES_CMD = new Command("Yes", Command.SCREEN, 1);
    static Command NO_CMD = new Command("No", Command.BACK, 1);

    public Guess() { 
        storage = StorageFactory.getInstance().createStorage();
    }

    protected void destroyApp(boolean unconditional) {
        storage.close();
    }    

    protected  void pauseApp() {
    }

    class Dialogue implements Answer { 
        TreeNode        node;
        NewNodeCallback action;

        Dialogue(TreeNode node, NewNodeCallback action) { 
            this.node = node;
            this.action = action;
        }

        static final int TIMEOUT = 3*1000;

        public void answer(boolean yes) { 
            if (yes) { 
                if (node.yes == null) { 
                    Alert alert = new Alert("Ah!", 
                                            "It was very simple question for me...", 
                                            null, AlertType.INFO);                       
                    alert.setTimeout(TIMEOUT);
                    Display.getDisplay(Guess.this).setCurrent(alert, areYouReady);
                } else { 
                    new AskQuestionForm(Guess.this, "May be " + node.yes.question, 
                                        new Dialogue(node.yes, 
                                                     new NewNodeCallback() { 
                                                         public void callback(TreeNode newNode) { 
                                                             node.yes = newNode;
                                                             node.modify();
                                                         }
                                                     } 
                                            )
                        );
                }
            } else {  
                if (node.no == null) { 
                    if (node.yes == null) { 
                        new WhatIsItForm(Guess.this, node, 
                                         new NewNodeCallback() { 
                                             public void callback(TreeNode newNode) { 
                                                 if (action != null) { 
                                                     action.callback(newNode); 
                                                 }
                                             }
                                         }
                            );                         
                    } else { 
                        new WhatIsItForm(Guess.this, null, 
                                         new NewNodeCallback() { 
                                             public void callback(TreeNode newNode) { 
                                                 node.no = newNode;
                                                 node.modify();
                                             }
                                         }
                            );            
                    }  
                } else { 
                    new AskQuestionForm(Guess.this, "May be " + node.no.question, 
                                        new Dialogue(node.no, 
                                                     new NewNodeCallback() { 
                                                         public void callback(TreeNode newNode) { 
                                                             node.no = newNode;
                                                             node.modify();
                                                         }
                                                     } 
                                            )
                        );
                }
            }
        }
    }
                    
    protected void startApp() 
    {
        try { 
            storage.open("guess.dbs"); 
        } catch (StorageError x) {
            Alert alert = new Alert("Error", 
                                    "Failed to open database",
                                    null, AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            Display.getDisplay(this).setCurrent(alert);
            return;
        }
        root = (TreeNode)storage.getRoot();
        areYouReady 
                = new AskQuestionForm(this, 
                                      "Are you ready", 
                                      new Answer() { 
                                          public void answer(boolean yes) { 
                                              if (yes) { 
                                                  if (root != null) { 
                                                      new AskQuestionForm(Guess.this, "May be " + root.question, 
                                                                          new Dialogue(root, null));
                                                  } else { 
                                                      new WhatIsItForm(Guess.this, null,
                                                                       new NewNodeCallback() { 
                                                                           public void callback(TreeNode newNode) { 
                                                                               root = newNode;
                                                                               storage.setRoot(newNode);
                                                                           }
                                                                       }
                                                          );
                                                  }                                                  
                                              } else { 
                                                  quit();
                                              }
                                          }
                                      }
                    );
    }

    void quit() { 
        destroyApp(true);
        notifyDestroyed();
    }

    Storage         storage;
    AskQuestionForm areYouReady;
    TreeNode        root; 
}
