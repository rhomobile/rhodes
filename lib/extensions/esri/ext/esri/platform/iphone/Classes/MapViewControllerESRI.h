//
//  Created by DmitryS .
//  Copyright 2011 rhomobile, inc. All rights reserved.
//


#ifdef __IPHONE_3_0

//#import <MapKit/MapKit.h>
//#import <MapKit/MKAnnotation.h>
//#import <MapKit/MKReverseGeocoder.h>
//#import "GoogleGeocoder.h"
//#import "RhoViewController.h"

#import "rhoruby.h"


#import "MapViewManager.h"

#import <UIKit/UIKit.h>
#import "ArcGIS.h"


// Good view maps, but use ESRI meters for coordinates
#define kESRI_standart_map_URL @"http://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer"
#define kESRI_satellite_map_URL @"http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer"

// 2D maps - support longitude/latitude coordinates
//#define kESRI_standart_map_URL @"http://server.arcgisonline.com/ArcGIS/rest/services/ESRI_StreetMap_World_2D/MapServer"
//#define kESRI_satellite_map_URL @"http://server.arcgisonline.com/ArcGIS/rest/services/ESRI_Imagery_World_2D/MapServer"

//PlaceName -> Latitude, Longitude
#define kESRI_geo_locator_URL @"http://tasks.arcgisonline.com/ArcGIS/rest/services/Locators/ESRI_Places_World/GeocodeServer"

//Adress + Country -> X,Y
//#define kESRI_geo_locator_URL @"http://tasks.arcgisonline.com/ArcGIS/rest/services/Locators/TA_Address_EU/GeocodeServer"

//Adress -> X,Y
//#define kESRI_geo_locator_URL @"http://tasks.arcgisonline.com/ArcGIS/rest/services/Locators/TA_Address_NA/GeocodeServer"

//Street -> X,Y
//#define kESRI_geo_locator_URL @"http://tasks.arcgisonline.com/ArcGIS/rest/services/Locators/TA_Streets_US/GeocodeServer"



typedef enum  {
	ESRI_MapType_RoadMap,
	ESRI_MapType_Satellite,
	ESRI_MapType_Hybrid
} RHO_ESRI_Map_Type;

//Set up constant for predefined where clause for search
//#define kLayerDefinitionFormat @"STATE_NAME = '%@'"





@interface MapViewControllerESRI : UIViewController <AGSMapViewDelegate> {
    //GoogleGeocoder* ggeoCoder;
    UIToolbar* toolbar;
    NSString *region_center;
    double region_radius;

    AGSGraphicsLayer *_graphicsLayer;
	AGSCalloutTemplate *_calloutTemplate;
	
	UIView* savedMainView;
	
	RHO_ESRI_Map_Type mapType;
    BOOL zoomEnabled;
    BOOL scrollEnabled;	
    BOOL showsUserLocation;
    BOOL region_set;	
	double region_latitude_min;
	double region_latitude_max;
	double region_longitude_min;
	double region_longitude_max;
	
	rho_param* params_value;

	UIImageView* esriLogo;
	
	//container for map layers
	AGSMapView *_mapView;
	
	//this map has a dynamic layer, need a view to act as a container for it
	AGSDynamicMapServiceLayer *_dynamicLayer;
	UIView *_dynamicLayerView;
	
	NSString* esri_standard_map_url;
	NSString* esri_satellite_map_url;
	
}

// 
@property (nonatomic, retain) NSString* esri_standard_map_url;
@property (nonatomic, retain) NSString* esri_satellite_map_url;
@property (nonatomic, retain) NSString *region_center;
@property (nonatomic, assign) double region_radius;
@property (nonatomic,retain) UIView *savedMainView;

//map view is an outlet so we can associate it with UIView
//in IB
@property (nonatomic, retain) IBOutlet AGSMapView *mapView;
@property (nonatomic, assign) AGSDynamicMapServiceLayer *dynamicLayer;
@property (nonatomic, assign) UIView *dynamicLayerView;
@property (nonatomic, assign) RHO_ESRI_Map_Type mapType;

@property (nonatomic, retain) AGSGraphicsLayer *graphicsLayer;
//@property (nonatomic, retain) AGSLocator *locator;
@property (nonatomic, retain) AGSCalloutTemplate *calloutTemplate;

@property (nonatomic, assign) rho_param* params_value;

@property (nonatomic, retain) UIImageView* esriLogo;


- (id)initWithParams:(rho_param*)params;




+ (void)createMap:(rho_param*)params;
+ (void)closeMap;
+ (BOOL)isStarted;
+ (double)centerLatitude;
+ (double)centerLongitude;

-(void)addAnnotation:(double)latitude longitude:(double)longitude 
						streetAdress:(NSString*)streetAdress
						title:(NSString*)title
						subtitle:(NSString*)subtitle
						url:(NSString*)url;

- (void)setParams:(rho_param*)params;
- (void)close;

+(UIView*)getCurrentView;

@end

#endif