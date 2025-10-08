REM Batch file to erase unneeded and possibly large files if using Visual Studio

REM *****Note:  it would be a BAD IDEA to take this outside this folder or the folder
REM  where you keep your own C++ code -- it may delete something you want!  Be careful! ****

REM  Apr 2025


REM Get rid of folders we don't need, possibly containing large files
for /r . %%d in (Debug,Release,.vs,Backup) do @if exist "%%d" rd /s/q "%%d"
REM Don't add x64 to this -- you'll delete part of the SDL library

del /s *.o      REM Delete Unix-created files
del /s a.out
del /s a.exe
del /s core
del /s *.dep

del /s *.pdb    REM and some other VS-created ones









