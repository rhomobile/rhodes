del /f \public_html\guess.zip
del /f \public_html\guess-src.zip
zip \public_html\guess.zip Guess.*
cd ..\..
zip -r \public_html\guess-src.zip Guess
