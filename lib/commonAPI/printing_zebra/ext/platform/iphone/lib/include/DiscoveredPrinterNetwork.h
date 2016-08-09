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

#import "DiscoveredPrinter.h"

/**
 * Instance of <c>DiscoveredPrinter</c> that is returned when performing a network discovery.
 */
@interface DiscoveredPrinterNetwork : DiscoveredPrinter {
	@private NSInteger port;
	@private NSString *dnsName;
}

/**
 * Printing port.
 */
@property (readonly) NSInteger port;

/**
 * DNS name.
 */
@property (nonatomic, copy) NSString *dnsName;


/**
 * Initializes a new instance of the <c>DiscoveredPrinterNetwork</c> class with \a anAddress and \a aPort.
 *
 * @param anAddress IP address of the printer.
 * @param aPort The printing port.
 */
-(id)initWithAddress:(NSString *)anAddress andWithPort:(NSInteger)aPort;

/**
 * Returns the IP address of the discovered printer.
 * @return the IP address.
 */
- (NSString*)toString;


@end
