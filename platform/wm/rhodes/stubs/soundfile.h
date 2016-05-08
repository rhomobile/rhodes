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
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//

// **************************************************************************
//  APIs for dealing with system sounds
//
//  SndGetSoundDirectoriesList       Gets a list of available directories for
//                                   an indicated sound event and memory
//                                   location(s).
//  SndGetSoundFileList              Gets a list of sounds for an indicated 
//                                   event currently available.
//  SndSetSound                      Sets sound for an indicated event
//  SndGetSound                      Gets current sound for indicated event
// **************************************************************************

#ifndef _SOUNDFILE_H_
#define _SOUNDFILE_H_

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */



// **************************************************************************
// SND_SOUNDTYPE
// 
// Purpose: This enum indicates the type of sound for the SNDFILEINFO 
//          structure.
//
// **************************************************************************
typedef enum tagSND_SOUNDTYPE
{
    SND_SOUNDTYPE_ON,             // If sound is currently Vibrate or None this will restore the 
                                  // sound to the previous value. This is only valid for the SND_EVENT_ALL
                                  // SND_EVENT.
    SND_SOUNDTYPE_FILE,           // Soundfile will be specified by SNDFILEINFO.szPathName
    SND_SOUNDTYPE_VIBRATE,        // Sound is vibration.  SNDFILEINFO.szPathName is ignored.
    SND_SOUNDTYPE_NONE,           // No sound (silence).  SNDFILEINFO.szPathName is ignored.
    SND_SOUNDTYPE_DISABLE,        // Only applies to SND_EVENTs for KNOWNCALLER and ROAMING
                                  // Sound will be disabled and behavior will revert back to default.
    
    SND_SOUNDTYPE_LAST = SND_SOUNDTYPE_DISABLE
} SND_SOUNDTYPE;





// **************************************************************************
// SNDFILEINFO
// 
// Purpose: This structure is used to set and retrieve information about 
//          event sounds.
//
// Fields:
//      [in/out] TCHAR szPathName[MAX_PATH]
//          A pointer to a string indicating the fully qualified
//          path to the sound file.
//
//      [in/out] TCHAR szDisplayName[MAX_PATH]
//          A pointer to a string indicating the friendly name of a 
//          sound file, that is the name of the file without extension
//          or path. When used with SndSetSoundFile, szDisplayName is ignored.
//
//      [in/out] SND_SOUNDTYPE sstType
//          An enum indicating the type of sound that is played for the event.
//          If this is not equal to SND_SOUNDTYPE_FILE, szPathName is ignored.
// **************************************************************************
typedef struct tagSNDFILEINFO
{
    TCHAR szPathName[MAX_PATH];
    TCHAR szDisplayName[MAX_PATH];
    SND_SOUNDTYPE sstType;    
} SNDFILEINFO;





// **************************************************************************
// SNDDIRECTORYINFO
// 
// Purpose: This structure is returned when querying for directories for
//          sound events.
//
// Fields:
//      [in/out] TCHAR szPathName[MAX_PATH]
//          A string indicating the fully qualified
//          path to the directory containing sound files.
//
//
// Notes:   This struct is used with SndGetSoundDirectoriesList.
//
//
// Description:
// **************************************************************************
typedef struct tagSNDDIRECTORYINFO
{
    TCHAR szPathName[MAX_PATH];
} SNDDIRECTORYINFO;






// **************************************************************************
// SND_EVENT
// 
// Purpose: This enum indicates which sound event type the user wishes to
//          set or query.
//
// **************************************************************************
typedef enum tagSND_EVENT
{
    SND_EVENT_ALL_NONRING,               // This is a special sound event that applies to all sounds (excluding ringtones)
                                         // on a system wide scale. Currently only supported on Pocket PC, the
                                         // only valid values for SNDFILEINFO.sstType are SND_SOUNDTYPE_ON,
                                         // SND_SOUNDTYPE_VIBRATE, and SND_SOUNDTYPE_NONE. Invoking SndSetSound
                                         // with SND_EVENT_ALL and a SNDFILEINFO struct set to one of the above
                                         // values will be equivalent to the user using the Pocket PC Volume Bubble
                                         // on the home screen. If the volume bubble is currently open the change will
                                         // be reflected in the bubble right away. If the bubble is not open the change
                                         // will take effect immediately and will be visible the next time the user
                                         // opens the bubble.
    SND_EVENT_RINGTONELINE1,             // The ringtone for line 1 of the phone
    SND_EVENT_RINGTONELINE2,             // The ringtone for line 2 of the phone
    SND_EVENT_KNOWNCALLERRINGTONELINE1,  // The ringtone to play for a caller in the contact list who has no contact-specific ring tone.
                                         // This event does not support the "vibrate" and "none" sound types
                                         // Currently applies to both lines without distinction
    SND_EVENT_ROAMINGRINGTONELINE1,      // The ringtone to play while roaming.  This has higher priority than other ring tones.
                                         // This event does not support the "vibrate" and "none" sound types
                                         // Currently applies to both lines without distinction
    SND_EVENT_RINGTONELINEVOIP,          // The ringtone for VoIP line of the phone
    SND_EVENT_ALL,                       // All sound events, including ringtones
    
    SND_EVENT_LAST = SND_EVENT_ALL
} SND_EVENT;


// **************************************************************************
// Bitmasks for SndGetSoundFileList and SndGetSoundDirectoriesList
// **************************************************************************
#define SND_LOCATION_STORAGECARD 0x01 // Retrieve sound files or directories from 
                                      // the storage card.  For some sound events,
                                      // this may not be a valid location.
#define SND_LOCATION_USERDATA    0x02 // Retrieve sound files in userdata (
                                      // \My Documents ,
                                      // \My Documents\My Ringtones and
                                      // \Application Data\Sounds\ 
                                      // When SND_LOCATION_STORAGECARD is set,
                                      // also look in...
                                      // \<<Storage card>>
                                      // \<<Storage card>>\My Ringtones
                                      // \<<Storage card>>\My Documents
                                      // \<<Storage card>>\My Documents\My Ringtones)
                                      // Note: My Ringtone directories are only searched
                                      // for ringtone specific sound events.
#define SND_LOCATION_ROM         0x04 // Retrieve sound files in ROM.

#define SND_LOCATION_ALL         0xFFFFFFFF
                                      // A combination of all locations.  Return all.



// **************************************************************************
// Function Name:   SndGetSoundDirectoriesList
// 
// Purpose: Returns an array of possible directories for sound files for the
//          specified sound event type and memory location.
//
// Arguments:
//      [in] seSoundEvent
//          Indicates the type of sound event you wish to query for.
//
//      [in] grfLocations
//          Indicates which locations in memory are searched for sound.
//          directories.  See the bitfields adove for a description.
//
//      [out] ppSoundDirectories
//          A pointer to an array of sound directories.  This function will 
//          create the array of SNDDIRECTORYINFO's, and allocate the  
//          appropriate amount of memory.  User is resposible for freeing 
//          this memory by calling LocalFree on the returned pointer.
//
//      [out] pcSoundDirectories
//          A pointer to an int indicated number of SNDDIRECTORYINFO
//          objects created and returned in ppSoundDirectories.
//
// Return Values:
//      HRESULT
//          This function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. 
//
// Description:     
//          This function allows an application to query the directories
//          where sound files should be copied to for any of the sound events 
//          listed in the enum SND_EVENT. The fully qualified path for 
//          the directory is returned. This function allocates
//          memory for all SNDDIRECTORYINFO's as one contiguous 
//          block.  User is responsible for freeing all memory returned by 
//          this function by calling LocalFree on the returned pointer.
// **************************************************************************
HRESULT SndGetSoundDirectoriesList( SND_EVENT seSoundEvent,
                                    DWORD grfLocations, 
                                    SNDDIRECTORYINFO** ppSoundDirectories, 
                                    int* pcSoundDirectories);






// **************************************************************************
// Function Name:   SndGetSoundFileList
// 
// Purpose: Returns an array of available soundfiles on the device for the
//          specified sound event.
//
// Arguments:
//      [in] seSoundEvent
//          Indicates the type of sound event you wish to query for.
//
//      [in] grfLocations
//          Indicates which locations in memory are searched for sound files.
//          See the bitfields above for a description.
//
//      [out] ppSoundFiles
//          A pointer to an array of sound files.  This function will create 
//          the array of SNDFILEINFO's, and allocate the appropriate amount
//          of memory.  User is resposible for freeing this memory
//          by calling LocalFree.
//
//      [out] pcSoundFiles
//          A pointer to an int indicating number of SNDFILEINFO
//          objects created and returned in ppSoundFiles.
//
// Return Values:
//      HRESULT
//          This function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. 
//
// Description:     
//          This function allows an application to query sound files
//          available to the device for any of the sound events listed
//          in the enum SND_EVENT.  By specifying the appropriate
//          bitfield, sound files can be retrieved from all valid
//          sound file storage locations.  The fully qualified file path and
//          friendly file name are returned for each sound file. This function
//          allocates memory for all SNDFILEINFO's as one contiguous 
//          block.  User is responsible for freeing all memory returned by 
//          this function by calling LocalFree on the returned pointer.
// **************************************************************************
HRESULT SndGetSoundFileList(SND_EVENT seSoundEvent,
                            DWORD grfLocations, 
                            SNDFILEINFO** ppSoundFiles, 
                            int* pcSoundFiles);




// **************************************************************************
// Function Name:   SndSetSound
// 
// Purpose: Sets the current sound file for the indicated event.
//
// Arguments:
//      [in] seSoundEvent
//          Indicates which type of sound event the user wishes to set.
//
//      [in] pSoundFileInfo
//          A pointer to a SNDFILEINFO which indicates
//          the file that the user wishes to set as the indicated sound.
//          The fully qualified path should be included.  szDisplayName
//          is ignored.
//
//      [in] fSuppressUI
//          Determines if error message should be displayed if the
//          function fails to set the current sound file.  This error
//          message would only appear in the case of failure.
//
// Return Values:
//      HRESULT
//          This function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results.
//
//        
// Description:     
//          This function sets the current sound file for the appropriate
//          event.  This function can fail and will not change the current
//          sound file if it determines it is unable to play the sound file
//          for any reason.  It will also fail if the sound file is not in
//          an appropriate sounds directory.  It will fail if a sound file
//          is in the ROM sounds directory, unless it is returned by
//          SndGetSoundFileList.  If fSuppressUI is true, no user interface
//          will be displayed explaining reasons that the sound file could
//          not be set.  If it is false, then a dialog may appear explaining
//          the reason the sound file could not be set (such as insufficient
//          digital rights, an invalid file, etc).
//
// Note:
//          The SND_EVENT_ALL event type and corresponding SND_SOUNDTYPE values
//          are currently only supported on Pocket PC.
// **************************************************************************
HRESULT SndSetSound(SND_EVENT seSoundEvent,
                    const SNDFILEINFO* pSoundFileInfo,
                    BOOL fSuppressUI);




// **************************************************************************
// Function Name:   SndGetSound
// 
// Purpose:         Returns the current sound file for the indicated sound
//                  event.
//
// Arguments:      
//      [in] seSoundEvent
//          Indicates the type of sound event you wish to query.
//
//      [out] pSoundFileInfo
//          Caller should pass in a pointer to a SNDFILEINFO object 
//          that will be filled in by this function.  
//
// Return Values:
//      HRESULT
//          This function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. 
//
// Description:
//          This function returns the current sound file for the specified 
//          sound event. The fully qualified file path along with the 
//          friendly file name of the sound event file are returned in 
//          pSoundFileInfo.  This function needs a pointer to a valid 
//          SNDFILEINFO structure.
//
// Note:
//          The SND_EVENT_ALL event type and corresponding SND_SOUNDTYPE values
//          are currently only supported on Pocket PC.
// **************************************************************************
HRESULT SndGetSound(SND_EVENT seSoundEvent,
                    SNDFILEINFO* pSoundFileInfo);


/// Possible Play Sound Flags:

/// <summary>
/// Used to play a sound at default volume ignoring the current user profile (Smartphone)
/// and current volume (Pocket PC and Smartphone).
/// The sound will not play if the user is in a phone call while the call to SndPlaySync
/// or SndPlayAsync occurs.
/// </summary>
#define SND_PLAY_IGNOREUSERSETTINGS 0x00000001

/// <summary>
/// Handle to a sound.
/// </summary>
DECLARE_HANDLE(HSOUND);

/// <summary>
/// SndScope describes a set of sounds.
/// <summary>
typedef enum SndScope
{
    SND_SCOPE_PROCESS = 0x1,
} SND_SCOPE;

/// <summary>
/// Plays a file synchronously.
/// </summary>
/// <param name="pszSoundFile">Sound filename.</param>
/// <param name="dwFlags">Indicates playback options. 
/// Currently the only flag supported is SND_PLAY_IGNOREUSERSETTINGS, so dwFlags 
/// can be either 0 or SND_PLAY_IGNOREUSERSETTINGS. If any flags beside 
/// SND_PLAY_IGNOREUSERSETTINGS are set, the function will return E_INVALIDARG.
/// </param>
/// <returns>S_OK if the sound played successfully,
/// or a standard HRESULT error code if not.</returns>
HRESULT SndPlaySync(__in LPCTSTR pszSoundFile, DWORD dwFlags);

/// <summary>
/// Creates a sound handle based on a sound file.
/// </summary>
/// <param name="pszSoundFile">Sound file name.</param>
/// <param name="phSound">Upon succesful return, it will 
/// contain a properly initialized sound handle.</param>
/// <remarks>The hSound returned should be closed using SndClose.</remarks>
/// <returns>S_OK if successful, or a standard HRESULT error code if not.</returns>
///<remarks>A sound handle is an event handle that is in a SET state when a sound is not
/// playing, and in a RESET state when a sound is playing. The following code snippet
/// shows how to wait for a sound to finish playing.
/// <code>
///    const TCHAR* c_szMid= TEXT("\\Windows\\Loudest.mid");
///    HRESULT hr = SndOpen(c_szMid, &hSound);
///    // verify hr == S_OK.
///    hr = SndPlayAsync(hSound, 0);
///    // verify hr == S_OK.
///    // do some work here and then wait for the sound to finish playing.
///    WaitForSingleObject(hSound, INFINITE);
///    hr = SndClose(hSound);
///    // verify hr == S_OK.
///    SndStop(SND_SCOPE_PROCESS, NULL);
/// </code>
/// You should call SndStop to terminate the asynchronous sounds on your process before your
/// application exits.
///</remarks>
HRESULT SndOpen (__in LPCTSTR pszSoundFile,  __out HSOUND*  phSound);

/// <summary>
/// Starts asynchronous playback of a sound. This call will be succesful
/// only if the sound is not playing already.
/// </summary>
/// <param name="hSound">The sound to play. 
/// The sound handle was created using SndOpen.</param>
/// <param name="dwFlags">Indicates playback options. 
/// Currently the only flag supported is SND_PLAY_IGNOREUSERSETTINGS, so dwFlags 
/// can be either 0 or SND_PLAY_IGNOREUSERSETTINGS. If any flags beside 
/// SND_PLAY_IGNOREUSERSETTINGS are set, the function will return E_INVALIDARG.
/// </param>
/// <returns>
/// S_OK if successful, 
/// E_HANDLE if the sound handle is currently used for playing a sound, 
/// or a standard HRESULT error code if an error occurs.
/// </returns>
///<remarks>
/// You should call SndStop to terminate the asynchronous sounds on your process before your
/// application exits.
///</remarks>
HRESULT SndPlayAsync(__in HSOUND  hSound, DWORD dwFlags);


/// <summary>
/// Returns a handle to an event that is set when the sound is done playing.
/// </summary>
/// <param name="hSound">The sound handle.</param>
/// <param name="phDoneEvent">Upon succesful return, it will contain 
/// handle to an event that is set when the sound is done playing.</param>
/// <returns>S_OK if succesful, a standard HRESULT if not.</returns>
HRESULT SndGetWaitHandle(__in HSOUND hSound, __out HANDLE* phDoneEvent);

/// <summary>
/// Stops playback of a sound.
/// </summary>
/// <param name="SoundScope">The type of sounds that should be stopped.</param>
/// <param name="hSound">Reserved for future use, set to NULL.</param>
/// <returns>S_OK if successful, or a standard HRESULT error code if not.</returns>
///<remarks>
/// You should call SndStop to terminate the asynchronous sounds on your process before your
/// application exits.
///<remarks>
HRESULT SndStop (__in SND_SCOPE SoundScope, __reserved HSOUND hSound);

/// <summary>
/// Closes a sound handle.
/// </summary>
/// <param name="hSound">The sound handle to close. 
/// The sound handle was created using SndOpen.</param>
/// <returns>S_OK if successful, or a standard HRESULT error code if not.</returns>
HRESULT SndClose (__in HSOUND hSound);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _SOUNDFILE_H_

