/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.*;
import Objects.*;
import Extensions.*;

/**
 *
 * @author admin
 */
public class XLuaObject {

    public static final int COND_ERROR = 0;
    public static final int COND_PRINT = 2;

    public static final int MODE_DEFAULT = 0;
    public static final int MODE_IMMEDIATE = 1;
    public static final int MODE_QUEUED = 2;
    public static final int MODE_EXPRESSION = 3;
    
    public XLuaState state;

    // public LPRDATA rdPtr;
    public CExtension ho;

    public Queue<String> errorQueue;
    public Queue<String> printQueue;
    public String errorString = "";
    public String lastErrorString = "";
    public String printString = "";
    public String lastPrintString = "";

    public int errorMode;
    public int printMode;
    public boolean runScript;
    public boolean autoReg;
    public boolean useBacktrace;

    public int stateId;

    public Map<String, XLuaScriptRecord> scripts;

    public XLuaObject() {
        errorQueue = new PriorityQueue<String>();
        printQueue = new PriorityQueue<String>();
        scripts = new HashMap<String, XLuaScriptRecord>();
    }

    void raiseError (String str) {
        raiseError(str, MODE_DEFAULT);
    }

    void raiseError(String str, int mode) {
        System.out.println("Raise Error: " + str);
        if (errorQueue.size() >= 20) {
            errorQueue.poll();
        }

        errorQueue.offer(str);
        lastErrorString = str;

        if (ho == null) {
            return;
        }

        switch (mode) {
        case MODE_DEFAULT:
        case MODE_IMMEDIATE:
            ho.generateEvent(COND_ERROR, 0);
            break;
        case MODE_QUEUED:
        case MODE_EXPRESSION:
            ho.pushEvent(COND_ERROR, 0);
            break;
        }
    }

    void raisePrint(String str) {
        if (printQueue.size() >= 20) {
            printQueue.poll();
        }

        printQueue.offer(str);
        lastPrintString = str;

        if (ho == null) {
            return;
        }

        // TODO: Eventually handle errMode - thread safety issues
        ho.pushEvent(COND_PRINT, 0);
    }

    String getError() {
        if (!errorQueue.isEmpty()) {
            errorString = errorQueue.poll();
            return errorString;
        }

        errorString = "";
        return errorString;
    }

    String getPrint() {
        if (!printQueue.isEmpty()) {
            printString = printQueue.poll();
            return printString;
        }

        printString = "";
        return printString;
    }

    void bindState(int sid) {
        if (state != null) {
            state.UnbindRd(this);
        }

        XLuaGlobal glob = XLuaGlobal.Get();
        state = glob.GetState(sid);
        if (state == null) {
            glob.CreateState(sid);
            state = glob.GetState(sid);
            stateId = sid;
        }

        if (state != null) {
            state.BindRd(this);
        }
    }

    void unbindState() {
        if (state != null) {
            state.UnbindRd(this);
            state = null;
        }
    }

    String GetEmbeddedScript (String name) {
        if (scripts.containsKey(name)) {
            return scripts.get(name).script;
        }
        
        return "";
    }
}
