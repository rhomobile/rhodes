/**
 * \file Comm.h
 * \brief Core functionality for the Comm module.  The class defined in 
 * this file is instantiated by instances of the CommModuleClass to perform
 * the Comm module's functions.  Instances of the CommModule class
 * contain a pointer to an object of the Comm class.
 */
#pragma once

#ifndef	__COMM__
#define	__COMM__

#include "../../../Common/Public/pbplugin.h"
#include "CommModule.h"
#include <windows.h>
#include "../../../Common/Private/SIP.h"

// sizes of the basic data structures in CComm
#define	PORT_NAME_CHAR_COUNT	 100
#define FILE_NAME_CHAR_COUNT	1000
#define	RECV_BUFFER_SIZE		4999
#define	DATA_BUFFER_SIZE		4999
#define	WRITE_BUFFER_SIZE		 999
#define MAX_OUTSTANDING_WRITES	 499

/**
 * Typedef that enumerates the types of comm port writes that can be performed by CComm.
 */
typedef enum comm_write_type{COMM_WRITE_STRING, COMM_WRITE_BYTES, COMM_WRITE_FILE} COMMWRITETYPE;

/**
 * Structure that defines an element in the CComm write queue.  The structure can
 * accomodate writes of all types defined in the COMMWRITETYPE type, and has storage
 * for write buffering and a file name.
 */
typedef struct WriteElement
{
	COMMWRITETYPE	 writeType;								  ///< type of write represented by this queue element
	char			*szWriteBuffer;							  ///< pointer to write data
	TCHAR			 tcWriteFileName[FILE_NAME_CHAR_COUNT+1]; ///< name of file to write (if type is file-write)
	WriteElement	*next;									  ///< pointer to next element in queue
} WRITEELEMENT;

/**
 * Class that contains all Comm module functionality.  The state of the comm 
 * port and all members that affect this state are contained in this class.
 */
class CComm
{
public:
	// set Browser behavior
	/**
	 * Public function which sets the Comm module input timeout in support of 
	 * the InputOn tag.
	 *
	 * Sets the CComm.m_iTimeout member variable.
	 *
	 * Calling this method instructs the Comm module that after the period of 
	 * time specified in the input parameter has elapsed, the module should 
	 * remove the data that has accumulated in the input buffer and send it to 
	 * the application.  If no data has accumulated since the last timeout, 
	 * nothing is done.
	 * Calling this function first clears any existing CComm.m_bCRLF, 
	 * CComm.m_cEndChar, CComm.m_iTimeout, and CComm.m_iCharCount member 
	 * variables, as these are all mutually exclusive.
	 *
	 * \param iTimeout the value of the timeout in milliseconds
	 * \return TRUE
	 */
	BOOL		SetTimeout(int iTimeout);
	/**
	 * Public function which sets the comm input end-character in support of 
	 * the InputOn tag.
	 *
	 * Sets the CComm.m_cEndChar member variable.
	 *
	 * Calling this method instructs the Comm module that after encountering 
	 * the specified end character in the input stream, the moduile should 
	 * remove the data that has accumulated in the input buffer and send it 
	 * to the application.
	 * Calling this function first clears any existing CComm.m_bCRLF, 
	 * CComm.m_cEndChar, CComm.m_iTimeout, and CComm.m_iCharCount member 
	 * variables, as these are all mutually exclusive.
	 *
	 * \param cEndChar the value of the end character to watch for
	 * \return TRUE
	 */
	BOOL		SetEndChar(const char cEndChar);
	/**
	 * Public function which sets the comm input end-characters to CRLF in 
	 * support of the InputOn tag.
	 *
	 * Sets the CComm.bCRLF member variable.
	 *
	 * Passing TRUE to this method instructs the Comm module that, on 
	 * encountering a CR followed immediately by an LF in the input stream, 
	 * the module should remove the data that has accumulated in the input 
	 * buffer and send it to the application.
	 * Calling this function first clears any existing CComm.m_bCRLF, 
	 * CComm.m_cEndChar, CComm.m_iTimeout, and CComm.m_iCharCount member 
	 * variables, as these are all mutually exclusive.
	 *
	 * \param bCRLF the value to set the CRLF flag
	 * \return TRUE
	 */
	BOOL		SetCRLF(const BOOL bCRLF);
	/**
	 * Public function which sets the comm input character-count in support 
	 * of the InputOn tag.  
	 *
	 * Sets the CComm.m_iCharCount member variable if the specified count 
	 * is less than or equal to the size of the input buffer.  
	 *
	 * If the specified value is greater than the size of the input 
	 * buffer, the CComm.m_bCRLF member variable is set to TRUE.
	 * Calling this method instructs the Comm module that after counting
	 * 'iCharCount' characters in the input stream, the data that has 
	 * accumulated in the input buffer should be removed from the input 
	 * buffer and sent to the application.
	 * Calling this function first clears any existing CComm.m_bCRLF, 
	 * CComm.m_cEndChar, CComm.m_iTimeout, and CComm.m_iCharCount 
	 * member variables, as these are all mutually exclusive.
	 *
	 * \param iCharCount the value of the character count to monitor
	 * \return TRUE
	 */
	BOOL		SetCharCount(int iCharCount);
	/**
	 * Public function that resets the CComm.m_bCRLF, CComm.m_cEndChar, 
	 * CComm.m_iTimeout, and CComm.m_iCharCount member variables.
	 *
	 * \return TRUE
	 */
	BOOL		ResetTempCommInputOn();
	/**
	 * Public function which sets the CommNavigate URL to the value passed 
	 * in the url string.
	 *
	 * \param url string containing the URL to set CComm.m_tcNavigateUrl
	 * \return TRUE if successful
	 */
	BOOL		SetNavigateUrl(const TCHAR *url);
	/**
	 * Public function which sets the CComm.m_bAutoEnter flag.  This flag, 
	 * if TRUE, appends a CR (carriage return) character to the end of each
	 * block of data sent via a CSIP::SendString (called if no URL is set).
	 *
	 * \param TRUE|FALSE
	 * \return TRUE if successful
	 */
	BOOL		SetAutoEnter(const BOOL bEnabled);
	/**
	 * Public function which sets the CComm.m_bAutoTab flag.  This flag, 
	 * if TRUE, appends a tab character to the end of each
	 * block of data sent via a CSIP::SendString (called if no URL is set).
	 *
	 * \param TRUE|FALSE
	 * \return TRUE if successful
	 */
	BOOL		SetAutoTab(const BOOL bEnabled);

	// set comm-port behavior

	/** 
	 * Public function to set the comm port handshake.
	 *
	 * This function sets the CComm.m_DCB.fOutxCtsFlow, CComm.m_DCB.fOutxDsrFlow, 
	 * and CComm.m_DCB.fInX fields of the CComm.m_DCB member variable, which 
	 * contains the settings for the comm port.
	 *
	 * \param tcHandshake Unicode string containing one of -- 'NONE', 'HW', or 'SW' 
	 * to specifiy no handshake, hardware handshaking, or software handshaking 
	 * respecitvely.  Any other string sets the handshake to 'NONE'. String is
	 * case-insensitive.
	 * \return TRUE
	 */
	BOOL		SetHandshake(const TCHAR *tcHandshake);
	/** 
	 * Public function to set the comm port parity.
	 *
	 * This function sets the CComm.m_DCB.Parity field of the
	 * CComm.m_DCB member variable, which contains the settings for the comm 
	 * port.
	 *
	 * \param tcParity Unicode string containing one of -- 'Even', 'Mark', 
	 * 'No Parity, 'Odd', or 'Space' to specifiy the type of parity calculation.  
	 * Any other string sets the parity to 'No Parity'. String is
	 * case-insensitive.
	 * \return TRUE
	 */
	BOOL		SetParity(const TCHAR *tcParity);
	/** 
	 * Public function to set the comm port data-bit count.
	 *
	 * This function sets the CComm.m_DCB.DataBits field of the
	 * CComm.m_DCB member variable, which contains the settings for the comm 
	 * port.
	 *
	 * \param tcDataBits string containing a Unicode character between '5' 
	 * and '9' inclusive, specifiying the number of data-bits per byte.  
	 * Any other values sets the data-bit count to 8.
	 * \return TRUE
	 */
	BOOL		SetDataBits(const TCHAR *tcDataBits);
	/** 
	 * Public function to set the comm port stop-bit count.
	 *
	 * This function sets the CComm.m_DCB.StopBits field of the
	 * CComm.m_DCB member variable, which contains the settings for the comm 
	 * port.
	 *
	 * \param tcStopBits Unicode string containins one of the following -- 
	 * '1', '1.5' or '2', specifiying the number of stop-bits per byte.  
	 * Any other values sets the stop-bit count to 1.
	 * \return TRUE
	 */
	BOOL		SetStopBits(const TCHAR *tcStopBits);
	/** 
	 * Public function to set the comm port baud rate.
	 *
	 * This function sets the CComm.m_DCB.BaudRate field of the
	 * CComm.m_DCB member variable, which contains the settings for the comm 
	 * port.
	 *
	 * \param tcBaudRate Unicode string containing one of -- '110', '300', 
	 * '600', '1200', '2400', '4800', '9600', '14400', '19200', '38400', 
	 * '56000', '57600', '115200', '128000', or '256000' to specifiy 
	 * the comm port baud rate.  Any other string sets the baud rate to '9600'.
	 * \return TRUE
	 */
	BOOL		SetBaudRate(const TCHAR *tcBaudRate);
	/**
	 * Public function to set the comm port.
	 *
	 * This function copies the first five characters in the input string 
	 * (tcPort), to the member variable CComm.m_tcPort.  No checking is 
	 * performed on the contents of the string.
	 *
	 * \param tcPort Unicode string containing the comm port identifier
	 * \return TRUE if string copy succeeds
	 * \return FALSE if string copy fails
	 */
	BOOL		SetPort(const TCHAR *tcPort);
	/**
	 * Public function which causes the specified port to be opened with 
	 * the current comm port settings.
	 *
	 * This function first checks to see whether the port is open 
	 * already, and if so, it returns TRUE.  If the port is not open, 
	 * this function opens it and checks that the handle returned is valid.  
	 * If the handle is valid, the port parameters are applied to the port, 
	 * and the DTR and RTS signals are set.  If the handle is invalid, a 
	 * message is logged and the function returns FALSE. In addition to 
	 * opening and configuring the port, this function creates the following 
	 * events to communicate state between threads -- CComm.m_hCancelPortWrite 
	 * (signals to the write thread that the port is in the process of closing), 
	 * CComm.m_hWriteDataAvailable (indicates that data has been placed in 
	 * the write queue), and CComm.m_hWriteQueueMutex (prevents simultaneous 
	 * access to the write queue pointers).  Additionally if 
	 * the application has specified a timeout-based navigate, the
	 * following events are created -- CComm.m_hCancelUserTimeout 
	 * (signals to the timeout thread that the port is in the process of closing),
	 * CComm.m_hRestartUserTimeout (indicates that data has been received, and 
	 * the timeout timer should restart), and CComm.m_hDataBufferMutex 
	 * (prevents simultaneous access to the read data buffer).	 
	 * The read thread and write thread are also created and launched by this 
	 * function, as is the timeout thread if timeouts have been specified.
	 *
	 * \return TRUE if port successfully opened
	 */
	BOOL		Open();
	/**
	 * Public function which causes the currently opened port to be closed, 
	 * and the read and write threads and associated events to be cleaned 
	 * up and freed from memory.
	 *
	 * This function closes the currently open comm port and frees the threads 
	 * and events currently in use by the module.  First, this function sets
	 * the CComm.m_hCancelPortWrite event to alert the write thread 
	 * that the port is closing.  Next, the function waits on the timeout thread 
	 * to finish in response to its port closing event, and if it has not 
	 * finished within one second, the thread is forcibly terminated.  Once 
	 * this thread has been stopped, its resources are freed.  Next the port is 
	 * closed and its handle freed, and following the closing of the port, 
	 * the write thread is stopped in a manner similar to the timeout thread 
	 * (wait, and if the wait times out, terminate).  The read 
	 * thread is stopped last in a similar manner, with the additional reliance
	 * on the Windows CommWaitEvent function returning when the specified port
	 * has closed, and its resources are also freed.  Events and state 
	 * variables exist to notify all threads to finish and return of their own 
	 * accord, and the thread termination option should not be needed under 
	 * normal circumstances.
	 *	
	 * \return TRUE
	 */
	void		Close();
	/**
	 * Public function which causes a string of ASCII characters to be 
	 * written to the comm port.
	 *
	 * This function creates a new write queue element (mallocs space for a 
	 * WRITEELEMENT type), populates it with the string to be written (passed 
	 * as the function's single argument), and attaches the new write queue 
	 * element to the tail of the write queue list.  It then signals the write 
	 * thread (created when port opened) via the CComm.m_hWriteDataAvailable 
	 * event that there is data available in the queue to be written to the 
	 * port.
	 *
	 * \param tcWriteData Unicode string to be written to port
	 * \return TRUE
	 */
	BOOL		PortWriteString(const TCHAR *tcWriteData);
	/**
	 * Public function which causes a list of bytes to be written to the 
	 * comm port.
	 *
	 * This function creates a new write queue element (mallocs space for a 
	 * WRITEELEMENT type), populates it with the data to be written (passed 
	 * as the function's single argument), and attaches the new write queue 
	 * element to the tail of the write queue list.  The data to be written 
	 * consists of a string of hex values, each preceeded by the '\%' character, 
	 * and each delimited by zero or more white-space characters.  Once the 
	 * queue is updated, the function then signals the write thread (created 
	 * when port opened) via the CComm.m_hWriteDataAvailable event that there 
	 * is data available in the queue to be written to the port.
	 *
	 * \param tcWriteData Unicode string representing the bytes to be written 
	 * to the port
	 * \return TRUE
	 */
	BOOL		PortWriteBytes(const TCHAR *tcWriteData);
	/**
	 * Public function which causes contents of the specified file to be 
	 * written to the comm port.
	 *
	 * This function creates a new write queue element (mallocs space for a 
	 * WRITEELEMENT type), populates it with the name of the file to be written 
	 * (passed as the function's single argument), and attaches the new write 
	 * queue element to the tail of the write queue list.  Once the queue is 
	 * updated, the function then signals the write thread (created when port 
	 * opened) via the m_hWriteDataAvailable event that there is data available 
	 * in the queue to be written to the port.
	 *
	 * \param tcWriteFileName Unicode string which specifies the path of file 
	 * to be written
	 * \return TRUE
	 */
	BOOL		PortWriteFile(const TCHAR *tcWriteFileName);

	// retrieve values

	/**
	 * Public function which returns a pointer being 
	 * used by the Comm module.
	 *
	 * \return pointer
	 */
	PBStruct*	GetPBStruct();

	CComm(PBStruct *pPBStructure, CCommModule *pWrapperClass);
	~CComm(void);

private:
	// thread entry points
	/**
	 * Private function which is the entry point for the Comm module read 
	 * thread.
	 *
	 * This function does nothing more that call the CComm::PortReadFunction 
	 * member function of the instance specified by the function's single 
	 * argument.
	 *
	 * \param instancePointer pointer to the instance of CComm that launched the thread
	 * \return DWORD value returned by CComm::PortReadFunction
	 */
	static DWORD WINAPI PortReadThread(LPVOID instancePointer);
	/**
	 * Private function which is the entry point for the Comm module write 
	 * thread.
	 *
	 * This function does nothing more that call the CComm::PortWriteFunction 
	 * member function of the instance specified by the function's single 
	 * argument.
	 *
	 * \param instancePointer pointer to the instance of CComm that launched 
	 * the thread
	 * \return DWORD value returned by CComm::PortWriteFunction
	 */
	static DWORD WINAPI PortWriteThread(LPVOID instancePointer);
	/**
	 * Private function which is the entry point for the Comm module write 
	 * thread.
	 *
	 * This function does nothing more that call the CComm::UserTimeoutFunction 
	 * member function of the instance specified by the function's single 
	 * argument.
	 *
	 * \param instancePointer pointer to the instance of CComm that launched the thread
	 * \return DWORD value returned by CComm::UserTimeoutFunction
	 */
	static DWORD WINAPI UserTimeoutThread(LPVOID instancePointer);

	/**
	 * Private function which closes the port in a non-blocking fashion.  This
	 * thread is used if the Close() method needs to be called from within
	 * one of the other port threads, which are terminated by Close() and thus
	 * would cause a deadlock if Close() were called directly.
	 *
	 * \param instancePointer pointer to the instance of CComm that launched the thread
	 * \return 0
	 */
	static DWORD WINAPI PortCloseThread(LPVOID instancePointer);

	// thread functionality
	//   these methods are separated out for readability
	//   eliminates the need for instPtr-> prefix required in static methods

	/**
	 * Private method which handles all of the serial port read thread 
	 * functionality.
	 *
	 * This method performs all of the read realted operations for the open
	 * comm port, which include waiting for data to arrive on the port, placing
	 * that data in the CComm.m_cDataBuffer array, and watching the data coming
	 * in for the event specified by the Browser programmer.  This event
	 * can be one of -- observing a CRLF in the datastream, reaching a programmer-
	 * specified character count, observing a programmer-specified character,
	 * or after a programmer-specified time interval has elapsed.
	 *
	 * The first thing this method does is create some temporary storage 
	 * structures and initialize the member variables that will be used to 
	 * perform the data read.
	 * 
	 * The main processing structure for the read function is composed of 
	 * multiple nested loops.  The outer loop is a while loop conditional on 
	 * the port being open.  This checks for the condition where the port was
	 * closed while the current read data is being processed, and if so, it
	 * causes the read thread to exit.  Once in this loop, the method waits 
	 * for a comm event (i.e., data present on the port, or port closed).
	 * When this wait is triggered, the method checks for valid data on the
	 * port and checks that a port closing event was not responsible for
	 * the wait triggering.  If the port is closed, the thread exits.  
	 * Next, if timeouts were specified by the applicaiton, the timout timer
	 * is restarted.  If the timer is running, it restarts, and if it was
	 * stopped, it simply starts up again.  If the port is open, and there 
	 * is data on the port, then that data is processed into the 
	 * CComm.m_cDataBuffer array.
	 *
	 * The processing block steps through the bytes present on the port,
	 * checking for the programmer-specified trigger.  This check is contained
	 * within the large if-then-else block in the inner loop.  Each condition
	 * in this if-then-else block corresponds to one of the four possible
	 * programmer-specified triggers.  If the trigger condition is met, the 
	 * method calls the CComm::SendInputData() method.  The only case where
	 * this is not true is for the timeout-based trigger, which is handled
	 * by a separate timeout thread.  In the case of a timeout, the processing
	 * block simply places the port data in the CComm.m_cDataBuffer array,
	 * relying on the timeout thread to call CComm::SendInputData() when
	 * the timeout occurs.  The CComm.m_hDataBufferMutex prevents the 
	 * modification of the CComm.m_cDataBuffer array and associated pointers
	 * simultaneously by the timeout thread and this method (read thread).
	 *
	 * This method is separate from the CComm::PortReadThread method for 
	 * readability (no need to reference methods and attributes via 
	 * instancePointer-> prefix).
	 *
	 * \return 0 when cancelled via the port close sequence
	 * \return -1 if timeout thread cannot be created when timeout read events 
	 * are spedified
	 */
	DWORD			PortReadFunction();
	/**
	 * Private method which handles all of the serial port write thread 
	 * functionality.
	 *
	 * This method preforms all comm port write operations and controls 
	 * the write queue, which consists of a linked list of WRITEELEMENT 
	 * structures, and which contains all of the information and buffering 
	 * needed to perform a sequence of write operations, one per queue element.
	 *
	 * This method first enters its top-level loop, a while loop that checks 
	 * that the port is open and returns if it is not (after attempting to 
	 * clean up what remains of the write queue, if anything).  This loop is 
	 * used to detect a port closure during a previous write and exit the 
	 * thread if the port has closed.  Within this loop, the thread waits on 
	 * both the CComm.m_hWriteDataAvailable and the CComm.m_hCancelPortWrite 
	 * events.  If the wait is triggered by the CComm.m_hCancelPortWrite event, 
	 * the thread exits.  If it is the CComm.m_hWriteDataAvailable event that 
	 * triggered the wait, then that means there is data in the write queue, 
	 * and this method processes it.
	 *
	 * The processing next enters another while loop which is conditional on 
	 * a non-empty write queue.  This loop prevents this method from missing 
	 * multiple new WRITELEMENET additions to the queue while the current 
	 * write is in process.  Multiple WRITEELEMENT additions that occur during
	 * the processing of an earlier write request will each set the 
	 * CComm.m_hWriteDataAvailable event, but this event will be cleared by 
	 * the very next wait operation, independent of the number of write
	 * requests that arrive during the current write, and only the first 
	 * element added during the write will be processed.  Continuing in the
	 * inner loop while there are write requests to process bypasses the event
	 * and ensures that all requests are processed.  It also is a simple check 
	 * for valid write data prior to initiating the write processing.
	 *
	 * In the inner loop, writes of type COMM_WRITE_FILE are handled in a 
	 * separate block of code from writes of types COMM_WRITE_BYTES or 
	 * COMM_WRITE_STRING.  When the write is complete, the element associated 
	 * with the just-completed write is removed from the queue, and the
	 * head pointer is modified to point to the next queue element.  This 
	 * pointer manipulation occurs within the protection of the 
	 * CComm.m_hWriteQueueMutex mutex, which prevents simultaneous access to 
	 * the queue by this thread and the CComm::PortWriteString, 
	 * CComm::PortWriteBytes, and CComm::PortWriteFile methods of the main
	 * thread, each of which add new elements to the queue.
	 *
	 * After each WRITEELEMENT is processed, control returns to the top of the 
	 * while loop conditioned on data being available in the queue, and any 
	 * queue elements added while the current element was processed are 
	 * processed.  Once the queue has become empty, control returns to the 
	 * while loop that conditional on the port still being open, and which
	 * contains the wait statement.
	 *
	 * This method is separate from the CComm::PortWriteThread method for 
	 * readability (no need to reference methods and attributes via 
	 * instancePointer-> prefix).
	 *
	 * \return 0 when cancelled via the port close sequence -- 
	 * CComm.m_hCancelPortWrite event & CComm.m_bPortOpen attribute
	 */
	DWORD			PortWriteFunction();
	/**
	 * Private method which handles all of the timeout thread functionality.
	 *
	 * This method performs all of the application-specified timeout
	 * related functionality.  Since timeout-based navigation in Browser
	 * is based on a period of inactivity on the serial port which follows
	 * a period of activity, the first thing this thread does is wait for
	 * the read thread to signal the CComm.m_hRestartUserTimeout event, which 
	 * initiates the timer for the first time.  The aforementioned wait also
	 * triggers on CComm.m_hCancelUserTimeout, which signals the timeout
	 * thread to return, stopping the thread.
	 *
	 * The main loop of the timeout thread consists of waiting on the 
	 * CComm.m_hCancelUserTimeout event (end thread), the 
	 * CComm.m_hRestartUserTimeout event (restart the timtout timer), 
	 * or the timeout itself (specified by the CComm.m_iTimeout attribute).  
	 * If the wait is triggered by the CComm.m_hCancelUserTimeout event, 
	 * the method returns and the thread exits.  If the wait is triggered 
	 * by the CComm.m_hRestartUserTimeout event, control returns to the top 
	 * of the loop, and the wait timer is restarted.  
	 * If the wait times out, the data that has accumulated in the read buffer 
	 * since the last timeout is sent to the application, and the method then 
	 * enters another wait for the CComm.m_hCancelUserTimeout event or the 
	 * CComm.m_hRestartUserTimeout event, which will either end the thread or 
	 * restart the timout timer respecitvely.
	 *
	 * This method is separate from the CComm::UserTimeoutThread method for 
	 * readability (no need to reference methods and attributes via 
	 * instancePointer-> prefix).
	 *
	 * \return 0 when cancelled via the CComm.m_hCancelUserTimeout event
	 */
	DWORD			UserTimeoutFunction();
	/**
	 * Private method which converts the data in the read buffer from ASCII 
	 * to Unicode, and sends the data to the navigate thread, which sends it
	 * on to the Browser application.
	 *
	 * This method first walks through the CComm.m_cDataBuffer array 
	 * converting a set of special characters to escaped versions of these 
	 * characters (adds the '\' prefix) and writing them to the local 
	 * cSendData array.  Once the special characters have all been escaped, 
	 * the cSendData array is converted to Unicode and stored in the 
	 * tcSendData array.  The tcSendData array is then sent to the core 
	 * to process.
	 *
	 * This method is blocking, and only needs special 
	 * attention with respect to buffer synchronization if it will be called 
	 * by more than one simultaneous thread.
	 *
	 * \return TRUE
	 */
	BOOL			SendNavigateData();
	/**
	 * When this
	 * event is received, the port is closed, preventing future activity on the serial 
	 * port
	 *
	 * \param instancePointer pointer to the instance of CComm that launched the thread
	 * \return 0 when cancelled by the CComm.m_hCancelPageCancel event
	 */
	DWORD			PageCancelFunction();

	///////////////
	// private data
	PBStruct		*m_pPBStructure;					///< Pointer to the most recent PBStruct
	CCommModule		*m_pWrapperClass;					///< Pointer to the higher-level wrapper class that contains this instance
	CSIP			*m_pCSIP;							///< Pointer to SIP object for handling SendKeys functionality


	// conditions for sending input data to the app
	int				m_iTimeout;							///< Indicates the elapsed period of port inactivity prior to a data-send (navigate) event if data is present in the queue
	int				m_iTempTimeout;						///< Holding location where timeout value is held until the port is opened
	char			m_cEndChar;							///< Specifies an end character that will trigger a data-send (navigate) event
	char			m_cTempEndChar;						///< Holding location where end character value is held until the port is opened
	int				m_iCharCount;						///< Character count that will trigger a data-send (navigate) event
	int				m_iTempCharCount;					///< Holding location where character-count value is held until the port is opened
	BOOL			m_bCRLF;							///< Indicates that the input thread is to trigger a data-send (navigate) on CRLF
	BOOL			m_bTempCRLF;						///< Holding location where CRLF flag is held until the port is opened
	BOOL			m_bAutoEnter;						///< Indicates the CRLF is to be tacked on to the end of the data sent via CSIP::SendString
	BOOL			m_bAutoTab;							///< Indicates the tab character is to be tacked on to the end of the data sent via CSIP::SendString

	// serial port parameters
	TCHAR			m_tcNavigateUrl[MAX_URL+1];			///< String containing the URL to use when performing a Comm-Navigate
	TCHAR			m_tcPort[PORT_NAME_CHAR_COUNT+1];	///< String containing the comm port designator (e.g., "com1:")
	HANDLE			m_hPort;							///< Handle to serial port
	DCB				m_DCB;								///< Windows-defined struct that holds comm params
	COMMTIMEOUTS	m_CommTimeouts;						///< Windows-defined struct describing comm-port timeouts

	// thread handles
	HANDLE			m_hPortReadThread;					///< Thread handle for the port read thread
	HANDLE			m_hPortWriteThread;					///< Thread handle for the port write thread
	HANDLE			m_hUserTimeoutThread;				///< Thread handle for the read timeout thread

	// event handles
	HANDLE			m_hCancelPortWrite;					///< Event set by the CComm::Close member function, which informs the write thread to finish
	HANDLE			m_hCancelUserTimeout;				///< Event set by the CComm::Close member function, which informs the timeout thread to finish
	HANDLE			m_hRestartUserTimeout;				///< Event set by the CComm::PortReadFunction member function, which informs the timeout timer to restart
	HANDLE			m_hWriteDataAvailable;				///< Event set by the public write functions, which indicates data is available in the queue to be written

	// state
	BOOL			m_bSawCR;							///< Indicates that a carriage character was just encountered in the input stream

	// data
	WRITEELEMENT	*m_pWriteQueueHead;					///< Pointer to the head of the write queue
	WRITEELEMENT	*m_pWriteQueueTail;					///< Pointer to the tail of the write queue
	HANDLE			 m_hWriteFile;						///< Handle to file to be written for a CComm::PortWriteFile call
	HANDLE			 m_hWriteQueueMutex;				///< Mutex to prevent simultaneous access of the write queue pointer by the write thread and main thread
	char			 m_cDataBuffer[DATA_BUFFER_SIZE];	///< Array of ASCII characters that serves as the read data input buffer
	int				 m_iDataBufferIndex;				///< Location in input data buffer where next input byte is to be placed
	HANDLE			 m_hDataBufferMutex;				///< Mutex to prevent simultaneous access of the read data buffer by the read thread and timeout thread
	HANDLE			 m_hNavigateUrlMutex;				///< Mutex to prevent the main thread from corrupting URLs that are in the process of being read by the read thread

};

#endif