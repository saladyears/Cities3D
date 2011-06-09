@echo off
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/Cities3D.po Cities3D.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/CitiesOnlineRules.po CitiesOnlineRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/CitiesRules.po CitiesRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/CoreRules.po CoreRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/NetworkRules.po NetworkRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/SeafarersRules.po SeafarersRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/StandardRules.po StandardRules.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/TexEditor.po TexEditor.po
"../../../gettext/0.13.1/bin/msgmerge.exe" -U --backup off %1/VolcanoRules.po VolcanoRules.po