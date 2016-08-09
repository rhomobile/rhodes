/**********************************************
 * CONFIDENTIAL AND PROPRIETARY
 *  
 * The information contained herein is the confidential and the exclusive property of
 * ZIH Corp. This document, and the information contained herein, shall not be copied, reproduced, published,
 * displayed or distributed, in whole or in part, in any medium, by any means, for any purpose without the express
 * written consent of ZIH Corp. 
 * 
 * Copyright ZIH Corp. 2012 
 * 
 * ALL RIGHTS RESERVED
 ***********************************************/

/**
 * Container holding information about a discovered printer.
 */
@interface DiscoveredPrinter : NSObject {
@private NSString *address;
}

/**
 * IP address of printer.
 */
@property (readonly) NSString* address;


/**
 * Creates an object holding information about a discovered printer.
 * @param anAddress IP address of the discovered printer.
 */
- (id)initWithAddress:(NSString *)anAddress;

/**
 * Returns the IP address of the discovered printer.
 * @return the IP address.
 */
- (NSString*)toString;

@end
