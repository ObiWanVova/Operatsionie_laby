@echo off
set /p var=name:
echo name is %var%
FOR %%a IN (*.txt) DO MD %var%\%%a 
FOR %%a IN (*.txt) DO move /y %%a %var%\%%a 
pause