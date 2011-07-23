/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Justin
 */
public class XLuaStackEventFrame extends XLuaStackFrame {

    protected XLuaParamSet _params = null;
    protected XLuaParamSet _returns = null;
    protected XLuaParamSet _paramBuffer = null;
    protected XLuaParamSet _returnBuffer = null;

    public XLuaStackEventFrame () {
        _params = new XLuaParamSet();
        _returns = new XLuaParamSet();
        _paramBuffer = new XLuaParamSet();
        _returnBuffer = new XLuaParamSet();
    }

    public XLuaParamSet Params () {
        return _params;
    }

    public XLuaParamSet Returns () {
        return _returns;
    }

    public XLuaParamSet ParamBuffer () {
        return _paramBuffer;
    }

    public XLuaParamSet ReturnBuffer () {
        return _returnBuffer;
    }

    public void MoveParamBufferToParams (XLuaStackEventFrame f) {
        f._params = _params;
        _params = new XLuaParamSet();
    }

    public void MoveReturnBufferToReturns (XLuaStackEventFrame f) {
        f._returns = _returns;
        _returns = new XLuaParamSet();
    }

    public XLuaStackFrame.FrameType Type () {
        return XLuaStackFrame.FrameType.EVENT_FRAME_TYPE;
    }
}
