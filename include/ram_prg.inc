.ifndef __RAM_PRG_INC__
__RAM_PRG_INC__ = 1

.segment "PRGRAM"

; =====================================
; PRG-RAM linear allocator
; =====================================
__PRGRAM_OFFSET__ .set 0
.macro PRGRAM_NOINC name
    name = PRGRAM_BASE + __PRGRAM_OFFSET__
.endmacro
.macro PRGRAM name, size
    .if (__PRGRAM_OFFSET__ + (size)) > $2000
        .error "PRGRAM overflow: name"
    .endif
    name = PRGRAM_BASE + __PRGRAM_OFFSET__
    __PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + (size)
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $6000-$7FFF MMC3 SRAM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; NOTE: $6800+ is used by 2P Vs RAM, see previous section

	; Tile_Mem stores for the greatest case:
	;	Vertical level max size is 	15 rows * 16 columns * 16 screens = 3840 ($0F00) bytes
	;	Non-vertical level max size is 	27 rows * 16 columns * 15 screens = 6480 ($1950) bytes
	PRGRAM Tile_Mem, 6480	; $6000-$794F Space used to store the 16x16 "tiles" that make up the World Map or Level

	PRGRAM Map_MoveRepeat, 2	; $7950-$7951 (Mario/Luigi) counts up to $18 and then you keep moving without pause
	PRGRAM AScrlURDiag_OffsetX, 1	; When diagonal autoscroller is wrapping, this holds an X offset for Player/Objects to temporarily correct
	PRGRAM AScrlURDiag_OffsetY, 1	; When diagonal autoscroller is wrapping, this holds an Y offset for Player/Objects to temporarily correct
	PRGRAM StatusBar_UpdFl, 1	; Status bar Update Flag; toggles so to update status bar only every other frame
	PRGRAM UpdSel_Disable, 1	; When set, disables the Update_Select routine during the NMI, which halts most activity due to no reported V-Blanking
	PRGRAM Map_Objects_Itm, 13	; $7956-$795D, "Item given by" map objects

	; Item that will be given by treasure box; set by the object OBJ_TREASURESET by its row
	; Level_TreasureItem:
	; 0 = INVALID
	; 1 = Mushroom
	; 2 = Flower
	; 3 = Leaf
	; 4 = Frog
	; 5 = Tanooki
	; 6 = Hammer
	; 7 = Judgem's cloud
	; 8 = P-Wing
	; 9 = Star
	; A = Anchor
	; B = Hammer
	; C = Warp Whistle
	; D = Music Box
	PRGRAM Level_TreasureItem, 1
	PRGRAM Reset_Latch, 1	; If this value is anything besides $5A, the reset is run (I assume this is considered a safe value in case of data corruption, e.g. from runaway generator)
	PRGRAM Map_BonusType, 1	; 0 = No bonus, 1 = White Toad House, 2 = UNKNOWN WHITE THING (MAPOBJ_UNK0C)
	PRGRAM Map_BonusCoinsReqd, 1	; Number of coins you need for White Toad House (or the MAPOBJ_UNK0C thing!); value ranges 0-127
	PRGRAM Coins_ThisLevel, 1	; Internal counter of coins earned -this level- (so always starts at 0 and increments)

	PRGRAM Map_NSpade_NextScore, 3	; $7968 (H)-$796A (L) treated as 3-byte integer

	PRGRAM Map_BonusAppY, 1	; Map "white" bonus appearance Y (set to Player's last "succeeded" map position)
	PRGRAM Map_BonusAppXHi, 1	; Map "white" bonus appearance XHi (set to Player's last "succeeded" map position)
	PRGRAM Map_BonusAppX, 1	; Map "white" bonus appearance X (set to Player's last "succeeded" map position)

	PRGRAM Map_NoLoseTurn, 1	; If set, Player does not lose turn after having completed a level (used for Toad House, pipeways, etc.)
	PRGRAM Map_Got13Warp, 1	; Set non-zero if Player already got the 1-3 Warp Whistle
	PRGRAM Map_Anchored, 1	; Set if anchor is set on this map
	PRGRAM Map_WhiteHouse, 1	; Set if you have already earned the White Toad House for this world
	PRGRAM Map_CoinShip, 1	; Set if you have already earned the Coin Ship for this world
	PRGRAM Map_WasInPipeway, 1	; Set if you just came out of a pipeway
	PRGRAM EndCard_Flag, 1	; Set when End Level card is hit (can determine when level has ended)
	PRGRAM Map_PlyrSprOvrY, 1	; "Player Sprite Override Y"; If set to $F8 during warp, erases Player's map sprite; otherwise provides a Y to put it at
	PRGRAM Map_Entered_Y, 2	; $7976-$7977 (Mario/Luigi) Stores the Y value when you enter a level; this is the Y used if you complete the level
	PRGRAM Map_Entered_XHi, 2	; $7978-$7979 (Mario/Luigi) Hi byte for Map_Entered_X
	PRGRAM Map_Entered_X, 2	; $797A-$797B (Mario/Luigi) Same as Map_Entered_Y, only for X
	PRGRAM Map_Previous_UnusedPVal2, 2	; $797C-$797D (Mario/Luigi) Backup of Map_UnusedPlayerVal2
	PRGRAM Map_Previous_Y, 2	; $797E-$797F (Mario/Luigi) Stores the previous Y you were "safe" at; this is the Y you go back to if you die
	PRGRAM Map_Previous_XHi, 2	; $7980-$7981 (Mario/Luigi) Same as Map_Previous_Y, only for XHi
	PRGRAM Map_Previous_X, 2	; $7982-$7983 (Mario/Luigi) Same as Map_Previous_Y, only for X
	PRGRAM Map_Unused7984, 2	; $7984-$7985 (Mario/Luigi) Unused; cleared and never touched again
	PRGRAM Map_Prev_XOff2, 2	; $7986-$7987 (Mario/Luigi) Holds a copy of Map_Prev_XOff, but I'm not sure why?
	PRGRAM Map_Prev_XHi2, 2	; $7988-$7989 (Mario/Luigi) Holds a copy of Map_Prev_XHi, but I'm not sure why?
	PRGRAM Map_Unused798A, 2	; $798A-$798B (Mario/Luigi) Unused; cleared and never touched again

	; These define values to use when you junction back
	; to the level you were before...
	PRGRAM Level_Jct_HSHi, 1	; Level junction horizontal scroll high value
	PRGRAM Level_Jct_HS, 1	; Level junction horizontal scroll value
	PRGRAM Level_Jct_VSHi, 1	; Level junction vertical scroll high value
	PRGRAM Level_Jct_VS, 1	; Level junction vertical scroll value

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 2   ; $7990-7991 unused

	PRGRAM_NOINC Map_Unused7992			; Value used in some dead code in PRG011; cleared elsewhere (NOT SURE if maybe it sometimes meant Bonus_DiePos?)
	PRGRAM Bonus_DiePos, 1	; UNUSED Die in the lost bonus games, counts 0-5
	PRGRAM Map_Previous_Dir, 2	; $7993-$7994 (Mario/Luigi) Backup movement dir (remember which way Player moved last) (8=Up, 4=Down, 2=Left, 1=Right)
	PRGRAM Map_Unused7995, 1	; Unused; cleared but never used otherwise
	PRGRAM Player_NoSlopeStick, 1	; If set, Player does not stick to slopes (noticeable running downhill)

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 105	; $7997-$79FF unused

	; Auto scroll effect variables -- everything to do with screens that aren't scrolling in the normal way
	; NOTE: Post-airship cinematic scene with Toad and King ONLY uses $7A01-$7A11 MMC3 SRAM (from Level_AScrlSelect to Level_AScrlHVelCarry)
	PRGRAM AScroll_Anchor, 1	; Used as starting point for $7A00-$7A14 clear, but never actually used in Auto-Scroll
	PRGRAM Level_AScrlSelect, 1	; Selects auto scroll routine to use (see PRG009_B922)

	; Values used in horizontal scrolling (Level_AScrlSelect = 0/1) only:
	; $00: World 3-6 / 1-4
	; $01: World 3 Airship
	; $02: World 6-2
	; $03: World 5 Airship
	; $04: World 2 Airship
	; $05: World 4 Airship
	; $06: World 6 Airship
	; $07: World 5-6
	; $0A: World 6-7
	; $0B: World 1 Airship
	; $0C: World 7 Airship
	; $0D: World 8 Airship
	; $0E: World 8 Battleship
	; $0F: World 7-4
	; $10: World 1 Coin Heaven
	; $11: Coin Ship
	; $13: World 8 Tank 1
	; $14: World 8 Tank 2
	PRGRAM Level_AScrlLimitSel, 1	; "Limit Selector" for the auto scroll (typically selects an end or a start/end pair, depending on style)

	; Level_AScrlVar
	; Variable used for different things depending on the auto scroll style
	; In horizontal scroll style (Level_AScrlSelect = 0), it's the current "movement" (see table AScroll_Movement)
	PRGRAM Level_AScrlVar, 1

	PRGRAM Level_AScrlLoopSel, 1	; Currently selected "movement loop" (horizontal only, see AScroll_MovementLoopStart; Just a var in others?)
	PRGRAM Level_AScrlMoveRepeat, 1	; Repeat current move until zero (decrements each full expiration of Level_AScrlMoveTicks); $FF when on last move, passes control to movement loop
	PRGRAM Level_AScrlLoopCurMove, 1	; Current "movement loop" index (into AScroll_MovementLoop)
	PRGRAM Level_AScrlSclLastDir, 1	; Auto scroll "Scroll_LastDir" 
	PRGRAM Level_AScrlMoveTicks, 1	; Counts down to zero, decrements Level_AScrlMoveRepeat (goes to next "movement")
	PRGRAM Level_AScrlTimer, 1	; Auto scroll counter, decrements to zero
	PRGRAM Level_AScrlPosHHi, 1	; Raster effect horizontal "high" position

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 1  ; $7A0B

	PRGRAM Level_AScrlPosH, 1	; Raster effect horizontal position
	PRGRAM Level_AScrlPosV, 1	; Raster effect vertical position
	PRGRAM Level_AScrlHVel, 1	; Auto scroll horizontal "velocity"
	PRGRAM Level_AScrlVVel, 1	; Auto scroll vertical "velocity"
	PRGRAM Level_AScrlHVelFrac, 1	; Auto scroll horizontal velocity fractional accumulator 
	PRGRAM Level_AScrlVVelFrac, 1	; Auto scroll vertical velocity fractional accumulator 
	PRGRAM Level_AScrlHVelCarry, 1	; '1' when last auto scroll H Velocity fraction accumulation rolled over
	PRGRAM Level_AScrlVVelCarry, 1	; '1' when last auto scroll V Velocity fraction accumulation rolled over
	PRGRAM World8Tank_OnTank, 1	; Set when Player is standing on tank surface in Tank level (as opposed to ground); for the illusion the tank is moving through...
;;;;;;;;;;;;


; "Cannon Fire" are sort of objects that exist to repeatedly fire off cannon balls
; or other such armaments.  They are created by 

	PRGRAM CannonFire_ID, 8	; $7A15-$7A1C ID of the cannon fire
	PRGRAM CannonFire_YHi, 8	; $7A1D-$7A24 Cannon fire Y Hi
	PRGRAM CannonFire_Y, 8	; $7A25-$7A2C Cannon fire Y
	PRGRAM CannonFire_XHi, 8	; $7A2D-$7A34 Cannon fire X Hi
	PRGRAM CannonFire_X, 8	; $7A35-$7A3C Cannon fire X
	PRGRAM CannonFire_Parent, 8	; $7A3D-$7A44 Tie back to level object index of "parent" object

	PRGRAM Splash_DisTimer, 1	; Player water splashes are disabled until decrements to zero; set when Player hits any bounce block

	; For that little "flash" that comes from the shell kill impact!
	PRGRAM ShellKillFlash_Cnt, 1	; "Shell Kill Flash" counter
	PRGRAM ShellKillFlash_Y, 1	; "Shell Kill Flash" Y
	PRGRAM ShellKillFlash_X, 1	; "Shell Kill Flash" X

; NOTE!! Objects_DisPatChng for OBJECT SLOT 0 - 5 ONLY!
	PRGRAM Objects_DisPatChng, 6	; $7A49-$7A4E If set, this object no longer enforces a pattern bank change

; NOTE!! These object vars are OBJECT SLOT 0 - 5 ONLY!
	PRGRAM ObjSplash_DisTimer, 6	; $7A4F-$7A54 Object water/lava splashes are disabled until decrements to zero

	PRGRAM PlayerProj_XVelFrac, 2	; $7A55-$7A56 Player Projectile X velocity fractional accumulator

	PRGRAM CannonFire_Timer2, 8	; $7A57-$7A5E Cannon Fire timer (decrements to zero)

	PRGRAM Roulette_Unused7A5F, 1	; Unused value in Roulette game
	PRGRAM Roulette_Unused7A5F_Delta, 1	; Delta value added to Roulette_Unused7A5F

	PRGRAM Bowser_Tiles, 2	; $7A61-$7A62 Bowser's detected tiles (to determine what to break)
	PRGRAM Bowser_Counter1, 1	; A counter used by Bowser, decrements to zero
	PRGRAM Bowser_Counter2, 1	; A counter used by Bowser, decrements to zero 
	PRGRAM Bowser_Counter3, 1	; A counter used by Bowser, random setting, decrements to zero

	PRGRAM CoinShip_CoinGlowIdx, 1	; Coin Ship only: Glowing coins palette color index
	PRGRAM CoinShip_CoinGlowCnt, 1	; Coin Ship only: Glowing coins palette color counter

	PRGRAM SObjBlooperKid_OutOfWater, 8	; $7A68-$7A6F Blooper kid only; if set, Blooper Kid is trying to go out of water

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 2   ; $7A70-$7A71 unused

	PRGRAM Object_SplashAlt, 1	; Used to alternate the "splash slots" 1 and 2 as objects hit the water

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 109 ; $7A73-$7ADF unused

	PRGRAM Music_Start, 1	; Music start index (beginning of this song)
	PRGRAM Music_End, 1	; Music end index (inclusive last index to play before loop)
	PRGRAM Music_Loop, 1	; Music loop index (index to start from when song reaches end)
	PRGRAM Sound_Octave, 1	; Used for calculating octave
__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 12  ; $7AE4-$7AEF unused
	PRGRAM Music_Sq1Bend, 1	; Alters PAPU_FT1 for bend effects
__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 3  ; $7AF1-$7AF3 unused
	PRGRAM Music_Sq2Bend, 1	; Alters PAPU_FT2 for bend effects
__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 2   ; $7AF5-$7AF6 unused
	PRGRAM Music_RestH_Off, 1	; Offset added to Music_RestH_Base; typically $00 or $10 (for low time warning on compatible songs)
__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 7   ; $7AF8-$7AFE unused
	PRGRAM PAPU_MODCTL_Copy, 1	; Current PAPU_MODCTL register

	PRGRAM Level_ObjIdxStartByScreen, 16	; $7B00-$7B0F Defines the starting index into Level_Objects for each "screen"

	PRGRAM Level_ObjectsSpawned, 48	; $7B10-$7B3F When $80 set, object is already spawned, $00 means not

; Level_ObjPtr_AddrL is an array that defines the level objects to appear
; The first byte copied in has no apparent purpose
; The rest is a repeating series of 3 bytes -- ID, Column, Row (C/R of tile grid, multiply by 16 for pixel location), $FF for terminator
	PRGRAM Level_Objects, 48*3	; $7B40-$7BCF

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 80  ; $7BD0-$7C1F unused


; For certain objects that require a buffer of X or Y values; only a couple are available.
; Each contains 32 bytes, intended for enemies that have "tails"; Buffer_Occupied determines
; which of the two buffers is free, if any at all.  The object will hold onto it then.
; Because of this, objects which employ it must also be hardcoded to release it; see
; "Object_Delete" for the hardcoded list of objects which must release this resource...
	PRGRAM Object_BufferX, 32*2	; $7C20-$7C3F / $7C40-$7C5F
	PRGRAM Object_BufferY, 32*2	; $7C60-$7C7F / $7C80-$7C9F

; Variables used by Chain Chomps ONLY -- manages the chain links 
	PRGRAM ChainChomp_ChainX1, 5	; $7CA0-$7CA4 Chain Link 1 X
	PRGRAM ChainChomp_ChainX2, 5	; $7CA5-$7CA9 Chain Link 2 X
	PRGRAM ChainChomp_ChainX3, 5	; $7CAA-$7CAE Chain Link 3 X
	PRGRAM ChainChomp_ChainX4, 5	; $7CAF-$7CB4 Chain Link 4 X

	PRGRAM ChainChomp_ChainY1, 5	; $7CB4-$7CB8 Chain Link 1 Y
	PRGRAM ChainChomp_ChainY2, 5	; $7CB9-$7CBD Chain Link 2 Y
	PRGRAM ChainChomp_ChainY3, 5	; $7CBE-$7CC2 Chain Link 3 Y
	PRGRAM ChainChomp_ChainY4, 5	; $7CC3-$7CC8 Chain Link 4 Y

; NOTE!! These object vars are OBJECT SLOT 0 - 4 ONLY!
	PRGRAM Objects_Var10, 5	; $7CC8-$7CCC Generic object variable 10
	PRGRAM Objects_Var11, 5	; $7CCD-$7CD1 Generic object variable 11
	PRGRAM Objects_Var12, 5	; $7CD2-$7CD6 Generic object variable 12
	PRGRAM Objects_Var13, 5	; $7CD7-$7CDB Generic object variable 13
	PRGRAM Objects_Var14, 5	; $7CDC-$7CE0 Generic object variable 14

; Player's hammer/fireball
	PRGRAM PlayerProj_ID, 2	; $7CE1-$7CE2 Player projectile ID (0 = not in use, 1 = fireball, 2 = hammer, 3+ = Fireball impact "Poof")
	PRGRAM PlayerProj_Y, 2	; $7CE3-$7CE4 Player projectile Y
	PRGRAM PlayerProj_X, 2	; $7CE5-$7CE6 Player projectile X
	PRGRAM PlayerProj_YVel, 2	; $7CE7-$7CE8 Player projectile Y Velocity (NOTE: Integer, not 4.4FP)
	PRGRAM PlayerProj_XVel, 2	; $7CE9-$7CEA Player projectile X Velocity (NOTE: Fireball is integer, 4.4FP for hammer ONLY)
	PRGRAM Fireball_HitChkPass, 2	; $7CEB-$7CEC Count of times Player's fireball has gone through hit check; when it hits 2, fireball poofs
	PRGRAM PlayerProj_Cnt, 2	; $7CED-$7CEE Player projectile counter

	PRGRAM Temp_VarNP0, 1	; A temporary not on page 0

	PRGRAM Lakitu_Active, 1	; Set while a Lakitu is active; keeps Lakitu "alive" even if off-screen etc.

	PRGRAM LevelEvent_Cnt, 1	; General purpose counter used by a couple LevelEvents
	PRGRAM Vert_Scroll_Off, 1	; Vertical scroll offset, used for "vibration" effects
	PRGRAM Level_Vibration, 1	; While greater than zero, screen vibrates (from impact of heavy fellow)
	PRGRAM Player_VibeDisable, 1	; While greater than zero, Player is unable to move (from impact of heavy fellow)
	PRGRAM Player_TwisterSpin, 1	; While greater than zero, Player is twirling from sand twister

; NOTE!! This object var is OBJECT SLOT 0 - 4 ONLY!
	PRGRAM Objects_HitCount, 5	; $7CF6-$7CFA Somewhat uncommon "HP" used generally for bosses only (e.g. they take so many fireballs)


	PRGRAM RotatingColor_Cnt, 1	; When non-zero, causes rainbow palettes in the background; $80 bit is used by Koopaling wand grab

; Some variables used by the recovered magic wand
	PRGRAM Wand_FrameCnt, 1	; A counter that overflows to increment Wand_Frame (added to by the wand's SpecialObj_Var1)
	PRGRAM Wand_Frame, 1	; Wand frame
	PRGRAM Wand_BounceFlag, 1	; Tracks the recovered wand bounce; odd on first bounce

	PRGRAM Player_DebugNoHitFlag, 1	; UNUSED: (Old debug routine) When set, disables getting hurt (would be toggled by pressing SELECT; see PRG000 $C91B)

; Map_Completions:
; Stores "rows" of completed levels or other map alterations (e.g. rock break,
; mini-fortress lock removal, etc.) for a given column, from the leftmost.
; Rows 1-7 use decending bits from $80, and row 9 uses bit $01.  This makes row 8
; invalid for hosting a level panel ever!
; --------- TOP OF MAP
; $80
; $40
; $20
; $10
; $08
; $04
; $02
; INVALID
; $01
; --------- BOT OF MAP
	PRGRAM Map_Completions, 16*4*2	; $7D00-$7D3F (Mario), $7D40-$7D7F (Luigi) Allows a MAX of 4 map screens (64 columns total each player, 16 columns per map screen)

	; Inventory_Items: 
	; 0 = Empty
	; 1 = Mushroom
	; 2 = Flower
	; 3 = Leaf
	; 4 = Frog
	; 5 = Tanooki
	; 6 = Hammer
	; 7 = Judgem's cloud
	; 8 = P-Wing
	; 9 = Star
	; A = Anchor
	; B = Hammer
	; C = Warp Whistle
	; D = Music Box
	PRGRAM Inventory_Items, 4*7	; $7D80-$7D9B Mario, 4 rows of 7 items 
	PRGRAM Inventory_Cards, 3	; $7D9C-$7D9E Mario, 3 cards
	PRGRAM Inventory_Score, 3	; $7D9F-$7DA1 Mario, 3 byte score
	PRGRAM Inventory_Coins, 1	; Mario's coins

	PRGRAM Inventory_Items2, 4*7	; $7DA3-$7DBE Luigi, 4 rows of 7 items 
	PRGRAM Inventory_Cards2, 3	; $7DBF-$7DC1 Luigi, 3 cards
	PRGRAM Inventory_Score2, 3	; $7DC2-$7DC4 Luigi, 3 byte score
	PRGRAM Inventory_Coins2, 1	; Luigi's coins
	PRGRAM Map_Unused7DC6, 5	; $7DC6-$7DCA? Indexed by Map_Unused738, value used in dead routine in PRG011 @ $A2AF

	PRGRAM Map_GameOver_CursorY, 1	; Game Over popup cursor Y ($60/$68)

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 9   ; $7DCC-$7DD4 unused

	PRGRAM Map_PrevMoveDir, 1	; Last SUCCESSFUL (allowed) movement direction on map R01 L02 D04 U08

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 8   ; $7DD6-$7DDD unused
	PRGRAM Pal_Data, 32	; $7DDE-$7DFD Holds an entire bg/sprite palette (this is the MASTER palette, what fades target, and others may source for "original" colors!)

	PRGRAM Level_AltLayout, 2	; $7DFE-$7DFF Pointer to level's "alternate" layout (when you go into bonus pipe, etc.)
	PRGRAM Level_AltObjects, 2	; $7E00-$7E01 Pointer to level's "alternate" object set (when you go into bonus pipe, etc.)

	PRGRAM Level_BlockGrabHitMem, 128	; $7E02-$7E81 Records coins and 1-ups grabbed, so they don't come back if you switch areas

CARD_MUSHROOM	= 0
CARD_FLOWER	= 1
CARD_STAR	= 2
CARD_1UP	= 3
CARD_10COIN	= 4
CARD_20COIN	= 5
CARD_WILD	= 8	; UNUSED Wild card (can match any other!)
	PRGRAM Card_ActiveSet, 18	; $7E82-$7E93 Active set of N-Spade game cards

	; Tile_AttrTable:
	; On the world map, it's always the following:
	; [03 67 BF E9] [03 67 BF E9]
	; There's a usage of checking which "quadrant" of tile the Player is standing on ($00, $40, $80, or $C0)
	; and using that as an index (shifted right 6) into the second half of this table
	; TILE_PANEL1		= $03	; Level Panel 1




	; TILE_FORT		= $67	; Mini-Fortress
	; TILE_POOL		= $BF	; Pool / Oasis
	; TILE_WORLD5STAR	= $E9	; Star used on World 5 Sky map
	; The check follows with a "less than", as a quick failure check (if you're in this "range"
	; of tiles, but less than that value, you can't possibly be on an enterable tile)
	; The second half is not used on the world map
	;
	; In levels, both "halves" define the first tile of a quadrant to be solid
	; The first half is solid at the ground (i.e. Player can stand on it)
	; The second half is solid at the head and walls (i.e. Player bumps head on it, typically "full solidity" when combined above)
	; Interestingly, the Sonic the Hedgehog games implemented this same solidity pattern...
	PRGRAM Tile_AttrTable, 8	; $7E94-$7E9B

	PRGRAM Level_UnusedSlopesTS5, 1	; UNUSED; If set to 2, forces slopes to be enabled for Level_Tileset = 5 (plant infestation)
	PRGRAM PlantInfest_ACnt_Max, 1	; Always set to $1A in plant infestation levels, sets max value for animation counter

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 24   ; $7E9E-$7EB5 unused

	PRGRAM LevelJctBQ_Flag, 1	; Set to '1' while in a Big Question block area, locks horizontal scrolling
	PRGRAM Level_JctBackupTileset, 1	; Level Junction tileset backup
	PRGRAM Level_AltTileset, 1	; Level's "alternate" tileset (when you go into bonus pipe, etc.)

	; The "ORIGINAL" series are so you can switch back after going to a level's "alternate"
	PRGRAM Level_LayPtrOrig_AddrL, 1	; ORIGINAL Low byte of address to tile layout
	PRGRAM Level_LayPtrOrig_AddrH, 1	; ORIGINAL High byte of address to tile layout
	PRGRAM Level_ObjPtrOrig_AddrL, 1	; ORIGINAL Low byte of address to object set
	PRGRAM Level_ObjPtrOrig_AddrH, 1	; ORIGINAL High byte of address to object set

	PRGRAM Level_BG_Page1_2, 1	; Sets which bank the first and second page (2K / 64 8x8 tiles) of BG is using (see Level_BG_Pages1/2)

	PRGRAM Map_BorderAttrFromTiles, 44	; $7EBE-$7EC8 (?) Attributes collected from map tiles that get overwritten by border FIXME SIZE UNCERTAIN

	PRGRAM Map_Unused7EEA, 1	; Unused; Value retrieved from LUT at initialization of world, but never used otherwise
	PRGRAM Map_Objects_Y, 14	; $7EEB-$7EF8, Y coordinate of all map objects
	PRGRAM Map_Objects_XLo, 14	; $7EF9-$7F06, X coordinate lo byte of all map objects
	PRGRAM Map_Objects_XHi, 14	; $7F07-$7F14, X coordinate hi byte of all map objects

; Map_Objects_IDs: ID of all 8 map objects 
MAPOBJ_EMPTY		= $00	; None
MAPOBJ_HELP		= $01	; HELP
MAPOBJ_AIRSHIP		= $02	; Airship
MAPOBJ_HAMMERBRO	= $03	; Hammer Bro
MAPOBJ_BOOMERANGBRO	= $04	; Boomerang Bro
MAPOBJ_HEAVYBRO		= $05	; Heavy Bro
MAPOBJ_FIREBRO		= $06	; Fire Bro
MAPOBJ_W7PLANT		= $07	; World 7 Plant
MAPOBJ_UNK08		= $08	; Unknown marching glitch object
MAPOBJ_NSPADE		= $09 	; N-Spade
MAPOBJ_WHITETOADHOUSE	= $0A	; White Toad House
MAPOBJ_COINSHIP		= $0B	; Coin Ship
MAPOBJ_UNK0C		= $0C	; Unknown white colorization of $0F (goes to World 7 level??)
MAPOBJ_BATTLESHIP	= $0D	; World 8 Battleship
MAPOBJ_TANK		= $0E	; World 8 Tank
MAPOBJ_W8AIRSHIP	= $0F	; World 8 Airship
MAPOBJ_CANOE		= $10	; Canoe

MAPOBJ_TOTALINIT	= $08	; Total number of map objects initialized per world
MAPOBJ_TOTAL		= $0E	; Total POSSIBLE map objects
	PRGRAM Map_Objects_IDs, 14	; $7F15-$7F22

	PRGRAM Map_SprRAMOffDistr, 1	; A free running counter on the map only which distributes Sprite_RAM offsets to ensure visibility

	; Map_2PVsGame
	; Sets which "style" of 2P Vs game will be played
	;  0: Spiny Only
	;  1: Fighter Fly Only
	;  2: Spiny and Fighter Fly
	;  3: Static coins
	;  4: Spiny and Sidestepper
	;  5: Fighter Fly and Sidestepper
	;  6: Sidestepper Only
	;  7: Coin Fountain
	;  8: Spiny Only
	;  9: Fighter Fly Only 
	; 10: Sidestepper Only
	; 11: Ladder and [?] blocks
	PRGRAM Map_2PVsGame, 1

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 8   ; $7F25-$7F2C unused

	PRGRAM Map_Airship_Dest, 1	; Airship travel destination; 6 X/Y map coordinates defined per world, after that it just sits still
	PRGRAM THouse_OpenByID, 16	; $7F2E-$7F3D UNUSED would keep track of chests opened for a given Toad House ID (THouse_ID)
	PRGRAM StatusBar_PMT, 8	; $7F3E-$7F45, tiles that currently make up the power meter >>>>>>[P]
	PRGRAM StatusBar_CoinH, 1	; Status bar tile for coin MSD
	PRGRAM StatusBar_CoinL, 1	; Status bar tile for coin LSD
	PRGRAM StatusBar_LivesH, 1	; Status bar tile for lives MSD
	PRGRAM StatusBar_LivesL, 1	; Status bar tile for lives LSD
	PRGRAM StatusBar_Score, 6	; $7F4A-$7F4F Status bar tiles for score
	PRGRAM StatusBar_Time, 3	; $7F50-$7F52 Status bar tiles for time remaining
	PRGRAM Map_MusicBox_Cnt, 1	; Number of turns remaining until hammer brothers wake up (>= 1 and they're be asleep on the map)

	; Store arrays defined by level data as starts after an "alternate" level junction event
	; Level_JctXLHStart:
	;	Lower 4 bits: X Hi
	;	Upper 4 bits: X Lo
	; Level_JctYLHStart:
	;	Bits 0 - 3: Go into Level_PipeExitDir
	;	Bits 4 - 6: 0 to 7, selects start position from LevelJct_YLHStarts and sets proper vertical with LevelJct_VertStarts
	;	Bit      7: If set, entering in vertical mode (for "dirty" refresh purposes)
	PRGRAM Level_JctYLHStart, 16	; $7F54-$7F63 Array of Y / YHi starts
	PRGRAM Level_JctXLHStart, 16	; $7F64-$7F73 Array of X / XHi starts

	PRGRAM Object_TileFeet2, 1	; ? Difference against Object_TileFeet?
	PRGRAM Object_TileWall2, 1	; ? Difference against Object_TileWall?

	PRGRAM ObjTile_DetYHi, 1	; Object tile detect Y Hi
	PRGRAM ObjTile_DetYLo, 1	; Object tile detect Y Lo
	PRGRAM ObjTile_DetXHi, 1	; Object tile detect X Hi
	PRGRAM ObjTile_DetXLo, 1	; Object tile detect X Lo

	PRGRAM Bubble_Cnt, 3	; $7F7A-$7F7C Bubble counter value (0 = no bubble)

; NOTE: Object_WatrHit* values are set only once, then WatrHit_IsSetFlag latches
; and they will never update again; seems it is leftover debug code or maybe
; an unused feature (that an object could respond to a splashdown)
	PRGRAM WatrHit_IsSetFlag, 1	; Set when Object_WatrHit* values are stored (but never cleared, so only once!)
	PRGRAM Bubble_YHi, 3	; $7F7E-$7F80 Water Bubble Y Hi
	PRGRAM Object_WatrHitYHi, 1	; Y Hi of object that just hit water
	PRGRAM Bubble_Y, 3	; $7F82-$7F84 Water Bubble Y
	PRGRAM Object_WatrHitY, 1	; Y of object that just hit water
	PRGRAM Bubble_XHi, 3	; $7F86-$7F88 Water Bubble X Hi
	PRGRAM Object_WatrHitXHi, 1	; X Hi of object that just hit water
	PRGRAM Bubble_X, 3	; $7F8A-$7F8C Water Bubble X
	PRGRAM Object_WatrHitX, 1	; X of object that just hit water

	PRGRAM Splash_Counter, 3	; $7F8E-$7F90 Water splash counter
	PRGRAM Splash_Y, 3	; $7F91-$7F93 Water splash X
	PRGRAM Splash_X, 3	; $7F94-$7F96 Water splash Y
	PRGRAM Splash_NoScrollY, 3	; $7F97-$7F99 If set, flags this water splash to not display sprite Y as relative to screen scroll

	PRGRAM BrickBust_En, 3	; $7F9A-$7F9C Brick bust "Enable" (0 = disable, 2 = brick debris, anything else = "poof" away)
	PRGRAM BrickBust_YUpr, 3	; $7F9D-$7F9F Brick bust upper chunks Y
	PRGRAM BrickBust_X, 3	; $7FA0-$7FA2 Brick bust base X
	PRGRAM BrickBust_YVel, 3	; $7FA3-$7FA5 Brick bust Y velocity
	PRGRAM BrickBust_XDist, 3	; $7FA6-$7FA8 Brick bust X split
	PRGRAM BrickBust_YLwr, 3	; $7FA9-$7FAB Brick bust lower chunks Y
	PRGRAM BrickBust_HEn, 3	; $7FAC-$7FAE Bits to hide chunks (Bit 0 = Right, 1 = Left, 2 = Lower, 3 = Upper) OR poof counter

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 3

	PRGRAM CoinPUp_State, 4	; $7FB2-$7FB5 State of up to 4 "Power Up" coins (i.e. coins that come out of ? blocks and bricks)
	PRGRAM CoinPUp_Y, 4	; $7FB6-$7FB9 Y of "Power Up" coins
	PRGRAM CoinPUp_X, 4	; $7FBA-$7FBD X of "Power Up" coins
	PRGRAM CoinPUp_YVel, 4	; $7FBE-$7FC1 Y velocity of "Power Up" coins
	PRGRAM CoinPUp_Counter, 4	; $7FC2-$7FC5 Counter used by "Power Up" coins

	PRGRAM SpecialObj_ID, 8	; $7FC6-$7FCD Special object spawn event IDs

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 2

	PRGRAM Objects_Var3, 5	; $7FD0-$7FD4 Generic variable 3 for objects SLOT 0 - 4 ONLY

	PRGRAM SpecialObj_YHi, 8	; $7FD5-$7FDC Special object Y high coordinate

__PRGRAM_OFFSET__ .set __PRGRAM_OFFSET__ + 2

	PRGRAM Objects_LastTile, 8	; $7FDF-$7FE6 Last tile this object detected

	PRGRAM Objects_SprAttr, 8	; $7FE7-$7FEE Object sprite attributes (only uses bit 6 for H-Flip and bits 0-1 for palette)
	PRGRAM Objects_UseShortHTest, 8	; $7FEF-$7FF6 If set, object will use a short horizontal test to determine if it is off-screen

	PRGRAM_NOINC Roulette_Lives			; Number of lives you are rewarded from winning the Roulette (NOTE: Shared with first byte of Objects_IsGiant)
	PRGRAM Objects_IsGiant, 8	; $7FF7-$7FFE Set mainly for World 4 "Giant" enemies (but some others, like Bowser, also use it)

.include "ram_prg_vs.inc"
.include "ram_prg_cineking.inc"

.endif; include guard