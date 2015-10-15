;======================================================
; Include

  !include "StrFunc.nsh"
  !include "MUI.nsh"
  !include "LogicLib.nsh"
  !include "EnvVarUpdate.nsh" #download http://nsis.sourceforge.net/mediawiki/images/a/ad/EnvVarUpdate.7z
 
;======================================================
; Installer Information
 
  Name RhoRuntimeQt5-VS2008
  OutFile "RhoRuntimeQt5-VS2008Setup.exe"
  InstallDir C:\RhoRuntimeQt5-VS2008
  BrandingText " "

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
  !define MUI_FINISHPAGE_TEXT "Thank you for installing RhoRuntimeQt5-VS2008 \r\n\n\n"
  
;======================================================
; Pages
 
  !insertmacro MUI_PAGE_WELCOME
  #!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
  #!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  #Page custom customerConfig
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
 
    SetOutPath "$SMPROGRAMS\rhomobile"
    # create shortcuts
    createShortCut "$SMPROGRAMS\rhomobile\Uninstall RhoRuntimeQt5-VS2008.lnk" "$INSTDIR\uninstall.exe"

    # added information in 'unistall programs' in contorol panel
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "DisplayName" "RhoRuntimeQt5-VS2008"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "DisplayVersion" "5.1.1"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "DisplayIcon" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "Publisher" "Zebra Technologies"
    WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "NoModify" 1
    WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008" \
                 "NoRepair" 1

    ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"
	${EnvVarUpdate} $0 "QT_PLUGIN_PATH" "A" "HKLM" "$INSTDIR" 

sectionEnd
 
# uninstaller section start
section "uninstall"
    SetShellVarContext all

    # confirmation dialog
    MessageBox MB_YESNO|MB_ICONQUESTION "Do you want to uninstall RhoRuntimeQt5-VS2008?" IDNO NoUninstallLabel

    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu    
    delete "$SMPROGRAMS\rhomobile\Uninstall RhoRuntimeQt5-VS2008.lnk"
    RMDir "$SMPROGRAMS\rhomobile"

    # remove information in 'unistall programs' in contorol panel
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoRuntimeQt5-VS2008"

    ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
	${un.EnvVarUpdate} $0 "QT_PLUGIN_PATH" "R" "HKLM" "$INSTDIR" 

    # remove $INSTDIR
    RMDir /r /REBOOTOK $INSTDIR

    NoUninstallLabel:

# uninstaller section end
sectionEnd


Section "RhoRuntimeQt5-VS2008" appSection
  SetOutPath $INSTDIR


  File icudt53.dll
  File icuin53.dll
  File icuuc53.dll
  File libEGL.dll
  File libGLESv2.dll
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Multimedia.dll
  File Qt5MultimediaWidgets.dll
  File Qt5Network.dll
  File Qt5OpenGL.dll
  File Qt5PrintSupport.dll
  File Qt5Qml.dll
  File Qt5Quick.dll
  File Qt5Sensors.dll
  File Qt5Sql.dll
  File Qt5V8.dll
  File Qt5WebKit.dll
  File Qt5WebKitWidgets.dll
  File Qt5Widgets.dll

  File /r "imageformats"
  File /r "platforms"
  File "RhoRuntimeQt5.ico"

SectionEnd

;======================================================
;Descriptions
 
  ;Language strings
  LangString DESC_InstallApp ${LANG_ENGLISH} "This installs RhoRuntimeQt5-VS2008"
  
  ;Assign language strings to sections
  
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${appSection} $(DESC_InstallApp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;======================================================
;Functions
