goto :platform_%RHO_PLATFORM%||exit !ERRORLEVEL!

:platform_android
cd rhoelementsext\platform\android||exit !ERRORLEVEL!
call rake --trace||exit !ERRORLEVEL!
goto :eof

:platform_iphone
cd rhoelementsext\platform\phone||exit !ERRORLEVEL!
call rake --trace||exit !ERRORLEVEL!
goto :eof

:platform_wm
:platform_win32
cd rhoelementsext\platform\wm||exit !ERRORLEVEL!
call rake --trace||exit !ERRORLEVEL!
goto :eof

:platform_bb
cd rhoelementsext\platform\bb||exit !ERRORLEVEL!
call rake --trace||exit !ERRORLEVEL!
goto :eof
