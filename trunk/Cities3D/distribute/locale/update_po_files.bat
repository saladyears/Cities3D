@echo off
REM First, create .po files from the source.  Then, update all the .po files in
REM each subdirectory.  Finally, remove all the temporary .po files.

CALL xgettext.bat
FOR /D %%d IN (*.*) DO CALL msgmerge %%d
del *.po