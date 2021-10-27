[![Build Status](https://travis-ci.org/rhomobile/rhodes.svg?branch=master)](https://travis-ci.org/rhomobile/rhodes)

[![Build status](https://ci.appveyor.com/api/projects/status/4c1uqndnfau9c8hq/branch/master?svg=true)](https://ci.appveyor.com/project/alex-epifanoff/rhodes/branch/master)

# Current state of Rhomobile solution

Currently Rhomobile solution supported and improved by TAU Technologies Inc (partner of Zebra Technologies).
For questions on commercial support contact [contact mail](mailto:info@tau-platform.com) or reach out to the [TAU Technologies website](http://tau-platform.com).

Current actual version is Rhomobile Suite 7.0. [Download RMS 7.0](http://tau-platform.com/developers/downloads/)

Actual documentation located on [RMS 7.0 documentation](http://docs.tau-platform.com/en/7.0/home)


# RhoMobile Open Source Documentation

This page contains links to documentation and source-code repositories for the RhoMobile Suite, which has been released by Zebra Technologies to the open source community.

For access to prior documentation, please visit **[Zebra's RhoMobile documentation site](http://docs.rhomobile.com/en/5.4/guide/welcome)**.


## Getting RhoMobile source code

[Getting Source Code](https://github.com/rhomobile/rhodes/blob/master/doc/oss/getting_source_code.md) - How to use git to download all the source code that Zebra is making available.

## RhoMobile versions

RhoMobile has a mainstream development branch which is forked for stable releases and further patches for them.
Every major release is named by MAJOR.MINOR numbers with separate branch created for it ( for instance 6-1-stable ).
After stable version is released patches are applied to it with increased PATCH number as MAJOR.MINOR.PATCH.
Patches may include bug fixes or back porting of some features ( new platforms support etc ). New functional features are usually not included to patches for previous releases.
Version naming is mostly based on Semantic Versioning v2 spec ( https://semver.org/spec/v2.0.0.html ) with some differences.

Current development builds versions always have increased MINOR number compared to already released stable version. Only latest development build usually should be actual for developers who want to use all latest features. For other users most recent stable release with applied patches is recommended.

Development builds does not have to end up in a stable branch. Instead, minor version number may be increased after certain amount of features is accumulated.

Development, Alpha and Beta releases have corresponding indication after PATCH value.

Example of RhoMobile release tree ( just for reference, it may not correspond to actual releases ):

```
6.1.0
  |  \
  |   6.1.1
  |   |
  |   6.1.2 - Previous supported stable/patched release. May lack some latest features.
  |
7.0.0.dev1 - obsolete development build. Not recommended for use.
  |
7.0.0.dev2 - obsolete development build. Not recommended for use.
  |
7.0.0.alphaX - alpha releases for 7.0. Not recommended for use.
  |
7.0.0.betaX  - beta releases for 7.0. Not recommended for use.
  |
7.0.0 - initial 7.0 stable release
  | \
  |  7.0.1 - Latest stable/patched release. Recommended for most users.
  |
7.1.0.devX - Latest development release. Not recommended for production but contains all recent features.
```

## Installation

[Windows First-time Install (32- and 64-bit)](http://docs.tau-platform.com/en/7.0/guide/rhomobile-install#windows-first-time-install-32--and-64-bit) - How to install RhoMobile Suite on Windows (32- and 64-bit).

[Mac OS X First-time Install](http://docs.tau-platform.com/en/7.0/guide/rhomobile-install#mac-os-x-first-time-install) - How to install RhoMobile Suite on Mac OS.

## Building apps

[Build for Android](http://docs.tau-platform.com/en/7.0/guide/build_android) - How to build apps for Android.

[Build for iOS](http://docs.tau-platform.com/en/7.0/guide/build_ios) - How to build apps for iOS.

[Building for Windows](http://docs.tau-platform.com/en/7.0/guide/build_win) - How to build apps for Windows.

[Building for Windows ME/CE](http://docs.tau-platform.com/en/7.0/guide/build_wm) - How to build apps for Windows Mobile.

[Building for Windows 10(UWP)](http://docs.tau-platform.com/en/7.0/guide/build_uwp) - How to build apps for Windows 10(UWP).


## RhoConnect Push Synchronization
[Introduction to Push](http://docs.tau-platform.com/en/7.0/rhoconnect/push)

[Set up Google Cloud Messaging on client and server for Rhodes Android Device](http://docs.tau-platform.com/en/7.0/rhoconnect/push-client-setup-android) - Setting Up RhoConnect Push-Based Synchronization with Google Cloud Messaging.

[Set up Apple Push Notification Service on client and server for Rhodes iOS Device](http://docs.tau-platform.com/en/7.0/rhoconnect/push-client-setup-ios) - Setting Up RhoConnect Push-Based Synchronization with Apple Push Notification Service.

[Set up RhoConnect Push Service on client and server for Rhodes Android and Windows Mobile/CE Devices](http://docs.tau-platform.com/en/7.0/rhoconnect/push-client-setup-rps) - Setting Up for RhoConnect Push Service on Client Application for Windows Mobile/CE Devices.


## Windows Mobile/CE Docs

[Dependent Include Files](https://github.com/rhomobile/rhodes/blob/master/doc/oss/WM_CE_Dependent_Include_Files.md) - Critical files that must be present for successfully targeting Windows Mobile/CE devices, and where and how to get them.


## Android Docs

[Guidelines To Support Barcode API On Android JellyBean Devices](https://github.com/rhomobile/rhodes/blob/master/doc/oss/Barcode_support_doc.md) - Lists the Zebra Android devices capable of barcode scanning, their OS image versions and steps for activating the feature.


## Repositories Released as Open Source

Clone these repo(s) as required for your needs to generate a copy of the open source code to the local development machine.

[Rhodes](https://github.com/rhomobile/rhodes/)

[RhoConnect](https://github.com/rhomobile/rhoconnect/)

[RhoConnect Client](https://github.com/rhomobile/rhoconnect-client/)

[RhoConnect Push](https://github.com/rhomobile/rhoconnect-push)

[RhoConnect Push Service](https://github.com/rhomobile/rhoconnect-push-service)

[RhoStudio](https://github.com/rhomobile/rhostudio/)

[RhoInstaller](https://github.com/rhomobile/rhoinstaller/)

[RMS-Testing](https://github.com/rhomobile/RMS-Testing)
