/**
 *  \file Alarm.h
 *  \brief Alarm Module workhorse Class.  
 */

#include "../../common/Public/PBPlugin.h"
#pragma once

static TCHAR* tcAlarmTriggeredEventNames[] = {NULL};

/**
*  Structure to hold the time at which an Alarm will fire.  Used by the 
*  CAlarm object when setting alarms.
*/
struct AlarmTime
{
	DWORD dwHHInterval;	///< the Hour the Alarm should fire (offset from current time)
	DWORD dwMMInterval;	///< The Minute the Alarm should fire (offset from current time)
	DWORD dwSSInterval;	///< The Second the Alarm should fire (offset from current time)
	BOOL bRepeat;		///< Whether the alarm should be repeated or not.
	SYSTEMTIME stTime;	///< The time at which the alarm should fire
};

/**
*  Class to enable Browser to set an Alarm.  Alarms will fire either 
*  after a specified duration or at a specified time.
*/
class CAlarm
{
public:
	/**
	*  Constructor for CAlarm, initialises member variables.
	*  \param iInstanceID Unique Browser application Identifier
	*  \param pModule Pointer to associated PBModule which represents the PB
	*  instance to which this object is related.
	*/
	CAlarm(int iInstanceID, PBModule* pModule);
	
	/**
	*  Free memory allocated by the CTelemetry object.
	*/ 
	~CAlarm();

	/**
	*  Set whether or not the Alarm should be repeated, note this is only 
	*  applicable for interval based alarms.  Alarms set to occur at a 
	*  certain time are not repeatable.
	*  \param bRepeat Whether or not the Alarm should be repeated
	*/
	void SetRepeat		(BOOL bRepeat);

	/**
	*  Parse the specified interval value into hours, minutes and seconds.  
	*  Once parsed the value is stored and will be used when the next alarm
	*  is set.
	*  \param tcIntervalValue The Interval to be parsed, this should be in 
	*  the form HH-MM-SS
	*  \return Whether or not the passed interval was valid.
	*/
	BOOL ParseInterval	(LPCTSTR tcIntervalValue);

	/**
	*  Parse the specified time into hours, minutes, seconds, day, month, 
	*  year and timezone.  Once parsed the value is stored and will be used when 
	*  the next alarm is set.
	*  \param tcTimeValue The Time to be parsed, this should be in the form 
	*  yyyy-mm-ddthh:mm:sstzd (e.g. 2005-03-22t11:56:00+01:00).
	*  \return Whether or not the passed time was valid.
	*/
	BOOL ParseTime		(LPCTSTR tcTimeValue);

	/**
	*  Sets the Alarm using the Windows CE API function CeSetUserNotificationEx
	*  to occur at the time specified by ParseInterval or ParseTime.  Create
	*  a AlarmNotificationListener thread to listen for the alarm firing.
	*  \return Whether or not the alarm was successfully set.
	*/
	BOOL Set();

	/**
	*  There can only be one alarm per instance of the Alarm Module, this 
	*  function will clear it if it is set.  Uses the Windows CE API call 
	*  CeClearUserNotification.
	*  \return Whether or not the alarm was successfully cleared.
	*/
	BOOL ClearAlarmNotifications();

	/**
	*  Specify the Navigation string.
	*  \param tcNavURI The Navigation String to be sent when invoking 
	*  SendPBNavigate
	*  \return Whether or not the Navigation URI was successfully set.
	*/
	BOOL SetNavigationURI(LPCWSTR tcNavURI);

private:
	//  METHODS

	/**
	*  Shut down the AlarmNotificationListener thread and free all associated
	*  memory.
	*  \return  TRUE 
	*/
	BOOL StopAlarmListenerThread();

	/**
	*  Thread to listen for the alarm set by the specified module.  When 
	*  an alarm triggered event is fired a PBNavigate will be sent back 
	*  to the core.  If an alarm cancelled event is detected the thread exits
	*  without further processing.
	*  If the alarm is set to be a repeating alarm the alarm is re-set within
	*  this thread.
	*  \param lpParam Pointer to instance of CAlarm which raised the alarm.
	*  \return 0 When the thread exits.
	*/
	static DWORD AlarmNotificationListener (LPVOID lpParam);

	//  ATTRIBUTES

	///  The URI To Navigate the PB Browser to
	TCHAR m_tcNavigationURI[MAXURL];
	///  The Named Event representing this Module Instance, each module has a unique named event.  This will be set to PB_ALARM_EVENT_<Instance ID>
	TCHAR m_tcNamedEvent[25];
	///  Handle for event which is fired when the Alarm fires.
	HANDLE m_hAlarmTriggeredEvent;
	///  Handle for event to indicate the currently set alarm is to be cancelled.
	HANDLE m_hAlarmCancelledEvent;
	///  Handle for thread which will listen for the alarm to fire.
	HANDLE m_hAlarmListenerThread;
	///  Boolean to specify whether or not the thread listening for an alarm is running or not.  This is set before the listener thread is created and unset within the thread its self.  This allows us to call alarm->set() within the thread (for repeating alarms) and also prevent multiple alarms being set simultaneously.
	BOOL m_bListenerThreadRunning;
	///  Pointer to PBModule associated with this Alarm object, used to send navigates and logs to the core.
	PBModule*	m_pModule;
	///  Unique Identifier for the Browser Application which uses this object
	int			m_iInstanceID;
	///  Time at which the alarm associated with this module instance should fire.
	AlarmTime	m_AlarmInfo;
	///  Currently running Alarm Notification Handle
	HANDLE		m_hNotify;
	
};