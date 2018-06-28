#Android Installation Guidelines

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)** 

2. Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)** 

3. Download and install **[Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** version from [Ruby.org]( http://rubyinstaller.org/downloads/archives)

4. Download and install **[Android SDK](http://developer.android.com/intl/es/sdk/index.html)**

5. Download and install **[Android NDK revision 9d](http://developer.android.com/intl/es/ndk/downloads/revision_history.html)**

6. If building barcode, indicators or smartCradle extensions, download and install extension-specific libraries: 
	*  EMDK 3.1 UP1 libraries from **[Zebra Support Central](http://support.symbol.com/)** 
7. Download and install **[Zebra SDK](https://www.zebra.com/us/en/products/software/barcode-printers/zebralink/link-os-sdk.html)**. This SDK is used by **Printing** APIs.

8. **Ruby Gem Installation**:
	* Open command prompt and type ```gem list```
	* Install any missing gems from the list below: 
		* At the command prompt, type **```gem install <gem_name>``` (i.e. ```rest_client```, ```rest-client```, ```listen```, etc.)** 
		* Set the Ruby environment variables prior to use of the ```gem``` command:
		- ```gem install rest_client``` 	<i>- Must be installed</i>
		- ```gem install listen --version 3.0.6```			<i>- Must be installed</i>
		- ```gem install templater``` 		<i>- Must be installed</i>
		- ```gem install uuid```			<i>- Must be installed</i>
		- ```gem install net-scp```			<i>- Must be installed</i>
		- ```gem install CFPropertyList```	<i>- Install only if required and not present</i>
		- ```gem install rubyzip``` <i>- Install only if required and not present</i>
		- ```gem install rhomobile-debug``` <i>- Install only if required and not present</i>
		- ```gem install rhodes-translator``` <i>- Must be installed if ***rhodes_translator*** extension is added in ```build.yml``` file</i>
		- ```gem install rake``` 			<i>- Install only if required and not present, this is actually present by default</i>
		- ```gem install systemu```			<i>- Present by default; install only if required and not present</i>
		- ```gem install json``` 			<i>- Present by default; install only if required and not present</i>
		- ```gem install celluloid```		<i>- Present by default; install only if required and not present. Note: Internally, this installs **hitimes** and **timers** **gems** as well.</i>

9. **Environment variables** - Append the variables below to the System **`Path`** environment variable: 

* C:\project\rhodes\bin;
* C:\Ruby193\bin;

#Android Build Guidelines
1. Clone **rhodes** repository to the local machine.

2. Create **rhobuild.yml** if it does not exist:
	- Open a command prompt and navigate to to any **RhoMobile Application** directory
	
	- Create ```rhobuild.yml```, if does not exist inside the **rhodes** directory:
		- Type from command prompt ```rhodes-setup```
		- At each prompt, set the indicated path and hit Enter (or just hit Enter to skip):
			- **Java JDK** (i.e. C:/Program Files/Java/jdk1.8.0_74)
		 	- **Android SDK**  
		 	- **Android NDK** 
		 	- **CabWiz** (i.e. C:/Program Files (x86)/Windows Mobile 6 SDK/Tools/CabWiz
			- Just hit Enter to skip the following: 
			- **BlackBerry JDE 4.6**
		 	- **BlackBerry JDE 4.6 MDS**
		 	- **BlackBerry JDE 4.2**
		 	- **BlackBerry JDE 4.2 MDS**

 The file **```rhobuild.yml```** will be created in the cloned **rhodes** directory.

&#51;. Modify **```build.yml```** of **RhoMobile Application** that has been chosen for building. Point the ```sdk``` path to the cloned **rhodes** directory location. Ex: ```sdk: ../rhodes```

&#52;. From the command prompt, navigate to **RhoMobile Application** project directory and type **```rake clean:android```**. This will clean the earlier build binaries (if any).

&#53;. After cleaning, type ```rake device:android:production``` to make a production build of **RhoMobile Application**.

 A successful build will generate the artifacts and apk files of the **RhoMobile Application** for the **Android** platform.

#Extension Build Guidelines for BARCODE
1. If building barcode, indicators or smartcradle extensions, be sure to install EMDK for Android (available from **[Zebra Support Central](http://support.symbol.com/)**) on the local machine.

2. After successfully installing EMDK, find ```addon-symbol-emdk_v4.0_API-16 and addon-symbol-emdk_v4.0_API-19``` in the add-on folder of the Android SDK (which is specified in the ```rhobuild.yml``` file).

3. For all extensions that need EMDK to be installed in developer’s machine to build an extension, the ```emdk3-manager``` extension must be inserted before all other extensions in the ```build.yml```. This is because the ```emdk3-manager``` extension is the single gateway for all EMDK interactions when multiple extensions are present.

 For example:

	  android:
    	extensions:
		- emdk3-manager
		- barcode


4. Barcode 2.2 APIs are not supported in OSS. If building from code, it is therefore necessary to migrate to the version 4.x API,. Alternatively, it's also possible to use DataWedge to scan from the app.

5. To build from source, only Symbol- and Zebra-branded devices and consumer devices (see note below) are supported as targets.

6. If EMDK is not installed, the application build itself will fail. In such cases, remove the extension ```barcode``` and ```emdk3-manager``` from ```build.yml``` and perform the build. Other extensions that are NOT dependent on EMDK should also build successfully.

7. Note: If building for a consumer device, EMDK and zxing libraries (for processing 1D/2D barcode images) are required.

8. The OSS release supports EMDK 3.1 UP1 as a base version. 

#Extension Build Guidelines for PRINTING

1. Download and install the **[Printer Zebra SDK for Android](https://www.zebra.com/us/en/products/software/barcode-printers/zebralink/link-os-sdk.html)**

2. Copy all the JARs from the local machine directory to the directory ```.\rhodes\lib\commonAPI\printing_zebra\ext\platform\android\ZEBRA_SDK```

3. If ```svg-android-1.1.jar``` and different versions of ```httpmime-4.1.1.jar and commons-net-3.1.jar``` are there in the printing directory (same or different), they must be replaced with latest version of the JARs in the location ```rhoelementsext/ext/rhoelementsext/platform/android/rhoelements_temp/libs``` directory when both 2.2 APIS and printing_zebra extension are getting built. As both extension require the same JAR, it will create build issue if different versions are included. Hence, it is recommended to replace any duplicate JAR file from rhoelements extension directory.

#Extension Build Guidelines for 2.2 APIS

1. To build 2.2 APIs, include the rhoelements extension or app_type="rhoelements"

2. MSR, Scanner and MPM plug-ins CANNOT be built from open source

#Extension Build Guidelines for CardReader

The Cardreader extension build is NOT supported on OSS. If using this API, the only option is to migrate to DataWedge.
