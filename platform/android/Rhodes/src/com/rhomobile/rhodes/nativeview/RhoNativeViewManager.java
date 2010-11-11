package com.rhomobile.rhodes.nativeview;


import android.view.View;
import android.webkit.WebView;
import com.rhomobile.rhodes.RhodesService;

public class RhoNativeViewManager {

	public interface RhoNativeView {
		View getView();
		void navigate(String url);
		void destroyView();
		String getViewType();
	}
	
	private static class RhoNativeViewImpl implements RhoNativeView {
		
		RhoNativeViewImpl(String viewType, long factory_h, long view_h) {
			mFactoryHandle = factory_h;
			mViewHandle = view_h;
		}
		
		public View getView() {
			return getViewByHandle(mViewHandle);
		}
		
		public void navigate(String url) {
			navigateByHandle(mViewHandle, url);
		}
		public void destroyView() {
			destroyByHandle(mFactoryHandle, mViewHandle);
		}
		public String getViewType() {
			return mViewType;
		}
		private String mViewType;
		private long mFactoryHandle;
		private long mViewHandle;
	}
	
	public static WebView getWebViewObject(int tab_index) {
		return RhodesService.getInstance().getMainView().getWebView(tab_index);
	}

	public static RhoNativeView getNativeViewByteType(String typename) {
		long factory_h = getFactoryHandleByViewType(typename);
		if (factory_h == 0) {
			return null;
		}
		long view_h = getViewHandleByFactoryHandle(factory_h);
		if (view_h == 0) {
			return null;
		}
		RhoNativeViewImpl nv = new RhoNativeViewImpl(typename, factory_h, view_h);
		return nv;
	}
	
	private native static View getViewByHandle(long handle);
	private native static void navigateByHandle(long handle, String url);
	private native static long getFactoryHandleByViewType(String viewtype);
	private native static long getViewHandleByFactoryHandle(long factory_h);
	private native static void destroyByHandle(long factory_h, long view_h);
	
	
}