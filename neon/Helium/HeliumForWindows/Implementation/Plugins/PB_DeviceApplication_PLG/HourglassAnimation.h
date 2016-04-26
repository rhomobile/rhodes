/**
*  \file HourglassAnimation.h
*  \brief File to house CHourglassAnimation class and related includes / defines.
*/

#include "windows.h"

#define ANIMATIONWIDTH	32	///< Width of the animation strip
#define ANIMATIONHEIGHT 32	///< Height of a single animation cell
#define MAXFRAMES		16 	///< Number of frames in the animation strip (bitmap)

/**
*  Class to encapsulate the Hourglass animation cursor.  The hourglass graphic
*  is defined as a resource within this DLL 'Hourglass.bmp' and consists of a 
*  graphic strip of (currently) 16 frames.  When the hourglass is initialised 
*  a new window is created to house the animation, when shown or hidden this 
*  window is shown or hidden accordingly.  The animation effect is achieved by 
*  subclassing the animation window to adapt the paint routine to shift the 
*  'Hourglass.bmp' texture within the window thus creating the illusion of 
*  movement.  There will only be one instance of CHourglassAnimation per 
*  CHourglassModule.
*/
class CHourglassAnimation
{
public:
	/**
	*  HourglassAnimation constructor, initialises variables, loads the 
	*  texture resource into memory and starts the animation thread
	*  \param hInstance Parent application HINSTANCE, used when creating the 
	*  animation window.
	*  \param dwAnimationInterval The time between each frame, i.e. the speed
	*  of the animation.
	*  \param dwStartX The default start X position of the animation window.
	*  \param dwStartY The default start Y position of the animation window.
	*/
	CHourglassAnimation(HINSTANCE hInstance, 
						HWND hParent,
						HANDLE hWindowInitialisedEvent,
						DWORD dwAnimationInterval, 
						DWORD dwStartX,
						DWORD dwStartY);

	/**
	*  Class destructor, Stops the animation thread and frees all memory
	*  associated with the object.
	*/
	~CHourglassAnimation();

	/**
	*  Creates the Animation thread
	*/
	VOID StartThread();

	/**
	*  Set the X position of the Hourglass animation window.  It is possible 
	*  to move the window off the screen if required.
	*  \param iLeft X Position to set the hourglass animation window to.
	*  \return Whether or not the window was successfully moved.
	*/
	BOOL MoveHourglassLeft(int iLeft);

	/**
	*  Set the Y position of the Hourglass animation window.  It is possible 
	*  to move the window off the screen if required.
	*  \param iTop Y Position to set the hourglass animation window to.
	*  \return Whether or not the window was successfully moved.
	*/
	BOOL MoveHourglassTop(int iTop);

	/**
	*  Method to inform the class whether or not its owning module has focus 
	*  or not, i.e. whether the Browser application which owns this 
	*  instance is visible to the user or not.  Only the visible application 
	*  will be able to display the hourglass.
	*  \param bCurrentlyHasFocus Whether or not the owning Browser 
	*  application currently has focus.
	*  \return TRUE
	*/
	BOOL SetFocus(BOOL bCurrentlyHasFocus);

	/**
	*  Interface call used if the module wishes to display the hourglass 
	*  wait cursor.  Handles the logic of whether or not the requesting 
	*  application is visible or not and only actions the request if necessary
	*  (e.g. requesting to show the hourglass when it is already visible is 
	*  ignored)
	*  \param bHourglassShouldBeShown Whether or not the Hourglass should be 
	*  shown.
	*  \return Whether or not the request was actioned successfully. 
	*/
	BOOL RequestToShow(BOOL bHourglassShouldBeShown);

private:  // Methods

	/**
	*  Makes the animation window visible and starts a timer which 
	*  will control the actual animation of the texture.
	*  \return Whether or not the animation was successfully started.
	*/
	BOOL StartAnimation();

	/**
	*  Stop the animiation window from animating (stop processing paint 
	*  messages) and hide the window from view.
	*  \return Whether or not the animation stopped.
	*/
	BOOL StopAnimation();

	/**
	*  Designed to be called from the Animation thread to control the animation.
	*  Creates and sets a new widow procedure for the animation window.  Acts
	*  as a message pump for the animation window.  The animation window is 
	*  given a pointer to its associated CHourglassAnimation object in its
	*  GWL_USERDATA.
	*  \param dwDefaultX Initial X position of the animation window.
	*  \param dwDefaultY Initial Y position of the animation window.
	*  \return 0 when the method exits (which will not be until it is destroyed 
	*  as it is acting as the message pump for the animation window)
	*/
	DWORD Run(DWORD dwDefaultX, DWORD dwDefaultY);

	/**
	*  Windows Procedure registered to the Animation Window.  Calls the 
	*  Paint() method when it receives a WM_TIMER message.  Unprocessed messages
	*  are forwarded back to the owner's message pump.
	*  \param hwnd HWND associated with the animation window
	*  \param message Message received.
	*  \param wParam wParam of the received message.
	*  \param lParam lParam of the received message.
	*  \return 0 for WM_TIMER, else the result of calling the owner window's 
	*  WndProc
	*/
	static LRESULT CALLBACK AnimationWndProc(HWND hwnd, UINT message,
												WPARAM wParam, LPARAM lParam);

	/**
	*  Draws the texture on the animation window, iterating through the 
	*  frames in the animation.  The frequency this function is called is 
	*  determined by a timer started by StartAnimation() and stopped by 
	*  StopAnimation()
	*/
	VOID Paint();

	/**
	*  Separate thread in which the Animation window will run.  This function 
	*  just invokes Run() and all the logic is handled there.
	*  \param lpParam Pointer to a CHourglassAnimation associated with 
	*  the animation window.
	*  \return 0 when the thread exist
	*/
	static DWORD WINAPI AnimationThreadProc(LPVOID lpParam);

private:  //  Attributes
    DWORD 		m_dwTimerInterval;	///<  Animation interval, the time between frames
	LONG 		m_iVisibleFrame;	///<  The index of the frame in the bitmap currently being shown
	HBITMAP		m_hBitmap;			///<  Handle to the animation bitmap
	HWND		m_hwndAnimation;	///<  Handle to the animation window
	HANDLE		m_hExit;			///<  Handle to the Exit Event
	HANDLE		m_hAnimationThread;	///<  Handle to the Animation thread	
	HANDLE		m_hWindowInitialisedEvent;	///<  Handle to the event set when the window is initialised
	LONG		m_bAnimationRunning;///<  Whether or not the animation is running, used by the Paint Method.
	HINSTANCE	m_hInstance;		///<  The Browser HInstance, used to define the owner window of the animation window.
	HWND		m_hParent;			///<  Parent window of the Hourglass animation
	WNDPROC		m_lpfnAnimationProc;///<  Pointer to the animation window's owner WNDPROC
	DWORD		m_dwStartX;			///<  Default Horizontal location of the animation window.
	DWORD		m_dwStartY;			///<  Default Vertical location of the animation window.
	BOOL		m_bModuleIsForeground;	///<  Whether the current module is the foreground window
	BOOL		m_bRequestedToShow;		///<  Whether or not the hourglass has been requested to show (it will be only be shown if this module is the foreground window)
};

