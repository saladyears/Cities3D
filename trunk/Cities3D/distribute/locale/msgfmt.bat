echo off
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/Cities3D.mo -D%1 Cities3D.po
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/CitiesOnlineRules.mo -D%1 CitiesOnlineRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/CitiesRules.mo -D%1 CitiesRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/CoreRules.mo -D%1 CoreRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/NetworkRules.mo -D%1 NetworkRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/SeafarersRules.mo -D%1 SeafarersRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/StandardRules.mo -D%1 StandardRules.po 
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/TexEditor.mo -D%1 TexEditor.po
"../../../gettext/0.13.1/bin/msgfmt.exe" -o%1/VolcanoRules.mo -D%1 VolcanoRules.po