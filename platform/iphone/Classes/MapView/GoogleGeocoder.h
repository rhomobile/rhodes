#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "MapAnnotation.h"

#define kGeoCodeURL @"http://maps.google.com/maps/geo?q="

@interface GoogleGeocoder : NSObject {
	NSMutableArray *annotations;
	MapAnnotation* currentAnnotation;
	NSString *theElement;
@public
	id  actionTarget;
	SEL onDidFindAddress;
	
}

// 
@property (assign) id  actionTarget;
@property (assign) SEL onDidFindAddress;
@property (nonatomic,retain) NSString *theElement;
@property (nonatomic,retain) NSMutableArray *annotations;

//
-(void)start;
-(id)initWithAnnotations:(NSMutableArray*)annotations;

-(BOOL)geocode:(MapAnnotation *)annotation;
-(CLLocation*)stringCooridinatesToCLLocation;
@end
