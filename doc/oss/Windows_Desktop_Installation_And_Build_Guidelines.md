#Windows Desktop Installation Guidelines

###Note: It is recommended to install in this order only.  

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)**.

2.  Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)**. 

3. **[Download Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** from [Ruby.org]( http://rubyinstaller.org/downloads/archives) and install.

4. **Ruby Gem Installation**:
    * Open command prompt and type ```gem list``` to display gems currently installed on the system. 
    * Install any missing gems from the list below: 
        * At the command prompt, type **```gem install <gem_name>``` (i.e. ```rest_client```, ```rest-client```, ```listen```, etc.)** 
        * Set the Ruby environment variables prior to use of the ```gem``` command:
        - ```gem install rest_client```     <i>- Must be installed</i>
        - ```gem install listen --version 3.0.6```          <i>- Must be installed</i>
        - ```gem install templater```       <i>- Must be installed</i>
        - ```gem install uuid```            <i>- Must be installed</i>
        - ```gem install net-scp```     <i>- Must be installed</i>
        - ```gem install CFPropertyList```  <i>- Install only if required and not present</i>
        - ```gem install rubyzip``` <i>- Install only if required and not present</i>
        - ```gem install rhomobile-debug``` <i>- Install only if required and not present</i>
        - ```gem install rhodes-translator``` <i>- Must be installed if ***rhodes_translator*** extension is added in ```build.yml``` file</i>
        - ```gem install rake```            <i>- Install only if required and not present, this is actually present by default</i>
        - ```gem install systemu```         <i>- Present by default; install only if required and not present</i>
        - ```gem install json```            <i>- Present by default; install only if required and not present</i>
        - ```gem install celluloid```       <i>- Present by default; install only if required and not present. Note: Internally, this installs **hitimes** and **timers** **gems** as well.</i>

5. **Environment variables** - Append the variables below to the System **`Path`** environment variable: 
**Note: This step is _required_ for proper operation**. 

* C:\project\rhodes\bin;
* C:\Ruby193\bin;

**Important**: Settings should reflect the installation paths for their respective tools. The example is based on the ***```rhodes```*** repository cloned inside the ***```C:\project\rhodes```*** directory and ***```Ruby193```*** being installed in the ***```C:\Ruby193\```*** directory. 

&#54;. Download and install a licensed version of **Microsoft Professional Visual Studio 2008 or 2012**. 

&#55;. Download and install **[.NET Compact Framework 3.5](https://www.microsoft.com/en-sg/download/details.aspx?id=65)** i.e. ```NETCFSetupv35.msi```.

&#56;. Download and install **[.NET Compact Framework 4.0](https://www.microsoft.com/en-in/download/details.aspx?id=17851)** i.e. ```dotNetFx40_Full_setup.exe```. 

&#57;. Download and install **[NSIS](http://sourceforge.net/projects/nsis/files/NSIS%203%20Pre-release/3.0b0/nsis-3.0b0-setup.exe/download)** - i.e. ```nsis-3.0b0-setup.exe```. 

&#49;&#48;. Download and install **[InstallShield Limited Edition for Visual Studio](http://learn.flexerasoftware.com/content/IS-EVAL-InstallShield-Limited-Edition-Visual-Studio)** i.e. ```InstallShield2013LimitedEdition``` or above. This is used by [RhoConnect Push Service](https://github.com/rhomobile/rhoconnect-push-service/blob/master/win32/README.md).

&#49;&#49;. **Download and Extract** the appropriate version of **Qt** as indicated below: 

- To target **Windows XP/XPE** with **Microsoft Visual Studio 2008**:
  - **[Download Zebra’s Qt 5.1.1.0 VS2008 binaries](http://rhomobile-suite.s3.amazonaws.com/Qt/Qt5-vs2008.7z)**
  - Extract the content at **```C:\Qt\Qt5-vs2008```** directory in the machine
  - Add the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-vs2008```**
- To target **Windows XP/XPE** with **Microsoft Visual Studio 2012**: 
  - **[Download Zebra’s Qt 5.1.1.0 VS2012 binaries](http://rhomobile-suite.s3.amazonaws.com/Qt/Qt5-rhoxp.7z)**
  - Extract the content at **```C:\Qt\Qt5-rhoxp```** directory in the machine 
  - Add the **```QTDIR``` System** variable with the value as **```C:\Qt\Qt5-rhoxp```**

#Windows Desktop Build Guidelines
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

&#52;. Modify the **build.yml** of **RhoMobile Application** choosen for building as indicated below.

   - Point the **```sdk```** path to the cloned **rhodes** directory location. For example: **```sdk: ../rhodes```**
   - If more than one version of **Microsoft Visual Studio** is installed, the system by default will build the application using the newest supported version. To explicitly specify the version of Visual Studio to use, add an **```msvc```** parameter to the **```win32```** section of your **```build.yml```** and specify **2012** or **2008** as shown in the below example.
   
     ```
     win32:
          msvc: 2008
     ```

&#53;. From command prompt, navigate to **RhoMobile Application** project directory and type as mentioned below.
    - **For cleaning out temporary and binary files, execute the following command:**
     - **For Windows** - ```rake clean:win32```
     - **Windows XP Embedded** - ```rake clean:winxpe```
    - **To build an installer bundle, execute the following command:**
     - **For Windows** - ```rake device:win32:production```
     - **Windows XP Embedded** - ```rake device:winxpe:production```
     
&#54;. A successful build will generate the ```.exe``` files of **RhoMobile Application** for **Windows Desktop**.

#Rhoconnect-Client Extension Guidelines

For successful operation, make sure that the **```rhoconnect-client```** extension has been included in **```build.yml```** file: 

- Clone **```rhoconnect-client```** repository into the same location where **```rhodes```** repository has been cloned.
- Add **```rhoconnect-client```** extension path in **```build.yml```** file.
  
 For Example: If the **```rhoconnect-client```** repository is cloned in the same location as the **RhoMobile Application** and the **```rhodes```** repository is also present, the path for the **```rhoconnect-client```** extension in **```build.yml``**` of **RhoMobile Application** file should be as follows:
 
    sdk: ../rhodes
    paths:
        extensions:
          - ../rhoconnect-client/ext
    ...
    ...
    win32:
    ...
    ...
    extensions: ["rhoconnect-push", "rhoconnect-client"]

 
#Rhoconnect-Push Extension Guidelines
For successful operation, make sure that the **```rhoconnect-push```** extension has been included in **```build.yml```** file: 

- Specify **```rhoconnect-client```** extension in **```build.yml```** file.

- Install **[Microsoft Messaging Queue (MSMQ)](https://msdn.microsoft.com/en-us/library/aa967729(v=vs.110).aspx)** on the machine that will be running the **RhoMobile Application**. 

- Install **```rhoconnect-push-service.exe```** on the machine that will be running the **RhoMobile Application**. 

For details, refer to the ** [rhoconnect-push-service.exe page](https://github.com/rhomobile/rhoconnect-push-service/blob/master/win32/README.md)**.

#Deploy Qt Guidelines (optional)

The size of the Windows executable can be minimized by excluding Qt and/or Visual C runtime libraries, and the target configured to execute multiple apps from a single shared instance of those DLLs. 

**Important: Optimized apps will not run on the target unless it has been configured as described this section; excluded DLLs must be installed separately on every target**. Zebra simplifies this process with a set of target runtime installers that automatically install all required Qt DLLs into a shared location and that configures target path settings accordingly. Alternatively, Qt5 DLLs can be copied to a folder on the target, the path to which must be added to the `PATH` environment variable and the `QTDIR` system variable need to be removed.

NOTE: Zebra runtime installers include Zebra's Qt library builds, and should not be used if different Qt DLLs are required.

###**Step 1 - Specify DLL exclusions**

1. **Add one or both of the boolean parameters** `deployqt` and `deploymsvc` to the `win32` section of your `build.yml` and exclude the corresponding runtimes by setting their values to 0, as below:

    ```
    win32:
      deployqt: 0
      deploymsvc: 0
    ```  

2. **Rebuild the app**; the resulting executable will exclude the specified DLLs. 

###Step 2 - Install RhoRuntime Qt installer
RhoRuntime Qt installers optimize memory footprint by installing a single instance of the Qt runtime libraries and define a path on the target to allow sharing of the libraries by multiple applications. 

1. **On the target(s)**, download and execute the appropriate installer:
   * **[RhoRuntime for QT 5.1.1.0 for Visual Studio 2008](http://rhomobile-suite.s3.amazonaws.com/Qt/RhoRuntimeQt5-VS2008Setup.exe)**
   * **[RhoRuntime for QT 5.1.1.0 for Visual Studio 2012](http://rhomobile-suite.s3.amazonaws.com/Qt/RhoRuntimeQt5-setup.exe)**
   * **[RhoRuntime for QT 5.5.0.0 for Visual Studio 2012](http://rhomobile-suite.s3.amazonaws.com/Qt/RhoRuntimeQt5.5.0.0_VS2012-Setup.exe) - (_Not compatible with Windows XP/XPE_)**

2. **Restart the target** to activate new path settings. 

###**Step 3- Install the Microsoft runtime**
[Download the Microsoft Visual C++ runtime](http://www.microsoft.com/en-sg/download/confirmation.aspx?id=5582) and install it. The installer configures the target to allow sharing of the runtime by multiple applications. 
