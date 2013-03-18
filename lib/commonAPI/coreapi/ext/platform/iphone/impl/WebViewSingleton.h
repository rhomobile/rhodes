
#import "IWebView.h"
#import "WebViewSingletonBase.h"

@interface WebViewSingleton : WebViewSingletonBase<IWebViewSingleton> {
}



-(void) getFramework:(id<IMethodResult>)methodResult;
-(void) getFullScreen:(id<IMethodResult>)methodResult;
-(void) setFullScreen:(BOOL)value;
-(void) getNativeMenu:(id<IMethodResult>)methodResult;
-(void) setNativeMenu:(NSDictionary*)value;
-(void) getEnableZoom:(id<IMethodResult>)methodResult;
-(void) setEnableZoom:(BOOL)value;
-(void) getEnablePageLoadingIndication:(id<IMethodResult>)methodResult;
-(void) setEnablePageLoadingIndication:(BOOL)value;
-(void) getEnableWebPlugins:(id<IMethodResult>)methodResult;
-(void) setEnableWebPlugins:(BOOL)value;
-(void) getNavigationTimeout:(id<IMethodResult>)methodResult;
-(void) setNavigationTimeout:(int)value;
-(void) getScrollTechnique:(id<IMethodResult>)methodResult;
-(void) setScrollTechnique:(NSString*)value;
-(void) getFontFamily:(id<IMethodResult>)methodResult;
-(void) setFontFamily:(NSString*)value;
-(void) getUserAgent:(id<IMethodResult>)methodResult;
-(void) setUserAgent:(NSString*)value;
-(void) getViewportEnabled:(id<IMethodResult>)methodResult;
-(void) setViewportEnabled:(BOOL)value;
-(void) getViewportWidth:(id<IMethodResult>)methodResult;
-(void) setViewportWidth:(int)value;
-(void) getCacheSize:(id<IMethodResult>)methodResult;
-(void) setCacheSize:(int)value;
-(void) getEnableCache:(id<IMethodResult>)methodResult;
-(void) setEnableCache:(BOOL)value;
-(void) getAcceptLanguage:(id<IMethodResult>)methodResult;
-(void) setAcceptLanguage:(NSString*)value;
-(void) getZoomPage:(id<IMethodResult>)methodResult;
-(void) setZoomPage:(float)value;
-(void) getTextZoomLevel:(id<IMethodResult>)methodResult;
-(void) setTextZoomLevel:(int)value;
-(void) getActiveTab:(id<IMethodResult>)methodResult;
-(void) refresh:(int)tabIndex;
-(void) navigate:(NSString*)url tabIndex:(int)tabIndex;
-(void) navigateBack:(int)tabIndex;
-(void) currentLocation:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) currentURL:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) executeJavascript:(NSString*)javascriptText tabIndex:(int)tabIndex;
-(void) active_tab:(id<IMethodResult>)methodResult;
-(void) full_screen_mode:(BOOL)enable;
-(void) setCookie:(NSString*)url cookie:(NSString*)cookie;
-(void) save:(NSString*)format path:(NSString*)path tabIndex:(int)tabIndex;
-(void) set_menu_items:(NSDictionary*)menuItems;




@end