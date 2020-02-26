package com.google.barcodereader;

import java.util.HashMap;
import com.google.android.gms.vision.barcode.Barcode;

public final class BarcodeFormats{
	public static final String ALL_FORMATS 	= "ALL_FORMATS";	//Barcode format constant representing the union of all supported formats.
    public static final String AZTEC 		= "AZTEC";			//Barcode format constant for AZTEC.
    public static final String CODABAR 		= "CODABAR"; 		//Barcode format constant for Codabar.
    public static final String CODE_128 	= "CODE_128"; 		//Barcode format constant for Code 128.
    public static final String CODE_39 		= "CODE_39"; 		//Barcode format constant for Code 39.
    public static final String CODE_93 		= "CODE_93"; 		//Barcode format constant for Code 93.
    public static final String DATA_MATRIX 	= "DATA_MATRIX"; 	//Barcode format constant for Data Matrix.
    public static final String EAN_13 		= "EAN_13"; 		//Barcode format constant for EAN-13.
    public static final String EAN_8 		= "EAN_8"; 			//Barcode format constant for EAN-8.
    public static final String ISBN  		= "ISBN"; 			//Barcode value format constant for ISBNs.
    public static final String ITF 			= "ITF"; 			//Barcode format constant for ITF (Interleaved Two-of-Five).
    public static final String PDF417 		= "PDF417"; 		//Barcode format constant for PDF-417.
    public static final String QR_CODE 		= "QR_CODEB"; 		//Barcode format constant for QR Code.
    public static final String UPC_A 		= "UPC_A"; 			//Barcode format constant for UPC-A.
    public static final String UPC_E 		= "UPC_E"; 			//Barcode format constant for UPC-E.

    public static int getBarcode(String format){ 
		if (format.equals(ALL_FORMATS)) 	return Barcode.ALL_FORMATS;
	    if (format.equals(AZTEC)) 		return Barcode.AZTEC;
	    if (format.equals(CODABAR)) 		return Barcode.CODABAR;
	    if (format.equals(CODE_128)) 	return Barcode.CODE_128;
	    if (format.equals(CODE_39)) 		return Barcode.CODE_39;
	    if (format.equals(CODE_93))		return Barcode.CODE_93;
	    if (format.equals(DATA_MATRIX)) 	return Barcode.DATA_MATRIX;
	    if (format.equals(EAN_13)) 		return Barcode.EAN_13;
	    if (format.equals(EAN_8)) 		return Barcode.EAN_8;
	    if (format.equals(ISBN )) 		return Barcode.ISBN;
	    if (format.equals(ITF)) 			return Barcode.ITF;
	    if (format.equals(PDF417)) 		return Barcode.PDF417;
	    if (format.equals(QR_CODE)) 		return Barcode.QR_CODE;
	    if (format.equals(UPC_A)) 		return Barcode.UPC_A;
	    if (format.equals(UPC_E)) 		return Barcode.UPC_E;
    	return Barcode.ALL_FORMATS;
    }
}