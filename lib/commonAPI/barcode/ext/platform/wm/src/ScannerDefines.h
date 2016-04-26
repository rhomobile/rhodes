/**
*  \file ScannerDefines.h
*  \brief Defines and constants associated with the Scanner implementation.
*  \author	Darryn Campbell (DCC)
*  \date	January 2012 (DCC, Initial Port from RhoElements 1 Code)
*/

#pragma once
#include "scancapi.h"

//  Porting 
//#include "../../Common/Public/PBPlugin.h"
//#include "../../Common/Public/PBUtil.h"
#define WM_SCAN_EVENT_RHOELEMENTS	WM_USER + 20	///<  Windows Message fired when the BT scanner connects / disconnects
#define MAX_URL 2048	//  REMOVE THIS - ONLY ADDED TO GET PROTOTYPE TO COMPILE
#define MAXURL MAX_URL

#define MAXLOG 1024							///<  Used internally for constructing log messages
#define MAXIMUM_PARAMETER_LENGTH 30

//  Scanner Beeper Defines
#define DEFAULT_SCAN_BUFFER_SIZE 2713		///< Maximum size of decoded data
#define MAX_SOURCE_SIZE 24   ///<  Maximum size of the source string (e.g. Codabar e.t.c)
#define DEFAULT_DECODE_VOLUME 5				///<  Default volume for a decode (beeper)
#define DEFAULT_GOOD_DECODE_FREQUENCY 3000	///<  Default frequency (Hz) of a valid decode (beeper)
#define DEFAULT_INVALID_DECODE_FREQUENCY 2500	///<  Default frequency (Hz) of an invalid decode (beeper)
#define DEFAULT_DECODE_DURATION 250			///<  Default length of a decode beep (ms)
#define MAX_DECODER_NAME_LENGTH 30			///<  Maximum length of a decoder name (This should be made dynamic)

//  RSM Defines
#define ATTR_MAX 0xFFFF	///<  Ficticious RSM Attribute, used internally to denote the end of parameters being retrieved.
#define ATTR_END 0xFFFE	///<  Ficticious RSM Attribute, used internally to denote the RSM is being shut down
#define MAX_RSM_READABLE_ATTRIBUTES 60	///<  The maximum number of RSM attributes which can be returned
#define MAX_RSM_STRING_LENGTH UNION_SIZE * sizeof(WCHAR)	///<  The maximum size of an RSM attributes' String

//  Minimum and Maximum values for Decoders, used when setting decoder properties
#define MINIMUM_CODABAR_LENGTH 0			///<  Minimum length of a Codabar barcode
#define MINIMUM_CODE128_LENGTH 0			///<  Minimum length of a Code 128 barcode
#define MINIMUM_CODE39_LENGTH 0				///<  Minimum length of a Code 39 barcode
#define MINIMUM_CODE93_LENGTH 0				///<  Minimum length of a Code 93 barcode
#define MINIMUM_D2OF5_LENGTH 0				///<  Minimum length of a D 2 of 5 barcode
#define MINIMUM_I2OF5_LENGTH 0				///<  Minimum length of a I 2 of 5 barcode
#define MINIMUM_MSI_LENGTH 0				///<  Minimum length of a MSI barcode
#define MINIMUM_KOREAN2OF5_LENGTH 0			///<  Minimum length of a Korean 2 of 5 barcode
#define MINIMUM_MATRIX2OF5_LENGTH 0			///<  Minimum length of a Matrix 2 of 5 barcode
#define MINIMUM_CODE11_LENGTH 0				///<  Minimum length of a Code 11 barcode
#define MINIMUM_UPCEAN_RETRY_COUNT 2		///<  Minimum value for the UPC EAN Retry Count
#define MINIMUM_CODE128_SECURITY_LEVEL 0	///<  Minimum value for Code 128 Security Level
#define MINIMUM_UPC_EAN_SECURITY_LEVEL 0	///<  Minimum value for the UPC EAN Security Level
#define MINIMUM_CODE39_SECURITY_LEVEL 0		///<  Minimum value for the Code 39 Security Level
#define MINIMUM_SIGNATURE_IMAGE_QUALITY 10	///<  Minimum value for the Signature Image quality
#define MINIMUM_SIGNATURE_IMAGE_SIZE 1		///<  Minimum value for the Signature Image Size
#define MINIMUM_SIGNATURE_IMAGE_WIDTH 20	///<  Minimum width of a Signature Barcode
#define MINIMUM_SIGNATURE_IMAGE_HEIGHT 20	///<  Minimum height of a Signature Barcode
#define MAXIMUM_CODABAR_LENGTH 55			///<  Maximum length of a Codabar barcode
#define MAXIMUM_CODE128_LENGTH 55			///<  Maximum length of a Code 128 barcode
#define MAXIMUM_CODE39_LENGTH 55			///<  Maximum length of a Code 39 barcode
#define MAXIMUM_CODE93_LENGTH 55			///<  Maximum length of a Code 93 barcode
#define MAXIMUM_D2OF5_LENGTH 55				///<  Maximum length of a D 2 of 5 barcode
#define MAXIMUM_I2OF5_LENGTH 55				///<  Maximum length of a I 2 of 5 barcode
#define MAXIMUM_MSI_LENGTH 55				///<  Maximum length of a MSI barcode
#define MAXIMUM_KOREAN2OF5_LENGTH 55		///<  Maximum length of a Korean 2 of 5 barcode
#define MAXIMUM_MATRIX2OF5_LENGTH 55		///<  Maximum length of a Matrix 2 of 5 barcode
#define MAXIMUM_CODE11_LENGTH 55			///<  Maximum length of a Code 11 barcode
#define MAXIMUM_UPCEAN_RETRY_COUNT 20		///<  Maximum value for the UPC EAN Retry Count
#define MAXIMUM_CODE128_SECURITY_LEVEL 3	///<  Maximum value for Code 128 Security Level
#define MAXIMUM_UPC_EAN_SECURITY_LEVEL 3	///<  Maximum value for the UPC EAN Security Level
#define MAXIMUM_CODE39_SECURITY_LEVEL 3		///<  Maximum value for the Code 39 Security Level
#define MAXIMUM_SIGNATURE_IMAGE_QUALITY 100	///<  Maximum value for the Signature Image Quality

//  Minimum and Maximum values for the Scanner Attributes
#define MINIMUM_TIMED_AIM_DURATION 1		///<  Minimum value for timedAimDuration
#define MINIMUM_CONNECTION_IDLE_TIMEOUT 1	///<  Minimum value for connectionIdleTimeout
#define MINIMUM_SCAN_TIMEOUT_DURATION 0		///<  Minimum value for scanTimeout (0 is infinite timeout)
#define MINIMUM_RASTER_HEIGHT 0				///<  Minimum value for rasterHeight
#define MINIMUM_SAME_SYMBOL_TIMEOUT 0		///<  Minimum value for sameSymbolTimeout
#define MINIMUM_DIFFERENT_SYMBOL_TIMEOUT 0	///<  Minimum value for differentSymbolTimeout
#define MINIMUM_VIEWFINDER_X 0				///<  Minimum value for viewfinderX
#define MINIMUM_VIEWFINDER_Y 0				///<  Minimum value for viewfinderY
#define MINIMUM_VIEWFINDER_WIDTH 1			///<  Minimum value for viewfinderWidth
#define MINIMUM_VIEWFINDER_HEIGHT 1			///<  Minimum value for viewfinderHeight
#define MINIMUM_VIEWFINDER_FEEDBACK_TIME 0	///<  Minimum value for viewfinderFeedbackTime
#define MINIMUM_DATA_BUFFER_SIZE 0			///<  Minimum value for dataBufferSize
#define MINIMUM_ENABLE_TIMEOUT 1			///<  Minimum value for enable timeout
#define MAXIMUM_RASTER_HEIGHT 100			///<  Maximum value for rasterHeight
#define MAXIMUM_DATA_BUFFER_SIZE ULONG_MAX	///<  Maximum value for dataBufferSize
#define MAXIMUM_ENABLE_TIMEOUT 60			///<  Maximum value for enable timeout

//  Minimum and Maximum values for Beeper Attributes
#define MINIMUM_DECODE_VOLUME				0		///<  Minimum value for decode volume
#define MAXIMUM_DECODE_VOLUME				5		///<  Maximum value for decode volume
#define MINIMUM_DECODE_DURATION				0		///<  Minimum value for decode duration
#define MAXIMUM_DECODE_DURATION				5000	///<  Maximum value for decode duration
#define MINIMUM_SUCCESSFUL_DECODE_FREQUENCY	0		///<  Minimum value for successful decode frequency
#define MAXIMUM_SUCCESSFUL_DECODE_FREQUENCY	0xFFFF	///<  Maximum value for successful decode frequency
#define MINIMUM_INVALID_DECODE_FREQUENCY	0		///<  Miminum value for invalid decode frequency
#define MAXIMUM_INVALID_DECODE_FREQUENCY	0xFFFF	///<  Maximum value for invalid decode frequency

//  Minimum and Maximum values for the RSM Attrbibutes
#define MINIMUM_RSM_BT_RECONNECT_ATTEMPTS 30	///<  Minimum value for bluetoothReconnectAttempts
#define MINIMUM_RSM_PAGING_BEEP_SEQ 0			///<  Minimum value for pagingBeepSequence
#define MINIMUM_RSM_GOOD_SCANS_DELAY 0			///<  Minimum value for goodScansDelay
#define MAXIMUM_RSM_BT_RECONNECT_ATTEMPTS 60	///<  Maximum value for bluetoothReconnectAttempts
#define MAXIMUM_RSM_PAGING_BEEP_SEQ 15			///<  Maximum value for pagingBeepSequence
#define MAXIMUM_RSM_GOOD_SCANS_DELAY 15000		///<  Maximum value for goodScansDelay

//  Definitions of Methods exported from EMDK Scanner Library
#define EMDK_DLL L"SCNAPI32.DLL"				///<  Name of the EMDK Scanner Library
typedef LPSCAN_BUFFER (WINAPI* LPFN_SCAN_ALLOCATEBUFFER_T)(BOOL,DWORD);													///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_CANCELREAD_T)(HANDLE, DWORD);										///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_DEALLOCATEBUFFER_T)(LPSCAN_BUFFER);								///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_DISABLE_T)(HANDLE);												///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_FLUSH_T)(HANDLE);													///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETDECODERPARAMS_T)(HANDLE, LPDECODER, LPDECODER_PARAMS);			///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETENABLEDDECODERS_T)(HANDLE, LPDECODER_LIST);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETREADERPARAMS_T)(HANDLE, LPREADER_PARAMS);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETSCANPARAMETERS_T)(HANDLE, LPSCAN_PARAMS_W);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETSCANSTATUS_T)(HANDLE, LPSCAN_STATUS);							///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETSUPPORTEDDECODERS_T)(HANDLE, LPDECODER_LIST);					///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETUPCEANPARAMS_T)(HANDLE, LPUPC_EAN_PARAMS);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_OPEN_T)(LPCTSTR, LPHANDLE);										///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_READLABELEVENT_T)(HANDLE, LPSCAN_BUFFER, HANDLE, DWORD, LPDWORD);	///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_READLABELMSG_T)(HANDLE, LPSCAN_BUFFER, HWND, UINT, DWORD, LPDWORD);///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_RSMGETATTRIBUTE_T)(HANDLE, RSM_VALUE_TYPE, LPRSM_ATTRIB);			///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_RSMGETSUPPORTEDATTRIBUTES_T)(HANDLE, LPATTRIB_NUMBER, LPDWORD);	///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_RSMSETATTRIBUTE_T)(HANDLE, LPRSM_ATTRIB);							///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETDECODERPARAMS_T)(HANDLE, LPDECODER, LPDECODER_PARAMS);			///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETENABLEDDECODERS_T)(HANDLE, LPDECODER_LIST);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETREADERPARAMS_T)(HANDLE, LPREADER_PARAMS);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETSCANPARAMETERS_T)(HANDLE, LPSCAN_PARAMS);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETSOFTTRIGGER_T)(HANDLE, LPBOOL);									///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETUPCEANPARAMS_T)(HANDLE, LPUPC_EAN_PARAMS);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_CLOSE_T)(HANDLE);													///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_ENABLE_T)(HANDLE);													///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_REGISTERSCANMESSAGE_T)(HANDLE, HWND, UINT);						///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_DEREGISTERSCANMESSAGE_T)(HANDLE);									///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_FINDFIRST_T)(LPSCAN_FINDINFO, LPHANDLE);							///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_FINDNEXT_T)(LPSCAN_FINDINFO, HANDLE);								///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_FINDCLOSE_T)(HANDLE);												///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETINTERFACEPARAMS_T)(HANDLE, LPINTERFACE_PARAMS);					///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_SETINTERFACEPARAMS_T)(HANDLE, LPINTERFACE_PARAMS);					///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETDEVICEINFO_T)(HANDLE, LPDEVICE_INFO);							///<  Type definition of function exported from EMDK Library (See EMDK Documentation)
typedef DWORD (WINAPI* LPFN_SCAN_GETVERSIONINFO_T)(HANDLE, LPSCAN_VERSION_INFO);					///<  Type definition of function exported from EMDK Library (See EMDK Documentation)

#define SCAN_GetScanParameters_EXPORT		"SCAN_GetScanParameters_W"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_AllocateBuffer_EXPORT			"SCAN_AllocateBuffer_W"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_DeallocateBuffer_EXPORT		"SCAN_DeallocateBuffer_W"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_ReadLabelEvent_EXPORT			"SCAN_ReadLabelEvent_W"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_ReadLabelMsg_EXPORT			"SCAN_ReadLabelMsg_W"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetScanParameters_EXPORT		"SCAN_SetScanParameters_W"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_FindFirst_EXPORT				"SCAN_FindFirst_W"				///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_FindNext_EXPORT				"SCAN_FindNext_W"				///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_CancelRead_EXPORT				"SCAN_CancelRead"				///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_Disable_EXPORT					"SCAN_Disable"					///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_Flush_EXPORT					"SCAN_Flush"					///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetDecoderParams_EXPORT		"SCAN_GetDecoderParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetEnabledDecoders_EXPORT		"SCAN_GetEnabledDecoders"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetReaderParams_EXPORT			"SCAN_GetReaderParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetScanStatus_EXPORT			"SCAN_GetScanStatus"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetSupportedDecoders_EXPORT	"SCAN_GetSupportedDecoders"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetUPCEANParams_EXPORT			"SCAN_GetUPCEANParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_Open_EXPORT					"SCAN_Open"						///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_RSMGetAttribute_EXPORT			"SCAN_RSMGetAttribute"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_RSMGetSupportedAttributes_EXPORT	"SCAN_RSMGetSupportedAttributes"	///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_RSMSetAttribute_EXPORT			"SCAN_RSMSetAttribute"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetDecoderParams_EXPORT		"SCAN_SetDecoderParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetEnabledDecoders_EXPORT		"SCAN_SetEnabledDecoders"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetReaderParams_EXPORT			"SCAN_SetReaderParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetSoftTrigger_EXPORT			"SCAN_SetSoftTrigger"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetUPCEANParams_EXPORT			"SCAN_SetUPCEANParams"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_Close_EXPORT					"SCAN_Close"					///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_Enable_EXPORT					"SCAN_Enable"					///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_RegisterScanMessage_EXPORT		"SCAN_RegisterScanMessage"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_DeRegisterScanMessage_EXPORT	"SCAN_DeregisterScanMessage"	///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_FindClose_EXPORT				"SCAN_FindClose"				///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetInterfaceParams_EXPORT		"SCAN_GetInterfaceParams"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_SetInterfaceParams_EXPORT		"SCAN_SetInterfaceParams"		///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetDeviceInfo_EXPORT			"SCAN_GetDeviceInfo"			///<  Name of function exported from EMDK Library (See EMDK Documentation)
#define SCAN_GetVersionInfo_EXPORT			"SCAN_GetVersionInfo"			///<  Name of function exported from EMDK Library (See EMDK Documentation)

class CScanner;
/**
*  Prototype for functions stored in map used to hash method names to the functions
*  which will set those values.
*  \param szPropertyName The name of the property being set
*  \param szPropertyValue The value to set the property to
*  \return Whether or not the property was successfully set
*/
typedef BOOL (CScanner::*fnMetaTagParse)(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
/**
*  Prototype for functions stored in map used to hash method names to the functions
*  which will retrieve those values.
*  \param szPropertyName The property being retrieved
*  \param szPropertyValue [out] parameter, populated with the value of the property.  
*         Memory for this parameter must be provided by the caller, specify this as NULL
*         to ascertain the length of the required string before calling.
*  \return The number of characters required to hold the value or -1 if there was as error retrieving the value.
*/
typedef int (CScanner::*fnEMMLTagGet)(LPCTSTR szPropertyName, WCHAR* szPropertyValue);

/**
 *  Many properties can not be set until the scanner is enabled, those values received
 *  before the scanner is ready are stored in a queue.  The MetaTagQEntry defines 
 *  an individual entry in that queue.
 */
struct MetaTagQEntry
{
	WCHAR* szParameter;	///<  The Property to be set
	WCHAR* szValue;		///<  The value to set the property to
};

/**
*  The Internal application state, used primarily to keep track of bluetooth 
*  scanners.
*/
enum ScannerState 
{ 
	NO_BT_CONNECTION,			///<  Initial State for all Scanners, Non BT Scanners will never leave this state.
	BT_CONNECTED,				///<  Bluetooth Scanner is connected to the device. 
	WAITING_FOR_BT_RECONNECTION,///<  Bluetooth Scanner has become disconnected from the device and is waiting for a reconnection.
	WAITING_FOR_BT_CONNECTION	///<  Waiting for the user to scan the BT association barcode.
};

/**
*  Enumeration of the types of data the scanner can return, as supported
*  by the EMDK.
*/
enum eScanBufferDataFormat
{
	BUFFER_TEXT		= TRUE,	///<  Barcode is returned as text (default)
	BUFFER_RAW_DATA = FALSE	///<  Barcode is returned as binary data, useful particularly for image barcodes such as Signature decoder.
};

/**
*  Enumeration of the status for the BT Pairing Barcode, whether it is displayed or not
*  during enabling.  We keep track of this as we need to apply it between 'Open' and 'Enable'
*/
enum eBTPairing
{
	BT_PAIRING_BARCODE_INITIAL	= -1,	///<  User has not requested state of displayBTAddressBarcodeOnEnable, this must be the first entry 
	BT_PAIRING_BARCODE_DISPLAY	= TRUE,	///<  User has specified displayBTAddressBarcodeOnEnable:True
	BT_PAIRING_BARCODE_HIDDEN	= FALSE	///<  User has specified displayBTAddressBarcodeOnEnable:False
};

/**
*  Structure into which is placed the last data received from scanner.
*  This is populated following each successful read
*/
struct tagScanData
{
	WCHAR*	szData;			///<  The barcode data (either text or binary as per the buffer format), note this value is assigned dynamically as the size of the buffer is user defined.
	WCHAR	szLength[8];	///<  The length of the returned data (in either characters or bytes, depending on buffer format)
	WCHAR	szTimeStamp[12];///<  The time the barcode was scanned
	WCHAR	szType[8];		///<  The type of  barcode read, as defined by EMDK Label Type
	WCHAR	szSource[MAX_SOURCE_SIZE];	///<  The source device (e.g. SCN1) and human readable decoder type of the decoded barcode or symbol
	WCHAR	szDirection[12];///<  The direction in which the barcode was scanned, only applicable to 1D scanners.
};

/**
*  Array of properties / methods which can be set even when the scanner is disabled.  All other
*  methods / properties will be queued until the scanner is enabled is being set.  Requests to
*  retrieve other settings prior to the scanner being enabled will fail.
*/
static WCHAR* wAllowedTagsWhilstNotEnabled[] = 
{
	L"Enable",
	L"Enabled",
	L"Disable",
	L"Disabled",
	L"Enumerate",
	L"Init",
	L"Initialise",
	L"Deinit",
	L"Deinitialise",
	L"EnumScannerEvent",
	L"DecodeEvent",
	L"EnumSupportedDecodersEvent",
	L"BluetoothStatusEvent",
	L"rsmGetEvent",
	L"DisplayBTAddressBarcodeOnEnable",
	L"EnableTimeout",
	L"ScanType",
	L"DataBufferSize",
	L"BarcodeDataFormat",
	L"FriendlyName",
	L"LowBatteryScan",
	L"DisableScannerDuringNavigate",
	L"DecodeVolume",
	L"DecodeDuration",
	L"DecodeFrequency",
	L"InvalidDecodeFrequency",
	L"DecodeSound",
	L"InvalidDecodeSound",
	L"AutoEnter",
	L"AutoTab",
	L"ConnectTrigger",
	L"DisconnectTrigger",
	NULL	//  Must terminate with a NULL
};

/**
*  JSON names associated with the deviceCapabilitiesEvent.  These names must match those given in the help file.
*/
static WCHAR* szDeviceCapabilitiesEventNames[] = 
{
	L"supportedDecoders",
	L"supportedBeamWidths",
	L"aimModeSupported",
	L"scanDirectionReportingSupported",
	L"supportedImageFormats",
	L"maximumImageRectangle",
	L"dpmAvailable",
	L"viewfinderSupported",
	L"adaptiveScanningSupported",
	L"interfaceType",
	L"hardwareVersion",
	L"decoderVersion",
	L"physicalDeviceDriverVersion",
	L"moduleDeviceDriverVersion",
	L"cApiVersion",
	NULL
};
