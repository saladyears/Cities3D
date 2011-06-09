@echo off

cd ..\..\src\%1
dir /b "*.h" > %1h.txt
dir /b "*.cpp" > %1cpp.txt

if %1 == %2 goto same

"../../../gettext/0.13.1/bin/xgettext.exe" -k_ -j -f%1h.txt -o../../distribute/locale/%2.po
"../../../gettext/0.13.1/bin/xgettext.exe" -k_ -j -f%1cpp.txt -o../../distribute/locale/%2.po
goto end

:same
"../../../gettext/0.13.1/bin/xgettext.exe" -k_ -f%1h.txt -o../../distribute/locale/%2.po
"../../../gettext/0.13.1/bin/xgettext.exe" -k_ -j -f%1cpp.txt -o../../distribute/locale/%2.po

:end
del %1h.txt
del %1cpp.txt
cd ..\..\distribute\locale