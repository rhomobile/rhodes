# Building on Mac OS X

RhoMobile depends on Java, Ruby, Git and other tools. Depending on the development host platform, these components are either installed automatically with the main suite or must be installed manually. 

## Java Development Kit

[Download the Java for Mac Installer](http://www.java.com/en/download/index.jsp) from Java.com and install Java 6 or higher.

Note: Java is required if targeting Android.

## Setup for Xcode, Command Line Tools and Homebrew

If the latest Xcode and command line tools aren't already installed: 

1. [Visit Apple's Xcode page](https://developer.apple.com/xcode/) and download and install Xcode. 

2. Install the command line tools: 
	- Launch Xcode 
	- Go to –> Preferences –> Downloads (tab)
	- Select **install the "Command Line Tools"**
	- Follow all prompts to complete installation

3. Install the [Homebrew](http://mxcl.github.com/homebrew/) package managemet tool into `/usr/local`.

## Set up Ruby
Much of the key functionality of the RhoMobile platform is delivered as Gems, which are platform- and processor-specific apps and libraries written in Ruby. RhoStudio requires Ruby v1.9.3 and is not compatible with any other version, incuding the newer version (2.x) that comes with current versions of Mac OS X. Fortunately, RVM makes it easy to switch between Ruby versions if needed.

If Ruby Version Manager (RVM) and Ruby 1.9.3 are not installed:

1. Install [Ruby Version Manager](https://rvm.io/) (RVM) into your home directory. 

**Note**: Do not use `sudo` when you install RVM.

2. Install Ruby 1.9.3 with the following RVM command: 

	$ rvm install 1.9.3

It's a good idea to create gemsets, which makes it easy to switch between rhodes setups. 

3. To create a gemset: 

	$ rvm gemset create rhodes_source

4. To use the gemset that you have created: 

	$ rvm use 1.9.3@rhodes_source --default

## Get Git 

Git for Mac is available as a command line tool and with a GUI. You will need one or the other. 

* For the command-line tool, [visit the git-osx repository](http://code.google.com/p/git-osx-installer/downloads/list?can=3) and download and install the latest. 

	-- or -- 

* [Visit GitHub.com](https://desktop.github.com) and download GitHub Desktop for the GUI. 

# Developing Without RhoStudio
Although installing RhoMobile Suite provides everything needed to build RhoMobile applications through the included RhoStudio IDE, some people might prefer to use a favorite IDE to build RhoMobile applications. This guide describes how to setup the environment as well as how to use the Rhomobile command line options.

## What are Gems?
A RubyGem is a software package. Commonly called a “gem," they contain a packaged Ruby applications or libraries that provide functionality. The RubyGems software itself makes it easy to download, install and manipulate gems on a system.

For more information on Ruby Gems, visit [Rubygems.org](http://guides.rubygems.org/what-is-a-gem/)

## Prerequisite gems for rhodes

In Terminal, execute the following commands to install the required gems on the machine: 

	$ gem install templater
	$ gem install rake
	$ gem install uuid
	$ gem install systemu
	$ gem install json
	$ gem install net-scp
	$ gem install rest-client
	$ gem install CFPropertyList
	$ gem install listen --version 3.0.6
	$ gem install rubyzip
	$ gem install rhodes-translator
	$ gem install rhomobile-debug

## Setting Up Rhodes Source Code

Rhodes source code is available on github.com, and you can use git to get it. Visit the [git-scm page](http://git-scm.com) and download that tool, or use git or [GitHub Desktop](https://desktop.github.com) to clone the rhodes code.

To clone Rhodes to your local computer, execute git clone from the command line:
	
	:::term
	$ git clone git://github.com/rhomobile/rhodes.git
	$ cd rhodes

Checkout the branch you are interested in (if not the master branch):

	:::term
	$ git checkout -f -b <branchname> --track origin/<branchname>

To keep your branch up to date, use the git pull command:

	:::term
	$ git pull

Add the bin folder of this clone to your path. On Linux/Mac it is typically done in the ```.profile``` file. On Windows, it is done using the Control Panel.

Configure your environment by running the rhodes-setup script. This will attempt to auto-detect installed SDKs. It will prompt you to verify the SDK paths or enter paths for SDKs that cannot be detected.

	:::term
	$ rhodes-setup

## Generating A Rhodes Application
The first step is to generate the application and base files. First we will generate an application called StoreManager with the following command:

	:::term
	$ rhodes app storemanager http://localhost:9292/application

This will generate an application directory called "storemanager" with several files. Note also that the third argument above is the URL to the RhoConnect server data source (required if you want to do synchronized data as we describe in the next section). This will set the rhoconfig.txt file to have the following option:

	:::txt
	syncserver = 'http://localhost:9292/application'

If you do not need synchronized offline data, you can leave the third argument out.

The default home screen of the app is defined in the ```storemanager/app/index.erb``` file. You can edit the HTML there to create what your app displays. You can link to other HTML files and ERB files from there with standard HTML techniques.

## Generating Models
You can also use the command line to add models and controllers. When the model is generated, Rhodes also will create files for a standard UI for displaying and editing the model. This follows the standard model-view-controller paradigm. 

Let's generate a model called "product" and give it some attributes.

	:::term
	$ cd storemanager
	$ rhodes model product brand,name,price,quantity,sku
	Generating with model generator:
		[ADDED] app/Product/index.erb
		[ADDED] app/Product/edit.erb
		[ADDED] app/Product/new.erb
		[ADDED] app/Product/show.erb
		[ADDED] app/Product/product_controller.rb
		[ADDED] app/Product/product.rb
		[ADDED] app/test/product_spec.rb

## Setting Up build.yml

By default, a Rhodes application uses the latest Rhodes gem installed on your computer. If you wish to use special version of Rhodes or use Rhodes git sources, check for this line in your application's ```build.yml``` file:

	:::term
	sdk: /Library/Ruby/Gems/1.8/gems/rhodes-3.0.2

Or for Rhodes sources:

	:::term
	sdk: ~/Projects/rhodes

# Setup for iOS
To build an iOS application locally, it must be performed on a Mac computer or by using [RhoHub](http://rhohub.com) online build service from any laptop/desktop. The instructions below describe how to do this from either the Mac Xcode development environment or from the command line on your local Mac.

### Prerequisites
To build for iPhone/iPad, you will need the following software installed:

* Mac OS X 10.6 or greater.
* XCode 4.2 or greater
* XCode Command Line Tools (already included in versions of XCode before 4.3) should be installed manually from XCode 4.3 - see Note below.
* iPhone SDK. We recommend installing the latest XCode with the latest iOS SDK (the latest XCode is already included with the iOS SDK). The latest stable version is available from the [iPhone Dev Center](http://developer.apple.com/iphone/index.action).

> Note: In XCode 4.3, command line tools are not included in the installation. You should install the command line tools from XCode by using the Components tab of the Downloads preferences panel. See [New Features in Xcode 4.3](https://developer.apple.com/library/ios/#documentation/DeveloperTools/Conceptual/WhatsNewXcode/Articles/xcode_4_3.html)

### SDK locations
The iOS SDK that is specified in your ```build.yml``` will need to be in the default XCode folder or the build process will not be able to find it.

Example:

    :::yml
    iphone:
        SDK: iphoneos7.0
or

    :::yml
    iphone:
        SDK: iphonesimulator7.0

With these SDKs specified, you need to make sure the SDKs are in the right folder. To find your default XCode installation directory, you can run the command `xcode-select --print-path` in your terminal. This will return something like this:

    :::term
    $ xcode-select --print-path
    /Applications/Xcode.app/Contents/Developer

This is the path that the build process will use when looking for your iOS SDKs. If you have an SDK that you will be using but it's in another folder, you'll need to move it to the appropriate folder in this directory. For example, if I have an iphoneos6.0 SDK that I want to use but it's in my downloads folder, I'll need to move it `/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/`.

The iphoneos and iphonesimulator SDKs go in two different folders:

* `/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs` - iPhoneOS SDKs, for running apps on your iOS device.
* `/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs` - iPhoneSimulator SDKs, for running apps in your iOS simulator.

> Note: These paths are solely for your information; they do not need to be copied into any files to be used. These are simply the directories in which you need to store your SDKs for them to work with the RhoMobile build process.

## Building the Application for the iOS Device

After you have modified and debugged your RhoMobile application, you can prepare your application to install onto your iOS device.

### Modifying the build.yml

First, in your application's ```build.yml``` file, change sdk from "iphonesimulator" to "iphoneos" and configuration from "Debug" to "Release," and add information such as "codesignidentity" as below:

  iphone:
    sdk: iphoneos4.3
    configuration: Release
    codesignidentity: "iPhone Developer"
    entitlements: ""

### Creating Your Application Package

Now you can create your .ipa file with this rake command:

    :::term
    $ rake device:iphone:production

# Setup for Android
## Setting Up the Android SDK and NDK

1. Visit the [Android Studio page](http://developer.android.com/sdk/requirements.html), scroll down to the System Requirements and verify that your machine meets those of the Android SDK. 

2) Download the [Android SDK](http://developer.android.com/sdk/index.html), and unzip it to the location of your choice.

> Note: A minimum Android API level 10 must be installed.

On Macintosh and Linux, navigate to and run `<Android SDK>/tools/android sdk`. On Windows, navigate to and run <Android SDK>/SDK Manager.exe. The Android SDK Manager window appears. Then perform the following steps:

 * Select Settings, then check "`Force https://... sources to be fetched using http://"`.
 * Run Android SDK Manager, check latest Android SDK Platform(s) and Google APIs, then click the "Install packages" button to download and install them.

> Note: To use Google Map View you need to install latest Google API from Android SDK Manager

2) Download and unzip the [latest Android NDK](http://developer.android.com/sdk/ndk/index.html) to the location of your choice.

> Note: You should update the Android SDK and NDK as you update to new versions of Rhodes. Android NDK version r8e is required as a minimum. 

**Android NDK r9 is the current recommendation**. 

Download Android NDK r9 from Google's servers:

* [for Linux](http://dl.google.com/android/ndk/android-ndk-r9-linux-x86.tar.bz2)

* [for Mac OS X](https://dl.google.com/android/ndk/android-ndk-r9d-darwin-x86_64.tar.bz2)

* [for Windows](http://dl.google.com/android/ndk/android-ndk-r9d-windows-x86_64.zip)

### Creating Your Application Package

Now you can create your .apk file with rake commands:

    :::term
    $ rake device:android:production

# Printing Zebra Support on IOS

To use printing_zebra apis/extension in your application, you need to perform the steps below. 

Zebra printer support requires a link SDK library. To obtain it you should go to the [LINK-OS MULTIPLATFORM SDK](http://www.zebra.com/gb/en/products-services/software/link-os/link-os-sdk.html#mainpartabscontainer_794f=downloads) site, get installer for mac os x and then run it.

After installation, the library will be placed into ```/Users/<_user\_name_>/Applications/link\_os\_sdk/```. You should navigate to ```link\_os\_sdk/iOS/<_sdk-ver_>/``` and copy ```lib/``` folder content to ```rhodes/lib/commonAPI/printing_zebra/ext/platform/iphone/lib/```. 

# Extension Build Guidelines for BARCODE in Android
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

#Extension Building Guidelines for PRINTING in Android

1. Download and install the **[Printer Zebra SDK for Android](https://www.zebra.com/us/en/products/software/barcode-printers/zebralink/link-os-sdk.html)**

2. Copy all the JARs from the local machine directory to the directory ```.\rhodes\lib\commonAPI\printing_zebra\ext\platform\android\ZEBRA_SDK```

3. If ```svg-android-1.1.jar``` and different versions of ```httpmime-4.1.1.jar and commons-net-3.1.jar``` are there in the printing directory (same or different), they must be replaced with latest version of the JARs in the location ```rhoelementsext/ext/rhoelementsext/platform/android/rhoelements_temp/libs``` directory when both 2.2 APIS and printing_zebra extension are getting built. As both extension require the same JAR, it will create build issue if different versions are included. Hence, it is recommended to replace any duplicate JAR file from rhoelements extension directory.

#Extension Building Guidelines for 2.2 APIs

1. To build 2.2 APIs, include the rhoelements extension or app_type="rhoelements".

2. MSR, scanner and MPM plug-ins CANNOT be built from open source.

#Extension Build Guidelines for CardReader

The Cardreader extension build is NOT supported on OSS. If using this API, the only option is to migrate to DataWedge.
