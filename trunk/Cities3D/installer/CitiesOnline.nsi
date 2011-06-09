; CitiesOnline.nsi
;
;--------------------------------

; The name of the installer
Name "Cities Online"

; The file to write
OutFile "Install_CitiesOnline.exe"

; Compression
SetCompressor /solid lzma

; The default installation directory
InstallDir "$PROGRAMFILES\Cities Online"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
; TODO must be done within a section or function
; SetShellVarContext all
InstallDirRegKey HKLM "Software\CitiesOnline" "Install_Dir"

; TODO check if the user has Admin rights.
; If so, ask the user if he wants to install for "Current User" or "All users"
; If not, only install for "Current User".
; Set the registry key root to HKLM or HKCU depending on choice


; = Style notes =

; == Adds XP manifest ==
XPStyle on

; == Modern UI ==

!include "MUI.nsh"

; Uncomment the following when the bitmaps (150x57)
; and icons are available
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "cities3d-header.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "cities3d-header.bmp"
!define MUI_ICON "cities3d-inst.ico"
!define MUI_UNICON "cities3d-uninst.ico"

!define MUI_COMPONENTSPAGE_CHECKBITMAP "cities3d-check.bmp"

; Components have no description for the moment
!define MUI_COMPONENTSPAGE_NODESC

!insertmacro MUI_LANGUAGE "English"

;--------------------------------

; Pages

!define MUI_PAGE_HEADER_TEXT "Components selection"
!define MUI_PAGE_HEADER_SUBTEXT "Please select the components that you want to install"
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_PAGE_HEADER_TEXT "Installation folder"
!define MUI_PAGE_HEADER_SUBTEXT "Please select the installation folder"
!insertmacro MUI_PAGE_DIRECTORY

Var STARTMENU_FOLDER
!define MUI_PAGE_HEADER_TEXT "Start Menu folder"
!define MUI_PAGE_HEADER_SUBTEXT "Please select the Start Menu folder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Cities Online"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\CitiesOnline"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenu"
!insertmacro MUI_PAGE_STARTMENU start_menu $STARTMENU_FOLDER

!define MUI_PAGE_HEADER_TEXT "Installing"
!define MUI_PAGE_HEADER_SUBTEXT "Installing the program now ..."
!insertmacro MUI_PAGE_INSTFILES

!define MUI_PAGE_HEADER_TEXT "Confirm"
!define MUI_PAGE_HEADER_SUBTEXT "Please confirm that you want to uninstall the program"
!insertmacro MUI_UNPAGE_CONFIRM

!define MUI_PAGE_HEADER_TEXT "Uninstalling"
!define MUI_PAGE_HEADER_SUBTEXT "Uninstalling the program now ..."
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------

; Some macros

; This define ensures that only tiles called Standard
; are set RO
!define Standard_is_default

; Install/remove tilesets

!macro add_tile dir tile
  Section "${tile}" tile_${dir}
    !ifdef ${dir}_is_default
      SectionIn RO
    !endif
    SetOutPath $INSTDIR\tex\${dir}
    File /nonfatal "..\distribute\tex\${dir}\glbarbarians.tex"
    File /nonfatal "..\distribute\tex\${dir}\glchips.tex"
    File /nonfatal "..\distribute\tex\${dir}\glcity.tex"
    File /nonfatal "..\distribute\tex\${dir}\glextra.tex"
    File /nonfatal "..\distribute\tex\${dir}\glknights.tex"
    File /nonfatal "..\distribute\tex\${dir}\glroad.tex"
    File /nonfatal "..\distribute\tex\${dir}\glseaports.tex"
    File /nonfatal "..\distribute\tex\${dir}\glsettlement.tex"
    File /nonfatal "..\distribute\tex\${dir}\glship.tex"
    File /nonfatal "..\distribute\tex\${dir}\gltilegold.tex"
    File /nonfatal "..\distribute\tex\${dir}\gltilehidden.tex"
    File /nonfatal "..\distribute\tex\${dir}\gltilejungle.tex"
    File /nonfatal "..\distribute\tex\${dir}\gltilestandard.tex"
    File /nonfatal "..\distribute\tex\${dir}\gltilevolcano.tex"
    File /nonfatal "..\distribute\tex\${dir}\glwalls.tex"
    File /nonfatal "..\distribute\tex\${dir}\uibuild.tex"
    File /nonfatal "..\distribute\tex\${dir}\uibuilddevcard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uibuildck.tex"
    File /nonfatal "..\distribute\tex\${dir}\uibuildship.tex"
    File /nonfatal "..\distribute\tex\${dir}\uicardsdev.tex"
    File /nonfatal "..\distribute\tex\${dir}\uicardspolitics.tex"
    File /nonfatal "..\distribute\tex\${dir}\uicardsscience.tex"
    File /nonfatal "..\distribute\tex\${dir}\uicardstrade.tex"
    File /nonfatal "..\distribute\tex\${dir}\uicomcard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uidevcard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uidice.tex"
    File /nonfatal "..\distribute\tex\${dir}\uidiceck.tex"
    File /nonfatal "..\distribute\tex\${dir}\uidicered.tex"
    File /nonfatal "..\distribute\tex\${dir}\uigoldcard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uiicons.tex"
    File /nonfatal "..\distribute\tex\${dir}\uiplayerck.tex"
    File /nonfatal "..\distribute\tex\${dir}\uipoints.tex"
    File /nonfatal "..\distribute\tex\${dir}\uipointsck.tex"
    File /nonfatal "..\distribute\tex\${dir}\uipointsdev.tex"
    File /nonfatal "..\distribute\tex\${dir}\uiprocard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uirescard.tex"
    File /nonfatal "..\distribute\tex\${dir}\uisplash.tex"
  SectionEnd
!macroend

!macro add_tile_small dir tile
  !insertmacro add_tile ${dir}_Small "${tile} (small)"
!macroend

!macro add_tile_both dir tile
  !insertmacro add_tile ${dir} ${tile}
  !insertmacro add_tile_small ${dir} ${tile}
!macroend

!macro rm_tile dir
  Delete $INSTDIR\tex\${dir}\glbarbarians.tex
  Delete $INSTDIR\tex\${dir}\glchips.tex
  Delete $INSTDIR\tex\${dir}\glcity.tex
  Delete $INSTDIR\tex\${dir}\glextra.tex
  Delete $INSTDIR\tex\${dir}\glknights.tex
  Delete $INSTDIR\tex\${dir}\glroad.tex
  Delete $INSTDIR\tex\${dir}\glseaports.tex
  Delete $INSTDIR\tex\${dir}\glsettlement.tex
  Delete $INSTDIR\tex\${dir}\glship.tex
  Delete $INSTDIR\tex\${dir}\gltilegold.tex
  Delete $INSTDIR\tex\${dir}\gltilehidden.tex
  Delete $INSTDIR\tex\${dir}\gltilejungle.tex
  Delete $INSTDIR\tex\${dir}\gltilestandard.tex
  Delete $INSTDIR\tex\${dir}\gltilevolcano.tex
  Delete $INSTDIR\tex\${dir}\glwalls.tex
  Delete $INSTDIR\tex\${dir}\uibuild.tex
  Delete $INSTDIR\tex\${dir}\uibuilddevcard.tex
  Delete $INSTDIR\tex\${dir}\uibuildck.tex
  Delete $INSTDIR\tex\${dir}\uibuildship.tex
  Delete $INSTDIR\tex\${dir}\uicardsdev.tex
  Delete $INSTDIR\tex\${dir}\uicardspolitics.tex
  Delete $INSTDIR\tex\${dir}\uicardsscience.tex
  Delete $INSTDIR\tex\${dir}\uicardstrade.tex
  Delete $INSTDIR\tex\${dir}\uicomcard.tex
  Delete $INSTDIR\tex\${dir}\uidevcard.tex
  Delete $INSTDIR\tex\${dir}\uidice.tex
  Delete $INSTDIR\tex\${dir}\uidiceck.tex
  Delete $INSTDIR\tex\${dir}\uidicered.tex
  Delete $INSTDIR\tex\${dir}\uigoldcard.tex
  Delete $INSTDIR\tex\${dir}\uiicons.tex
  Delete $INSTDIR\tex\${dir}\uiplayerck.tex
  Delete $INSTDIR\tex\${dir}\uipoints.tex
  Delete $INSTDIR\tex\${dir}\uipointsck.tex
  Delete $INSTDIR\tex\${dir}\uipointsdev.tex
  Delete $INSTDIR\tex\${dir}\uiprocard.tex
  Delete $INSTDIR\tex\${dir}\uirescard.tex
  Delete $INSTDIR\tex\${dir}\uisplash.tex
  RMDir  $INSTDIR\tex\${dir}
!macroend

; Install/remove sounds

!macro add_sound dir sound
  Section "${sound}" sound_${dir}
    !ifdef ${dir}_is_default
      SectionIn RO
    !endif
    SetOutPath $INSTDIR\sounds\${dir}
    File /nonfatal "..\distribute\sounds\${dir}\ActivateKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\AttackKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\BarbarianAdvance.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\BarbarianDefend.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\BarbarianPillage.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\BuyDevelopmentCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\ChargeKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\ChatMessageReceived.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\CityReduced.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\DestroyKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\DiscardProgressCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\EarnExtraPoints.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\EndTurn.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\ExpelCriminals.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\GiveWeddingGift.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\LargestArmy.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\LongestRoad.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\LoseCardsOn7.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\LoseGame.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\LowOnTime.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\MoveKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\MoveShip.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\NoSteal.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\OutOfTime.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PassiveQuit.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceCity.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceCityWalls.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceDeserterKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceKnight1.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceMerchant.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceMetropolis.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlacePirate.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceRoad.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceRobber.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceSettlement.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceSettlementOnPort.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaceShip.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayerJoin.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayerLeave.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayAlchemistCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayArsonistCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayBishopCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayCommercialHarborCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayCraneCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayDeserterCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayDiplomatCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayEngineerCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayIntrigueCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayInventorCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayIrrigationCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayMasterMerchantCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayMedicineCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayMerchantFleetCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayMiningCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayMonopolyCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayRoadBuildingCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaySaboteurCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaySoldierCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaySmithCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlaySpyCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayVPCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayWarlordCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayWeddingCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\PlayYearOfPlentyCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\ReceiveProgressCard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\RepairCity.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\RollDice.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\SelectDeserterKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\SelectDiplomatRoad.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\SelectIntrigueKnight.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\SelectSaboteurCity.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\StealACard.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\SwitchRollChips.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeAccept.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeCounter.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeExecute.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeOffer.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeRefuse.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeReject.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeSelect.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\TradeToBank.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UndoAction.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UpgradeKnight2.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UpgradeKnight3.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UpgradePolitics.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UpgradeScience.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UpgradeTrade.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\UseOneTurnTimer.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\VolcanicDestruction.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\VolcanicEruption.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\WinGame.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\YouNeedToDoSomething.mp3"
    File /nonfatal "..\distribute\sounds\${dir}\YourTurn.mp3"
    SectionEnd
!macroend

!macro rm_sound dir
  Delete $INSTDIR\sounds\${dir}\ActivateKnight.mp3
  Delete $INSTDIR\sounds\${dir}\AttackKnight.mp3
  Delete $INSTDIR\sounds\${dir}\BarbarianAdvance.mp3
  Delete $INSTDIR\sounds\${dir}\BarbarianDefend.mp3
  Delete $INSTDIR\sounds\${dir}\BarbarianPillage.mp3
  Delete $INSTDIR\sounds\${dir}\BuyDevelopmentCard.mp3
  Delete $INSTDIR\sounds\${dir}\ChargeKnight.mp3
  Delete $INSTDIR\sounds\${dir}\ChatMessageReceived.mp3
  Delete $INSTDIR\sounds\${dir}\CityReduced.mp3
  Delete $INSTDIR\sounds\${dir}\DiscardProgressCard.mp3
  Delete $INSTDIR\sounds\${dir}\DestroyKnight.mp3
  Delete $INSTDIR\sounds\${dir}\EarnExtraPoints.mp3
  Delete $INSTDIR\sounds\${dir}\EndTurn.mp3
  Delete $INSTDIR\sounds\${dir}\ExpelCriminals.mp3
  Delete $INSTDIR\sounds\${dir}\GiveWeddingGift.mp3
  Delete $INSTDIR\sounds\${dir}\LargestArmy.mp3
  Delete $INSTDIR\sounds\${dir}\LongestRoad.mp3
  Delete $INSTDIR\sounds\${dir}\LoseCardsOn7.mp3
  Delete $INSTDIR\sounds\${dir}\LoseGame.mp3
  Delete $INSTDIR\sounds\${dir}\LowOnTime.mp3
  Delete $INSTDIR\sounds\${dir}\MoveKnight.mp3
  Delete $INSTDIR\sounds\${dir}\MoveShip.mp3
  Delete $INSTDIR\sounds\${dir}\NoSteal.mp3
  Delete $INSTDIR\sounds\${dir}\OutOfTime.mp3
  Delete $INSTDIR\sounds\${dir}\PassiveQuit.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceCity.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceCityWalls.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceDeserterKnight.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceKnight1.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceMerchant.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceMetropolis.mp3
  Delete $INSTDIR\sounds\${dir}\PlacePirate.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceRoad.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceRobber.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceSettlement.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceSettlementOnPort.mp3
  Delete $INSTDIR\sounds\${dir}\PlaceShip.mp3
  Delete $INSTDIR\sounds\${dir}\PlayerJoin.mp3
  Delete $INSTDIR\sounds\${dir}\PlayerLeave.mp3
  Delete $INSTDIR\sounds\${dir}\PlayAlchemistCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayArsonistCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayBishopCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayCommercialHarborCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayCraneCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayDeserterCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayDiplomatCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayEngineerCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayIntrigueCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayInventorCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayIrrigationCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayMasterMerchantCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayMedicineCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayMerchantFleetCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayMiningCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayMonopolyCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayRoadBuildingCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlaySaboteurCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlaySoldierCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlaySmithCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlaySpyCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayVPCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayWarlordCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayWeddingCard.mp3
  Delete $INSTDIR\sounds\${dir}\PlayYearOfPlentyCard.mp3
  Delete $INSTDIR\sounds\${dir}\ReceiveProgressCard.mp3
  Delete $INSTDIR\sounds\${dir}\RepairCity.mp3
  Delete $INSTDIR\sounds\${dir}\RollDice.mp3
  Delete $INSTDIR\sounds\${dir}\SelectDeserterKnight.mp3
  Delete $INSTDIR\sounds\${dir}\SelectDiplomatRoad.mp3
  Delete $INSTDIR\sounds\${dir}\SelectIntrigueKnight.mp3
  Delete $INSTDIR\sounds\${dir}\SelectSaboteurCity.mp3
  Delete $INSTDIR\sounds\${dir}\StealACard.mp3
  Delete $INSTDIR\sounds\${dir}\SwitchRollChips.mp3
  Delete $INSTDIR\sounds\${dir}\TradeAccept.mp3
  Delete $INSTDIR\sounds\${dir}\TradeCounter.mp3
  Delete $INSTDIR\sounds\${dir}\TradeExecute.mp3
  Delete $INSTDIR\sounds\${dir}\TradeOffer.mp3
  Delete $INSTDIR\sounds\${dir}\TradeRefuse.mp3
  Delete $INSTDIR\sounds\${dir}\TradeReject.mp3
  Delete $INSTDIR\sounds\${dir}\TradeSelect.mp3
  Delete $INSTDIR\sounds\${dir}\TradeToBank.mp3
  Delete $INSTDIR\sounds\${dir}\UndoAction.mp3
  Delete $INSTDIR\sounds\${dir}\UpgradeKnight2.mp3
  Delete $INSTDIR\sounds\${dir}\UpgradeKnight3.mp3
  Delete $INSTDIR\sounds\${dir}\UpgradePolitics.mp3
  Delete $INSTDIR\sounds\${dir}\UpgradeScience.mp3
  Delete $INSTDIR\sounds\${dir}\UpgradeTrade.mp3
  Delete $INSTDIR\sounds\${dir}\UseOneTurnTimer.mp3
  Delete $INSTDIR\sounds\${dir}\VolcanicDestruction.mp3
  Delete $INSTDIR\sounds\${dir}\VolcanicEruption.mp3
  Delete $INSTDIR\sounds\${dir}\WinGame.mp3
  Delete $INSTDIR\sounds\${dir}\YouNeedToDoSomething.mp3
  Delete $INSTDIR\sounds\${dir}\YourTurn.mp3
  RMDir $INSTDIR\sounds\${dir}
!macroend

!macro rm_tile_small dir
  !insertmacro rm_tile ${dir}_Small
!macroend

!macro rm_tile_both dir
  !insertmacro rm_tile ${dir}
  !insertmacro rm_tile_small ${dir}
!macroend

; Install/remove skins

!macro add_skin dir skin
  Section "${skin}" skin_${dir}
    !ifdef ${dir}_is_default
      SectionIn RO
    !endif
    SetOutPath $INSTDIR\skins\${dir}
    File "..\distribute\skins\${dir}\uibuttons.tex"
    File "..\distribute\skins\${dir}\uiicons.tex"
    File "..\distribute\skins\${dir}\Cities3D.xml"
  SectionEnd
!macroend

!macro rm_skin dir
  Delete $INSTDIR\skins\${dir}\uibuttons.tex
  Delete $INSTDIR\skins\${dir}\uiicons.tex
  Delete $INSTDIR\skins\${dir}\Cities3D.xml
  RMDir  $INSTDIR\skins\${dir}
!macroend

; Install/remove languages

!macro add_lang dir lang nat
  Section /o "${lang} (${nat})" lang_${dir}
    SetOutPath $INSTDIR\locale\${dir}
    File "..\distribute\locale\${dir}\Cities3D.mo"
    File "..\distribute\locale\${dir}\CitiesOnlineRules.mo"
    File "..\distribute\locale\${dir}\CitiesRules.mo"
    File "..\distribute\locale\${dir}\CoreRules.mo"
    File "..\distribute\locale\${dir}\NetworkRules.mo"
    File "..\distribute\locale\${dir}\SeafarersRules.mo"
    File "..\distribute\locale\${dir}\StandardRules.mo"
    File "..\distribute\locale\${dir}\TexEditor.mo"
    File "..\distribute\locale\${dir}\VolcanoRules.mo"
  SectionEnd
!macroend

!macro rm_lang dir
  Delete $INSTDIR\locale\${dir}\Cities3D.mo
  Delete $INSTDIR\locale\${dir}\CitiesOnlineRules.mo
  Delete $INSTDIR\locale\${dir}\CitiesRules.mo
  Delete $INSTDIR\locale\${dir}\CoreRules.mo
  Delete $INSTDIR\locale\${dir}\NetworkRules.mo
  Delete $INSTDIR\locale\${dir}\SeafarersRules.mo
  Delete $INSTDIR\locale\${dir}\StandardRules.mo
  Delete $INSTDIR\locale\${dir}\TexEditor.mo
  Delete $INSTDIR\locale\${dir}\VolcanoRules.mo
  RMDir  $INSTDIR\locale\${dir}
!macroend

;--------------------------------

; The stuff to install


Section "Cities Online (required)"
  SectionIn RO

  ; Binary files
  SetOutPath $INSTDIR
  File "..\distribute\Cities3D.exe"
  File "..\distribute\CitiesOnline.exe"
  File "..\distribute\cryptopp.dll"
  File "..\distribute\DatabaseCore.dll"
  File "..\distribute\fmod.dll"
  File "..\distribute\GLCore.dll"
  File "..\distribute\ModelCore.dll"
  File "..\distribute\RulesCore.dll"
  File "..\distribute\UICore.dll"
  File "..\distribute\wxbase28u_net_vc_custom.dll"
  File "..\distribute\wxbase28u_vc_custom.dll"
  File "..\distribute\wxbase28u_xml_vc_custom.dll"
  File "..\distribute\wxmsw28u_adv_vc_custom.dll"
  File "..\distribute\wxmsw28u_aui_vc_custom.dll"
  File "..\distribute\wxmsw28u_core_vc_custom.dll"
  File "..\distribute\wxmsw28u_gl_vc_custom.dll"
  File "..\distribute\wxmsw28u_html_vc_custom.dll"

  ; Plugin files
  SetOutPath $INSTDIR\plugins
  File "..\distribute\plugins\CitiesOnlineRules.dll"
  File "..\distribute\plugins\CitiesRules.dll"
  File "..\distribute\plugins\CoreRules.dll"
  File "..\distribute\plugins\NetworkRules.dll"
  File "..\distribute\plugins\SeafarersRules.dll"
  File "..\distribute\plugins\StandardRules.dll"
  File "..\distribute\plugins\TexEditor.dll"
  File "..\distribute\plugins\VolcanoRules.dll"

  ; Data files
  SetOutPath $INSTDIR\data
  File "..\distribute\data\maps.db"
  File "..\distribute\data\mapkey.db"
  
  ; Option files
  SetOutPath $INSTDIR\options
  File "..\distribute\options\C&K_Basic_Game.xml"
  File "..\distribute\options\C&K_European_Deck.xml"
  File "..\distribute\options\C&K_Saboteur_Deck.xml"
  File "..\distribute\options\Standard_Basic_Game.xml"

  SetOutPath $INSTDIR

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\CitiesOnline "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CitiesOnline" "DisplayName" "CitiesOnline"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CitiesOnline" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CitiesOnline" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CitiesOnline" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  ; Write the Cities Online protocol
  WriteRegStr HKCR "CitiesOnline" "" "URL:CitiesOnline Protocol"
  WriteRegStr HKCR "CitiesOnline" "URL Protocol" ""
  WriteRegStr HKCR "CitiesOnline\DefaultIcon" "" '"$INSTDIR\CitiesOnline.exe"'
  WriteRegStr HKCR "CitiesOnline\shell\open\command" "" '"$INSTDIR\CitiesOnline.exe" "%1"'

  !insertmacro MUI_STARTMENU_WRITE_BEGIN start_menu
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Cities Online.lnk" "$INSTDIR\Cities3D.exe" "" "$INSTDIR\Cities3D.exe" 0 "" "" "Run the Cities Online client by itself"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall Cities Online.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0 "" "" "Uninstall Cities Online"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

SectionGroup "Tilesets"
  !insertmacro add_tile_both Standard Standard
  !insertmacro add_tile_both Impressionist Impressionist
SectionGroupEnd

SectionGroup "Sounds"
  !insertmacro add_sound Standard Standard
SectionGroupEnd

SectionGroup "Skins"
  !insertmacro add_skin Standard Standard
  !insertmacro add_skin Brushed_Metal "Brushed Metal"
SectionGroupEnd

SectionGroup /e "Languages"
  !insertmacro add_lang ca_ES Catalan Spain
  !insertmacro add_lang cs_CZ Czech "Czech Republic"
  !insertmacro add_lang da_DK Danish Denmark
  !insertmacro add_lang de_AT German Austria
  !insertmacro add_lang el_GR Greek Greece
  !insertmacro add_lang en_US English USA
  !insertmacro add_lang es_CO Spanish Colombia
  !insertmacro add_lang fi_FI Finnish Finland
  !insertmacro add_lang fr_FR French France
  !insertmacro add_lang hu_HU Hungarian Hungary
  !insertmacro add_lang is_IS Icelandic Iceland
  !insertmacro add_lang it_IT Italian Italy
  !insertmacro add_lang ja_JP Japanese Japan
  !insertmacro add_lang ko_KR Korean Korea
  !insertmacro add_lang nl_BE Dutch Belgium
  !insertmacro add_lang nl_NL Dutch Netherlands
  !insertmacro add_lang pl_PL Polish Poland
  !insertmacro add_lang pt_PT Portuguese Portugal
  !insertmacro add_lang ru_RU Russian Russia
  !insertmacro add_lang sv_SE Swedish Sweden
  !insertmacro add_lang tr_TR Turkish Turkey
SectionGroupEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Binary files
  Delete $INSTDIR\Cities3D.exe
  Delete $INSTDIR\CitiesOnline.exe
  Delete $INSTDIR\cryptopp.dll
  Delete $INSTDIR\DatabaseCore.dll
  Delete $INSTDIR\fmod.dll
  Delete $INSTDIR\GLCore.dll
  Delete $INSTDIR\ModelCore.dll
  Delete $INSTDIR\RulesCore.dll
  Delete $INSTDIR\UICore.dll
  Delete $INSTDIR\wxbase28u_net_vc_custom.dll
  Delete $INSTDIR\wxbase28u_vc_custom.dll
  Delete $INSTDIR\wxbase28u_xml_vc_custom.dll
  Delete $INSTDIR\wxmsw28u_adv_vc_custom.dll
  Delete $INSTDIR\wxmsw28u_aui_vc_custom.dll
  Delete $INSTDIR\wxmsw28u_core_vc_custom.dll
  Delete $INSTDIR\wxmsw28u_gl_vc_custom.dll
  Delete $INSTDIR\wxmsw28u_html_vc_custom.dll

  ; Plugin files
  Delete $INSTDIR\plugins\CitiesOnlineRules.dll
  Delete $INSTDIR\plugins\CitiesRules.dll
  Delete $INSTDIR\plugins\CoreRules.dll
  Delete $INSTDIR\plugins\NetworkRules.dll
  Delete $INSTDIR\plugins\SeafarersRules.dll
  Delete $INSTDIR\plugins\StandardRules.dll
  Delete $INSTDIR\plugins\TexEditor.dll
  Delete $INSTDIR\plugins\VolcanoRules.dll

  ; Data files
  Delete $INSTDIR\data\maps.db
  Delete $INSTDIR\data\mapkey.db

  ; Option files
  Delete "$INSTDIR\options\C&K_Basic_Game.xml"
  Delete "$INSTDIR\options\C&K_European_Deck.xml"
  Delete "$INSTDIR\options\C&K_Saboteur_Deck.xml"
  Delete "$INSTDIR\options\Standard_Basic_Game.xml"
  
  ; Logs
  Delete "$INSTDIR\logs\*.log"
  RMDir $INSTDIR\logs

  IfFileExists $INSTDIR\options\*.xml 0 rmoptdir
    ; Ask if they want to remove extra options they created.
    MessageBox MB_YESNO "Remove user-created option sets?" IDYES optionTrue IDNO optionFalse
    optionTrue:
      Delete "$INSTDIR\options\*.xml"
      Goto rmoptdir
    optionFalse:
      Goto optdone
  rmoptdir:
    RMDir "$INSTDIR\options"
  optdone:

  !insertmacro rm_tile_both Standard
  !insertmacro rm_tile_both Impressionist
  RMDir "$INSTDIR\tex"

  !insertmacro rm_sound Standard
  RMDir "$INSTDIR\sounds"

  !insertmacro rm_skin Standard
  !insertmacro rm_skin Brushed_Metal
  RMDir "$INSTDIR\skins"

  !insertmacro rm_lang ca_ES
  !insertmacro rm_lang cs_CZ
  !insertmacro rm_lang da_DK
  !insertmacro rm_lang de_AT
  !insertmacro rm_lang el_GR
  !insertmacro rm_lang en_US
  !insertmacro rm_lang es_CO
  !insertmacro rm_lang fi_FI
  !insertmacro rm_lang fr_FR
  !insertmacro rm_lang is_IS
  !insertmacro rm_lang it_IT
  !insertmacro rm_lang ja_JP
  !insertmacro rm_lang ko_KR
  !insertmacro rm_lang nl_BE
  !insertmacro rm_lang nl_NL
  !insertmacro rm_lang pl_PL
  !insertmacro rm_lang pt_PT
  !insertmacro rm_lang ru_RU
  !insertmacro rm_lang sv_SE
  !insertmacro rm_lang tr_TR
  RMDir "$INSTDIR\locale"


  ; Remove uninstaller
  Delete $INSTDIR\uninstall.exe

  ; Ask if they want to get rid of players and saved games.
  MessageBox MB_YESNO "Remove player and game databases?" IDYES true IDNO false
  true:
    Delete $INSTDIR\data\players.db
    Delete $INSTDIR\data\games.db
    Delete $INSTDIR\data\gamekey.db
    Delete $INSTDIR\data\replay.db
    Delete $INSTDIR\data\replaykey.db
  false:

  RMDir "$INSTDIR\data"

  ; Remove shortcuts, if any
  Push $R0
  !insertmacro MUI_STARTMENU_GETFOLDER start_menu $R0
  Delete  "$SMPROGRAMS\$R0\Cities Online.lnk"
  Delete  "$SMPROGRAMS\$R0\Uninstall Cities Online.lnk"
  RMDir   "$SMPROGRAMS\$R0"
  Pop $R0

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CitiesOnline"
  DeleteRegKey HKLM SOFTWARE\CitiesOnline
  DeleteRegKey HKCR CitiesOnline

  ; Remove last directories
  RMDir "$INSTDIR\plugins"

  RMDir "$INSTDIR"


SectionEnd

;--------------------------------

; Some functions

; Put here because .onInit needs to be after the sections

!include "Sections.nsh"

Var user_nat  ; User nation
Var lcid      ; LCID
Var lang_sec  ; section index

Function LCID2Lang
; This function converts a Windows LCID
; to a lang section index. Entries are sorted by
; lang_xx_XX code, not by Windows LCID
  StrCpy $lang_sec ${lang_en_US} ; Fall back
  StrCmp $lcid "0403" 0 +3
    StrCpy $lang_sec ${lang_ca_ES}
    Goto done
  StrCmp $lcid "0405" 0 +3
    StrCpy $lang_sec ${lang_cs_CZ}
    Goto done
  StrCmp $lcid "0406" 0 +3
    StrCpy $lang_sec ${lang_da_DK}
    Goto done
  StrCmp $lcid "0c07" 0 +3
    StrCpy $lang_sec ${lang_de_AT}
    Goto done
  StrCmp $lcid "0408" 0 +3
    StrCpy $lang_sec ${lang_el_GR}
    Goto done
; We skip this ...
;  StrCmp $lcid "0409" 0 +3
;    StrCpy $lang_sec ${lang_en_US}
;    Goto done
  StrCmp $lcid "240a" 0 +3
    StrCpy $lang_sec ${lang_es_CO}
    Goto done
  StrCmp $lcid "040b" 0 +3
    StrCpy $lang_sec ${lang_fi_FI}
    Goto done
  StrCmp $lcid "040c" 0 +3
    StrCpy $lang_sec ${lang_fr_FR}
    Goto done
  StrCmp $lcid "040e" 0 +3
    StrCpy $lang_sec ${lang_hu_HU}
    Goto done
  StrCmp $lcid "040f" 0 +3
    StrCpy $lang_sec ${lang_is_IS}
    Goto done
  StrCmp $lcid "0410" 0 +3
    StrCpy $lang_sec ${lang_it_IT}
    Goto done
  StrCmp $lcid "0411" 0 +3
    StrCpy $lang_sec ${lang_ja_JP}
    Goto done
  StrCmp $lcid "0412" 0 +3
    StrCpy $lang_sec ${lang_ko_KR}
    Goto done
  StrCmp $lcid "0813" 0 +3
    StrCpy $lang_sec ${lang_nl_BE}
    Goto done
  StrCmp $lcid "0413" 0 +3
    StrCpy $lang_sec ${lang_nl_NL}
    Goto done
  StrCmp $lcid "0415" 0 +3
    StrCpy $lang_sec ${lang_pl_PL}
    Goto done
  StrCmp $lcid "0816" 0 +3
    StrCpy $lang_sec ${lang_pt_PT}
    Goto done
  StrCmp $lcid "0419" 0 +3
    StrCpy $lang_sec ${lang_ru_RU}
    Goto done
  StrCmp $lcid "041d" 0 +3
    StrCpy $lang_sec ${lang_sv_SE}
    Goto done
  StrCmp $lcid "041f" 0 +3
    StrCpy $lang_sec ${lang_tr_TR}
    Goto done
done:
FunctionEnd

Function .onInit
  findconprocess:
    FindProcDLL::FindProc "CitiesOnline.exe"
    StrCmp $R0 "1" foundcon

    Goto notfoundcon

  foundcon:
    ; Tell them they need to shut down the process.
    MessageBox MB_RETRYCANCEL "Cities Online is currently running.  You must close it before installing.  Right-click the city icon in your tray and select Exit." IDRETRY findconprocess IDCANCEL quitcon
    Goto notfoundcon
    
  quitcon:
	Abort
	
  notfoundcon:
  
  findc3dprocess:
    FindProcDLL::FindProc "Cities3D.exe"
    StrCmp $R0 "1" foundc3d

    Goto notfoundc3d

  foundc3d:
    ; Tell them they need to shut down the process.
    MessageBox MB_RETRYCANCEL "Cities3D is currently running.  You must close it before installing." IDRETRY findc3dprocess IDCANCEL quitc3d
    Goto notfoundc3d
    
  quitc3d:
	Abort
	
  notfoundc3d:

  !insertmacro SelectSection ${lang_en_US}

  ReadRegStr $lcid HKLM "SYSTEM\CurrentControlSet\Control\Nls\Language" Default
  Call LCID2Lang
  !insertmacro SelectSection $lang_sec

  ReadRegStr $user_nat  HKU "Control Panel\International" Locale
  StrCpy $lcid $user_nat 4 -4
  Call LCID2Lang
  !insertmacro SelectSection $lang_sec

  ReadRegStr $user_nat  HKCU "Control Panel\International" Locale
  StrCpy $lcid $user_nat 4 -4
  Call LCID2Lang
  !insertmacro SelectSection $lang_sec
FunctionEnd

Function un.onInit
  findconprocess:
    FindProcDLL::FindProc "CitiesOnline.exe"
    StrCmp $R0 "1" foundcon

    Goto notfoundcon

  foundcon:
    ; Tell them they need to shut down the process.
    MessageBox MB_RETRYCANCEL "Cities Online is currently running.  You must close it before uninstalling.  Right-click the city icon in your tray and select Exit." IDRETRY findconprocess IDCANCEL quitcon
    Goto notfoundcon
    
  quitcon:
	Abort
	
  notfoundcon:
  
  findc3dprocess:
    FindProcDLL::FindProc "Cities3D.exe"
    StrCmp $R0 "1" foundc3d

    Goto notfoundc3d

  foundc3d:
    ; Tell them they need to shut down the process.
    MessageBox MB_RETRYCANCEL "Cities3D is currently running.  You must close it before uninstalling." IDRETRY findc3dprocess IDCANCEL quitc3d
    Goto notfoundc3d
    
  quitc3d:
	Abort
	
  notfoundc3d:
FunctionEnd