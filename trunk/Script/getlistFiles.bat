@echo off
if EXIST %1 goto iftrue
goto iffalse
:iftrue
set target=%1
set file=%2
goto run
:iffalse
set target=.
set file=list.txt
:run
dir /B %target% > %file%
