#ifdef __IPHONE_3_0

#import "GoogleGeocoder.h"

@implementation GoogleGeocoder

@synthesize actionTarget,onDidFindAddress,theElement,annotations,gapikey;

-(void)doGeocoding:(id)items {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSEnumerator * enumerator = [items objectEnumerator];
	MapAnnotation* annotation, *element;	
	while(element = [enumerator nextObject]) {
		annotation = nil;
		if ( (element.coordinate.latitude==10000) ||
			(element.coordinate.longitude==10000) ) {
			NSString* address = [element street_address];
			if ( ([address length] > 0) && [self geocode:element]) {
				annotation = element;
			}
		} else {
			annotation = element;
		}
		
		if(annotation && actionTarget && [actionTarget respondsToSelector:onDidFindAddress]) {
			[actionTarget performSelectorOnMainThread:onDidFindAddress withObject:annotation waitUntilDone:NO];
		}			
	}

    [pool release];	
}

-(void)start {
	if (annotations) {
		[NSThread detachNewThreadSelector:@selector(doGeocoding:) toTarget:self withObject:annotations];
	}
}

-(id)initWithAnnotations:(NSMutableArray*)_annotations apikey:(NSString*)key{
	self = [super init];
	annotations = _annotations;
	gapikey = [key copy];
    return self;
}

-(BOOL)geocode:(MapAnnotation *)annotation
{
	BOOL retvar = TRUE;
	currentAnnotation = annotation;
	NSMutableString *url = [[NSMutableString alloc] initWithString:kGeoCodeURL];
	[url appendString:[annotation street_address]];
	[url appendFormat:@"&output=xml"];
	[url replaceOccurrencesOfString:@"ä" withString:@"ae" options:1 range:(NSRange){0,[url length]}];
	[url replaceOccurrencesOfString:@"ö" withString:@"oe" options:1 range:(NSRange){0,[url length]}];
	[url replaceOccurrencesOfString:@"ü" withString:@"ue" options:1 range:(NSRange){0,[url length]}];
	[url replaceOccurrencesOfString:@"ß" withString:@"ss" options:1 range:(NSRange){0,[url length]}];
	[url replaceOccurrencesOfString:@" " withString:@"+" options:1 range:(NSRange){0,[url length]}];
	if (gapikey) {
		[url appendString:@"&key="];
		[url appendString:gapikey];
	}
	NSLog(@"Geocoding url = %@\n", url);
	NSURLRequest *request=[NSURLRequest requestWithURL:[NSURL URLWithString:url]							  
										cachePolicy:NSURLRequestUseProtocolCachePolicy
										timeoutInterval:60.0];	
	@try {
		NSHTTPURLResponse *response = NULL;
		NSData* data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
		NSXMLParser *parser = [[NSXMLParser alloc] initWithData:data];
		[parser setDelegate:self];
		[parser parse];
		[self stringCooridinatesToCLLocation];
	}
	@catch (NSException *exception) {
		NSLog(@"Geocoding failed");
		retvar = FALSE;
	}
	@finally {
		//NSLog(@"finally");
	}
	return retvar;
}

-(CLLocation *)stringCooridinatesToCLLocation{
	NSArray *array = [[currentAnnotation coordinate_string] componentsSeparatedByString:@","];
	double longitude = [[array objectAtIndex:0] doubleValue];
	double lattitude = [[array objectAtIndex:1] doubleValue];
	CLLocation *location = [[CLLocation alloc] initWithLatitude:lattitude longitude:longitude];
	[currentAnnotation setCoordinate:location.coordinate];
	return location;
}

#pragma mark -
#pragma mark XML Parsing Methods

-(void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName
 namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName
   attributes:(NSDictionary *)attributeDict {
	if(theElement) [theElement release];
	theElement = [[NSString alloc] initWithString:elementName];
}

-(void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string {
	if([theElement isEqualToString:@"address"])
		if([string isEqualToString:@"\n"]){
		}
		else if([string isEqualToString:@"\t"]){
		}
		else{
			[currentAnnotation setResolvedAddress:string];
		}
	
	if([theElement isEqualToString:@"coordinates"]){
		if([string isEqualToString:@"\n"]){
		}
		else if([string isEqualToString:@"\t"]){
		}
		else{
			[currentAnnotation setCoordinateString:string];
		}
	}
}

-(void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName
 namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qname {
}

	

@end

#endif
