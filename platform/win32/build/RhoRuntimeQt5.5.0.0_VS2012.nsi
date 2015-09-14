;======================================================
; Include

  !include "StrFunc.nsh"
  !include "MUI.nsh"
  !include "LogicLib.nsh"
  !include "EnvVarUpdate.nsh" #download http://nsis.sourceforge.net/mediawiki/images/a/ad/EnvVarUpdate.7z
  !define WELCOME_TITLE "Welcome RhoRuntime Components for QT 5.5.0.0 for Microsoft Visual Studio 2012 Environment"
;======================================================
; Installer Information
  Name RhoRuntimeQt5.5.0.0_VS2012
  OutFile "RhoRuntimeQt5.5.0.0_VS2012-Setup.exe"
  InstallDir C:\RhoRuntimeQt5.5.0.0_VS2012
  BrandingText "Zebra"
  Caption "RhoRuntimeQt5.5.0.0 for Visual Studio 2012"
  ShowInstDetails show
  VIProductVersion "5.5.0.0"
  VIAddVersionKey  "ProductName" "RhoRuntimeQt5.5.0.0 for Visual Studio 2012"
  VIAddVersionKey  "Comments" "RhoRuntimeQt5.5.0.0 Visual Studio 2012 for RhoMobile Windows Applications"
  VIAddVersionKey  "CompanyName" "Zebra Technologies"
  VIAddVersionKey  "FileDescription" "RhoRuntimeQt5.5.0.0 components for Visual Studio 2012"
  VIAddVersionKey  "FileVersion" "5.5.0.0"
  VIAddVersionKey  "ProductVersion" "5.5.0.0"


;======================================================
; Modern Interface Configuration
 
  !define MUI_ICON RhoRuntimeQt5.ico
  !define MUI_UNICON RhoRuntimeQt5.ico
  !define MUI_HEADERIMAGE
  !define MUI_ABORTWARNING
  !define MUI_COMPONENTSPAGE_SMALLDESC
  !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
  #!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\README.html
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_TITLE_3LINES
  !define MUI_FINISHPAGE_TEXT "Thank you for installing RhoRuntimeQt5.5.0.0 for Visual Studio 2012 \r\n\n\n"
  
;======================================================
; Pages
  !define MUI_WELCOMEPAGE_TITLE '${WELCOME_TITLE}'
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !insertmacro MUI_PAGE_WELCOME
  #!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
  #!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  #Page custom customerConfig
  !define MUI_FINISHPAGE_TITLE '${MUI_FINISHPAGE_TEXT}'
  !insertmacro MUI_PAGE_FINISH
 
;======================================================
; Languages
 
  !insertmacro MUI_LANGUAGE "English"
 
;======================================================
; Reserve Files 

;======================================================
; Sections

RequestExecutionLevel admin #NOTE: You still need to check user rights with UserInfo!

# start default section
section
    SetShellVarContext all

    # set the installation directory as the destination for the following actions
    setOutPath $INSTDIR
 
    # create the uninstaller
    writeUninstaller "$INSTDIR\uninstall.exe"
 
    SetOutPath "$SMPROGRAMS\RhoMobileSuite"
    # create shortcuts
    createShortCut "$SMPROGRAMS\RhoMobileSuite\Uninstall RhoRuntimeQt5.5.0.0 for Visual Studio 2012.lnk" "$INSTDIR\uninstall.exe"

    # added information in 'unistall programs' in contorol panel
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "DisplayName" "RhoRuntimeQt5.5.0.0 for Visual Studio 2012"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "DisplayVersion" "5.5.0.0"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "DisplayIcon" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "Publisher" "Zebra Technologies"
    WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "NoModify" 1
    WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "NoRepair" 1
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012" \
                 "Comments" "RhoRuntimeQt5.5.0.0 for Visual Studio 2012"              

    ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"
    ${EnvVarUpdate} $0 "QT_PLUGIN_PATH" "A" "HKLM" "$INSTDIR" 

sectionEnd
 
# uninstaller section start
section "uninstall"
    SetShellVarContext all

    # confirmation dialog
    MessageBox MB_YESNO|MB_ICONQUESTION "Do you want to uninstall RhoRuntimeQt5.5.0.0 for Visual Studio 2012?" IDNO NoUninstallLabel

    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu    
    delete "$SMPROGRAMS\RhoMobileSuite\Uninstall RhoRuntimeQt5.5.0.0 for Visual Studio 2012.lnk"
    RMDir "$SMPROGRAMS\RhoMobileSuite"

    # remove information in 'unistall programs' in contorol panel
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5.5.0.0_VS2012"

    ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
    ${un.EnvVarUpdate} $0 "QT_PLUGIN_PATH" "R" "HKLM" "$INSTDIR" 

    # remove $INSTDIR
    RMDir /r /REBOOTOK $INSTDIR

    NoUninstallLabel:

# uninstaller section end
sectionEnd


Section "RhoRuntimeQt5.5.0.0_VS2012" appSection
  SetOutPath $INSTDIR

  File D3Dcompiler_46.dll
  File icudt54.dll
  File icuin54.dll
  File icuuc54.dll
  File msvcp110.dll
  File msvcr110.dll
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Multimedia.dll
  File Qt5MultimediaWidgets.dll
  File Qt5Network.dll
  File Qt5OpenGL.dll
  File Qt5Positioning.dll
  File Qt5PrintSupport.dll
  File Qt5Qml.dll
  File Qt5Quick.dll
  File Qt5Sensors.dll
  File Qt5Sql.dll
  File Qt5WebChannel.dll
  File Qt5WebKit.dll
  File Qt5WebKitWidgets.dll
  File Qt5Widgets.dll
  File vccorlib110.dll
  File vcomp110.dll
  File /r "imageformats"
  File /r "platforms"
  File "RhoRuntimeQt5.ico"

SectionEnd

;======================================================
;Descriptions
 
  ;Language strings
  LangString DESC_InstallApp ${LANG_ENGLISH} "This installs RhoRuntimeQt5.5.0.0 for Visual Studio 2012"
  
  ;Assign language strings to sections
  
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${appSection} $(DESC_InstallApp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;======================================================
;Functions
