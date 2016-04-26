/**
 * \file Scanner.h
 * \brief File containing the CScanner definitions.
*  \author	Darryn Campbell (DCC)
*  \date	January 2012 (DCC, Initial Port from RhoElements 1 Code)
 */
#pragma once

#include "KeyInjector.h"
#include "ScannerDefines.h"
#include "DecodeNotifier.h"
#include <map>

/**
*  Structure defining comparison operator given to map 
*  functions metaTagSetFunctions and metaTagGetFunctions.
*/
struct wcharCmp
{
	/**
	*  Comparison operator given to map 
	*  functions metaTagSetFunctions and metaTagGetFunctions.
	*/
	bool operator()(const WCHAR* s1, const WCHAR* s2) const
	{
		return wcsicmp(s1, s2) < 0;
	}
}; 

/**
*  Structure defining comparison operator given to map
*  'decoderStrings'
*/
struct byteCmp
{
	/**
	*  Comparison operator given to 'decoderStrings' map
	*/
	bool operator()(const BYTE b1, const BYTE b2) const
	{
		return b1 < b2;
	}
};

/**
*  Class to manage a Scanner via the EMDK for C interface.  This is a fairly thin client, 
*  abstracting the C interface away from the user and presenting a much friendlier EMML
*  interface.  The significant improvements over Browser are that you no longer
*  have to disable / reenable the scanner after setting decoders and the Scanner now offers
*  support for the full EMDK interface.  RhoElements version two introduced a configurable
*  buffer type (data / text) and should be providing the ability to retrieve current
*  values for properties / decoders / decoder properties.
*  \author	Darryn Campbell (DCC)
*  \date	January 2012 (DCC, Initial Port from RhoElements 1 Code)
*/
class CScanner
{
public:
	/**
	*  Initialise all Scanner variables
	*  \param iInstanceID The identifier of this class provided by the framework for
	*         use in a multi instance environment.
	*  \param pModule Additional structure required to interface with RhoElements framework
	*/
	CScanner(int iInstanceID, PBModule* pModule);

	/**
	*  Free all memory associated with the Scanner
	*/
	~CScanner();

	/**
	*  Perform memory allocation and initialisation that was not done in the constructor.
	*  \return FALSE if some error occured during initialisation (perhaps if insufficient memory
	*          available or the EMDK scanner could not be loaded)
	*/
	BOOL Initialise(bool bLaunchingAppHasFocus);

	////////////////////////////////
	//  Abstraction of Framework
	////////////////////////////////

	/**
	*  Log a scanner message in line with the framework logging mechanism
	*  \param logSeverity The severity of the Log, Error, warning and Info
	*         are defined in ScannerDefines.h
	*  \param pLogComment The string data to log
	*  \param pFunctionName The name of the function from where the log originated
	*  \param dwLineNumber The Line number from where the log originated.
	*/
	BOOL Log(LogTypeInterface logSeverity, LPCTSTR pLogComment, 
		   LPCTSTR pFunctionName, DWORD dwLineNumber);

	/**
	*  Clears out event strings and disables Scanner when a page 
	*  navigation occurs.
	*  \return TRUE
	*/
	BOOL BeforeNavigate();

	/**
	*  When the page load has completed, all meta tags received during the 
	*  navigation can be sent to the scanner (they are all queued during 
	*  navigation)
	*/
	BOOL DocumentComplete();

	/**
	*   An Application focus change is received when this Scanner instance either
	*  goes to the background (bActivated False) or comes to the foreground
	*  (bActivated True).
	*/
	BOOL ApplicationFocusChange(bool bActivated);

	/**
	*   This function checks whether the scanner functionality to be disabled in
	*	application. Internally this function reads the value of DisableScannerInApp
	*	elements from config.xml. The value is either 0 or 1.
	*	This logic does not effect the scanner functionality on license screen page.
	*	Irrespective of whatever value mentioned in config.xml, on license screen 
	*	page the scanner functionality is always enabled.
	*	Value: 1 in config.xml, is used for disabling the scanner functionality
	*	Value: 0 in config.xml, is used for not disabling the scanner functionality
	*	Return Value:
	*	TRUE - If scanner functionality need to be disabled
	*	FALSE - If scanner functionality to be disabled
	*/
	BOOL NotScannerDisabledInApplication();

	/**
	*  Top level function for processing a received Meta Tag (set).  If a navigation
	*  is in progress or the scanner is disabled (and this property can not
	*  be set unless the scanner is enabled) the property is queued up
	*  to be processed later.  If the property can be processed now
	*  the appropriate method is called (from the hash map) to process
	*  the property.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value to set the property to
	*  \return Whether or not the property was successfully set (or successfully
	*          queued)
	*/
	BOOL ParseMETATag(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/**
	*  Not yet implemented.  This function has been set aside to retrieve
	*  the values of properties from the scanner.
	*  \param szParameterName The name of the property to get
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve
	*         the length being returned.
	*  \return The length of the value being returned (including the terminating
	*          NULL character)
	*/
	int RetrieveEMMLTag(LPCTSTR szParameterName, WCHAR* szParameterValue);

private:
	/////////////////////////////
	//  EMML Tag Handlers (Set)
	//  These functions set the appropriate scanner properties to the specified
	//  values through the EMDK interface with the Scanner.  Implemented
	//  in SetScannerProperties.cpp and RSM.cpp
	/////////////////////////////

	/**
	*  EMML Parser for top level Scanner tags such as event declaration, 
	*  start / stop and enabled / disabled.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value of the property being set
	*  \return Whether or not the property was successfully set
	*/
	BOOL ScannerTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/**
	*  EMML Parser for Scanner tags which were formerly configuration parameters 
	*  such as decodeVolume and decodeDuration.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value of the property being set
	*  \return Whether or not the property was successfully set
	*/
	BOOL ScannerBeeper_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/**
	*  EMML Parser for Reader settings such as raster mode and aim mode.  After
	*  each of these properties is set the scan buffer is flushed to ensure
	*  they take effect immediately.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value of the property being set
	*  \return Whether or not the property was successfully set
	*/
	BOOL ScannerReaderTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/**
	*  EMML Parser for Bluetooth related tags (not RSM), such as connection 
	*  idle timeout.  These relate to the SSI specific scanner interface.  These
	*  parameters may not take effect until the next SCAN_Enable.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value of the property being set
	*  \return Whether or not the property was successfully set
	*/
	BOOL ScannerBluetoothTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	

	/**
	*  EMML Parser for RSM tags such as MarkForRetrievalScanLineWidth and RSMGetEvent.
	*  This parser must be backwardly compatible with the Browser 3 technique
	*  for marking RSM parameters for retrieval as well as the RhoElements way.  The
	*  external interface has changed for RhoElements to avoid having methods / 
	*  properties with the same name.
	*  \param szPropertyName The name of the property to set
	*  \param szPropertyValue The value of the property being set
	*  \return Whether or not the property was successfully set
	*/
	BOOL rsm_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);

	/////////////////////////////////
	//  EMML Tag Handlers (Set Decoder and Decoder Properties).
	//  Defined in SetDecoderProperties.cpp
	////////////////////////////////
	BOOL SetSimpleDecoder			(BYTE cCurrentDecoder, LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for decoders which just have Enable / Disable
	BOOL Set_all_decoders_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for All_Decoders.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_pdf417_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for PDF417 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_Chinese_2Of5_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Chinese 2of5 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_AZTEC_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Aztec decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_MicroQR_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Micro QR decoder.  Parameters and return documentation match ScannerTag_MetaProc	
	BOOL Set_Korean_3Of5_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Korean 3of5 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_Us4State_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for US 4 State decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_webcode_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Webcode decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_uspostnet_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for US Postnet decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_usplanet_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for US Planet decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_ukpostal_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for UK Postal decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_tlc39_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for TLC39 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_signature_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Signature decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_rsslim_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for GS1 Databar Limited decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_rssexp_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for GS1 Databar expanded decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_rss14_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for GS1 Databar decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_qrcode_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for QR Code decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_micropdf_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Micro PDf decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_maxicode_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Maxicode decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_macropdf_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for MacroPDF decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_macromicropdf_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Macro Micro PDF decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_jappostal_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for JapPostal decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_ean13_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for EAN 13 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_dutchpostal_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Dutchpostal decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_datamatrix_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Datamatrix decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_composit_c_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Composite C decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_composit_ab_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Composite AB decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_canpostal_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for CanPostal decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_auspostal_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for AusPostal decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_upce1_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for UPCE 1 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_upce0_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for UPCE 0 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_upca_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for UPCA decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_upc_ean_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for UPC EAN decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_trioptic39_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Trioptic 39 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_msi_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for MSI decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_i2of5_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for I 2of 5 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_ean8_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for EAN 8 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_d2of5_MetaProc			(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for D 2of5 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_code93_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Code 93 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_code39_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Code 39 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_code11_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Code 11 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_code128_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Code 128 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_codabar_MetaProc		(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Codabar decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_matrix2of5_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for Matrix 2of5 decoder.  Parameters and return documentation match ScannerTag_MetaProc
	BOOL Set_Us4StateFics_MetaProc	(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);	///<  EMML Tag handler for US 4State FICS decoder.  Parameters and return documentation match ScannerTag_MetaProc

	/////////////////////////////
	//  EMML Tag Handlers (Get)
	//  These functions get the specified scanner properties.  Implemented
	//  in GetScannerProperties.cpp and RSM.cpp
	/////////////////////////////

	/**
	*  EMML Tag retrieval for top level Scanner tags such as event declaration, 
	*  and enabled / disabled.
	*  \param szPropertyName The name of the property to retrieve
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve the length being returned.
	*  \return The length of the value being returned (including the terminating NULL character)
	*/
	int ScannerTag_GetPropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);

	/**
	*  EMML Parser retrieval for Scanner tags which were formerly configuration parameters 
	*  such as decodeVolume and decodeDuration.
	*  \param szPropertyName The name of the property to get
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve the length being returned.
	*  \return The length of the value being returned (including the terminating NULL character)
	*/
	int ScannerBeeper_GetPropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);

	/**
	*  EMML Tag retrieval for Reader settings such as raster mode and aim mode.
	*  \param szPropertyName The name of the property to get
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve the length being returned.
	*  \return The length of the value being returned (including the terminating NULL character)
	*/
	int ScannerReaderTag_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue);

	/**
	*  EMML Tag retrieval for Bluetooth related tags (not RSM), such as connection 
	*  idle timeout.  These relate to the SSI specific scanner interface.
	*  \param szPropertyName The name of the property to get
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve the length being returned.
	*  \return The length of the value being returned (including the terminating NULL character)
	*/
	int ScannerBluetoothTag_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	

	/**
	*  EMML Tag retrieval for RSM tags such as MarkForRetrievalScanLineWidth and RSMGetEvent.
	*  \param szPropertyName The name of the property to get
	*  \param szParameterValue [out] Buffer into which the retrieved
	*         parameter's value is stored.  Specify NULL to just retrieve the length being returned.
	*  \return The length of the value being returned (including the terminating NULL character)
	*/
	int rsm_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue);

	/////////////////////////////////
	//  EMML Tag Handlers (Get Decoder and Decoder Properties).
	//  Defined in GetDecoderProperties.cpp
	////////////////////////////////
	int GetSimpleDecoder				(BYTE cDecoder, LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve properties for decoders which just have Enable / Disable.
	int Get_all_decoders_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: All_Decoders.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_pdf417_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: PDF417 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_Chinese_2Of5_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Chinese 2of5 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_AZTEC_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Aztec decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_MicroQR_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Micro QR decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue	
	int Get_Korean_3Of5_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Korean 3of5 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_Us4State_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: US 4 State decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_webcode_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Webcode decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_uspostnet_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: US Postnet decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_usplanet_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: US Planet decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_ukpostal_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: UK Postal decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_tlc39_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: TLC39 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_signature_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Signature decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_rsslim_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: GS1 Databar Limited decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_rssexp_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: GS1 Databar expanded decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_rss14_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: GS1 Databar decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_qrcode_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: QR Code decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_micropdf_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Micro PDf decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_maxicode_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Maxicode decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_macropdf_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: MacroPDF decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_macromicropdf_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Macro Micro PDF decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_jappostal_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: JapPostal decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_ean13_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: EAN 13 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_dutchpostal_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Dutchpostal decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_datamatrix_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Datamatrix decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_composit_c_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Composite C decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_composit_ab_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Composite AB decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_canpostal_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: CanPostal decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_auspostal_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: AusPostal decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_upce1_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: UPCE 1 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_upce0_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: UPCE 0 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_upca_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: UPCA decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_upc_ean_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: UPC EAN decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_trioptic39_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Trioptic 39 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_msi_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: MSI decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_i2of5_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: I 2of 5 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_ean8_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: EAN 8 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_d2of5_PropertyValue			(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: D 2of5 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_code93_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Code 93 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_code39_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Code 39 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_code11_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Code 11 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_code128_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Code 128 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_codabar_PropertyValue		(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Codabar decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_matrix2of5_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: Matrix 2of5 decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue
	int Get_Us4StateFics_PropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue);	///<  Retrieve value for property: US 4State FICS decoder.  Parameters and return documentation match ScannerTag_Get_PropertyValue

	////////////////////////////////
	//  EMDK Interface
	////////////////////////////////

	/**
	*  Loads the EMDK Scanner DLL into memory and maps the function pointers to the 
	*  exported functions.
	*  \return FALSE if there was a problem loading or mapping to the DLL, all errors
	*          will be logged.
	*/
	BOOL LoadEMDK();

	/**
	*  Unloads the EMDK Scanning library and resets all the pointers to the 
	*  exported functions
	*  \return TRUE if the unloading was successful.
	*/
	BOOL UnloadEMDK();

	////////////////////////
	//  Initialisation functions.  Provide additional initialisation and are called from
	//  the Public Function 'Initialise()'.
	////////////////////////

	/**
	*  Nullify all the pointers to functions exported by the EMDK Scanner DLL
	*/
	void InitialiseEMDKFunctionPointers();

	/**
	*  metaTagsSetFunctions is a map of EMML Properties to functions which will
	*  set those properties.  The scanner deals with too many properties to be
	*  handled efficiently in a traditional case statement.  This function 
	*  initialises the metaTagsSetFunctions with all EMML Properties and their
	*  corresponding parse functions.
	*/
	void InitialiseMETATagMapForSet();

	/**
	*  metaTagsGetFunctions is a map of EMML Properties to functions which will
	*  retrieve those properties.
	*  \todo Implement Scanner property retrieval
	*/
	void InitialiseMETATagMapForGet();
	
	/**
	*  decoderStrings is a hash map of EMDK Decoders (bytes) to the string
	*  representations of those decoders.  decoderStrings is populated by
	*  this function.
	*/
	void InitialiseDecoderStringMap();

	/**
	*  In Browser and RhoElements 1.x scanner beeper properties were read from
	*  the configuration file.  Scanner configuration settings are read in during
	*  initialisation with this function.
	*  \return TRUE
	*/
	BOOL ReadConfigurationSettings();

	///////////////////////////////////////
	//  Meta Tag Queue Control.  These functions are responsible for managing
	//  the meta tag queue, buffering properties which can not be set
	//  during navigation or until the Scanner is enabled.
	///////////////////////////////////////

	/**
	*  Add the specified property and value to the queue of EMML tags to be 
	*  parsed when the scanner is ready.  This queue is only concerned with 
	*  setting properties.  Memory allocation for queue entries takes place here.
	*  \param szPropertyName The Property to be set.
	*  \param szPropertyValue The value of the property being set.
	*  \param hQueueToAddTo Message queue to which the meta tag should be added.
	*  \return Whether or not the EMML tag was successfully written to the queue.
	*/
	BOOL AddMetaTagToQueue(HANDLE hQueueToAddTo, LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	
	/**
	*  Process the queue of meta tags which are pending either the naviagtion
	*  to finish or the scanner to be enabled.  The scanner will only call this 
	*  function when it is enabled and ready to receive EMML tags.  Memory 
	*  deallocation for queue entries takes place here.
	*  \param hQueueToProcess read Handle to the message queue of meta tags which
	*         should be parsed.
	*  \param bParseTags If set to true the meta tags will be parsed and applied
	*         to the scanner.  Specifying bParseTags to be false will just empty
	*         the queue.
	*/
	void ActionPendingMetaTags(HANDLE hQueueToProcess, bool bParseTags);

	////////////////////////////////////////////////
	//  Scanner Control Functions.  The following functions control
	//  the high level scanner operations such as enabling and disabling it.
	////////////////////////////////////////////////

	/**
	*  Open an interface to the specified scanner, this is a prerequisite 
	*  for any other scanner operation and populates m_hScanner, the handle
	*  to the Scanner.  Only one scanner can be disabled at any one time on a 
	*  single device.  If it has been requested to open a different scanner
	*  to the one already opened (by this CScanner instance) the existing
	*  scanner is closed and the new one opened.
	*  \param szScannerName Scanner device to open, e.g. SCN1, SCN2.  If 
	*         NULL then the default scanner will be opened (SCN1 or SCN2 if
	          there is no SCN1 on the device)
	*  \return Whether or not the Scanner was successfully opened.
	*/
	BOOL OpenScanner(LPCTSTR szScannerName);

	/**
	*  Determines if the specified scanner is present on the device.
	*  \param szScannerName The name of the scanner to test exists, e.g. SCN1, SCN2
	*  \param bIsBluetoothScanner [out] Set to true or false depending on 
	*         whether or not the requested scanner was a BT scanner.
	*  \return Whether or not the scanner was found.
	*/
	BOOL FindScanner(LPCTSTR szScannerName, bool* bIsBluetoothScanner);

	/**
	*  Shut down all communication with the Scanner, 
	*  equivilent to ScanTerm() in PB3.  Also frees all memory associated 
	*  with the open scanner interface.  To use the scanner again it is
	*  necessary to open the scanner.
	*  \return Whether or not the Scanner was successfully closed.
	*/
	BOOL CloseScanner();

	/**
	*  When switching to another another application the scanner must be disabled
	*  and put into a state where it can be used by the other application.  The
	*  logic of this plugin depends on the scanner being in a default state
	*  when the application regains focus so it should be reset to that default
	*  state when losing focus.
	*  \return Whether or not the scanner was successfully closed.
	*/
	BOOL DisableScannerAndResetToDefault();

	/**
	*  Called in response to the user requesting the Scanner to be Enabled, 
	*  the scanner is opened (if closed) and an EnableScannerThread thread
	*  is spawned so the Scanner can be enabled in a separate thread.
	*  \return TRUE
	*/
	BOOL EnableScanner();

	/**
	*  Enables the scanner if it is not already (though under normal operation 
	*  it will be) and create a SubmitReadThread Thread to send a 
	*  read request to the scanner.  The read request must be posted in 
	*  a separate thread as it will block, waiting for the read to complete.
	*  \return TRUE
	*/
	BOOL SubmitRead();

	/**
	*  After a successful scan the data associated with that scan is placed
	*  in a scan buffer.  This function converts the data in the scan 
	*  buffer into a user friendly format for use in the decode event.  Binary
	*  data is encoded to base 64, the timestamp is modified accordingly and
	*  any auxiliary data in the barcode is appended if appropriate.
	*  \return TRUE
	*/
	BOOL RetrieveDataFromScanBuffer();

	/**
	*  Deallocates the scan buffer, used to hold the result of the last scan.
	*  This is required when changing the type of the buffer between 
	*  text (default) and binary.
	*  \return If the buffer deallocation was successful
	*/
	BOOL ResetScanBuffer();

	/**
	*  Switches the "Scan Type" in response to a user request.  The scan 
	*  type is required to allow multiple instances of CScanner to co-exist
	*  harmoneously.  See the help file for more information on interaction
	*  between Scanner types.
	*  \param scanType The type of scan to change to
	*  \return Whether the read type was successfully changed.
	*/
	BOOL ChangeReadType(tagSCANTYPE scanType);  	//  Change between a foreground / background and monitor read


	//////////////////////////////////////////////
	//  User Interface Routines.  These functions assist in returning 
	//  information back to the user.
	//////////////////////////////////////////////

	/**
	*  Informs the application that a Bluetooth Scanner 
    *  has become connected or disconnected.  The logic for communicating the 
	*  status to the user is handled by the 'InformUserOfBTEvent' thread.
	*  \param bConnected Whether the scanner has become connected or disconnected.
	*/
	void SetBluetoothStatus(BOOL bConnected);

	/**
	*  Enumerates the scanners present on the device in a user friendly format.
	*  Used by EnumerateScannersToApplication thread.
	*  \param szScannersArray [out] Populated with a 2D Javascript array of 
	*         scanners.  The memory for szScannersArray must be allocated
	*         by the calling function.
	*/
	void GiveScanners(WCHAR* szScannersArray);

	/**
	*  The scanner has a number of events for which the user can register, 
	*  the logic for allocating the memory for these events is handled by
	*  this function.
	*  \param szUserDefinedEvent The URI the user has requested be associated
	*         with the event, e.g. Javascript:doScan(%json)
	*  \param szScannerMemberVariable [out] Pointer to the scanner's member variable
	*         which is being set, e.g. to set the decode event, pass in 
	*         &m_szEventDecode.  Memory is allocated / deallocated from within 
	*         the function.
	*/
	BOOL SetScannerEvent(LPCTSTR szUserDefinedEvent, WCHAR** szScannerMemberVariable);

	////////////////////////////////////////
	//  Threads.  Since the scanner class interfaces with hardware, multiple threads
	//  are employed to ensure the user interface is not blocked.
	////////////////////////////////////////

	/**
	*  Enables the currently open scanner and perform any other initialisation 
	*  such as registering for BT events (from BT scanners) and customising the 
	*  scanner parameters, e.g. to disable the EMDK scanner beeper.  If there is
	*  a problem enabling the scanner then the specified scanner will be closed
	*  and must be reopened again.  Once enabled and configured a read is submitted
	*  to the scanner to commence scanning.  The scanner must be enabled in a 
	*  separate thread as it may block, especially true of BT scanners which will
	*  block whilst the BT connection is established.
	*  \param lParam Pointer to the CScanner object which started the thread
	*  \return 0 if the scanner enabled successfully
	*/
	static DWORD EnableScannerThread(LPVOID lParam);

	/**
	*  Submits a read to the scanner and waits for that read to be satisfied.
	*  Once complete the scanned data will be returned to the user via a decodeEvent.
	*  A named event is used to alert the class when the read is complete, the name
	*  of this event is unique for each CScanner instance.
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0 if the scan was successful and the data was read
	*/
	static DWORD SubmitReadThread(LPVOID lParam);

	/**
	*  Causes the enumScannerEvent to be sent to the application.
	*  This is done in a separate thread because in RhoElements 1.x (and possibly
	*  RhoElements 2.x the core will block when calling methods like 'enumerate'.
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0
	*/
	static DWORD EnumerateScannersToApplication(LPVOID lParam);

	/**
	*  Causes the DeviceCapabilitiesEvent to be sent to the application, this
	*  contains the supported decoders, capabilities of the scanner and 
	*  version of the firmware.  This needs to be done in a separate thread
	*  for the same reason as enumScannerEvent.
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0
	*/
	static DWORD DeviceCapabilitiesToApplication(LPVOID lParam);

	/**
	*  Notify the user via a Bluetooth Status Event that the BT scanner has 
	*  become connected or disconnected.  Note that the BluetoothStatusEvent
	*  was only introduced in RhoElements 1.x, in order to be backwardly 
	*  compatible with Browser, if the user has not registered for
	*  the event then a decodeEvent is fired instead.
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0
	*/
	static DWORD InformUserOfBTEvent(LPVOID lParam);

	/**
	*  When a user wants to obtain the values of RSM Attributes they mark them
	*  for retrieval using the appropriate method (e.g. markForRetrievalModelNumber).
	*  Registering for a RSMGetEvent will cause the queued attributes to be 
	*  returned to the user, this thread is responsible for both: 
	*  1) Retrieving the value of the specified RSM attribute and adding it to a 
	*  queue to return and 2) Processing that queue and assembling the JS array to
	*  return once an RSMGetEvent is registered.
	*  This function is defined in RSM.cpp
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0
	*/
	static DWORD ProcessRSMGetQueue(LPVOID lParam);

	/**
	*  If the user has configured the scanner to disable when the battery power
	*  gets too low (via RhoElements configuration) this thread is spawned and is
	*  responsible for disabling the scanner when power gets low.  This was a 
	*  specific addition requested by a single customer back in Browser 2.x
	*  \param lParam Pointer to the CScanner object which started the thread.
	*  \return 0
	*/
	static DWORD LowBatteryScanThread(LPVOID lParam);

	/**
	*  The only way of retrieving BT Connected / Disconnected events from the EMDK
	*  is to register to be told so via a Windows Message.  When the BT scanner is
	*  enabled a hidden window is created to receive the connected / disconnected
	*  events, this thread is the WNDPROC for the hidden window.
	*  \param hwnd Window which posted the message
	*  \param message The message, we are only interested in WM_SCAN_EVENT_RHOELEMENTS.
	*  \param wparam The WPARAM of the message, we are only concerned 
	*         with SCAN_EVENT_BT_CONNECTION_STATE_CHANGE
	*  \param lparam The LPARAM of the message
	*  \return Result from the default WNDPROC
	*/
	static LRESULT CALLBACK HiddenWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


	/////////////////////////////////////////////////////
	//  Reader Parameters.  The following functions are declared in EMDKInterfaceAbstraction.cpp
	//  and abstract the EMDK to present a higher level interface for handling the scanner parameters.
	/////////////////////////////////////////////////////

	/**
	*  Obtains the current reader params from the EMDK scanner driver, this structure
	*  is technically laser or Imager specific but many of the parameters are shared
	*  between the two, these parameters map to EMML Tags under Scanner, e.g. 
	*  "Scanner" "aimType:trigger".  It is intended that the caller calls 
	*  ReadScannerParameters before using the retrieved paraters to in a call to 
	*  SetScannerParameters.
	*  \param pReaderParams [out] pointer to a READER_PARAMS Structure, populated with 
	*         current scanner settings.
	*  \return Whether the Scanner parameters were successfully retrieved.
	*/
	BOOL RetrieveScannerParameters(READER_PARAMS* pReaderParams);

	/**
	*  Sets the current reader params at the EMDK layer.  It is intended that the caller calls 
	*  RetrieveScannerParameters before using the retrieved paraters to in a call to 
	*  SetScannerParameters.
	*  \param pReaderParams [out] pointer to a READER_PARAMS Structure, populated with 
	*         desired scanner settings.
	*  \param dwSize size (bytes) of the structure being set.
	*  \return Whether the Scanner Parameters were successfully set.
	*/
	BOOL SetScannerParameters(READER_PARAMS* pReaderParams, DWORD dwSize);

	/**
	*  Retrieves the "interface specific" parameters from the EMDK Scanner driver, 
	*  applicable to the type of scanner attached (and enabled).  RhoElements 1.x 
	*  only supported Bluetooth specific parameters.  These parameters are applicable
	*  to all open application instances.
	*  \param pInterfaceParams [out] pointer to the INTERFACE_PARAMS structure, 
	*         populated with current scanner interface settings.
	*  \return Whether the Scanner Interface parameters were successfully retrieved.
	*/
	BOOL RetrieveScannerInterfaceParameters(INTERFACE_PARAMS* pInterfaceParams);

	/**
	*  Sets the current scanner interface parameters at the EMDK scanner layer.  It is 
	*  intended that the caller calls RetrieveScannerInterfaceParameters before
	*  modifying them and applying them with SetScannerInterfaceParameters.
	*  \param pInterfaceParams pointer to the INTERFACE_PARAMS structure.
	*  \param dwSize size (bytes) of the structure being set.
	*  \return Whether the Scanner Interface parameters were successfully set.
	*/
	BOOL SetScannerInterfaceParameters(INTERFACE_PARAMS* pInterfaceParams, DWORD dwSize);


	//////////////////////////////////////////////////////////
	//  Decoder Functions.  The following functions are declared in EMDKInterfaceAbstraction.cpp
	//  and abstract the EMDK to present a higher level interface for handling decoders.
	//////////////////////////////////////////////////////////

	/**
	*  Returns a 1 dimensional javascript array of the decoders supported by the current
	*  enabled scanner.
	*  \param szSupportedDecodersArray String populated with the 1D Javascript array
	*         of decoders, sufficient memory must be allocated by the caller.
	*/
	BOOL RetrieveSupportedDecoders(WCHAR* szSupportedDecodersArray);

	/**
	*  Returns whether or not the specified decoder is supported by the enabled
	*  Scanner.
	*  \param cDecoder The EMDK representation of the decoder (as a BYTE)
	*  \return Whether or not the decoder is supported
	*/
	BOOL IsDecoderSupported(BYTE cDecoder);

	/**
	*  Enable the specified decoder if it is not already enabled.  Attempting to 
	*  enable an enabled decoder will log a message.
	*  \param cDecoder The EMDK representation of the decoder (as a BYTE)
	*  \return Whether or not the decoder was successfully enabled.
	*/
	BOOL EnableDecoder(BYTE cDecoder);

	/**
	*  Disable the specified decoder if it is not already disabled.  Attempting to 
	*  disable a disabled decoder will log a message.
	*  \param cDecoder The EMDK representation of the decoder (as a BYTE)
	*  \return Whether or not the decoder was successfully disabled.
	*/
	BOOL DisableDecoder(BYTE cDecoder);

	/**
	*  Retrieve the parameters associated with the specified decoder, e.g. 
	*  Codabar has ClsiEditing, maxlength, minlength, notisediting and 
	*  redundancy.
	*  \param cDecoder The EMDK representation of the decoder whos parameters
	*         are to be obtained.
	*  \param lpvParams [out] Pointer to a structure which will be populated
	*         with the decoder parameters.  Memory must be allocated by the
	*         caller.  Different decoders will return different structures.
	*  \param dwSize Size of lpvParams, since different Decoders require 
	*         different numbers of parameters the size is specified here.
	*  \return Whether or not the decoder parameters were successfully retrieved.
	*/
	BOOL RetrieveDecoderParams(BYTE cDecoder, LPVOID lpvParams, DWORD dwSize);

	/**
	*  Set the parameters associated with a specific decoder, e.g. 
	*  Codabar has ClsiEditing, maxlength, minlength, notisediting and 
	*  redundancy.
	*  \param cDecoder The EMDK representation of the decoder whos parameters
	*         are to be set.
	*  \param lpvParams Pointer to a structure containing the parameters 
	*         applicable to this decoder.
	*  \param dwSize size of lpvParams.
	*  \return Whether or not the decoder parameters were successfully set.
	*/
	BOOL SetDecoderParams(BYTE cDecoder, LPVOID lpvParams, DWORD dwSize);

	/**
	*  Sets the minimum or maximum length of the specified decoder.
	*  \param cDecoder The EMDK representation of the decoder whos parameters
	*         are to be set.
	*  \param iMinLength The minimum length to set the decoder to, specify
	*         this value as -1 to leave the minimum length unchanged.
	*  \param iMaxLength The maximum length to set the decoder to, specify
	*         this value as -1 to leave the maximum length unchanged.
	*  \return Whether or not the decoder lengths were successfully set.
	*/
	BOOL SetDecoderLengths(BYTE cDecoder, int iMinLength, int iMaxLength);

	/**
	*  Retrieves the maximum or minimum length of the specified decoder.
	*  \param cDecoder The EMDK representation of the decoder whos parameters
	*         are to be set.
	*  \param bMinLength Set to true to return the minimum length, otherwise
	*         the maximum length is returned.
	*  \return The maximum or minimum length of the specified decoder
	*/
	int GetDecoderLength(BYTE cDecoder, bool bMinLength);

	/**
	*  Retrieves the current values for UPC EAN params, because UPC EAN parameters 
	*  do not apply to a specific decoder they are handled separately by
	*  the EMDK and appear at the top level "Scanner" tag in EMML, e.g.
	*  "Scanner" "UpcEanBookland:true".
	*  \param lpParams [out] Pointer to the UPCEAN PARAMS structure to be
	*         populated with the current values.
	*  \return Whether or not the UPC EAN parameters were successfully 
	*  obtained.
	*/
	BOOL RetrieveUPCEANParams(LPUPC_EAN_PARAMS lpParams);

	/**
	*  Sets the values for UPC EAN parameters, it is intended that callers first
	*  call RetrieveUPCEANParams before calling SetUPCEANParams with modified
	*  values.
	*  \param lpParams Pointer to the UPCEAN PARAMS structure with the values
	*         to be set.
	*  \return Whether or not the UPC EAN Parameters were successfully set.
	*/
	BOOL SetUPCEANParams(LPUPC_EAN_PARAMS lpParams);


	///////////////////////////////////////////////////
	//  RSM.
	//  These functions are declared in RSM.cpp
	//////////////////////////////////////////////////

	/**
	*  Retrieves all the RSM attributes supported by the currently enabled scanner.
	*  This populates the m_rsmSupportedAttributes array which is used when setting
	*  and retrieving RSM attributes.
	*  This function is called when the scanner is enabled.
	*  \return Whether or not the RSM parameters could be successfully obtained.
	*/
	BOOL RSMGetSupportedAttributes();

	/**
	*  Places the specified RSM attribute in the queue of attributes to return
	*  when the user specifies an RSMGetEvent.
	*  \param attribute Identifier of the attribue whose value is to be retrieved.
	*  \return Whether or not the attribute was successfully written to the queue.
	*/
	BOOL MarkRSMForRetrieval(ATTRIB_NUMBER attribute);

	/**
	*  Perform checks to ensure the attribute is appropriate to retrieve and populate the 
	*  m_szRSMValuesToReturn array with 'unsupported' or 'failed to get' if it is not appropriate
	*  to retrieve the value.  If the value is retrievable the function 
	*  RetrieveRSMAttributeValueAsString is called.
	*  \param rsmAttribute RSM attribute whose value is to be obtained.
	*  \param szValueBuffer Memory buffer into which the RSM attribute value will be placed 
	*         (memory allocated by the caller).  Can be NULL to just return the length of 
	*         string required.
	*  \return -1 if the value could not be retrieved, else the length of the string
	*          returned.  
	*/
	int RetrieveRSMAttributeValue(LPRSM_ATTRIB rsmAttribute, WCHAR* szValueBuffer);

	/**
	*  Convert the specified attribute's value to a string and add it
	*  to m_szRSMValuesToReturn, which contains the values to be returned in 
	*  the RSMGetEvent.
	*  \param rsmAttribute The RSM attribute whose value is to be retrieved.
	*  \param szRSMValue [out] Parameter into which the RSM attribute will be copied.  
	*         specify this parameter as NULL to just retrieve the size of the string
	*         being returned.  This memory must be allocated by the caller.
	*  \return The length of the parameter copied into szRSMValue (or -1 if error).  If
	*          szRSMValue is NULL this value will be the number of characters required
	*          to be allocated by the caller (including NULL)
	*/
	int RetrieveRSMAttributeValueAsString(LPRSM_ATTRIB rsmAttribute, WCHAR* szRSMValue);

	/**
	*  Set the specified RSM attribute to the specified value.  The value is converted 
	*  into a value recognisable by the EMDK layer (i.e. RhoElements specifies
	*  strings but most of the EMDK uses flags or byte values for RSM).  RSM parameters
	*  contain more data then just the values, they also have R/W properties and types;
	*  all those are set by this function.
	*  \param rsmAttributeID RSM 'number' (as defined at the EMDK) of the RSM
	*         attribute whose value is being set.
	*  \param szValue String representation of value to set the attribute's value to.
	*/
	BOOL SetRSMValue(ATTRIB_NUMBER rsmAttributeID, LPCTSTR szValue);


	//////////////////////////////////
	//  Utility Functions, 
	//  Implemented in Utilities.cpp
	//////////////////////////////////

	/**
	*  Case insensitive comparison operator for two strings
	*  \param tc1 String 1 to compare
	*  \param tc2 String 2 to compare
	*  \return TRUE if the two strings are the same, regardless of case
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);

	/**
	*  Method to convert a string to an integer.  This function is safe to call with NULL
	*  \param szInString String to convert to an integer (or NULL)
	*  \return Integer representation of the string (or 0 if NULL)
	*/
	int StringToInt(LPCTSTR szInString);

	/**
	*  Method to abstract the enabled property.  Eases backwards compatibility with
	*  Enable / Enabled
	*  \param szParameter Parameter to determine whether or not it represents 'enabled'.
	*  \return Whether or not the specified parameter represents 'enabled'.
	*/
	BOOL EnabledValue(LPCTSTR szParameter);

	/**
	*  Method to abstract the disabled property.  Eases backwards compatibility with
	*  Disable / Disabled
	*  \param szParameter Parameter to determine whether or not it represents 'disabled'.
	*  \return Whether or not the specified parameter represents 'disabled'.
	*/
	BOOL DisabledValue(LPCTSTR szParameter);

	/**
	*  Given a number this function will return the number of digits in it, so
	*  given 100, this function will return 3; given 56739 this function will return 5
	*  \param iNumber Number whose length is to be returned
	*  \return Length of passed number.
	*/
	unsigned int StringLength(DWORD iNumber);

	/**
	*  If the Scanner needs to return a string parameter to the user this function takes care
	*  of checking there is sufficient memory and determining the length of the returned string
	*  \param szPropertyValue The value to be returned to the user
	*  \param szPropertyBuffer Buffer into which the value is to be placed, allocated by the 
	*         calling function.  If set to NULL this function will just return the number of
	*         characters required.
	*  \return Length of the string being returned (including terminating NULL character)
	*/
	int ProcessDecoderPropertyForReturn(LPCTSTR szPropertyValue, WCHAR* szPropertyBuffer);

	/**
	*  If the Scanner needs to return an integer parameter to the user this function takes care
	*  of checking there is sufficient memory and determining the length of the returned string
	*  \param iPropertyValue The value to be returned to the user
	*  \param szPropertyBuffer Buffer into which the value is to be placed, allocated by the 
	*         calling function.  If set to NULL this function will just return the number of
	*         characters required.
	*  \return Length of the string being returned (including terminating NULL character)
	*/
	int ProcessDecoderPropertyForReturn(DWORD iPropertyValue, WCHAR* szPropertyBuffer);

	/**
	*  Converts a Hexadecimal string to an Integer
	*  \param value String representation of a hex number
	*/
	int _httoi(const TCHAR *value);

	///////////////////////
	//  Attributes	
	///////////////////////
	int			m_iInstanceID;				///<  Id of this CScanner instance (provided by the framework)
	int			m_iInstanceID2;				///<  Second Id of this CScanner instance (provided by the framework)
	PBModule*	m_pModule;					///<  Instance of the framework to enable communication with core
	HMODULE		m_hScnAPIDLL;				///<  HMODULE of the loaded EMDK Scanner DLL
	bool		g_bShuttingDown;			///< Flag that we're currently shutting down
	std::map<const WCHAR*, fnMetaTagParse, wcharCmp> metaTagsSetFunctions;		///<  Map of Meta tags to functions used to action those, parsed meta tags
	///  And the corresponding iterator
	std::map<const WCHAR*, fnMetaTagParse, wcharCmp>::iterator metaTagsSetIterator;
	std::map<const WCHAR*, fnEMMLTagGet, wcharCmp> metaTagsGetFunctions;		///<  Map of Meta tags to functions to retrieve those tags
	///  And the corresponding iterator
	std::map<const WCHAR*, fnEMMLTagGet, wcharCmp>::iterator metaTagsGetIterator;
	std::map<const BYTE, const WCHAR*, byteCmp>decoderStrings;					///<  Map of Scanner decoders to their string equivilents ('Img)
	bool		m_bScannerEnabled;			///<  Set to true if the Scanner is currently enabled
	bool		m_bScannerBeingEnabled;		///<  Set to true if the Scanner is currently being enabled, required because scanners take some finite time to enable
	bool		m_bScannerOpen;				///<  Set to true if the Scanner is currently 'open', i.e. it must be closed before another scanner is open on the same device.
	bool		m_bApplicationActive;		///<  Set to true if the Application housing the scanner is currently Active (In a Windows sense)
	HANDLE		m_hMetaMsgQRead;			///<  Read handle to meta tag queue
	HANDLE		m_hMetaMsgQWrite;			///<  Write handle to meta tag queue
	HANDLE		m_hRSMMsgQRead;				///<  Read Handle for RSM Retrieval Queue
	HANDLE		m_hRSMMsgQWrite;			///<  Write handle for RSM Retrieval Queue
	HANDLE		m_hTagsOnCurrentPageQRead;	///<  Read Handle for queue of EMML tags on the current page.
	HANDLE		m_hTagsOnCurrentPageQWrite;	///<  Write Handle for queue of EMML tags on the current page.
	WCHAR*		m_szUserSpecifiedScanner;	///<  Scanner Name requested by User
	WCHAR*      m_szCurrentlyOpenScanner;	///<  String representation of the currently open scanner, used to determine if the user has requested a different scanner be enabled.
	bool		m_bUsingBluetoothScanner;	///<  Whether or not the connected scanner is a BT scanner
	HANDLE		m_hScanner;					///<  Handle to the open scanner
	ScannerState	m_ScannerState;			///<  Internal State of the Application, used to keep track of whether a Bluetooth scanner is connected or not
	LPSCAN_BUFFER	m_lpScanBuffer;			///<  Pointer to Scanner buffer which receives decoded data.
	HANDLE		m_hReadComplete;			///<  Named Event signalled when a scan is complete
	HANDLE		m_hUniqueEvent;				///<  Handle used to determine a unique Scanner Instance
	DWORD		m_dwRequestID;				///<  Request ID for scanner read request
	tagScanData			m_lastScannedData;	///<  Stores Decoded data, the size of the data in this buffer is specified by the user.
	bool		m_bResetScanBuffer;			///<  Flag whether the Scan buffer should be reset after this scan (used for data type and buffer size changes)
	CKeyInjector*		m_keyInjector;		///<  Pointer to a KeyInjector Object to handle key sending;
	CDecodeNotifier*	m_decodeNotifier;	///<  Pointer to object to indicate bad / good scans to the user
	bool		m_bCurrentlyNavigating;		///<  Flag that we're currently navigating to a different page
	WCHAR		m_szBluetoothStatus[30];	///<  Current Bluetooth Status, used in BluetoothStatusEvent
	eBTPairing	m_iDisplayBTPairBarcode;	///<  We need to maintain the state of whether the user has requested this as we need to apply it between Open and Enable
	DWORD		m_dwEnableTimeout;			///<  The maximum allowable time given for the Scanner to enable, only really useful when connecting to a BluetoothScanner which blocks to connect.
	bool		m_bScannerReadUnsubmittedDuringNav;	///<  Set to true if the scanner read is not re-submitted during a navigation.
	//  Scanner Events
	WCHAR*		m_szEventDecode;			///<  DecodeEvent
	WCHAR*		m_szEventEnumScanners;		///<  EnumScannerEvent
	WCHAR*		m_szEventBluetoothStatus;	///<  BluetoothStatusEvent (New for RhoElements)
	WCHAR*		m_szEventRSMGetEvent;		///<  RSM Get Event to return the RSM Attributes requested
	WCHAR*		m_szEventDeviceCapabilities;///<  Device Capabilities Event (New for RhoElements 2.x)
	//  Scanner Properties
	bool		m_bAutoEnter;				///<  Set to true if the user has specified autoEnter.
	bool		m_bAutoTab;					///<  Set to true if the user has specified autoTab
	tagSCANTYPE	m_currentScanType;			///<  Holds the current type of scan, Foreground, Background or Monitor.
	DWORD		m_iDataBufferSize;			///<  The user requested size of the scan read buffer (which receives barcodes)
	eScanBufferDataFormat	m_bufferFormat;	///<  The format of the buffer to receive scans, either Text or Data
	//  Configuration Settings
	DWORD		m_dwDecodeVolume;			///<  Volume of the device beeper on a decode
	DWORD		m_dwDecodeFrequency;		///<  Frequency of the device beeper on a good decode
	DWORD		m_dwInvalidDecodeFrequency;	///<  Frequency of the device beeper on a bad decode
	DWORD		m_dwDecodeDuration;			///<  Duration of the device beeper on a decode
	WCHAR*		m_szScanDecodeWav;			///<  Wave file to be played on a good decode (takes precedence over beeper)
	WCHAR*		m_szScanInvalidWav;			///<  Wave file to be played on a bad decode (takes precedence over beeper)
	bool		m_bLowBatteryScan;			///<  Flag to indicate that the scanner should be disabled when the battery gets low
	bool		m_bLegacyNavBehaviour;		///<  RhoElements disables the scanner when a page navigation occurs, if this attribute is set the scanner will just stop the current scan.
	//  RhoElements Window
	WCHAR		m_szHiddenWindowName[25];	///<  Name of the hidden window to receive Bluetooth events (instance specific)
	HWND		m_hiddenWindow;				///<  Handle of the hidden window to receive Bluetooth events.
	//  RSM
	LPATTRIB_NUMBER	m_rsmSupportedAttributes;			///< Array of RSM attributes supported by the attached scanner
	int				m_iCountOfSupportedRSMAttributes;	///<  Number of RSM Attributes that are supported.
	int				m_iCountOfMarkedRSMAttributes;		///< Length of the array of attributes supported by the attached scannner
	TCHAR*			m_szRSMValuesToReturn[MAX_RSM_READABLE_ATTRIBUTES];  ///< Array of strings to be returned for RSMGetNavigate
	//  Viewfinder
	int	m_iViewfinderRequestedLeft;			///<  User requested left position of the viewfinder (for Camera scanners)
	int m_iViewfinderRequestedRight;		///<  User requested right position of the viewfinder (for Camera scanners)
	int m_iViewfinderRequestedTop;			///<  User requested top position of the viewfinder (for Camera scanners)
	int m_iViewfinderRequestedBottom;		///<  User requested bottom position of the viewfinder (for Camera scanners)
	//  Low Battery Scan
	HANDLE m_hPowerNotifications;				///<  Handle for the thread which monitors for low power conditions.
	HANDLE m_hLowBatteryScanMessageQueueRead;	///<  Handle of Queue that will receive power notifications
	HANDLE m_hLowBatteryScanMessageQueueWrite;  ///<  Handle of queue to allow us to inject power notification (to shut down the thread)

	//////////////////////////////
	//  Instances of those Function Pointers to exported DLL functions
	//////////////////////////////
	LPFN_SCAN_ALLOCATEBUFFER_T				lpfn_SCAN_AllocateBuffer;		///< function pointer to EMDK Scanner API SCAN_AllocateBuffer
	LPFN_SCAN_CANCELREAD_T					lpfn_SCAN_CancelRead;			///< function pointer to EMDK Scanner API SCAN_CancelRead 
	LPFN_SCAN_DEALLOCATEBUFFER_T			lpfn_SCAN_DeallocateBuffer;		///< function pointer to EMDK Scanner API SCAN_DeallocateBuffer 
	LPFN_SCAN_DISABLE_T						lpfn_SCAN_Disable;				///< function pointer to EMDK Scanner API SCAN_Disable  
	LPFN_SCAN_FLUSH_T						lpfn_SCAN_Flush;				///< function pointer to EMDK Scanner API SCAN_Flush 
	LPFN_SCAN_GETDECODERPARAMS_T			lpfn_SCAN_GetDecoderParams;		///< function pointer to EMDK Scanner SCAN_GetDecoderParams 
	LPFN_SCAN_GETENABLEDDECODERS_T			lpfn_SCAN_GetEnabledDecoders;	///< function pointer to EMDK Scanner SCAN_GetEnabledDecoders
	LPFN_SCAN_GETREADERPARAMS_T				lpfn_SCAN_GetReaderParams;		///< function pointer to EMDK Scanner SCAN_GetReaderParams
	LPFN_SCAN_GETSCANPARAMETERS_T			lpfn_SCAN_GetScanParameters;	///< function pointer to EMDK Scanner SCAN_GetScanParameters
	LPFN_SCAN_GETSCANSTATUS_T				lpfn_SCAN_GetScanStatus;		///< function pointer to EMDK Scanner SCAN_GetScanStatus
	LPFN_SCAN_GETSUPPORTEDDECODERS_T		lpfn_SCAN_GetSupportedDecoders;	///< function pointer to EMDK Scanner SCAN_GetSupportedDecoders
	LPFN_SCAN_GETUPCEANPARAMS_T				lpfn_SCAN_GetUPCEANParams;		///< function pointer to EMDK Scanner SCAN_GetUPCEANParams
	LPFN_SCAN_OPEN_T						lpfn_SCAN_Open;					///< function pointer to EMDK Scanner SCAN_Open
	LPFN_SCAN_READLABELEVENT_T				lpfn_SCAN_ReadLabelEvent;		///< function pointer to EMDK SCAN_ReadLabelEvent
	LPFN_SCAN_READLABELMSG_T				lpfn_SCAN_ReadLabelMsg;			///< function pointer to EMDK SCAN_ReadLabelMsg
	LPFN_SCAN_RSMGETATTRIBUTE_T				lpfn_SCAN_RSMGetAttribute;		///< function pointer to EMDK SCAN_RSMGetAttribute
	LPFN_SCAN_RSMGETSUPPORTEDATTRIBUTES_T	lpfn_SCAN_RSMGetSupportedAttributes;	///< function pointer to EMDK API SCAN_RSMGetSupportedAttributes
	LPFN_SCAN_RSMSETATTRIBUTE_T				lpfn_SCAN_RSMSetAttribute;		///< function pointer to EMDK API SCAN_RSMSetAttribute
	LPFN_SCAN_SETDECODERPARAMS_T			lpfn_SCAN_SetDecoderParams;		///< function pointer to EMDK API SCAN_SetDecoderParams
	LPFN_SCAN_SETENABLEDDECODERS_T			lpfn_SCAN_SetEnabledDecoders;	///< function pointer to EMDK API SCAN_SetEnabledDecoders
	LPFN_SCAN_SETREADERPARAMS_T				lpfn_SCAN_SetReaderParams;		///< function pointer to EMDK API SCAN_SetReaderParams
	LPFN_SCAN_SETSCANPARAMETERS_T			lpfn_SCAN_SetScanParameters;	///< function pointer to EMDK API SCAN_SetScanParameters
	LPFN_SCAN_SETSOFTTRIGGER_T				lpfn_SCAN_SetSoftTrigger;		///< function pointer to EMDK API SCAN_SetSoftTrigger
	LPFN_SCAN_SETUPCEANPARAMS_T				lpfn_SCAN_SetUPCEANParams;		///< function pointer to EMDK API SCAN_SetUPCEANParams
	LPFN_SCAN_CLOSE_T						lpfn_SCAN_Close;				///< function pointer to EMDK API SCAN_Close
	LPFN_SCAN_ENABLE_T						lpfn_SCAN_Enable;				///< function pointer to EMDK API SCAN_Enable
	LPFN_SCAN_REGISTERSCANMESSAGE_T			lpfn_SCAN_RegisterScanMessage;	///< function pointer to EMDK API SCAN_RegisterScanMessage
	LPFN_SCAN_DEREGISTERSCANMESSAGE_T		lpfn_SCAN_DeRegisterScanMessage;///< function pointer to EMDK API SCAN_DeRegisterScanMessage
	LPFN_SCAN_FINDFIRST_T					lpfn_SCAN_FindFirst;			///< function pointer to EMDK API SCAN_FindFirst
	LPFN_SCAN_FINDNEXT_T					lpfn_SCAN_FindNext;				///< function pointer to EMDK API SCAN_FindNext
	LPFN_SCAN_FINDCLOSE_T					lpfn_SCAN_FindClose;			///< function pointer to EMDK API SCAN_FindClose
	LPFN_SCAN_GETINTERFACEPARAMS_T			lpfn_SCAN_GetInterfaceParams;	///< function pointer to EMDK API SCAN_GetInterfaceParams
	LPFN_SCAN_SETINTERFACEPARAMS_T			lpfn_SCAN_SetInterfaceParams;	///< function pointer to EMDK API SCAN_SetInterfaceParams
	LPFN_SCAN_GETDEVICEINFO_T				lpfn_SCAN_GetDeviceInfo;		///< function pointer to EMDK API SCAN_GetDeviceInfo
	LPFN_SCAN_GETVERSIONINFO_T				lpfn_SCAN_GetVersionInfo;		///< function pointer to EMDK API SCAN_GetVersionInfo


};



