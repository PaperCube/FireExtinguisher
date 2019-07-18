@echo on
git pull -f
if %ERRORLEVEL% == 0 goto success
else goto fail
:success
echo Great.
goto quitnopause
:fail
echo Failed
:quit
pause
:quitnopause