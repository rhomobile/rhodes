//
//  MapViewController.m
//
//  Created by Vlad on 9/16/09.
//  Copyright 2009 Rhomobile, Inc. All rights reserved.
//
#ifdef __IPHONE_3_0

#import "MapAnnotation.h"
#import "MapViewController.h"
#import "Rhodes.h"
#import "RhoMainView.h"

#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

static MapViewController *mc = nil;





@interface RhoCreateMapTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoCreateMapTask
+ (void)run:(NSValue*)value {
    if (mc) {
        [mc close];
        mc = nil;
    }
    MapViewController* map = [[MapViewController alloc] init];
    [map setParams:[value pointerValue]];
    UIWindow *window = [[Rhodes sharedInstance] rootWindow];
	map.view.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	map.view.autoresizesSubviews = YES;
	
	UIView* v = [[[Rhodes sharedInstance] mainView] view];
	map.savedMainView = v;
	[map.savedMainView retain];
    [map.savedMainView removeFromSuperview];
	//map.savedMainView.hidden = YES;
	[window addSubview:map.view];
    
    mc = map;
}
@end

@interface RhoCloseMapTask : NSObject
+ (void)run;
@end

@implementation RhoCloseMapTask
+ (void)run {
    if (mc) {
        [mc close];
        mc = nil;
    }
}
@end


@implementation MapViewController

@synthesize region_center, gapikey, savedMainView;

+ (void)createMap:(rho_param *)params {
    id runnable = [RhoCreateMapTask class];
    id arg = [NSValue valueWithPointer:params];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

+ (void)closeMap {
    id runnable = [RhoCloseMapTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        mapType =  MKMapTypeStandard;
        zoomEnabled = TRUE;
        scrollEnabled = TRUE;	
        showsUserLocation = TRUE;
        region_set = FALSE;
        region_center = nil;
    }
    return self;
}

- (void)close {
    [self dismissModalViewControllerAnimated:YES]; 
    
	UIWindow *window = [[Rhodes sharedInstance] rootWindow];

	CGRect frame = self.view.bounds;
	
	self.savedMainView.frame = frame;
	

	[window addSubview:self.savedMainView];
	[window layoutSubviews];
	[self.view removeFromSuperview];
	//self.view.hidden = YES;

	[self.savedMainView release];
	self.savedMainView = nil;
}

- (void)setSettings:(rho_param*)p {
    if (!p || p->type != RHO_PARAM_HASH)
        return;
    
    for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
        char *name = p->v.hash->name[i];
        rho_param *value = p->v.hash->value[i];
        if (!name || !value)
            continue;
        
        if (strcasecmp(name, "map_type") == 0) {
            if (value->type != RHO_PARAM_STRING)
                continue;
            char *map_type = value->v.string;
            if (strcasecmp(map_type, "standard") == 0 || strcasecmp(map_type, "roadmap") == 0)
                mapType = MKMapTypeStandard;
            else if (strcasecmp(map_type, "satellite") == 0)
                mapType = MKMapTypeSatellite;
            else if (strcasecmp(map_type, "hybrid") == 0)
                mapType = MKMapTypeHybrid;
            else {
                RAWLOG_ERROR1("Unknown map type: %s", map_type);
                continue;
            }
        }
        else if (strcasecmp(name, "region") == 0) {
            if (value->type == RHO_PARAM_ARRAY) {
                if (value->v.array->size != 4)
                    continue;
                
                rho_param *lat = value->v.array->value[0];
                rho_param *lon = value->v.array->value[1];
                rho_param *latSpan = value->v.array->value[2];
                rho_param *lonSpan = value->v.array->value[3];
                
                CLLocationCoordinate2D location;	
                location.latitude = lat->type == RHO_PARAM_STRING ? strtod(lat->v.string, NULL) : 0;
                location.longitude = lon->type == RHO_PARAM_STRING ? strtod(lon->v.string, NULL) : 0;
                MKCoordinateSpan span;
                span.latitudeDelta = latSpan->type == RHO_PARAM_STRING ? strtod(latSpan->v.string, NULL) : 0;
                span.longitudeDelta = lonSpan->type == RHO_PARAM_STRING ? strtod(lonSpan->v.string, NULL) : 0;
                region.span = span;
                region.center = location;
                region_set = TRUE;
            }
            else if (value->type == RHO_PARAM_HASH) {
                char *center = NULL;
                char *radius = NULL;
                
                for (int j = 0, limm = value->v.hash->size; j < limm; ++j) {
                    char *rname = value->v.hash->name[j];
                    rho_param *rvalue = value->v.hash->value[j];
                    if (strcasecmp(rname, "center") == 0) {
                        if (rvalue->type != RHO_PARAM_STRING) {
                            RAWLOG_ERROR("Wrong type of 'center', should be String");
                            continue;
                        }
                        center = rvalue->v.string;
                    }
                    else if (strcasecmp(rname, "radius") == 0) {
                        if (rvalue->type != RHO_PARAM_STRING) {
                            RAWLOG_ERROR("Wrong type of 'radius', should be String");
                            continue;
                        }
                        radius = rvalue->v.string;
                    }
                }
                
                if (!center || !radius)
                    continue;
                
                region_center = [NSString stringWithUTF8String:center];
                region_radius = strtod(radius, NULL);
            }
        }
        else if (strcasecmp(name, "zoom_enabled") == 0) {
            if (value->type != RHO_PARAM_STRING)
                continue;
            zoomEnabled = strcasecmp(value->v.string, "true") == 0;
        }
        else if (strcasecmp(name, "scroll_enabled") == 0) {
            if (value->type != RHO_PARAM_STRING)
                continue;
            scrollEnabled = strcasecmp(value->v.string, "true") == 0;
        }
        else if (strcasecmp(name, "shows_user_location") == 0) {
            if (value->type != RHO_PARAM_STRING)
                continue;
            showsUserLocation = strcasecmp(value->v.string, "true") == 0;
        }
        else if (strcasecmp(name, "api_key") == 0) {
            if (value->type != RHO_PARAM_STRING)
                continue;
            gapikey = [NSString stringWithUTF8String:value->v.string];
        }
    }
}

- (void)setAnnotations:(rho_param*)p {
    int size = 1;
    if (p && p->type == RHO_PARAM_ARRAY)
        size += p->v.array->size;
    NSMutableArray *annotations = [NSMutableArray arrayWithCapacity:size];
    if (region_center) {
        MapAnnotation *annObj = [[MapAnnotation alloc] init];
        annObj.type = @"center";
        annObj.address = region_center;
        CLLocationCoordinate2D c;
        c.latitude = c.longitude = 10000;
        annObj.coordinate = c;
        [annotations addObject:annObj];
        [annObj release];
    }
    if (p && p->type == RHO_PARAM_ARRAY) {
        for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
            rho_param *ann = p->v.array->value[i];
            if (ann->type != RHO_PARAM_HASH)
                continue;
            
            CLLocationCoordinate2D coord;
            coord.latitude = 10000;
            coord.longitude = 10000;
            
            NSString *address = nil;
            NSString *title = nil;
            NSString *subtitle = nil;
            NSString *url = nil;
            
            for (int j = 0, limm = ann->v.hash->size; j < limm; ++j) {
                char *name = ann->v.hash->name[j];
                rho_param *value = ann->v.hash->value[j];
                if (!name || !value)
                    continue;
                if (value->type != RHO_PARAM_STRING)
                    continue;
                char *v = value->v.string;
                
                if (strcasecmp(name, "latitude") == 0) {
                    coord.latitude = strtod(v, NULL);
                }
                else if (strcasecmp(name, "longitude") == 0) {
                    coord.longitude = strtod(v, NULL);
                }
                else if (strcasecmp(name, "street_address") == 0) {
                    address = [NSString stringWithUTF8String:v];
                }
                else if (strcasecmp(name, "title") == 0) {
                    title = [NSString stringWithUTF8String:v];
                }
                else if (strcasecmp(name, "subtitle") == 0) {
                    subtitle = [NSString stringWithUTF8String:v];
                }
                else if (strcasecmp(name, "url") == 0) {
                    url = [NSString stringWithUTF8String:v];
                }
            }
            
            MapAnnotation *annObj = [[MapAnnotation alloc] init];
            [annObj setCoordinate:coord];
            if (address) [annObj setAddress:address];
            if (title) [annObj setTitle:title];
            if (subtitle) [annObj setSubtitle:subtitle];
            if (url) [annObj setUrl:url];
            [annotations addObject:annObj];
            [annObj release];
        }
    }
    ggeoCoder = [[GoogleGeocoder alloc] initWithAnnotations:annotations apikey:gapikey];
    ggeoCoder.actionTarget = self;
    ggeoCoder.onDidFindAddress = @selector(didFindAddress:);
}

- (void)setParams:(rho_param*)p {
    if (p && p->type == RHO_PARAM_HASH) {
        rho_param *st = NULL;
        rho_param *ann = NULL;
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (strcasecmp(name, "settings") == 0)
                st = value;
            else if (strcasecmp(name, "annotations") == 0)
                ann = value;
        }
        if (st)
            [self setSettings:st];
        [self setAnnotations:ann];
    }
    rho_param_free(p);
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    //Initialize the toolbar
    toolbar = [[UIToolbar alloc] init];
    toolbar.barStyle = UIBarStyleBlack;
    UIBarButtonItem *closeButton = [[UIBarButtonItem alloc]
                               initWithTitle:@"Close" style:UIBarButtonItemStyleBordered 
                               target:self action:@selector(close_clicked:)];
    [toolbar setItems:[NSArray arrayWithObjects:closeButton,nil]];

    [toolbar sizeToFit];
	
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | 
	UIViewAutoresizingFlexibleWidth;
	toolbar.autoresizesSubviews = YES;
	
    CGFloat toolbarHeight = [toolbar frame].size.height;

	//RhoMainView* rw = [[Rhodes sharedInstance] mainView];
    CGRect rootViewBounds = [[[Rhodes sharedInstance] mainView] view].bounds;
	
	self.view.frame = rootViewBounds;
	
    CGFloat rootViewHeight = rootViewBounds.size.height;
	//CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
    CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
    CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
    toolbar.frame = rectArea;
    
	
	[self.view addSubview:toolbar];
    [closeButton release];
    
    CGRect rectMapArea = CGRectMake(0, 0, rootViewWidth, rootViewHeight - toolbarHeight);
    mapView =[[MKMapView alloc] initWithFrame:rectMapArea];
	mapView.frame = rectMapArea;
    mapView.delegate=self;

    mapView.showsUserLocation=showsUserLocation;
    mapView.scrollEnabled=scrollEnabled;
    mapView.zoomEnabled=zoomEnabled;
    mapView.mapType=mapType;
	
    mapView.autoresizesSubviews = YES;
    mapView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	
	
	
    
    /*Geocoder Stuff*/
    [ggeoCoder start];
	
    //geoCoder=[[MKReverseGeocoder alloc] initWithCoordinate:location];
    //geoCoder.delegate=self;
    //[geoCoder start];
	
    /*Region and Zoom*/
    if (region_set) {
        [mapView setRegion:region animated:TRUE];
        [mapView regionThatFits:region];
    }
    
    [self.view insertSubview:mapView atIndex:0];
	[self.view layoutSubviews];
	
}

- (void)mapView:(MKMapView *)mapView annotationView:(MKAnnotationView *)view 
    calloutAccessoryControlTapped:(UIControl *)control
{
    MapAnnotation *ann = (MapAnnotation*)[view annotation];
    NSString* url = [ann url];
    NSLog(@"Callout tapped... Url = %@\n", url);
    id<RhoMainView> mainView = [[Rhodes sharedInstance] mainView];
    [mainView navigateRedirect:url tab:[mainView activeTab]];
    [self dismissModalViewControllerAnimated:YES]; 
    self.view.hidden = YES;
}

- (void) close_clicked:(id)sender {
    [self close];
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

/*- (IBAction)changeType:(id)sender{
	if(mapType.selectedSegmentIndex==0){
		mapView.mapType=MKMapTypeStandard;
	}
	else if (mapType.selectedSegmentIndex==1){
		mapView.mapType=MKMapTypeSatellite;
	}
	else if (mapType.selectedSegmentIndex==2){
		mapView.mapType=MKMapTypeHybrid;
	}
}*/

- (void)reverseGeocoder:(MKReverseGeocoder *)geocoder didFailWithError:(NSError *)error{
    NSLog(@"Reverse Geocoder Errored");
}

- (void)didFindAddress:(MapAnnotation*)annotation {
    if ([[annotation type] isEqualToString:@"center"]) {
        MKCoordinateSpan span;
        span.latitudeDelta = region_radius;
        span.longitudeDelta = region_radius;
        region.center = annotation.coordinate;
        region.span = span;
        region_set = TRUE;
        
        [mapView setRegion:region animated:YES];
        [mapView regionThatFits:region];
    }
    else
        [mapView addAnnotation:annotation];
}
	
- (void)reverseGeocoder:(MKReverseGeocoder *)geocoder didFindPlacemark:(MKPlacemark *)placemark{
    NSLog(@"Reverse Geocoder completed");
    //mPlacemark=placemark;
    //[mapView addAnnotation:placemark];
}

- (MKAnnotationView *) mapView:(MKMapView *)mapView viewForAnnotation:(id <MKAnnotation>) annotation{
    MKPinAnnotationView *annView = [[[MKPinAnnotationView alloc]
                                     initWithAnnotation:annotation reuseIdentifier:@"currentloc"] autorelease];
    annView.animatesDrop = TRUE;
    annView.canShowCallout = YES;
    if ([annotation isKindOfClass:[MapAnnotation class]]) {
        MapAnnotation* ann = (MapAnnotation*)annotation;
        NSString* url = [ann url];
        if ([url length] > 0) {
            [annView setRightCalloutAccessoryView:[UIButton buttonWithType:UIButtonTypeDetailDisclosure]];
        }
    }
    return annView;
}

+ (BOOL)isStarted {
    return mc != nil;
}

+ (CLLocationCoordinate2D)center {
    CLLocationCoordinate2D center;
    if (mc) {
        center = mc->region.center;
    }
    else {
        center.latitude = 0;
        center.longitude = 0;
    }
        
    return center;
}

@end

#endif

void mapview_create(rho_param *p) {
    if (!rho_rhodesapp_check_mode())
        return;
#ifdef __IPHONE_3_0
	[MapViewController createMap:rho_param_dup(p)];
#endif	
}

void mapview_close() {
    if (!rho_rhodesapp_check_mode())
        return;
#ifdef __IPHONE_3_0
    [MapViewController closeMap];
#endif
}

VALUE mapview_state_started() {
#ifdef __IPHONE_3_0
    BOOL started = [MapViewController isStarted];
    return rho_ruby_create_boolean(started);
#else
    return 0;
#endif
}

double mapview_state_center_lat() {
#ifdef __IPHONE_3_0
    return [MapViewController center].latitude;
#else
    return 0;
#endif
}

double mapview_state_center_lon() {
#ifdef __IPHONE_3_0
    return [MapViewController center].longitude;
#else
    return 0;
#endif
}
