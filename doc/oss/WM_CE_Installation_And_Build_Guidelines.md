# **WM/CE Installation Guidelines:**

## Note: It is recommended to install in this order only.

1. Download and install the latest version of **[Git](https://git-scm.com/download/win)**. 

2.  Download and install the latest **[JDK and JRE](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)**. 

3. **[Download Ruby-1.9.3-p194](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-1.9.3-p194.exe)** from [ruby.org]( http://rubyinstaller.org/downloads/archives) and install. 

4. Download and install a licensed version of **Microsoft Professional Visual Studio 2008**.

5. Download and install **[.NET Compact Framework 4.0](https://www.microsoft.com/en-in/download/details.aspx?id=17851)** i.e. ```dotNetFx40_Full_setup.exe```

6. Download and install **[Microsoft Windows Mobile Device Center](https://www.microsoft.com/en-in/download/details.aspx?id=3182)** for **Windows Vista or higher version** OR **[Microsoft Active Sync](https://www.microsoft.com/en-in/download/details.aspx?id=3182)** for **Windows XP**.

7. Download and install **[Windows Mobile 6 Professional SDK Refresh.msi](https://www.microsoft.com/en-sg/download/details.aspx?id=6135)**

8. Download and install **Windows Mobile 6.5.3 Professional DTK**

9. Download and install **MC3000c50b PSDK** i.e. ```30XXC50BxxPS010403.exe``` from **[Zebra Support Central](http://support.symbol.com)**

10. Download and install **EMDK for C v2.7** i.e. ```EMDK-C-020701.exe``` from **[Zebra Support Central](http://support.symbol.com)**

11. Download and install **[Zebra SDK](https://www.zebra.com/us/en/products/software/barcode-printers/zebralink/link-os-sdk.html)**. 
       
 ***Note:*** The **Zebra SDK** is used by **PrintingService**. If the **Zebra SDK** is not installed, the respective **PrintingService** cab will not be generated. The current version used for **PrintingService** cab generation is **```v1.0.122```**. If the **Zebra SDK** was installed in ***```C:\Program Files\Zebra Technologies\link_os_sdk\```***, the version detail can be found at ***```C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122```*** location on the machine. Further detailed guidelines on the printing service is below under section **[Printing Service](https://github.com/rhomobile/rhodes/blob/master/doc/oss/WM_CE_Installation_And_Build_Guidelines.md#printing-service)**.

12. **Ruby Gem Installation**:
    * Open command prompt and type ```gem list``` to display gems currently installed on the system. 
    * Install any missing gems from the list below: 
        * At the command prompt, type **```gem install <gem_name>``` (i.e. ```rest_client```, ```rest-client```, ```listen```, etc.)** 
        * Set the Ruby environment variables prior to use of the ```gem``` command:
        - ```gem install rest_client```     <i>- Must be installed</i>
        - ```gem install listen --version 3.0.6```          <i>- Must be installed</i>
        - ```gem install templater```       <i>- Must be installed</i>
        - ```gem install uuid```            <i>- Must be installed</i>
        - ```gem install net-scp```         <i>- Must be installed</i>
        - ```gem install CFPropertyList```  <i>- Install only if required and not present</i>
        - ```gem install rubyzip``` <i>- Install only if required and not present</i>
        - ```gem install rhomobile-debug``` <i>- Install only if required and not present</i>
        - ```gem install rhodes-translator``` <i>- Must be installed if ***rhodes_translator*** extension is added in ```build.yml``` file</i>
        - ```gem install rake```            <i>- Install only if required and not present, this is actually present by default</i>
        - ```gem install systemu```         <i>- Present by default; install only if required and not present</i>
        - ```gem install json```            <i>- Present by default; install only if required and not present</i>
        - ```gem install celluloid```       <i>- Present by default; install only if required and not present. Note: Internally, this installs **hitimes** and **timers** **gems** as well.</i>

13. **Environment variables** - Append the variables below to the System **`Path`** environment variable: 
**Note: This step is _required_ for proper operation**. 

* C:\project\rhodes\bin;
* C:\Ruby193\bin;

**Important**: Settings should reflect the installation paths for their respective tools. The example is based on the ***```rhodes```*** repository cloned inside the ***```C:\project\rhodes```*** directory and ***```Ruby193```*** being installed in the ***```C:\Ruby193\```*** directory. 

#**WM/CE Build Guidelines:**
1. Clone **```rhodes```** repository to the local machine.

2. Copy **```thirdpartyWebkit.zip```** and **```unzip_thirdpartyWebkit.bat```** files to the **rhodes** directory. After copying, run **```unzip_thirdpartyWebkit.bat```** to extract the content in the **```thirdpartyWebkit.zip```** file. The content will be extracted inside the **rhodes** directory.

 **Note:**: These files will never be exposed as per the terms and conditions made between **Zebra Technologies** and **Ekioh**. Check further details below under section **[Thirdparty Webkit Dependency For WM/CE Platform](https://github.com/rhomobile/rhodes/blob/master/doc/oss/WM_CE_Installation_And_Build_Guidelines.md#third-party-webkit-dependency-for-wmce)**. 

3. After step 2, run **```build_wmbinaries.bat```** from command prompt. It will build the source code for webkit, 2.2 plugins, npapi and printing service, placing them in the **```rhodes\libs\```** directory. Refer the **Printing Service** guidelines below for detailed information if there are any build errors related to the printing service.

 During execution of **```build_wmbinaries.bat```**, user will be prompted to verify the location of ```Include\Armv4i``` for **Windows Mobile 6 SDK** and **Windows Mobile 6.5.3 DTK**. Follow the prompts at the command line to proceed.

 **Note:** Internally, **```build_wmbinaries.bat```** makes a call to **```ms_include.bat```** batch file, which copies the dependent include files required in **```rhodes```**. The dependent include files are copied either from **Windows Mobile 6 SDK** or **Windows Mobile 6.5.3 DTK**. Learn more [about dependent files](https://github.com/rhomobile/rhodes/blob/master/doc/oss/WM_CE_Dependent_Include_Files.md).

4. Create **rhobuild.yml** if it does not exist:
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

5. Modify **build.yml** of **RhoMobile Application**, which has been choosen for building. Point the ```sdk``` path to the cloned **rhodes** directory location. For example: ```sdk: ../rhodes```

6. From the command prompt, navigate to **RhoMobile Application** project directory and type ```rake clean:wm```. This will clean the earlier build binaries, if any.

7. After cleaning, type ```rake device:wm:production``` for production build of **RhoMobile Application**.

8. A successful build will generate the cab files of **RhoMobile Application** for **WM/CE** platform.

##Third Party Webkit Dependency For WM/CE

**Windows Mobile/CE** framework APIs will be open-sourced without the **Webkit** component. 

User-managed **Webkit** options:

* Provide/develop their own **Webkit** replacement 
* Engage with a partner that is developing **Webkit** options, such as **[Tau Technologies](http://tau-technologies.com/)** or **[Kutir](http://kutirtech.com/)**

[Learn more about RMS open sourcing](https://developer.zebra.com/community/rhomobile-suite/rhomobile-community/rhomobile-blogs/blog/2016/03/25/announcement-rhomobile-transitioning-to-open-source?et=watches.email.blog) and licensing options.

As of now, a build on **Windows Mobile/CE** cannot progress unless the webkit dependency files are placed outside the **```rhodes```** directory with the open source version of rhodes. The files required after cloning **```rhodes```** locally into the machine are listed below. These files will never be exposed as per the terms and conditions made between **Zebra Technologies** and **Ekioh**. 

* **```thirdpartyWebkit.zip```** - contains additional dependent files required for building the **RhoMobile app** on WM/CE

* **```unzip_thirdpartyWebkit.bat```** - batch file to extract the contents of **thirdpartyWebkit.zip** inside the **```rhodes```** directory

#Printing Service
The **[Zebra SDK](https://www.zebra.com/us/en/products/software/barcode-printers/zebralink/link-os-sdk.html)** is used for generating the **PrintingService** cab file. 

#### Windows Mobile/CE Notes

The Printing Service application **PrintingService.cab** must be installed in **Windows Mobile/CE** device. This application should always be installed and running in **Windows Mobile/CE** device in order to use the RhoMobile Printing and PrintingZebra APIs from RhoMobile application.

 - Before installing the Printing Service application i.e. **PrintingService.cab** on **Windows Mobile** devices, one must need to install the **.NET compact framework** on your device. You may find the device installation package on your build machine at **```C:\Program Files (x86)\Microsoft.NET\SDK\CompactFramework\v3.5\WindowsCE\NETCFv35.wm.armv4i.cab```**
 
 - **Windows CE only** – You’ll also need to install the messaging framework found on your build machine at **```C:\Program Files (x86)\Microsoft.NET\SDK\CompactFramework\v3.5\WindowsCE\Diagnostics\NETCFv35.Messages.EN.cab on Windows 7```**.

#### Printing Service Build Notes

If the **Zebra SDK** is not installed, the respective **PrintingService** cab file will not be generated. The current version used for **PrintingService** cab generation is **```v1.0.122```**. 

 - If the **Zebra SDK** was installed in **```C:\Program Files\Zebra Technologies\link_os_sdk\```**, the version detail can be found at ***```C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122```*** location on the machine.
 
 - If the **Zebra SDK** is installed in a location other than **```v1.0.122```**, the user must apply the appropriate version detail and location in **PrintingService** project settings  available under **```rhodes\lib\commonAPI\printing_zebra\ext\platform\wm\PrintingService\PrintingService```**.

- **```PrintingService.inf```** - This file will be available under **```rhodes\lib\commonAPI\printing_zebra\ext\platform\wm\PrintingService\PrintingService\installer\```** directory. Open the file and edit the version and location information appropriately. This needs to be done only if the installed version is **NOT ```v1.0.122```**.
    
     Example: Inside **PrintingService.inf**, under ```[SourceDisksNames]``` as indicated below must be modified.
     ```
     [SourceDisksNames]
     1=,"",,"..\bin\Release"
     2=,"",,"C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122\lib"
     ...
     2=,"",,"C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\<VERSION_NUMBER>\lib"
     ```
     <i>Here **v1.0.122 (VERSION_NUMBER)** must be replaced with the appropriate version detail that can be fetched after installing **Zebra SDK** from the installed location.</i>

    -  **```PrintingService.csproj```** - This file will be available under **```rhodes\lib\commonAPI\printing_zebra\ext\platform\wm\PrintingService\PrintingService\```** directory. The dependent library of Zebra SDK which are referenced in **```PrintingService.csproj```** are mentioned below. Open the file and edit the version and location information appropriately. This need to be done only if the installed version is other than **```v1.0.122```** . 
    
     - ZSDK_API.dll
     - ZebraBluetoothAdapter.dll 
     - ZebraPlatformUtil.dll

     Example: Inside **PrintingService.csproj**, details below need to be modified.
     
     ```
     <Reference Include="ZebraBluetoothAdapter, Version=1.0.122.0, Culture=neutral, processorArchitecture=MSIL">
            <SpecificVersion>False</SpecificVersion>
            <HintPath>C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122\lib\ZebraBluetoothAdapter.dll</HintPath>
    </Reference>
    <Reference Include="ZebraPlatformUtil, Version=1.0.122.0, Culture=neutral, processorArchitecture=MSIL">
            <SpecificVersion>False</SpecificVersion>
            <HintPath>C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122\lib\ZebraPlatformUtil.dll</HintPath>
    </Reference>
    <Reference Include="ZSDK_API, Version=1.0.122.0, Culture=neutral, processorArchitecture=MSIL">
            <SpecificVersion>False</SpecificVersion>
            <HintPath>C:\Program Files\Zebra Technologies\link_os_sdk\windows_mobile_ce\v1.0.122\lib\ZSDK_API.dll</HintPath>
    </Reference>
     ```
     
#####**Generating Printing Service Cab for Windows Mobile/CE**
1. Clone **rhodes** repository to your local machine.
2. Copy **thirdpartyWebkit.zip** and **unzip_thirdpartyWebkit.bat** file inside **rhodes** directory.
3. Run **unzip_thirdpartyWebkit.bat** to extract the contents of **thirdpartyWebkit.zip**. The content will be extracted inside the **rhodes** directory.
4. After Step 3, open a command prompt, change the directory to ```rhodes``` and generate the printing service cab file using the command:

        rake wm_gem:build:printing_service --trace 
  
For example, if ```rhodes``` is cloned or present in ```C:\project``` location, then type the following from the command prompt:

   - ```cd /D C:\project\rhodes``` - <i>**Hit enter**</i>
   - ```rake wm_gem:build:printing_service --trace``` - <i>**Hit enter**</i>

After a successful build, the **PrintingService.cab** file will be generated inside **printing-service** folder located at **```C:\<path to rhodes>\libs\printing-service\PrintingService.cab```**.
