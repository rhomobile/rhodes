package com.rho.rubyext;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import rhomobile.RhodesApplication;

public class WebView
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("WebView");

	public static void navigate(String url)
	{
		RhodesApplication.getInstance().addToHistory(url, null);
		RhodesApplication.getInstance().navigateUrl(url);
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("refresh", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					RhodesApplication.getInstance().refreshCurrentPage();
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("refresh failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});		
		klass.getSingletonClass().defineMethod("navigate", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				try {
					if ( args.get(0) == RubyConstant.QNIL )
						LOG.ERROR("WebView.navigate failed: url is nil");
					else
					{
						String url = args.get(0).toString();
						navigate(url);
					}
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("navigate failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("current_location", new RubyNoOrOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					String url = RhodesApp.getInstance().getCurrentUrl(0); 
						//RhodesApplication.getInstance().getCurrentPageUrl();
					return ObjectFactory.createString(url);
				} catch(Exception e) {
					LOG.ERROR("current_location failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) 
			{
				try {
					int nIndex = arg.toInt();
					String url = RhodesApp.getInstance().getCurrentUrl(nIndex); 
						//RhodesApplication.getInstance().getCurrentPageUrl();
					return ObjectFactory.createString(url);
				} catch(Exception e) {
					LOG.ERROR("current_location failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
			
		});		
		klass.getSingletonClass().defineMethod("set_menu_items", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					RhodesApplication.getInstance().resetMenuItems();
					RubyHash items = (RubyHash)arg0;
					RubyArray keys = items.keys();
					RubyArray values = items.values();
					for( int i = 0; i < keys.size(); i++ ){
						String label = keys.get(i).toString();
						String value = values.get(i).toString();
						RhodesApplication.getInstance().addMenuItem(label, value);
					}
					return RubyConstant.QTRUE;
				} catch(Exception e) {
					LOG.ERROR("set_menu_items failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.getSingletonClass().defineMethod("execute_js", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) 
			{
				try {
					RhodesApplication.getInstance().executeJavascript(args.get(0).toStr());
					//String url = "javascript:"+args.get(0).toStr();
					//RhodesApplication.getInstance().navigateUrl(url);
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("execute_js failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("active_tab", new RubyNoArgMethod() {
		 	protected RubyValue run(RubyValue receiver, RubyBlock block) {
		 		return ObjectFactory.createInteger(0);
		 	}
		});
		klass.getSingletonClass().defineMethod("set_cookie", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
				try {
					String url = arg1.toString();
					String cookie = arg2.toString();
					RhodesApplication.getInstance().setCookie(url, cookie);
					return RubyConstant.QNIL;
				}
				catch (Exception e) {
					LOG.ERROR("set_cookie failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	}
	
}
