.zeropage
ZP_BASE:
    .res $100; $0 - $100 zero page

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SMB3 RAM DEFS 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; =====================================
; ZP RAM linear allocator
; =====================================
__ZP_OFFSET__ .set $0
.macro ZP_NOINC name
    .exportzp name = ZP_BASE + __ZP_OFFSET__
.endmacro
.macro ZP name, size
    .if (__ZP_OFFSET__ + (size)) > $100
        .error "ZP overflow name"
    .endif
    .exportzp name = ZP_BASE + __ZP_OFFSET__
    __ZP_OFFSET__ .set __ZP_OFFSET__ + (size)
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM COMMON
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Common use zero page RAM.  Bytes in $75-$F3 are context-dependent

; For clarification, none of the other "Temp" vars are damaged by NMI,
; the NMI does employ Temp_Var1-3, and restores them when it's done.

	ZP Temp_Var1, 1	; Temporary storage variable (protected from damage by NMI)
	ZP Temp_Var2, 1	; Temporary storage variable (protected from damage by NMI)
	ZP Temp_Var3, 1	; Temporary storage variable (protected from damage by NMI)
	ZP Temp_Var4, 1	; Temporary storage variable
	ZP Temp_Var5, 1	; Temporary storage variable
	ZP Temp_Var6, 1	; Temporary storage variable
	ZP Temp_Var7, 1	; Temporary storage variable
	ZP Temp_Var8, 1	; Temporary storage variable
	ZP Temp_Var9, 1	; Temporary storage variable
	ZP Temp_Var10, 1	; Temporary storage variable
	ZP Temp_Var11, 1	; Temporary storage variable
	ZP Temp_Var12, 1	; Temporary storage variable
	ZP Temp_Var13, 1	; Temporary storage variable
	ZP Temp_Var14, 1	; Temporary storage variable
	ZP Temp_Var15, 1	; Temporary storage variable
	ZP Temp_Var16, 1	; Temporary storage variable

	ZP VBlank_Tick, 1	; can be used for timing, or knowing when an NMI just fired off

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1 ; $11 unused

	ZP Horz_Scroll_Hi, 1	; Provides a "High" byte for horizontally scrolling, or could be phrased as "current screen"
	ZP_NOINC PPU_CTL1_Mod; NOT DURING GAMEPLAY, this is used as an additional modifier to PPU_CTL1
	ZP Vert_Scroll_Hi, 1	; Provides a "High" byte for vertically scrolling (only used during vertical levels!)

	ZP Level_ExitToMap, 1	; When non-zero, kicks back to map (OR to event when Player_FallToKing or Player_RescuePrincess is nonzero!)

	ZP Counter_1, 1	; This value simply increments every frame, used for timing various things

	ZP PPU_CTL2_Copy, 1	; Essentially a copy of PPU_CTL2, which updates it as well, though the sprite/BG visibility setting is usually (always?) forced on

	ZP Pad_Holding, 1	; Active player's inputs (i.e. 1P or 2P, whoever's playing) buttons being held in (continuous)
	ZP Pad_Input, 1	; Active player's inputs (i.e. 1P or 2P, whoever's playing) buttons newly pressed only (one shot)

	ZP Roulette_RowIdx, 1	; Roulette Bonus Game only obviously

; Pal_Force_Set12
; This overrides the normal palette routine of selecting by Level_Tileset and 
; loading the color sets PalSel_Tile_Colors/PalSel_Obj_Colors.  Setting 
; Pal_Force_Set12 to a non-zero value will select as the index instead of
; Level_Tileset, and then it will copy the first two sets of 16 colors from
; the palette data as bg / sprite colors.  FIXME is this used though??
	ZP Pal_Force_Set12, 1

	ZP PlantInfest_ACnt, 1	; Plant infestation level animation counter

	ZP VBlank_TickEn, 1	; Enables the VBlank_Tick decrement and typically other things like joypad reading

	ZP Map_Enter2PFlag, 1	; If $00, entering level, otherwise set if entering 2P VS mode


	ZP Map_EnterViaID, 1	; Overrides whatever spot on the map you entered with something special (see Map_DoEnterViaID)

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	; $20 has a lot of different uses on the World Map...
	ZP_NOINC Map_EnterLevelFX; When entering a level on the map, dictates the status of the entry (0=None, 1=Boxing in, 2=Boxing out [J only]) NOTEOverlap/reuse
	ZP_NOINC Map_IntBoxErase; Used for determining where in erasing the "World X" intro box we are NOTEOverlap/reuse
	ZP_NOINC Map_ClearLevelFXCnt; Counter for "clear level" FX occurring (1-6Poof, 7-9Flip) ("poof"/"panel flip") NOTEOverlap/reuse
	ZP Map_ScrollOddEven, 1	; Toggles odd/even column as it scrolls

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	ZP Level_Width, 1	; Width of current level, in screens (0 = don't move at all, max is 15H/16V)

	; In horizontal "typical" levels, Scroll_ColumnR/L are a column and
	; levels are rendered in vertical stripes by these start points
	ZP_NOINC Scroll_ColumnR	; ($23) Current tile column (every 16px) of right side of screen (non-vertical level style ONLY)
	; In vertical style levels, Scroll_VOffsetT/B are an offset into the
	; visible tile grid, and levels are rendered in horizontal strips
	ZP Scroll_VOffsetT, 1	; ($23) Current tile offset (every 16px) of top of screen (vertical level style ONLY)
	ZP_NOINC Scroll_ColumnL	; ($24) Current tile column (every 16px) of left side of screen (non-vertical level style ONLY)
	ZP Scroll_VOffsetB, 1	; ($24) Current tile offset (every 16px) of bottom of screen (vertical level style ONLY)

	ZP Scroll_ColorStrip, 54	; $25-$5A This stores a strip of just the upper 2 bits of a tile ($00, $40, $80, $C0) to produce attribute info

	ZP Scroll_LastDir, 1	; 0=screen last moved right (or up, if vertical), 1=screen last moved left (or down, if vertical)

	ZP_NOINC Scroll_RightUpd; Indicates every 8 pixels update going to the right, or $FF if screen moves left
	ZP Scroll_VertUpd, 1	; Indicates every 8 pixels update up or down, in vertical levels

	ZP Scroll_LeftUpd, 1	; Indicates every 8 pixels update going to the left, or $FF if screen moves right

	; Prepares to perform a Video_Update when possible, indexes the "Video_Upd_Table" 
	; in PRG030 OR Video_Upd_Table2 in PRG025 (whichever is currently in context)
	; Also resets the graphics buffer afterward, since the RAM buffer is
	; constantly being called to possibly perform its own updates after this value
	; resets to zero.
	ZP Graphics_Queue, 1

__ZP_OFFSET__ .set __ZP_OFFSET__ +  2

	ZP Level_LayPtr_AddrL, 1	; Low byte of address to tile layout (ORIGINAL stored in Level_LayPtrOrig_AddrL)
	ZP Level_LayPtr_AddrH, 1	; High byte of address to tile layout (ORIGINAL stored in Level_LayPtrOrig_AddrH)

			; Typical use pair at $63/$64
	ZP_NOINC Map_Tile_AddrL	; Low byte of tile address
    ZP BonusText_BaseL, 1	; Instruction text base address low
	ZP_NOINC Map_Tile_AddrH	; High byte of tile address
	ZP BonusText_BaseH, 1	; Instruction text base address high

	ZP Level_ObjPtr_AddrL, 1	; Low byte of address to object set (ORIGINAL stored in Level_ObjPtrOrig_AddrL)
	ZP Level_ObjPtr_AddrH, 1	; High byte of address to object set (ORIGINAL stored in Level_ObjPtrOrig_AddrH)

__ZP_OFFSET__ .set __ZP_OFFSET__ +  2

	ZP Video_Upd_AddrL, 1	; Video_Misc_Updates routine uses this as an address, low byte
	ZP Video_Upd_AddrH, 1	; Video_Misc_Updates routine uses this as an address, hi byte
	ZP Music_Base_L, 1	; Current music segment base address low byte
	ZP Music_Base_H, 1	; Current music segment base address high byte

	ZP Sound_Sqr_FreqL, 1	; Calculated square wave frequency for Note On (low byte)
	ZP Sound_Sqr_FreqH, 1	; Calculated square wave frequency for Note On (high byte)
	ZP Sound_Map_EntrV, 1	; Current index into the volume ramp-down table used exclusively for the "level enter" sound
	ZP Sound_Map_EntV2, 1	; Same as Sound_Map_EntrV, only for the second track

	ZP Music_PatchAdrL, 1	; Music current patch address low byte
	ZP Music_PatchAdrH, 1	; Music current patch address high byte
	ZP Sound_Map_Off, 1	; Current "offset" within a map sound effect


	; NOTE$75 - $F3 are context specific

__ZP_OFFSET__ .set $f4

	ZP Scroll_OddEven, 1	; 0 or 1, depending on what part of 8 pixels has crossed (need better description)

	ZP Controller1Press, 1	; Player 1's controller "pressed this frame only" (see Controller1 for values)
	ZP Controller2Press, 1	; Player 2's controller "pressed this frame only" (see Controller2 for values)
	ZP Controller1, 1	; Player 1's controller inputs -- R01 L02 D04 U08 S10 E20 B40 A80
	ZP Controller2, 1	; Player 2's controller inputs -- R01 L02 D04 U08 S10 E20 B40 A80

__ZP_OFFSET__ .set __ZP_OFFSET__ +  3

	ZP Vert_Scroll, 1	; Vertical scroll of name table; typically at $EF (239, basically showing the bottom half)
	ZP Horz_Scroll, 1	; Horizontal scroll of name table

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	ZP PPU_CTL1_Copy, 1	; Ho, PPU_CTL1 register data 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM: GAMEPLAY CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__ZP_OFFSET__ .set $75

; There's a consistent difference of $12 between X and Y; this consistent distancing is meant to be maintained, so leave it alone!

	ZP Player_XHi, 1	; Player X Hi 
	ZP Objects_XHi, 8	; $76-$7D Other object's X Hi positions

__ZP_OFFSET__ .set __ZP_OFFSET__ + 1   ; $7E unused (need to maintain X and Y $12 difference)

	; Reuse of $7F
	ZP_NOINC CineKing_DialogState	; Toad & King Cinematic: When 1, we're doing the text versus the dialog box itself

	; NOTE!! This object var is OBJECT SLOT 0 - 4 ONLY!
	ZP Objects_Var4, 5	; $7F-$83 Generic variable 4 for objects SLOT 0 - 4 ONLY

    ; NOTE, the following two are also $84/$85
	ZP_NOINC Level_GndLUT_Addr
	; Pipe_PlayerX/Y variables in use when traveling through pipes
    ZP Pipe_PlayerX, 1	; Stores Player's X when they went into pipe (non-transit)
	ZP Pipe_PlayerY, 1	; Stores Player's Y when they went into pipe (non-transit, aligned to nearest 16, minus 1)

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	ZP Player_YHi, 1	; Player Y Hi
	ZP Objects_YHi, 8	; $88-$8F Other object's Y Hi positions
	ZP Player_X, 1	; Player X
	ZP Objects_X, 8	; $91-$98 Other object's X positions

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	; Reuse of $9A
	ZP_NOINC CineKing_Var		; General variable

	ZP Objects_Var5, 8	; $9A-$A1 Generic variable 5 for objects
	ZP Player_Y, 1	; Player Y
	ZP Objects_Y, 8	; $A3-$A9 Other object's Y positions

	ZP Player_SpriteX, 1	; Player's sprite X
	ZP Objects_SpriteX, 8	; $AC-$B3 Other object's sprite X positions
	ZP Player_SpriteY, 1	; Player's sprite Y
	ZP Objects_SpriteY, 8	; $B5-$BC Other object's sprite Y positions
	; WARNING: The distance between Player/Objects_XVel and Player/Objects_YVel must be same as Player/Objects_X/YVelFrac!
	ZP Player_XVel, 1	; Player's X Velocity (negative values to the left) (max value is $38)
	ZP Objects_XVel, 8	; $BE-$C5 Other object's X velocities

	ZP Objects_VarBSS, 7	; $C6-$CC OBJECT SLOTS 0 - 5 ONLY ... uncleared var??
	ZP SlotIndexBackup, 1	; Used as a backup for the slot index (e.g. current object, current score, etc.)
	ZP Player_HaltGame, 1	; Player is halting game (e.g. dying, shrinking/growing, etc.)

	; WARNING: The distance between Player/Objects_XVel and Player/Objects_YVel must be same as Player/Objects_X/YVelFrac!
	ZP Player_YVel, 1	; Player's Y Velocity (negative values upward)
	ZP Objects_YVel, 8	; $D0-$D7 Other object's Y velocities

	ZP Player_InAir, 1	; When set, Player is in the air

	; Reuse of $D9
	ZP_NOINC CineKing_Frame2		; Used only by the World 6 King (Seal juggling a crown, the crown's frame)

	; Objects_DetStat:
	; Object's detection bits:
	;	$01-hit wall right
	;	$02-hit wall left
	;	$04-hit ground
	;	$08-hit ceiling
	;	$80-object touching "32 pixel partition" floor (if active)
	ZP Objects_DetStat, 8	; $D9-$E0  on screen

	ZP Player_SprWorkL, 1	; Sprite work address low
	ZP Player_SprWorkH, 1	; Sprite work address high

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	ZP Level_TileOff, 1	; Tile mem offset
	ZP Level_Tile, 1	; Temporary holding point for a detected tile index
	ZP Player_Slopes, 3	; for sloped levels only (3 bytes allocated, but only one actually used)
				; *NOTE: Code at PRG030_9EDB clears Player_Slopes+1 and Player_Slopes+2, but these are never used!

__ZP_OFFSET__ .set __ZP_OFFSET__ +  2

	ZP Player_XStart, 1	; Set to Player's original starting X position (also used to check if level has initialized)

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

; Player_Suit -- Player's active powerup (see also: Player_QueueSuit)
	ZP Player_Suit, 1

	ZP Player_Frame, 1	; Player display frame
	ZP Player_FlipBits, 1	; Set to $00 for Player to face left, Set to $40 for Player to face right

	ZP Player_WagCount, 1	; after wagging raccoon tail, until this hits zero, holding 'A' keeps your fall rate low
	ZP Player_IsDying, 1	; 0 = Not dying, 1 = Dying, 2 = Dropped off screen, 3 = Death due to TIME UP

__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

	ZP Obj01_Flag, 1	; Not sure what Obj01 is!! This blocks its left/right handler logic.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM: WORLD MAP CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__ZP_OFFSET__ .set $75    ; $75-$F3 is available for this context-dependent situation

    ZP World_Map_Y, 2   ; $75-$76 (Mario/Luigi) Y pixel coordinate position of Mario on world map
    ZP World_Map_XHi, 2   ; $77-$78 (Mario/Luigi) X pixel (hi byte) coordinate position of Mario on world map
    ZP World_Map_X, 2   ; $79-$7A (Mario/Luigi) X pixel (lo byte) coordinate position of Mario on world map
    ZP World_Map_Move, 2   ; $7B-$7C (Mario/Luigi) Movement left in specified direction (even numbers only!)
    ZP World_Map_Dir, 2   ; $7D-$7E (Mario/Luigi) Specified travel direction (8=Up, 4=Down, 2=Left, 1=Right)

    ZP Map_UnusedPlayerVal, 2   ; $7F-$80 (Mario/Luigi) Set for each Player to $20 when returning to map, but apparently unused otherwise!

__ZP_OFFSET__ .set __ZP_OFFSET__ +  3

    ZP Map_UnusedPlayerVal2, 2   ; $84-$85 (Mario/Luigi) Apparently unused at all, but backed up and persisted on the world map

; All the WarpWind vars are shared with the HandTrap; they share code, too...
    ZP Map_WWOrHT_Y, 1   ; Warp Whistle wind or Hand Trap Y position
    ZP Map_HandTrap_XHi, 1   ; Hand Trap X Hi (most vars are shared with warp wind, but technically not this one!)
    ZP Map_WWOrHT_X, 1   ; Warp Whistle wind or Hand Trap X position
    ZP Map_WWOrHT_Cnt, 1   ; Warp Whistle wind or Hand Trap counter
    ZP Map_WWOrHT_Dir, 1   ; Direction the Warp Whistle wind travels (0 = right, 1 = left)

    ; Double use
    ZP_NOINC Map_WarpWind_FX        ; 1 - 4 is the warp whistle effect
    ZP Map_StarFX_State, 1   ; 0 - 2 NOTE: Shared with Map_WarpWind_FX

    ZP World_Map_Twirl, 1   ; If set, Mario is "twirling"
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

    ; When Player is "skidding" backward (from death or "twirling" from game over continuation)
    ZP Map_Skid_DeltaY, 1   ; Delta applied directly to Y
    ZP Map_Skid_DeltaFracY, 1   ; Fractional delta Y
    ZP Map_Skid_FracY, 1   ; Fractional Y accumulator
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

    ZP Map_Skid_DeltaX, 1   ; Delta applied directly to X
    ZP Map_Skid_DeltaFracX, 1   ; Fractional delta X
    ZP Map_Skid_FracX, 1   ; Fractional X accumulator
    ZP Map_Skid_FracCarry, 1   ; Fractional carry over accumulator (I think?)
    ZP Map_Skid_Count, 1   ; Just a ticker controlling the display frame of the twirl
    ZP Map_Skid_Counter, 1

    ; Map_Skid_TravDirs -- specifies which way Player must "twirl" to get to the destination
    ; Bit 0 Set = Player must travel to the right versus the left
    ; Bit 1 Set = Player must travel downward versus upward
    ZP Map_Skid_TravDirs, 1
__ZP_OFFSET__ .set __ZP_OFFSET__ +  2

    ZP Map_StarsX, 8   ; $9B-$A2 During World Intro, X position of each star
    ZP Map_StarsY, 8   ; $A3-$AA During World Intro, Y position of each star
    ZP Map_StarsOutRad, 1   ; During World Intro, stars take off radius (0 = smallest, increments for larger)
__ZP_OFFSET__ .set __ZP_OFFSET__ +  3

    ZP Map_StarsXSteps, 1   ; During World Intro, number of "steps" remaining in the X position adjustment
    ZP Map_StarsRadCnt, 1   ; During World Intro, adds $70 per display frame and adds 1 to the radius when it overflows
    ZP Map_StarsCenterX, 1   ; During World Intro, X center of stars
    ZP Map_StarsCenterY, 1   ; During World Intro, Y center of stars
    ZP Map_StarsDeltaR, 1   ; During World Intro, delta to the star radii
    ZP Map_StarsConst9, 1   ; During World Intro, ... Constant 9?
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

    ZP Map_StarsAnimCnt, 1   ; During World Intro, a simple counter that adds 32 per frame and toggles Map_StarsFrame when it overflows
    ZP Map_StarsFrame, 1   ; During World Intro, "frame" of stars (0/1)
    ZP Map_StarsPattern, 1   ; During World Intro, stars current VROM pattern
    ZP Map_StarsLandRad, 1   ; During World Intro, stars landing radius (0 = largest, increments for smaller)
    ZP Map_StarsYSteps, 1   ; During World Intro, number of "steps" remaining in the Y position adjustment
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

    ZP Map_StarsRadius, 8   ; $BC-$C3 During World Intro, each star's "radius" position (each radius position is 0-31)
    ZP Map_StarsState, 1   ; 0 = Stars coming out from center, 1 = Stars moving in towards Player start
    ZP Map_SkidBack, 1   ; Player is skidding back (Map_Player_SkidBack stores whether they skidded on their last turn at all)
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1

    ZP Map_UnusedGOFlag, 1   ; Set at map initialization or if Player gets Game Over and selects CONTINUE/END, no apparent purpose
__ZP_OFFSET__ .set __ZP_OFFSET__ +  4

    ZP Map_Intro_CurStripe, 1   ; Current stripe of the "World X" intro box to be erased (0 - 7)
    ZP Map_Intro_NTOff, 1   ; Offset into nametable for erasing the "World X" intro box
    ZP Map_Intro_ATOff, 1   ; Offset into the attribute table for erasing the "World X" intro box

    ZP Map_Airship_DC, 1   ; set to 1 when the Airship knows where it's going
    ZP Map_Airship_DY, 1   ; Airship delta between current and target Y coordinate
    ZP Map_Airship_YNib, 1   ; Map_Airship_DY shifts out its lower 4 bits as upper 4 bits to this value
    ZP Map_Airship_YAcc, 1   ; Additional Y accumulator when traveling
    ZP Map_Airship_DXHi, 1   ; Airship delta between current and target X Hi coordinate
    ZP Map_Airship_DX, 1   ; Airship delta between current and target X coordinate
    ZP Map_Airship_XNib, 1   ; Map_Airship_DXHi/Map_Airship_DX shifts out its lower 4 bits as upper 4 bits to this value
    ZP Map_Airship_Dir, 1   ; Airship horizontal travel direction in bit 0, vertical direction in bit 1
    ZP Map_HideObj, 1   ; used for completion)

    ZP MapPoof_Y, 1   ; When using a power-up, "poof" appears at this Y coordinate
    ZP MapPoof_X, 1   ; When using a power-up, "poof" appears at this X coordinate
    ZP Map_UseItem, 1   ; Flag to signal that item is to be used
__ZP_OFFSET__ .set __ZP_OFFSET__ +  10

    ZP World_Map_Tile, 1   ; Current tile index Mario is standing on
__ZP_OFFSET__ .set __ZP_OFFSET__ +  3

    ZP Scroll_Temp, 1   ; Scroll hold value
__ZP_OFFSET__ .set __ZP_OFFSET__ +  2

    ZP Player_WalkFrame, 1   ; relative, not the same as Player_Frame
__ZP_OFFSET__ .set __ZP_OFFSET__ +  7


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM: TITLE SCREEN / ENDING CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__ZP_OFFSET__ .set $75    ; $75-$F3 is available for this context-dependent situation

; Title screen "objects", which includes Mario, Luigi, and the assortment of other things
; The following are the offsets from any of the object arrays:
; 0 = Mario, 1 = Luigi, 2 = Starman, 3 = Mushroom, 4 = Super Leaf, 5 = Goomba, 6 = Buzzy Beatle, 7 = Koopa shell

; Note that some of this is used for the engine (especially in the Princess's chamber) but some of it is
; different (especially during the montage) so consider the overlapped variables in the next section

    ZP Title_XPosHi, 8   ; $75-$7C "High" part of the extended precision X position for all objects
    ZP Title_YPosHi, 8   ; $7D-$84 "High" part of the extended precision X position for all objects
    ZP Title_ObjX, 8   ; $85-$8C Title screen object X positions
    ZP Title_ObjY, 8   ; $8D-$94 Title screen object Y positions
    ZP Title_ObjXVel, 8   ; $95-$9C X velocities of title screen objects (4.4FP)
    ZP Title_ObjYVel, 8   ; $9D-$A3 Y velocities of title screen objects
    ZP Title_XPosFrac, 8   ; $A5-$AC X position extended precision of objects (provides 4-bit fixed point)
    ZP Title_YPosFrac, 8   ; $AD-$B4 Y position extended precision of objects (provides 4-bit fixed point)
    ZP Title_ObjYVelChng, 2   ; $B5-$B6 Mario / Luigi change in Y velocity flag
    ZP Title_ObjMLFlags, 2   ; $B7-$B8 Mario / Luigi Sprite flags
    ZP Title_ObjMLMoveDir, 1   ; 0 = No move, 1 = Left, 2 = Right
__ZP_OFFSET__ .set __ZP_OFFSET__ +  1
    ZP Title_ObjMLAnimFrame, 2   ; $BB-$BC Mario / Luigi animation frame
    ZP Title_ObjMLDirTicks, 2   ; $BD-$BE Mario / Luigi animation ticks
    ZP Title_ObjMLSprite, 2   ; $BF-$C0 Mario / Luigi next sprite to display
    ZP Title_ObjMLPower, 2   ; $C1-$C2 Mario / Luigi current powerup (0 = Small, 1 = Big, 2 = Leaf)
    ZP Title_ObjMLSprRAMOff, 2   ; $C3-$C4 Mario / Luigi Defines a Sprite_RAM offset for Mario / Luigi
    ZP Title_ObjMLSprVis, 2   ; $C5-$C6 Mario / Luigi sprite sliver visibility bits (generated by Title_MLDetermineSpriteVis)
    ZP Title_ObjMLTailTick, 2   ; $C7-$C8 Mario / Luigi tail wagging tick
    ZP Title_ObjMLHold, 2   ; $C9-$CA Mario / Luigi holding something flag (when non-zero)
    ZP Title_ObjMLBonkTick, 2   ; $CB-$CC Mario / Luigi use "bonked" frame while > 0
    ZP Title_ObjMLKickTick, 2   ; $CD-$CE Mario / Luigi use kicking frame while > 0
    ZP Title_ObjMPowerDown, 1   ; Mario power down animation counter
    ZP Title_ObjMLStop, 1   ; Flag used briefly to "hold" Mario/Luigi from moving so they get a "running start"
    ZP Title_CurMLIndex, 1   ; 0 for Mario, 1 for Luigi
    ZP Title_ObjFlags, 6   ; $D2-$D7 Minor objects' sprite flags
    ZP Title_ObjStates, 6   ; $D8-$DD Title screen array of states for the individual objects (NOT including Mario/Luigi)
    ZP Title_State, 1   ; 00 = Prior to red curtain rise, 01 = Rising curtain...
    ZP Title_ResetCnt, 1   ; Title reset counter -- when on the menu, once this hits zero, the title sequence restarts
    ZP Title_ResetCnt2, 1   ; when this goes to zero, it decrements Title_ResetCnt
    ZP Title_ResetTrig, 1   ; when non-zero, resets title screen
    ZP Title_UnusedFlag, 1   ; doesn't seem to do anything useful but not do the "skip" state if Player presses START early on the title screen
    ZP Title_Ticker, 1   ; Tick counter for title screen intro "movie"
    ZP Title_MActScriptPos, 1   ; Offset within Mario's action script
    ZP Title_LActScriptPos, 1   ; Offset within Luigi's action script
    ZP Title_MActScriptDelay, 1   ; Mario's action script delay until next event
    ZP Title_LActScriptDelay, 1   ; Luigi's action script delay until next event
    ZP Title_MActScriptDirSet, 1   ; Mario's action script Buffer for last queue command (sets respective "Title_ObjMLDir" variable)
    ZP Title_LActScriptDirSet, 1   ; Luigi's action script Buffer for last queue command (sets respective "Title_ObjMLDir" variable)
    ZP Title_ObjMLDir, 2   ; $EA-$EB Mario / Luigi vector direction bitfield (1 = Left, 2 = Right, 4 = Down, 8 = Up, $10 = Sprite behind BG, $80 = Tail wagging)
    ZP Title_ObjMLQueue, 2   ; $EC-$ED Mario / Luigi queue to do something ($04 = Luigi's rebound off Mario, $10 = Kick shell, $20 = Begin carrying, $40 = Clear carry/bonk, do kick)
    ZP Title_EventIndex, 1   ; Title background event index (dynamic jump index for events on the title
    ZP Title_EventGrafX, 1   ; Title background current graphic index to load (loads items from Video_Upd_Table2 in PRG025)
    ZP Title_ObjInitIdx, 1   ; Current title screen "event" ID during the intro scene with Mario and Luigi
    ZP Title_ObjInitDly, 1   ; Timer count before next object init
    ZP Title_3GlowFlag, 1   ; When non-zero, begins the "glowing" effect for the big '3'
    ZP Title_3GlowIndex, 1   ; Index into an array of colors to cause the big '3' on the title screen to glow

; Ending-specific vars -- NOTE that Ending system uses some of the Title Screen code, so these variables overlap some of the above
; Basically don't assume anything here is free space without consulting above as well...

__ZP_OFFSET__ .set $75
    ZP Ending2_PicState, 1   ; Ending part 2 picture loader state
    ZP Ending2_ClearLen, 1   ; Length of clear run
    ZP Ending2_ClearPat, 1   ; Pattern to clear the screen with
    ZP Ending2_PicVRAMH, 1   ; Ending part 2 picture VRAM Hi
    ZP Ending2_PicVRAML, 1   ; Ending part 2 picture VRAM Hi
    ZP Ending2_QCmdEnd, 1   ; Ending2_QueueCmd is incremented to this point
    ZP Ending2_FadeTimer, 1   ; Timer which controls the speed of the fade between worlds
    ZP Ending2_QueueCmd, 1   ; incremented after posting, up to Ending2_QCmdEnd
    ZP Ending2_TimerH, 1   ; Ending part 2 timer "high" part
    ZP Ending2_TimerL, 1   ; Ending part 2 timer "low" part
    ZP Ending2_CurWorld, 1   ; Current world we're showing (8 = THE END)

__ZP_OFFSET__ .set $D2
    ZP Ending_Timer, 2   ; $D2-$D3 Twin ending timers, generally one for Mario and one for Princess
    ZP EndText_Timer, 1   ; Timer used for the ending text display
    ZP Ending_State, 1   ; Current state value for initial part of ending (the princess, prior to curtain)

    ZP EndText_VL, 1   ; Princess speech VRAM Address Low
    ZP EndText_VH, 1   ; Princess speech VRAM Address High
    ZP EndText_CPos, 1   ; Princess speech Character Position
    ZP EndText_State, 1   ; Princess speech state variable

__ZP_OFFSET__ .set $F4
    ZP Ending2_IntCmd, 1   ; used during ending to buffer out the ending picture data on the interrupt.  Triggers "Do_Ending2_IntCmd" in PRG024 in interrupt context.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM: BONUS GAME CONTEXT (see PRG022 for lots more info)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__ZP_OFFSET__ .set $75    ; $75-$F3 is available for this context-dependent situation

__ZP_OFFSET__ .set __ZP_OFFSET__ +  22

    ZP BonusCoins_State, 1
    
__ZP_OFFSET__ .set __ZP_OFFSET__ +  59

    ZP BonusDie_Y, 1   ; UNUSED Bonus Game Die (1-6) Y position
    ZP BonusDie_X, 1   ; UNUSED Bonus Game Die (1-6) X position
    ZP BonusDie_YVel, 1   ; UNUSED Bonus Game Die Y Velocity (when it departs)
    ZP BonusDie_YVelFrac, 1   ; UNUSED Bonus Game Die Y Velocity fractional accumulator

__ZP_OFFSET__ .set __ZP_OFFSET__ +  41

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ZERO PAGE RAM: 2P VS CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__ZP_OFFSET__ .set $75    ; $75-$F3 is available for this context-dependent situation

    ZP Vs_State, 1   ; 2P Vs Mode state
    ZP Vs_IsPaused, 1   ; If set, 2P Vs is paused

__ZP_OFFSET__ .set __ZP_OFFSET__ +  125
