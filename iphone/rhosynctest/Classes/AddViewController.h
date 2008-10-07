/*
 *  AddViewController.h
 *  RhoSyncClient
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#import <UIKit/UIKit.h>
#import "DetailViewController.h"
#import "rhosynctestappdelegate.h"
#import "AppDelegateMethods.h"
#import "SyncObjectWrapper.h"


@interface AddViewController : UIViewController {
	SyncObjectWrapper *newObjectValue;
	SyncObjectWrapper *newObjectIndustry;
	IBOutlet UITextField *newValue;
	IBOutlet UITextField *newIndustry;
}

@property (nonatomic, assign)SyncObjectWrapper *newObjectValue;
@property (nonatomic, assign)SyncObjectWrapper *newObjectIndustry;
	
- (IBAction)cancel:(id)sender;
- (IBAction)save:(id)sender;

@end
