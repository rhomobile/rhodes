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
 * A connection to a Zebra printer.
 */
@protocol ZebraPrinterConnection

/**
 * See the classes which implement this method for the format of the description string.
 * 
 * @return The connection description string.
 */
- (NSString *)toString;

/**
 * Returns the maximum time, in milliseconds, to wait for any data to be received.
 * 
 * @return The maximum time, in milliseconds, to wait for any data to be received.
 */
- (NSInteger) getMaxTimeoutForRead;

/**
 * Returns the maximum time, in milliseconds, to wait between reads after the initial read.
 * 
 * @return The maximum time, in milliseconds, to wait between reads after the initial read.
 */
- (NSInteger) getTimeToWaitForMoreData;

/**
 * Returns <c>YES</c> if the connection is open.
 * 
 * @return <c>YES</c> if this connection is open.
 */
- (BOOL) isConnected;

/**
 * Opens the connection to a device. If the ZebraPrinterConnection::open method is called on an open connection 
 * this call is ignored. When a handle to the connection is no longer needed, call ZebraPrinterConnection::close
 * to free up system resources.
 * 
 * @return <c>NO</c> if the connection cannot be established.
 */
- (BOOL) open;

/**
 * Closes this connection and releases any system resources associated with the connection. If the connection is
 * already closed then invoking this method has no effect.
 */
- (void) close;

/**
 * Writes the number of bytes from <c>data</c> to the connection. The connection must be
 * open before this method is called. If ZebraPrinterConnection::write:error: is called when a connection is closed, -1 is returned.
 * 
 * @param data The data.
 * @param error Will be set to the error that occured.
 * @return The number of bytes written or -1 if an error occurred.
 */
- (NSInteger) write:(NSData *)data error:(NSError **)error;

/**
 * Reads all the available data from the connection. This call is non-blocking.
 * 
 * @param error Will be set to the error that occured.
 * @return The bytes read from the connection or <c>nil</c> if an error occurred.
 */
- (NSData *)read: (NSError**)error;

/**
 * Returns <c>YES</c> if at least one byte is available for reading from this connection.
 * 
 * @return <c>YES</c> if there is data avaiilable.
 */
- (BOOL) hasBytesAvailable;

/**
 * Causes the currently executing thread to sleep until <c>hasBytesAvailable</c> equals <c>YES</c>, or for a maximum of
 * <c>maxTimeout</c> milliseconds.
 * 
 * @param maxTimeout Maximum time in milliseconds to wait for an initial response from the printer.
 */
- (void) waitForData: (NSInteger)maxTimeout;

@end



/*! \mainpage Zebra API
 *  Provides classes for interfacing with Zebra printers from an Apple&reg; mobile digital device.<br /><br />
 *	<b>I want to...</b>
 *	\li <a href="libToProj.html">Add ZSDK_API.a to my development environment project</a>
 *	\li \link TcpPrinterConnection Print over TCP/IP\endlink
 *	\li \link MfiBtPrinterConnection Print over Bluetooth\endlink
 *	\li \link FormatUtil Create and print formats\endlink
 *	\li \link FileUtil Send files to the printer\endlink
 *	\li \link PrinterStatus Query printer status\endlink
 *	\li \link GraphicsUtil Print graphics\endlink
 *	\li \link MagCardReader Read a magnetic stripe\endlink
 *	\li \link SmartCardReader Read a smart card\endlink
 *	\li <a href="http://www.zebra.com/link">Find more information</a>
 *
 *	<b>Tips for developing with this API...</b>
 *	<ol>
 *	<li>As a best pracitce, Zebra recommends not making calls to our API from the GUI thread. Use <a href="http://developer.apple.com/library/ios/#documentation/Performance/Reference/GCD_libdispatch_Ref/Reference/reference.html">Grand Central Dispatch</a> to accomplish this.</li>
 *	<li>Only Mobile printers are available with built-in magnetic card readers</li>
 *	<li>Each ZebraPrinterConnection object should only be used on a single thread</li>
 *	</ol>
 *
 *  <b>Project Settings and Configuration</b>
 *  <ul>
 *      <li>The API library was built using XCode 6</li>
 *      <li>Minimum iOS SDK is 8</li>
 *      <li>Valid architectures are armv7 and arm64</li>
 *      <li>Code is compiled with LLVM 6</li>
 *      <li>Library compiled using Manual Reference Counting</li>
 *      <li>If you wish to communicate to a <b>Made For iPhone</b> Zebra printer using Bluetooth, you need to:</li>
 *      <ul>
 *          <li>Link your project to the <b>ExternalAccessory</b> framework</li>
 *          <li>Add the proper protocol string to your plist file; <b>com.zebra.rawport</b></li>
 *          <li>Set the key <b>Required Background modes</b> to <b>App Communicates with an accessory</b> in your app's plist file</li>
 *          <li>To submit an App to the App Store which uses the MFi Zebra printer, <a href="http://www.zebra.com/link">follow the instructions here</a></li>
 *      </ul>
 *      <li>Please supply your project's settings and configuration information if you need to <a href="http://www.zebra.com/us/en/about-zebra/contact-zebra/contact-tech-support.html">contact Tech Support</a></li>
 *  </ul>
 */







