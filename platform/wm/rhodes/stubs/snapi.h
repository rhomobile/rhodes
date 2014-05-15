//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//*************************************************************
//
// snapi.h
//
// Registry values for use with the State & Notification Broker APIs in regext.h
//
//*************************************************************
 
 
////////////////////////////////////////////////////////////////////////////////
// ActiveApplication
// Gets the name of the application that was previously active and the name of the application that is currently active, separated by the 'Escape' character ('\e').
#define SN_ACTIVEAPPLICATION_ROOT HKEY_CURRENT_USER
#define SN_ACTIVEAPPLICATION_PATH TEXT("System\\State\\Shell")
#define SN_ACTIVEAPPLICATION_VALUE TEXT("Active Application")
 
  
////////////////////////////////////////////////////////////////////////////////
// DisplayRotation
// Gets the orientation of the display, specified as the number of degrees, ranging from 0 to 360. Portrait = 0. Landscape = 90 or -90.
#define SN_DISPLAYROTATION_ROOT HKEY_LOCAL_MACHINE
#define SN_DISPLAYROTATION_PATH TEXT("System\\GDI\\Rotation")
#define SN_DISPLAYROTATION_VALUE TEXT("Angle")
 
  
////////////////////////////////////////////////////////////////////////////////
// KeyboardPresent
// Gets a value indicating whether a keyboard is attached and enabled.
#define SN_KEYBOARDPRESENT_ROOT HKEY_CURRENT_USER
#define SN_KEYBOARDPRESENT_PATH TEXT("Software\\Microsoft\\Shell")
#define SN_KEYBOARDPRESENT_VALUE TEXT("HasKeyboard")
 
  
////////////////////////////////////////////////////////////////////////////////
// CradlePresent
// Gets a value indicating whether the device is connected to a cradle.
#define SN_CRADLEPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_CRADLEPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_CRADLEPRESENT_VALUE TEXT("Cradled")
 
  
////////////////////////////////////////////////////////////////////////////////
// CameraPresent
// Gets a value indicating whether a camera is attached.
#define SN_CAMERAPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_CAMERAPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_CAMERAPRESENT_VALUE TEXT("Camera")

////////////////////////////////////////////////////////////////////////////////
// CameraEnabled
// Gets a value indicating whether a camera is enabled.
#define SN_CAMERAENABLED_ROOT HKEY_LOCAL_MACHINE
#define SN_CAMERAENABLED_PATH TEXT("System\\State\\Hardware")
#define SN_CAMERAENABLED_VALUE TEXT("CameraEnabled")
  
////////////////////////////////////////////////////////////////////////////////
// MessagingSmsUnread
// Gets the number of unread SMS messages.
#define SN_MESSAGINGSMSUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGSMSUNREAD_PATH TEXT("System\\State\\Messages\\SMS\\Unread")
#define SN_MESSAGINGSMSUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingMmsUnread
// Gets the number of unread MMS messages
#define SN_MESSAGINGMMSUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGMMSUNREAD_PATH TEXT("System\\State\\Messages\\MMS\\Unread")
#define SN_MESSAGINGMMSUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingVoiceMailTotalUnread
// Gets the number of unplayed voice mail messages.
#define SN_MESSAGINGVOICEMAILTOTALUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGVOICEMAILTOTALUNREAD_PATH TEXT("System\\State\\Messages\\VMail\\Total\\Unread")
#define SN_MESSAGINGVOICEMAILTOTALUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingVoiceMail1Unread
// Gets the number of unplayed voice mail messages for line 1.
#define SN_MESSAGINGVOICEMAIL1UNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGVOICEMAIL1UNREAD_PATH TEXT("System\\State\\Messages\\VMail\\Line1\\Unread")
#define SN_MESSAGINGVOICEMAIL1UNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingVoiceMail2Unread
// Gets the number of unplayed voice mail messages for line 2
#define SN_MESSAGINGVOICEMAIL2UNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGVOICEMAIL2UNREAD_PATH TEXT("System\\State\\Messages\\VMail\\Line2\\Unread")
#define SN_MESSAGINGVOICEMAIL2UNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingActiveSyncEmailUnread
// Gets the number of unread ActiveSync e-mail messages.
#define SN_MESSAGINGACTIVESYNCEMAILUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGACTIVESYNCEMAILUNREAD_PATH TEXT("System\\State\\Messages\\Sync\\Unread")
#define SN_MESSAGINGACTIVESYNCEMAILUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingTotalEmailUnread
// Gets the total number of unread e-mail messages from all POP3/IMAP accounts.
#define SN_MESSAGINGTOTALEMAILUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGTOTALEMAILUNREAD_PATH TEXT("System\\State\\Messages\\TotalEmail\\Unread")
#define SN_MESSAGINGTOTALEMAILUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingOtherEmailUnread
// Gets the number of unread e-mail messages from all non-ActiveSync messaging accounts.
#define SN_MESSAGINGOTHEREMAILUNREAD_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGOTHEREMAILUNREAD_PATH TEXT("System\\State\\Messages\\OtherEmail\\Unread")
#define SN_MESSAGINGOTHEREMAILUNREAD_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingLastEmailAccountName
// Gets the name of the e-mail account that was last active.
#define SN_MESSAGINGLASTEMAILACCOUNTNAME_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGLASTEMAILACCOUNTNAME_PATH TEXT("System\\State\\Messages")
#define SN_MESSAGINGLASTEMAILACCOUNTNAME_VALUE TEXT("Last Received Account")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingSmsAccountName
// Gets the name of the SMS messaging account.
#define SN_MESSAGINGSMSACCOUNTNAME_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGSMSACCOUNTNAME_PATH TEXT("System\\State\\Messages\\SMS")
#define SN_MESSAGINGSMSACCOUNTNAME_VALUE TEXT("Account Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingMmsAccountName
// Gets the name of the MMS messaging account.
#define SN_MESSAGINGMMSACCOUNTNAME_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGMMSACCOUNTNAME_PATH TEXT("System\\State\\Messages\\MMS")
#define SN_MESSAGINGMMSACCOUNTNAME_VALUE TEXT("Account Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// MessagingActiveSyncAccountName
// Get the name of the Outlook e-mail messaging account.
#define SN_MESSAGINGACTIVESYNCACCOUNTNAME_ROOT HKEY_CURRENT_USER
#define SN_MESSAGINGACTIVESYNCACCOUNTNAME_PATH TEXT("System\\State\\Messages\\Sync")
#define SN_MESSAGINGACTIVESYNCACCOUNTNAME_VALUE TEXT("Account Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// TasksActive
// Gets the number of active tasks. 
#define SN_TASKSACTIVE_ROOT HKEY_CURRENT_USER
#define SN_TASKSACTIVE_PATH TEXT("System\\State\\Tasks")
#define SN_TASKSACTIVE_VALUE TEXT("Active")
 
  
////////////////////////////////////////////////////////////////////////////////
// TasksHighPriority
// Gets the number of high priority tasks that are currently active.
#define SN_TASKSHIGHPRIORITY_ROOT HKEY_CURRENT_USER
#define SN_TASKSHIGHPRIORITY_PATH TEXT("System\\State\\Tasks")
#define SN_TASKSHIGHPRIORITY_VALUE TEXT("High Priority")
 
  
////////////////////////////////////////////////////////////////////////////////
// TasksDueToday
// Gets the number of active tasks that are due today. 
#define SN_TASKSDUETODAY_ROOT HKEY_CURRENT_USER
#define SN_TASKSDUETODAY_PATH TEXT("System\\State\\Tasks")
#define SN_TASKSDUETODAY_VALUE TEXT("Due Today")
 
  
////////////////////////////////////////////////////////////////////////////////
// TasksOverdue
// Gets the number of overdue tasks. 
#define SN_TASKSOVERDUE_ROOT HKEY_CURRENT_USER
#define SN_TASKSOVERDUE_PATH TEXT("System\\State\\Tasks")
#define SN_TASKSOVERDUE_VALUE TEXT("Overdue")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentSubject
// Gets the subject of the next Appointment.
#define SN_CALENDARNEXTAPPOINTMENTSUBJECT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTSUBJECT_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTSUBJECT_VALUE TEXT("Subject")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentLocation
// Gets the location of the next Appointment.
#define SN_CALENDARNEXTAPPOINTMENTLOCATION_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTLOCATION_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTLOCATION_VALUE TEXT("Location")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentStart
// Gets the starting time of the next Appointment.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARNEXTAPPOINTMENTSTART_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTSTART_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTSTART_VALUE TEXT("Start Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentEnd
// Gets the ending time of the next Appointment.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARNEXTAPPOINTMENTEND_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTEND_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTEND_VALUE TEXT("End Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerAlbumArtist
// Gets the album artist's name of the currently playing track in Windows Media Player.
#define SN_MEDIAPLAYERALBUMARTIST_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERALBUMARTIST_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERALBUMARTIST_VALUE TEXT("WM/AlbumArtist")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackBitrate
// Gets the title of the track currently playing in Windows Media Player. For example, "128Kbps"
#define SN_MEDIAPLAYERTRACKBITRATE_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKBITRATE_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKBITRATE_VALUE TEXT("Bitrate")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackTitle
// Gets the title of the track currently playing in Windows Media Player.
#define SN_MEDIAPLAYERTRACKTITLE_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKTITLE_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKTITLE_VALUE TEXT("Title")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerAlbumTitle
// Gets the album title of the currently playing track in Windows Media Player.
#define SN_MEDIAPLAYERALBUMTITLE_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERALBUMTITLE_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERALBUMTITLE_VALUE TEXT("Wm/AlbumTitle")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackArtist
// Gets the artist's name of the currently playing track in Windows Media Player.
#define SN_MEDIAPLAYERTRACKARTIST_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKARTIST_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKARTIST_VALUE TEXT("WM/OriginalArtist")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackNumber
// Gets the number of the current track playing in Windows Media Player.
#define SN_MEDIAPLAYERTRACKNUMBER_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKNUMBER_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKNUMBER_VALUE TEXT("WM/TrackNumber")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackGenre
// Gets the genre of the currently playing track in Windows Media Player.
#define SN_MEDIAPLAYERTRACKGENRE_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKGENRE_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKGENRE_VALUE TEXT("WM/Genre")
 
  
////////////////////////////////////////////////////////////////////////////////
// MediaPlayerTrackTimeElapsed
// Gets the elapsed time (in milliseconds) of the currently playing track in Windows Media Player.
#define SN_MEDIAPLAYERTRACKTIMEELAPSED_ROOT HKEY_CURRENT_USER
#define SN_MEDIAPLAYERTRACKTIMEELAPSED_PATH TEXT("System\\State\\MediaPlayer")
#define SN_MEDIAPLAYERTRACKTIMEELAPSED_VALUE TEXT("Elapsed")
 
  
////////////////////////////////////////////////////////////////////////////////
// PowerBatteryStrength
// Gets the remaining battery power level, expressed as a percentage of fully charged.
#define SN_POWERBATTERYSTRENGTH_ROOT HKEY_LOCAL_MACHINE
#define SN_POWERBATTERYSTRENGTH_PATH TEXT("System\\State\\Battery")
#define SN_POWERBATTERYSTRENGTH_VALUE TEXT("Main")
#define SN_POWERBATTERYSTRENGTH_BITMASK 0xFFFF0000

  
////////////////////////////////////////////////////////////////////////////////
// PowerBatteryState
// Gets the current battery state (for example, it is low, and charging). 
// This value can be a bitwise combination of SN_POWERBATTERYSTATE_FLAG_* values.
#define SN_POWERBATTERYSTATE_ROOT HKEY_LOCAL_MACHINE
#define SN_POWERBATTERYSTATE_PATH TEXT("System\\State\\Battery")
#define SN_POWERBATTERYSTATE_VALUE TEXT("Main")
#define SN_POWERBATTERYSTATE_BITMASK 0x0000FFFF

#define SN_POWERBATTERYSTATE_FLAG_NOTPRESENT 1
#define SN_POWERBATTERYSTATE_FLAG_CHARGING   2
#define SN_POWERBATTERYSTATE_FLAG_LOW        4
#define SN_POWERBATTERYSTATE_FLAG_CRITICAL   8

 
  
////////////////////////////////////////////////////////////////////////////////
// PowerBatteryBackupStrength
// Gets the remaining backup battery power level, expressed as a percentage of fully charged.
#define SN_POWERBATTERYBACKUPSTRENGTH_ROOT HKEY_LOCAL_MACHINE
#define SN_POWERBATTERYBACKUPSTRENGTH_PATH TEXT("System\\State\\Battery")
#define SN_POWERBATTERYBACKUPSTRENGTH_VALUE TEXT("Backup")
#define SN_POWERBATTERYBACKUPSTRENGTH_BITMASK 0xFFFF0000
 
  
////////////////////////////////////////////////////////////////////////////////
// PowerBatteryBackupState
// Gets the current backup battery state (for example, it is low, and charging). 
// This value can be a bitwise combination of SN_POWERBATTERYBACKUPSTATE_FLAG_* values.
#define SN_POWERBATTERYBACKUPSTATE_ROOT HKEY_LOCAL_MACHINE
#define SN_POWERBATTERYBACKUPSTATE_PATH TEXT("System\\State\\Battery")
#define SN_POWERBATTERYBACKUPSTATE_VALUE TEXT("Backup")
#define SN_POWERBATTERYBACKUPSTATE_BITMASK 0x0000FFFF

#define SN_POWERBATTERYBACKUPSTATE_FLAG_NOTPRESENT 1
#define SN_POWERBATTERYBACKUPSTATE_FLAG_CHARGING   2
#define SN_POWERBATTERYBACKUPSTATE_FLAG_LOW        4
#define SN_POWERBATTERYBACKUPSTATE_FLAG_CRITICAL   8

 
  
////////////////////////////////////////////////////////////////////////////////
// Time
// Gets the current time. This Date/Time value updates once per minute.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_TIME_ROOT HKEY_LOCAL_MACHINE
#define SN_TIME_PATH TEXT("System\\State\\DateTime")
#define SN_TIME_VALUE TEXT("Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// Date
// Gets the current date. This Date/Time value changes once per day. Monitor this property to perform actions on a daily basis. Note: Do not use Date to get an accurate clock time; instead, use Time.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_DATE_ROOT HKEY_LOCAL_MACHINE
#define SN_DATE_PATH TEXT("System\\State\\DateTime")
#define SN_DATE_VALUE TEXT("Date")

////////////////////////////////////////////////////////////////////////////////
// LocalDate
// Gets the current date in local time. This Date/Time value changes once per day at local time 12:00AM. Monitor this property to perform actions on a daily basis. Note: Do not use LocalDate to get an accurate clock time; instead, use Time.
// The value is a local FILETIME stored in REG_BINARY format.
#define SN_LOCALDATE_ROOT HKEY_LOCAL_MACHINE
#define SN_LOCALDATE_PATH TEXT("System\\State\\DateTime")
#define SN_LOCALDATE_VALUE TEXT("LocalDate")

////////////////////////////////////////////////////////////////////////////////
// PhoneSignalStrength
// Get the phone signal strength, expressed as a percentage of full strength.
#define SN_PHONESIGNALSTRENGTH_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONESIGNALSTRENGTH_PATH TEXT("System\\State\\Phone")
#define SN_PHONESIGNALSTRENGTH_VALUE TEXT("Signal Strength Raw")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneOperatorName
// Gets the name of the mobile operator (i.e., the mobile phone company, or carrier).
#define SN_PHONEOPERATORNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEOPERATORNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONEOPERATORNAME_VALUE TEXT("Current Operator Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallerName
// Gets the name of the person you are talking to on the phone. This value is not set when you are talking on a conference call.
#define SN_PHONETALKINGCALLERNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLERNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLERNAME_VALUE TEXT("Caller Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallStartTime
// Gets the time of the current active call in FILETIME format
#define SN_PHONETALKINGCALLSTARTTIME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLSTARTTIME_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLSTARTTIME_VALUE TEXT("Call Start Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneActiveCallCount
// Gets the number of active phone calls.
#define SN_PHONEACTIVECALLCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEACTIVECALLCOUNT_PATH TEXT("System\\State\\Phone")
#define SN_PHONEACTIVECALLCOUNT_VALUE TEXT("Active Call Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneProfileName
// Gets the localized name of the current sound profile. (Smartphone only)
#define SN_PHONEPROFILENAME_ROOT HKEY_CURRENT_USER
#define SN_PHONEPROFILENAME_PATH TEXT("System\\State")
#define SN_PHONEPROFILENAME_VALUE TEXT("Profile")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneProfile
// Gets the non-localized name of the current sound profile. For example, "Normal", "Silent", "Car", "Headset", "Loud", "Meeting", or "Speakerphone". (Smartphone only)
#define SN_PHONEPROFILE_ROOT HKEY_CURRENT_USER
#define SN_PHONEPROFILE_PATH TEXT("ControlPanel\\Profiles")
#define SN_PHONEPROFILE_VALUE TEXT("ActiveProfile")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCellBroadcast
// Gets the cell broadcast message.
#define SN_PHONECELLBROADCAST_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECELLBROADCAST_PATH TEXT("System\\State\\Phone")
#define SN_PHONECELLBROADCAST_VALUE TEXT("Cell Broadcast")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentHasConflict
// Gets a value indicating whether there is a conflict with the next Appointment.
#define SN_CALENDARNEXTAPPOINTMENTHASCONFLICT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTHASCONFLICT_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTHASCONFLICT_VALUE TEXT("Conflict")
 
  
////////////////////////////////////////////////////////////////////////////////
// OwnerName
// Gets the mobile device owner's name.
#define SN_OWNERNAME_ROOT HKEY_CURRENT_USER
#define SN_OWNERNAME_PATH TEXT("ControlPanel\\Owner")
#define SN_OWNERNAME_VALUE TEXT("Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// OwnerPhoneNumber
// Gets the mobile device owner's phone number.
#define SN_OWNERPHONENUMBER_ROOT HKEY_CURRENT_USER
#define SN_OWNERPHONENUMBER_PATH TEXT("ControlPanel\\Owner")
#define SN_OWNERPHONENUMBER_VALUE TEXT("Telephone")
 
  
////////////////////////////////////////////////////////////////////////////////
// OwnerEmail
// Gets the mobile device owner's e-mail address.
#define SN_OWNEREMAIL_ROOT HKEY_CURRENT_USER
#define SN_OWNEREMAIL_PATH TEXT("ControlPanel\\Owner")
#define SN_OWNEREMAIL_VALUE TEXT("E-mail")
 
  
////////////////////////////////////////////////////////////////////////////////
// OwnerNotes
// Gets the text notes entered by the owner of the mobile device.
#define SN_OWNERNOTES_ROOT HKEY_CURRENT_USER
#define SN_OWNERNOTES_PATH TEXT("ControlPanel\\Owner")
#define SN_OWNERNOTES_VALUE TEXT("Notes")
 
  
////////////////////////////////////////////////////////////////////////////////
// ActiveSyncStatus
// Gets a value indicating whether ActiveSync is synchronizing.
#define SN_ACTIVESYNCSTATUS_ROOT HKEY_LOCAL_MACHINE
#define SN_ACTIVESYNCSTATUS_PATH TEXT("System\\State\\ActiveSync")
#define SN_ACTIVESYNCSTATUS_VALUE TEXT("Synchronizing")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneMissedCalls
// Gets the number of missed phone calls.
#define SN_PHONEMISSEDCALLS_ROOT HKEY_CURRENT_USER
#define SN_PHONEMISSEDCALLS_PATH TEXT("System\\State\\Phone")
#define SN_PHONEMISSEDCALLS_VALUE TEXT("Missed Call Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// HeadsetPresent
// Gets a value indicating whether a headset is present
#define SN_HEADSETPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_HEADSETPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_HEADSETPRESENT_VALUE TEXT("Headset")
 
  
////////////////////////////////////////////////////////////////////////////////
// CarKitPresent
// Gets a value indicating whether a car kit is present
#define SN_CARKITPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_CARKITPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_CARKITPRESENT_VALUE TEXT("Car Kit")
 
  
////////////////////////////////////////////////////////////////////////////////
// SpeakerPhoneActive
// Gets a value indicating whether the speakerphone is active
#define SN_SPEAKERPHONEACTIVE_ROOT HKEY_LOCAL_MACHINE
#define SN_SPEAKERPHONEACTIVE_PATH TEXT("System\\State\\Hardware")
#define SN_SPEAKERPHONEACTIVE_VALUE TEXT("Speaker")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneMultiLine
// Gets a value indicating whether the phone supports multiple lines.
#define SN_PHONEMULTILINE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEMULTILINE_PATH TEXT("System\\State\\Phone")
#define SN_PHONEMULTILINE_VALUE TEXT("Multiline Capabilities")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneSimFull
// Gets a value indicating whether the Subscriber Identity Module (SIM) memory is full.
#define SN_PHONESIMFULL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONESIMFULL_PATH TEXT("System\\State\\Phone")
#define SN_PHONESIMFULL_VALUE TEXT("Status")
#define SN_PHONESIMFULL_BITMASK 1
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneNoSim
// Gets a value indicating whether the Subscriber Identity Module (SIM) is installed in the mobile device.
#define SN_PHONENOSIM_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONENOSIM_PATH TEXT("System\\State\\Phone")
#define SN_PHONENOSIM_VALUE TEXT("Status")
#define SN_PHONENOSIM_BITMASK 2
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneInvalidSim
// Gets a value indicating whether the Subscriber Identity Module (SIM) is invalid.
#define SN_PHONEINVALIDSIM_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINVALIDSIM_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINVALIDSIM_VALUE TEXT("Status")
#define SN_PHONEINVALIDSIM_BITMASK 4
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneBlockedSim
// Gets a value indicating whether the Subscriber Identity Module (SIM) is blocked.
#define SN_PHONEBLOCKEDSIM_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEBLOCKEDSIM_PATH TEXT("System\\State\\Phone")
#define SN_PHONEBLOCKEDSIM_VALUE TEXT("Status")
#define SN_PHONEBLOCKEDSIM_BITMASK 8
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneRadioOff
// Gets a value indicating whether the phone's radio is turned off.
#define SN_PHONERADIOOFF_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONERADIOOFF_PATH TEXT("System\\State\\Phone")
#define SN_PHONERADIOOFF_VALUE TEXT("Status")
#define SN_PHONERADIOOFF_BITMASK 0x10
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneRadioPresent
// Gets a value indicating whether the mobile device has a phone.
#define SN_PHONERADIOPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONERADIOPRESENT_PATH TEXT("System\\State\\Phone")
#define SN_PHONERADIOPRESENT_VALUE TEXT("Status")
#define SN_PHONERADIOPRESENT_BITMASK 0x20
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneRingerOff
// Gets a value indicating whether the phone's ringer is off (i.e., if it rings and/or vibrates).
#define SN_PHONERINGEROFF_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONERINGEROFF_PATH TEXT("System\\State\\Phone")
#define SN_PHONERINGEROFF_VALUE TEXT("Status")
#define SN_PHONERINGEROFF_BITMASK 0x40
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLine1Selected
// Gets a value indicating whether line 1 is selected.
#define SN_PHONELINE1SELECTED_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELINE1SELECTED_PATH TEXT("System\\State\\Phone")
#define SN_PHONELINE1SELECTED_VALUE TEXT("Status")
#define SN_PHONELINE1SELECTED_BITMASK 0x80
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLine2Selected
// Gets a value indicating whether line 2 is selected.
#define SN_PHONELINE2SELECTED_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELINE2SELECTED_PATH TEXT("System\\State\\Phone")
#define SN_PHONELINE2SELECTED_VALUE TEXT("Status")
#define SN_PHONELINE2SELECTED_BITMASK 0x100
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneRoaming
// Gets a value indicating whether the phone is currently in roaming mode.
#define SN_PHONEROAMING_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEROAMING_PATH TEXT("System\\State\\Phone")
#define SN_PHONEROAMING_VALUE TEXT("Status")
#define SN_PHONEROAMING_BITMASK 0x200
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallForwardingOnLine1
// Gets a value indicating whether call forwarding is enabled on line 1.
#define SN_PHONECALLFORWARDINGONLINE1_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLFORWARDINGONLINE1_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLFORWARDINGONLINE1_VALUE TEXT("Status")
#define SN_PHONECALLFORWARDINGONLINE1_BITMASK 0x400
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneMissedCall
// Gets a value indicating whether there was a new missed call.
#define SN_PHONEMISSEDCALL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEMISSEDCALL_PATH TEXT("System\\State\\Phone")
#define SN_PHONEMISSEDCALL_VALUE TEXT("Status")
#define SN_PHONEMISSEDCALL_BITMASK 0x800
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneActiveDataCall
// Gets a value indicating whether the phone has an active cellular data connection.
#define SN_PHONEACTIVEDATACALL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEACTIVEDATACALL_PATH TEXT("System\\State\\Phone")
#define SN_PHONEACTIVEDATACALL_VALUE TEXT("Status")
#define SN_PHONEACTIVEDATACALL_BITMASK 0x1000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallBarring
// Gets a value indicating whether the call barring feature is enabled.
#define SN_PHONECALLBARRING_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLBARRING_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLBARRING_VALUE TEXT("Status")
#define SN_PHONECALLBARRING_BITMASK 0x2000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallOnHold
// Gets a value indicating whether a phone call is currently on hold.
#define SN_PHONECALLONHOLD_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLONHOLD_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLONHOLD_VALUE TEXT("Status")
#define SN_PHONECALLONHOLD_BITMASK 0x4000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneConferenceCall
// Gets a value indicating whether a conference call is currently in progress.
#define SN_PHONECONFERENCECALL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECONFERENCECALL_PATH TEXT("System\\State\\Phone")
#define SN_PHONECONFERENCECALL_VALUE TEXT("Status")
#define SN_PHONECONFERENCECALL_BITMASK 0x8000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCall
// Gets a value indicating whether there is an incoming (ringing) call.
#define SN_PHONEINCOMINGCALL_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALL_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALL_VALUE TEXT("Status")
#define SN_PHONEINCOMINGCALL_BITMASK 0x10000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallCalling
// Gets a value indicating whether the phone is currently attempting to connect an outgoing call.
#define SN_PHONECALLCALLING_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLCALLING_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLCALLING_VALUE TEXT("Status")
#define SN_PHONECALLCALLING_BITMASK 0x20000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneGprsCoverage
// Gets a value indicating whether the phone currently has GPRS coverage.
#define SN_PHONEGPRSCOVERAGE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEGPRSCOVERAGE_PATH TEXT("System\\State\\Phone")
#define SN_PHONEGPRSCOVERAGE_VALUE TEXT("Status")
#define SN_PHONEGPRSCOVERAGE_BITMASK 0x100000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneNoService
// Gets a value indicating whether the phone is not currently connected to a network.
#define SN_PHONENOSERVICE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONENOSERVICE_PATH TEXT("System\\State\\Phone")
#define SN_PHONENOSERVICE_VALUE TEXT("Status")
#define SN_PHONENOSERVICE_BITMASK 0x200000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneSearchingForService
// Gets a value indicating whether the phone is currently searching for service on a network.
#define SN_PHONESEARCHINGFORSERVICE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONESEARCHINGFORSERVICE_PATH TEXT("System\\State\\Phone")
#define SN_PHONESEARCHINGFORSERVICE_VALUE TEXT("Status")
#define SN_PHONESEARCHINGFORSERVICE_BITMASK 0x400000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneHomeService
// Gets a value indicating whether the phone is currently registered on its home network.
#define SN_PHONEHOMESERVICE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEHOMESERVICE_PATH TEXT("System\\State\\Phone")
#define SN_PHONEHOMESERVICE_VALUE TEXT("Status")
#define SN_PHONEHOMESERVICE_BITMASK 0x800000
 
  
////////////////////////////////////////////////////////////////////////////////
// Phone1xRttCoverage
// Gets a value indicating whether the phone currently has 1xRTT coverage.
#define SN_PHONE1XRTTCOVERAGE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONE1XRTTCOVERAGE_PATH TEXT("System\\State\\Phone")
#define SN_PHONE1XRTTCOVERAGE_VALUE TEXT("Status")
#define SN_PHONE1XRTTCOVERAGE_BITMASK 0x10000000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallTalking
// Gets a value indicating whether there is currently a phone call in the talking state.
#define SN_PHONECALLTALKING_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLTALKING_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLTALKING_VALUE TEXT("Status")
#define SN_PHONECALLTALKING_BITMASK 0x20000000
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneCallForwardingOnLine2
// Gets a value indicating whether call forwarding is currently active on line 2.
#define SN_PHONECALLFORWARDINGONLINE2_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONECALLFORWARDINGONLINE2_PATH TEXT("System\\State\\Phone")
#define SN_PHONECALLFORWARDINGONLINE2_VALUE TEXT("Status")
#define SN_PHONECALLFORWARDINGONLINE2_BITMASK 0x40000000

////////////////////////////////////////////////////////////////////////////////
// PhoneMuteState
// Gets a value indicating whether phone is muted
#define SN_PHONEMUTESTATE_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEMUTESTATE_PATH TEXT("System\\State\\Phone")
#define SN_PHONEMUTESTATE_VALUE TEXT("Extended Status")
#define SN_PHONEMUTESTATE_BITMASK 0x00000010
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointment
// Gets the CEOID of the next Appointment.
#define SN_CALENDARNEXTAPPOINTMENT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENT_PATH TEXT("System\\State\\Appointments")
#define SN_CALENDARNEXTAPPOINTMENT_VALUE TEXT("Next")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentBusyStatus
// Gets the user's availability (e.g., Free, Busy, Tenative, Out of Office) status for the next Appointment.
// The value is of type OlBusyStatus (from pimstore.h).
#define SN_CALENDARNEXTAPPOINTMENTBUSYSTATUS_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTBUSYSTATUS_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTBUSYSTATUS_VALUE TEXT("Busy Status")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarNextAppointmentCategories
// Gets the next Appointment's categories.
#define SN_CALENDARNEXTAPPOINTMENTCATEGORIES_ROOT HKEY_CURRENT_USER
#define SN_CALENDARNEXTAPPOINTMENTCATEGORIES_PATH TEXT("System\\State\\Appointments\\Next")
#define SN_CALENDARNEXTAPPOINTMENTCATEGORIES_VALUE TEXT("Categories")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointment
// Gets the CEOID of the current Appointment.
#define SN_CALENDARAPPOINTMENT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENT_PATH TEXT("System\\State\\Appointments")
#define SN_CALENDARAPPOINTMENT_VALUE TEXT("Current")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentSubject
// Gets the current Appointment's subject.
#define SN_CALENDARAPPOINTMENTSUBJECT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTSUBJECT_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTSUBJECT_VALUE TEXT("Subject")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentLocation
// Gets the current Appointment's location.
#define SN_CALENDARAPPOINTMENTLOCATION_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTLOCATION_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTLOCATION_VALUE TEXT("Location")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentStartTime
// Gets the current Appointment's starting time.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARAPPOINTMENTSTARTTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTSTARTTIME_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTSTARTTIME_VALUE TEXT("Start Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentEndTime
// Gets the current Appointment's ending time.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARAPPOINTMENTENDTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTENDTIME_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTENDTIME_VALUE TEXT("End Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentHasConflict
// Gets a value indicating whether the current Appointment has a scheduling conflict with an existing Appointment.
#define SN_CALENDARAPPOINTMENTHASCONFLICT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTHASCONFLICT_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTHASCONFLICT_VALUE TEXT("Conflict")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentBusyStatus
// Gets the user's availability (free/busy status) for the time-slot taken by the current Appointment.
// The value is of type OlBusyStatus (from pimstore.h).
#define SN_CALENDARAPPOINTMENTBUSYSTATUS_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTBUSYSTATUS_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTBUSYSTATUS_VALUE TEXT("Busy Status")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarAppointmentCategories
// Gets the current Appointment's categories.
#define SN_CALENDARAPPOINTMENTCATEGORIES_ROOT HKEY_CURRENT_USER
#define SN_CALENDARAPPOINTMENTCATEGORIES_PATH TEXT("System\\State\\Appointments\\Current")
#define SN_CALENDARAPPOINTMENTCATEGORIES_VALUE TEXT("Categories")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointment
// Gets the CEOID of the Appointment that is currently displayed on the Home Screen.
#define SN_CALENDARHOMESCREENAPPOINTMENT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENT_PATH TEXT("System\\State\\Appointments")
#define SN_CALENDARHOMESCREENAPPOINTMENT_VALUE TEXT("HomeScreen")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentSubject
// Gets the subject of the Appointment that is currently displayed on the Home Screen.
#define SN_CALENDARHOMESCREENAPPOINTMENTSUBJECT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTSUBJECT_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTSUBJECT_VALUE TEXT("Subject")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentLocation
// Gets the location of the Appointment that is currently displayed on the Home Screen.
#define SN_CALENDARHOMESCREENAPPOINTMENTLOCATION_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTLOCATION_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTLOCATION_VALUE TEXT("Location")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentStartTime
// Gets the starting time of the Appointment that is currently displayed on the Home Screen.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARHOMESCREENAPPOINTMENTSTARTTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTSTARTTIME_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTSTARTTIME_VALUE TEXT("Start Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentEndTime
// Gets the ending time of the Appointment that is currently displayed on the Home Screen.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDARHOMESCREENAPPOINTMENTENDTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTENDTIME_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTENDTIME_VALUE TEXT("End Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentHasConflict
// Gets a value indicating whether the Appointment that is currently displayed on the Home Screen has a scheduling conflict.
#define SN_CALENDARHOMESCREENAPPOINTMENTHASCONFLICT_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTHASCONFLICT_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTHASCONFLICT_VALUE TEXT("Conflict")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentBusyStatus
// Gets the user's availability (free/busy status) for the Appointment displayed on the Home Screen.
// The value is of type OlBusyStatus (from pimstore.h).
#define SN_CALENDARHOMESCREENAPPOINTMENTBUSYSTATUS_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTBUSYSTATUS_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTBUSYSTATUS_VALUE TEXT("Busy Status")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarHomeScreenAppointmentCategories
// Gets the categories of the Appointment that is currently displayed on the Home Screen.
#define SN_CALENDARHOMESCREENAPPOINTMENTCATEGORIES_ROOT HKEY_CURRENT_USER
#define SN_CALENDARHOMESCREENAPPOINTMENTCATEGORIES_PATH TEXT("System\\State\\Appointments\\HomeScreen")
#define SN_CALENDARHOMESCREENAPPOINTMENTCATEGORIES_VALUE TEXT("Categories")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCallerName
// Gets the name of the person who is currently placing the incoming call.
#define SN_PHONEINCOMINGCALLERNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALLERNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALLERNAME_VALUE TEXT("Incoming Caller Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLastIncomingCallerName
// Gets the name of the last caller to place an incoming call.
#define SN_PHONELASTINCOMINGCALLERNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELASTINCOMINGCALLERNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONELASTINCOMINGCALLERNAME_VALUE TEXT("Last Incoming Caller Name")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCallerNumber
// Gets the incoming call's phone number (Caller ID).
#define SN_PHONEINCOMINGCALLERNUMBER_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALLERNUMBER_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALLERNUMBER_VALUE TEXT("Incoming Caller Number")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLastIncomingCallerNumber
// Gets the last incoming call's phone number (Caller ID).
#define SN_PHONELASTINCOMINGCALLERNUMBER_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELASTINCOMINGCALLERNUMBER_PATH TEXT("System\\State\\Phone")
#define SN_PHONELASTINCOMINGCALLERNUMBER_VALUE TEXT("Last Incoming Caller Number")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCallerContactPropertyName
// Gets the name of the property that matches the Caller ID, e.g. "h" for "Home Telephone".
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYNAME_VALUE TEXT("Incoming Caller PropName")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLastIncomingCallerContactPropertyName
// Gets the name of the property that matches the last Caller ID, e.g. "h" for "Home Telephone".
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYNAME_VALUE TEXT("Last Incoming Caller PropName")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCallerContactPropertyID
// Gets the CEPROPID of the property that matches the Caller ID, for example, PIMPR_HOME_TELEPHONE_NUMBER.
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYID_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYID_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALLERCONTACTPROPERTYID_VALUE TEXT("Incoming Caller PropID")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLastIncomingCallerContactPropertyID
// Gets the CEPROPID of the property that matches the last Caller ID, for example, PIMPR_HOME_TELEPHONE_NUMBER.
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYID_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYID_PATH TEXT("System\\State\\Phone")
#define SN_PHONELASTINCOMINGCALLERCONTACTPROPERTYID_VALUE TEXT("Last Incoming Caller PropID")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneIncomingCallerContact
// Gets the CEOID of the Contact that matches the Caller ID.
#define SN_PHONEINCOMINGCALLERCONTACT_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONEINCOMINGCALLERCONTACT_PATH TEXT("System\\State\\Phone")
#define SN_PHONEINCOMINGCALLERCONTACT_VALUE TEXT("Incoming Caller OID")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneLastIncomingCallerContact
// Gets the CEOID of the Contact that matches the last Caller ID.
#define SN_PHONELASTINCOMINGCALLERCONTACT_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONELASTINCOMINGCALLERCONTACT_PATH TEXT("System\\State\\Phone")
#define SN_PHONELASTINCOMINGCALLERCONTACT_VALUE TEXT("Last Incoming Caller OID")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallerNumber
// Gets the currently connected caller's phone number.
#define SN_PHONETALKINGCALLERNUMBER_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLERNUMBER_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLERNUMBER_VALUE TEXT("Caller Number")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallerContactPropertyName
// Gets the name of the property of the contact who is on the active phone call, e.g. "h" for "Home Telephone".
#define SN_PHONETALKINGCALLERCONTACTPROPERTYNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLERCONTACTPROPERTYNAME_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLERCONTACTPROPERTYNAME_VALUE TEXT("Caller PropName")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallerContactPropertyID
// Gets the CEPROPID of the property of the contact who is on the active phone call, for example, PIMPR_HOME_TELEPHONE_NUMBER.
#define SN_PHONETALKINGCALLERCONTACTPROPERTYID_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLERCONTACTPROPERTYID_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLERCONTACTPROPERTYID_VALUE TEXT("Caller PropID")
 
  
////////////////////////////////////////////////////////////////////////////////
// PhoneTalkingCallerContact
// Gets the CEOID of the contact who is on the active phone call.
#define SN_PHONETALKINGCALLERCONTACT_ROOT HKEY_LOCAL_MACHINE
#define SN_PHONETALKINGCALLERCONTACT_PATH TEXT("System\\State\\Phone")
#define SN_PHONETALKINGCALLERCONTACT_VALUE TEXT("Caller OID")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsCount
// Gets a value indicating the number of connections that are currently connected.
#define SN_CONNECTIONSCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSCOUNT_PATH TEXT("System\\State\\Connections")
#define SN_CONNECTIONSCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsBluetoothCount
// Gets a value indicating the number of Bluetooth connections that are currently connected.
#define SN_CONNECTIONSBLUETOOTHCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSBLUETOOTHCOUNT_PATH TEXT("System\\State\\Connections\\Bluetooth")
#define SN_CONNECTIONSBLUETOOTHCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsBluetoothDescriptions
// Gets a semicolon delimited string containing the description of each of the Bluetooth connections.
#define SN_CONNECTIONSBLUETOOTHDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSBLUETOOTHDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Bluetooth")
#define SN_CONNECTIONSBLUETOOTHDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsCellularCount
// Gets a value indicating the number of cellular connections that are currently connected.
#define SN_CONNECTIONSCELLULARCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSCELLULARCOUNT_PATH TEXT("System\\State\\Connections\\Cellular")
#define SN_CONNECTIONSCELLULARCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsCellularDescriptions
// Gets a semicolon delimited string containing the description of each cellular connection.
#define SN_CONNECTIONSCELLULARDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSCELLULARDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Cellular")
#define SN_CONNECTIONSCELLULARDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsNetworkCount
// Gets a value indicating the number of network connections that are currently connected.
#define SN_CONNECTIONSNETWORKCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKCOUNT_PATH TEXT("System\\State\\Connections\\Network")
#define SN_CONNECTIONSNETWORKCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsNetworkDescriptions
// Gets a semicolon delimited string containing the description of each network connection.
#define SN_CONNECTIONSNETWORKDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Network")
#define SN_CONNECTIONSNETWORKDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsNetworkAdapters
// Gets a semicolon delimited string containing the adapter name of each network connection.
#define SN_CONNECTIONSNETWORKADAPTERS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSNETWORKADAPTERS_PATH TEXT("System\\State\\Connections\\Network")
#define SN_CONNECTIONSNETWORKADAPTERS_VALUE TEXT("Adapters")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsDesktopCount
// Gets a value indicating the number of desktop connections that are currently connected.
#define SN_CONNECTIONSDESKTOPCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSDESKTOPCOUNT_PATH TEXT("System\\State\\Connections\\Desktop")
#define SN_CONNECTIONSDESKTOPCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsDesktopDescriptions
// Gets a semicolon delimited string containing the description of each desktop connection.
#define SN_CONNECTIONSDESKTOPDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSDESKTOPDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Desktop")
#define SN_CONNECTIONSDESKTOPDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsProxyCount
// Gets a value indicating the number of network connections that are currently connected.
#define SN_CONNECTIONSPROXYCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSPROXYCOUNT_PATH TEXT("System\\State\\Connections\\Proxy")
#define SN_CONNECTIONSPROXYCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsProxyDescriptions
// Gets a semicolon delimited string containing the description of each proxy connection.
#define SN_CONNECTIONSPROXYDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSPROXYDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Proxy")
#define SN_CONNECTIONSPROXYDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsModemCount
// Gets a value indicating the number of modem connections that are currently connected.
#define SN_CONNECTIONSMODEMCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSMODEMCOUNT_PATH TEXT("System\\State\\Connections\\Modem")
#define SN_CONNECTIONSMODEMCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsModemDescriptions
// Gets a semicolon delimited string containing the description of each modem connection.
#define SN_CONNECTIONSMODEMDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSMODEMDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Modem")
#define SN_CONNECTIONSMODEMDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsUnknownCount
// Gets a value indicating the number of connections of unknown type that are currently connected.
#define SN_CONNECTIONSUNKNOWNCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSUNKNOWNCOUNT_PATH TEXT("System\\State\\Connections\\Unknown")
#define SN_CONNECTIONSUNKNOWNCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsUnknownDescriptions
// Gets a semicolon delimited string containing the description of each connections of unknown type.
#define SN_CONNECTIONSUNKNOWNDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSUNKNOWNDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Unknown")
#define SN_CONNECTIONSUNKNOWNDESCRIPTIONS_VALUE TEXT("Descriptions")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsVpnCount
// Gets a value indicating the number of virtual private network connections that are currently connected.
#define SN_CONNECTIONSVPNCOUNT_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSVPNCOUNT_PATH TEXT("System\\State\\Connections\\Vpn")
#define SN_CONNECTIONSVPNCOUNT_VALUE TEXT("Count")
 
  
////////////////////////////////////////////////////////////////////////////////
// ConnectionsVpnDescriptions
// Gets a semicolon delimited string containing the description of each virtual private network connection.
#define SN_CONNECTIONSVPNDESCRIPTIONS_ROOT HKEY_LOCAL_MACHINE
#define SN_CONNECTIONSVPNDESCRIPTIONS_PATH TEXT("System\\State\\Connections\\Vpn")
#define SN_CONNECTIONSVPNDESCRIPTIONS_VALUE TEXT("Descriptions")
 

////////////////////////////////////////////////////////////////////////////////
// BluetoothStatePowerOn
// Gets a value indicating whether Bluetooth is powered on
#define SN_BLUETOOTHSTATEPOWERON_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEPOWERON_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEPOWERON_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEPOWERON_BITMASK 1


////////////////////////////////////////////////////////////////////////////////
// BluetoothStateDiscoverable
// Gets a value indicating whether Bluetooth is discoverable
#define SN_BLUETOOTHSTATEDISCOVERABLE_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEDISCOVERABLE_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEDISCOVERABLE_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEDISCOVERABLE_BITMASK 2


////////////////////////////////////////////////////////////////////////////////
// BluetoothStateA2DPConnected
// Gets a value indicating whether Bluetooth A2DP is connected
#define SN_BLUETOOTHSTATEA2DPCONNECTED_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEA2DPCONNECTED_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEA2DPCONNECTED_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEA2DPCONNECTED_BITMASK 4


////////////////////////////////////////////////////////////////////////////////
// BluetoothStateHardwarePresent
// Gets a value indicating whether Bluetooth hardware is present
#define SN_BLUETOOTHSTATEHARDWAREPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEHARDWAREPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEHARDWAREPRESENT_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEHARDWAREPRESENT_BITMASK 8


////////////////////////////////////////////////////////////////////////////////
// BluetoothStateHandsFreeControl
// Gets a value indicating whether device is under Bluetooth Hands Free Control 
#define SN_BLUETOOTHSTATEHANDSFERECONTROL_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEHANDSFERECONTROL_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEHANDSFERECONTROL_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEHANDSFERECONTROL_BITMASK 0x10


////////////////////////////////////////////////////////////////////////////////
// BluetoothStateHandsFreeAudio
// Gets a value indicating whether the device is under Bluetooth handsfree audio and control
#define SN_BLUETOOTHSTATEHANDSFREEAUDIO_ROOT HKEY_LOCAL_MACHINE
#define SN_BLUETOOTHSTATEHANDSFREEAUDIO_PATH TEXT("System\\State\\Hardware")
#define SN_BLUETOOTHSTATEHANDSFREEAUDIO_VALUE TEXT("Bluetooth")
#define SN_BLUETOOTHSTATEHANDSFREEAUDIO_BITMASK 0x20


////////////////////////////////////////////////////////////////////////////////
// WiFiStateHardwarePresent
// Gets a value indicating whether Wi-Fi is powered on
#define SN_WIFISTATEHARDWAREPRESENT_ROOT HKEY_LOCAL_MACHINE
#define SN_WIFISTATEHARDWAREPRESENT_PATH TEXT("System\\State\\Hardware")
#define SN_WIFISTATEHARDWAREPRESENT_VALUE TEXT("WiFi")
#define SN_WIFISTATEHARDWAREPRESENT_BITMASK 1
 

////////////////////////////////////////////////////////////////////////////////
// WiFiStatePowerOn
// Gets a value indicating whether Wi-Fi is powered on
#define SN_WIFISTATEPOWERON_ROOT HKEY_LOCAL_MACHINE
#define SN_WIFISTATEPOWERON_PATH TEXT("System\\State\\Hardware")
#define SN_WIFISTATEPOWERON_VALUE TEXT("WiFi")
#define SN_WIFISTATEPOWERON_BITMASK 2
 

////////////////////////////////////////////////////////////////////////////////
// WiFiStateNetworksAvailable
// Gets a value indicating whether Wi-Fi is powered on
#define SN_WIFISTATENETWORKSAVAILABLE_ROOT HKEY_LOCAL_MACHINE
#define SN_WIFISTATENETWORKSAVAILABLE_PATH TEXT("System\\State\\Hardware")
#define SN_WIFISTATENETWORKSAVAILABLE_VALUE TEXT("WiFi")
#define SN_WIFISTATENETWORKSAVAILABLE_BITMASK 4
 
  
////////////////////////////////////////////////////////////////////////////////
// WiFiStateConnecting
// Gets a value indicating whether Wi-Fi is connecting to a network
#define SN_WIFISTATECONNECTING_ROOT HKEY_LOCAL_MACHINE
#define SN_WIFISTATECONNECTING_PATH TEXT("System\\State\\Hardware")
#define SN_WIFISTATECONNECTING_VALUE TEXT("WiFi")
#define SN_WIFISTATECONNECTING_BITMASK 8
 
  
////////////////////////////////////////////////////////////////////////////////
// WiFiStateConnected
// Gets a value indicating whether Wi-Fi is connected to a network
#define SN_WIFISTATECONNECTED_ROOT HKEY_LOCAL_MACHINE
#define SN_WIFISTATECONNECTED_PATH TEXT("System\\State\\Hardware")
#define SN_WIFISTATECONNECTED_VALUE TEXT("WiFi")
#define SN_WIFISTATECONNECTED_BITMASK 0x10
 
  
////////////////////////////////////////////////////////////////////////////////
// ClamshellClosed
// Gets a value indicating whether the clamshell is closed.
#define SN_CLAMSHELLCLOSED_ROOT HKEY_LOCAL_MACHINE
#define SN_CLAMSHELLCLOSED_PATH TEXT("System\\State\\Hardware")
#define SN_CLAMSHELLCLOSED_VALUE TEXT("ClamshellClosed")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEvent
// Gets the CEOID of the current "All Day Event" Appointment.
#define SN_CALENDAREVENT_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENT_PATH TEXT("System\\State\\Appointments")
#define SN_CALENDAREVENT_VALUE TEXT("Event")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventSubject
// Gets the subject of the current "All Day Event" Appointment.
#define SN_CALENDAREVENTSUBJECT_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTSUBJECT_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTSUBJECT_VALUE TEXT("Subject")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventLocation
// Gets the location of the current "All Day Event" Appointment.
#define SN_CALENDAREVENTLOCATION_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTLOCATION_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTLOCATION_VALUE TEXT("Location")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventStartTime
// Gets the starting time of the current "All Day Event" Appointment. 
// This will be the starting day of an Appointment Event that spans multiple days.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDAREVENTSTARTTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTSTARTTIME_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTSTARTTIME_VALUE TEXT("Start Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventEndTime
// Gets the ending time of the current "All Day Event" Appointment. 
// This will be the starting day of an Appointment Event that spans multiple days.
// The value is a FILETIME stored in REG_BINARY format.
#define SN_CALENDAREVENTENDTIME_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTENDTIME_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTENDTIME_VALUE TEXT("End Time")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventHasConflict
// Gets a value indicating whether the user's availability (free/busy) status conflicts with the current "All Day Event".
#define SN_CALENDAREVENTHASCONFLICT_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTHASCONFLICT_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTHASCONFLICT_VALUE TEXT("Conflict")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventBusyStatus
// Gets the user's availability (free/busy status) for the current "All Day Event".
// The value is of type OlBusyStatus (from pimstore.h).
#define SN_CALENDAREVENTBUSYSTATUS_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTBUSYSTATUS_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTBUSYSTATUS_VALUE TEXT("Busy Status")
 
  
////////////////////////////////////////////////////////////////////////////////
// CalendarEventCategories
// Gets the categories for the current "All Day Event".
#define SN_CALENDAREVENTCATEGORIES_ROOT HKEY_CURRENT_USER
#define SN_CALENDAREVENTCATEGORIES_PATH TEXT("System\\State\\Appointments\\Event")
#define SN_CALENDAREVENTCATEGORIES_VALUE TEXT("Categories")
 
////////////////////////////////////////////////////////////////////////////////
// CellNetwork available
// Gets a value indicating current network attachment state
#define SN_CELLSYSTEMAVAILABLE_ROOT HKEY_LOCAL_MACHINE
#define SN_CELLSYSTEMAVAILABLE_PATH TEXT("System\\State\\Phone")
#define SN_CELLSYSTEMAVAILABLE_VALUE TEXT("Cellular System Available")
#define SN_CELLSYSTEMAVAILABLE_GPRS_BITMASK     1
#define SN_CELLSYSTEMAVAILABLE_1XRTT_BITMASK    2
#define SN_CELLSYSTEMAVAILABLE_1XEVDO_BITMASK   4
#define SN_CELLSYSTEMAVAILABLE_EDGE_BITMASK     8
#define SN_CELLSYSTEMAVAILABLE_UMTS_BITMASK     16
#define SN_CELLSYSTEMAVAILABLE_EVDV_BITMASK     32
#define SN_CELLSYSTEMAVAILABLE_HSDPA_BITMASK    64

////////////////////////////////////////////////////////////////////////////////
// CellNetwork Connected
// Gets a value indicating what cellular system is used for connection 
#define SN_CELLSYSTEMCONNECTED_ROOT HKEY_LOCAL_MACHINE
#define SN_CELLSYSTEMCONNECTED_PATH TEXT("System\\State\\Phone")
#define SN_CELLSYSTEMCONNECTED_VALUE TEXT("Cellular System Connected")
#define SN_CELLSYSTEMCONNECTED_GPRS_BITMASK     1
#define SN_CELLSYSTEMCONNECTED_1XRTT_BITMASK    2
#define SN_CELLSYSTEMCONNECTED_1XEVDO_BITMASK   4
#define SN_CELLSYSTEMCONNECTED_EDGE_BITMASK     8
#define SN_CELLSYSTEMCONNECTED_UMTS_BITMASK     16
#define SN_CELLSYSTEMCONNECTED_EVDV_BITMASK     32
#define SN_CELLSYSTEMCONNECTED_HSDPA_BITMASK    64
#define SN_CELLSYSTEMCONNECTED_CSD_BITMASK      2147483648 

////////////////////////////////////////////////////////////////////////////////
// InternetSharing UI State
// Gets the bits indicating the current state of InternetSharing
#define SN_INTERNETSHARING_ROOT HKEY_LOCAL_MACHINE
#define SN_INTERNETSHARING_PATH TEXT("System\\State\\Connectivity")
#define SN_INTERNETSHARING_VALUE TEXT("InternetSharing")

// InternetSharing has successfully been loaded and is ready for use
#define SN_INTERNETSHARING_PROCESS_RUNNING    0x00000001
// InternetSharing data session is currently enabled.  This means InternetSharing is either connecting or connected.
#define SN_INTERNETSHARING_ENABLED            0x00000002
// InternetSharing has a valid cellular data connection 
#define SN_INTERNETSHARING_DATA_CONNECTED     0x00000004
// InternetSharing has a valid connection with PC
#define SN_INTERNETSHARING_HOST_CONNECTED     0x00000008
// Connection with PC is over USB
#define SN_INTERNETSHARING_HOST_USB           0x00000010
// Connection with PC is over Bluetooth
#define SN_INTERNETSHARING_HOST_BLUETOOTH     0x00000020
 
////////////////////////////////////////////////////////////////////////////////
// Lock
// Specifies the current lock state.
#define SN_LOCK_ROOT     HKEY_LOCAL_MACHINE
#define SN_LOCK_PATH     TEXT("System\\State")
#define SN_LOCK_VALUE    TEXT("Lock")

#define SN_LOCK_BITMASK_DEVICELOCKED    0x00000001  // Device locked
#define SN_LOCK_BITMASK_KEYLOCKED       0x00000002  // Key locked
#define SN_LOCK_BITMASK_SIMLOCKED       0x00000004  // SIM locked

////////////////////////////////////////////////////////////////////////////////
//Keyboard input mode
// Gets a value indicating what’s the current input mode of keyboard driver
#define SN_KEYBDINPUTMODE_ROOT HKEY_LOCAL_MACHINE
#define SN_KEYBDINPUTMODE_PATH TEXT("System\\State\\Keyboard")
#define SN_KEYBDINPUTMODE_VALUE TEXT("CurrentInputMode")

////////////////////////////////////////////////////////////////////////////////
// Clock Alarm
// Gets the Clock Alarm Status
#define SN_CLOCKALARMSTATUS_ROOT    HKEY_LOCAL_MACHINE
#define SN_CLOCKALARMSTATUS_PATH    TEXT("System\\State\\Shell")
#define SN_CLOCKALARMSTATUS_VALUE   TEXT("Notify")
#define SN_CLOCKALARMSTATUS_BITMASK 0x00000400 //Clock Alarm status, ON / OFF.


////////////////////////////////////////////////////////////////////////////////
// Call Recording Status
// Specifies if there is an active call recording
#define SN_CALLRECORDINGSTATUS_ROOT    HKEY_LOCAL_MACHINE
#define SN_CALLRECORDINGSTATUS_PATH    TEXT("System\\State\\CallRecording")
#define SN_CALLRECORDINGSTATUS_VALUE   TEXT("Status")
#define SN_CALLRECORDINGSTATUS_BITMASK 0x00000001


////////////////////////////////////////////////////////////////////////////////
// Call Recording Start Time
// If there is a recording in progress, it keeps the UTC Filetime of the start
// of the recording
#define SN_CALLRECORDINGSTARTTIME_ROOT     HKEY_LOCAL_MACHINE
#define SN_CALLRECORDINGSTARTTIME_PATH     TEXT("System\\State\\CallRecording")
#define SN_CALLRECORDINGSTARTTIME_VALUE    TEXT("StartTime")


////////////////////////////////////////////////////////////////////////////////
// Service Provider Name
// Gets the name of the mobile operator (i.e., the mobile phone company, or carrier).
#define SN_SERVICEPROVIDERNAME_ROOT HKEY_LOCAL_MACHINE
#define SN_SERVICEPROVIDERNAME_PATH TEXT("System\\State\\Phone")
#define SN_SERVICEPROVIDERNAME_VALUE TEXT("Service Provider Name")

