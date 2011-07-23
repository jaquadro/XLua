/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Justin
 */
public class XLuaStackFrame {

    public enum FrameType {
        BASE_FRAME_TYPE,
        EVENT_FRAME_TYPE,
        LUA_FRAME_TYPE
    }

    protected XLuaStackFrame _prevFrame = null;
    protected XLuaStackFrame _nextFrame = null;

    protected String _unitName = "";

    public XLuaStackFrame () { }

    public XLuaStackFrame Next () {
        return _nextFrame;
    }

    public XLuaStackFrame Prev () {
        return _prevFrame;
    }

    public void BindNext (XLuaStackFrame f) {
        _nextFrame = f;
    }

    public void BindPrev (XLuaStackFrame f) {
        _prevFrame = f;
    }

    public void SetUnitName (String str) {
        _unitName = str;
    }

    public String GetUnitName () {
        return _unitName;
    }

    public String CalledBy () {
        return "";
    }

    public FrameType Type () {
        return FrameType.BASE_FRAME_TYPE;
    }

}
