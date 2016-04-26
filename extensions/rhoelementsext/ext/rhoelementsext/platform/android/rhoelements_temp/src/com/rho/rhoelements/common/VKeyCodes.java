/**
 * 
 */
package com.rho.rhoelements.common;

import java.util.HashMap;

import android.view.KeyEvent;

/**
 * @author FPC843
 * This class translates Android key codes in Windows key codes and viceversa
 * There is one map storing the Android to Windows relationship and another one storing the reverse relationship.
 * This is done to improve Time performances, as we don't want to iterate over a map when key events occur and we need the reverse relationship. 
 */
public class VKeyCodes
{
	public static final HashMap<Integer, Integer> androidToWindowsEventMap = new HashMap<Integer, Integer>();
	public static final HashMap<Integer, Integer> windowsToAndroidEventMap = new HashMap<Integer, Integer>();
	
	/**
	 * This method initializes the key event maps and has to be called before using them.
	 */
	public static void init()
	{
		initAndroid2WindowsMap();
		initWindows2AndroidMap();
	}
	
	private static void initAndroid2WindowsMap()
	{
		
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_DEL, 0x08); //to VK_BACK (BACKSPACE key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_TAB, 0x09); //to VK_TAB (TAB key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_CLEAR, 0x0C); //to VK_CLEAR (CLEAR key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_ENTER, 0x0D); //to VK_RETURN (ENTER key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_MENU, 0x12); //to VK_MENU (ALT key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE, 0x13); //to VK_PAUSE (PAUSE key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_SPACE, 0x20); //to VK_SPACE (Space bar)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_PAGE_UP, 0x21); //to VK_PRIOR (PAGE UP key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_PAGE_DOWN, 0x22); //to VK_NEXT (PAGE DOWN key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_DPAD_LEFT, 0x25); //to VK_LEFT (LEFT ARROW key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_DPAD_UP, 0x26); //to VK_UP (UP ARROW key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_DPAD_RIGHT, 0x27); //to VK_RIGHT (RIGHT ARROW key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_DPAD_DOWN, 0x28); //to VK_DOWN (DOWN ARROW key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_BUTTON_SELECT, 0x29); //to VK_SELECT (SELECT key)
				
		// Populate numbers
		int j=0;
		for (int i=0x30; i <= 0x3A; i++, j++)
			androidToWindowsEventMap.put(KeyEvent.KEYCODE_0 + j, i);
	
		// Populate letters
		j=0;
		for (int i=0x41; i <= 0x5A; i++, j++)
			androidToWindowsEventMap.put(KeyEvent.KEYCODE_A + j, i);
		
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_STAR, 0x6A); //to VK_MULTIPLY (Multiply key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_PLUS, 0x6B); //to VK_ADD (Add key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_MINUS, 0x6D); //to VK_SUBTRACT (Subtract key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_SHIFT_LEFT, 0x10); //to VK_LSHIFT (SHIFT key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_BACK, 0xA6); //to VK_BROWSER_SEARCH (Browser search key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_SEARCH, 0xAA); //to VK_BROWSER_SEARCH (Browser search key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_VOLUME_DOWN, 0xAE); //to VK_VOLUME_DOWN (Volume Down key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_VOLUME_UP, 0xAF); //to VK_VOLUME_UP (Volume Up key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_SEMICOLON, 0xBA); //to VK_OEM_1 (For the US standard keyboard, the ';:' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_COMMA, 0xBC); //to VK_OEM_COMMA (For any country/region, the ',' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_PERIOD, 0xBE); //to VK_OEM_PERIOD (For any country/region, the '.' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_SLASH, 0xBF); //to VK_OEM_2 (For the US standard keyboard, the '/?' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_LEFT_BRACKET, 0xDB); //to VK_OEM_4 (For the US standard keyboard, the '[{' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_BACKSLASH, 0xDC); //to VK_OEM_5 (For the US standard keyboard, the '\|' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_RIGHT_BRACKET, 0xDD); //to VK_OEM_6 (For the US standard keyboard, the ']}' key)
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_APOSTROPHE, 0xDE); //to VK_OEM_7 (For the US standard keyboard, the 'single-quote/double-quote' key)
		
		//Characters which can't be mapped
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_GRAVE, KeyEvent.KEYCODE_GRAVE + 0xFE);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_AT, KeyEvent.KEYCODE_AT + 0xFE);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_POUND, KeyEvent.KEYCODE_POUND + 0xFE);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_ALT_LEFT, KeyEvent.KEYCODE_ALT_LEFT + 0xFE);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_EQUALS, KeyEvent.KEYCODE_EQUALS + 0xFE);
	}
	
	private static void initWindows2AndroidMap()
	{
		windowsToAndroidEventMap.put(0x08, KeyEvent.KEYCODE_DEL);
		windowsToAndroidEventMap.put(0x09, KeyEvent.KEYCODE_TAB);
		windowsToAndroidEventMap.put(0x0C, KeyEvent.KEYCODE_CLEAR);
		windowsToAndroidEventMap.put(0x0D, KeyEvent.KEYCODE_ENTER);
		windowsToAndroidEventMap.put(0x12, KeyEvent.KEYCODE_MENU);
		windowsToAndroidEventMap.put(0x13, KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE);
		windowsToAndroidEventMap.put(0x20, KeyEvent.KEYCODE_SPACE);
		windowsToAndroidEventMap.put(0x21, KeyEvent.KEYCODE_PAGE_UP);
		windowsToAndroidEventMap.put(0x22, KeyEvent.KEYCODE_PAGE_DOWN);
		windowsToAndroidEventMap.put(0x25, KeyEvent.KEYCODE_DPAD_LEFT);
		windowsToAndroidEventMap.put(0x26, KeyEvent.KEYCODE_DPAD_UP);
		windowsToAndroidEventMap.put(0x27, KeyEvent.KEYCODE_DPAD_RIGHT);
		windowsToAndroidEventMap.put(0x28, KeyEvent.KEYCODE_DPAD_DOWN);
		windowsToAndroidEventMap.put(0x29, KeyEvent.KEYCODE_BUTTON_SELECT);
		
		int j=0;
		for (int i=0x30; i <= 0x3A; i++, j++)
			windowsToAndroidEventMap.put(i, KeyEvent.KEYCODE_0 + j);
		
		j=0;
		for (int i=0x41; i <= 0x5A; i++, j++)
			windowsToAndroidEventMap.put(i, KeyEvent.KEYCODE_A + j);
		
		windowsToAndroidEventMap.put(0x6A, KeyEvent.KEYCODE_STAR);
		windowsToAndroidEventMap.put(0x6B, KeyEvent.KEYCODE_PLUS);
		windowsToAndroidEventMap.put(0x6D, KeyEvent.KEYCODE_MINUS);
		windowsToAndroidEventMap.put(0x10, KeyEvent.KEYCODE_SHIFT_LEFT);
		windowsToAndroidEventMap.put(0xA6, KeyEvent.KEYCODE_BACK);
		windowsToAndroidEventMap.put(0xAA, KeyEvent.KEYCODE_SEARCH);
		windowsToAndroidEventMap.put(0xAE, KeyEvent.KEYCODE_VOLUME_DOWN);
		windowsToAndroidEventMap.put(0xAF, KeyEvent.KEYCODE_VOLUME_UP);
		windowsToAndroidEventMap.put(0xBA, KeyEvent.KEYCODE_SEMICOLON);
		windowsToAndroidEventMap.put(0xBC, KeyEvent.KEYCODE_COMMA);
		windowsToAndroidEventMap.put(0xBE, KeyEvent.KEYCODE_PERIOD);
		windowsToAndroidEventMap.put(0xBF, KeyEvent.KEYCODE_SLASH);
		windowsToAndroidEventMap.put(0xDB, KeyEvent.KEYCODE_LEFT_BRACKET);
		windowsToAndroidEventMap.put(0xDC, KeyEvent.KEYCODE_BACKSLASH);
		windowsToAndroidEventMap.put(0xDD, KeyEvent.KEYCODE_RIGHT_BRACKET);
		windowsToAndroidEventMap.put(0xDE, KeyEvent.KEYCODE_APOSTROPHE);
		
		//Characters which can't be mapped
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_GRAVE + 0xFE, KeyEvent.KEYCODE_GRAVE);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_AT + 0xFE, KeyEvent.KEYCODE_AT);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_POUND + 0xFE, KeyEvent.KEYCODE_POUND);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_ALT_LEFT + 0xFE, KeyEvent.KEYCODE_ALT_LEFT);
		androidToWindowsEventMap.put(KeyEvent.KEYCODE_EQUALS + 0xFE, KeyEvent.KEYCODE_EQUALS);
	}
}

