import java.awt.*;

public class BlanksApplet extends javax.swing.JApplet {
    String parameter1;
    String parameter2;
    String parameter3;

    public void init() {
        parameter1 = getParameter("adjective1");
        parameter2 = getParameter("adjective2");
        parameter3 = getParameter("adjective3");
    }

    public void paint(Graphics screen) {
        screen.drawString("The " + parameter1 + " " + parameter2 +
                          "fox jumped over the " + parameter3 + " dog.", 5, 50);
    }
}
