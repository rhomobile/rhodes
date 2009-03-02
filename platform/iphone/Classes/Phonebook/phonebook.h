/*
 *  phonebook.h
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#import <UIKit/UIKit.h>
#import <AddressBook/AddressBook.h>

typedef struct __LocalPhonebook {
	ABAddressBookRef _ab;
	CFArrayRef		 _people;
	CFIndex			 _index;
	CFIndex			 _len;
} LocalPhonebook;
