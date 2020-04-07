package com.nemoli.sprite;

import android.view.KeyEvent;
import android.view.View;

/**
 * @author : wangli
 * @date : 2020/4/3
 * Email: wangli@znds.com
 */
public class TvControl {

    private final GestureClass gesture;

    public View.OnKeyListener onKeyListener = new View.OnKeyListener() {
        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
                    gesture.ScrollNative(10, 0, 10, 540);
                    return true;
                } else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
                    gesture.ScrollNative(-10, 0, 10, 540);
                    return true;
                }
            }
            return false;
        }
    };

    public TvControl(GestureClass mGestureObject) {
        this.gesture = mGestureObject;
    }
}
