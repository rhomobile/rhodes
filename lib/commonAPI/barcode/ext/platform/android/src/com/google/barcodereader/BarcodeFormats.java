package com.google.barcodereader;

import java.util.HashMap;
import java.util.ArrayList;
import com.google.android.gms.vision.barcode.Barcode;

public final class BarcodeFormats{
	public static final String ALL_FORMATS 	= "allDecoders";	//Barcode format constant representing the union of all supported formats.
    public static final String AZTEC 		= "aztec";			//Barcode format constant for AZTEC.
    public static final String CODABAR 		= "codabar"; 		//Barcode format constant for Codabar.
    public static final String CODE_128 	= "code128"; 		//Barcode format constant for Code 128.
    public static final String CODE_39 		= "code39"; 		//Barcode format constant for Code 39.
    public static final String CODE_93 		= "code93"; 		//Barcode format constant for Code 93.
    public static final String DATA_MATRIX 	= "dataMatrix"; 	//Barcode format constant for Data Matrix.
    public static final String EAN_13 		= "ean13"; 		    //Barcode format constant for EAN-13.
    public static final String EAN_8 		= "ean8"; 			//Barcode format constant for EAN-8.
    public static final String ISBN  		= "isbn"; 			//Barcode value format constant for ISBNs.
    public static final String ITF 			= "itf"; 			//Barcode format constant for ITF (Interleaved Two-of-Five).
    public static final String PDF417 		= "pdf417"; 		//Barcode format constant for PDF-417.
    public static final String QR_CODE 		= "qrCode"; 		//Barcode format constant for QR Code.
    public static final String UPC_A 		= "upca"; 			//Barcode format constant for UPC-A.
    public static final String UPC_E 		= "upce"; 			//Barcode format constant for UPC-E.

    public static int getBarcodeFormat(String format){ 
		if (format.equals(ALL_FORMATS)) return Barcode.ALL_FORMATS;
	    if (format.equals(AZTEC)) 		return Barcode.AZTEC;
	    if (format.equals(CODABAR)) 	return Barcode.CODABAR;
	    if (format.equals(CODE_128)) 	return Barcode.CODE_128;
	    if (format.equals(CODE_39)) 	return Barcode.CODE_39;
	    if (format.equals(CODE_93))		return Barcode.CODE_93;
	    if (format.equals(DATA_MATRIX)) return Barcode.DATA_MATRIX;
	    if (format.equals(EAN_13)) 		return Barcode.EAN_13;
	    if (format.equals(EAN_8)) 		return Barcode.EAN_8;
	    if (format.equals(ISBN )) 		return Barcode.ISBN;
	    if (format.equals(ITF)) 		return Barcode.ITF;
	    if (format.equals(PDF417)) 		return Barcode.PDF417;
	    if (format.equals(QR_CODE))     return Barcode.QR_CODE;
	    if (format.equals(UPC_A)) 		return Barcode.UPC_A;
	    if (format.equals(UPC_E)) 		return Barcode.UPC_E;
    	return Barcode.ALL_FORMATS;
    }

    public static ArrayList<String> getFormats(){
        ArrayList<String> result = new ArrayList<String>();

        result.add(ALL_FORMATS);
        result.add(AZTEC);
        result.add(CODABAR);
        result.add(CODE_128);
        result.add(CODE_39);
        result.add(CODE_93);
        result.add(DATA_MATRIX);
        result.add(EAN_13);
        result.add(EAN_8);
        result.add(ISBN);
        result.add(ITF);
        result.add(PDF417);
        result.add(QR_CODE);
        result.add(UPC_A);
        result.add(UPC_E); 

        return result;
    }
}