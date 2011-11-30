//
//  main.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/19/10.
//  Copyright RhoMobile 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "../../RhoConnectClient.h"

RhoConnectClient* sclient;

RhomModel* customer;
RhomModel* product;
RhomModel* perftest;
RhomModel* blobTest;

NSFileManager* fileManager;

int ResetAndLogout()
{
	[sclient database_full_reset_and_logout];
	if ([sclient is_logged_in]) 
		return 0;

	return 1;
}

int shouldNotSyncWithoutLogin()
{
	return 1;
}

int shouldFindBySql()
{
	NSMutableArray* items = [product find_bysql:@"SELECT * FROM sources" args: nil];	
	if ( !items )
		return 0;
    
    if ( [items count] != 3 )
        return 0;
    
    NSArray* params = [NSArray arrayWithObjects: product.name, nil];
	NSMutableArray* items2 = [product find_bysql:@"SELECT * FROM sources WHERE name=?" args: params];	
	if ( !items2 )
		return 0;
    
    if ( [items2 count] != 1 )
        return 0;
    
    return 1;
}

int shouldLogin()
{
	RhoConnectNotify* res = [sclient loginWithUser:@"" pwd:@""];
	int nErr = res.error_code;
	if ( nErr!= RHO_ERR_NONE || ![sclient is_logged_in]) {
		return 0;
	}
	
	return 1;
}

int shouldSyncProductByName()
{
	RhoConnectNotify* res = [product sync];
	int	nErr = res.error_code;
	if ( nErr!= RHO_ERR_NONE ) {
		return 0;
	}

	return 1;
}

int shouldSearchProduct()
{
	return 1;
}

@interface CObjectCallback : NSObject {
}
@property(readonly) RhoConnectObjectNotify* m_pNotify;

- (id) init;
- (void)objectNotifyCallback:(RhoConnectObjectNotify*) notify;
@end

@implementation CObjectCallback
@synthesize  m_pNotify;
- (id) init
{
    self = [super init];
	    
    [sclient setObjectNotification:@selector(objectNotifyCallback:) target:self];        
    return self;
}

- (void)objectNotifyCallback:(RhoConnectObjectNotify*) notify
{
    m_pNotify = notify;
}
@end

int shouldCreateNewProduct()
{
	NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
	[item setValue:@"Test" forKey:@"name"];	
    [item setValue:@"123" forKey:@"sku"];	
	[product create:item];
	if ( [item objectForKey:@"object"] == NULL || [item objectForKey:@"source_id"] == NULL ) 
		return 0;

	NSDictionary* item2 = [product find:[item valueForKey:@"object"]];
	if ( ![item2 isEqualToDictionary: item])
		return 0;

    CObjectCallback* pObjectCallback = [[ CObjectCallback alloc] init];
    [sclient addObjectNotify: [[item objectForKey:@"source_id"] intValue] szObject:[item valueForKey:@"object"] ];
     
	RhoConnectNotify* res = [product sync];
	if ( res.error_code!= RHO_ERR_NONE )
		return 0;

	NSDictionary* item3 = [product find:[item valueForKey:@"object"]];
	if ( item3 )
		return 0;

	if ( !pObjectCallback.m_pNotify.created_objects )
        return 0;

	if ( [pObjectCallback.m_pNotify.created_objects count] <= 0 )
        return 0;
    
	if ( [[pObjectCallback.m_pNotify.created_objects objectAtIndex:0] compare: [item valueForKey:@"object"]] != 0 )
        return 0;
    
	[item release];
	[pObjectCallback release];
    
	return 1;
}

int shouldModifyProduct()
{
	NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
	[cond setValue:@"Test" forKey:@"name"];							 
	
	NSMutableDictionary* item = [product find_first: cond];
	if ( !item )
		return 0;
	
	NSString* saved_object = [NSString stringWithString: [item valueForKey:@"object"]];
	NSMutableString* new_sku = [[NSMutableString alloc]init];
    if ( [item valueForKey:@"sku"] != nil)
    {
        [new_sku appendString:[item valueForKey:@"sku"]];
    }
    
	[new_sku appendString: @"_TEST"];
	
	[item setValue:new_sku forKey:@"sku"];
	[product save: item];
	
	RhoConnectNotify* res = [product sync];
	if ( res.error_code!= RHO_ERR_NONE )
		return 0;
	
	NSDictionary* item3 = [product find: saved_object];
	if ( !item3 )
		return 0;
	
	if ( ![[item3 valueForKey:@"sku"] isEqual: [item valueForKey:@"sku"]])
		return 0;
	
	[cond release];
	[new_sku release];				
	
	return 1;
}

int shouldProcessCreateError()
{
	NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
	[item setValue:@"Test" forKey:@"name"];		
	[product create:item];
	if ( [item objectForKey:@"object"] == NULL || [item objectForKey:@"source_id"] == NULL ) 
		return 0;
    
    NSMutableString* err_resp = [NSMutableString stringWithString:@"[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\""];
    [err_resp appendString: [item objectForKey:@"object"]];
    [err_resp appendString: @"\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\""];
    [err_resp appendString: [item objectForKey:@"object"]];
    [err_resp appendString: @"-error\":{\"message\":\"error create\"}}}]"];
    [sclient setSourceProperty: [product source_id] szPropName:@"rho_server_response" szPropValue:err_resp];
                                  
    RhoConnectNotify* res = [product sync];
    if ( ![res hasCreateErrors] )
        return 0;
    
    [sclient onCreateError: res action: @"delete"];
    
    NSArray* params = [NSArray arrayWithObjects: [item objectForKey:@"object"], nil];
	NSMutableArray* items2 = [product find_bysql:@"SELECT * FROM changed_values WHERE object=?" args: params];	
	if ( !items2 )
		return 0;
    
    if ( [items2 count] != 0 )
        return 0;

	items2 = [product find_bysql:@"SELECT * FROM changed_values WHERE update_type='create'" args: nil];	
	if ( !items2 )
		return 0;
    
    if ( [items2 count] != 0 )
        return 0;
    
    [product find_bysql:@"DELETE FROM changed_values" args: nil];
    
    return 1;
}

int shouldProcessUpdateError()
{
	NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
	[item setValue:@"Test" forKey:@"name"];		
	[product create:item];
	if ( [item objectForKey:@"object"] == NULL || [item objectForKey:@"source_id"] == NULL ) 
		return 0;
    
    NSMutableString* err_resp = [NSMutableString stringWithString:@"[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"update-rollback\": {\""];
    [err_resp appendString: [item objectForKey:@"object"]];
    [err_resp appendString: @"\": {\"name\": \"OLD_NAME\"}},\"update-error\":{\""];
    [err_resp appendString: [item objectForKey:@"object"]];
    [err_resp appendString: @"\":{\"name\":\"wrongname\",\"an_attribute\":\"error update\"},\""];
    [err_resp appendString: [item objectForKey:@"object"]];
    [err_resp appendString: @"-error\":{\"message\":\"error update\"}}}]"];
    [sclient setSourceProperty: [product source_id] szPropName:@"rho_server_response" szPropValue:err_resp];
    
    RhoConnectNotify* res = [product sync];
    if ( ![res hasUpdateErrors] )
        return 0;
    
    [sclient onUpdateError: res action: @"rollback"];
    
    NSMutableArray* items2 = [product find_bysql:@"SELECT * FROM changed_values WHERE update_type='update'" args: nil];	
	if ( !items2 )
		return 0;
    
    if ( [items2 count] != 0 )
        return 0;
    
    return 1;
}

int shouldProcessDeleteError()
{
    NSString* err_resp = @"[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"delete-error\":{\"broken_object_id\":{\"name\":\"wrongname\",\"an_attribute\":\"error delete\"},\"broken_object_id-error\":{\"message\":\"Error delete record\"}}}]";
    [sclient setSourceProperty: [product source_id] szPropName:@"rho_server_response" szPropValue:err_resp];
    
    RhoConnectNotify* res = [product sync];
    if ( ![res hasDeleteErrors] )
        return 0;
    
    [sclient onDeleteError: res action: @"retry"];
    
    NSMutableArray* items2 = [product find_bysql:@"SELECT * FROM changed_values WHERE update_type='delete'" args: nil];	
	if ( !items2 )
		return 0;
    
    if ( [items2 count] != 2 )
        return 0;
    
    return 1;
}

int shouldProcessServerErrors()
{

    if ( !shouldProcessCreateError() )
        return 0;
    
    if ( !shouldProcessUpdateError() )
        return 0;
    
    if ( !shouldProcessDeleteError() )
        return 0;
    
    [sclient setSourceProperty: [product source_id] szPropName:@"rho_server_response" szPropValue:@""];
    
    return 1;
}

int shouldDeleteAllTestProduct()
{
	NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
	[cond setValue:@"Test" forKey:@"name"];							 
	
	NSMutableArray* items = [product find_all:cond];	
	if ( !items )
		return 0;
	
	for( NSDictionary* item in items)
	{
		[product destroy: item];
	}
	
	RhoConnectNotify* res = [product sync];
	//if ( res.error_code!= RHO_ERR_NONE )
	//	return 0;

	NSMutableDictionary* item = [product find_first:cond];	
	if ( item )
		return 0;
    
    [cond release];
	return 1;
}

int shouldPerfomanceTest_create(int nCount)
{
	[perftest startBulkUpdate];	
	for (int i = 0; i < nCount; i++) 
	{
		NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
		[item setValue: [NSString stringWithFormat:@"Test%d", i] forKey:@"name"];							 
		[perftest create:item];
	}
	[perftest stopBulkUpdate];
	
	return 1;
}

int shouldPerfomanceTest_delete()
{
	NSMutableArray* items = [perftest find_all:NULL];
	if ( !items )
		return 0;
	
	[perftest startBulkUpdate];
	for( NSDictionary* item in items)
	{
		[perftest destroy: item];
	}
	[perftest stopBulkUpdate];
	
	return 1;
}

int shouldCreateNewProductWithCustomers()
{
    NSMutableDictionary* cust1;
    NSMutableDictionary* cust2;
    NSMutableDictionary* item;
    ;
    
    // exception to throw on test failure
    NSException *e = [NSException
                      exceptionWithName: @"NSException"
                      reason: @"test faled"
                      userInfo: nil];

    int result = 1;
    @try {

        cust1 = [[NSMutableDictionary alloc] init];
        [cust1 setValue: @"CustTest1" forKey: @"first"];	
        [customer create:cust1];

        cust2 = [[NSMutableDictionary alloc] init];
        [cust2 setValue: @"CustTest2" forKey: @"first"];	
        [customer create:cust2];

        item = [[NSMutableDictionary alloc] init];
        [item setValue: [NSString stringWithFormat: @"%f", [[NSDate date] timeIntervalSince1970]] forKey:@"name"];	
        [item setValue: [cust1 objectForKey: @"object"] forKey:@"quantity"];	
        [item setValue: [cust2 objectForKey: @"object"] forKey:@"sku"];	
        [product create: item];
        
        RhoConnectNotify* res = [sclient syncAll];
        if (   res.error_code != RHO_ERR_NONE
            || NO == [@"complete" isEqualToString: res.status] ) {
            @throw e;
        }
        
        NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
        [cond setValue: [item objectForKey: @"name"] forKey:@"name"];							 
        NSMutableDictionary* prod = [product find_first: cond];
        [cond release];
        
        if ( !prod )
            @throw e;
        
        // we shoud have all object id changed after sync performed
        if (   [prod objectForKey: @"object"] == [item objectForKey: @"object"]
            || [prod objectForKey: @"quantity"] == [cust1 objectForKey: @"object"]
            || [prod objectForKey: @"sku"] == [cust2 objectForKey: @"object"]
            )
            @throw e;

        NSMutableDictionary* cust11 = [customer find: [prod objectForKey: @"quantity"]];
        if ( !cust11
            || NO == [[cust11 objectForKey: @"first"] isEqualToString: [cust1 objectForKey: @"first"]] )
            @throw e;
        
        
        NSMutableDictionary* cust22 = [customer find: [prod objectForKey: @"sku"]];
        if ( !cust22
            || NO == [[cust22 objectForKey: @"first"] isEqualToString: [cust2 objectForKey: @"first"]] )
            @throw e;

    } @catch( NSException* e) {
        result = 0;
    } @finally {
        [customer destroy: cust1];
        [customer destroy: cust2];
        [product destroy: item];
        [cust1 release];
        [cust2 release];
        [item release];
    }

    return result;
}

int runObjCClientTest()
{
	customer = [[RhomModel alloc] init];
	customer.name = @"Customer";
	
	product = [[RhomModel alloc] init];
	product.name = @"Product";
    /*
     IMPORTANT NOTE:
     Here is no any real meaning to associate product with customer via "quantity" or "sku" attribute,
     it has been used just to test it works from the technical point of view.
     Please, don't consider it as some real application logic sample. It demonstrates just an API capabilities.
     In a real world application it looks like you will need additional model named Order to refer Product and
     Customer models.
     */
    product.associations = [NSDictionary dictionaryWithObjectsAndKeys: @"Customer", @"quantity", @"Customer", @"sku", nil];
    
    //product.name = @"Product_s";
    //product.model_type = RMT_PROPERTY_FIXEDSCHEMA;    
    //add to schema:
    /*CREATE TABLE "Product_s" ( 
     "brand" varchar default null,
     "created_at" varchar default null,
     "name" varchar default null,
     "price" varchar default null,
     "quantity" varchar default null,
     "sku" varchar default null,
     "updated_at" varchar default null,
     "object" varchar(255) PRIMARY KEY );
     
     */
	perftest = [[RhomModel alloc] init];
	perftest.name = @"Perftest";
	perftest.sync_type = RST_NONE;
	
	[RhoConnectClient initDatabase];
	sclient = [[RhoConnectClient alloc] init];
    NSMutableArray* models = [NSMutableArray arrayWithObjects:customer, product, perftest, nil];	
	
	[sclient addModels:models];
    
    //sclient.threaded_mode = FALSE;
	//sclient.poll_interval = 0;
    [sclient setLogSeverity:1];
	
    // exception to throw on test failure
    NSException *e = [NSException
                      exceptionWithName: @"NSException"
                      reason: @"test faled"
                      userInfo: nil];
    int result = 1;
    @try {
        if ( !ResetAndLogout() )
            @throw e;
        
        sclient.sync_server = @"http://rhodes-store-server.heroku.com/application";
        //sclient.sync_server = @"http://192.168.0.103:9292/application";

        if ( !shouldFindBySql() )
            @throw e;
        
        if ( !shouldNotSyncWithoutLogin() )
            @throw e;
        
        if ( !shouldLogin() )
            @throw e;
        
        if ( !shouldSyncProductByName() )
            @throw e;
        
        if ( !shouldSearchProduct() )
            @throw e;
        
        if ( !shouldCreateNewProduct() )
            @throw e;
        
        if ( !shouldModifyProduct() )
            @throw e;
      
        if ( !shouldProcessServerErrors() )
            @throw e;
        
        if ( !shouldDeleteAllTestProduct() )
            @throw e;
        
        
        
        if ( !shouldCreateNewProductWithCustomers() )
            @throw e;
        
        if ( !shouldPerfomanceTest_create(100) )
            @throw e;
        
        if ( !shouldPerfomanceTest_delete() )
            @throw e;
        
    } @catch( NSException* e) {
        result = 0;
    }
    
    [customer release];
    [product release];
    [perftest release];
    [sclient release];
    
    return result;
}


int shouldSupportBlobType()
{
    NSString* blobUri = [[RhoConnectClient blobFolder] stringByAppendingString: @"MyText123.txt"];
    NSString* blobContent = [@"Blob test at " stringByAppendingString: [[NSDate date] description]];
    NSString* fileName = [RhoConnectClient pathForBlob: blobUri];
    
    NSError* error;
    
    //  verify file file doesn't exist
    if ([fileManager fileExistsAtPath: fileName]) {
        [fileManager removeItemAtPath: fileName error: &error];
    }
    if ([fileManager fileExistsAtPath: fileName])
        return 0;
    
    // create file with content
    [blobContent writeToFile: fileName atomically: YES encoding: NSUTF8StringEncoding error: &error];
    if ( ![fileManager fileExistsAtPath: fileName] )
        return 0;
    
    // create item with blob attribute value
    NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
    [item setValue: [NSString stringWithFormat: @"%f", [[NSDate date] timeIntervalSince1970]] forKey:@"name"];	
    [item setValue: blobUri forKey:@"image_uri"];	
    [blobTest create: item];

    // blob attribute still contain the same value
    if ( ![blobUri isEqualToString: [item valueForKey: @"image_uri"]] )
        return 0;
    // file should still in place
    if ( ![fileManager fileExistsAtPath: fileName] )
        return 0;
    
    // ensure item has been destroyed
    [blobTest destroy: item];
    NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
    [cond setValue: [item objectForKey: @"name"] forKey:@"name"];							 
    NSMutableDictionary* item2 = [blobTest find_first: cond];
    [cond release];
    if ( item2 )
        return 0;
    
    // file should be destroyed too
    if ( [fileManager fileExistsAtPath: fileName] )
        return 0;
    
    [item release];

    return 1;
}

int shouldSyncBlobTest()
{
    // should be logged in
    if ( ![sclient is_logged_in] )
        return 0;
    
	RhoConnectNotify* res = [blobTest sync];
	int	nErr = res.error_code;
	if ( nErr!= RHO_ERR_NONE
        || NO == [@"ok" isEqualToString: res.status] ) {
		return 0;
	}
    
	return 1;
}

int shouldDeleteAllBlobTestObjects()
{
    // should be logged in
    if ( ![sclient is_logged_in] )
        return 0;
    
    // find all blob items
    NSMutableArray* items = [blobTest find_all:nil];	
    if ( !items )
        return 0;
    
    // check blob files for items and destroy them
    NSMutableDictionary* item;
    for( item in items ) {
        // blob file name for item
        NSString* fileName = [RhoConnectClient pathForBlob: [item objectForKey: @"image_uri"]];
        // file should be in place
        if ( ![fileManager fileExistsAtPath: fileName] )
            return 0;
        // item destroy
        [blobTest destroy: item];
        // file should be destroyed too
        if ( [fileManager fileExistsAtPath: fileName] )
            return 0;
    }
    
    // sync to push items destroy to the server
    RhoConnectNotify* res = [blobTest sync];
    int	nErr = res.error_code;
    if ( nErr!= RHO_ERR_NONE
        || NO == [@"ok" isEqualToString: res.status] ) {
        return 0;
    }
    
    // there shuld be no items anymore
    item = [blobTest find_first: nil];
    if ( item )
        return 0;

    return 1;
}

int shouldCreateNewBlobTestObject()
{
    NSString* fileName = @"test.png";
    NSString* filePath = [RhoConnectClient pathForStorageFile: fileName];
    
    // should be logged in
    if ( ![sclient is_logged_in] )
        return 0;
    
    // copy test.png file from bundle to blob directory
    [RhoConnectClient fromMainBundle: fileManager copyFile: fileName toStorage: fileName forceRemove: YES];
    if ( ![fileManager fileExistsAtPath: filePath] )
        return 0;
    
    NSError* err;
    NSNumber* fileSize = [[fileManager attributesOfItemAtPath: filePath error: &err] objectForKey: NSFileSize];
    NSData* fileContent = [NSData dataWithContentsOfFile: filePath];
    
    // create item with blob attribute value
    NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
    //[item setValue: [NSString stringWithFormat: @"%f", [[NSDate date] timeIntervalSince1970]] forKey:@"name"];	
    [item setValue: fileName forKey:@"image_uri"];
    [blobTest create: item];
    [blobTest save: item];
    
    // sync to push items to the server
    RhoConnectNotify* res = [blobTest sync];
    int	nErr = res.error_code;
    if ( nErr!= RHO_ERR_NONE
        || NO == [@"ok" isEqualToString: res.status] ) {
        return 0;
    }
    
    // find all blob items, there should be exact one
    NSMutableArray* items = [blobTest find_all:nil];	
    if ( !items || 1 != [items count])
        return 0;
    
    // the only item should have image_uri changed
    NSString* newFileName = [[items objectAtIndex: 0] objectForKey:@"image_uri"];
    if ( [fileName isEqualToString: newFileName] )
        return 0;
    
    // but file size should be the same
    NSString* newFilePath = [RhoConnectClient pathForStorageFile: newFileName];
    NSNumber* newFileSize = [[fileManager attributesOfItemAtPath: newFilePath error: &err] objectForKey: NSFileSize];
    if ( ![newFileSize isEqualToNumber: fileSize] )
        return 0;
    
    // file content should be the same too
    NSData* newFileContent = [NSData dataWithContentsOfFile: newFilePath];
    if ( ![newFileContent isEqualToData: fileContent] )
        return 0;
    
    [item release];

    return 1;
}

int shouldModifyBlobTestObject()
{
    NSString* fileName = @"test2.png";
    NSString* filePath = [RhoConnectClient pathForStorageFile: fileName];
    
    // should be logged in
    if ( ![sclient is_logged_in] )
        return 0;
    
    // at least one item should exist
    NSMutableDictionary* item = [blobTest find_first: nil];
    if ( !item )
        return 0;
    NSString* savedId = [item objectForKey: @"object"];

    // copy test2.png file from bundle to blob directory
    [RhoConnectClient fromMainBundle: fileManager copyFile: fileName toStorage: fileName forceRemove: YES];
    if ( ![fileManager fileExistsAtPath: filePath] )
        return 0;
    
    // save old and set new file value
    NSString* oldFileName = [item objectForKey: @"image_uri"];
    [item setObject: fileName forKey: @"image_uri"];
    [blobTest save: item];
    
    // old file should be removed
    if ( [fileManager fileExistsAtPath: [RhoConnectClient pathForStorageFile: oldFileName]] )
        return 0;
    
    // sync to push items to the server
    RhoConnectNotify* res = [blobTest sync];
    int	nErr = res.error_code;
    if ( nErr!= RHO_ERR_NONE
        || NO == [@"ok" isEqualToString: res.status] ) {
        return 0;
    }
    
    // readn item back, it should still present
    NSMutableDictionary* item2 = [blobTest find: savedId];
    if ( !item2 )
        return 0;
    
    // file name should keep new value
    if ( ![fileName isEqualToString: [item2 objectForKey: @"image_uri"]] )
        return 0;
    
    return 1;
}

int runObjCClientBlobTest()
{
	blobTest = [[RhomModel alloc] init];
	blobTest.name = @"BlobTest";
    // BlobTest is not very good class name for the model, but
    // it should be the same as the Source name on server side
    [blobTest add_blob_attribute: @"image_uri"];
	
	[RhoConnectClient initDatabase];
	sclient = [[RhoConnectClient alloc] init];
    NSMutableArray* models = [NSMutableArray arrayWithObjects: blobTest, nil];	
	[sclient addModels:models];
	
    //sclient.threaded_mode = FALSE;
	//sclient.poll_interval = 0;
    [sclient setLogSeverity:1];
	
    // exception to throw on test failure
    NSException *e = [NSException
                      exceptionWithName: @"NSException"
                      reason: @"test faled"
                      userInfo: nil];
    int result = 1;
    @try {
        if ( !ResetAndLogout() )
            @throw e;
        
        sclient.sync_server = @"http://rhodes-samples-server.heroku.com/application";
        
        if ( !shouldNotSyncWithoutLogin() )
            @throw e;
        
        if ( !shouldLogin() )
            @throw e;
        
        if ( !shouldSupportBlobType() )
            @throw e;
        
        if( !shouldSyncBlobTest() )
            @throw e;
        
        if( !shouldDeleteAllBlobTestObjects() )
            @throw e;
        
        if( !shouldCreateNewBlobTestObject() )
            @throw e;
        
        if( !shouldModifyBlobTestObject() )
            @throw e;
        
    } @catch( NSException* e) {
        result = 0;
    }
    
    [blobTest release];
    [sclient release];
    
    return result;
}

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    fileManager = [NSFileManager defaultManager];
	
	int retVal = runObjCClientTest();
    if (0 < retVal)
        ;//retVal = runObjCClientBlobTest();
	
	if (retVal)
		NSLog(@"SUCCESS");
	else
		NSLog(@"FAILURE");
	
    [pool drain];

    return retVal;
}
