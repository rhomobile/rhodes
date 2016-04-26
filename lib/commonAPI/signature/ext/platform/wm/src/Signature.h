/**
*  \file Signature.h
*  \brief File to house CSignature class and related includes / defines.
*/

#include <windows.h>
#include "common/RhodesApp.h"
#include "IButtonNotification.h"

#pragma once
//#include "../../common/Public/PBPlugin.h"
#include "PBUtil.h"

#define	WM_MOUSEOUT			WM_USER + 2 

#define MAX_URL	2048

//  Errors
///  Signature Capture module is running low on memory.
#define ERR_MEM			L"Low Memory"
///  Signature capture is unable to register the window class.
#define ERR_CLASS		L"Couldn't register the Window class"
///  Signature Capture error drawing
#define ERR_DIM_XY		L"The window cannot be drawn ouside of the parent window"

//  Defaults
#define WIN_CLASSNAME L"SigBox"		///<  Class name of the Signature Capture window
#define XCOORD		15				///<  Default X position of the Signature Capture window
#define YCOORD		60				///<  Default Y position of the Signature Capture window
#define WIDTH_SIG	200				///<  Default width of the Signature Capture window
#define HEIGHT_SIG	150				///<  Default height of the Signature Capture window
#define BG_COLOR	0xFFFFFF		///<  Default background colour of the window (white)
#define PEN_COLOR	0x000000		///<  Default Pen Colour (black)
#define PEN_WIDTH	3				///<  Default width of the pen.
#define DEFAULT_FILENAME	L"signature"	///<  Default name of the captured bitmap which will be placed in the core.  
#define MAXLEN_VECTORARR	MAX_URL/2		///<  The maximum length of each vectornavigate data string

static TCHAR* tcVectorEventNames[] = 
{
	L"vectorArray",
	NULL
};

static TCHAR* tcSignatureCaptureEventNames[] = 
{
	L"imageData",
	NULL
};


/**
*  A Basic Signature Capture point
*/
typedef struct Point
{
	
	bool  bNewLine;		///<  Helps determine when to draw lines
	unsigned short XPos;///<  Point X Position
	unsigned short YPos;///<  Point Y Position
	Point* pPoint;		///<  Pointer to next point in list
	
}*P_COORDXY;


/**
*  CSignature class wraps the functionality of the Signature Capture box.  
*  Creates a window on the screen which responds to mouse move events by drawing
*  a series of lines allowing a signature to be captured.  The signature can be 
*  captured to a bitmap and transferred to a server or somewhere else on the 
*  device.  Alternatively the signature can be captured as a series of vectors.
*  The signature capture box and associated drawing pen are fully configurable
*  in terms of colour, size and position on the screen.
*/
class CSignature  
{
public:  
	
	
	
	//  Methods

	/**
	*  Initialise Variables
	*  \param hInstance		HINSTANCE of the parent window on top of which the 
	*						signature capture window will be displayed.
	*  \param hParent		HWND of the parent window on top of which the 
	*						signature capture window will be displayed.
	*  \param iInstanceID	Unique identifier of the PocketBrowser instance in which 
	*						this signature capture module is running.
	*  \param pModule		Pointer to the PocketBrowser CSignatureModule class
	*						which created this signature.  Used for logging and 
	*						sending navigations back to the core.
	*/
	CSignature			(HINSTANCE hInstance,HWND hParent, int iInstanceID, bool bBuiltAsRhodesExtension);


	/**
	*  Used to free memory and  values
	*/
	~CSignature	();
		
	/**
	*  Draw a border around the signature capture window.
	*  \param bOn Whether or not to draw a border
	*  \return TRUE
	*/
	BOOL SetBorder		(BOOL bOn);

	/**
	*  Set the width of the pen
	*  \param iWidth Pen width, must be greater than 0
	*  \return TRUE if the passed parameter is valid
	*/
	BOOL SetLineWidth	(int iWidth);

	/**
	*  Set the colour of the drawing pen.
	*  \param lpSzLineColor Colour to set the line to, expressed as a web colour, 
	*  e.g. #FFFFFF.
	*  \return TRUE if the specified line colour is valid
	*/
	BOOL SetLineColor	(LPCTSTR lpSzLineColor);

	/**
	*  Set the background colour of the signature capture window.
	*  \param lpSzColor Colour to set the background to, expressed as a web 
	*  colour, e.g. #FFFFFF.
	*  \return TRUE.
	*/
	BOOL SetBgColor(LPCTSTR lpSzColor);

	/**
	*  Sets the width of the signature capture window, if the window already 
	*  exists it will change immediately.
	*  \param iWidth The new width of the signature capture box, in pixels.
	*  \return Whether or not the width was successfully set
	*/
	BOOL SetWidth		(int iWidth);

	/**
	*  Sets the height of the signature capture window, if the window already 
	*  exists it will change immediately.
	*  \param iHeight The new height of the signature capture box, in pixels.
	*  \return Whether or not the height was successfully set.
	*/
	BOOL SetHeight		(int iHeight);

	/**
	*  Sets the left position of the signature capture window, if the window 
	*  already exists it will change immediately.
	*  \param iXpos The new x position of the box, in pixels.
	*  \return Whether or not the x position was successfully set.
	*/
	BOOL SetXpos		(int iXpos);

	/**
	*  Sets the top position of the signature capture window, if the window
	*  already exists it will change immediately.
	*  \param iYpos The new y position of the box, in pixels.
	*  \return Whether or not the y position was successfully set.
	*/
	BOOL SetYpos		(int iYpos);

	/**
	*  Specify the default name to be used for the captured signature, this will 
	*  be the name of the file placed in the root of the device when the user
	*  invokes the Capture method.
	*  \param lpSzFilename The new filename to use, no need to include the .bmp
	*         extension as this is done automatically.
	*  \return Whether or not the filename was successfully set.
	*/
	BOOL SetName		(LPCTSTR lpSzFilename); 

	/**
	*  Return the filename used to save the bitmap.
	*  \return Image filename.
	*/
	LPCTSTR GetName ();

	/**
	*  Overloaded function used to register the class and create the window. 
	*  NB: This will use default values
	*  \param buttons Whether to create Clear/Cancel/Capure buttons.
	*  \return Whether or not Signature capture was successfully initialised.
	*/
	BOOL Initialise		(bool buttons, IButtonNotification *pcallback);

	/**
	*  Clear the screen and delete the 'lines' linked list.
	*  \return Whether or not the screen and lines were successfully cleared.
	*/
	BOOL ClearScreen	();

	/**
	*  Shows the signature capture window.  Note in a multiple instance 
	*  environment only the focussed application will have its signature 
	*  capture window shown.
	*  \return Whether or not the window was shown.
	*/
	BOOL Show			();

	/**
	*  Hides the signature capture window.
	*  \return Whether or not the window was successfully hidden.
	*/
	BOOL Hide			();

	/**
	*  Creates a bitmap at the location specified by the user using the name 
	*  parameter, or default to "Signature" in the root.
	*/
	BOOL CreateDIBitmap ();

	/**
	*  Specifies whether or not the signature should be output as vectors, 
	*  this is realised by the VectorNavigateEvent being sent to the user
	*  on pen up.
	*  \param bDoVectors Whether or not the Signature should be output as 
	*                    vectors.
	*/
	void DoVectors		(BOOL bDoVectors);

	/**
	 *  Return the vectors that are stored in this signature object.
	 */
	rho::Vector<rho::String> ReturnVectors();

	/**
	*  Sets the URI to be used when vectors are to be returned to the user.  
	*  This must be set in order for the users to receive the 
	*  VectorNavigateEvent
	*  \param tcNavURI URI to be used when vectors are sent to the user.
	*  \return TRUE if the URI was successfully set.
	*/
	BOOL SetVectorNavigationURI(LPCWSTR tcNavURI);

	/**
	*  Sets the URI to be used in the Signature Capture Event
	*/
	BOOL SetSignatureCaptureURI(LPCWSTR tcNavURI);

	/**
	*  Sends the DataURI encoding of the signature to the user.
	*/
	BOOL CaptureSignature();

	/**
	* Retrieves the encoded image data. CreateDIBitmap() must have been called first.
	*/
	void GetDataURI (rho::String& data);

public:  //  Attributes
	HWND		m_hSigBox;		///<  HWND of the signature capture box
	///  Filename & path of the .bmp file created when the user invokes the 'Capture' method.
	LPTSTR		m_lpSzFilename;	

	IButtonNotification*	m_pCallback;	///< The interface to call back into when a button is pressed
	bool buttons;

private:
	/**
	*  If DoVectors has been set to true and the user has specified a 
	*  VectorNavigate URI then the line vectors are sent using this thread.
	*  Called whenever a pen up event takes place, the data is formatted into 
	*  the navigation string as set by the user with VectorNavigateEvent.  
	*  It is sent in batches if the maximum data is larger than MAXLEN_VECTORARR
	*  \param lParam pointer to the CSignature object associated with the 
	*                thread.
	*  \return 0 on success.
	*/
	static  DWORD WINAPI	sendVectorsThread(LPVOID lParam);
	
	/**
	*  Creates a bitmap of the signature capture window.
	*  \param szFilePathName Location to store the bitmap, e.g. 
	*                        L"\\program Files\\Symbol\\Signature.bmp"	
	*  \return Whether or not the bitmap was successfully created
	*/
	BOOL CreateDIBitmap (LPTSTR szFilePathName);
	
	/**
	*  Converts a web colour string to a COLORREF
	*  \param lpSzColor String representation of colour, e.g. #FFFFFF
	*  \param pCrColor [out] COLORREF representation of lpSzColor.
	*  \return Whether or not the conversion was successful.
	*/
	BOOL strToCR		(LPCTSTR lpSzColor,COLORREF* pCrColor);

	/**
	*  Determines whether or not the given coordinates are sensible for the 
	*  Signature Capture box given the size of the current screen.
	*  \param iCoordX The X coordinate
	*  \param iCoordY The Y Coordinate
	*  \param iWidth The requested box width
	*  \param iHeight The requested box height
	*  \return True if a box at x,y with specified width and height will fit
	*  on the screen.
	*/
	BOOL Validate(int iCoordX, int iCoordY,int iWidth, int iHeight);
	
	/**
	*  WNDPROC for the signature capture box.
	*  \param hSigBox HWND of the signature capture box
	*  \param message Message received.
	*  \param wParam WPARAM of the Message
	*  \param lParam LPARAM of the Message
	*  \return Either TRUE or the result of calling the parent's WNDPROC
	*/
	static BOOL CALLBACK	SigProc(HWND hSigBox,UINT message,WPARAM wParam,LPARAM lParam);

	/**
	*  Redraw the signature capture box with the latest coordinates.
	*  \return Whether or not the redraw was successful
	*/
	BOOL ReDraw			();// with the latest coords

	/**
	*  This repaints the Signature box using the saved vectors
	*  \param hDc Device Context
	*  \param bFill Whether or not to fill the rectangle
	*/
	void		RePaintBox		(HDC hDc,BOOL bFill,BOOL isTakeFullScreen);

	/**
	*  Used to draw the background of the signature box.
	*  \param hWnd HWND of signature capture box
	*  \param hDc Device Context
	*  \param rt Rectangle to draw
	*  \param crColor Colour to draw the rectangle
	*  \param bFilled Whether or not to fill the rectangle
	*/
	void		DrawRect		(HWND hWnd, HDC hDc, RECT rt, COLORREF crColor, BOOL bFilled);

	/**
	*  Converts from web color reference (RGB) to COLOREF (BGR)
	*  \param dWebColor The Web Colour to convert
	*  \return The colour converted into a COLORREF
	*/
	COLORREF	WebToColorRef	(DWORD dWebColor);

	/**
	*  Converts from COLOREF to a RGBQUAD structure, used by CreateDIBBitmap 
	*  to create the Bitmap.
	*  \param cColor the COLORREF to convert
	*  \return The COLORREF converted into an RGBQUAD value.
	*/
	RGBQUAD		CRefToRGB		(COLORREF cColor);
	
	/**
	*  Called during mouse movement this function adds coordinates to the 
	*  signature.  Store a line's data in memory.
	*  \param xCoord X Coordinate of the point to add.
	*  \param yCoord Y Coordinate of the point to add.
	*  \param bNewLine Whether or not this point is the beginning of a new 
	*  line.
	*  \return the added coordinate in a structure, or NULL if there was an 
	*  error
	*/
	P_COORDXY	AddCoord		(int xCoord, int yCoord,bool bNewLine);

	/**
	*  Remove the list of lines (which represents the signature) from memory.
	*/
	void		DeleteSigMem	();

	/**
	*  Converts a hex TCHAR to a byte value.
	*  \param chHex HEX TCHAR value to convert.
	*  \return chHex converted to a byte.
	*/
	BYTE		CharHexToByte	(TCHAR chHex);
	
	/**
	*  Called whenever the user lifts the stylus or goes outside the capture 
	*  area this function calls the sendVectors thread to transmit the
	*  signature vectors via the VectorNavigate Event.
	*/
	void		SendVectors		();

	
	BOOL		m_bSigning;			///<  Used to test if the signature went offscreen
	DWORD		m_BorderPadding;	///<  Width of the Signature capture box's border.
	RECT		m_Rect;				///<  RECT of the signature capture box
	bool		m_bNewLine;			///<  Whether or not we are adding a new line, used when listening for mouse movement
	P_COORDXY	m_pPrevPos;			///<  Pointer to the last added point in the linked list of points representing the signature
	P_COORDXY	m_Head;				///<  Head of the linked list of points representing the signature.
	P_COORDXY	m_pCurrentPos;		///<  Pointer to the currently adding point in the linked list of points representing the signature
	P_COORDXY	m_pCurrentVector;	///<  Vectors are stored in a linked list, this attribute is the currently adding vector
	BOOL		m_DoVectors;		///<  Whether or not vectors should be sent to the user whenever a signature is captured.  Defaulting to false this controls whether or not the SendVectorsThread is started.
	TCHAR		m_szError[100];		///<  Holds error information, currently unused.
	TCHAR		WcName[20];			///<  Holds the class name of the signature capture box.
	HINSTANCE	m_hInstance;		///<  HINSTANCE of the Parent of the Signature capture box.
	
	HWND		m_hParent;			///<  HWND of the Parent of the Signature Capture Box.
	int			m_iCoordY;			///<  X coordinate of the signature capture area
	int			m_iCoordX;			///<  Y coordinate of the signature capture area
	int			m_iSigBoxHeight;	///<  Height of the Signature Capture Area
	int			m_iSigBoxWidth;		///<  Width of the Signature Capture Area
	int			m_iLineWidth;		///<  Line thickness setting of the pen
	COLORREF	m_crLineColor;		///<  Colour of the line used for the signature pen.
	COLORREF 	m_crBgColor;		///<  Colour of the background used for the Signature Capture area.
	long		m_showBorder;		///<  Whether or not the Signature capture area should have a border (Windows Style)
	BOOL		m_bHidden;			///<  Whether or not the signature capture area is hidden

	TCHAR		m_tcVectorNavigateURI[MAX_URL];	///<  URI Registered to receive VectorNavigate Events
	WCHAR*		m_szSignatureCaptureURI;	///<  Registered URI for Signature Capture event (blob data)
	int			m_iInstanceID;		///<  Unique identifier of the CSignatureModule object owning this CSignature instance.  Used for navigation.
	bool		m_bOutputHex;		///<  Legacy behaviour was to output hex but that's not compatible with JSON, keeping the legacy behaviour as an option
	bool		m_bBuiltAsRhodesExtension;  ///<  Whether or not we're running in RE2
	BYTE*		m_pLastSignatureAsBytes;	///<  The Byte representation of the last captured signature, for purposes of Data URI
	int			m_iSigBytesCount;	///<  The number of bytes in the last captured signature.

	HWND		m_hwndClear;
	HWND		m_hwndCapture;
	HWND		m_hwndCancel;///< The window handle of the Clear button, if any
};
