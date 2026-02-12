.ifndef __RAM_PRG_VS_INC__
__RAM_PRG_VS_INC__ = 1

; ============================================
; PRG-RAM (versus context) linear allocator
; versus context starts at $6800
; ============================================
__PRGRAM_VS_OFFSET__ .set $800
.macro PRGRAM_VS_NOINC name
    name = PRGRAM_BASE + __PRGRAM_VS_OFFSET__
.endmacro
.macro PRGRAM_VS name, size
    .if (__PRGRAM_VS_OFFSET__ + (size)) > $2000
        .error "PRGRAM overflow: name"
    .endif
    name = PRGRAM_BASE + __PRGRAM_VS_OFFSET__
    __PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + (size)
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $68xx SRAM for 2P Vs ONLY
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; NOTE: $6000-$67FF is still in considered tile grid memory (see next section)
; 2P Vs just utilizes a chunk where no tiles will ever exist in 2P Mode

; 2P Vs Only
	PRGRAM_VS_NOINC Vs_MemStart ; Should be at "top"; this point and 512 bytes forward are cleared at start of 2P Vs

	; Vs_PlayerFrame
	; 0/1: Standing (0) / walking (0/1) / falling-not-jumped (1) frames
	; 2: Skidding turnaround
	; 3: Jumping/Falling-jumped
	; 4: Dizzy
	; 5: Kicking
	; 6: Dying
	; 7: Climbing
	PRGRAM_VS Vs_PlayerFrame, 2	; $6800-$6801 Mario/Luigi Frame
	PRGRAM_VS Vs_PlayerState, 2	; $6802-$6803 Mario/Luigi State (0=Init, 1=Normal, 2=Dying, 3=Ladder climbing)
	PRGRAM_VS Vs_ObjectState, 12	; $6804-$680F Objects State (0=Dead/empty, 1=Normal, 2=Flipped over, 3=Dying)

__PRGRAM_VS_OFFSET__ .set (__PRGRAM_VS_OFFSET__ + 1)

	PRGRAM_VS Vs_PlayerBlkHitCnt, 2	; $6811-$6812 Mario/Luigi Hit block counter value
	PRGRAM_VS Vs_PlayerY, 2	; $6813-$6814 Mario/Luigi Y
	PRGRAM_VS Vs_ObjectsY, 12	; $6815-$6820 Objects Y

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerBlkHitY, 2	; $6822-$6823 Mario/Luigi Aligned Y position where block was hit
	PRGRAM_VS Vs_PlayerX, 2	; $6824-$6825 Mario/Luigi X
	PRGRAM_VS Vs_ObjectsX, 12	; $6826-$6831 Objects X

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerBlkHitX, 2	; $6833-$6834 Mario/Luigi Aligned Y position where block was hit
	PRGRAM_VS Vs_PlayerYVel, 2	; $6835-$6836 Mario/Luigi Y Velocity
	PRGRAM_VS Vs_ObjectYVel, 12	; $6837-$6842 Objects Y Velocity

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerBlkHitYVel, 2	; $6844-$6845 Mario/Luigi Hit block Y velocity
	PRGRAM_VS Vs_PlayerXVel, 2	; $6846-$6847 Mario/Luigi X Velocity
	PRGRAM_VS Vs_ObjectXVel, 12	; $6848-$6853 Objects X Velocity

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 3

	PRGRAM_VS Vs_PlayerClimbFrame, 2	; $6857-$6858 incremented as Player climbs
	PRGRAM_VS Vs_ObjectAnimCnt, 12	; $6859-$6864 A continuous counter per object for animating (typically 2 frames)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerDir, 2	; $6866-$6867 Mario/Luigi direction (1=Right, 2=Left)
	PRGRAM_VS Vs_ObjectDir, 12	; $6868-$6873 Objects direction (1=Right, 2=Left)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerYVelFrac, 2	; $6875-$6876 Mario/Luigi Y velocity fractional accumulator
	PRGRAM_VS Vs_ObjectYVelFrac, 12	; $6877-$6882

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 3

	PRGRAM_VS Vs_PlayerXVelFrac, 2	; $6886-$6887 Mario/Luigi X velocity fractional accumulator
	PRGRAM_VS Vs_ObjectXVelFrac, 12	; $6888-$6893

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 3

	PRGRAM_VS Vs_PlayerDetStat, 2	; $6897-$6898 Mario/Luigi detection status
	PRGRAM_VS Vs_ObjectDetStat, 12	; $6899-$68A4 Objects detection status

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 3

	PRGRAM_VS Vs_ObjectVar1, 12	; $68A8-$68B3 General variable 1

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerKick, 2	; $68B5-$68B6 Mario/Luigi Player is kicking until decrements to zero
	PRGRAM_VS Vs_PlayerDizzy, 2	; $68B7-$68B8 Mario/Luigi Player "dizzy" face until decrements to zero
	PRGRAM_VS Vs_PlayerStick, 2	; $68B9-$68BA Mario/Luigi Mario/Luigi Player "sticking" to ceiling; decrements to zero
	PRGRAM_VS Vs_PlayerBumpTimer, 1	; Mario/Luigi Players bumped off eachother (and can't again until zero); decrements to zero
	PRGRAM_VS Vs_POWBlockCnt, 1	; POW block counter; decrements to zero; until then, POW shaking!

	; 2P Vs Object IDs
VSOBJID_SPINY		= 0	; Spiny
VSOBJID_SIDESTEPPER	= 2	; Sidestepper
VSOBJID_FIGHTERFLY	= 3	; Fighter Fly
VSOBJID_FIREBALL_HORZ	= 4	; Horizontal Fireball that spawns to keep Players from hiding down at the bottom
VSOBJID_FIREBALL_ENDER	= 5	; Game Ender Fireball (bounces around, attempts to kill Players who've stuck around too long!)
VSOBJID_FIREBALL_FOUNTAIN= 6	; Fountain Fireball
VSOBJID_COIN		= 7	; Coin (from [?] block)
VSOBJID_MUSHROOMCARD	= 8	; Mushroom card
VSOBJID_FLOWERCARD	= 9	; Flower card
VSOBJID_STARCARD	= 10	; Star card
VSOBJID_KICKEDBLOCK	= 11	; Kicked block (from [?] block match)
	PRGRAM_VS Vs_ObjectId, 12	; $68BD-$68C8 Objects ID

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_ObjectSprRAMOff, 1	; Current object Sprite RAM offset
	PRGRAM_VS Vs_ObjectSprRAMSel, 1	; Counter that runs $D to $0 (inclusive) and helps distribute Sprite RAM offsets among the objects
	PRGRAM_VS Vs_EnemyCount, 1	; Number of spawned enemies (in the typical game)
	PRGRAM_VS Vs_PlayerHaltTimer, 2	; $68CD-$68CE Mario/Luigi timer which halts gameplay; decrements to zero
	PRGRAM_VS Vs_ObjHaltTimer, 12	; $68CF-$68DA Object timer which halts object when greater than zero; decrements to zero

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_ObjectTimer3, 12	; $68DC-$68E7

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerCnt, 2	; $68E9-$68EA Mario/Luigi "counter" value; decrements to zero
	PRGRAM_VS Vs_EnemyGetUpTimer, 12	; $68EB-$68F6 Timer for flipped-over enemy; decrements to zero

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_PlayerJumped, 2	; $68F8-$68F9 Set to 1 if Player jumped; prevents Player from jumping again until they hit floor
	PRGRAM_VS Vs_PlayerTileL, 2	; $68FA-$68FB Mario/Luigi Tile detected at Player's feet
	PRGRAM_VS Vs_ObjectTileL, 12	; $68FC-$6907

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 2

	PRGRAM_VS Vs_PlayerBlkHit, 2	; $690A-$690B Mario/Luigi Holds Tile_Mem offset to bounce block they hit
	PRGRAM_VS Vs_PlayerFlashInv, 2	; $690C-$690D Mario/Luigi Flashing invicibility (?)
	PRGRAM_VS Vs_SpawnCnt2, 1	; FIXME describe better
	PRGRAM_VS Vs_TooLongCnt, 1	; Increments after each round of spawning; if it overflows, "game ender" fireballs are spawned 
	PRGRAM_VS Vs_CurIndex, 1	; Current index (Player or object)
	PRGRAM_VS Vs_PlayerTileU, 2	; $6911-$6912 Mario/Luigi Tile detected above Player's feet

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 13

	PRGRAM_VS Vs_ObjectPipeTimer, 12	; $6920-$692B Timer used for enemies to exit and emerge from pipes; decrements to zero

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_Random, 3	; $692D-$692F Random generator for 2P Vs mode
	PRGRAM_VS Vs_PlayerCoins, 2	; $6930-$6931 Player's coins (in 2P Vs); 5 wins the match
	PRGRAM_VS Vs_TimeToExit, 1	; Decrements to zero then exits the 2P Vs
	PRGRAM_VS Vs_ObjectIsLast, 12	; $6933-$693E Set if this is the last object (turns blue, move fast)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_POWHits, 1	; Number of times POW block has been hit (disabled on 3)
	PRGRAM_VS Vs_PlayerYOff, 2	; $6941-$6942 Mario/Luigi Y offset applied
	PRGRAM_VS Vs_UNKGAMECnt, 1	; Unknown "game" counter; after overflow, we exit
	PRGRAM_VS Vs_PlayerYHi, 2	; $6944-$6945 Mario/Luigi Y Hi
	PRGRAM_VS Vs_ObjectYHi, 12	; $6946-$6951 Object Y Hi

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 3

	PRGRAM_VS Vs_ObjectIsAngry, 12	; $6955-$6960 Set when Sidestepper is angry (not used for anything else)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_AngrySidesteppers, 1	; When greater than zero, and spawning a Sidestepper, next one is an "angry" Sidestepper (then decrement)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_ObjectVDir, 12	; $6964-$696F Objects vertical direction (4=Down, 8=Up)

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_ObjectRestoreXVel, 12	; $6971-$697C Flipped over object restore X velocity

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_ObjTimer2, 12	; $697E-$6989 Object timer; decrements to zero

__PRGRAM_VS_OFFSET__ .set __PRGRAM_VS_OFFSET__ + 1

	PRGRAM_VS Vs_CardFlash, 2	; $698B-$698C Mario/Luigi Cycles color for card (when picked up from another Player)
	PRGRAM_VS Vs_HaltTimerBackup, 15	; $698D-$699B Backs up all halt timers
	PRGRAM_VS Vs_EnemySet, 1	; Specifies an index of active enemy set, selecting one of the quintuples from Vs_5EnemySets
	PRGRAM_VS Vs_ObjectXOff, 1	; A one-shot X offset for display of object FIXME: When?
	PRGRAM_VS Vs_PlayerWalkCnt, 2	; $699E-$699F Mario/Luigi counts up and overflows to toggle walk frames
	PRGRAM_VS Vs_PlayerWalkFrame, 2	; $69A0-$69A1 Mario/Luigi incremented when Vs_PlayerWalkCnt overflows
	PRGRAM_VS Vs_NextObjectIsLast, 1	; If there are 5 enemies and this is set, next enemy out is the "last" (turns blue, moves fast)

	; Display of "x Up" after getting 3 cards
	PRGRAM_VS Vs_xUpCnt, 2	; $69A3-$69A4 Mario/Luigi "x Up" counter
	PRGRAM_VS Vs_xUpY, 2	; $69A5-$69A6 Mario/Luigi "x Up" Y pos
	PRGRAM_VS Vs_xUpX, 2	; $69A7-$69A8 Mario/Luigi "x Up" X pos
	PRGRAM_VS Vs_xUpLives, 2	; $69A9-$69AA Mario/Luigi "x Up" Lives amount (1, 2, 3, 5)
	PRGRAM_VS Vs_SpawnCnt, 1	; Spawn counter; increments and triggers spawning
.endif; include guard
