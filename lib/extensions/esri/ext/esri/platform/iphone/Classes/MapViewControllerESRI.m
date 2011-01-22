//
//  Created by DmitryS .
//  Copyright 2011 rhomobile, inc. All rights reserved.
//

#ifdef __IPHONE_3_0

//#import "MapAnnotation.h"
#import "MapViewControllerESRI.h"
//#import "Rhodes.h"
//#import "RhoMainView.h"

#import "logging/RhoLog.h"
#import "rhoruby.h"
#import "common/RhoConf.h"

#import "rubyext/WebView.h"


#import "MapViewManager.h"
#import "ESRIViewManager.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapViewESRI"

static MapViewControllerESRI *mc = nil;

static NSString *kGeoLocatorURL = kESRI_geo_locator_URL;

static NSMutableArray* geocode_requests = nil;








@interface MapEngine_ESRI : NSObject <RhoMapViewProvider>
{
}

- (void)createMap:(rho_param*)params;
- (void)closeMap;
- (BOOL)isStarted;
- (double)centerLatitude;
- (double)centerLongitude;

@end

@implementation MapEngine_ESRI

- (void)createMap:(rho_param*)params {
	[MapViewControllerESRI createMap:params];
}

- (void)closeMap {
	[MapViewControllerESRI closeMap];
}

- (BOOL)isStarted {
	return [MapViewControllerESRI isStarted];
}

- (double)centerLatitude {
	return [MapViewControllerESRI centerLatitude];
}

- (double)centerLongitude {
	return [MapViewControllerESRI centerLongitude];
}

@end





@interface RhoESRIGeoLocatorItem : NSObject <AGSLocatorDelegate> {
	NSString* query_adress;
	AGSLocator* locator;
}
@property (nonatomic,retain) NSString* query_adress;
@property (nonatomic,retain) AGSLocator* locator;

-(id)initWithAdress:(NSString*)adress;
-(void)start;
-(void)locator:(AGSLocator *)locator operation:(NSOperation *)op didFindLocationsForAddress:(NSArray *)candidates;
-(void)locator:(AGSLocator *)locator operation:(NSOperation *)op didFailLocationsForAddress:(NSError *)error;
-(void)onFounded:(double)longitude latitude:(double)latitude;

@end


@interface RhoESRIGeoLocatorItemAnnotation : RhoESRIGeoLocatorItem {
	MapViewControllerESRI* map;
	NSString* title;
	NSString* subtitle;
	NSString* url;
	
}
@property (nonatomic,assign) MapViewControllerESRI* map;
@property (nonatomic,retain) NSString* title;
@property (nonatomic,retain) NSString* subtitle;
@property (nonatomic,retain) NSString* url;

-(id)init:(MapViewControllerESRI*)map adress:(NSString*)adress title:(NSString*)title subtitle:(NSString*)subtitle url:(NSString*)url;
-(void)onFounded:(double)longitude latitude:(double)latitude;

@end


@interface RhoESRIGeoLocatorItemMapCenter : RhoESRIGeoLocatorItem {
	MapViewControllerESRI* map;
	double radius;
	
}
@property (nonatomic,assign) MapViewControllerESRI* map;
@property (nonatomic,assign) double radius;

-(id)init:(MapViewControllerESRI*)map adress:(NSString*)adress radius:(double)radius;
-(void)onFounded:(double)longitude latitude:(double)latitude;

@end






@implementation RhoESRIGeoLocatorItem

@synthesize query_adress, locator;

-(id)initWithAdress:(NSString*)adress {
	self = [super init];
	self.query_adress = adress;
	if (geocode_requests != nil) {
		//[geocode_requests addObject:self];
	}
	return self;
}

-(void)start {
    self.locator = [AGSLocator locatorWithURL:[NSURL URLWithString:kGeoLocatorURL]];
    self.locator.delegate = self;
    
    NSArray *outFields = [NSArray arrayWithObjects:@"Score",
                          @"Latitude",
                          @"Longitude",
                          nil];
    
    //Create the address dictionary with the contents of the search bar
    NSDictionary *addresses = [NSDictionary dictionaryWithObjectsAndKeys:self.query_adress, @"PlaceName", nil];
	
    //now request the location from the locator for our address
    [self.locator locationsForAddress:addresses returnFields:outFields];
}

-(void)locator:(AGSLocator *)locator operation:(NSOperation *)op didFindLocationsForAddress:(NSArray *)candidates {
	if ((candidates != nil) && ([candidates count] > 0))
	{
        //loop through all candidates/results and add to graphics layer
		for (int i=0; i<[candidates count]; i++)
		{            
			AGSAddressCandidate *addressCandidate = (AGSAddressCandidate *)[candidates objectAtIndex:i];
			
            //get the location from the candidate
            AGSPoint *pt = addressCandidate.location;
			[self onFounded:pt.x latitude:pt.y];
			
			//[geocode_requests removeObject:self];
			return;
		}
	}	
}

-(void)locator:(AGSLocator *)locator operation:(NSOperation *)op didFailLocationsForAddress:(NSError *)error {
	//[geocode_requests removeObject:self];
}

-(void)onFounded:(double)longitude latitude:(double)latitude {
}

- (void)dealloc {
	[self.locator release];	
    [super dealloc];
}


@end


@implementation RhoESRIGeoLocatorItemAnnotation

@synthesize map, title, subtitle, url;

-(id)init:(MapViewControllerESRI*)map adress:(NSString*)adress title:(NSString*)title subtitle:(NSString*)subtitle url:(NSString*)url {
	self = [super initWithAdress:adress];
	self.map = map;
	self.title = title;
	self.subtitle = subtitle;
	self.url = url;
	[self start];
	return self;
}

-(void)onFounded:(double)longitude latitude:(double)latitude {
	[self.map addAnnotation:latitude longitude:longitude streetAdress:self.query_adress title:self.title subtitle:self.subtitle url:self.url];
}


@end


@implementation RhoESRIGeoLocatorItemMapCenter

@synthesize map, radius;

-(id)init:(MapViewControllerESRI*)map adress:(NSString*)adress radius:(double)radius {
	[super initWithAdress:adress];
	self.map = map;
	self.radius = radius;
	[self start];
	return self;
}

-(void)onFounded:(double)longitude latitude:(double)latitude {
	AGSSpatialReference *sr = [AGSSpatialReference spatialReferenceWithWKID:4326];
	double xmin, ymin, xmax, ymax;
	xmin = longitude - self.radius;
	ymin = latitude - self.radius;
	xmax = longitude + self.radius;
	ymax = latitude + self.radius;
	// zoom to region
	AGSEnvelope *env = [AGSEnvelope envelopeWithXmin:xmin ymin:ymin xmax:xmax ymax:ymax spatialReference:sr];
	env = (AGSEnvelope*)AGSGeometryGeographicToWebMercator(env);
	[self.map.mapView zoomToEnvelope:env animated:YES];
}

@end







@interface RhoCreateMapTaskESRI : NSObject {}

+ (void) run;

@end

@implementation RhoCreateMapTaskESRI

static RhoCreateMapTaskESRI* instance_create = nil;

-(void)runCommand:(NSValue*)value {
    if (mc) {
        [mc close];
        mc = nil;
    }
    MapViewControllerESRI* map = [[MapViewControllerESRI alloc] initWithParams:[value pointerValue]];
	mc = map;
	[ESRIViewManager openView];

}

+(void) run:(NSValue*)value {
	if (instance_create == nil) {
		instance_create = [[RhoCreateMapTaskESRI alloc] init];
	}
    [instance_create performSelectorOnMainThread:@selector(runCommand:) withObject:value waitUntilDone:NO];
}

@end

@interface RhoCloseMapTaskESRI : NSObject
+ (void)run;
@end

@implementation RhoCloseMapTaskESRI

static RhoCloseMapTaskESRI* instance_close = nil;

- (void)runCommand:(NSValue*)value {
    if (mc) {
        [mc close];
        mc = nil;
    }
}
+ (void)run {
	if (instance_close == nil) {
		instance_close = [[RhoCloseMapTaskESRI alloc] init];
	}
	[instance_close performSelectorOnMainThread:@selector(runCommand:) withObject:nil waitUntilDone:NO];
}

@end

@implementation MapViewControllerESRI

@synthesize savedMainView;

@synthesize mapView = _mapView;
@synthesize dynamicLayer = _dynamicLayer, dynamicLayerView = _dynamicLayerView;

@synthesize graphicsLayer = _graphicsLayer;
//@synthesize locator = _locator;
@synthesize calloutTemplate = _calloutTemplate;

@synthesize params_value, region_center, region_radius, esriLogo, esri_standard_map_url, esri_satellite_map_url;


- (id)initWithParams:(rho_param*)params {
    geocode_requests = [NSMutableArray arrayWithCapacity:10]; 
	
	[self init];

	mapType =  ESRI_MapType_RoadMap;
	zoomEnabled = TRUE;
	scrollEnabled = TRUE;	
	showsUserLocation = TRUE;
	region_set = FALSE;
	params_value = NULL;
	
	esri_standard_map_url = kESRI_standart_map_URL;
	esri_satellite_map_url = kESRI_satellite_map_URL;
	
	{
		char* std_map = rho_conf_getString("esri_map_url_roadmap");
		if (std_map) {
			esri_standard_map_url = [[NSString stringWithUTF8String:std_map] stringByAppendingString:@"MapServer"];
		}
		char* sat_map = rho_conf_getString("esri_map_url_satellite");
		if (sat_map) {
			esri_satellite_map_url = [[NSString stringWithUTF8String:std_map] stringByAppendingString:@"MapServer"];
		}
	}
	
	self.calloutTemplate = [[AGSCalloutTemplate alloc]init];

    if (params && params->type == RHO_PARAM_HASH) {
        rho_param *st = NULL;
        rho_param *ann = NULL;
        for (int i = 0, lim = params->v.hash->size; i < lim; ++i) {
            char *name = params->v.hash->name[i];
            rho_param *value = params->v.hash->value[i];
            if (strcasecmp(name, "settings") == 0)
                st = value;
            else if (strcasecmp(name, "annotations") == 0)
                ann = value;
        }
        if (st)
            [self setSettings:st];
        params_value = params;//[self setAnnotations:ann];
    }
    //rho_param_free(params);
	return self;
}




+ (void)createMap:(rho_param *)params {
	
	[RhoCreateMapTaskESRI run:[NSValue valueWithPointer:params]];
	
    //id runnable = [RhoCreateMapTaskESRI class];
    //id arg = [NSValue valueWithPointer:params];
    //[Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

+ (void)closeMap {
	[RhoCloseMapTaskESRI run];
    //id runnable = [RhoCloseMapTaskESRI class];
    //[Rhodes performOnUiThread:runnable wait:NO];
}



- (void)close {
    //[self dismissModalViewControllerAnimated:YES]; 
    
	[ESRIViewManager closeView];

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
                mapType = ESRI_MapType_RoadMap;
            else if (strcasecmp(map_type, "satellite") == 0)
                mapType = ESRI_MapType_Satellite;
            else if (strcasecmp(map_type, "hybrid") == 0)
                mapType = ESRI_MapType_Hybrid;
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
                
                double location_latitude = lat->type == RHO_PARAM_STRING ? strtod(lat->v.string, NULL) : 0;
                double location_longitude = lon->type == RHO_PARAM_STRING ? strtod(lon->v.string, NULL) : 0;
                double span_latitudeDelta = latSpan->type == RHO_PARAM_STRING ? strtod(latSpan->v.string, NULL) : 0;
                double span_longitudeDelta = lonSpan->type == RHO_PARAM_STRING ? strtod(lonSpan->v.string, NULL) : 0;

				region_latitude_min = location_latitude - span_latitudeDelta;
				region_latitude_max = location_latitude + span_latitudeDelta;
				region_longitude_min = location_longitude - span_longitudeDelta;
				region_longitude_max = location_longitude + span_longitudeDelta;
				
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
    }
}

- (void)setAnnotations:(rho_param*)p {
///*	
    int size = 1;
    if (p && p->type == RHO_PARAM_ARRAY)
        size += p->v.array->size;
    NSMutableArray *annotations = [NSMutableArray arrayWithCapacity:size];
    //if (region_center) {
    //    MapAnnotation *annObj = [[MapAnnotation alloc] init];
    //    annObj.type = @"center";
    //    annObj.address = region_center;
    //    CLLocationCoordinate2D c;
    //    c.latitude = c.longitude = 10000;
    //    annObj.coordinate = c;
    //    [annotations addObject:annObj];
    //    [annObj release];
    //}
    if (p && p->type == RHO_PARAM_ARRAY) {
        for (int i = 0, lim = p->v.array->size; i < lim; ++i) {
            rho_param *ann = p->v.array->value[i];
            if (ann->type != RHO_PARAM_HASH)
                continue;
            
            //CLLocationCoordinate2D coord;
            double coord_latitude = 10000;
            double coord_longitude = 10000;
            
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
                    coord_latitude = strtod(v, NULL);
                }
                else if (strcasecmp(name, "longitude") == 0) {
                    coord_longitude = strtod(v, NULL);
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
			
			if ((coord_latitude < 10000) && (coord_longitude < 10000)) {
				[self addAnnotation:coord_latitude longitude:coord_longitude streetAdress:address title:title subtitle:subtitle url:url];
			}
			else {
				if (address != nil) {
					[[RhoESRIGeoLocatorItemAnnotation alloc] init:self adress:address title:title subtitle:subtitle url:url];
				}
			}
            
         }
    }
 }

- (void)loadView {
	//CGRect rootViewBounds = [[[Rhodes sharedInstance] mainView] view].frame;
	CGRect rootViewBounds = CGRectMake(0, 20, 200, 200);
	UIView* mv = [[UIView alloc] initWithFrame:rootViewBounds];
	mv.autoresizesSubviews = YES;
	self.view = mv;
	[mv release];

}


+(UIView*)getCurrentView {
	if (mc != nil) {
		return mc.view;
	}
	return nil;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return YES;
}

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	//[self.mapView removeMapLayerWithName:@"Graphics Layer"];
    //[self.mapView addMapLayer:self.graphicsLayer withName:@"Graphics Layer"];
	//[self.view layoutSubviews];
	//[self.graphicsLayer dataChanged];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.view.opaque = YES;
	self.view.backgroundColor = [UIColor blackColor];

    //Initialize the toolbar
    toolbar = [[UIToolbar alloc] init];
    toolbar.barStyle = UIBarStyleBlack;
    UIBarButtonItem *closeButton = [[UIBarButtonItem alloc]
									initWithTitle:@"Close" style:UIBarButtonItemStyleBordered 
									target:self action:@selector(close_clicked:)];
    [toolbar setItems:[NSArray arrayWithObjects:closeButton,nil]];
	
	
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | 
	UIViewAutoresizingFlexibleWidth;
	toolbar.autoresizesSubviews = YES;
    [toolbar sizeToFit];
	
    CGFloat toolbarHeight = [toolbar frame].size.height;
	// hack for do not reduce height of toolbar in Landscape mode
	if (toolbarHeight < 44) {
		toolbarHeight = 44;
	}
	
	//RhoMainView* rw = [[Rhodes sharedInstance] mainView];
    
	CGRect rootViewBounds = CGRectMake(0, 20, 200, 200);//[[[Rhodes sharedInstance] mainView] view].frame;//bounds;
	
	self.view.frame = rootViewBounds;
	
    CGFloat rootViewHeight = rootViewBounds.size.height;
	//CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
    CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
    CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
    toolbar.frame = rectArea;
    
	
	[self.view addSubview:toolbar];
    [closeButton release];
    
    CGRect rectMapArea = CGRectMake(0, 0, rootViewWidth, rootViewHeight - toolbarHeight);

	AGSMapView* content = [[AGSMapView alloc] initWithFrame:rectMapArea];
	
	self.mapView = content;	
	self.mapView.frame = rectMapArea;
    self.mapView.autoresizesSubviews = YES;
    self.mapView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	[content release];

    
	/*Geocoder Stuff*/
    //[ggeoCoder start];
  
    [self.view insertSubview:self.mapView atIndex:0];
	

	// set the delegate for the map view
	self.mapView.mapViewDelegate = self;
	
	
	
	NSURL* baseURL = [NSURL URLWithString:kESRI_standart_map_URL];
	//create an instance of a tiled map service layer
	if (mapType == ESRI_MapType_RoadMap) {
		baseURL = [NSURL URLWithString:kESRI_standart_map_URL];
	}
	else if (mapType == ESRI_MapType_Satellite) {
		baseURL = [NSURL URLWithString:kESRI_satellite_map_URL];
	}
	else if (mapType == ESRI_MapType_Hybrid) {
		baseURL = [NSURL URLWithString:kESRI_satellite_map_URL];
	}
	AGSTiledMapServiceLayer *tiledLayer = [[AGSTiledMapServiceLayer alloc] initWithURL:baseURL];
	
	//Add it to the map view
	UIView<AGSLayerView>* lyr = [self.mapView addMapLayer:tiledLayer withName:@"Tiled Layer"];
	
	//release to avoid memory leaks
	[tiledLayer release];
	
	// Setting these two properties lets the map draw while still performing a zoom/pan
	lyr.drawDuringPanning = scrollEnabled;
	lyr.drawDuringZooming = zoomEnabled;
	
	
	//if (mapType == ESRI_MapType_Hybrid) {
	if (0) {	
		//create an instance of a dynmaic map layer
		self.dynamicLayer = [[[AGSDynamicMapServiceLayer alloc] initWithURL:[NSURL URLWithString:kESRI_standart_map_URL]] autorelease];
		
		//set visible layers
		self.dynamicLayer.visibleLayers = [NSArray arrayWithObjects:[NSNumber numberWithInt:2], nil];
		
		//name the layer. This is the name that is displayed if there was a property page, tocs, etc...
		self.dynamicLayerView = [self.mapView addMapLayer:self.dynamicLayer withName:@"Dynamic Layer"];
		
		//set transparency
		self.dynamicLayerView.alpha = 1.0;
	}
	
    
    //create the graphics layer that the geocoding result
    //will be stored in and add it to the map
    self.graphicsLayer = [AGSGraphicsLayer graphicsLayer];
    [self.mapView addMapLayer:self.graphicsLayer withName:@"Graphics Layer"];
	
	
	
	AGSSpatialReference *sr = [AGSSpatialReference spatialReferenceWithWKID:4326];
	double xmin, ymin, xmax, ymax;
	if (region_set) {
		xmin = region_longitude_min;
		ymin = region_latitude_min;
		xmax = region_longitude_max;
		ymax = region_latitude_max;
		
		// zoom to region
		AGSEnvelope *env = [AGSEnvelope envelopeWithXmin:xmin ymin:ymin xmax:xmax ymax:ymax spatialReference:sr];
		
		env = (AGSEnvelope*)AGSGeometryGeographicToWebMercator(env);
		
		//AGSEnvelope *env = [AGSEnvelope envelopeWithXmin:xmin ymin:ymin xmax:xmax ymax:ymax spatialReference:self.mapView.spatialReference];
		
		[self.mapView zoomToEnvelope:env animated:YES];
	}
	else if (region_center != nil){
		[[RhoESRIGeoLocatorItemMapCenter alloc] init:self adress:region_center radius:self.region_radius];
	}
	
	
	
    if (params_value && params_value->type == RHO_PARAM_HASH) {
        rho_param *ann = NULL;
        for (int i = 0, lim = params_value->v.hash->size; i < lim; ++i) {
            char *name = params_value->v.hash->name[i];
            rho_param *value = params_value->v.hash->value[i];
            if (strcasecmp(name, "annotations") == 0)
                ann = value;
        }
		if (ann != NULL) {
			[self setAnnotations:ann];
		}
		rho_param_free(params_value);
    }
	
	// ESRI logo
	// http://resources.arcgis.com/content/attribution
	//
	UIImage* logoimg = [UIImage imageNamed:@"esri.png"];
	self.esriLogo = [[UIImageView alloc] initWithImage:logoimg];
	
	CGSize size = [logoimg size]; 
	
	CGRect fr = self.mapView.frame; 
	
	CGRect logo_fr;
	logo_fr.origin.x = 0;
	logo_fr.origin.y = fr.size.height - size.height;
	logo_fr.size.width = size.width;
	logo_fr.size.height = size.height;
	self.esriLogo.frame = logo_fr;
	self.esriLogo.autoresizingMask = UIViewAutoresizingFlexibleTopMargin;
	[self.view addSubview:esriLogo]; 
	
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
	self.mapView = nil;
	self.dynamicLayer = nil;
	self.dynamicLayerView = nil;
	
    self.graphicsLayer = nil;
	//self.locator = nil;
	self.calloutTemplate = nil;	
	
	geocode_requests = nil;
	
    [super dealloc];
}


#pragma mark AGSMapViewDelegate methods

-(void) mapViewDidLoad:(AGSMapView*)mapView {
	
	// comment to disable the GPS on start up
	if (showsUserLocation) {
		[self.mapView.gps start];
	}
}

- (void)mapView:(AGSMapView *) mapView didClickAtPoint:(CGPoint) screen mapPoint:(AGSPoint *) mappoint graphics:(NSDictionary *) graphics {
    // on click on map
}


#pragma mark -
#pragma mark AGSMapViewDelegate

- (void)mapView:(AGSMapView *) mapView didClickCalloutAccessoryButtonForGraphic:(AGSGraphic *) graphic
{
	NSDictionary* dict = graphic.attributes;
    NSString* url = [dict objectForKey:@"URL"];
    NSLog(@"Callout tapped... Url = %@\n", url);
    //id<RhoMainView> mainView = [[Rhodes sharedInstance] mainView];
    //[mainView navigateRedirect:url tab:[mainView activeTab]];
    rho_webview_navigate([url UTF8String], 0);	
	[self close];
}



-(void)addAnnotation:(double)latitude longitude:(double)longitude 
								streetAdress:(NSString*)streetAdress
								title:(NSString*)title
								subtitle:(NSString*)subtitle
								url:(NSString*)url 
{
	AGSPictureMarkerSymbol *marker = [AGSPictureMarkerSymbol pictureMarkerSymbolWithImageNamed:@"BluePushpin.png"];
	marker.xoffset = 9;
	marker.yoffset = -16;
	marker.hotspot = CGPointMake(-9, -11);
	
	
	AGSSpatialReference *sr = [AGSSpatialReference spatialReferenceWithWKID:4326];
	AGSPoint *point = [[AGSPoint alloc] initWithX:longitude y:latitude spatialReference:sr];
	
	point = (AGSPoint*)AGSGeometryGeographicToWebMercator(point);
	
	//set the text and detail text based on 'Name' and 'Descr' fields in the attributes
	self.calloutTemplate.titleTemplate = @"${Name}";
	self.calloutTemplate.detailTemplate = @"${Descr}";
	
	//title - @"Name",
	//street adress - @"Match_addr",
	//subtitle - @"Descr",
	//latitude - @"Latitude"
	//longitude  - @"Longitude"
	//url - @"URL"
	
	
	NSMutableDictionary* attributes = [NSMutableDictionary dictionaryWithCapacity:6];
	if (title != nil) 
		[attributes setObject:title forKey:@"Name"];
	
	if (subtitle != nil) 
		[attributes setObject:subtitle forKey:@"Descr"];
	
	if (streetAdress != nil) 
		[attributes setObject:streetAdress forKey:@"Match_addr"];
	
	if (url != nil) 
		[attributes setObject:url forKey:@"URL"];
	
	//create the graphic
	AGSGraphic *graphic = [[AGSGraphic alloc] initWithGeometry: point
														symbol:marker 
													attributes:attributes
										  infoTemplateDelegate:self.calloutTemplate];
	
	
	//add the graphic to the graphics layer
	[self.graphicsLayer addGraphic:graphic];
	
	//[graphic release];   
	//[attributes release];

	self.mapView.callout.width = 250;
	//[self.mapView showCalloutAtPoint:(AGSPoint*)graphic.geometry forGraphic:graphic animated:YES];	
	
	
	[self.graphicsLayer dataChanged];
	
}
	




- (void) close_clicked:(id)sender {
    [self close];
	//mc = nil;
}


+ (BOOL)isStarted {
    return mc != nil;
}


+ (double)centerLatitude {
	AGSPoint* p = (AGSPoint*)AGSGeometryWebMercatorToGeographic(mc.mapView.envelope.center);
	return p.y;
}

+ (double)centerLongitude {
	AGSPoint* p = (AGSPoint*)AGSGeometryWebMercatorToGeographic(mc.mapView.envelope.center);
	return p.x;
}




@end

#endif


void register_ESRI_mapViewProvider() {
	[RhoMapViewProvidersManager registerMapViewProvider:@"ESRI" provider:[[MapEngine_ESRI alloc] init]];
}
