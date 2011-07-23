//
//  RootViewController.h
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RootViewController : UITableViewController {
	NSObject* arItems;
	NSString *CurrentTitle;
	NSInteger CurrentLevel;
}

@property (nonatomic, retain) NSObject *arItems;
@property (nonatomic, retain) NSString *CurrentTitle;
@property (nonatomic, readwrite) NSInteger CurrentLevel;

-(UITableViewCell *)reuseTableViewCellWithIdentifier:(NSString *)identifier;

@end
