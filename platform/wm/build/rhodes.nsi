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
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\README.html
  !define MUI_FINISHPAGE
  !define MUI_FINISHPAGE_TEXT %FINISHPAGE_TEXT%
  
;======================================================
; Pages
 
  !insertmacro MUI_PAGE_WELCOME
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

# start default section
section
    # set the installation directory as the destination for the following actions
    setOutPath $INSTDIR
 
    # create the uninstaller
    writeUninstaller "$INSTDIR\uninstall.exe"
 
    # create a shortcut named "new shortcut" in the start menu programs directory
    # point the new shortcut at the program uninstaller
    createShortCut %SCAPPPATH% "$INSTDIR\%APP_EXECUTABLE%"
    createShortCut %SCUNISTALLPATH% "$INSTDIR\uninstall.exe"

    # added information in 'unistall programs' in contorol panel
    #WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoStudio" \
    #             "DisplayName" "RhoStudio - RAD tool for develop and debug rhodes/rhoconnect applications"
    #WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoStudio" \
    #             "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    #WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RhoStudio" \
    #             "DisplayIcon" "$\"$INSTDIR\uninstall.exe$\""
    
sectionEnd
 
# uninstaller section start
section "uninstall"
    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"
 
    # second, remove the link from the start menu    
    delete %SCUNISTALLPATH%
    delete %SCAPPPATH%

    # remove $INSTDIR
    RMDir /r /REBOOTOK $INSTDIR

# uninstaller section end
sectionEnd


Section %SECTION_NAME% appSection
  SetOutPath $INSTDIR
 
  File /r "rho"
  File %APP_EXECUTABLE%
  File *.dll
  File *.manifest
  File /r "imageformats"

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
