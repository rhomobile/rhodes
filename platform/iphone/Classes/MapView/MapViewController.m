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

#include "common/rhoparams.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

@interface RhoCreateMapTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoCreateMapTask
+ (void)run:(NSValue*)value {
    MapViewController* map = [[MapViewController alloc] init];
    [map setParams:[value pointerValue]];
    UIWindow *window = [[Rhodes sharedInstance] rootWindow];
	[window addSubview:map.view];
    [map.view setFrame:[window frame]];
}
@end

@implementation MapViewController

@synthesize gapikey;

+ (void)createMap:(rho_param *)params {
    id runnable = [RhoCreateMapTask class];
    id arg = [NSValue valueWithPointer:params];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
		mapType =  MKMapTypeStandard;
		zoomEnabled = TRUE;
		scrollEnabled = TRUE;	
		showsUserLocation = TRUE;
		region_set = FALSE;
    }
    return self;
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
            if (strcasecmp(map_type, "standard") == 0)
                mapType = MKMapTypeStandard;
            else if (strcasecmp(map_type, "satellite") == 0)
                mapType = MKMapTypeSatellite;
            else if (strcasecmp(map_type, "hybrid") == 0)
                mapType = MKMapTypeHybrid;
        }
        else if (strcasecmp(name, "region") == 0) {
            if (value->type != RHO_PARAM_ARRAY)
                continue;
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
    int size = 0;
    if (p && p->type == RHO_PARAM_ARRAY)
        size = p->v.array->size;
    NSMutableArray *annotations = [NSMutableArray arrayWithCapacity:size];
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
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            if (strcasecmp(name, "settings") == 0)
                [self setSettings:value];
            else if (strcasecmp(name, "annotations") == 0)
                [self setAnnotations:value];
        }
    }
    rho_param_free(p);
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	
	//Initialize the toolbar
	toolbar = [[UIToolbar alloc] init];
	toolbar.barStyle = UIBarStyleBlack;
	[toolbar sizeToFit];
	CGFloat toolbarHeight = [toolbar frame].size.height;
	CGRect rootViewBounds = self.view.bounds;
	CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
	CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
	CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
	[toolbar setFrame:rectArea];
	UIBarButtonItem *closeButton = [[UIBarButtonItem alloc]
								   initWithTitle:@"Close" style:UIBarButtonItemStyleBordered 
								   target:self action:@selector(close_clicked:)];
	[toolbar setItems:[NSArray arrayWithObjects:closeButton,nil]];
	[self.view addSubview:toolbar];
    [closeButton release];
	
	CGRect rectMapArea = CGRectMake(0, 0, rootViewWidth, rootViewHeight - toolbarHeight);
	mapView=[[MKMapView alloc] initWithFrame:rectMapArea];
	mapView.delegate=self;

	mapView.showsUserLocation=showsUserLocation;
	mapView.scrollEnabled=scrollEnabled;
	mapView.zoomEnabled=zoomEnabled;
	mapView.mapType=mapType;
	
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
    [self dismissModalViewControllerAnimated:YES]; 
	self.view.hidden = YES;
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
	[mapView addAnnotation:annotation];
}
	
- (void)reverseGeocoder:(MKReverseGeocoder *)geocoder didFindPlacemark:(MKPlacemark *)placemark{
	NSLog(@"Reverse Geocoder completed");
	//mPlacemark=placemark;
	//[mapView addAnnotation:placemark];
}

- (MKAnnotationView *) mapView:(MKMapView *)mapView viewForAnnotation:(id <MKAnnotation>) annotation{
	MKPinAnnotationView *annView=[[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"currentloc"];
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

@end

#endif

void mapview_create(rho_param *p) {
#ifdef __IPHONE_3_0
	[MapViewController createMap:rho_param_dup(p)];
#endif	
}
