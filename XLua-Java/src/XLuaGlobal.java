/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.*;
import org.luaj.vm2.*;

/**
 *
 * @author admin
 */
public class XLuaGlobal {

    private static XLuaGlobal ref;

    private Map<Integer, XLuaState> stateTable;
    private Map<LuaValue, XLuaState> stateLookup;
    private Map<XLuaState, Integer> idLookup;

    //public List<XLuaState> winiStates;
    
    private XLuaGlobal () {
        stateTable = new HashMap<Integer, XLuaState>();
        stateLookup = new HashMap<LuaValue, XLuaState>();
        idLookup = new HashMap<XLuaState, Integer>();

    }

    public static synchronized XLuaGlobal Get () {
        if (ref == null) {
            ref = new XLuaGlobal();
        }

        return ref;
    }

    public Object clone () throws CloneNotSupportedException {
        throw new CloneNotSupportedException();
    }

    public boolean CreateState (int sid) {
        DeleteState(sid);

        XLuaState state = new XLuaState();
        stateTable.put(sid, state);

        if (!state.ready) {
            return false;
        }

        stateLookup.put(state._G, state);

        return true;
    }

    public boolean DeleteState (int sid) {
        XLuaState state = stateTable.get(sid);
        if (state != null) {
            stateTable.remove(sid);
            stateLookup.remove(state._G);

            return true;
        }

        return false;
    }

    public boolean DeleteState (XLuaState s) {
        if (idLookup.containsKey(s)) {
            return DeleteState(idLookup.get(s));
        }

        return false;
    }

    public XLuaState GetState (int sid) {
        return stateTable.get(sid);
    }

    public XLuaState GetStateByState (LuaValue state) {
        return stateLookup.get(state);
    }
}
