
package org.cocos2dx.simplegame;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;

public class SimpleGame extends Cocos2dxActivity{

	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
	}
	
    static {
        System.loadLibrary("game");
    }
}
