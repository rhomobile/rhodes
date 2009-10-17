//
//  MapViewController.m
//
//  Created by Vlad on 9/16/09.
//  Copyright 2009 Rhomobile, Inc. All rights reserved.
//
#ifdef __IPHONE_3_0

#import "MapAnnotation.h"
#import "MapViewController.h"
#import "WebViewUrl.h"

@implementation MapViewController

@synthesize actionTarget,onNavigate,gapikey;

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

- (void)setParams:(NSMutableArray*)params {
	NSEnumerator * enumerator = [params objectEnumerator];
	NSString* element;	
	while(element = [enumerator nextObject]) {
		if ([element isEqualToString:@"map_type"]) {
			element = [enumerator nextObject];
			if ([element isEqualToString:@"standard"]) {
				mapType =  MKMapTypeStandard;
			} else if ([element isEqualToString:@"satellite"]) {
				mapType = MKMapTypeSatellite;
			} else if ([element isEqualToString:@"hybrid"]) {
				mapType = MKMapTypeHybrid;
			}
		} else if ([element isEqualToString:@"region"]) {
			NSMutableArray *arr = [enumerator nextObject];
			if ([arr count] != 4) {			
				region_set = FALSE;
			} else {
				CLLocationCoordinate2D location;	
				location.latitude=[[arr objectAtIndex:0] doubleValue];
				location.longitude=[[arr objectAtIndex:1] doubleValue];
				MKCoordinateSpan span;
				span.latitudeDelta=[[arr objectAtIndex:2] doubleValue];
				span.longitudeDelta=[[arr objectAtIndex:3] doubleValue];
				region.span=span;
				region.center=location;
				region_set = TRUE;
			}
		} else if ([element isEqualToString:@"zoom_enabled"]) {
			element = [enumerator nextObject];
			if ([element isEqualToString:@"true"]) {
				zoomEnabled = TRUE;
			} else {
				zoomEnabled = FALSE;
			}
		} else if ([element isEqualToString:@"scroll_enabled"]) {
			element = [enumerator nextObject];
			if ([element isEqualToString:@"true"]) {
				scrollEnabled = TRUE;
			} else {
				scrollEnabled = FALSE;
			}
		} else if ([element isEqualToString:@"shows_user_location"]) {
			element = [enumerator nextObject];
			if ([element isEqualToString:@"true"]) {
				showsUserLocation = TRUE;
			} else {
				showsUserLocation = FALSE;
			}
		} else if ([element isEqualToString:@"api_key"]) {
			gapikey = [[enumerator nextObject] copy]; 
		} else {
			[enumerator nextObject];
		}
    }
}

- (void)setAnnotations:(NSMutableArray*)annotations {
	ggeoCoder = [[GoogleGeocoder alloc] initWithAnnotations:annotations apikey:gapikey];
	ggeoCoder.actionTarget = self;
	ggeoCoder.onDidFindAddress = @selector(didFindAddress:);
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
calloutAccessoryControlTapped:(UIControl *)control {
	MapAnnotation *ann = (MapAnnotation*)[view annotation];
	NSString* url = [ann url];
	NSLog(@"Callout tapped... Url = %@\n", url);
	if(actionTarget && [actionTarget respondsToSelector:onNavigate]) {
		WebViewUrl *webViewUrl = [[[WebViewUrl alloc] init] autorelease];
		webViewUrl.url = url;
		webViewUrl.webViewIndex = 0;				
		[actionTarget performSelector:onNavigate withObject:webViewUrl];
		[self dismissModalViewControllerAnimated:YES]; 
		self.view.hidden = YES;
	}
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
	MapAnnotation* ann = (MapAnnotation*)annotation;
	NSString* url = [ann url];
	if ( [url length] > 0 ) {
		[annView setRightCalloutAccessoryView:[UIButton buttonWithType:UIButtonTypeDetailDisclosure]];
	}
	return annView;
}

@end

NSMutableArray* parse_annotations(int nannotations, char** annotation) {
	if (nannotations<=0) return [NSMutableArray arrayWithCapacity:0];
	NSMutableArray *annotations = [NSMutableArray arrayWithCapacity:1];
	CLLocationCoordinate2D current_coordinate;
	NSString* tmp;
	for(int i = 0; i < nannotations;) {
		MapAnnotation* annotation_obj = [[MapAnnotation alloc] init];
		tmp = [NSString stringWithCString:annotation[i++]];
		current_coordinate.latitude = [tmp doubleValue];
		tmp = [NSString stringWithCString:annotation[i++]];
		current_coordinate.longitude = [tmp doubleValue];
		[annotation_obj setCoordinate:current_coordinate];
		
		[annotation_obj setAddress:[NSString stringWithCString:annotation[i++]]];
		[annotation_obj setTitle:[NSString stringWithCString:annotation[i++]]];
		[annotation_obj setSubtitle:[NSString stringWithCString:annotation[i++]]];
		[annotation_obj setUrl:[NSString stringWithCString:annotation[i++]]];
		[annotations addObject:annotation_obj];		
	}
	return annotations;	
}

NSMutableArray* parse_settings(int nparams, char** params) {
	if (nparams <= 0) return [NSMutableArray arrayWithCapacity:0];
	NSMutableArray *settings = [NSMutableArray arrayWithCapacity:nparams];
	BOOL array_flag = FALSE;
	for(int i = 0; i < nparams; i++) {
		if (params[i]) {
			if (array_flag) {
				NSMutableArray *arr = [NSMutableArray arrayWithCapacity:1];
				char **array = (char**)params[i];
				while(*array) {
					[arr addObject:[NSString stringWithCString:*array]];
					array++;
				}
				array_flag = FALSE;
				[settings addObject:arr];
			} else {
				if (strcmp(params[i],"region")==0) {
					array_flag = TRUE;
				} 
				[settings addObject:[NSString stringWithCString:params[i]]];
				printf("param %s\n", params[i]);
			}
		} else {
			if (array_flag) {
				array_flag = FALSE;
				NSMutableArray *arr = [NSMutableArray arrayWithCapacity:1];
				[settings addObject:arr];
			} else {
				[settings addObject:@""];
			}
		}
	}
	return settings;
}

#endif