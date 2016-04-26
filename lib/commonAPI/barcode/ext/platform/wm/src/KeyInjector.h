/**
*  \file KeyInjector.h
*  \brief Interface for the CKeyInjector class.
*  This class provides gives the ability to inject keys into the operating system
*  to allow 'Wedge' type behaviour.
*  \author	Darryn Campbell (DCC)
*  \date	January 2012 (DCC, Initial Port from RhoElements 1 Code)
*/
#pragma once
#include <windows.h>
#define ESCAPE_CHAR L'\\'	///< Definition of the escape character


/**
*  Class to inject keys into the operating system to allow 'Wedge' type behaviour.  
*  This class is a cut down version of the RhoElements 1.x CSIP.
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	January 2012, Initial Creation, DCC
*/
class CKeyInjector  
{
public:
	CKeyInjector();		///< Constructor, no implementation.
	~CKeyInjector();	///<  Destructor, no implementation.

	/**
	*  Repeatedly calls the SendKey to send an entire data
	*  string to the keyboard buffer.
	*  \param lpszString String to send
	*/
	void SendString(LPCTSTR lpszString);	
	
	/**
	*  Sends the Keystroke to the keyboard buffer.  Used by the
	*  scanner to 'fake' keystrokes from the input device.
	*  \param cKey Key to send (character)
	*/
	void SendKey(TCHAR cKey);

};
