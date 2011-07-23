/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.*;
import org.luaj.vm2.*;

/**
 *
 * @author Justin
 */
public class XLuaParamSet {

    //protected ArrayList<XLuaParam> _params;
   // protected XLuaParamTable _tablePtr = null;

    protected Varargs _params = LuaValue.NONE;
    protected Varargs _tablePtr = null;

    protected ArrayList<Varargs> _tableStack = null;

    public XLuaParamSet () {
        _tableStack = new ArrayList<Varargs>();
    }

    public Varargs Params () {
        return _params;
    }

    public void PushNil () {
        if (_tablePtr == null) {
            _params = LuaValue.varargsOf(LuaValue.NIL, _params);
        } else {
            _tablePtr = LuaValue.varargsOf(LuaValue.NIL, _tablePtr);
        }
    }

    public void PushNumber (double v) {
        if (_tablePtr == null) {
            _params = LuaValue.varargsOf(LuaValue.valueOf(v), _params);
        } else {
            _tablePtr = LuaValue.varargsOf(LuaValue.valueOf(v), _tablePtr);
        }
    }

    public void PushBoolean (boolean b) {
        if (_tablePtr == null) {
            _params = LuaValue.varargsOf(LuaValue.valueOf(b), _params);
        } else {
            _tablePtr = LuaValue.varargsOf(LuaValue.valueOf(b), _tablePtr);
        }
    }

    public void PushString (String str) {
        if (_tablePtr == null) {
            _params = LuaValue.varargsOf(LuaValue.valueOf(str), _params);
        } else {
            _tablePtr = LuaValue.varargsOf(LuaValue.valueOf(str), _tablePtr);
        }
    }

    //public void PushTable (LuaState L, int i) {
        // Unsupported
    //}

    public void StartTable () {
        _tableStack.add(_tablePtr);

        if (_tablePtr == null) {
            _params = LuaValue.varargsOf(LuaValue.NONE, _params);
            _tablePtr = _params.arg(_params.narg());
        } else {
            _tablePtr = LuaValue.varargsOf(LuaValue.NONE, _tablePtr);
            _tablePtr = _tablePtr.arg(_tablePtr.narg());
        }
    }

    public void EndTable () {
        if (_tablePtr == null) {
            return;
        }

        _tablePtr = _tableStack.get(_tableStack.size() - 1);
        _tableStack.remove(_tableStack.size() - 1);
    }

    public LuaValue GetParam (int index) {
        if (index > _params.narg()) {
            return null;
        }

        return _params.arg(index);
    }

    public int Count () {
        return _params.narg();
    }

    public void Clear () {
        _params = LuaValue.NONE;

        _tablePtr = null;
        _tableStack.clear();
    }
}
