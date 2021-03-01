
# Developer Notes #

Detailed information on unsupported features, known issues or specific notes relating to the open source version of the RhoMobile Suite.

##Notes

- **Usage of RMS OSS release is completely free of charge**. All fees and royalties have been dropped from the [RMS open source licensing agreement](https://github.com/rhomobile/rhodes/blob/master/LICENSE).
- OSS users will **not have access cloud build** or other features of rhomobile.com. 
- Users of RMS 5.4 and earlier versions will continue to have access to [rhomobile.com](http://www.rhomobile.com). 
- **[Zebra's RhoMobile documentation site](http://docs.rhomobile.com/en/5.4/guide/welcome)** will remain available for [API reference](http://docs.rhomobile.com/en/5.4/guide/apisummary). 

For more information please refer to the [Zebra RMS Open Sourcing Announcement and FAQ](https://developer.zebra.com/community/rhomobile-suite/rhomobile-community/rhomobile-blogs/blog/2016/03/25/announcement-rhomobile-transitioning-to-open-source?et=watches.email.blog) and licensing options.

#### APIs No Longer Supported

| Module | Version | Platform | Status | Remarks |
|:-----------:|:------------:|:------------:|:------------:|:------------:|
| [CardReader](http://docs.rhomobile.com/en/5.4/api/cardreader) | 5.X API | Android |   Not Supported |  |
| [CardReader](http://docs.rhomobile.com/en/2.2.0/rhoelements/cardreader) | RE 2.2 API | Android | Not Supported |  |
| [Scanner](http://docs.rhomobile.com/en/2.2.0/rhoelements/scanner) | RE 2.2 API | Android | Not Supported |  |
| [RhoConnect Push](http://docs.rhomobile.com/en/5.4/rhoconnect/push) | 5.X API | Android | Not Supported | Supported on Symbol-branded devices only |
| [KeyState](http://docs.rhomobile.com/en/5.4/api/keystate) | 5.X API | WM/CE | Not Supported | [Use RE 2.2 KeyState API](http://docs.rhomobile.com/en/2.2.0/rhoelements/keystate) |
| [ScreenOrientation](http://docs.rhomobile.com/en/2.2.0/rhoelements/screenorientation) | RE 2.2 API | WM/CE | Not Supported | [Use 5.X ScreenOrientation API](http://docs.rhomobile.com/en/5.4/api/screenorientation) |
| [Telemetry](http://docs.rhomobile.com/en/2.2.0/rhoelements/Telemetry) | RE 2.2 API | WM/CE | Not Supported |  |
| [EInk](http://docs.rhomobile.com/en/2.2.0/rhoelements/eink) | RE 2.2 API | WM/CE | Not Supported |  |
| [IOSystem](http://docs.rhomobile.com/en/2.2.0/rhoelements/IOSystem) | RE 2.2 API | WM/CE | Not Supported |  |
| [IOAnalog](http://docs.rhomobile.com/en/2.2.0/rhoelements/IOAnalog) | RE 2.2 API | WM/CE | Not Supported |  |
| [IODigital](http://docs.rhomobile.com/en/2.2.0/rhoelements/IODigital) | RE 2.2 API | WM/CE | Not Supported |  |
| MPM | RE 2.2 API | WM/CE, Android | Not Supported |  |
| NFC | 5.X API | WM/CE, Android | Not Supported |  |

RE = RhoElements

#### Methods No Longer Supported

| Module | API Version | Method | Status | Platform |
|:-----------:|:------------:|:------------:|:------------:|:------------:|
| [Cradle](http://docs.rhomobile.com/en/2.2.0/rhoelements/cradle) | RE 2.2 API | ```getCradleId``` | Not Supported | WM/CE |

#### Properties No Longer Supported 

| Module | API Version | Properties | Status | Platform |
|:-----------:|:------------:|:------------:|:------------:|:------------:|
| [System](http://docs.rhomobile.com/en/5.4/api/system#pisMotorolaDevice) | 5.X API | ```isMotorolaDevice``` | Not Supported | All |

##Third Party Webkit Dependencies For WM/CE
**Windows Mobile/CE** framework APIs will be open-sourced without the **Webkit** component; terms and conditions of the licensing agreement between **Zebra Technologies** and **Ekioh** prohibit this module from being open-sourced. A **Windows Mobile/CE** build cannot progress unless the webkit dependency files are placed outside the **```rhodes```** directory with the open source version of rhodes. The files required after cloning **```rhodes```** locally into the machine are listed below.  

* **```thirdpartyWebkit.zip```** - contains additional dependent files required for building the **RhoMobile app** on WM/CE

* **```unzip_thirdpartyWebkit.bat```** - batch file to extract the contents of **thirdpartyWebkit.zip** inside the **```rhodes```** directory

User-managed **webkit** options:

* Provide or develop your own **webkit**  
* Engage with a partner that is developing **webkit** options, such as **[Tau Technologies](http://tau-platform.com/)** or **[Kutir](http://kutirtech.com/)** 

##OTHER KNOWN ISSUES 

### KeyState APIs Issues

Support for **5.X KeyState API** has been dropped. Developers must use the **RhoElements (RE) 2.2 KeyState API** instead, with the following conditions: 

- The **RE 2.2 KeyState API** will work only if the app makes no attempt to simultaneously use the **5.X KeyState API** in web pages.
- If **5.X KeyState API** is required, the code must be modified to handle the resource files appropriately; such handling is not done automatically. Handling of the required bitmap resource file can be done either in a ```*.dll``` or in ```rhodes.rc``` file. 
- The **5.X KeyState API** code in the rhodes cloned repositories at ```rhodes\lib\commonAPI\indicators\ext\platform\wm``` can be modified for this purpose. 

### RhoConnect Issues

- Async mode is not supported with RhoStudio debugger.
- Async mode is supported only with Thin Server.
- RhoConnect apps do not run on Windows XP due to issues with the redis server.

### RhoStudio Issues

- Debugging of RhoConnect applications does not work on Windows.
- The Clean command does not delete the cab files associated with a build.

###RhoElements App Issues

####Modules

- The following APIs are not supported in the JavaScript ORM: 
	- ```deleteAll()```
	- ```find(args)``` with advanced queries
	- ```clearNotification()```
	- ```findAll()```
	- ```findBySql(string)```
	- ```paginate```
	- ```setNotification()```
	- ```search()```
	- ```sync()``` **Note**: use ```RhoConnectClient.doSync()``` instead

- The following APIs are not supported in RhoConnect client:
	- JS Search
	- JS bulk Sync
	- JS blob Sync
	- JS onSyncCreate / Update / DeleteError handlers (apps that require this functionality are limited to the Ruby API).

- The Sensor API ```readData()``` method is not a reliable source for immediate Sensor data. Instead, use the ```start()``` method to read the data periodically.

- The RhomSource API will return incorrect sync information for the following fields: 
	- ```last_inserted_size```
	- ```last_deleted_size and distinct_objects```

####Known issues on Windows Mobile, Windows Embedded Handheld and Windows CE

- HTTP authentication (Basic and Digest) does not work on non-Zebra WM devices.
- Native applications will not launch on the MC21XX series of devices; hybrid applications are supported only using RhoElements that ships on the device.
- Supplementals will not be decoded if picklist mode is enabled on the MC65.
- The ```displayBtAddressBarcodeOnEnable``` parameter will fail to subsequently pair with an RSM device after a connection has timed out. The RSM must be manually reconnected using the hardware reconnection button.
- On large-screen CE devices such as the VC70, the MapView control will not be rendered full-screen.
- The Bluetooth module should not be instructed to ```disconnect()``` on Windows Mobile prior to establishing a connection or without specifying a device name.
- The signal icon will continue to report full signal strength on the VC70 even after the WLAN has been disconnected.
- The MapView API's ```zoom_enabled``` and ```scroll_enabled``` properties are ignored on CE7 and will default to true.
- 3D gestures will not be reliably detected on the WT41N0.
- Minimized applications on WM/CE when restored to full screen will not return to full screen regardless of the value of the fullScreen configuration option.
- On WM/CE, the Bluetooth method ```is_bluetooth_available``` will return a result regardless of whether the device has Bluetooth capabilities. 
- On Android, the Bluetooth method ```is_bluetooth_available``` will return a result regardless of whether Bluetooth is turned on.
- The Alarm API is not available on WM/CE devices.
- The ```convertGs1DataBarToUpcean``` property has no effect when scanning GS1Databar barcodes.
- Disabling poll-based synchronization with RhoConnect Client may cause issues with SSL connections. 
- The ```navigationTimeout``` property of the WebView API is not available; this setting must be specified in the configuration file.
- The following APIs should not be exercised on non-Zebra WM devices: 
	- ```Notification```
	- ```ScreenOrientation```
	- ```CardReader```
	- ```Log```
	- ```Network```
- For two or more applications to share the scanner resource on the device, each application must re-enable the scanner with its required configuration it gains focus.
- The ```app_install``` method of the System API will not work for CE devices; use ```System.applicationInstall()``` instead (to install remote applications, first download the app to the device with the Network API).
- The CaptureTrigger API is not available on the MC32 CE7 device.
- Rho supports only Microsoft Bluetooth Stack printing; Stone Street Bluetooth Stack printing is not supported. As a workaround, change the Bluetooth Stack from StoneStreet to Microsoft by changing the following registry key from 1 to 0: 
	- [HKEY_LOCAL_MACHINE\Software\SymbolBluetooth\SSStack]
- Imager on MC92 CE7 opens the camera in upside down mode. 

####Android Issues (including Zebra and consumer devices)

- The Battery and Signal icons are not shown on x86 Android devices.
- On Android 3.2, apps may not work for some carrier providers when the device has no connectivity.
- Bluetooth printers should be paired to the device prior to using APD, otherwise the first call may not be received.
- If running RhoElements on the ET1, disable the Camera when it is not in use.
- The FaceUp 3D gesture does not work on Android.
- Setting the Scanner illuminationMode will have no effect on Android.
- On Android and iOS, the Bluetooth method ```session_bt_disconnect``` will always return status 'error' in the callback regardless of success.
- The ```getProperty``` and ```getProperties``` functions do not work for the Barcode API on consumer Android.
- The Sensor API method ```makeSensorType()``` does not return a valid Sensor object on Android.
- The Sensor and Media Player APIs are not available on MC40 Gingerbread. To correct this, update the device to Jelly Bean.
- The Audio Capture API should be allowed to record for the configured number of seconds, rather than allowing the user to stop the recording on demand.
- Inline Signature Capture should not be used. Use the (non-inline) Signature API instead.
- Web Plug-ins (Flash) is available on Android 2.x and below only. On Android 3.x and above, the WebView API 'enableWebPlugins' will not successfully enable Flash.
- Regardless of the device type, the System API method ```isSymbolDevice``` will return false if the application is built without the property 'app_type:RhoElements.'
- The Intent API cannot be accessed through Ruby; it must be accessed through JavaScript.
- Tilt and Shake gestures are unavailable on the MC32 Android device.
- The Camera API is not available on the Nexus S device.

####iOS Issues

- Barcode scanning is not yet available on IPod.
- The Webview API methods ```navigationBack()``` and ```currentLocation()``` may not work as expected.
- The Camera API should not be used on iOS7.
- "outputFormat" as dataURI is not supported for Ruby over VPN; use "outputFormat" as image instead.

####Windows Phone 8 Issues

- If the compiler complains about machine type conflicts following a build, perform a clean build first.

####Issues with Windows desktop apps and RhoSimulator

- The mask color of the Toolbar API will have no effect.
- The application log is not available in RhoSimulator.

####Rendering Engine Issues

- Cookie expiration times are not reliable in Windows Mobile and Windows CE.
- CSS Font-Family property is not functioning.
- Once set, the viewport meta tag will persist across pages and is not compatible with device rotation.
- Specifying a viewport may cause form components or iFrames to be scaled incorrectly.

####Live Update Issues

- Deleting folders and sub folders works only for Full update.

###New ORM Common API Issues

- Blob type is not supported.
- Ruby implementation for Property Bag models with advanced find options (:conditions, :select, :op, :order) is incomplete and should be avoided.

## KNOWN ISSUES AFFECTING 2.2 APIS

- Not all properties of the ```Alert.show_popup``` call are available on Windows CE. To reliably display an alert box on this platform, use the simplified ```Alert.show_popup``` 'message' function.
- ```Application.restore``` does not work through Ruby on Windows devices, and debug buttons should not be used in conjunction with minimized applications.
- Setting color-model to gray scale on Android will have no effect with the Camera API.
- ```The System.zip_file``` JavaScript APIs are not supported. Please use Ruby instead.
- When using the File API, ensure the file exists before attempting to open it.
- The front-facing camera is not supported via the Camera API on the Samsung Galaxy Tab.
- ```Rhom.database_import``` does not work on Android devices.
- On Android devices, a Map created by the Rho.MapView.create API call is unstable when the page with the map remains in an idle state for a long period of time. try using JavaScript MapView instead.
- The ```System.set_network_status_notify``` method does not work on Windows Mobile, Windows CE or Windows Desktop. 
- The Bluetooth method ```stop_current_connection_process``` does not work on iPhone devices.
- The Timer and Alert Rhodes' APIs supplied in 2.2 do not work in 4.0. Use their 4.0 equivalents instead.
- The ```WebView.current_location()``` function returns the referrer page URL rather than the current URL.
- The forward button is not available on the Native Toolbar
- Issues with the ```System.get_property(attribute)``` method:

 | Attribute Name | Note |
 |----------------|----------------|
 | has_calendar | Always returns false on Android |
 | phone_id | Available only on Android |
 | country |	Not supported on Windows Mobile/CE devices. Use System.get_property('locale') instead. |

- NPAPI Objects (JavaScript Object syntax and ActiveX replacement in WebKit) are not available in HTML embedded within an SVG application.
- The Push module's "Unattended" parameter has no effect.
- The resolution should not be changed from the default value when using the Camera API.
- The Rho.GeoLocation API does not work on Windows Mobile devices.
- On Android, videoSaveEvent is not currently supported.
- The transfer capability of the video capture module should not be exercised.
- The Restore method of the Application module should not be used in hybrid or native applications.
- On Android, neither the TriggerEvent nor AlarmFiredEvent get fired reliably from Ruby.
- On Android, the Notification module does not support the LED.
- On Android, the APD 'SendString' command is not available. 
- The Signal icon available in 2.2 will no longer function on CE devices. Switch to the Signal API introduced in 4.0 for CE support.
- The Destination parameter in 2.2 Signature is not working on WM/CE. Use the Network API to upload any files to a remote destination.
- Applications using the 2.2 Scanner API should not rely on the following properties (Use 4.0 Barcode API to resolve this issue): 
	- ```sameSymbolTimeout```
	- ```differentSymbolTimeout```
	- ```connectionIdleTimeout```
	- ```enableTimeout```

- Depending on the device, the RawSensors API available in 2.2 may no longer work in 4.0. It is recommended to use Sensor API 4.0 instead.
