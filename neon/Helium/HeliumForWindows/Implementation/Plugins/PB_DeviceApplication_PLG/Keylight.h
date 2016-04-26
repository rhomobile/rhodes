/**
 *  \file Keylight.h
 *  \brief Keylight module workhorse class, holds the CKeylight object to 
 *  interface with the keylight hardware via the EMDK.  
 */

#include "KeylightCAPI.h"
#include "../../common/Public/PBPlugin.h"

#pragma once

/**
*  Class to interface with the keylight hardware via the EMDK API.  To use this 
*  class create an instance of it and call Initialise() upon it.  The keylight 
*  can be turned on and off via TurnOn() / TurnOff() and the intensity to 
*  illuminate the keylight (when turned on) can be set via SetIntensity(...).
*  The intensity levels supported will vary from device to device.  This module 
*  currently only supports the VC6090 keylight but will be expanded to other 
*  devices once this functionality is exported by the EMDK.
*/
class CKeylight
{
public:
	/**
	*  Constructor for the Keylight Object, initialises variables and loads 
	*  the EMDK Keylight library.
	*  \param iInstanceID Unique Browser application Identifier
	*  \param pModule Pointer to associated PBModule which represents the PB
	*  instance to which this object is related.
	*/
	CKeylight(int iInstanceID, PBModule* pModule);
	
	/**
	*  Unloads the EMDK Keylight library
	*/
	~CKeylight();

	/**
	*  Allocate memory required for the Keylight and
	*  load functions from the EMDK KeyLightAPI DLL
	*  \return Whether or not the Keylight was successfully initialised.
	*/
	BOOL Initialise(bool bAppHasFocus);

	/**
	*  Illuminate the Keylight
	*  \return Whether or not the keylight was successfully illuminated.
	*/
	BOOL TurnOn();

	/**
	*  Extinguishes the Keylight
	*  \return Whether or not the keylight was successfully extinguished.
	*/
	BOOL TurnOff();

	/**
	*  Set the intensity of the keylight
	*  This will only have a visible effect if the keylight is
	*  turned on.
	*  \param dwIntensity The intensity to set the Keylight to.  If the passed
	*  value is higher than the device is capable of then the device's maximum
	*  value will be used.
	*/
	BOOL SetIntensity(DWORD dwIntensity);

	/**
	*  m_bKeylightOn and m_dwCurrentIntensity hold the current state of the 
	*  keylight as known by this class instance.  Ensure the hardware is in 
	*  sync with these values.  This method is called when the PB Application 
	*  which owns this module gains focus and is called to ensure any the 
	*  state of the keylight held by this module is the same as the hardware.
	*/
	BOOL RefreshHardware();

	/**
	*  Indicates that this application has gained or lost focus.  If focus
	*  has been lost we must reset the keylight hardware settings back to
	*  defaults and store any future state or intensity changes without
	*  accessing the hardware.  If we have regained focus we must synchronise
	*  the Keylight hardware to our stored settings, and allow state and
	*  intensity changes once again to set the hardware.
	*/
	BOOL ApplicationFocusChange(bool bActivated);

private:
	//  Private Functions
	
	/**
	*  Load the Keylight DLL into memory
	*/
	BOOL LoadKeylightDLL();
	
	/**
	*  Remove the Keylight DLL from memory.
	*/
	void UnloadKeylightDLL();

	/**
	*  Set all exported function pointers to NULL
	*/
	void NullifyExportedFunctionPointers();
	
	//  Private member variables
	DWORD		m_dwKeylightDLL;		///<  Result of loading the EMDK Keylight DLL
	DWORD		m_dwIntensityLevels;	///<  Number of intensity levels supported by the device.
	PBModule*	m_pModule;				///<  Pointer to PBModule associated with this Alarm object, used to send navigates and logs to the core.
	int			m_iInstanceID;			///<  Unique Identifier for the Browser Application which uses this object
	DWORD		m_dwKeylightOn;			///<  Whether or not the Keylight is currently on
	DWORD		m_dwCurrentIntensity;	///<  The current intensity the Keylight has been set to
	DWORD		m_dwDefaultState;		///<  The default state for the Keylight
	DWORD		m_dwDefaultIntensity;	///<  The default intensity for the Keylight
	bool		m_bAppHasFocus;			///<  Whether we are currently the foreground app
	
	//  function pointers to exported DLL functions
	typedef DWORD (WINAPI* LPFN_KEYLIGHT_GET_INTENSITY_LEVELS)	(LPDWORD);
	typedef DWORD (WINAPI* LPFN_KEYLIGHT_GET_INTENSITY)			(LPDWORD);
	typedef DWORD (WINAPI* LPFN_KEYLIGHT_SET_INTENSITY)			(DWORD);
	typedef DWORD (WINAPI* LPFN_KEYLIGHT_GET_STATE)				(LPDWORD);
	typedef DWORD (WINAPI* LPFN_KEYLIGHT_SET_STATE)				(DWORD);
	LPFN_KEYLIGHT_GET_INTENSITY_LEVELS	lpfn_KeylightGetIntensityLevels;
	LPFN_KEYLIGHT_GET_INTENSITY			lpfn_KeylightGetIntensity;
	LPFN_KEYLIGHT_SET_INTENSITY			lpfn_KeylightSetIntensity;
	LPFN_KEYLIGHT_GET_STATE				lpfn_KeylightGetState;
	LPFN_KEYLIGHT_SET_STATE				lpfn_KeylightSetState;

};