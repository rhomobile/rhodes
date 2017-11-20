
#import "WebViewSingleton.h"


extern int rho_sysimpl_get_property_iphone(char* szPropName, NSObject** resValue);
extern void rho_webview_refresh(int tab_index);
extern void rho_webview_navigate(const char* url, int tab_index);
extern char* rho_webview_current_location(int tab_index);
extern char* rho_webview_execute_js(const char* js, int tab_index);
extern int rho_webview_active_tab();
extern void rho_webview_full_screen_mode(int enable);
extern BOOL rho_webview_get_full_screen_mode();
extern void rho_webview_set_cookie(const char* url, const char* cookie);
extern void rho_webview_get_cookies( NSString* url, NSDictionary** retVal );
extern BOOL rho_webview_remove_cookie( NSString* url, NSString* name );
extern BOOL rho_webview_remove_all_cookies();
extern void rho_webview_navigate_back();
extern void rho_webview_navigate_back_with_tab(int tabIndex);
extern void rho_webview_save(const char* format, const char* path, int tab_index);
extern NSString* rho_webview_get_current_url(int tab_index);
extern BOOL rho_webview_get_KeyboardDisplayRequiresUserAction();
extern void rho_webview_set_KeyboardDisplayRequiresUserAction(BOOL value);
extern BOOL rho_webview_get_EnableDragAndDrop();
extern void rho_webview_set_EnableDragAndDrop(BOOL value);


@implementation WebViewSingleton



-(void) getFramework:(id<IMethodResult>)methodResult {
    NSObject* result = nil;
    if (rho_sysimpl_get_property_iphone("webview_framework", &result)) {
        [methodResult setResult:((NSString*)result)];
    }
}

-(void) getFullScreen:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:rho_webview_get_full_screen_mode()]];
}

-(void) setFullScreen:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    rho_webview_full_screen_mode(value?1:0);
}

-(void) getNativeMenu:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setNativeMenu:(NSDictionary*)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getEnableZoom:(id<IMethodResult>)methodResult {
    //TODO?
}

-(void) setEnableZoom:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    //TODO?
}

-(void) getEnablePageLoadingIndication:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setEnablePageLoadingIndication:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getEnableWebPlugins:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setEnableWebPlugins:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getKeyboardDisplayRequiresUserAction:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:rho_webview_get_KeyboardDisplayRequiresUserAction()]];
}

-(void) setKeyboardDisplayRequiresUserAction:(BOOL)keyboardDisplayRequiresUserAction methodResult:(id<IMethodResult>)methodResult {
    rho_webview_set_KeyboardDisplayRequiresUserAction(keyboardDisplayRequiresUserAction);
}


-(void) getNavigationTimeout:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setNavigationTimeout:(int)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getScrollTechnique:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setScrollTechnique:(NSString*)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getFontFamily:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setFontFamily:(NSString*)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getUserAgent:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setUserAgent:(NSString*)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getViewportEnabled:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setViewportEnabled:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getViewportWidth:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setViewportWidth:(int)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getCacheSize:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setCacheSize:(int)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getEnableCache:(id<IMethodResult>)methodResult {
    //TODO?
}

-(void) setEnableCache:(BOOL)value methodResult:(id<IMethodResult>)methodResult{
    //TODO?
}

-(void) getAcceptLanguage:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setAcceptLanguage:(NSString*)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getZoomPage:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setZoomPage:(float)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getTextZoomLevel:(id<IMethodResult>)methodResult {
    //unsupported on iOS
}

-(void) setTextZoomLevel:(int)value methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

-(void) getActiveTab:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:rho_webview_active_tab()]];
}

-(void) refresh:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    rho_webview_refresh(tabIndex);
}

-(void) navigate:(NSString*)url tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    rho_webview_navigate([url UTF8String], tabIndex);
}

-(void) navigateBack:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    rho_webview_navigate_back_with_tab(tabIndex);
}

-(void) currentLocation:(int)tabIndex methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSString stringWithUTF8String:rho_webview_current_location(tabIndex)]];
}

-(void) currentURL:(int)tabIndex methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:rho_webview_get_current_url(tabIndex)];
}

-(void) executeJavascript:(NSString*)javascriptText tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    rho_webview_execute_js([javascriptText UTF8String], tabIndex);
}

-(void) active_tab:(id<IMethodResult>)methodResult {
    [self getActiveTab:methodResult];
}

-(void) full_screen_mode:(BOOL)enable methodResult:(id<IMethodResult>)methodResult{
    [self setFullScreen:enable methodResult:methodResult];
}

-(void) setCookie:(NSString*)url cookie:(NSString*)cookie methodResult:(id<IMethodResult>)methodResult{
    rho_webview_set_cookie([url UTF8String], [cookie UTF8String]);
}

-(void) getCookies:(NSString*)url methodResult:(id<IMethodResult>)methodResult {
    dispatch_async(dispatch_get_main_queue(), ^(void){
        NSDictionary* cookies = nil;
        rho_webview_get_cookies(url, &cookies);
        [methodResult setResult:cookies];
    });
}

-(void) removeAllCookies:(id<IMethodResult>)methodResult {
  dispatch_async(dispatch_get_main_queue(), ^(void){
    BOOL b = rho_webview_remove_all_cookies();
    NSNumber* bObj = [NSNumber numberWithBool:b];
    [methodResult setResult:bObj];
  });
}

-(void) removeCookie:(NSString*)url name:(NSString*)name methodResult:(id<IMethodResult>)methodResult {
    dispatch_async(dispatch_get_main_queue(), ^(void){
        BOOL removed = rho_webview_remove_cookie(url,name);
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:1];
        if ( removed ) {
            [dict setObject:name forKey:@"removed_cookie"];
        }
        [methodResult setResult:dict];
    });
}

-(void) save:(NSString*)format path:(NSString*)path tabIndex:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    rho_webview_save([format UTF8String], [path UTF8String], tabIndex);
}

-(void) set_menu_items:(NSDictionary*)menuItems methodResult:(id<IMethodResult>)methodResult{
    //unsupported on iOS
}

/* getter for "enableDragAndDrop" property */
-(void) getEnableDragAndDrop:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:rho_webview_get_EnableDragAndDrop()]];
}

/* setter for "enableDragAndDrop" property */
-(void) setEnableDragAndDrop:(BOOL)enableDragAndDrop methodResult:(id<IMethodResult>)methodResult {
    rho_webview_set_EnableDragAndDrop(enableDragAndDrop);
}




@end
