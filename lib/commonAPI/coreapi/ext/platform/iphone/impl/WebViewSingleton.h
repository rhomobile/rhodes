
#import "IWebView.h"
#import "WebViewSingletonBase.h"

@interface WebViewSingleton : WebViewSingletonBase<IWebViewSingleton> {
}



-(void) getFramework:(id<IMethodResult>)methodResult;
-(void) getFullScreen:(id<IMethodResult>)methodResult;
-(void) setFullScreen:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getNativeMenu:(id<IMethodResult>)methodResult;
-(void) setNativeMenu:(NSDictionary*)value methodResult:(id<IMethodResult>)methodResult;
-(void) getEnableZoom:(id<IMethodResult>)methodResult;
-(void) setEnableZoom:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getEnablePageLoadingIndication:(id<IMethodResult>)methodResult;
-(void) setEnablePageLoadingIndication:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getEnableWebPlugins:(id<IMethodResult>)methodResult;
-(void) setEnableWebPlugins:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getKeyboardDisplayRequiresUserAction:(id<IMethodResult>)methodResult;
-(void) setKeyboardDisplayRequiresUserAction:(BOOL)keyboardDisplayRequiresUserAction methodResult:(id<IMethodResult>)methodResult;
-(void) getNavigationTimeout:(id<IMethodResult>)methodResult;
-(void) setNavigationTimeout:(int)value methodResult:(id<IMethodResult>)methodResult;
-(void) getScrollTechnique:(id<IMethodResult>)methodResult;
-(void) setScrollTechnique:(NSString*)value methodResult:(id<IMethodResult>)methodResult;
-(void) getFontFamily:(id<IMethodResult>)methodResult;
-(void) setFontFamily:(NSString*)value methodResult:(id<IMethodResult>)methodResult;
-(void) getUserAgent:(id<IMethodResult>)methodResult;
-(void) setUserAgent:(NSString*)value methodResult:(id<IMethodResult>)methodResult;
-(void) getViewportEnabled:(id<IMethodResult>)methodResult;
-(void) setViewportEnabled:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getViewportWidth:(id<IMethodResult>)methodResult;
-(void) setViewportWidth:(int)value methodResult:(id<IMethodResult>)methodResult;
-(void) getCacheSize:(id<IMethodResult>)methodResult;
-(void) setCacheSize:(int)value methodResult:(id<IMethodResult>)methodResult;
-(void) getEnableCache:(id<IMethodResult>)methodResult;
-(void) setEnableCache:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) getEnableDragAndDrop:(id<IMethodResult>)methodResult;
-(void) setEnableDragAndDrop:(BOOL)enableDragAndDrop methodResult:(id<IMethodResult>)methodResult;
-(void) getAcceptLanguage:(id<IMethodResult>)methodResult;
-(void) setAcceptLanguage:(NSString*)value methodResult:(id<IMethodResult>)methodResult;
-(void) getZoomPage:(id<IMethodResult>)methodResult;
-(void) setZoomPage:(float)value methodResult:(id<IMethodResult>)methodResult;
-(void) getTextZoomLevel:(id<IMethodResult>)methodResult;
-(void) setTextZoomLevel:(int)value methodResult:(id<IMethodResult>)methodResult;
-(void) getActiveTab:(id<IMethodResult>)methodResult;
-(void) refresh:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) navigate:(NSString*)url tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) navigateBack:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) currentLocation:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) currentURL:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) executeJavascript:(NSString*)javascriptText tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) active_tab:(id<IMethodResult>)methodResult;
-(void) full_screen_mode:(BOOL)enable methodResult:(id<IMethodResult>)methodResult;
-(void) setCookie:(NSString*)url cookie:(NSString*)cookie methodResult:(id<IMethodResult>)methodResult;
-(void) getCookies:(NSString*)url methodResult:(id<IMethodResult>)methodResult;
-(void) removeCookie:(NSString*)url name:(NSString*)name methodResult:(id<IMethodResult>)methodResult;
-(void) removeAllCookies:(id<IMethodResult>)methodResult;
-(void) save:(NSString*)format path:(NSString*)path tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) set_menu_items:(NSDictionary*)menuItems methodResult:(id<IMethodResult>)methodResult;




@end
