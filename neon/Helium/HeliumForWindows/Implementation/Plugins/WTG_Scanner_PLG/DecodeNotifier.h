/**
 * \file DecodeNotifier.h
 * \brief File containing the CDecoderNotifier definitions.
 *  \author	Darryn Campbell (DCC)
 *  \date	January 2012 (DCC, Initial Port from RhoElements 1 Code)
 */

#include <windows.h>
#include "../../Common/Private/NotificationLoader.h"

/**
*  The purpose of this class is to abstract the control of the Device
*  beeper and wave player so the Scanner instance can just indicate a
*  good or bad decode.
*/
class CDecodeNotifier
{
public:
	/**
	*  Constructor, initialises variables.
	*/
	CDecodeNotifier();

	/**
	*  Frees memory associated with Wave file paths / names
	*/
	~CDecodeNotifier();

	/**
	*  Allocates memory for Wave file paths / names and initialises
	*  variables to provided values.
	*  \param dwDecodeVolume The volume of both good and bad decodes
	*  \param dwDecodeFrequency The frequency of a good decode beep
	*  \param dwInvalidDecodeFrequency The frequency of a bad decode beep
	*  \param dwDecodeDuration The time (ms) that the beeper will sound for
	*  \param szScanDecodeWav The Wave file to play on a good decode, this will
	*         override the beeper settings.
	*  \param szScanInvalidWav The wave file to play on a bad decode, this will
	*         override the beeper settings.
	*/
	void Initialise(DWORD dwDecodeVolume,
				DWORD dwDecodeFrequency,
				DWORD dwInvalidDecodeFrequency,
				DWORD dwDecodeDuration,
				WCHAR* szScanDecodeWav,
				WCHAR* szScanInvalidWav);
	

	/**
	*  Alert the user of a good decode using either the device's beeper or
	*  wave file, depending on configuration.
	*/
	BOOL GoodDecode();
	
	/**
	*  Alert the user of a bad decode using eith the device's beeper or
	*  wave file, depending on configuration.
	*/
	BOOL BadDecode();

	/**
	*  Accessor for m_dwDecodeVolume
	*  \param dwVolume The new decode volume
	*/
	void SetDecodeVolume(DWORD dwVolume) {m_dwDecodeVolume = dwVolume;}

	/**
	*  Accessor for m_dwDecodeVolume
	*/
	DWORD GetDecodeVolume() {return m_dwDecodeVolume;}

	/**
	*  Accessor for m_dwDecodeDuration
	*  \param dwDuration The new decode duration
	*/
	void SetDecodeDuration(DWORD dwDuration) {m_dwDecodeDuration = dwDuration;}

	/**
	*  Accessor for m_dwDecodeDuration
	*/
	DWORD GetDecodeDuration() {return m_dwDecodeDuration;}

	/**
	*  Accessor for m_dwDecodeFrequency
	*  \param dwFrequency The new decode frequency
	*/
	void SetSuccessfulDecodeFrequency(DWORD dwFrequency) {m_dwDecodeFrequency = dwFrequency;}

	/**
	*  Accessor for m_dwDecodeFrequency
	*/
	DWORD GetSuccessfulDecodeFrequency() {return m_dwDecodeFrequency;}

	/**
	*  Accessor for m_dwInvalidDecodeFrequency
	*  \param dwFrequency The new invalid decode frequency
	*/
	void SetInvalidDecodeFrequency(DWORD dwFrequency) {m_dwInvalidDecodeFrequency = dwFrequency;}

	/**
	*  Accessor for m_dwInvalidDecodeFrequency
	*/
	DWORD GetInvalidDecodeFrequency() {return m_dwInvalidDecodeFrequency;}

	/**
	*  Accessor for m_szScanDecodeWav, allocates and deallocates memory as appropriate.
	*  \param szDecodeSound The new sound to use for a good decode, Windows only supports local file
	*         paths to Wave files.
	*/
	void SetSuccessfulDecodeSound(LPCTSTR szDecodeSound);

	/**
	*  Accessor for m_szScanDecodeWav.  The returned string will be invalid once SetGoodDecodeSound
	*  is called again.
	*/
	LPCTSTR GetSuccessfulDecodeSound() {return m_szScanDecodeWav;}

	/**
	*  Accessor for m_szScanInvalidWav, allocates and deallocates memory as appropriate.
	*  \param szDecodeSound The new sound to use for a bad decode, Windows only supports local file
	*         paths to Wave files.
	*/
	void SetInvalidDecodeSound(LPCTSTR szInvalidDecodeSound);

	/**
	*  Accessor for m_szScanInvalidWav.  The returned string will be invalid once 
	*  SetInvalidDecodeSound is called again.
	*/
	LPCTSTR GetInvalidDecodeSound() {return m_szScanInvalidWav;}

	/**
	*  Thread to trigger a device beep at the specified volume / frequency
	*  using the EMDK notification API
	*  \param lParam Pointer to the CDecodeNotifier object requesting the beep
	*/
	static DWORD DecodeBeepThreadNotify(LPVOID lParam);
private:
	DWORD m_dwDecodeVolume;				///<  Decode volume for this beeper
	DWORD m_dwDecodeDuration;			///<  Decode duration for the beeper (good and bad)
	DWORD m_dwDecodeFrequency;			///<  Good Decode frequency for this beeper
	DWORD m_dwInvalidDecodeFrequency;	///<  Bad Decode frequency for this beeper
	WCHAR* m_szScanDecodeWav;			///<  Wave file to play on good decode
	WCHAR* m_szScanInvalidWav;			///<  Wave file to play on bad decode
	int m_iBeeperIndex;					///<  EMDK Notification API index of the device beeper
	bool m_bGood;						///<  Whether or not the next beeper thread is for a good scan or not.
	CNotificationLoader* m_pNotifications;	///<  EMDK Notifications DLL loaded dynamically
	BOOL m_bNotificationsLoaded;		///<  Whether or not the Notifications DLL has been loaded
};

