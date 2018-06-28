#RhoSimulator Build Prerequisites For Windows

###Note: It is recommended to perform steps in this order:

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)** 

2.  Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)** 

3. **[Download Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** from [ruby.org]( http://rubyinstaller.org/downloads/archives) and install it.

4. **Ruby Gem Installation**:
	* Open command prompt and type ```gem list``` to display gems currently installed on the system. 
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

5. **Environment variables** - Append the variables below to the System **`Path`** environment variable: 
**Note: This step is _required_ for proper operation**. 

* C:\project\rhodes\bin;
* C:\Ruby193\bin;

**Important**: Settings should reflect the installation paths for their respective tools. The example is based on the ***```rhodes```*** repository cloned inside the ***```C:\project\rhodes```*** directory and ***```Ruby193```*** being installed in the ***```C:\Ruby193\```*** directory. 

If not already present:

&#54;. Download and install a licensed version of **Microsoft Professional Visual Studio 2008 or 2012**.

&#55;. Download and install **[.NET Compact Framework 3.5](https://www.microsoft.com/en-sg/download/details.aspx?id=65)** i.e. ```NETCFSetupv35.msi```.

&#56;. Download and install **[.NET Compact Framework 4.0](https://www.microsoft.com/en-in/download/details.aspx?id=17851)** i.e. ```dotNetFx40_Full_setup.exe```.

&#57;. Download and install **[NSIS](http://sourceforge.net/projects/nsis/files/NSIS%203%20Pre-release/3.0b0/nsis-3.0b0-setup.exe/download)** - i.e. ```nsis-3.0b0-setup.exe```.

&#49;&#48;. **Download and Extract** the appropriate version of **Qt** as indicated below: 

- To target **Windows XP/XPE** with **Microsoft Visual Studio 2008**:
	- **[download Zebra’s Qt 5.1.1.0 VS2008 binaries](http://rhomobile-suite.s3.amazonaws.com/Qt/Qt5-vs2008.7z)**
 	- Extract the content at **```C:\Qt\Qt5-vs2008```** directory in the machine
 	- Add the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-vs2008```**
- To target **Windows XP/XPE** with **Microsoft Visual Studio 2012**: 
	- **[download Zebra’s Qt 5.1.1.0 VS2012 binaries](http://rhomobile-suite.s3.amazonaws.com/Qt/Qt5-rhoxp.7z)**
 	- Extract the content at **```C:\Qt\Qt5-rhoxp```** directory in the machine 
 	- Add the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-rhoxp```**

#RhoSimulator Build Guidelines For Windows Desktop Machine
1. Clone **```rhoconnect-client```** repository to the local machine.

2. Clone **```rhodes```** repository to the local machine into the same location as **```rhoconnect-client```** repository.

3. Create **rhobuild.yml** if it does not exist:
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

&#52;. Open **```rhobuild.yml```** that was created inside the cloned **```rhodes```** repository: 

   - Ensure **```nsis```** path is configured in ```rhobuild.yml```. (**Example:** ```nsis: C:/Program Files (x86)/NSIS/makensis.exe``` or wherever installed in the desktop machine.)
   - If more than one version of **Microsoft Visual Studio** is installed, the system by default will build the application using the newest supported version. To explicitly specify the version of Visual Studio to use, add the parameter **```msvc```** to the **```win32```** section of your **```rhobuild.yml```** and specify **2012** or **2008** as shown in the example: 
   
     ```
     win32:
          msvc: 2008
     ```
     or
     
     ```
     win32:
          msvc: 2012
     ```
   
&#53;. Ensure that the right version of **```Qt```** is set for the above configuration:
   
   * For **```Windows Desktop```** machine, if **Visual Studio 2008** is used for building then change the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-vs2008```**. Ensure that the extracted **```Qt```** stuff for **Visual Studio 2008** environment is **```Qt5-vs2008```**, otherwise it will lead to error.
   
   * For **```Windows Desktop```** machine, if **Visual Studio 2012** is used for building then change the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-rhoxp```**. Ensure that the extracted **```Qt```** stuff for **Visual Studio 2012** environment is **```Qt5-rhoxp```**, otherwise it will lead to error.
   
&#54;. From the command prompt, navigate to the cloned **```rhodes```** repository and type the following command to build ```RhoSimulator```: 

    rake config:wm:win32:ignore_vsprops build:rhosimulator --trace
     
&#55;. A successful build will generate a folder called **```RhoSimulator```** in the **```rhodes\platform\win32\```** directory.
