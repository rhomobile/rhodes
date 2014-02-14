
#import "<%= $cur_module.name %>FactoryBase.h"
#import "<%= $cur_module.name %>.h"
#import "<%= $cur_module.name %>Singleton.h"



@implementation <%= $cur_module.name %>FactoryBase

-(id)init {
    self = [super init];
    m<%= $cur_module.name %>Instances = [[NSMutableDictionary dictionaryWithCapacity:4] retain];
    m<%= $cur_module.name %>Singleton = nil;
    return self;
}

-(id<I<%= $cur_module.name %>>) get<%= $cur_module.name %>ByID:(NSString*)ID {
    id<I<%= $cur_module.name %>> obj = (<%= $cur_module.name %>*)[m<%= $cur_module.name %>Instances objectForKey:ID];
    if (obj == nil) {
        obj = [self create<%= $cur_module.name %>ByID:ID];
        [m<%= $cur_module.name %>Instances setObject:obj forKey:ID];
    }
    return obj;
}

-(void) destroyObjectByID:(NSString*)ID {
    <%= $cur_module.name %>* obj = [m<%= $cur_module.name %>Instances objectForKey:ID];
    if (obj != nil) {
        [m<%= $cur_module.name %>Instances removeObjectForKey:ID];
    }
}


-(id<I<%= $cur_module.name %>>)create<%= $cur_module.name %>ByID:(NSString*)ID {
    <%= $cur_module.name %>* obj = [[<%= $cur_module.name %> alloc] init];
<% if $cur_module.is_template_propertybag %>
    [obj setProperty:@"ID" propertyValue:ID methodResult:nil];
<% end %>
    return obj;
}


-(id<I<%= $cur_module.name %>Singleton>) get<%= $cur_module.name %>Singleton {
    if (m<%= $cur_module.name %>Singleton == nil) {
        m<%= $cur_module.name %>Singleton = [[<%= $cur_module.name %>Singleton alloc] init];
    }
    return m<%= $cur_module.name %>Singleton;
}

-(NSArray*) enumerate<%= $cur_module.name %>Instances {
    return [m<%= $cur_module.name %>Instances allKeys];
}

-(void)dealloc {
    [m<%= $cur_module.name %>Instances release];
    [super dealloc];
}


@end