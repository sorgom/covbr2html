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

set exe=%buildDir%\covbr2html.exe
set todoTxt=%buildDir%\todo_%~n0.txt

rem set code folder to base
rem activate macro coverage
set covcopt=--srcdir %srcDir% --macro
rem set individual coverage file
set covfile=%buildDir%\%~n0.cov

if exist %covfile% del %covfile%
if exist %todoTxt% del %todoTxt%

cd %makeDir%

echo - gen
rem generate project files
rem exit unless premake5 is available
premake5 %vsVersion%
if %errorlevel% neq 0 exit /b %errorlevel%

echo - build
rem clean solution
rem exit unless msbuild is available
msbuild covbr2html.sln -t:clean >NUL
if %errorlevel% neq 0 exit /b %errorlevel%

rem activate coverage instrumentation
rem exit unless cov01 is available
cov01 -q1
if %errorlevel% neq 0 exit /b %errorlevel%

rem build the project
rem turn of coverage instrumentation
msbuild covbr2html.sln >NUL
set elevel=%errorlevel%
cov01 -q0
rem exit if msbuild failed
if %elevel% neq 0 exit /b %elevel%

echo - run
:run
rem run executable to generate coverage data
%exe% -h >NUL
%exe% -c %myDir%\todo_*.txt >NUL

rem apply exclusions
covselect -qd --import %myDir%\sample_BullseyeCoverageExclusions

echo - report
cd %srcDir%
rem generate coverage report for source files
covsrc -q --srcdir .
echo - todo html
rem generate covbr report
covbr -qu --srcdir . > %todoTxt%
rem generate html report using covbr2html
%exe% -c %todoTxt%

