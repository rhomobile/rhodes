/**
*  \file Notification.h
*  \brief File to house CNotification class and related includes / defines.
*/

#include "../../common/Public/PBPlugin.h"
#include "../../common/Private/NotificationLoader.h"

static TCHAR* tcEnumNotificationsEventNames[] = {L"notificationsArray=notificationIndex,notificationType,notificationName", NULL};

/**
*  Structure to hold the attributes associated with an LED notification.
*/
struct LEDCycleAttributes
{
	DWORD dwOnDuration;	///<  Duration LED is on per cycle in milliseconds
	DWORD dwOffDuration;///<  Duration LED is off per cycle in milliseconds
	DWORD dwCount;		///<  Number of cycles (on+off) to generate
};

/**
*  Structure to hold the attributes associated with a beeper notification.
*/
struct BeeperAttributes
{
	DWORD dwFrequency;	///< Frequency of beep in Hertz
	DWORD dwVolume;		///< Volume of beeper
	DWORD dwDuration;	///< Duration of beep in milliseconds if set to cyclic
};

/**
*  Structure to hold the attributes associated with a pager notification.
*/
struct PagerAttributes
{
	DWORD dwDuration;	///< Duration of vibrate in milliseconds if set to cyclic	
};


/**
*  Class to encapsulate the logic of querying and invoking the deivce's 
*  notification object.  There are 3 types of notification, LED, Beeper and 
*  Pager.  All notifications are in one of three states: On, Off and Cycling.  
*  When cycling an LED will blink for the specified number of times at 
*  whichever frequency has been set at the interface.  The beeper and Pager 
*  'cycle' will only cycle once and therefore these two notification types have 
*  an associated duration.  Beepers also have two additional attributes, 
*  frequency and volume which act when the beeper is both 'on' and 'cycling'.
*/
class CNotification
{
public:
	/**
	*  Constructor for the Notification Object, initialises variables.
	*  \param iInstanceID Unique Browser application Identifier
	*  \param pModule Pointer to associated PBModule which represents the PB
	*  instance to which this object is related.
	*/
	CNotification(int iInstanceID, PBModule* pModule, bool bHasFocus);

	~CNotification();

	/**
	*  Query the EMDK to iterate through the Notifications available on the 
	*  current device.  Provided the user has registered a Navigation URI 
	*  these notifications are returned to the user in the form 
	*  of a 2D JavaScript array with the following format:
	*  \code
	*  	new Array(
	*            new Array(notID, notType, notName), 
	*            new Array(notID, notType, notName), 
	*            new Array(notID, notType, notName)
	*            )
	*  \endcode
	*  	notID is the identifier for the notification, numbered 0 upwards.
	*  notType is an integer of type tagNOTIFY_OBJECT_TYPES
	*  notName is a User friendly name for the notification, e.g. Green LED.
	*  In addition to sending a PBNavigate this function will also list the 
	*  available notifications in the log file.
	*  \return Whether or not the notifications were successfully enumerated.
	*/
	BOOL EnumerateNotifications();

	/**
	*  Sets the duration the LED should remain on.  This duration is used
	*  the next time an LED is asked to cycle.
	*  \param dwOnDuration Length of time to remain on in a cycle, in milliseconds.
	*  \return TRUE
	*/
	BOOL SetLEDOnDuration (DWORD dwOnDuration);

	/**
	*  Sets the duration the LED should remain off.  This duration is used
	*  the next time an LED is asked to cycle.
	*  \param dwOffDuration Length of time to remain off in a cycle, in milliseconds.
	*  \return TRUE
	*/
	BOOL SetLEDOffDuration (DWORD dwOffDuration);

	/**
	*  Sets the number of on / off cycles an LED should undergo.  This 
	*  attribute is used the next time an LED is asked to cycle.
	*  \param dwNumberOfCycles The number of times to cycle the LED.
	*  \return TRUE
	*/
	BOOL SetLEDNumberOfCycles (DWORD dwNumberOfCycles);

	/**
	*  Sets the Frequency to use for the beeper, used the next time the beeper 
	*  is turned on or cycled.
	*  \param dwBeeperFrequency Frequency of the Beeper in Hz
	*  \return TRUE
	*/
	BOOL SetBeeperFrequency (DWORD dwBeeperFrequency);

	/**
	*  Sets the Volume to use for the beeper, used the next time the beeper 
	*  is turned on or cycled.
	*  \param dwBeeperVolume Volume of the Beeper.  0 is the quietest.  
	*  3 is the loudest.
	*  \return TRUE
	*/
	BOOL SetBeeperVolume (DWORD dwBeeperVolume);

	/**
	*  Sets the duration the beeper should sound for when it is asked to cycle
	*  a beeper notification object.  It will only sound once.
	*  \param dwBeeperDuration length of time the beeper should sound for.
	*  \return TRUE
	*/
	BOOL SetBeeperDuration (DWORD dwBeeperDuration);

	/**
	*  Sets the length of time the device will vibrate when it is asked to 
	*  cycle a pager notification object.
	*  \param dwVibrateDuration length of time the beeper will vibrate.
	*  \return TRUE
	*/
	BOOL SetVibrateDuration (DWORD dwVibrateDuration);

	/**
	*  Set the specified notification to on.  Depending on the type of 
	*  notification this will either continuously light an LED, sound a beeper
	*  or vibrate the device.
	*  \param dwNotificationIndex The index of the notification object which 
	*  is being turned on.  This index can be obtained via 
	*  EnumerateNotifications().
	*/
	BOOL SetOn (DWORD dwNotificationIndex);

	/**
	*  Set the specified notification to off.  Depending on the type of 
	*  notification this will either extinguish an LED, silence a beeper
	*  or stop vibrating the device.
	*  \param dwNotificationIndex The index of the notification object which 
	*  is being turned off.  This index can be obtained via 
	*  EnumerateNotifications().
	*/
	BOOL SetOff (DWORD dwNotificationIndex);

	/**
	*  Set the specified notification to cycle.  Depending on the type of 
	*  notification this will either turn an LED on / off for the 
	*  configured number of times (via SetLEDNumberOfCycles); sound the beeper
	*  for the configured duration or vibrate the device for the 
	*  configured duration.
	*  \param dwNotificationIndex The index of the notification object which 
	*  is being cycled.  This index can be obtained via 
	*  EnumerateNotifications().
	*/
	BOOL SetCycle (DWORD dwNotificationIndex);

	/**
	*  Specify the Navigation string.
	*  \param szNavURI The Navigation String to be sent when invoking 
	*  SendPBNavigate
	*  \return Whether or not the Navigation URI was successfully set.
	*/
	BOOL SetNavigationURI(LPCWSTR szNavURI);

	/**
	*  Provides backwards compatibility with PB2.x, the string to parse 
	*  is what used to be the content string for PB2.x.  The parameters contained
	*  therein are returned to the user via the DWORD [out] paramters.
	*  \param tcStringToParse Formally the PB2.x content string.
	*  \param dwNotification [out] The Type of Notification
	*  \param dwNotificationIndex [out] The Index specified for the Notification.
	*  \param dwParameter1 [out] The first paramter given by the user.
	*  \param dwParameter2 [out] The second paramter given by the user.
	*  \param dwParameter3 [out] The third parameter given by the user.
	*  \return Whether or not the parse was successful.
	*/
	BOOL ParseParams(LPCTSTR tcStringToParse, DWORD* dwNotificationType, 
		DWORD* dwNotificationIndex, DWORD* dwParameter1, DWORD* dwParameter2, DWORD* dwParameter3);

	/**
	*  Indicates that the app has gained or lost focus.  On losing focus
	*  the notification state must be saved, and further notifications
	*  should not be activated until focus is regained.  On gaining focus
	*  the saved state and settings should be restored to the hardware
	*  \param bActivated  TRUE if app has gained focus
	*/
	BOOL ApplicationFocusChange(bool bActivated);

private:  //  Functions

	/**
	*  This functions sets the attributes for the specified notification at the 
	*  EMDK.  The application sets the attributes via the public SetXXX methods, 
	*  and the requested values are stored locally, this function takes those 
	*  local values and sets them at the EMDK layer.  In the most part the 
	*  attributes are the cycle attributes (LED On Time, LED Cycle Duration, 
	*  Pager Duration) but for the buzzer the frequency and volume attributes 
	*  also apply equally to when it is constantly on).
	*  \param dwNotificationIndex Index of the notification object whose 
	*  attributes to update.  This can be retrieved via EnumerateNotifications.
	*  \return Whether or not the notification attributes were successfully set
	*/
	BOOL SetAttributes (DWORD dwNotificationIndex);

	/**
	*  Separate thread to send the PBNavigate to the core, this is to avoid 
	*  blocking issues on Windows Mobile where the SendPBNavigate was failing
	*  \param pparam Pointer to the CNotification object
	*  \return 0
	*/
	static DWORD SendNavigateURI (LPVOID pparam);

private:  //  Attributes
	/// The URI To Navigate the PB Browser to when Enumerating Notifications
	TCHAR				m_tcNavigationURI[MAX_URL];
	///  The Array of notifications, temporarily stored in the object before being sent to the page in a separate thread.
	TCHAR				m_tcNotificationArray[MAX_URL];
	///  Pointer to PBModule associated with this Alarm object, used to send navigates and logs to the core.
	PBModule*			m_pModule;
	///  Unique Identifier for the Browser Application which uses this object
	int					m_iInstanceID;
	///  Attributes for the LED (Actioned on next SetState for an LED Notification).  This is not the current value on the device, this is the last value set by the user.
	LEDCycleAttributes	m_LEDCycleAttributes;
	///  Attributes for the Beeper (Actioned on next SetState for a beeper Notification).  This is not the current value on the device, this is the last value set by the user.
	BeeperAttributes	m_BeeperAttributes;
	///  Attributes for the Vibrator (Actioned on next SetState for a vibrator Notification).  This is not the current value on the device, this is the last value set by the user.
	PagerAttributes		m_PagerAttributes;
	CNotificationLoader* m_pNotifications;			///<  EMDK Notifications DLL loaded dynamically
	BOOL				m_bNotificationsLoaded;		///<  Whether or not the Notifications DLL has been loaded
	bool				m_bAppHasFocus;				///<  TRUE if the app is in the foreground
	int					m_iNotificationObjCount;	///<  Number of notification objects found at the enumerate
	CYCLE_INFO*			m_pCycleInfoStore;			///<  Pointer to CycleInfo array for storing background changes
	DWORD*				m_pStateInfoStore;			///<  Pointer to StateInfo array for storing background changes 
};

