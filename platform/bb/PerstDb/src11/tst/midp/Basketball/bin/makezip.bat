rd /s/q ..\tmpclasses
rd /s/q ..\tmplib
del /f \public_html\uar-src.zip
del /f  \public_html\uar.zip
zip \public_html\uar.zip Basketball.*
cd ..\..
zip -r \public_html\uar-src.zip Basketball
