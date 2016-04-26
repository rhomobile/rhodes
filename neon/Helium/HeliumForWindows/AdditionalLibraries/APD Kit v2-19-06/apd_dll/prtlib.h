/*----------------------------------------------------------------------------*/
/*                                                                            */
/*         External Cordless Printer Driver         Version 2.00              */
/*         Copyright                     Symbol Technologies Ltd              */
/*         Written in Microsoft C 6.00                18/02/2000              */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma pack(1)

#ifndef __PS_CORDLESS
  #define __PS_CORDLESS

#ifdef WIN32

	#ifdef PRTLIB_EXPORTS
	#define PRTLIB_API __declspec(dllexport)
	#else
	#define PRTLIB_API __declspec(dllimport)
	#endif

	#include "Resource.h"

#else

	#define PRTLIB_API
	#define TCHAR char

#endif

extern "C"
{

/*----------------------------------------------------------------------------*/

/*---- Literals --------------------------------------------------------------*/
#define  byte                   unsigned char

/*---- Command Id's ----------------------------------------------------------*/
/* These are passed as parameters to the external routine                     */
/* To indicate the external function to perform                               */

#define Cmd_PSBuildLabel        0x0001
#define Cmd_PSEndLabel          0x0002
#define Cmd_PSEndSession        0x0003
#define Cmd_PSGetPorts          0x0004
#define Cmd_PSNewLabel          0x0005
#define Cmd_PSSelectPrinter     0x0006
#define Cmd_PSSendLabel         0x0007
#define Cmd_PSPrinterStat       0x0008
#define Cmd_PSStartSession      0x0009
#define Cmd_PSCatLabel          0x000a
#define Cmd_WakeupCOMTEC        0x000b
#define Cmd_PSSetParameter      0x000c
#define Cmd_PSPrinterReady      0x000d
#define Cmd_PSLink              0x000e
#define Cmd_PSComputeComtecID   0x000f
#define Cmd_PSHashComtecID      0x0010
#define Cmd_PSConnectType       0x0011
#define Cmd_PSPrnType           0x0012
#define Cmd_PSPrnPort           0x0013
#define Cmd_PSSetDeliveryMode   0x0014

#define Cmd_PSSendFormat        0x0100
#define Cmd_PSSendData          0x0101
#define Cmd_PSDisplaySetup      0x0102
#define Cmd_PSSetupPrinter      0x0103
#define Cmd_PSGetVersion        0x0104
#define Cmd_PSSetPrinter        0x0105
#define Cmd_PSGetPrinterList    0x0106
#define Cmd_PSGetLastMessage    0x0107
#define Cmd_PSPrintIdLabel      0x0108
#define Cmd_PSGetResponse       0x0109
#define	Cmd_PSSendString		    0x010a
#define Cmd_PSSetTemplatePath	  0x010b
#define Cmd_PSSetFormatPath		  0x010c
#define Cmd_PSSetIDLabelPath		0x010d
#define Cmd_PSOpenPort          0x010e
#define Cmd_PSClosePort         0x010f
#define Cmd_PSPowerOn           0x0110
#define Cmd_PSPowerOff          0x0111
#define Cmd_PSHideProgress      0x0112
#define Cmd_PSShowProgress      0x0113
#define Cmd_PSBthPowerOn        0x0114
#define Cmd_PSBthPowerOff       0x0115

// Alternative definitions
#define Cmd_PSetTemplatePath	  0x010b
#define Cmd_PSetFormatPath		  0x010c
#define Cmd_PSetIDLabelPath     0x010d

#define PRINTER_DRIVER_NAME		  "\\program files\\prtdrv.exe"
#define PRINTER_DATA_FILE       "\\program files\\prtdrv.prn"
#define PRINTER_RESPONSE        "\\program files\\prtdrv.rsp"
#define PRINTER_MESSAGE         "\\program files\\prtdrv.msg"
#define PRINTER_CONFIG          "\\program files\\prtdrv.cfg"
#define PRINTER_TEMP            "\\program files\\prtdrv.tmp"

#define	PRN_ID_LABEL			"ID"

#define PRN_KEY_PRINT_SETUP     0x0010  // Key code to enter printer setup.
#define PRN_KEY_PRINT_QUIT      0x0011  // Key code to exit printer setup.
#define PRN_KEY_PRINT_LABEL     0x000c  // Key code to print id label.

#define PRN_RASCAL              'R'
#define PRN_MONARCH             'M'
#define PRN_TEC                 'T'
#define PRN_ZEBRA               'Z'
#define PRN_COURIER             'D'
#define PRN_COMTEC              'C'
#define PRN_ELTRON              'E'
#define PRN_NONE                '0'

#ifndef TRUE
  #define TRUE                  1
#endif
  
#ifndef FALSE
  #define FALSE                 0
#endif

/*---- Prototypes ------------------------------------------------------------*/
/* Generic command interface (This should not be called directly).            */
PRTLIB_API int     PSExternal      (int wCmd, TCHAR *szParam);                       


/*---- Macros and Prototypes -------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*---- Get external driver version -------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This routine allows the application to retrieve the external printer       */
/* driver version. The function returns a (char*) pointer.                    */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/*  fputs(PSGetVersion());                                                       */
/*----------------------------------------------------------------------------*/

PRTLIB_API TCHAR  *PSGetVersion(void);


/*----------------------------------------------------------------------------*/
/*---- Display Set-up screen. ------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This routine provides a simple user interface to set the printer type and  */
/* connection parameters by entering the Peripheral device Identifier (PID).  */
/* This routine can by bypassed if the application directly sets the          */
/* configuration parameters using "PS_SetPrinter".                            */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* PSDisplaySetup();                                                          */
/*----------------------------------------------------------------------------*/

#define PSDisplaySetup()        PSExternal(Cmd_PSDisplaySetup, NULL)


/*----------------------------------------------------------------------------*/
/*---- Set the current printer ID. -------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This routine allows the application to directly set the current peripheral */
/* device by sending the device’s PID. If the new PID is already in the       */
/* driver’s list, it is moved up to the first position and becomes the        */
/* ‘current’ device. If it is not in the list, it is inserted at the first    */
/* position.  In either case, any other devices in the list are moved down.   */
/* If the total number of devices exceeds four the lowest entry in the list   */
/* will be removed.                                                           */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* PSSetPrinter("1234R");                                                     */
/*----------------------------------------------------------------------------*/

#define	PSSetPrinter(pcId)	  PSExternal(Cmd_PSSetPrinter, pcId)


/*----------------------------------------------------------------------------*/
/*---- Send Label Format/File to current printer -----------------------------*/
/*----------------------------------------------------------------------------*/
/* This routine opens a file containing the printer commands and sends them   */
/* through the protocol and interface driver to the printer. The file name    */
/* extension is derived from the printer type. If an application supports a   */
/* range of printer types, each type will need a format file containing the   */
/* commands to produce the label.  Since the format files are different for   */
/* each printer type, the actual label stock and field layout can vary with   */
/* each type, even though they contain the same variable data.                */
/*                                                                            */
/* Printers that do not support the pre-downloading of a format use format    */
/* files in which the positions for variable data fields are marked by        */
/* $$[..]$$. These files are not sent when this command is used.              */
/*                                                                            */
/* This function can also be used to send ready-formatted data to printers,   */
/* and other devices.                                                         */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* if (!PSSendFormat("Label1"))                                               */
/*   puts("Label format sent OK!");                                           */
/* else                                                                       */
/*   puts(PSGetLastMessage());                                                */
/*----------------------------------------------------------------------------*/

#define PSSendFormat(pcFile)    PSExternal(Cmd_PSSendFormat, pcFile)

/*----------------------------------------------------------------------------*/
/*---- Print routine (Sends variable data) -----------------------------------*/
/*----------------------------------------------------------------------------*/
/* This accepts variable data as a list of char pointers.  The first field    */
/* in the list specifies the label format name or number, subsequent fields   */
/* contain the variable data.  The list is terminated with a NULL pointer.    */
/* The printer driver adds the appropriate commands and passes the data       */
/* through the protocol and interface driver and on to the printer.           */
/*                                                                            */
/* Note that details of the printer’s command language must be defined in     */
/* the PRTDRV.INI file before this feature will work.                         */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* char *pcData[]  = {"1",                                                    */
/*                    "Baked Beans",                                          */
/*                    "5010010123458",                                        */
/*                    "0.45p",                                                */
/*                    "12",                                                   */
/*                    "12/06/2000",                                           */
/*                    NULL};                                                  */
/*                                                                            */
/* if (!PSSendData(pcData,1))                                                 */
/*   puts("Label printed OK!");                                               */
/* else                                                                       */
/*   puts(PSGetLastMessage());                                                */
/*----------------------------------------------------------------------------*/

PRTLIB_API int     PSSendData(TCHAR **pcData, int iQty);


/*----------------------------------------------------------------------------*/
/*---- Get last error message ------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This returns a C char pointer with a text message up to 20 characters in   */
/* length indicating the most recent error (not necessarily the most recent   */
/* result).  This message may then be incorporated into an application        */
/* generated message.                                                         */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* if (!PSSendFormat("Label1"))                                               */
/*   puts("Label format sent OK!");                                           */
/* else                                                                       */
/*   puts(PSGetLastMessage());                                                */
/*----------------------------------------------------------------------------*/

PRTLIB_API TCHAR  *PSGetLastMessage(void);


/*----------------------------------------------------------------------------*/
/*---- Get printer response --------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This command is printer specific and is included to allow an application   */
/* to call complex, printer specific commands.                                */
/* The actual reponse from the printer is stored in a file defined as:        */
/* PRINTER_RESPONSE                                                           */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* if (!PSGetResponse("~HS"))                                                  */
/* {                                                                          */
/*   if ((psHandle = fopen(PRINTER_RESPONSE,"rb")) != NULL)                   */
/*   {                                                                        */
/*     fgets(pcBuff,sizeof(pcBuff),psHandle));                                */
/*     fclose(psHandle);                                                      */
/*   }                                                                        */
/* }                                                                          */
/*----------------------------------------------------------------------------*/

#define PSGetResponse(pcCommand)    PSExternal(Cmd_PSGetResponse, pcCommand)


/*----------------------------------------------------------------------------*/
/*---- Get a list of available printers --------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This returns an array of C char pointers terminated with a NULL, listing   */
/* the currently available devices.  Each device is represented by its PID,   */
/* followed by a comma, then the description.  The first device in the list   */
/* is the current device.                                                     */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/* char	**pcList;                                                            */
/*                                                                            */
/* if ((pcList = PSGetPrinterList(&pcList)) != NULL)                          */
/*   printf("Current printer is %s\r\n",pcList[0]);                           */
/*----------------------------------------------------------------------------*/

PRTLIB_API TCHAR **PSGetPrinterList(void);


/*----------------------------------------------------------------------------*/
/*---- Send request to print an ID label -------------------------------------*/
/*----------------------------------------------------------------------------*/
/* This issues a request to print an ID label using the supplied label format.*/
/* The file extension of the ID label is used to determine the transmission   */
/* protocol                                                                   */
/*                                                                            */
/*---- Example ---------------------------------------------------------------*/
/*  PSPrintIdLabel("Id.r");                                                     */
/*----------------------------------------------------------------------------*/

#define PSPrintIdLabel(pcLabel) PSExternal(Cmd_PSPrintIdLabel, pcLabel)

} // end of extern "C"

#endif
