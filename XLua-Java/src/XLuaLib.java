/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.*;

import org.luaj.vm2.lib.OneArgFunction;
import org.luaj.vm2.lib.VarArgFunction;
import org.luaj.vm2.LuaClosure;
import org.luaj.vm2.LuaFunction;
import org.luaj.vm2.LuaString;
import org.luaj.vm2.LuaTable;
import org.luaj.vm2.LuaThread;
import org.luaj.vm2.LuaValue;
import org.luaj.vm2.LoadState;
import org.luaj.vm2.UpValue;
import org.luaj.vm2.Varargs;

/**
 *
 * @author admin
 */
public class XLuaLib extends OneArgFunction {
    
    // Type I
    
    // Type V
    //public static final int INIT = 0;
    public static final int DOCALL = 0;
    public static final int DOCALL_A = 1;
    public static final int DOCALL_B = 2;
    public static final int DOCALL_C = 3;
    public static final int DOCALL_D = 4;
    public static final int DOCALL_E = 5;
    public static final int DOCALL_F = 6;
    public static final int DOCALL_G = 7;

    public static XLuaLib XLUALIB = null;

    public XLuaLib () {
        XLUALIB = this;
    }

    public LuaValue call (LuaValue arg) {
        bind(env, XLuaLibV.class, new String[] {
            "DoCall", "DoCallA", "DoCallB", "DoCallC", "DoCallD", "DoCallE",
            "DoCallF", "DoCallG",
        } );

        return env;
    }

    public static final class XLuaLibV extends VarArgFunction {
        protected XLuaLib xlualib;

        public Varargs invoke (Varargs args) {
            switch (opcode) {
            case DOCALL:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_DEF);
            case DOCALL_A:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_A);
            case DOCALL_B:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_B);
            case DOCALL_C:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_C);
            case DOCALL_D:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_D);
            case DOCALL_E:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_E);
            case DOCALL_F:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_F);
            case DOCALL_G:
                return DoCall(args, XLuaStack.MF_FUNCTION_GROUP_G);
            }

            return NONE;
        }

        public Varargs DoCall (Varargs args, int funcGroup) {
            XLuaState state = XLuaGlobal.Get().GetStateByState(LuaThread.getGlobals());
            if (state == null) {
                return null;
            }

            if (args.narg() < 1) {
                error("DoCall: No function identifier passed");
                return null;
            }

            String func = args.checkjstring(1);

            return state.stack.CallMMFFunction(func, funcGroup, state._G, args.subargs(2));
        }
    }

}
