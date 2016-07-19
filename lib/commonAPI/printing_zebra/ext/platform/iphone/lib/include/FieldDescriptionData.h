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
 *  A class used to describe format variable fields.
 * 
 *  In CPCL, the description of the following format will contain 2 <code>FieldDescriptionData</code> objects.
 *  \code
! DF SHELF.FMT
! 0 200 200 210 1
CENTER
TEXT 4 3 0 15 \\
TEXT 4 0 0 95 \\
FORM
PRINT
 *  \endcode
 *  
 *  The first <c>FieldDescriptionData</c> will contain a <c>fieldNumber</c> of 1, and a <c>fieldName</c> of
 *  nil. The second <c>FieldDescriptionData</c> will contain a <c>fieldNumber</c> of 2, and a
 *  <c>fieldName</c> of nil.<br/><br/>
 * 
 *  In ZPL, the description of the following ^FN command will contain 2 <c>FieldDescriptionData</c> objects.
 *  
 *  \code
^XA
^DFR:SHELF.ZPL^FS
^FO25,25^A0N,50,50^FN15"Name"^FS
^FO25,75^A0N,50,50^FN15"Address"^FS
^FO25,125^A0N,50,50^FN18^FS
^FO25,175^A0N,50,50^FN15
^XZ
 *  \endcode
 *  
 *  The first <c>FieldDescriptionData</c> will contain a <c>fieldNumber</c> of 15, and a <c>fieldName</c> of
 *  "Address". The second <c>FieldDescriptionData</c> will contain a <c>fieldNumber</c> of 18, and a
 *  <c>fieldName</c> of nil.
 * 
 *  <b>Note:</b> If a ZPL label format contains multiple field numbers that use the same identifier, only 1 will be returned since the data will be shared
 *  by all variables with the number. The <c>fieldName</c> portion of the variable will be the
 *  last one in the format, unless it is not present. For example, in the format above, there are 3 ^FN15's. The
 *  first 2 have a "prompt" parameter, the third does not. The second one, "Address", overwrites the first one, "Name".
 *  The third one is not present, so the previous one, "Address", is preserved.
 */
@interface FieldDescriptionData : NSObject {

/**
 * In CPCL, this number will be the sequential number of the variable field in the format.
 * In ZPL, this number will correspond to the ^FN number.
 */
@private NSNumber *fieldNumber;

/**
 * In CPCL, this field is always nil.
 * In ZPL, this field will correspond to the optional name parameter of the ^FN command, or nil if the parameter is
 * not present.
 */
@private NSString *fieldName;
}

/**
 * Initializes a new instance of the FieldDescriptionData class.
 * 
 * @param aFieldNumber The number of the field.
 * @param aFieldName The name of the field, or nil if not present.
 */
-(id)initWithFieldNumber:(NSNumber*)aFieldNumber andWithFieldName:(NSString*)aFieldName;

@property (readonly) NSNumber *fieldNumber;
@property (readonly) NSString *fieldName;

@end
