@echo off
rem sample script using msbuild
rem requires premake5, msbuild, bullseye coverage tools
SETLOCAL

cd %~dp0
cd ..
set repo=%CD%
set buildDir=%CD%\build
set srcDir=%CD%\code
set exe=%buildDir%\covbr2html.exe
cd %~dp0

set covcopt=--srcdir %srcDir% --macro
set covfile=%buildDir%\sample.cov

if exist %covfile% del %covfile%

rem set vsVersion for premake5
rem see premake5 --help for available options
set vsVersion=%1
if "%vsVersion%"=="" set vsVersion=vs2019

rem generate project files
rem exit unless premake5 is available
call premake5 %vsVersion%
if %errorlevel% neq 0 exit /b %errorlevel%

call msbuild covbr2html.sln -t:clean

rem activate coverage instrumentation
rem exit unless cov01 is available
call cov01 -q1
if %errorlevel% neq 0 exit /b %errorlevel%

rem build the project
rem exit unless msbuild is available
call msbuild covbr2html.sln
if %errorlevel% neq 0 exit /b %errorlevel%
call cov01 -q0

:run
rem run executable to generate coverage data
call %exe% >NUL
call %exe% -cw >NUL

rem apply excusions
call covselect -qd
call covselect -q --import sample_BullseyeCoverageExclusions

cd %srcDir%
rem generate coverage report for source files
call covsrc -q --srcdir .
rem generate covbr report
call covbr -qu --srcdir . > %buildDir%\todo.txt
rem generate html report using covbr2html
call %exe% -c %buildDir%\todo.txt

