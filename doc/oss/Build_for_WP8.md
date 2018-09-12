#Windows Phone 8 Installation Guidelines

####Prerequisites

1. Windows 8.x OS

2. Visual Studio 2013 installed or VS Express 2013 for Windows Phone


####Steps

###Note: It is recommended to install in this order only.

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)** 

2. Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)** 

3. Download and install **[Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** version from [Ruby.org]( http://rubyinstaller.org/downloads/archives)

4. Download and install a licensed version of **Visual Studio 2013 or VS Express 2013 for Windows Phone**

5. Download and install **[Windows Phone SDK 8.0](https://www.microsoft.com/en-in/download/details.aspx?id=35471)**

6. **Ruby Gem Installation**:
	* Open command prompt and type ```gem list```
	* Install any missing gems from the list below: 
		* At the command prompt, type **```gem install <gem_name>``` (i.e. ```rest_client```, ```rest-client```, ```listen```, etc.)** 
		* Set the Ruby environment variables prior to use of the ```gem``` command:
		- ```gem install rest_client``` 	<i>- Must be installed</i>
		- ```gem install net-scp```		<i>- Must be installed</i>
		- ```gem install listen --version 3.0.6```			<i>- Must be installed</i>
		- ```gem install templater``` 		<i>- Must be installed</i>
		- ```gem install uuid```			<i>- Must be installed</i>
		- ```gem install CFPropertyList```	<i>- Install only if required and not present</i>
		- ```gem install rubyzip``` <i>- Install only if required and not present</i>
		- ```gem install rhomobile-debug``` <i>- Install only if required and not present</i>
		- ```gem install rhodes-translator``` <i>- Must be installed if ***rhodes_translator*** extension is added in ```build.yml``` file</i>
		- ```gem install rake``` 			<i>- Install only if required and not present, this is actually present by default</i>
		- ```gem install systemu```			<i>- Present by default; install only if required and not present</i>
		- ```gem install json``` 			<i>- Present by default; install only if required and not present</i>
		- ```gem install celluloid```		<i>- Present by default; install only if required and not present. Note: Internally, this installs **hitimes** and **timers** **gems** as well.</i>

7. **Environment variable** - If the **```rhodes```** repository is cloned inside the **```C:\project\rhodes```** directory, and if **```Ruby193```** is installed in the **```C:\Ruby193\```** directory, then append the path below to the System **```Path```** environment variable: 
 
    - C:\project\rhodes\bin;
	- C:\Ruby193\bin;

**Note: A correct setting of the environment variable on the development machine is required**.

#Windows Phone 8 Build Guidelines
1. Clone **rhodes** repository to the local machine

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

&#52;. From command prompt, navigate to **RhoMobile Application** project directory and type ```rake clean:wp8```. This will clean the earlier build binaries (if any).

&#53;. After cleaning, type ```rake device:wp8:production``` for production build of **RhoMobile Application**.

A successful build will generate the XAP files of the **RhoMobile Application** for **WP8**.
