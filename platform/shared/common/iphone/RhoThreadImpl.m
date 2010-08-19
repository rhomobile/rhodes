//
//  RhoFileImpl.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 09.10.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#endif

#define ZIP_STD 1

void* rho_nativethread_start()
{
	return [[NSAutoreleasePool alloc] init];
}

void rho_nativethread_end(void* pData)
{
    NSAutoreleasePool *pool = (NSAutoreleasePool *)pData;	
    [pool release];	
}
