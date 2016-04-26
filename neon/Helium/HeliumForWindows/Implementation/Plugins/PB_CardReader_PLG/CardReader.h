/**
 * \file CardReader.h
 * \brief Card reader (CardReader) plugin body. This defines the class/
 * variables/methods used by the PB 3.0 card reader plugin API methods
 * to implement the card reader functionality.
 */
#pragma once

#include "../../Common/Public/pbplugin.h"

#include "MsrCApi.h"
#include "Windows.h"
#include "msr3000.h"
#include "../../Common/private/SIP.h"
#include "CardReaderMod.h"

/*
 * DUKPT defines
 */
#define INVALIDREADERHANDLE		-4					///< Error code - Handle to read thread is null
#define CARDREADER_DATA_MAX_PANLENGTH	19			///< Maximum length of string containing PAN
#define CARDREADER_DATA_MIN_PANLENGTH	2			///< Minimum length of string containing PAN

#define CARDREADER_READ			TEXT("CR")			///< Message code indicating data is card reader read data
#define DEBITCARDREADER_ENC		TEXT("ENCDATA")		///< Message code indicating data is extracted PIN value
#define CARDREADER_MSG			TEXT("MESSAGE")		///< Message code indicating data is error message
#define DEBITCARDREADER_PAN		TEXT("PAN")			///< Message code indicating data is encrypted PAN

#define PIN_ENTRY_ON			TEXT("ON")			///< Metatag pinentry value to enable pin entry
#define PIN_ENTRY_OFF			TEXT("OFF")			///< Metatag pinentry value to disable pin entry

#define DCR_TEXT				TEXT("DCR")			///< String that GetVersion supplies, indicating h/w is a DCR
#define MSR_TEXT				TEXT("MSR")			///< String that GetVersion supplies, indicating h/w is an MSR

#define MAX_DRIVER_LEN			50					///< Maximum length of string containing card reader driver dll name
#define MAX_DEVICE_LEN			50					///< Maximum length of string containing card reader device name

static TCHAR* tcReadEventNames[] = 
{
	L"data",
	L"mode",
	NULL
};


/*
 * Typedefs for dynamic pointers to card reader driver functions
 */
/**
 * Template for pointer to driver function that loads and initializes the specified card reader driver. 
 * Access to the card reader hardware is provided through the specified communications port. 
 * A handle to the card reader reader is returned for use in subsequent calls to the card reader API.
 * \param lpszDeviceName  Name of hardware specific magstripe reader dll to open
 * \param lpszPortName  COM Port used by the magstripe reader
 * \param lphReader  Handle to magnetic stripe reader for subsequent operations
 */
typedef RET_STATUS (*LPOPEN)(LPCTSTR lpszDeviceName, LPCTSTR lpszPortName, LPHANDLE lphReader);
/**
 * Template for pointer to driver function that closes the communication port and disables the MSR.
 * \param lphReader  Handle of open reader.
 */
typedef RET_STATUS (*LPCLOSE)(HANDLE lphReader);
/**
 * Template for pointer to driver function that requests card data information from the MSR.
 * \param hReader  Handle of open reader.
 * \param lpszDataBuffer  Pointer to data buffer that will contain MSR data (This parameter must be at least 512 bytes in size).
 * \param dwTimeout  Size of the buffer. This parameter is currently not being used.
 */
typedef RET_STATUS (*LPREADMSRUNBUFFER)(HANDLE hReader, LPCTSTR lpszDataBuffer, DWORD dwTimeout);
/**
 * Template for pointer to driver function that finds the first available MSR DLL and creates 
 * a find handle for use by MSR_FindNext.
 * \param lpMsrFindInfo  Pointer to MSR_FINDINFO structure to be filled in with information about the first available MSR DLL. Information provided by this structure is used to open an MSR with MSR_Open.
 * \param lphFindHandle  Pointer to a new find handle to be used by MSR_FindNext to continue the search. The handle must be closed by MSR_FindClose when done.
 */
typedef RET_STATUS (*LPFINDFIRST)(LPMSR_FINDINFO lpMsrFindInfo,	LPHANDLE lphFindHandle);
/**
 * Template for pointer to driver function that finds the next available MSR DLL in the search 
 * specified by hFindHandle.
 * \param lpMsrFindInfo  Pointer to MSR_FINDINFO structure to be filled in with information about the next available MSR DLL. Information provided by this structure is used to open an MSR with MSR_Open.
 * \param hFindHandle  Find handle created by a call to MSR_FindFirst
 */
typedef RET_STATUS (*LPFINDNEXT) (LPMSR_FINDINFO lpMsrFindInfo, HANDLE hFindHandle);
/**
 * Template for pointer to driver function that closes the find handle created by MSR_FINDINFO.
 * \param hFindHandle  Find handle created by a call to MSR_FindFirst
 */
typedef RET_STATUS (*LPFINDCLOSE)(HANDLE hFindHandle);
/**
 * Template for pointer to driver function that is used to obtain a PIN (Personal Identification Number) 
 * from the user. It first prompts the user to enter their PIN number. It then creates and returns an 
 * EPB (Encrypted PIN Block), which is a combination of the entered PIN and the PAN (Personal Account Number) 
 * that was obtained from reading the Magnetic card.
 * \param hReader  Handle of open reader
 * \param lpszPANDataBuffer  Full PAN from card reader swipe in ASCII null-terminated char string
 * \param Encryption_Format  Encryption format to be used. Currently only the value ISO-0 format is supported. 
 * \param Wait_Time  Timeout in milliseconds
 * \param ubszEncryptedKSNECPAN  Pointer to encrypted PAN data returned by DCR
 */
typedef RET_STATUS (*LPGETPINBLOCK)(HANDLE hReader, LPCTSTR lpszPANDataBuffer, UBYTE Encryption_Format, 
					UHWORD Wait_Time, UBYTE* ubszEncryptedKSNECPAN); 
/**
 * Template for pointer to driver function that cancels any pending MSR read request.
 * \param hFindHandle
 */
typedef RET_STATUS (*LPFLUSH)(HANDLE hFindHandle);

/**
 * Template for pointer to driver function that gets MSR hardware and driver version info.
 * \param hFindHandle
 */
typedef RET_STATUS (*LPGETVERSION)(HANDLE hReader, LPMSR_VERSIONINFO lpMsrVerInfo);

/*
 * Standard DLL function names
 */
#define FUNC_NAME_OPEN				TEXT("MSR_Open")			///< DLL function that loads and initializes the specified MSR DLL.
#define FUNC_NAME_CLOSE				TEXT("MSR_Close")			///< DLL function that closes the communication port and disables the MSR.
#define FUNC_NAME_READMSRUNBUFFER	TEXT("MSR_ReadUnbuffer") 	///< DLL function that requests card data information from the MSR.
#define FUNC_NAME_FINDFIRST			TEXT("MSR_FindFirst") 		///< DLL function that finds the first available MSR DLL and creates a find handle for use by MSR_FindNext
#define FUNC_NAME_FINDNEXT			TEXT("MSR_FindNext") 		///< DLL function that finds the next available MSR DLL.
#define FUNC_NAME_FINDCLOSE			TEXT("MSR_FindClose") 		///< DLL function that closes handle used between MSR_FindFirst and MSR_FindNext
#define FUNC_NAME_GETPINBLOCK		TEXT("MSR_GetPINBlock") 	///< DLL function that is used to obtain a PIN (Personal Identification Number) from the user
#define FUNC_NAME_FLUSH				TEXT("MSR_Flush") 			///< DLL function that cancels any pending MSR read request
#define FUNC_NAME_GETVERSION		TEXT("MSR_GetVersion") 			///< DLL function that cancels any pending MSR read request


#define DEFAULT_DLL_NAME			TEXT("MsrAPI32.dll")		///< Default DLL name to use

#define MAX_DLLS					10		///< Limit to the number of MSR DLL's that will be searched for in the DLL file

#define READ_THREAD_TIMEOUT			2000						///< Max number of milliseconds to wait for read thread to terminate after issuing a stop
#define DEFAULT_PIN_ENTRY_TIMEOUT	30000						///< Default value of pin entry timeout
#define DEFAULT_PIN_ENTRY_TIMEOUT_S	TEXT("30000")				///< Default value of pin entry timeout - string form

/**
 * CCardReader is the class responsible for maintaining the state of the
 * card reader (CR), providing access methods for setting up the card reader
 * driver and generally operating the card reader.
 */
class CCardReader
{
public:
	/**
	 * CardReader-specific actions to be taken upon "Preloading" of the device.
	 * essentially initializing the CardReader object, loading
	 * the card reader DLL and setting object's method pointers to the DLL proper functions
	 * \param pMod  pointer to the parent Module that manages all device instances
	 * \param iID  id of the app to which this instance belongs
	 * \param hWnd handle to the window of the app to which this instance belongs
	 */
	CCardReader(CCardReaderMod *pMod, int iID, HWND hWnd);

	/**
	 * CardReader-specific actions to be taken upon "Disposing" of the device,
	 * including stopping the read thread, deinitializing the object 
	 * referring to the CardReader object, and unloading the CardReader DLL.
	 */
	~CCardReader();

	/**
	 * Steps through all of the card reader drivers in the DLL and finds the one
	 * that successfully opens with the attached device.
	 * \param bFocusSwitch  flag indicating whether this method is being invoked as a result of a change in focus
	 * \return 0  if successful in opening card reader driver and all card reader-related method pointers are non-zero
	 * \return Non-zero if unsuccessful in opening card reader driver or any card reader-related method pointers are 0
	 */
	DWORD	OpenTheCardReader(BOOL bFocusSwitch);

	/**
	 * Responsible for forwarding data from the card reader to the PB core in the form
	 * of a callback via SendPBNavigate(). CardReaderReadThread continually loops, 
	 * waiting on the blocking function m_lpCardReaderReadMsrUnbuffer. Once data is
	 * received, m_lpCardReaderReadMsrUnbuffer returns and CardReaderReadThread sends
	 * the data back to the PB core. If the card reader buffer cannot be successfully
	 * read, then it resets the card reader and tries again. If the device is a debit card reader,
	 * then CardReaderReadThread continues to PIN entry and forwards that data back to
	 * the PB core.
	 * \param lpParam  pointer to object holding state of device to which this thread belongs
	 * \return 0  if thread to terminate normally
	 * \return 1  if error in reading card reader read buffer
	 */
	static	DWORD WINAPI CardReaderReadThread (LPVOID lpParam);

	/**
	 * Stops the card reader by telling the read thread to terminate, closing the card reader driver, and
	 * clearing the thread pointer and handle. Terminates the read thread if it does not 
	 * terminate on its own.
	 * \param bFocusSwitch  flag indicating whether this method is being invoked as a result of a change in focus
	 */
	void	StopTheCardReader(BOOL bFocusSwitch);

	/**
	 * Sends data back to the PB core. Under test conditions (PLUGINTESTER_DEBUG defined) prints this data
	 * to the console in a message box
	 * \param bPinEntry  m_bPinEntry flag of the calling app
	 * \param iID  id of the calling app
	 * \param lpszBuffer  data to be sent to core
	 * \param lpszMessageType  flag indicating nature of message to send to the core
	 */
	void CardReaderMessage(int iID, LPTSTR lpszBuffer, LPTSTR lpszMessageType);

	/**
	 * Get the value of the attribute m_bCardReaderHasBeenOpened
	 * \return  value of the attribute m_bCardReaderHasBeenOpened
	 */
	BOOL	GetCardReaderHasBeenOpened();

	/**
	 * Set the value of the attribute m_lpszCardReaderNavString to the first MAX_URL chars of String
	 * \param lpszValue  value with which to set m_lpszCardReaderNavString
	 */
	void	SetCardReaderNavString(LPCTSTR lpszValue);

	/**
	 * Get the value of the attribute m_hThreadRead
	 * \return  value of the attribute m_hThreadRead
	 */
	HANDLE	GetThreadReadHandle();

	/**
	 * Set the value of the attribute m_hThreadRead
	 * \param hHandle  value with which to set m_hThreadRead
	 */
	void	SetThreadReadHandle(HANDLE hHandle);

	/**
	 * Set the value of the attribute m_bAutoTab. If the new value is invalid,
	 * leave the value unchanged.
	 * \param lpszValue  value with which to set m_bAutoTab
	 * \return TRUE  if the new value is valid
	 * \return FALSE  otherwise
	 */
	BOOL	SetAutoTab(LPCTSTR lpszValue);

	/**
	 * Set the value of the attribute m_bAutoEnter. If the new value is invalid,
	 * leave the value unchanged.
	 * \param lpszValue  value with which to set m_bAutoEnter
	 * \return TRUE  if the new value is valid
	 * \return FALSE  otherwise
	 */
	BOOL	SetAutoEnter(LPCTSTR lpszValue);

	/***********************************************************************/
	/********************** DebitCardReader Support ************************/
	/***********************************************************************/
	/**
	 * Parses the card data and picks out the PAN (personal account number) Data
	 * \param lpszCardData  data read from card
	 * \param lpszPANData  PAN data extracted from card data
	 * \return TRUE  if able to successfully parse lpSzCardData and put results in lpSzPANData
	 * \return FALSE  otherwise
	 */
	BOOL	retPANData(LPCTSTR lpszCardData, LPTSTR lpszPANData);

	/**
	 * Submits the card reader PAN data to the card reader to produce an encrypted string for server
	 * \param lpszPANData  PAN data to encrypt
	 * \return 1  if able to successfully encrypt card reader PAN data
	 * \return 0  otherwise
	 */
	DWORD	PinTransactionDebitCardReader(LPCTSTR lpszPANData);

	/**
	 * Adds some formatting characters and the PAN data to the beginning of the data
	 * buffer to be sent back to the PB core. If the new value is invalid,
	 * leave the value unchanged.
	 * \param lpszPAN  value of PAN to communicate back to PB core
	 * \return TRUE  if the new value is valid and DCR functionality is enabled
	 * \return FALSE  otherwise
	 */
	BOOL	ManualPANEntry(LPCTSTR lpszPAN);

	/**
	 * Sets the m_Wait_Time variable which indicates the timeout limit when encrypting the 
	 * PAN data by the DebitCardReader. If the new value is invalid,
	 * leave the value unchanged.
	 * \param lpszMilliStr  PIN timeout duration in milliseconds
	 * \return TRUE  if the new value is valid and DCR functionality is enabled
	 * \return FALSE  otherwise
	 */
	BOOL	SetPinTimeout(LPCTSTR lpszMilliStr);

	/**
	 * Sets the m_bPinEntry variable which indicates whether PIN entry is enabled or not.
	 * If the new value is invalid, leave the value unchanged.
	 * \param lpszSW  if szSW has value PIN_ENTRY_ON, then enable PIN entry, else disable
	 * \return TRUE  if the new value is valid and DCR functions have been successfully loaded
	 * \return FALSE  otherwise
	 */
	BOOL	SetPinEntry(LPCTSTR lpszSW);

	/**
	 * Sets the m_bDCR variable which indicates whether device is a DCR or not.
	 * \param bValue  value used to set m_bDCR flag
	 */
	void	SetDCR(BOOL bValue);

	/**
	 * Sets the m_lpszDriverName variable which holds the name of the base level
	 * MSR/DCR driver dll file to be used in opening the card reader
	 * If lpszModuleName is not a recognized module name, m_lpszDriverName is left
	 * unchanged.
	 * \param lpszModuleName  name of the card reader module (device)
	 * \return TRUE  if the new value is valid and the name has been successfully set
	 * \return FALSE  otherwise
	 */
	BOOL	SetDriverName(LPCTSTR lpszModuleName);

	/***********************************************************************/

private:
	BOOL				m_bCardReaderHasBeenOpened;	////< Flag indicating whether or not the card reader DLL library has already been loaded
	TCHAR				m_lpszCardReaderNavString[MAX_URL];	///< Maintains the string to be sent back to the PB core via SendPBNavigate whenever the card reader plugin has data to communicate back to the core.
	HANDLE				m_hThreadRead;			///< Handle of the read thread responsible for reading the card reader buffer
	HANDLE				m_hReader;				///< Handle of the reader object/thread maintained by the card reader driver
	int					m_iCardReaderCount;			///< A count of the number of unique card reader drivers found within the card reader DLL
	MSR_FINDINFO		m_CardReaderFindInfo[MAX_DLLS];	///< A list of the unique card reader drivers found in the card reader DLL.
	MSR_VERSIONINFO		m_CardReaderVer;			///< Data structure containing info on cardreader h/w and driver
	int					m_iCardReaderFindIndex;		///< Index of the card reader driver, amongst the set of drivers in card reader DLL, that successfully opens with the attached device
	BOOL				m_bKeyEntry;				///< Indicates that user has entered PAN manually
	BOOL				m_bAutoTab;					///< Indicates that key sequences returned via SendKeys are to have tabs appended
	BOOL				m_bAutoEnter;				///< Indicates that key sequences returned via SendKeys are to have enter appended
	TCHAR				m_lpszDriverName[MAX_DRIVER_LEN];	///< Name of the driver dll to be used with the attached device
	TCHAR				m_lpszDeviceName[MAX_DEVICE_LEN];	///< Name of the attached device

	HINSTANCE			m_hCardReaderLibrary;		///< Handle to card reader DLL being used
	LPOPEN				m_lpCardReaderOpen;			///< Pointer to the Open function in the card reader DLL
	LPCLOSE				m_lpCardReaderClose;		///< Pointer to the Close function in the card reader DLL
	LPFINDFIRST			m_lpCardReaderFindFirst;	///< Pointer to the FindFirst function in the card reader DLL
	LPFINDNEXT			m_lpCardReaderFindNext;		///< Pointer to the FindNext function in the card reader DLL
	LPFINDCLOSE			m_lpCardReaderFindClose;	///< Pointer to the FindClose function in the card reader DLL
	LPREADMSRUNBUFFER	m_lpCardReaderReadMsrUnbuffer;	///< Pointer to the ReadMsrUnbuffer function in the card reader DLL
	LPFLUSH				m_lpCardReaderMsrFlush;		///< Pointer to the Flush function in the card reader DLL
	LPGETVERSION		m_lpCardReaderGetVersion;	///< Pointer to GetVersion function in the card reader DLL

	TCHAR				m_CardReaderReadDataBuffer[MAX_CARD_DATA];	///< Data buffer for card reader read. Sent back to PB core when ready.

	CCardReaderMod*		m_pMod;					///< Pointer to the card reader module
	int					m_iInstId;				///< ID of the app that owns this instance
	HWND				m_hWnd;					///< Handle to the owning app's window
	CSIP*				m_pCSIP;				///< Pointer to SIP object for handling SendKeys functionality

	/***********************************************************************/
	/********************** DebitCardReader Support ************************/
	/***********************************************************************/
	BOOL			m_bDCR;						///< Indicates whether or not device is a DCR
	BOOL			m_bPinEntry;				///< Indicates whether PIN entry is enabled or not
	UHWORD			m_iWait_Time;				///< Indicates the timeout limit when encrypting the PAN data by the DebitCardReader
	LPGETPINBLOCK	m_lpCardReaderGetPinBlock;	///< Pointer to the GetPinBlock function in the card reader DLL
	/***********************************************************************/

	/**
	 * Counts the number of card reader drivers found within the DLL
	 * \return  number of card reader drivers found within the DLL. 
	 * \return  0 if no card reader drivers found or card reader library not open
	 */
	int		EnumerateCardReaders();

	/**
	 * Simply closes the card reader driver by calling its MsrClose() method.
	 * Does not unload it. m_hReader must be set to NULL after calling CloseTheCardReader, but 
	 * only after the read thread has been terminated.
	 * \return 0  if able to close the card reader or it was already closed
	 * \return Non-zero  if unable to close the card reader
	 */
	DWORD	CloseTheCardReader(BOOL bFocusSwitch);

	/**
	 * Unloads the card reader driver and sets all of the driver method pointers to NULL prior to
	 * deletion of the CardReader object. Further use of the card reader will require creation of
	 * a new CardReader object.
	 */
	void	UnloadTheCardReader();

	/**
	 * Resets the card reader by closing and then reopening driver to clear any error, 
	 * if necessary.
	 * \return 0  if successful
	 * \return Non-zero  if unable to close card reader or successfully reopen
	 */
	DWORD	ResetTheCardReader();
};
