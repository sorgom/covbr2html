@echo off
rem sample script using msbuild
rem requires premake5, msbuild, bullseye coverage tools
SETLOCAL

rem set vsVersion for premake5
rem see premake5 --help for available options
set vsVersion=%1
if "%vsVersion%"=="" set vsVersion=vs2019

cd %~dp0
set myDir=%CD%
cd ..
set buildDir=%CD%\build
set srcDir=%CD%\code
set makeDir=%CD%\make
set reportsDir=%CD%\sample_reports

set exe=%buildDir%\covbr2html.exe
set todoTxt=%buildDir%\todo_%~n0.txt
set buildReport=%buildDir%\build.log

rem set code folder to base
rem activate macro coverage
set covcopt=--srcdir %srcDir% --macro
rem set individual coverage file
set covfile=%buildDir%\%~n0.cov

del /Q %covfile% %todoTxt% %reportsDir%\*.* >NUL 2>&1

cd %makeDir%

echo - premake5
rem generate project files
rem requires premake5
premake5 %vsVersion%
if %errorlevel% neq 0 exit /b %errorlevel%

echo - msbuild clean
rem clean solution
rem requires msbuild
msbuild covbr2html.sln -t:clean >NUL
if %errorlevel% neq 0 exit /b %errorlevel%

echo - bullseye on
rem activate coverage instrumentation
rem requires bullseye coverage tools
cov01 -q1
if %errorlevel% neq 0 exit /b %errorlevel%

echo - msbuild solution
rem build solution
rem turn off coverage instrumentation
msbuild covbr2html.sln > %buildReport%
set elevel=%errorlevel%
cov01 -q0
rem exit if msbuild failed
if %elevel% neq 0 (
    type %buildReport%
    exit /b %elevel%
)

echo - run
rem run executable to generate coverage data
%exe% -h >NUL
%exe% -cwo %buildDir% %myDir%\covbr_*.txt >NUL
del /Q %buildDir%\*.html %buildDir%\*.txt 2>NUL

rem apply exclusions
covselect -qd --import %myDir%\sample_BullseyeCoverageExclusions

for %%n in (%reportsDir%) do echo - report to %%~nn folder
cd %srcDir%
rem generate coverage report for source files
covsrc -q --srcdir .
echo - todo html
rem generate covbr report
covbr -qu --srcdir . > %todoTxt%
rem generate html report using covbr2html
%exe% -cwo %reportsDir% %todoTxt%
