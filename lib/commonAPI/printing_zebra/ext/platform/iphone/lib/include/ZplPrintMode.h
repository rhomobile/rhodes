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
 * Enumeration of the various print modes supported by Zebra printers.
 */
@interface ZplPrintModeI : NSObject
{
}

/*! \public 
 * Enumeration of the various print modes supported by Zebra printers.
 */
typedef enum {
	
	/**
     * Rewind print mode
     */
	ZPL_PRINT_MODE_REWIND,
	
    /**
     * Peel-off print mode
     */
	ZPL_PRINT_MODE_PEEL_OFF,
	
    /**
     * Tear-off print mode (this also implies Linerless Tear print mode)
     */
	ZPL_PRINT_MODE_TEAR_OFF,
	
    /**
     * Cutter print mode
     */
	ZPL_PRINT_MODE_CUTTER,
	
    /**
     * Applicator print mode
     */
	ZPL_PRINT_MODE_APPLICATOR,
	
    /**
     * Delayed cut print mode
     */
	ZPL_PRINT_MODE_DELAYED_CUT,
	
    /**
     * Linerless peel print mode
     */
	ZPL_PRINT_MODE_LINERLESS_PEEL,
	
    /**
     * Linerless rewind print mode
     */
	ZPL_PRINT_MODE_LINERLESS_REWIND,
	
    /**
     * Partial cutter print mode
     */
	ZPL_PRINT_MODE_PARTIAL_CUTTER,
	
    /**
     * RFID print mode
     */
	ZPL_PRINT_MODE_RFID,
	
    /**
     * Kiosk print mode
     */
	ZPL_PRINT_MODE_KIOSK,
	
    /**
     * Unknown print mode
     */
	ZPL_PRINT_MODE_UNKNOWN
	
} ZplPrintMode;

@end

