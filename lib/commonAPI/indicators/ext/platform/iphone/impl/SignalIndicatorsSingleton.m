
#import "SignalIndicatorsSingleton.h"

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include "route.h"
#include <netinet/in.h>
#include <sys/sysctl.h>
#include <string.h>

#import <SystemConfiguration/CaptiveNetwork.h>

@implementation SignalIndicatorsSingleton

@synthesize adapter, ssid, macAddress, adapterName, dhcpServer, dhcpStatic, gateway, ipAddress, rssi, subnetMask, wins;

-(id)init {
    self = [super init];
    
    if (self)
    {
        [self setAdapter:@"en0"];
        iRefreshInterval = 5000;
        timer = nil;
        callbackHolder = nil;
    }
    
    return self;
}

-(void)dealloc
{
    if (timer)
    {
        [self stopWlanStatus];
    }
    
    self.adapter = nil;
    self.ssid = nil;
    self.macAddress = nil;
    self.adapterName = nil;
    self.dhcpServer = nil;
    self.dhcpStatic = nil;
    self.gateway = nil;
    self.ipAddress = nil;
    self.rssi = nil;
    self.subnetMask = nil;
    self.wins = nil;
    
    [super dealloc];
}

-(void) getRefreshInterval:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:iRefreshInterval]];
}

-(void) setRefreshInterval:(int)refreshInterval methodResult:(id<IMethodResult>)methodResult {
    iRefreshInterval = refreshInterval;
}

-(void) readWlanStatus:(id<IMethodResult>)methodResult {
    NSMutableDictionary* info = [NSMutableDictionary new];
    
    [info setObject:adapter forKey:HK_ADAPTER_NAME];
    [info setObject:[NSNumber numberWithInt:-1] forKey:HK_SIGNAL_STRENGTH];
    [self updateSSID];
    [info setObject:ssid forKey:HK_ESSID];
    [self updateMacAddress];
    [info setObject:macAddress forKey:HK_MAC_ADDRESS];
    [self updateIpAddress];
    [info setObject:ipAddress forKey:HK_IP_ADDRESS];
    [info setObject:subnetMask forKey:HK_SUBNET_MASK];
    [info setObject:gateway forKey:HK_GATEWAY];
    
    [methodResult setResult:info];
}

-(void) updateWlanStatus:(id<IMethodResult>)methodResult {
    [self readWlanStatus:callbackHolder];
}

- (void)setupTimer:(NSNumber*)delay methodResult:(id<IMethodResult>)methodResult {
	if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    
    int iDelay = [delay intValue];
    
    if (iDelay > 0)
    {
        if (iDelay < 100)
        {
            iDelay = 100;
        }
        timer = [NSTimer scheduledTimerWithTimeInterval:(iDelay/1000)
                                                 target:self
                                               selector:@selector(updateWlanStatus:)
                                               userInfo:nil
                                                repeats:YES];
    }
}

-(void) wlanStatus:(id<IMethodResult>)methodResult {       
    [self readWlanStatus:methodResult];
    
    int updateInterval = iRefreshInterval;

    
    if ([methodResult hasCallback] && updateInterval > 0)
    {
        callbackHolder = methodResult;
        
        [self performSelectorOnMainThread:@selector(setupTimer:methodResult:) withObject:[NSNumber numberWithInt:updateInterval] waitUntilDone:NO];
    }
}

-(void) stopWlanStatus:(id<IMethodResult>)methodResult {
    [self performSelectorOnMainThread:@selector(setupTimer:methodResult:) withObject:[NSNumber numberWithInt:0] waitUntilDone:NO];
    callbackHolder = nil;
}

-(void) showIcon:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    // Not implemented on iOS
}

-(void) hideIcon:(id<IMethodResult>)methodResult {
    // Not implemented on iOS
}

- (void)updateSSID {
    NSString* __ssid = @"<<NONE>>";
    CFArrayRef adapters = CNCopySupportedInterfaces();
    if (adapters != nil){
        NSDictionary* netinfo = (NSDictionary *) CNCopyCurrentNetworkInfo(CFArrayGetValueAtIndex(adapters, 0));
        if (netinfo!=nil){
            __ssid = [netinfo valueForKey:@"SSID"];
        }
    }
    [self setSsid:__ssid];
}

-(void) updateMacAddress {
    int                 mgmtInfoBase[6];
    char                *msgBuffer = NULL;
    size_t              length;
    unsigned char       binaryMacAddress[6];
    struct if_msghdr    *interfaceMsgStruct;
    struct sockaddr_dl  *socketStruct;
    NSString            *errorFlag = NULL;
    
    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces
    
    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex([[self adapter] UTF8String])) == 0)
        errorFlag = @"if_nametoindex failure";
    else
    {
        // Get the size of the data available (store in len)
        if (sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0)
            errorFlag = @"sysctl mgmtInfoBase failure";
        else
        {
            // Alloc memory based on above call
            if ((msgBuffer = malloc(length)) == NULL)
                errorFlag = @"buffer allocation failure";
            else
            {
                // Get system information, store in buffer
                if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0)
                    errorFlag = @"sysctl msgBuffer failure";
            }
        }
    }
    // Befor going any further...
    if (errorFlag != NULL)
    {
        NSLog(@"Error: %@", errorFlag);
        [self setMacAddress:@"unknown"];
        return;
    }
    // Map msgbuffer to interface message structure
    interfaceMsgStruct = (struct if_msghdr *) msgBuffer;
    // Map to link-level socket structure
    socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);
    // Copy link layer address data in socket structure to an array
    memcpy(&binaryMacAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
    // Release the buffer memory
    free(msgBuffer);
    
    // Read from char array into a string object, into traditional Mac address format
    NSString *macAddressString = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
                                  binaryMacAddress[0], binaryMacAddress[1], binaryMacAddress[2],
                                  binaryMacAddress[3], binaryMacAddress[4], binaryMacAddress[5]];
    
    [self setMacAddress:macAddressString];
}

#define ROUNDUP(a) \
((a) > 0 ? (1 + (((a) - 1) | (sizeof(long) - 1))) : sizeof(long))

-(void) updateIpAddress {
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    NSString *__ip = @"0.0.0.0";
    NSString *__netmask = @"0.0.0.0";
    NSString *__gateway = @"0.0.0.0";
    
    // retrieve the current interfaces - returns 0 on success
    if (!getifaddrs(&interfaces)) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
            if (sa_type == AF_INET || sa_type == AF_INET6) {
                NSString *name = [NSString stringWithUTF8String:temp_addr->ifa_name];
                
                if ([name isEqualToString:[self adapter]]) {
                    __ip = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
                    __netmask = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_netmask)->sin_addr)];
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
        // Free memory
        freeifaddrs(interfaces);
    }
    [self setIpAddress: __ip];
    [self setSubnetMask: __netmask];
    
    // find out gatewayIp
    int mib[] = {CTL_NET, PF_ROUTE, 0, AF_INET,
        NET_RT_FLAGS, RTF_GATEWAY};
    size_t l;
    char * buf, * p;
    struct rt_msghdr * rt;
    struct sockaddr * sa;
    struct sockaddr * sa_tab[RTAX_MAX];
    int i;
    
    if (sysctl(mib, sizeof(mib)/sizeof(int), 0, &l, 0, 0) >= 0) {
        if (l>0) {
            buf = malloc(l);
            if (sysctl(mib, sizeof(mib)/sizeof(int), buf, &l, 0, 0) >= 0) {
                for (p=buf; p<buf+l; p+=rt->rtm_msglen) {
                    rt = (struct rt_msghdr *)p;
                    sa = (struct sockaddr *)(rt + 1);
                    for (i=0; i<RTAX_MAX; i++) {
                        if (rt->rtm_addrs & (1 << i)) {
                            sa_tab[i] = sa;
                            sa = (struct sockaddr *)((char *)sa + ROUNDUP(sa->sa_len));
                        } else {
                            sa_tab[i] = NULL;
                        }
                    }
                    
                    if ( ((rt->rtm_addrs & (RTA_DST|RTA_GATEWAY)) == (RTA_DST|RTA_GATEWAY))
                        && sa_tab[RTAX_DST]->sa_family == AF_INET
                        && sa_tab[RTAX_GATEWAY]->sa_family == AF_INET) {
                        
                        if (((struct sockaddr_in *)sa_tab[RTAX_DST])->sin_addr.s_addr == 0) {
                            char ifName[128];
                            if_indextoname(rt->rtm_index,ifName);
                            
                            if (strcmp([adapter UTF8String],ifName)==0) {
                                __gateway = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)(sa_tab[RTAX_GATEWAY]))->sin_addr)];
                            }
                        }
                    }
                }
            }
            free(buf);
        }
    }
    [self setGateway:__gateway];
}

@end
