/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.*;

import org.luaj.vm2.lib.OneArgFunction;
import org.luaj.vm2.lib.VarArgFunction;
import org.luaj.vm2.lib.ZeroArgFunction;
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
 * @author Justin
 */
public class XLuaLibInternal {

    public static class RegCall extends VarArgFunction {

        protected String _func = "";
        protected int _funcGroup = 0;
        protected int _minParams = 0;

        RegCall (String func, int funcGroup, int minParams) {
            _func = func;
            _funcGroup = funcGroup;
            _minParams = minParams;
        }

        public Varargs invoke (Varargs args) {
            XLuaState state = XLuaGlobal.Get().GetStateByState(LuaThread.getGlobals());
            if (state == null) {
                return null;
            }

            int actualParams = args.narg();
            if (actualParams < _minParams) {
                error("Function " + _func + " expected " + _minParams + " parameters, got " + actualParams);
                return null;
            }

            return state.stack.CallMMFFunction(_func, _funcGroup, state._G, args);
        }
    }

    public static class ErrorHandler extends OneArgFunction {

        public LuaValue call (LuaValue arg) {
            XLuaState state = XLuaGlobal.Get().GetStateByState(LuaThread.getGlobals());
            if (state == null) {
                return NIL;
            }

            String message = arg.tojstring();
            String bt = "";

            // Manually raise errors depending on XLua settings
            Iterator<XLuaObject> iter = state.rdList.iterator();
            while (iter.hasNext()) {
                XLuaObject lm = iter.next();
                if (lm != null) {
                    if (lm.useBacktrace) {
                        // Let's be lazy about backtrace calculations
                        if (bt.isEmpty()) {
                            //vm.pushinteger(1);
                            // db_errorfb();
                            //bt = vm.tostring(1);
                        }

                        lm.raiseError(bt + XLuaState.newline);
                    } else {
                        lm.raiseError(message + XLuaState.newline);
                    }
                }
            }

            return LuaValue.valueOf(message);
        }
    }
}
