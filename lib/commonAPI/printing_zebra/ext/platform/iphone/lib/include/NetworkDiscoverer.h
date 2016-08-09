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
 * A class used to discover printers on an IP network.
 */
@interface NetworkDiscoverer : NSObject {

}

/**
 * Sends a multicast discovery packet to each adapter's subnet.
 *
 * @param hops Number of hops.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)multicastWithHops:(NSInteger)hops error:(NSError **)error;

/**
 * Sends a multicast discovery packet to each adapter's subnet.
 * This method will wait up to \a waitForResponsesTimeout milliseconds before determining that there are no more discovery responses.
 * If \a waitForResponsesTimeout is less than 0 it will be set to the default timeout.
 * @param hops Number of hops.
 * @param waitForResponsesTimeout Time to wait, in milliseconds, before determining that there are no more discovery responses.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)multicastWithHops:(NSInteger)hops andWaitForResponsesTimeout:(NSInteger)waitForResponsesTimeout error:(NSError **)error;

/**
 * Sends a discovery packet to the IPs specified in the \a subnetRange.
 * Subnet searches are defined by the first three subnet octets, followed by a range, such as 192.168.2.* \n
 * This method accepts IP addresses of the form, assuming a subnet of 192.168.2:
 * \li 192.168.2.254 (will send a discovery packet to 192.168.2.254)
 * \li 192.168.2.* (will send a discovery packet for the range 192.168.2.1 - 192.168.2.254)
 * \li 192.168.2.8-* (will send a discovery packet for the range 192.168.2.8 - 192.168.2.254)
 * \li 192.168.2.37-42 (will send a discovery packet for the range 192.168.2.37 - 192.168.2.42)
 * @param subnetRange The subnet search range.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)subnetSearchWithRange:(NSString *)subnetRange error:(NSError **)error;

/**
 * Sends a discovery packet to the IPs specified in the \a subnetRange.
 * This method will wait up to \a waitForResponsesTimeout milliseconds before determining that there are no more discovery responses.
 * If \a waitForResponsesTimeout is less than 0 it will be set to the default timeout.
 * Subnet searches are defined by the first three subnet octets, followed by a range, such as 192.168.2.* \n
 * This method accepts IP addresses of the form, assuming a subnet of 192.168.2:
 * \li 192.168.2.254 (will send a discovery packet to 192.168.2.254)
 * \li 192.168.2.* (will send a discovery packet for the range 192.168.2.1 - 192.168.2.254)
 * \li 192.168.2.8-* (will send a discovery packet for the range 192.168.2.8 - 192.168.2.254)
 * \li 192.168.2.37-42 (will send a discovery packet for the range 192.168.2.37 - 192.168.2.42)
 *
 * @param subnetRange The subnet search range.
 * @param waitForResponsesTimeout Time to wait, in milliseconds, before determining that there are no more discovery responses.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)subnetSearchWithRange:(NSString*)subnetRange andWaitForResponsesTimeout:(NSInteger)waitForResponsesTimeout error:(NSError **)error;

/**
 * Sends a directed broadcast discovery packet to the subnet specified by \a ipAddress.
 * Directed broadcasts are defined by the first three subnet octets, followed by 255, such as 192.168.2.255. \n
 * This method accepts IP addresses of the form, assuming a subnet of 192.168.2:
 * \li 192.168.2.255
 * \li 192.168.2.1 (last octect will be replaced with 255)
 * \li 192.168.2 (will append 255 for the last octect)
 * \li 192.168.2. (will append 255 for the last octect)
 * \remark Directed broadcasts are defined by the first three subnet octets, followed by 255, such as 192.168.2.255.
 * @param ipAddress The IP address of the subnet.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)directedBroadcastWithIpAddress:(NSString *)ipAddress error:(NSError **)error;

/**
 * Sends a directed broadcast discovery packet to the subnet specified by \a ipAddress.
 * This method will wait up to \a waitForResponsesTimeout milliseconds before determining that there are no more discovery responses.
 * If \a waitForResponsesTimeout is less than 0 it will be set to the default timeout.
 * Directed broadcasts are defined by the first three subnet octets, followed by 255, such as 192.168.2.255. \n
 * This method accepts IP addresses of the form, assuming a subnet of 192.168.2:
 * \li 192.168.2.255
 * \li 192.168.2.1 (last octect will be replaced with 255)
 * \li 192.168.2 (will append 255 for the last octect)
 * \li 192.168.2. (will append 255 for the last octect)
 * \remark Directed broadcasts are defined by the first three subnet octets, followed by 255, such as 192.168.2.255.
 * @param ipAddress The IP address of the subnet.
 * @param waitForResponsesTimeout Time to wait, in milliseconds, before determining that there are no more discovery responses.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)directedBroadcastWithIpAddress:(NSString *)ipAddress andWaitForResponsesTimeout:(NSInteger)waitForResponsesTimeout error:(NSError **)error;

/**
 * Sends a local broadcast discovery packet to each adapter's subnet.
 *
 * @param error <code>nil</code> if there is no error or a description of the error if one occurs.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)localBroadcast:(NSError **)error;

/**
 * Sends a local broadcast discovery packet to each adapter's subnet.
 * This method will wait up to \a waitForResponsesTimeout milliseconds before determining that there are no more discovery responses.
 * If \a waitForResponsesTimeout is less than 0 it will be set to the default timeout.
 *
 * @param waitForResponsesTimeout Time to wait, in milliseconds, before determining that there are no more discovery responses.
 * @param error Will be set to the error that occured.
 * @return An array of <c>DiscoveredPrinter</c> objects.
 */
+(NSArray *)localBroadcastWithTimeout:(NSInteger)waitForResponsesTimeout error:(NSError **)error;

@end
