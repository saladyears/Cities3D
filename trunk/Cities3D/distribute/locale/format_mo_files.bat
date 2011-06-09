@echo off
REM Call each of the subdirectories and convert their .po files to .mo files.
FOR /D %%d IN (*.*) DO CALL msgfmt %%d