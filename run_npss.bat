@echo off
set NPSS_CONFIG=nt
set NPSS_TOP=C:\NPSS.nt.ver271_VC10_64\
set NPSS_DEV_TOP=%NPSS_TOP%\DLMdevkit
set NPSS_TEST_TOP=%NPSS_TOP%\Test
set VBS_HOME=%NPSS_TOP%\VBS
set MICODIR=%NPSS_TOP%
%NPSS_TOP%\bin\npss.nt.exe -I . -I .\ -I WATEinfo/ -I viewers\WATEformatting_SMJ/ -I %NPSS_TOP%\DLMComponents\nt -I %NPSS_TOP%\InterpIncludes -I %NPSS_TOP%\MetaData -I %NPSS_TOP%\MetaData\nt -I %NPSS_TOP%\DLMdevkit\WATE++ -I %NPSS_TOP%\DLMdevkit\WATE++\test\WATEwrapper -I %NPSS_TOP%\DLMdevkit\WATE++\include -I %NPSS_TOP%\DLMdevkit\Executive\include -I %NPSS_TOP%\InterpComponents %*

