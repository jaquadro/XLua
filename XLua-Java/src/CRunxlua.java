//----------------------------------------------------------------------------------
//
// CRUNSTATICTEXT: extension object
//
//----------------------------------------------------------------------------------
import Services.CFontInfo;
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.util.*;
import Extensions.*;
import Services.*;
import RunLoop.*;
import Expressions.*;
import Sprites.*;
import Conditions.*;
import Actions.*;
import Expressions.*;
import Objects.*;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;

public class CRunxlua extends CRunExtension {

    public static final int SIZEOF_EDIT_RECORD = 32;
    public static final int COUNTER_OFFSET = 20;

    public XLuaObject luaMan;

    public CRunxlua () {
    }

    public int getNumberOfConditions () {
        return 9;
    }

    public boolean createRunObject (CBinaryFile file, CCreateObjectInfo cob, int version) {
        luaMan = new XLuaObject();
        luaMan.ho = ho;

        int initState = file.readInt();

        luaMan.errorMode = file.readInt();
        luaMan.printMode = file.readInt();
        luaMan.runScript = true;

        boolean bindState = file.readByte() != 0;
        boolean useMMFI = file.readByte() != 0;
        boolean useWINI = file.readByte() != 0;
        boolean runScript = file.readByte() != 0;
        boolean loadLibs = file.readByte() != 0;

        luaMan.autoReg = file.readByte() != 0;
        luaMan.useBacktrace = file.readByte() != 0;

        file.skipBytes(1);

        int numScripts = file.readInt();
        int numLibs = file.readInt();
        int scriptRecordOffset = file.readInt() - COUNTER_OFFSET;
        int libRecordOffset = file.readInt() - COUNTER_OFFSET;
        int stringOffset = file.readInt() - COUNTER_OFFSET;

        file.skipBytes(16);

        // Read embedded scripts
        for (int i = 0; i < numScripts; i++) {
            XLuaScriptRecord sr = new XLuaScriptRecord();
            int srOffset = scriptRecordOffset + (i * SIZEOF_EDIT_RECORD);

            file.seek(srOffset);
            int erNameOffset = file.readInt() - COUNTER_OFFSET;
            int erNameLen = file.readInt();
            int erSrcOffset = file.readInt() - COUNTER_OFFSET;
            int erSrcLen = file.readInt();
            int erFlags = file.readInt();

            file.seek(erNameOffset);
            String erName = file.readString(erNameLen);

            file.seek(erSrcOffset);
            String erSrc = file.readString(erSrcLen);

            sr.name = erName;
            sr.script = erSrc;
            sr.loadAtStart = erFlags & 0x01;

            luaMan.scripts.put(sr.name, sr);
        }

        if (bindState) {
            luaMan.bindState(initState);

            if (loadLibs) {
                luaMan.state.LoadDefaultLib(XLuaState.LuaPackage.PACKAGE_ALL);
            }
        }

        return false;
    }

    public void destroyRunObject (boolean bFast) {
    }

    public int handleRunObject () {
        if (luaMan.state != null) {
            if (luaMan.autoReg) {
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_DEF, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_A, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_B, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_C, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_D, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_E, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_F, 0);
                ho.generateEvent(XLuaStack.MF_FUNCTION_GROUP_G, 0);
                luaMan.autoReg = false;
            }

            if (luaMan.runScript) {
                for (XLuaScriptRecord sr : luaMan.scripts.values()) {
                    if (sr.loadAtStart == 1) {
                        luaMan.state.LoadEmbedded(sr.name, this);
                        sr.loadAtStart = 0;

                        if (luaMan.state == null) {
                            return 0;
                        }
                    }
                }
                luaMan.runScript = false;
            }
        }

        return 0;
    }

    public void displayRunObject (Graphics2D g2) {
    }

    public void pauseRunObject () {
    }

    public void continueRunObject () {
    }

    public boolean saveRunObject (DataOutputStream stream) {
        return true;
    }

    public boolean loadRunObject (DataInputStream stream) {
        return true;
    }

    public void saveBackground (BufferedImage img) {
    }

    public void restoreBackground (Graphics2D g) {
    }

    public void killBackground () {
    }

    public CFontInfo getRunObjectFont () {
        return null;
    }

    public void setRunObjectFont (CFontInfo fi, CRect rc) {
    }

    public int getRunObjectTextColor () {
        return 0;
    }

    public void setRunObjectTextColor (int rgb) {
    }

    public CMask getRunObjectCollisionMask (int flags) {
        return null;
    }

    public BufferedImage getRunObjectSurface () {
        return null;
    }

    public void getZoneInfos () {
    }

    // Conditions
    // --------------------------------------------------
    public boolean condition (int num, CCndExtension cnd) {
        switch (num) {
        case 0:
            return condOnError(cnd);
        case 1:
            return condOnFunction(cnd);
        case 2:
            return condOnPrint(cnd);
        default:
            luaMan.raiseError("Condition not supported in Java Runtime");
            break;
        }
        return false;
    }

    boolean condOnError (CCndExtension cnd) {
        return true;
    }

    boolean condOnFunction (CCndExtension cnd) {
        String func = cnd.getParamExpString(rh, 0);
        if (luaMan.autoReg) {
            luaMan.state.RegisterFunction(func, 0);
            return false;
        }
        
        if (luaMan.state != null && luaMan.state.stack.GetFunction().compareTo(func) == 0) {
            return true;
        }

        return false;
    }

    boolean condOnPrint (CCndExtension cnd) {
        return true;
    }

    // Actions
    // -------------------------------------------------
    public void action (int num, CActExtension act) {
        switch (num) {
        case 1:
            actRunString(act);
            break;
        case 2:
            actPushIntegerParam(act);
            break;
        case 3:
            actPushStringParam(act);
            break;
        case 4:
            actPushBooleanParam(act);
            break;
        case 5:
            actCallFunction(act);
            break;
        case 6:
            actPushIntegerReturn(act);
            break;
        case 7:
            actPushStringReturn(act);
            break;
        case 8:
            actPushBooleanReturn(act);
            break;
        case 19:
            actRegisterFunction(act);
            break;
        case 20:
            actSetIntegerVar(act);
            break;
        case 21:
            actSetStringVar(act);
            break;
        case 22:
            actSetBooleanVar(act);
            break;
        case 34:
            actCreateLuaState(act);
            break;
        case 35:
            actDeleteLuaState(act);
            break;
        case 36:
            actBindLuaState(act);
            break;
        case 37:
            actUnbindLuaState(act);
            break;
        case 39:
            actPushDoubleParam(act);
            break;
        case 40:
            actPushDoubleReturn(act);
            break;
        case 46:
            actSetDoubleVar(act);
            break;
        //case 47:
        //    actClearMMFIExport(act);
        //    break;
        case 48:
            actDeleteVar(act);
            break;
        case 49:
            actCallFuncInline1(act);
            break;
        case 50:
            actCallFuncInline2(act);
            break;
        case 51:
            actCallFuncInline3(act);
            break;
        case 52:
            actCallFuncInline4(act);
            break;
        case 53:
            actCallFuncInlineDelim(act);
            break;
        default:
            luaMan.raiseError("Action not supported in Java Runtime");
            break;
        }
    }

    void actRunString (CActExtension act) {
        String src = act.getParamExpString(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.LoadString(src, null);
        }
    }

    void actPushIntegerParam (CActExtension act) {
        int param = act.getParamExpression(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushParam(param);
        }
    }

    void actPushStringParam (CActExtension act) {
        String param = act.getParamExpString(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushParam(param);
        }
    }

    void actPushBooleanParam (CActExtension act) {
        int param = act.getParamExpression(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushParam(param != 0);
        }
    }

    void actCallFunction (CActExtension act) {
        String func = act.getParamExpString(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.CallLuaFunction(func);
        }
    }

    void actPushIntegerReturn (CActExtension act) {
        int param = act.getParamExpression(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushReturn(param);
        }
    }

    void actPushStringReturn (CActExtension act) {
        String param = act.getParamExpString(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushReturn(param);
        }
    }

    void actPushBooleanReturn (CActExtension act) {
        int param = act.getParamExpression(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushReturn(param != 0);
        }
    }

    void actRegisterFunction (CActExtension act) {
        String func = act.getParamExpString(rh, 0);
        int params = act.getParamExpression(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.RegisterFunction(func, params, XLuaStack.MF_FUNCTION_GROUP_DEF);
        }
    }

    void actSetIntegerVar (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        int val = act.getParamExpression(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.SetIntegerVariable(name, val);
        }
    }

    void actSetStringVar (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String val = act.getParamExpString(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.SetStringVariable(name, val);
        }
    }

    void actSetBooleanVar (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        int val = act.getParamExpression(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.SetBooleanVariable(name, val != 0);
        }
    }

    void actCreateLuaState (CActExtension act) {
        int sid = act.getParamExpression(rh, 0);
        XLuaGlobal.Get().CreateState(sid);
    }

    void actDeleteLuaState (CActExtension act) {
        int sid = act.getParamExpression(rh, 0);
        XLuaGlobal.Get().DeleteState(sid);
    }

    void actBindLuaState (CActExtension act) {
        int sid = act.getParamExpression(rh, 0);
        luaMan.bindState(sid);
    }

    void actUnbindLuaState (CActExtension act) {
        luaMan.unbindState();
    }

    void actPushDoubleParam (CActExtension act) {
        double param = act.getParamExpDouble(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushParam(param);
        }
    }

    void actPushDoubleReturn (CActExtension act) {
        double param = act.getParamExpDouble(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.stack.PushReturn(param);
        }
    }

    void actSetDoubleVar (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        double val = act.getParamExpDouble(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.SetDoubleVariable(name, val);
        }
    }

    void actDeleteVar (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        if (luaMan.state != null) {
            luaMan.state.SetNilVariable(name);
        }
    }

    void actCallFuncInline1 (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String p1 = act.getParamExpString(rh, 1);
        if (luaMan.state != null) {
            luaMan.state.stack.ClearLFParameters();
            luaMan.state.stack.PushParam(p1);
            luaMan.state.stack.CallLuaFunction(name);
        }
    }

    void actCallFuncInline2 (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String p1 = act.getParamExpString(rh, 1);
        String p2 = act.getParamExpString(rh, 2);
        if (luaMan.state != null) {
            luaMan.state.stack.ClearLFParameters();
            luaMan.state.stack.PushParam(p1);
            luaMan.state.stack.PushParam(p2);
            luaMan.state.stack.CallLuaFunction(name);
        }
    }

    void actCallFuncInline3 (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String p1 = act.getParamExpString(rh, 1);
        String p2 = act.getParamExpString(rh, 2);
        String p3 = act.getParamExpString(rh, 3);
        if (luaMan.state != null) {
            luaMan.state.stack.ClearLFParameters();
            luaMan.state.stack.PushParam(p1);
            luaMan.state.stack.PushParam(p2);
            luaMan.state.stack.PushParam(p3);
            luaMan.state.stack.CallLuaFunction(name);
        }
    }

    void actCallFuncInline4 (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String p1 = act.getParamExpString(rh, 1);
        String p2 = act.getParamExpString(rh, 2);
        String p3 = act.getParamExpString(rh, 3);
        String p4 = act.getParamExpString(rh, 4);
        if (luaMan.state != null) {
            luaMan.state.stack.ClearLFParameters();
            luaMan.state.stack.PushParam(p1);
            luaMan.state.stack.PushParam(p2);
            luaMan.state.stack.PushParam(p3);
            luaMan.state.stack.PushParam(p4);
            luaMan.state.stack.CallLuaFunction(name);
        }
    }

    void actCallFuncInlineDelim (CActExtension act) {
        String name = act.getParamExpString(rh, 0);
        String delim = act.getParamExpString(rh, 1);
        if (luaMan.state == null) {
            return;
        }

        luaMan.state.stack.ClearLFParameters();

        StringTokenizer tok = new StringTokenizer(delim, "|");
        while (tok.hasMoreTokens()) {
            luaMan.state.stack.PushParam(tok.nextToken());
        }

        luaMan.state.stack.CallLuaFunction(name);
    }


    // Expressions
    // --------------------------------------------
    public CValue expression (int num) {
        switch (num) {
        case 0:
            return exprErrorString();
        case 1:
            return exprLuaIntegerReturn();
        case 2:
            return exprLuaStringReturn();
        case 3:
            return exprLuaBooleanReturn();
        case 4:
            return exprLuaReturnCount();
        case 5:
            return exprMMFIntegerParam();
        case 6:
            return exprMMFStringParam();
        case 7:
            return exprMMFBooleanParam();
        case 8:
            return exprMMFParamCount();
        case 9:
            return exprIntegerVar();
        case 10:
            return exprStringVar();
        case 11:
            return exprBooleanVar();
        case 12:
            luaMan.raiseError("'Get Local Value' expression not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0);
        case 13:
            luaMan.raiseError("'Get Local String' expression not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue("");
        case 14:
            return exprPrintString();
        case 15:
            luaMan.raiseError("Inline-function [->I] expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0);
        case 16:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue("");
        case 17:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0);
        case 18:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0);
        case 19:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue("");
        case 20:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue("");
        case 21:
            return exprLuaDoubleReturn();
        case 22:
            return exprMMFDoubleParam();
        case 23:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0f);
        case 24:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0f);
        case 25:
            luaMan.raiseError("Inline-function expressions not supported in Java Runtime", XLuaObject.MODE_EXPRESSION);
            return new CValue(0f);
        case 26:
            return exprDoubleVar();
        case 27:
            return exprTableLength();
        case 28:
            return exprTableMaxN();
        case 29:
            return exprEmbeddedDep();
        case 30:
            return exprErrorMode();
        case 31:
            return exprPrintMode();
        case 32:
            return exprVarType();
        case 33:
            return exprEmbedded();
        default:
            luaMan.raiseError("Unknown expression", XLuaObject.MODE_EXPRESSION);
            return null;
        }
        //return null;
    }

    CValue exprErrorString () {
        return new CValue(luaMan.getError());
    }

    CValue exprLuaIntegerReturn () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.stack.GetNumberReturn(index));
    }

    CValue exprLuaStringReturn () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue("");
        }
        return new CValue(luaMan.state.stack.GetStringReturn(index));
    }

    CValue exprLuaBooleanReturn () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.stack.GetBooleanReturn(index) ? 1 : 0);
    }

    CValue exprLuaReturnCount () {
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.stack.GetReturnCount());
    }

    CValue exprMMFIntegerParam () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.stack.GetNumberParam(index));
    }

    CValue exprMMFStringParam () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue("");
        }
        return new CValue(luaMan.state.stack.GetStringParam(index));
    }

    CValue exprMMFBooleanParam () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.stack.GetBooleanParam(index) ? 1 : 0);
    }

    CValue exprMMFParamCount () {
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.stack.GetParamCount());
    }

    CValue exprIntegerVar () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.GetIntegerVariable(name));
    }

    CValue exprStringVar () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue("");
        }
        return new CValue(luaMan.state.GetStringVariable(name));
    }

    CValue exprBooleanVar () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return luaMan.state.GetBooleanVariable(name) ? new CValue(1) : new CValue(0);
    }

    CValue exprPrintString () {
        return new CValue(luaMan.getPrint());
    }

    CValue exprLuaDoubleReturn () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.stack.GetNumberReturn(index));
    }

    CValue exprMMFDoubleParam () {
        int index = ho.getExpParam().getInt();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.stack.GetNumberParam(index));
    }

    CValue exprDoubleVar () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue(luaMan.state.GetDoubleVariable(name));
    }

    CValue exprTableLength () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.GetTableLength(name));
    }

    CValue exprTableMaxN () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue(0);
        }
        return new CValue((int) luaMan.state.GetTableMaxIndex(name));
    }

    CValue exprEmbeddedDep () {
        return new CValue(luaMan.GetEmbeddedScript("Main"));
    }

    CValue exprErrorMode () {
        if (luaMan.errorMode == 0) {
            return new CValue("Immediate");
        } else if (luaMan.errorMode == 1) {
            return new CValue("Queued");
        }
        return new CValue("");
    }

    CValue exprPrintMode () {
        if (luaMan.printMode == 0) {
            return new CValue("Immediate");
        } else if (luaMan.printMode == 1) {
            return new CValue("Queued");
        }
        return new CValue("");
    }

    CValue exprVarType () {
        String name = ho.getExpParam().getString();
        if (luaMan.state == null) {
            return new CValue("");
        }
        return new CValue(luaMan.state.GetVariableType(name));
    }

    CValue exprEmbedded () {
        String name = ho.getExpParam().getString();
        return new CValue(luaMan.GetEmbeddedScript(name));
    }
}
