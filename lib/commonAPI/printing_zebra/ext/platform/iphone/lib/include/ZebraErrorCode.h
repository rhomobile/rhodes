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
 * Enumeration of the various error codes which can be returned by the Zebra API.
 */
@interface ZebraErrorCodeI : NSObject {

}

/*! \public
 * Enumeration of the various error codes which can be returned by the Zebra API.
 */
typedef enum {
	/**
	 * Unable to create a connection to a printer.
	 */
	ZEBRA_ERROR_NO_CONNECTION,
	/**
	 * Write to a connection failed.
	 */
	ZEBRA_ERROR_WRITE_FAILURE,
	/**
	 * Read from a connection failed.
	 */
	ZEBRA_ERROR_READ_FAILURE,
	/**
	 * Unable to determine the control language of a printer.
	 */
	ZEBRA_UNKNOWN_PRINTER_LANGUAGE,
	/**
	 * Malformed discovery address
	 */
	ZEBRA_MALFORMED_NETWORK_DISCOVERY_ADDRESS,
	/**
	 * Network error during discovery
	 */
	ZEBRA_NETWORK_ERROR_DURING_DISCOVERY,
	/**
	 * Invalid multicast hop count
	 */
	ZEBRA_INVALID_DISCOVERY_HOP_COUNT,
	/**
	 * Malformed status response - unable to determine printer status
	 */
	ZEBRA_MALFORMED_PRINTER_STATUS_RESPONSE,
	/**
	 * Invalid format name
	 */
	ZEBRA_INVALID_FORMAT_NAME,
	/**
	 * Bad file directory entry
	 */
	ZEBRA_BAD_FILE_DIRECTORY_ENTRY,
	/**
	 * '^FN' integer must be between 1 and 9999
	 */
	ZEBRA_MALFORMED_FORMAT_FIELD_NUMBER,
	/**
	 * Invalid printer language
	 */
	ZEBRA_INVALID_PRINTER_LANGUAGE,
	/**
	 * Invalid file name
	 */
	ZEBRA_INVALID_FILE_NAME,
	/**
	 * Invalid drive specified
	 */
	ZEBRA_INVALID_PRINTER_DRIVE_LETTER
	
} ZebraErrorCode;

@end
