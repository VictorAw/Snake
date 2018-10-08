@echo off

cl ^
-EHsc ^
-I include ^
-D CONSOLE ^
src\*.cpp ^
src\windows\*.cpp ^
src\windows\console\*.cpp ^
-link ^
user32.lib kernel32.lib gdi32.lib ^
-out:Snake.exe

if not exist bin (
  md bin
)

if not exist build (
  md build
)

move /y *.obj build
move /y *.ilk build
move /y *.idb build
move /y *.pdb build
move /y *.xml build
move /y Snake.exe bin

