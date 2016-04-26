#RhoSimulator Build Prerequisites For Mac OS

RhoSimulator requires: 
* **Mac OS X** 10.9 or lower
* **Xcode** 6.2
* **[Qt 5.4.2](https://download.qt.io/official_releases/qt/5.4/5.4.2/qt-opensource-mac-x64-clang-5.4.2.dmg)** installed in the following way: 

 	- Locate and launch the downloaded file **```qt-opensource-mac-x64-clang-5.4.2.dmg```**
 	- When prompted, install Qt inside **```/Users/user-name/build/shared/install/```**
 	- Set the **```QTDIR``` path** as **```/Users/user-name/build/shared/install/Qt5.4.2/5.4/clang_64```**

#Build RhoSimulator on Mac OS X
###Note: It is recommended to perform steps in this order: 

1. Create (and/or navigate to) a directory in the local machine suitable for cloning. 

2. **Clone the ```rhoconnect-client``` repository** to that directory.

3. **Clone the ```rhodes``` repository** to the same directory. 

4. **Create the file ```rhobuild.yml```** if it does not exist.
   
5. From command prompt, navigate to the cloned **```rhodes```** repository and build RhoSimulator by entering the following command: 

    - **```rake build:rhosimulator```**
     
6. On successful build, rhodes will generate a folder called **```RhoSimulator```** under **the ```rhodes/platform/osx/bin/```** directory.

Follow the **[Building on Mac OS X](https://github.com/rhomobile/rhodes/blob/master/doc/oss/building_on_mac.md)** guidelines.
