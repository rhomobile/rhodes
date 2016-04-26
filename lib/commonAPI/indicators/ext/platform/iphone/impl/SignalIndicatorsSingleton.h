
#import "ISignalIndicators.h"
#import "SignalIndicatorsSingletonBase.h"

@interface SignalIndicatorsSingleton : SignalIndicatorsSingletonBase<ISignalIndicatorsSingleton> {
    int iRefreshInterval;
    NSTimer* timer;
    id<IMethodResult> callbackHolder;
    
    NSString *adapter;
    NSString *ssid;
    NSString *macAddress;
    NSString *adapterName;
    NSString *dhcpServer;
    NSString *dhcpStatic;
    NSString *gateway;
    NSString *ipAddress;
    NSString *rssi;
    NSString *subnetMask;
    NSString *wins;
}

@property(nonatomic, copy)    NSString *adapter;
@property(nonatomic, copy)    NSString *ssid;
@property(nonatomic, copy)    NSString *macAddress;
@property(nonatomic, copy)    NSString *adapterName;
@property(nonatomic, copy)    NSString *dhcpServer;
@property(nonatomic, copy)    NSString *dhcpStatic;
@property(nonatomic, copy)    NSString *gateway;
@property(nonatomic, copy)    NSString *ipAddress;
@property(nonatomic, copy)    NSString *rssi;
@property(nonatomic, copy)    NSString *subnetMask;
@property(nonatomic, copy)    NSString *wins;

-(id) init;

-(void) getRefreshInterval:(id<IMethodResult>)methodResult;
-(void) setRefreshInterval:(int)refreshInterval methodResult:(id<IMethodResult>)methodResult ;
-(void) wlanStatus:(id<IMethodResult>)methodResult;
-(void) stopWlanStatus:(id<IMethodResult>)methodResult ;
-(void) showIcon:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult ;
-(void) hideIcon:(id<IMethodResult>)methodResult ;

@end