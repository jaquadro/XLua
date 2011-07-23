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
public final class XLuaStack {

    public static final int MF_FUNCTION_GROUP_DEF = 1;
    public static final int MF_FUNCTION_GROUP_A = 9;
    public static final int MF_FUNCTION_GROUP_B = 10;
    public static final int MF_FUNCTION_GROUP_C = 11;
    public static final int MF_FUNCTION_GROUP_D = 12;
    public static final int MF_FUNCTION_GROUP_E = 13;
    public static final int MF_FUNCTION_GROUP_F = 14;
    public static final int MF_FUNCTION_GROUP_G = 15;

    protected XLuaState _state = null;

    // Top stack framae
    protected XLuaStackFrame _stack = null;

    // Top event stack frame
    protected XLuaStackEventFrame _topEventFrame = null;

    // Current number of stack frames
    protected int _size = 0;

    // Nesting level
    protected int _level = 0;

    protected boolean	_exception = false;

    public XLuaStack () {
        Init();
    }

    public XLuaStack (XLuaState L) {
        _state = L;
        Init();
    }

    public void Init () {
        PushEventFrame();
    }

    public void Reinit () {
        ClearStack();
        PushEventFrame();
    }

    public void SetState (XLuaState L) {
        _state = L;
    }

    public boolean Exception () {
        return _exception;
    }

    public void ClearException () {
        _exception = false;
    }

    public int Level () {
        return _level;
    }

    // INTERNAL ACCESS
    public int EventNumber () {
        try {
            return _state.rdList.iterator().next().ho.hoAdRunHeader.rhEvtProg.rhEventGroup.evgIdentifier;
        } catch (Exception e) {
            return -1;
        }
    }

    public XLuaStackFrame TopFrame () {
        return _stack;
    }

    public XLuaStackEventFrame TopEventFrame () {
        return _topEventFrame;
    }

    public XLuaStackFrame BaseFrame () {
        XLuaStackFrame f = TopFrame();
        while (f != null && f.Prev() != null) {
            f = f.Prev();
        }

        return f;
    }

    public int FrameCount () {
        return _size;
    }

    public XLuaStackEventFrame PushEventFrame () {
        XLuaStackEventFrame f = new XLuaStackEventFrame();

        if (_stack != null) {
            f.BindPrev(_stack);
            _stack.BindNext(f);
        }

        _stack = f;
        _topEventFrame = f;
        _size++;

        return f;
    }

    public XLuaStackLuaFrame PushLuaFrame () {
        XLuaStackLuaFrame f = new XLuaStackLuaFrame();

        if (_stack != null) {
            f.BindPrev(_stack);
            _stack.BindNext(f);
        }

        _stack = f;
        _size++;

        return f;
    }

    public void PopFrame () {
        if (_stack != null) {
            XLuaStackFrame f = _stack;
            _stack = _stack.Prev();
            _stack.BindNext(null);

            XLuaStackFrame t = _stack;
            while (t != null && t.Type() != XLuaStackFrame.FrameType.EVENT_FRAME_TYPE) {
                t = t.Prev();
            }
            _topEventFrame = (XLuaStackEventFrame)t;
        }
    }

    public void ClearStack () {
        _stack = null;
        _size = 0;
    }

    public int CallLuaFunction (String func) {
        _level++;
        
        // Get M frame
        XLuaStackEventFrame mFrame = TopEventFrame();

        // For a nested table call, seek the parent table and place it on stack
        Varargs tf = _state.SeekField(func);
        LuaValue fieldTable = tf.arg(1);
        LuaValue fieldFunc = tf.arg(2);

        Varargs params = mFrame.ParamBuffer().Params();

        if (fieldFunc.type() == LuaValue.TSTRING) {
            String element = fieldFunc.tojstring();
            int opIndex = element.indexOf(':');
            if (opIndex > -1) {
                // : operator present

                String fieldTableStr = element.substring(0, opIndex);
                String fieldFuncStr = element.substring(opIndex + 1);

                fieldTable = fieldTable.get(fieldTableStr);
                if (!fieldTable.istable()) {
                    _state.RaiseError("Event " + EventNumber() + ": attempt to call a " + fieldTable.typename() + " value");
                    return 0;
                }

                fieldFunc = fieldTable.get(fieldFuncStr);
                if (!fieldFunc.isfunction()) {
                    _state.RaiseError("Event " + EventNumber() + ": attempt to call a " + fieldFunc.typename() + " value");
                    return 0;
                }

                params = LuaValue.varargsOf(fieldTable, params);
            } else {
                // : operator not present, string index

                fieldFunc = fieldTable.get(element);
            }
        } else {
            // integer index

            fieldFunc = fieldTable.get(fieldFunc);
        }

        if (!fieldFunc.isfunction()) {
            _state.RaiseError("Event " + EventNumber() + ": attempt to call a " + fieldFunc.typename() + " value");
            return 0;
        }
        
        // Create L frame
        XLuaStackLuaFrame lFrame = PushLuaFrame();
        lFrame.SetUnitName(func);
        
        // Push all params onto stack
        mFrame.ParamBuffer().Clear();
        
        // Setup error handler
        // int base = state.gettop() - (paramCount + paramAdjust);
        // state.pushfunction(...);
        // state.insert(base);
        
        // Do protected call
        Varargs returns = _state._G.get("xpcall").invoke(fieldFunc, new XLuaLibInternal.ErrorHandler(), params);

        // No longer need L frame since Lua buffers on itsown
        PopFrame();
        
        // Clean out returns list
        mFrame.Returns().Clear();
        
        // Get count of returned items
        int returnCount = returns.narg();
        
        // Capture all returned items
        for (int i = 2; i <= returnCount; i++) {
            switch (returns.arg(i).type()) {
                case LuaValue.TNUMBER:
                    mFrame.Returns().PushNumber(returns.arg(i).todouble()); break;
                case LuaValue.TSTRING:
                    mFrame.Returns().PushString(returns.arg(i).tojstring()); break;
                case LuaValue.TBOOLEAN:
                    mFrame.Returns().PushBoolean(returns.arg(i).toboolean()); break;
                default:
                    _state.RaiseError("Event " + EventNumber() + ": Invalid return type at index " + i);
                    mFrame.Returns().PushNil();
                    break;
            }
        }

        _level--;

        // If we're waiting to die and we finished unwinding
        if (_level == 0 && _state.queueDelete) {
            XLuaGlobal.Get().DeleteState(_state);
        }

        return returnCount - 1;
    }

    public Varargs CallMMFFunction (String func, int funcgroup, LuaValue env, Varargs args) {
        _level++;

        if (env == null) {
            env = _state._G;
        }

        // Get number of parameters
        int paramCount = args.narg();

        // Create new event frame
        XLuaStackEventFrame mFrame = PushEventFrame();
        mFrame.SetUnitName(func);

        //
        //
        //

        // Capture all parameters
        for (int i = 1; i <= paramCount; i++) {
            switch (args.type(i)) {
                case LuaValue.TNUMBER:
                    mFrame.Params().PushNumber(args.checkdouble(i)); break;
                case LuaValue.TSTRING:
                    mFrame.Params().PushString(args.checkjstring(i)); break;
                case LuaValue.TBOOLEAN:
                    mFrame.Params().PushBoolean(args.checkboolean(i)); break;
                default:
                    _state.RaiseError("Event " + EventNumber() + ": Invalid parameter type at index " + i);
                    mFrame.Params().PushNil();
                    break;
            }
        }

        // Call MMF event(s) corresponding to this function
        for (XLuaObject rd : _state.rdList) {
            if (rd != null) {
                rd.ho.generateEvent(funcgroup, 0);
            }
        }

        // Handle a disconnect request if we recievd it
        _state.ReleaseRd();

        // Push collected return values onto the stack
        Varargs returnValues = mFrame.ReturnBuffer().Params();

        // Don't need event frame anymore, going back to Lua
        PopFrame();

        _level--;
        return returnValues;
    }

    public int CallMMFFunctionFromMMF (String func) {
        _level++;

        // Get current event frame
        XLuaStackEventFrame mFrame = TopEventFrame();

        // Check stack size
        int paramCount = mFrame.ParamBuffer().Count();

        // Create new event frame
        XLuaStackEventFrame mFrame2 = PushEventFrame();
        mFrame2.SetUnitName(func);

        // Transfer param buffer of current frame to new frame
        mFrame.MoveParamBufferToParams(mFrame2);

        // Call MMF event(s) corresponding to this function
        for (XLuaObject rd : _state.rdList) {
            if (rd != null) {
                rd.ho.generateEvent(1, 0);
            }
        }

        // Get return count
        int returnCount = mFrame2.ReturnBuffer().Count();

        // Transfer return buffer back
        mFrame2.MoveReturnBufferToReturns(mFrame);

        // Pop "new" frame off stack
        PopFrame();

        _level--;
        return returnCount;
    }

    public String GetFunction () {
        if (TopEventFrame() == null) {
            _state.RaiseError("Event " + EventNumber() + ": Invalid place to get MMF function name");
            return "";
        }
        System.out.println("Stack getFunction: " + TopEventFrame().GetUnitName());

        return TopEventFrame().GetUnitName();
    }

    public void ClearMFParameters () {
        TopEventFrame().Params().Clear();
    }

    public void ClearMFReturns () {
        TopEventFrame().Returns().Clear();
    }

    public void ClearLFParameters () {
        TopEventFrame().ParamBuffer().Clear();
    }

    public void ClearLFReturns () {
        TopEventFrame().ReturnBuffer().Clear();
    }

    //
    // PushParam
    //

    public void PushParam () {
        TopEventFrame().ParamBuffer().PushNil();
    }

    public void PushParam (int n) {
        TopEventFrame().ParamBuffer().PushNumber(n);
    }

    public void PushParam (double v) {
        TopEventFrame().ParamBuffer().PushNumber(v);
    }

    public void PushParam (String s) {
        TopEventFrame().ParamBuffer().PushString(s);
    }

    public void PushParam (boolean b) {
        TopEventFrame().ParamBuffer().PushBoolean(b);
    }

    public void StartParamTable () {
        TopEventFrame().ParamBuffer().StartTable();
    }

    public void EndParamTable () {
        TopEventFrame().ParamBuffer().EndTable();
    }

    //
    // PushReturn
    //

    public void PushReturn () {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().PushNil();
        }
    }

    public void PushReturn (int n) {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().PushNumber(n);
        }
    }

    public void PushReturn (double v) {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().PushNumber(v);
        }
    }

    public void PushReturn (String s) {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().PushString(s);
        }
    }

    public void PushReturn (boolean b) {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().PushBoolean(b);
        }
    }

    public void StartReturnTable () {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().StartTable();
        }
    }

    public void EndReturnTable () {
        if (CheckPushReturn()) {
            TopEventFrame().ReturnBuffer().EndTable();
        }
    }

    public boolean CheckPushReturn () {
        if (FrameCount() <= 1) {
            _state.RaiseError("Event " + EventNumber() + "Invalid place to push MMF return value");
            return false;
        }

        return true;
    }

    //
    // GetParam
    //

    public double GetNumberParam (int index) {
        return GetParam(index).todouble();
    }

    public String GetStringParam (int index) {
        return GetParam(index).tojstring();
    }

    public boolean GetBooleanParam (int index) {
        return GetParam(index).toboolean();
    }

    public LuaValue GetParam (int index) {
        XLuaStackEventFrame f = TopEventFrame();
        if (f == null) {
            _state.RaiseError("Event " + EventNumber() + "Invalid place to get MMF parameter", XLuaObject.MODE_EXPRESSION);
            return LuaValue.NIL;
        }

        if (f.Params().Count() < index) {
            _state.RaiseError("Event " + EventNumber() + "Invalid parameter index requested", XLuaObject.MODE_EXPRESSION);
            return LuaValue.NIL;
        }

        return f.Params().GetParam(index);
    }

    public int GetParamCount () {
        return TopEventFrame().Params().Count();
    }

    //
    // GetReturn
    //

    public double GetNumberReturn (int index) {
        return GetReturn(index).todouble();
    }

    public String GetStringReturn (int index) {
        return GetReturn(index).tojstring();
    }

    public boolean GetBooleanReturn (int index) {
        return GetReturn(index).toboolean();
    }

    public LuaValue GetReturn (int index) {
        XLuaStackEventFrame f = TopEventFrame();
        if (f.Returns().Count() < index) {
            _state.RaiseError("Event " + EventNumber() + "Invalid return index requested", XLuaObject.MODE_EXPRESSION);
            return LuaValue.NIL;
        }

        return f.Returns().GetParam(index);
    }

    public int GetReturnCount () {
        return TopEventFrame().Returns().Count();
    }

}
