package cz.cvut.fel.pjv.logging;

import java.util.logging.Level;
import java.util.logging.Logger;

public class BasicLoggingUsage {

    private static final Logger LOG = Logger.getLogger(BasicLoggingUsage.class.getName());

    public static void main(String[] args) {
        LOG.setLevel(Level.ALL);
        LOG.getParent().getHandlers()[0].setLevel(Level.ALL);
        LOG.severe("Severe message");
        LOG.info("Info message");
        LOG.finest("Finest message");
    }

}
