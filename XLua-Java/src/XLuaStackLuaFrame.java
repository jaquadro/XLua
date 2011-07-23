/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Justin
 */
public class XLuaStackLuaFrame extends XLuaStackFrame {

    public XLuaStackLuaFrame () { }

    public XLuaStackFrame.FrameType Type () {
        return XLuaStackFrame.FrameType.LUA_FRAME_TYPE;
    }
}
