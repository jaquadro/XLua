/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.*;
import java.io.*;
import org.luaj.vm2.*;
import org.luaj.vm2.lib.jse.*;

import Extensions.*;
import Services.*;
import RunLoop.*;
import Expressions.*;
import Sprites.*;
import Conditions.*;
import Actions.*;
import Expressions.*;
import Objects.*;

/**
 *
 * @author admin
 */
public class XLuaState {

    public enum LuaPackage {
        PACKAGE_ALL,
        PACKAGE_BASE,
        PACKAGE_IO,
        PACKAGE_TABLE,
        PACKAGE_PACKAGE,
        PACKAGE_OS,
        PACAKGE_STRING,
        PACKAGE_DEBUG,
        PACKAGE_MATH,
        PACKAGE_BIT,
    }

    public LuaValue _G;

    public XLuaStack stack;

    // public XLuaMMF mmfi;
    // public Map<Integer, XLuaMFObject> objects;
    // public Map<Integer, Double> localValues;
    // public Map<Integer, String> localStrings;

    //public Map<Integer, Integer>		winExports;

    public List<XLuaObject> rdList;
    
    public int stateId = 0;

    public int dsCount = 0;

    public boolean ready = false;
    // public boolean mmfiReady = false;
    // public boolean winiReady;

    public XLuaObject queueRd = null;
    public boolean queueDelete = false;

    public static String newline = System.getProperty("line.separator");

    public XLuaState () {
        rdList = new LinkedList<XLuaObject>();

        _G = JsePlatform.standardGlobals();
        if (_G == null) {
            return;
        }

        stack = new XLuaStack ();
        stack.SetState(this);

        //org.luaj.vm2.lib.BaseLib.install();
        //org.luaj.vm2.compiler.LuaC.install();

        // Register DoCall function
        _G.load(new XLuaLib());

        // Register print function

        ready = true;
    }

    /*public void Destroy () {
        ready = false;
        //mmfiReady = false;

        Iterator<XLuaObject> iter = rdList.iterator();
        while (iter.hasNext()) {
            XLuaObject lm = iter.next();
            if (lm != null) {
                lm.state = null;
            }
        }

        rdList.clear();
    }*/

    public void BindRd (XLuaObject rd) {
        if (rd == null) {
            return;
        }

        Iterator<XLuaObject> iter = rdList.iterator();
        while (iter.hasNext()) {
            if (iter.next() == rd) {
                return;
            }
        }

        rdList.add(rd);
    }

    public void UnbindRd (XLuaObject rd) {
        Iterator<XLuaObject> iter = rdList.iterator();
        while (iter.hasNext()) {
            if (iter.next() == rd) {
                iter.remove();
                return;
            }
        }
    }

    public void ReleaseRd () {
        if (queueRd == null && !queueDelete) {
            return;
        }

        if (queueDelete) {
            for (XLuaObject rd : rdList) {
                rd.state = null;
            }
            rdList.clear();
            return;
        }

        for (XLuaObject rd : rdList) {
            if (rd == queueRd) {
                rdList.remove(rd);
                queueRd = null;
                return;
            }
        }

        queueRd = null;
    }

    public void RaiseError (String str) {
        RaiseError(str, XLuaObject.MODE_DEFAULT);
    }

    public void RaiseError (String str, int mode) {
        Iterator<XLuaObject> iter = rdList.iterator();
        while (iter.hasNext()) {
            XLuaObject lm = iter.next();
            if (lm != null) {
                lm.raiseError(str, mode);
            }
        }
    }

    public void RaisePrint (String str) {
        Iterator<XLuaObject> iter = rdList.iterator();
        while (iter.hasNext()) {
            XLuaObject lm = iter.next();
            if (lm != null) {
                lm.raisePrint(str);
            }
        }
    }

    public void LoadFile (String str) {

    }

    public void LoadString(String code, String name) {
        if (name == null) {
            name = "Run String " + dsCount++;
        }

        stack.PushLuaFrame();

        Varargs error = _G.get("loadstring").call(LuaValue.valueOf(code), LuaValue.valueOf(name));
        
        stack.PopFrame();

        if (error.arg(1).isnil()) {
            RaiseError(error.arg(2).tojstring());
            return;
        }

        _G.get("xpcall").call(error.arg(1), new XLuaLibInternal.ErrorHandler());
        //_G.get("pcall").call(error.arg(1));

        if (stack.Level() == 0 && queueDelete) {
            XLuaGlobal.Get().DeleteState(this);
        }
    }

    public void LoadEmbedded (String name, CRunxlua cxr) {
        if (!cxr.luaMan.scripts.containsKey(name)) {
            return;
        }

        XLuaScriptRecord sr = cxr.luaMan.scripts.get(name);
        LoadString(sr.script, sr.name);
    }

    public void LoadDefaultLib (LuaPackage pkg) {

    }

    public void LoadCModule (String path, String name) {

    }

    public void RegisterFunction(String func) {
        RegisterFunction(func, 0, XLuaStack.MF_FUNCTION_GROUP_DEF);
    }

    public void RegisterFunction(String func, int minParams) {
        RegisterFunction(func, minParams, XLuaStack.MF_FUNCTION_GROUP_DEF);
    }

    public void RegisterFunction(String func, int minParams, int funcGroup) {
        _G.set(func, new XLuaLibInternal.RegCall(func, funcGroup, minParams));
    }

    public void SetIntegerVariable(String name, int n) {
        SetField(name, LuaValue.valueOf(n));
    }

    public void SetDoubleVariable(String name, double v) {
        SetField(name, LuaValue.valueOf(v));
    }

    public void SetStringVariable(String name, String str) {
        SetField(name, LuaValue.valueOf(str));
    }

    public void SetBooleanVariable(String name, boolean b) {
        SetField(name, LuaValue.valueOf(b));
    }

    public void SetNilVariable(String name) {
        SetField(name, LuaValue.NIL);
    }

    public int GetIntegerVariable(String name) {
        LuaValue v = GetField(name);
        if (!v.isnumber()) {
            RaiseError(name + " not a number", XLuaObject.MODE_EXPRESSION);
            return 0;
        }

        return v.toint();
    }

    public double GetDoubleVariable(String name) {
        LuaValue v = GetField(name);
        if (!v.isnumber()) {
            RaiseError(name + " not a number", XLuaObject.MODE_EXPRESSION);
            return 0;
        }

        return v.todouble();
    }

    public String GetStringVariable(String name) {
        LuaValue v = GetField(name);
        if (!v.isstring()) {
            RaiseError(name + " not a string", XLuaObject.MODE_EXPRESSION);
            return "";
        }

        return v.tojstring();
    }

    public boolean GetBooleanVariable(String name) {
        LuaValue v = GetField(name);

        return v.toboolean();
    }

    public String GetVariableType(String name) {
        LuaValue v = GetField(name);
        return v.typename();
    }

    public int GetTableLength(String name) {
        LuaValue v = GetField(name);
        if (!v.istable()) {
            RaiseError(name + " not a table", XLuaObject.MODE_EXPRESSION);
            return 0;
        }

        return v.checktable().length();
    }

    public int GetTableMaxIndex(String name) {
        LuaValue v = GetField(name);
        if (!v.istable()) {
            RaiseError(name + " not a table", XLuaObject.MODE_EXPRESSION);
            return 0;
        }

        return v.checktable().maxn();
    }

    CValue FieldEntry(String name) {
        if (name.charAt(name.length() - 1) == ']') {
            int off = 0;
            if (name.charAt(0) == '[') {
                off = 1;
            }

            if (name.charAt(off) == '\'' || name.charAt(off) == '"') {
                return new CValue(name.substring((1 + off), name.length() - (2 + off)));
            } else {
                try {
                    int i = Integer.parseInt(name.substring(off, name.length() - (1 + off)));
                    return new CValue(i);
                } catch (Exception e) {
                    return new CValue(0);
                }
            }
        }

        return new CValue(name);
    }

    // Returns Varargs pair {table, field_name/index}
    Varargs SeekField(String name) {
        StringTokenizer st = new StringTokenizer(name, ".[");

        LuaValue table = _G;
        LuaValue field = _G;

        while (st.hasMoreTokens()) {
            String tok = st.nextToken();
            if (tok.isEmpty()) {
                continue;
            }

            CValue entry = FieldEntry(tok);

            if (!st.hasMoreTokens()) {
                switch (entry.getType()) {
                case CValue.TYPE_STRING:
                    return LuaValue.varargsOf(table, LuaValue.valueOf(entry.getString()));
                case CValue.TYPE_INT:
                    return LuaValue.varargsOf(table, LuaValue.valueOf(entry.getInt()));
                }

                return LuaValue.NIL;
            }

            switch (entry.getType()) {
            case CValue.TYPE_STRING:
                table = table.get(entry.getString()); break;
            case CValue.TYPE_INT:
                table = table.get(entry.getInt()); break;
            }

            // If field exists but is not a table, report error and abort
            if (!table.istable()) {
                RaiseError("Attempt to index field '" + tok + "' (a " + table.typename() + " value)", XLuaObject.MODE_EXPRESSION);
                return LuaValue.NIL;
            }
        }

        return LuaValue.NIL;
    }

    LuaValue GetField(String name) {
        Varargs tf = SeekField(name);
        if (tf.arg(1).isnil()) {
            return LuaValue.NIL;
        }

        return tf.arg(1).get(tf.arg(2));
    }

    boolean SetField(String name, LuaValue val) {
        Varargs tf = SeekField(name);
        if (tf.arg(1).isnil()) {
            return false;
        }

        tf.arg(1).set(tf.arg(2), val);
        return true;
    }

    String TranslateLuaType(int type) {
        switch (type) {
            case LuaValue.TNIL:
                return "nil";
            case LuaValue.TBOOLEAN:
                return "boolean";
            case LuaValue.TLIGHTUSERDATA:
                return "lightuserdata";
            case LuaValue.TNUMBER:
                return "number";
            case LuaValue.TSTRING:
                return "string";
            case LuaValue.TTABLE:
                return "table";
            case LuaValue.TFUNCTION:
                return "function";
            case LuaValue.TUSERDATA:
                return "userdata";
            case LuaValue.TTHREAD:
                return "thread";
            default:
                return "unknown";
        }
    }
}
