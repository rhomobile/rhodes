;======================================================
; Include
 
  !include "MUI.nsh"
  !include "LogicLib.nsh"
 
;======================================================
; Installer Information
 
  Name %APPNAME%
  OutFile "%OUTPUTFILE%"
  InstallDir %APPINSTALLDIR%
  BrandingText " "

;======================================================
; Modern Interface Configuration
 
  !define MUI_ICON %APPICON%
  !define MUI_UNICON %APPICON%
  !define MUI_HEADERIMAGE
  !define MUI_ABORTWARNING
  !define MUI_COMPONENTSPAGE_SMALLDESC
  !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
  %README_PRESENT%!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\README.html
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_TEXT %FINISHPAGE_TEXT%
  
;======================================================
; Pages
 
  !insertmacro MUI_PAGE_WELCOME
  %LICENSE_PRESENT%!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
  !insertmacro MUI_PAGE_COMPONENTS
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
 
    SetOutPath "$SMPROGRAMS\%GROUP_NAME%"

    # create shortcuts
    createShortCut "$SMPROGRAMS\%GROUP_NAME%\%APPNAME%.lnk" "$INSTDIR\%APP_EXECUTABLE%"
    createShortCut "$SMPROGRAMS\%GROUP_NAME%\Uninstall %APPNAME%.lnk" "$INSTDIR\uninstall.exe"
    createShortCut "$DESKTOP\%APPNAME%.lnk" "$INSTDIR\%APP_EXECUTABLE%" "" "$INSTDIR\icon.ico" 0

    # added information in 'unistall programs' in contorol panel
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "DisplayName" "%APPNAME% %APPVERSION%"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "DisplayIcon" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "Publisher" "%GROUP_NAME%"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%" \
                 "NoRepair" 1

    # update installed rhodes applications catalogue
    WriteRegStr HKLM "Software\Rhomobile\RhoGallery\%APPNAME%" "InstallDir" "$INSTDIR"
    WriteRegStr HKLM "Software\Rhomobile\RhoGallery\%APPNAME%" "Executable" "$INSTDIR\%APP_EXECUTABLE%"
    WriteRegStr HKLM "Software\Rhomobile\RhoGallery\%APPNAME%" "Uninstaller" "$INSTDIR\uninstall.exe"
    WriteRegStr HKLM "Software\Rhomobile\RhoGallery\%APPNAME%" "DisplayName" "%APPNAME% %APPVERSION%"

sectionEnd
 
# uninstaller section start
section "uninstall"
    SetShellVarContext all

    # confirmation dialog
    MessageBox MB_YESNO|MB_ICONQUESTION "Do you want to uninstall %APPNAME%?" IDNO NoUninstallLabel

    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu    
    delete "$SMPROGRAMS\%GROUP_NAME%\%APPNAME%.lnk"
    delete "$SMPROGRAMS\%GROUP_NAME%\Uninstall %APPNAME%.lnk"
    delete "$DESKTOP\%APPNAME%.lnk"
    RMDir "$SMPROGRAMS\%GROUP_NAME%"

    # remove information in 'unistall programs' in contorol panel
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\%APPNAME%"

    # remove app from installed rhodes applications catalogue
    DeleteRegKey HKLM "Software\Rhomobile\RhoGallery\%APPNAME%"

    # remove $INSTDIR
    RMDir /r /REBOOTOK $INSTDIR

    NoUninstallLabel:

# uninstaller section end
sectionEnd


Section %SECTION_NAME% appSection
  SetOutPath $INSTDIR
 
  %LICENSE_FILE%
  %README_FILE%
  File /r "rho"
  File %APP_EXECUTABLE%
  File *.dll
  File *.manifest
  File /r "imageformats"
  File "icon.ico"

SectionEnd

;======================================================
;Descriptions
 
  ;Language strings
  LangString DESC_InstallApp ${LANG_ENGLISH} %SECTOIN_TITLE%
  
  ;Assign language strings to sections
  
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${appSection} $(DESC_InstallApp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;======================================================
;Functions
