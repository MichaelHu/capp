echo off

set EXE_FILE_NAME=myencrypt

rem make sure that the operation only be applied 
rem to the directory in which the build.bat file is located
set BUILD_DIR=%~dp0%
pushd %BUILD_DIR%
cd "%BUILD_DIR%"

:main
call:prepare_env
call:prepare_lib
call:clear
call:build
call:clear
popd
rem pause
goto :eof

:prepare_lib
rem copy ..\..\release\logger.dll .
copy ..\..\release\logger.lib .
goto :eof

:prepare_env
call ..\..\cpp_compiler_env.bat
goto :eof

:build
echo [ start to compile ... ]
rem compiling
rem cl /nologo /Gz /EHs /EHc /MTd /c /Fe..\..\release\%EXE_FILE_NAME%.exe *.cpp
rem cl /nologo /Gd /EHs /EHc /MTd /TP /c *.cpp
rem cl /nologo /Gd /EHs /EHc /LDd /Zi /TP /c *.cpp
rem cl /nologo /Gd /EHs /MTd /EHc /Zi /TP /c *.cpp
g++ -g -c *.cpp
rem cl /nologo /Gd /EHs /MTd /EHc /TP /Zi /c *.cpp
echo [ compile successfully ... ]

echo [ start to link ... ]
rem ʹ��/DEFAULTLIB�����ڳ�����ʹ��#pragma comment(lib, "logger")����ʹ��/IMPLIB����[ȷʵ���У�������/DEFAULTLIB]��Ҳ����ֱ��ƴ��������β��
rem link /nologo /subsystem:console /DEFAULTLIB:logger /OUT:..\..\release\%EXE_FILE_NAME%.exe *.obj 

rem link /nologo /subsystem:console /OUT:..\..\release\%EXE_FILE_NAME%.exe *.obj logger.lib
g++ -o m.exe *.o logger.lib
echo [ link successfully ... ] 

goto :eof


:clear
echo.
echo [ start to clean files temporarily generated ... ]
rem for /r %%i in (*) do (
rem 	if "%%~xi" == ".obj" (
rem 		echo removing file: [ %%~fi ]
rem 		del /f "%%~fi"
rem 	)
rem )
del /f /s /q *.o
del /f /s /q *.obj
del /f /s /q *.s
goto :eof

