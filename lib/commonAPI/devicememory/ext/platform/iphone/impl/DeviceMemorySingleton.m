
#import <UIKit/UIKit.h>
#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

#import "DeviceMemorySingleton.h"


long long calc_free_memory_improved() {
    int mib[6];
    mib[0] = CTL_HW;
    mib[1] = HW_PAGESIZE;
    
    int pagesize;
    size_t length;
    length = sizeof (pagesize);
    if (sysctl (mib, 2, &pagesize, &length, NULL, 0) < 0)
    {
        NSLog(@"ERROR with getting page size");
    }
    
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    
    vm_statistics_data_t vmstat;
    if (host_statistics (mach_host_self (), HOST_VM_INFO, (host_info_t) &vmstat, &count) != KERN_SUCCESS)
    {
        NSLog(@"Failed to get VM statistics.");
    }
    
    //double total = vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
    //double wired = vmstat.wire_count / total;
    //double active = vmstat.active_count / total;
    //double inactive = vmstat.inactive_count / total;
    //double free = vmstat.free_count / total;
    
    task_basic_info_64_data_t info;
    unsigned size = sizeof (info);
    task_info (mach_task_self (), TASK_BASIC_INFO_64, (task_info_t) &info, &size);
    
    double unit = 1024 * 1024;
    NSLog([NSString stringWithFormat: @"% 3.1f MB\n% 3.1f MB\n% 3.1f MB", vmstat.free_count * pagesize / unit, (vmstat.free_count + vmstat.inactive_count) * pagesize / unit, info.resident_size / unit]);
    
    return vmstat.free_count * pagesize;
}


NSArray* get_all_processes() {
    
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
    size_t miblen = 4;
    
    size_t size;
    int st = sysctl(mib, miblen, NULL, &size, NULL, 0);
    
    struct kinfo_proc * process = NULL;
    struct kinfo_proc * newprocess = NULL;
    
    do {
        
        size += size / 10;
        newprocess = realloc(process, size);
        
        if (!newprocess){
            
            if (process){
                free(process);
            }
            
            return nil;
        }
        
        process = newprocess;
        st = sysctl(mib, miblen, process, &size, NULL, 0);
        
    } while (st == -1 && errno == ENOMEM);
    
    if (st == 0){
        
        if (size % sizeof(struct kinfo_proc) == 0){
            int nprocess = size / sizeof(struct kinfo_proc);
            
            if (nprocess){
                
                NSMutableArray * array = [[NSMutableArray alloc] init];
                
                for (int i = nprocess - 1; i >= 0; i--){
                    
                    NSString * processID = [[NSString alloc] initWithFormat:@"%d", process[i].kp_proc.p_pid];
                    NSString * processName = [[NSString alloc] initWithFormat:@"%s", process[i].kp_proc.p_comm];
                    
                    NSDictionary * dict = [[NSDictionary alloc] initWithObjects:[NSArray arrayWithObjects:processID, processName, nil]
                                                                        forKeys:[NSArray arrayWithObjects:@"ProcessID", @"ProcessName", nil]];
                    [processID release];
                    [processName release];
                    [array addObject:dict];
                    [dict release];
                }
                
                free(process);
                return [array autorelease];
            }
        }
    }
    
    return nil;
}


static long long get_free_memory(void)
{
    
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);
    
    vm_statistics_data_t vm_stat;
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
        NSLog(@"Failed to fetch vm statistics");
        return 0;
    }
    
    natural_t mem_used = (vm_stat.active_count +
                          vm_stat.inactive_count +
                          vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
    natural_t mem_total = mem_used + mem_free;
    NSLog(@"RAM status :   used: %u free: %u total: %u", mem_used, mem_free, mem_total);
    
    return mem_free;
}

static long long get_free_diskspace(void)
{
    uint64_t totalSpace = 0;
    uint64_t totalFreeSpace = 0;
    
    __autoreleasing NSError *error = nil;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSDictionary *dictionary = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[paths lastObject] error: &error];
    
    if (dictionary) {
        NSNumber *fileSystemSizeInBytes = [dictionary objectForKey: NSFileSystemSize];
        NSNumber *freeFileSystemSizeInBytes = [dictionary objectForKey:NSFileSystemFreeSize];
        totalSpace = [fileSystemSizeInBytes unsignedLongLongValue];
        totalFreeSpace = [freeFileSystemSizeInBytes unsignedLongLongValue];
        NSLog(@"Storage Capacity of %llu MiB with %llu MiB Free memory available.", ((totalSpace/1024ll)/1024ll), ((totalFreeSpace/1024ll)/1024ll));
    } else {
        NSLog(@"Error Obtaining System Memory Info: Domain = %@, Code = %d", [error domain], [error code]);
    }
    
    return totalFreeSpace;
}


@implementation DeviceMemorySingleton

@synthesize callback;

- (id) init {
    self = [super init];
    callback = nil;
    return self;
}

- (void) handleMemoryWarning:(NSNotification *)notification
{
    if (callback == nil) {
        return;
    }
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:2];
    [dict setObject:DEVICE_MEMORY_STATUS_CRITICAL forKey:HK_STATUS];
    [callback setResult:dict];
    
}


-(void) startListeningPlatformMemoryWarning:(id<IMethodResult>)methodResult {
    self.callback = methodResult;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleMemoryWarning:) name: UIApplicationDidReceiveMemoryWarningNotification object:nil];
}

-(void) stopListeningPlatformMemoryWarning:(id<IMethodResult>)methodResult {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    self.callback = nil;
}

-(void) getAvailableMemory:(id<IMethodResult>)methodResult {
    long long freemem = get_free_memory() >> 10;
    //NSString* str_free_mem = [[NSNumber numberWithLongLong:freemem] stringValue];
    [methodResult setResult:[NSNumber numberWithLongLong:freemem]];
}

-(void) getInternalStorage:(id<IMethodResult>)methodResult {
    long long freemem = get_free_diskspace() >> 10;
    //NSString* str_free_mem = [[NSNumber numberWithLongLong:freemem] stringValue];
    [methodResult setResult:[NSNumber numberWithLongLong:freemem]];
}

-(void) getExternalStorage:(id<IMethodResult>)methodResult {
    // unsupported on iOS
}





@end