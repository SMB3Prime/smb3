.ifndef __RAM_INC__
__RAM_INC__ = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Note that we have these "allocator" macros instead of using .res or .org
; to build up the variables. This makes for the cleanest solution where
; the linker is in charge of placing all variables in each segment and
; we don't break things unknowingly by having the linker place something
; in a segment that's based at a different place than where we thought
; a variable was defined (e.g. if we used .org)
; If an address is truly -unused-, we can just remove it. However, there
; are places where the code assumes certain variables overlap and that
; certain variables won't be used in certain contexts, so we still need
; those variables to align properly. In these cases, we manually increase
; or set __RAM_OFFSET__
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.include "ram_zp.inc"
.include "ram_stack.inc"
.include "ram_oam.inc"
.include "ram_prg.inc"

.segment "RAM"

; =====================================
; RAM linear allocator
; =====================================
__RAM_OFFSET__ .set $0
.macro RAM_NOINC name
    name = RAM_BASE + __RAM_OFFSET__
.endmacro
.macro RAM name, size
    .if (__RAM_OFFSET__ + (size)) > $800
        .error "RAM overflow name"
    .endif
    name = RAM_BASE + __RAM_OFFSET__
    __RAM_OFFSET__ .set __RAM_OFFSET__ + (size)
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $03xx RAM (Largely graphics updating / control)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    RAM Graphics_BufCnt, 1   ; first byte holds current position within buffer (Graphics_Buffer+) to store info
    RAM Graphics_Buffer, 107 ; $0301-$036B Simple (and small!) delayed write buffer; uses same format as Video_Upd_Table in PRG030, get format info there
    RAM TileChng_VRAM_H, 1   ; High part of VRAM address to change
    RAM TileChng_VRAM_L, 1   ; Low part of VRAM address to change
    RAM TileChng_Pats, 4   ; $036E-$0371 The four patterns required to change a tile (for Level_ChgTileEvent)
    RAM Level_SizeOrig, 1   ; Holds original size (width or height) of level (in screens)
    RAM Level_PipeExitDir, 1   ; Direction Player is going to exit from a pipe (1 = Up, 2 = Down, 3 = Right, 4 = Left, 5 = In-level Transit)
    RAM Level_7VertCopy, 1   ; Just seems to be an unmaintained copy of Level_7Vertical from level load, but that's it
    RAM Level_PipeNotExit, 1   ; If set, pipes do NOT exit to map (i.e. as in pipe junctions)
    RAM Level_PauseFlag, 1   ; Set to 0 when not paused, or 1 when paused
    RAM Level_SkipStatusBarUpd, 1   ; When set, skips updating the status bar for one frame (priority graphics buffer changes I think)
    RAM Raster_State, 1   ; This variable's meaning depends on the Raster_Effect in use; typically 0 is first pass, then more for further scanlines

__RAM_OFFSET__ .set __RAM_OFFSET__ + 7

    RAM Scroll_ToVRAMHi, 1   ; High byte for when pushing a column of tile data to VRAM (Set to $20, Name Table 0, after scroll update)

    ; $381 dual use
    RAM_NOINC Scroll_LastCol8        ; Last 8x8 block column that was updated (non-vertical level ONLY)
    RAM Scroll_LastOff8, 1   ; Last 8x8 block offset that was updated (vertical level style ONLY)

    RAM Scroll_PatStrip, 54  ; $0382-$03B7 (may be less?) This stores a strip of 8x8 blocks, (non-vertical level: top to bottom with 2 columns), to render the next 16 pixel "sliver" of screen space
    RAM Scroll_ToVRAMHA, 1   ; High byte for when pushing attribute data to VRAM (Set to $23, attribute table 0)
    RAM Scroll_LastAttr, 1
    RAM Scroll_AttrStrip, 14  ; $03BA-$03C7 (may be less?) This stores a 2x2 block of attribute coloring data
    RAM World_Num_Debug, 1   ; When debug mode is activated (KKKZSPIU), this is the world you select to start on
    RAM Map_StarsDeltaX, 1   ; Delta the stars move in X to reach the Player (always positive, code figures direction)
    RAM Map_StarsDeltaY, 1   ; Delta the stars move in Y to reach the Player (always positive, code figures direction)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 16

    RAM Map_Stars_PRelX, 1   ; During world intro, screen relative position of Player X
    RAM Map_Stars_PRelY, 1   ; During world intro, screen relative position of Player Y
    RAM Player_Power, 1   ; >>>>>>[P] charge level ($7F max)

    ; Level_JctCtl is configured when you enter a door or a pipe
    ; * When $80, use current values for Level_AltLayout and Level_AltObjects
    ; * When otherwise non-zero (inc $80), skips setting vertical start position
    ;
    ; Normal values for the junction:
    ;  0 - Disables junction (i.e. no junction occurring)
    ;  1 - Junction initialization
    ;  2 - Big Question Block bonus area
    ;  3 - General purpose junction (as specified by Level_AltLayout and Level_AltObjects)
    ;  4 - Generic level exit (where you come up from a pipe in the generic exit area)
    ;  5 - Special Toad House (used for the 1-3 warp whistle)
    RAM Level_JctCtl, 1

    RAM Level_JctFlag, 1   ; Toggles when you junction

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Map_DrawPanState, 1   ; Map draw/pan state
    RAM ObjGroupRel_Idx, 1   ; Holds relative index of object within its group (see PRG000_CA51)
    RAM InvFlip_VAddrHi, 1   ; Hi byte of VRAM address during inventory flip modifications

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    ; CLOSING
    ;  0: Erase old status bar complete, palette changed to blue
    ;  1: Draw top bar (at middle)
    ;  2: Draw bottom bar (at middle)
    ;  3: Erase top bar (at middle)
    ;  4: Draw top middle row of normal status bar/cards
    ;  5: Draw bottom middle row of normal status bar/cards
    ;  6: Draw top bar (at top)
    ;  7: Draw bottom bar (at bottom)

    ; OPENING
    ;  8: Erase old status bar complete, palette changed to yellow
    ;  9: Draw top bar (at middle)
    ; 10: Draw bottom bar (at middle)
    ; 11: Erase top bar (at middle)
    ; 12: Erase bottom bar (at middle)
    ; 13: Draw top middle row of inventory
    ; 14: Draw bottom middle row of inventory
    ; 15: Draw top bar (at top)
    ; 16: Draw bottom bar (at bottom)
    RAM InvFlip_Frame, 1   ; Sort of a "frame" of animation during flipping of the inventory box; 0-7 during closing, 8-15 during opening
    RAM InvFlip_Counter, 1   ; 0 = Closed, 4 = Fully Open
    RAM InvStart_Item, 1   ; Starting inventory item; typ. $00, $07, $0E, $15 (Rows 1-4)
    RAM InvHilite_X, 1   ; Current hilite position X coordinate
    RAM InvHilite_Item, 1   ; Which item in the current row is highlighted, 0-6

    RAM THouse_ID, 1   ; An unused ID value that would track boxes already opened in a Toad House (concept unused!)

    ; Treasure type you'll get at this Toad House
    ; 0 = INVALID
    ; 1 = Warp Whistle
    ; 2 = P-Wing
    ; 3 = Frog
    ; 4 = Tanooki
    ; 5 = Hammer
    ; 6 = Random Super Suit
    ; 7 = Random Basic Item
    RAM THouse_Treasure, 1

    RAM Coins_Earned, 1   ; A "buffer" of coins earned to be added to your total, actual coinage stored in Inventory_Coins[2]
    RAM Map_Powerup_Poof, 1   ; Counter that handles the "poof" effect when a powerup is used on the map (requires Inventory to be open, and forces it to close afterward)

    ; Level_FreeVertScroll
    ; 0 = Screen locked at $EF (lowest point) unless flying or climbing a vine
    ; 1 = Free vertical scroll
    ; 2 = Locked at arbitrary point (i.e. whatever Vert_Scroll is, it stays there)
    RAM Level_FreeVertScroll, 1

    RAM Level_7Vertical, 1   ; Set in World 7 vertical type levels
    RAM Level_SelXStart, 1   ; Selects X starting position when level begins (valid values 0-3)

UPDATERASTER_32PIXPART  = 1 ; 32 pixel partition; common use is for levels with water along the bottom
UPDATERASTER_SPADEGAME  = 2 ; Spade game sliders
UPDATERASTER_WATERLINE  = 3 ; "Water line" mode (described at ObjHorzAutoScroller_Init)
UPDATERASTER_32PIXSHOWSPR= $80  ; If NOT set, hides sprites that fall beneath the partition (i.e. for fixed water effect)
    RAM Update_Request, 1   ; This changes the current Raster_Effect and Update_Select and doesn't persist
    RAM Map_Starman, 1   ; Player used a Starman!
    RAM Map_Power_Disp, 1   ; This is the powerup currently DISPLAYED on the map; it should be the same as $0746 World_Map_Power, except for Judgem's Cloud
    RAM Map_Warp_PrevWorld, 1   ; The world you're coming FROM when warping (also used as output from warp zone what world you're going to)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $04xx WORLD MAP CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; $0400-$04CF (except $0461 and $0462, see "$04xx RAM SOUND/MUSIC ENGINE") is available for this context-dependent situation
    ; $0400-$0443 unused in either of the following cases

    ; Setting __RAM_OFFSET__ because of major overlap, but be careful!

__RAM_OFFSET__ .set $147; $0447

; W8D = World 8 Darkness; overlaps the vars used by the entrance transition
    RAM Map_W8D_VAddrH, 1
__RAM_OFFSET__ .set __RAM_OFFSET__ + 3
    RAM Map_W8D_VAddrL, 1
__RAM_OFFSET__ .set __RAM_OFFSET__ + 5
    RAM Map_W8D_VAddrH2, 1
__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    RAM Map_W8D_VAddrL2, 1
__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    RAM Map_W8D_TileOff, 1   ; In-tile offset (0 = upper left, 1 = lower left, 2 = upper right, 3 = lower right)
__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM Map_W8D_YOff, 1   ; Y Offset from Player when doing darkness
    RAM Map_W8D_XOff, 1   ; X Offset from Player when doing darkness
    RAM Map_W8D_RC, 1   ; Row in the upper bits, column in the lower bits
    RAM Map_W8D_Dir, 1   ; Direction of travel in darkness (1 = Right, 2 = Left, 4 = Down, 8 = Up)
    RAM Map_W8D_X, 1
    RAM Map_W8D_Y, 1
    RAM Map_W8D_Idx, 1

__RAM_OFFSET__ .set  $144; $0444

    ; Entrance transition; overlaps with above
    ; NOTE: Memory is cleared from here to +$1C, $460
    ; For border arrays, 0-3: Top 0, bottom 1, right 2, left 3
    RAM Map_EntTran_VLHalf, 1   ; When 1, offset starts vertically "lower half" (at $F0 of first screen)
    RAM Map_EntTran_TBCnt, 1   ; decreasing counter as the transition moves inward top/bottom
    RAM Map_EntTran_LRCnt, 1   ; decreasing counter as the transition moves inward left/right
    RAM Map_EntTran_BVAddrH, 4   ; $0447-$044A Each border's current high part of VRAM address
    RAM Map_EntTran_BVAddrL, 4   ; $044B-$044E Each border's current low part of VRAM address
    RAM Map_EntTran_BorderLoop, 1   ; Border loop counter
    RAM Map_EntTran_Cnt, 1
    RAM Map_EntTran_VAddrH, 1   ; high part of VRAM address to modify
    RAM Map_EntTran_VAddrHAdj, 1   ; An adjusted version of the high address as needed to wrap (used in removed "box out" version only)
    RAM Map_EntTran_VAddrL, 1   ; low part of VRAM address to modify
    RAM Map_EntTran_TileOff, 1   ; Offset into tile memory (used in removed "box out" version only)
    RAM Map_EntTran_Tile8x8, 1   ; Offset to which 8x8 pattern of the tile we're grabbing that we need (used in removed "box out" version only)
    RAM Map_EntTran_VRAMGap, 1   ; Sets gap (i.e. 1 for vertical, 32 for horizontal; used in removed "box out" version only)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 8   ; $0457-$045E unused

    RAM Map_EntTran_Temp, 1   ; Seems to me a multi-purpose value in entrance transition
    RAM Map_EntTran_InitValIdx, 1   ; Selects an index of values to initialize by


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $04xx BONUS GAME CONTEXT (see PRG022 for lots more info)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $100  ; $0400-$04CF (except $0461 and $0462, see "$04xx RAM SOUND/MUSIC ENGINE") is available for this context-dependent situation
    ; WARNING: $0400-$04CF gets cleared at end of bonus game!

    RAM Roulette_Pos, 3   ; $0400-$0402 horizontal position of each row
    RAM Roulette_PosHi, 3   ; $0403-$0405 Hi part of Roulette_Pos
    RAM Roulette_ShapeLock, 3   ; $0406-$0408 Locked position of each row (0 = Mushroom, 1 = Flower, 2 = Mushroom, 3 = Star)
    RAM Roulette_ConfigRun, 1   ; 0 = Configuring, 1 = Running
    RAM Roulette_ConfigState, 1   ; Early configuration state
    RAM Roulette_RunState, 1   ; Running state of game

    ; NOTE: Watch the reuse here...
    RAM_NOINC Roulette_StopState     ; $040C-$040E Current "state" of each row while it is coming to a stop (1+)
    RAM Roulette_xUpY, 1   ; "x Up" display Y position (overlaps first byte of Roulette_StopState)
    RAM Roulette_LivesToGive, 1   ; Lives left to give in reward (overlaps second byte of Roulette_StopState)
__RAM_OFFSET__ .set __RAM_OFFSET__ + 1   ; Third byte of Roulette_StopState

    RAM Roulette_Speed, 3   ; $040F-$0411 Movement speed of each row (4.4FP)
    RAM Roulette_StopCnt, 3   ; $0412-$0414 Decrements to zero while each row is stopping
    RAM Roulette_PosFrac, 3   ; $0415-$0417 Fractional accumulator for position of row 1-3
__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Roulette_Turns, 1   ; UNUSED "Turns Remaining" (if > 0, and you lose Roulette, hit a button and spin again!)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 12
    ; UNUSED "Prize" that appears in some varities of the lost bonus games
    ; (Or that's my best guess anyway.)  BMF54123's patch shows these as
    ; Mushroom, Flower, Star, or Judgem's Cloud.  Perhaps it was a situation
    ; like "Here's the prize on the table, if you can get it"...
    RAM_NOINC Card_SelectX           ; Card cursor X (NOTE: Shared with Bonus_PrizeX)
    RAM Bonus_PrizeX, 1   ; UNUSED Prize sprite X

    RAM_NOINC Card_SelectY           ; Card cursor Y (NOTE: Shared with Bonus_PrizeY)
    RAM Bonus_PrizeY, 1   ; UNUSED Prize sprite Y

    RAM_NOINC Card_Index         ; Currently selected card in "N-Spade" Card game (NOTE: Shared with Bonus_CurBufOffset)
    RAM Bonus_CurBufOffset, 1   ; Offset in graphics buffer while generating instruction box

    RAM Card_FirstIndex, 1   ; Card_Index of the first card you flipped (in case you get it wrong on the second...)

    RAM_NOINC Bonus_Round2           ; UNUSED Picks which "Round 2" game would be played (NOTE: Shared with Card_SelectXOrig)
    RAM Card_SelectXOrig, 1   ; Card cursor original X

    RAM Card_MoveDelay, 1   ; Decrements to zero; if not zero, can't move on N-Spade game
    RAM Card_SprRAMOff, 1   ; Sprite RAM offset after cursor is drawn; for drawing card flip
    RAM Card_AnimTick, 1   ; Animation tick, decrements to zero
    RAM Card_VRAM_L, 1   ; Low part of VRAM address of N-Spade card to modify
    RAM Card_VRAM_H, 1   ; High part of VRAM address of N-Spade card to modify

    RAM Card_FlipCount, 1   ; +1 for every card flip
    RAM Card_MatchCard, 1   ; Card to match, i.e. the first card you selected of the pair
    RAM Card_UnusedVL, 1   ; ?? Some VRAM Low; Seems to only be part of an unused routine
    RAM Card_UnusedVH, 1   ; ?? Some VRAM High; Seems to only be part of an unused routine

    ; Bonus_GameHost
    ;   0 = Toad Host: The only one we ever got
    ; All others are UNUSED...
    ;   1 = Koopa Troopa Host + "Prize" Game
    ;   2 = Koopa Troopa Host, no "Prize" Game
    ;   3 = Hammer Bro
    RAM_NOINC Bonus_GameHost         ; NOTE: Shared with Card_GameState
    RAM Card_GameState, 1   ; State of N-Spade card game
    RAM Card_TurnsRemain, 1   ; Number of turns remaining
    RAM Card_FlipFrame, 1   ; Frame of flipping card

; The next three vars belong to an unused routine @ PRG022:DA62; unclear what it did exactly
    RAM Card_UnusedArray, 4   ; $0437-$043A ??
    RAM Card_UnusedAttr, 1   ; Seems to only be part of an unused routine; attribute table offset of some sort
    RAM Card_UnusedArrIdx, 1   ; Seems to only be part of an unused routine; index for Card_UnusedArrIdx

    RAM Card_Matches, 1   ; Matches (local, this round; Seems to only be used as part of the unused routine)
    RAM Card_CoinsToGive, 1   ; If greater than zero, coins are being given

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    ; Bonus_GameState
    ;
    ; 0 = ?
    ; 1 = Giving instructions die game
    ; 2 = Rotating "die"
    ; 3 = Coin spurt prize
    ; 4 = Stopped die
    ; 5 = Die flies away
    ; 6 = ?? Initialize instructions?
    ; 7 = Giving instructions
    ; 8 = Pause before proceeding
    ; 9 = Exiting
    RAM Bonus_GameState, 1

    RAM Card_InitState, 1   ; Initialization state of N-Spade card game

    ; Bonus_GameType
    ; This appears to determine what game you're about to play.
    ; This also reveals some of the greatest questionable loss in SMB3...
    ; When this value is anything besides 1 or 2, it plays what I SUPPOSE
    ; was a "roulette" game?  With a fixed graphics patch, it appears to
    ; be a die cycling 1-6.
    ;
    ; 0 = Japanese text, translated as:
    ;   If "1" appears, 1 (?)
    ;   If "2" appears, I'll give you a key
    ;   Otherwise, I'll give you coins.
    ; 1 = Standard Spade game (UNUSED BONUS GAMES NOTE: May have once been the "odd" game spoken of in type 5)
    ; 2 = N-Spade (UNUSED BONUS GAMES NOTE: May have once been the "even" game spoken of in type 5)
        ; 3, 1   ; I suspect this is a placeholder ('C' being the 3rd)
        ; 4, 1   ; I suspect this is a placeholder ('D' being the 4th)
    ; 5 = Japanese: "If an odd number appears, I'll let you play the Roulette Game."
    ; 6 = Japanese: "If an even number appears, I'll let you play the Card Game."
    ; 7 = Japanese: "2, return (?)"
    ;

    ; The "unused" ones are just guesses (where even possible) based on the
    ; semi-translated "greetings" for the sake of tracking...
BONUS_UNUSED_KEYCOIN    = 0
BONUS_SPADE     = 1 ; Line up images Spade game
BONUS_NSPADE        = 2 ; Card matching N-Spade game
BONUS_UNUSED_CCCC   = 3
BONUS_UNUSED_DDDD   = 4 ; Unused placeholder (I think), but does actually set something when it exits!
BONUS_UNUSED_ODDROULETTE= 5
BONUS_UNUSED_EVENCARD   = 6
BONUS_UNUSED_2RETURN    = 7 ; MAY have been Koopa Troopa's "Prize" Game...
    RAM Bonus_GameType, 1

    RAM Bonus_KTPrize, 1   ; UNUSED Koopa Troopa's "Prize" Game Prize ID (0 = Mushroom, 1 = Star, 2 = Flower, 3 = Judgem's, by BMF54123's patch)

    ; $0444-$04CF unused in this context (excluding $0461 and $0462, see "$04xx RAM SOUND/MUSIC ENGINE")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $04xx GAMEPLAY CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $100  ; $0400-$04CF (except $0461 and $0462, see "$04xx RAM SOUND/MUSIC ENGINE") is available for this context-dependent situation

__RAM_OFFSET__ .set __RAM_OFFSET__ + 27

    RAM Fade_State, 1   ; 0 = Nothing, 1 = Fade in, 3 = Fade out
    RAM Fade_Tick, 1   ; Ticks down and then decrements Fade_Level
    RAM Fade_Level, 1   ; 4 to 0, fade in level
    RAM FadeOut_Cancel, 1   ; If set, the next attempted fade out will be cancelled, which then resets this to zero
    RAM Player_AllowAirJump, 1   ; Counts down to zero, but while set, you can jump in the air
    RAM Player_XVelAdj, 1   ; Applies additional value to the X Velocity

    RAM_NOINC CineKing_Frame         ; King's animation frame (NOTE: Shared with Objects_Var7 first byte)
    RAM Objects_Var7, 8   ; $0421-$0428 General object variable 7

    ; $0429-$04CF unused in this context (excluding $0461 and $0462, see "$04xx RAM SOUND/MUSIC ENGINE")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $04xx RAM SOUND/MUSIC ENGINE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $161; $0461

; $0461-$0462 are reserved for use by the sound/music engine
; These ought to be moved into the greater range to spare this area...
; $04EB and $04EC are unused, good place for these vars!!

    RAM Level_MusicQueue, 1   ; Requests a song from Set 2A/B (used to allow delayed start)
    RAM Level_MusicQueueRestore, 1   ; What to "restore" the BGM to when it changes (e.g. Starman, P-Switch, etc.)

__RAM_OFFSET__ .set $1D0; $04D0

; $04D0-$04FF is reserved for use by the sound/music engine
; Lower ranges are context-dependent

    RAM Music_TriTrkPos, 1   ; Offset of triangle track in currently playing index
    RAM Music_NseTrkPos, 1   ; Offset of noise track in currently playing index
    RAM Music_PCMTrkPos, 1   ; Offset of DMC track in currently playing index
    RAM Music_Sq2RestH, 1   ; Square 2 Track hold for rest values to be applied after each event
    RAM Music_Sq2Rest, 1   ; Square 2 Track "Rest" period (counts down to zero)
    RAM Music_Sq2NoteLen, 1   ; Square 2 Track length of note (counts down to zero, cuts off sound)
    RAM Music_Sq1Rest, 1   ; Square 1 Track "Rest" period (counts down to zero)
    RAM Music_Sq1NoteLen, 1   ; Square 1 Track length of note (counts down to zero, cuts off sound)
    RAM Music_TriRestH, 1   ; Triangle Track hold for rest values to be applied after each event
    RAM Music_TriRest, 1   ; Triangle Track "Rest" period (counts down to zero)
    RAM Music_NoiseRest, 1   ; Noise Track "Rest" period (counts down to zero)
    RAM Music_NseRestH, 1   ; Noise Track hold for rest values to be applied after each event
    RAM Music_DMCRest, 1   ; DMC Track "Rest" period (counts down to zero)
    RAM Music_DMCRestH, 1   ; DMC Track hold for rest values to be applied after each event
    RAM Music_PCMStart, 1   ; Holds the starting offset of the DMC track
    RAM Music_NextIndex, 1   ; Next "index" to be played

    RAM SFX_Counter1, 1   ; Generic purpose SFX counter
    RAM SndCur_Player, 1   ; Currently playing Player sound (blocks music on Square 1)
    RAM SndCur_Level1, 1   ; Currently playing "level 1" sound (blocks music on Square 2)
    RAM SndCur_Level2, 1   ; Currently playing "level 2" sound
    RAM SndCur_Music1, 1   ; Currently playing BGM from Music 1 set
    RAM SndCur_Music2, 1   ; Currently playing BGM from Music 2 set
    RAM SndCur_Map, 1   ; Currently playing map sound (blocks music on Square 1)
    RAM SndCur_Pause, 1   ; Holds copy of Sound_QPause, to play sound while everything else is paused
    RAM SFX_Counter2, 1   ; Generic purpose SFX counter for level sounds
    RAM SFX_Counter3, 1   ; Generic purpose SFX counter
    RAM SFX_Counter4, 1   ; Generic purpose SFX counter
__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM Sound_IsPaused, 1   ; When set, sound processing is PAUSED
__RAM_OFFSET__ .set __RAM_OFFSET__ + 3

; For any of these queues, the value is a bit value, which offers
; a simple prioritization system; lowest value plays over any other

; Queue Player sound effects
SND_PLAYERJUMP  = $01   ; Jump
SND_PLAYERBUMP  = $02   ; Bump
SND_PLAYERSWIM  = $04   ; Swim / Squish
SND_PLAYERKICK  = $08   ; Kick
SND_PLAYERPIPE  = $10   ; Pipe / shrink
SND_PLAYERFIRE  = $20   ; Fireball
SND_PLAYERPOWER = $40   ; Full power ringing (must be constantly set or you don't hear it)
SND_PLAYERFROG  = $80   ; frog hop
    RAM Sound_QPlayer, 1

; Queue Level sound effects 1
SND_LEVELCOIN   = $01   ; Coin
SND_LEVELRISE   = $02   ; Powerup rising from block
SND_LEVELVINE   = $04   ; Vine rising
SND_LEVELBABOOM = $08   ; Cannon fire
SND_LEVELBLIP   = $10   ; Text "type" sound / card select
SND_LEVELPOWER  = $20   ; Power up
SND_LEVEL1UP    = $40   ; 1-up
SND_LEVELPOOF   = $80   ; Lost suit / wand shot
SND_LEVELUNK    = $90   ; Unknown / lost sound
SND_LEVELSHOE   = $A0   ; Lost Kuirbo's Shoe
SND_LEVELTAILWAG= $B0   ; Tail wag
    RAM Sound_QLevel1, 1

; Queue Level sound effects 2
SND_LEVELCRUMBLE= $01   ; Crumbling brick
SND_LEVELFLAME  = $02   ; Flame jet
SND_BOOMERANG   = $04   ; Boomerang
SND_LEVELAIRSHIP= $08   ; Airship fly
SND_LEVELMARCH  = $10   ; Hammer Bros. march around
; $20 - Unused
; $40 - Unused
SND_LEVELSKID   = $80   ; Skid
    RAM Sound_QLevel2, 1


; Queue music request 1
; The following I've grouped into "Set 1" (which play song index 0-7):
MUS1_PLAYERDEATH    = $01   ; Player death
MUS1_GAMEOVER       = $02   ; Game over
MUS1_BOSSVICTORY    = $04   ; Victory normal
MUS1_WORLDVICTORY   = $08   ; Victory super (King reverted, Bowser defeated, etc.)
MUS1_BOWSERFALL     = $10   ; Bowser dramatic falling
MUS1_COURSECLEAR    = $20   ; Course Clear
MUS1_TIMEWARNING    = $40   ; Time Warning (attempts to speed up song playing)
MUS1_STOPMUSIC      = $80   ; Stops playing any music
    RAM Sound_QMusic1, 1

; Queue music request 2
; The following I've grouped into "Set 2A":
MUS2A_WORLD1        = $01   ; World 1
MUS2A_WORLD2        = $02   ; World 2
MUS2A_WORLD3        = $03   ; World 3
MUS2A_WORLD4        = $04   ; World 4
MUS2A_WORLD5        = $05   ; World 5
MUS2A_WORLD6        = $06   ; World 6
MUS2A_WORLD7        = $07   ; World 7
MUS2A_WORLD8        = $08   ; World 8
MUS2A_SKY       = $09   ; Coin Heaven / Sky World / Warp Zone (World 9)
MUS2A_INVINCIBILITY = $0A   ; Invincibility
MUS2A_WARPWHISTLE   = $0B   ; Warp whistle
MUS2A_MUSICBOX      = $0C   ; Music box
MUS2A_THRONEROOM    = $0D   ; King's room
MUS2A_BONUSGAME     = $0E   ; Bonus game
MUS2A_ENDING        = $0F   ; Ending music


; The following I've grouped into "Set 2B":
MUS2B_OVERWORLD     = $10   ; Overworld 1
MUS2B_UNDERGROUND   = $20   ; Underground
MUS2B_UNDERWATER    = $30   ; Water
MUS2B_FORTRESS      = $40   ; Fortress
MUS2B_BOSS      = $50   ; Boss
MUS2B_AIRSHIP       = $60   ; Airship
MUS2B_BATTLE        = $70   ; Hammer Bros. battle
MUS2B_TOADHOUSE     = $80   ; Toad House
MUS2B_ATHLETIC      = $90   ; Overworld 2
MUS2B_PSWITCH       = $A0   ; P-Switch
MUS2B_BOWSER        = $B0   ; Bowser
MUS2B_WORLD8LETTER  = $C0   ; Bowser's World 8 Letter
MUS2B_MASK      = $F0   ; Not intended for use in code, readability/traceability only
    RAM Sound_QMusic2, 1

; Queue map sound effects
SND_MAPENTERWORLD   = $01   ; World begin starry entrance sound
SND_MAPPATHMOVE     = $02   ; Path move
SND_MAPENTERLEVEL   = $04   ; Enter level
SND_MAPINVENTORYFLIP    = $08   ; Flip inventory
SND_MAPBONUSAPPEAR  = $10   ; Bonus appears
; $20: ?? unused ?
; $40: ?? unused ?
SND_MAPDENY     = $80   ; Denied
    RAM Sound_QMap, 1

; Queue pause sound
PAUSE_STOPMUSIC     = $01   ; Pause sound effect (like pressing START, pauses music!)
PAUSE_RESUMEMUSIC   = $02   ; Resume sound (resumes music)
    RAM Sound_QPause, 1

    RAM DMC_Time, 1   ; Time remaining on DMC sound
    RAM Music_Sq1RestH, 1   ; Square 1 Track hold for rest values to be applied after each event
    RAM Music_Sq1AltRamp, 1   ; When Square 1 track encounters a $00 byte, it actives a ramping mode, the value of which is stored here
    RAM Music_LOST4FB, 1   ; AFAIK, value in music engine that is "lost"; nothing seems to set it, and it doesn't do very much (possibly was a note length override??)
    RAM Music_LOST4FC, 1   ; AFAIK, value in music engine that is "lost"; nothing seems to set it, and it doesn't do very much

    RAM Music_RestH_Base, 1   ; Base offset into Music_RestH_LUT
    RAM Music_Sq2TrkOff, 1   ; Offset of square wave 2 track in currently playing index
    RAM Music_Sq1TrkOff, 1   ; Offset of square wave 1 track in currently playing index

; NOTE: CONTEXT -- RAM in the $500-$5FF range changes meaning depending on the "context", i.e. what
; state of the game we're currently in!  This means that variables are defined with overlapping
; addresses, and care must be taken to use the correct labels depending on the code!


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $05xx TITLE SCREEN CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $200  ; $0500-$05FF is available for this context-dependent situation

__RAM_OFFSET__ .set __RAM_OFFSET__ + 16

    ; NOTE: All of this block is cleared by the title screen
    RAM Title_MLAccelCnt, 1   ; constant moving counter which overflows to provide a more "natural" acceleration to the Bros
    RAM Title_MarioPoof, 1   ; countdown of Mario's "poof" when he collects the power up
    RAM Title_MLHoldTick, 1   ; Set to $40 when Mario/Luigi are "held" (count down to zero)
    RAM Title_ObjVar, 6   ; $513-$518 Minor objects user-defined variable 1 (automatically decrements if not zero!)
    RAM Title_ObjVar2, 6   ; $519-$51E Minor objects user-defined variable 2
    RAM Title_ObjFrame, 6   ; $51F-$524 Minor object "frame"; adds 2x the value here to the pattern selection for the object's sprites


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $5xx MAP CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $200  ; $0500-$05FF is available for this context-dependent situation

    ; NOTE: Most of the memory in this space is shared with Gameplay Context
    ; so don't assume that any value that should be spared is safe in here...

    ; NOTE: There are 8 defined "core" map objects, but up to 15 (?) can exist!
    ; *** Index 1 is assumed reserved for the Airship, however...
    RAM Map_Object_ActY, 15  ; $0500-$050E For map objects, their actual display Y coordinate (as it may modify, esp. with marchers)
    RAM Map_Object_ActX, 15  ; $050F-$051D For map objects, their actual display X coordinate (as it may modify, esp. with marchers)
    RAM Map_Object_ActXH, 15  ; $051E-$052C For map objects, their actual display X Hi byte (as it may modify, esp. with marchers)
    RAM Map_Object_Data, 15  ; $052D-$053B For marching objects, 0/1 for their direction, could be anything though
    RAM Map_March_Count, 15  ; $053C-$054A Simply counts and overflows, but used to determine position of marching map objects (PER ID, not object index ... Actually is seems they go back and forth on that??)
__RAM_OFFSET__ .set __RAM_OFFSET__ + 60

    RAM Map_Objects_Vis, 15  ; $0587-$058E Set for map objects as visible, clear if it's not
    RAM Map_MarchInit, 1   ; Set when marching data has been initialized (done once per marching cycle on the map)
    RAM Map_InCanoe_Flag, 1   ; Set when Player is in Canoe (modifies movement, allows movement in water, etc.)
    RAM World_8_Dark, 1   ; Darkness on World 8 Map, level 2 -- counts 0-7 while setting up the effect
    RAM World_Map_AnimF, 1   ; World map animation frame (for bushes, etc.)
    RAM World_Map_AnimT, 1   ; World map animation tick


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $5xx BONUS GAME CONTEXT (see PRG022 for lots more info)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $200  ; $0500-$05FF is available for this context-dependent situation

__RAM_OFFSET__ .set __RAM_OFFSET__ + 231
    RAM BonusText_VH, 1
    RAM BonusText_VL, 1


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $5xx GAMEPLAY CONTEXT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $200  ; $0500-$05FF is available for this context-dependent situation

__RAM_OFFSET__ .set __RAM_OFFSET__ + 16
    ; Event_Countdown is context dependent; without context, does nothing
    ; * When you come out of a pipe, this counter decrements until the pipe should be finished
    ; * When you die, counts down until dropping back to map

    ; NOTE: Event_Countdown and the following 7 are all decremented together!
    RAM Event_Countdown, 1

    RAM Player_TailCount, 1   ; Determines display frame of tail wag
    RAM Player_InAir_OLD, 1   ; Stores backup of Player_InAir
    RAM_NOINC Player_FireCount       ; Player shoots fireball/hammer, sets sprite frame (shared with Player_FrogHopCnt)
    RAM Player_FrogHopCnt, 1   ; Counter used for frog hopping along the ground (shared with Player_FireCount)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Player_PMeterCnt, 1   ; Tick counter used to count when to increase/decrease Power Meter
    RAM B10Coin_Timer, 1   ; Decrements until zero, which is how much time you have to get the max coins from a 10 coin block
    RAM Player_TailAttack, 1   ; Initiailized to $12; counts down to zero, performs tail attack!

    RAM_NOINC CineKing_Timer         ; Timer; decrements to zero (shares Objects_Timer first byte)
    RAM Objects_Timer, 8   ; $0518-$051F "Timer" values; automatically decrements to zero

    ; NOTE: Until Timer2 expires, object will not hit other objects.
    ; Probably used as a dampener to keep an object from slaughtering
    ; another bunch of objects TOO quickly!
    RAM Objects_Timer2, 8   ; $0520-$0527 "Timer" values; automatically decrements to zero

    ; All the Level_BlockChgX/Y values are aligned to nearest 16 (i.e. tile coordinate)
    RAM Level_BlockChgXHi, 1   ; Player X High value when block change was queued
    RAM Level_BlockChgXLo, 1   ; Player X Low value when block change was queued
    RAM Level_BlockChgYHi, 1   ; Player Y High value when block change was queued
    RAM Level_BlockChgYLo, 1   ; Player Y Low value when block change was queued

    ; the block "bounce" that occurs after hitting ? block, music note block, etc.
    RAM Level_BlkBump_Pos, 2   ; $052C-$052D Block bump effect slot "position" (from 10 down, "position" of bounce)
    RAM Level_BlkBump, 3   ; $052E-$0530 Block bump effect slot (use Level_ChgTileEvent value, or 0 for inactive)
    RAM Level_BlkBump_XHi, 3   ; $0531-$0533 Block bump slot X Hi
    RAM Level_BlkBump_XLo, 3   ; $0534-$0536 Block bump slot X Lo
    RAM Level_BlkBump_YHi, 3   ; $0537-$0539 Block bump slot Y Hi
    RAM Level_BlkBump_YLo, 3   ; $053A-$053C Block bump slot Y Lo

__RAM_OFFSET__ .set __RAM_OFFSET__ + 5

    ; The alternate vertical scrolls are used so that raster effects can be properly implemented!
    RAM Level_VertScrollH, 1   ; Alternate VertScroll_Hi used by engine, adjusted before being sent to Vert_Scroll_Hi
    RAM Level_VertScroll, 1   ; Alternate VertScroll used by engine, adjusted before being sent to Vert_Scroll

    RAM Player_AboveTop, 1   ; If Player is above top of level, this is $FF (-1), otherwise it is zero

    RAM_NOINC Level_InitAction       ; AT LEVEL INITIALIZATION ONLY: Performs a specific initialization routine (NOTE: Shared with Player_Slide)
    RAM Player_Slide, 1   ; Positive values sliding forward, negative values sliding backward; directly sets Player_XVel

    RAM Player_UphillFlag, 1   ; When set, Player is walking uphill, and uses speed index value at Player_UphillSpeedIdx

__RAM_OFFSET__ .set __RAM_OFFSET__ + 8

    RAM Player_Flip, 1   ; Invincibility Somersault is used when set; only works in air, reset on ground

    RAM_NOINC Level_AirshipH         ; Height of the airship during the Airship Intro (shared with Player_KuriboDir)
    RAM Player_KuriboDir, 1   ; While Kuribo's shoe is moving: 0 - Not requesting move, 1 - move right, 2 - move left

    RAM Player_Grow, 1   ; Tick counter used to animate growth into Super Mario (20 is starting value, or 2f to shrink)
    RAM Player_FlashInv, 1   ; Player "flashing invincibility" after being hit, counts down to zero
    RAM Player_StarInv, 1   ; Starman Invincibility counter; full/fatal invincibility, counts down to zero
    RAM Player_SuitLost, 1   ; Suit lost to hit discard "poof" counter
    RAM Player_StarOff, 1   ; Starman Invincibility wear-off (the last second or so when it slows and vanishes), counts down to zero
    RAM Player_HaltTick, 1   ; When non-zero, all action halts until this countdown hits zero
    RAM Player_Kick, 1   ; Kick frame in use, counts down to zero
    RAM Player_PipeFace, 1   ; Player forward frame, used for vertical pipe entrance, counts down to zero
    RAM Player_EndLevel, 1   ; Player's end of level run-off until count down to zero (player will actually wrap around horizontally if too large)
    RAM Level_AirshipCtl, 1   ; Airship control -- 1 = run and jump to catch air ship, 2 = climb to enter, 3 = enter

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Counter_Wiggly, 1   ; "Wiggly" counter, provides rippled movement (like the airship rising during its intro)
    RAM Counter_7to0, 1   ; Counter that runs from 7 to 0 continuously while level is in progress

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM LevelPartialInit, 1   ; When set, performs a partial reinitialization of level data (notably does not perform the Level InitAction unless it is airship related)
    RAM Level_TilesetIdx, 1   ; Holds Level_Tileset as an "index" value instead, relative to levels (i.e. Level_Tileset - 1)
    RAM Level_ChangeReset, 1   ; When set to zero, a mass reset is performed (used when changing "scenes" in a single level)
    RAM Level_UnusedFlag, 1   ; Unused; only set in a couple places, but never read back!
    RAM Level_SlopeEn, 1   ; If set, enables slope tiles (otherwise they're considered flat top-only solids)

CHNGTILE_DELETECOIN = $01
CHNGTILE_DELETETOBG = $02
CHNGTILE_TOGNOTEBLOCK   = $03   ; miscolored note block
CHNGTILE_TOBOUNCEWOOD   = $04
CHNGTILE_TONOTEBLOCK    = $05
CHNGTILE_COINHEAVEN = $06
CHNGTILE_TOBRICK    = $07
CHNGTILE_TOMETALPLATE   = $08   ; i.e. "plate" that appears after ? block is hit
CHNGTILE_PSWITCHSTOMP   = $09
CHNGTILE_TOBRICKCOIN    = $0B   ; brick containing coin
CHNGTILE_DELETETOBGALT  = $0C
CHNGTILE_PIPEJCT    = $0E   ; UNUSED replaces the unused TILE9_PIPEWORKS_JCT tile!
CHNGTILE_DELETEDONUT    = $0F
CHNGTILE_FROZENMUNCHER  = $10
CHNGTILE_FROZENCOIN = $11
CHNGTILE_PSWITCHAPPEAR  = $12
CHNGTILE_DOORAPPEAR = $13
CHNGTILE_TOADBOXOPEN    = $14
CHNGTILE_4WAYCANNON = $15
CHNGTILE_GIANTBRICKBUST = $16   ; Giant World brick bust
CHNGTILE_GIANTBLOCKHIT  = $17   ; Giant World [?] block hit to metal
CHNGTILE_GIANTBRICKFIX  = $18   ; Giant World brick restore (small Mario hit giant brick)
    RAM Level_ChgTileEvent, 1   ; When non-zero, queues a "change tile" event

    RAM Level_NoStopCnt, 1   ; A counter which continuously increments unless something is "stopping" the action
    RAM Level_Event, 1   ; Check "LevelEvent_Do" for values; 0 means nothing
    RAM Level_PSwitchCnt, 1   ; When non-zero, P-Switch is active (init @ $80); counts down to zero and restarts music

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    RAM Player_SlideRate, 1   ; While Player is sliding, this is added to X Velocity (does not persist, however)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1
    RAM Player_IsClimbing, 1   ; Set when Player is climing vine
    RAM Player_FlipBits_OLD, 1   ; Holds backup of Player_FlipBits
    RAM Player_HitCeiling, 1   ; Flag set when Player has just hit head off ceiling
    RAM Player_FlyTime, 1   ; When > 0, Player can fly (for power ups that do so); decrements (unless $FF) to 0
    RAM Player_IsDucking, 1   ; Set when Player is ducking down
    RAM Player_WhiteBlkCnt, 1   ; White block counter; counts up while Player is standing on white block and holding down

    ; Level_PipeMove is set to various values that dictate
    ; how Player is moving within a pipe
    ;
    ; The lower 2 bits form a direction as follows:
    ;   00 - right
    ;   01 - left
    ;   10 - down
    ;   11 - up
    ;
    ; If bit 7 ($80) is NOT set, then we're just entering
    ; the pipe, and next 3 bits say what it will do:
    ; 000xx - Does nothing (not used, invalid)
    ; 001xx - Exits level (i.e. World Map pipe junctions)
    ; 010xx - Junction to appropriate "Big Question Block" bonus area (Level_JctCtl = 2)
    ; 011xx - Junction general (Level_JctCtl = 3)
    ; 100xx - Junction to generic exit area (Level_JctCtl = 4)
    ; 101xx - In-level transit (a la World 7 pipe mazes and other places; does not set Level_JctCtl)
    ;
    ; If bit 7 IS set, we're moving through the pipe (either exiting out one end or in-transit)
    RAM Level_PipeMove, 1

    RAM Level_CoinHeav, 1   ; Enter coin heaven when set $80; Increments; at $D0, "soft jump" arrival; terminates at wrap to $00

    RAM Player_MoveLR, 1   ; 0 - Not moving left/right, 1 - Moving left, 2 - Moving right (reversed from the pad input)

    RAM Player_WalkAnimTicks, 1   ; Ticks between animation frames of walking; max value varies by Player's X velocity

    RAM Player_InWater, 1   ; Set for when in water (1 = Regular water specifically, other non-zero values indicate waterfall)
    RAM Player_SwimCnt, 1   ; Swim counter FIXME Describe better 0-3
    RAM Player_Kuribo, 1   ; Set for Kuribo's Shoe active
    RAM Player_QueueSuit, 1   ; Queues a suit change (values like Player_Suit, but add 1, EXCEPT: $0F = Statue enable, $40 = Splash, $80 = Kuribo's Shoe)
    RAM Player_mGoomba, 1   ; Player is caught by a micro Goomba (jump short)
    RAM Player_Statue, 1   ; Player is in Tanooki Statue mode; counts down to zero
    RAM Player_RunFlag, 1   ; Set while Player is actually considered "running" (holding down B and at enough speed; doesn't persist)
    RAM Player_Bounce, 1   ; Set to cause block bounce (upper 4 bits specifies what kind of block will be bounced)
    RAM Player_BounceDir, 1   ; Direction of Player bounce -- 0 = Down, 1 = Up, 2 = Left, 3 = Right
    RAM Player_BounceObj, 1   ; Set if it was a kicked shelled object that hit the bounce block (i.e. don't bounce the Player if the object is the one that hit)
    RAM Counter_ByPlayerSpd, 1   ; A counter which increments faster as the Player goes faster

    RAM Level_HAutoScroll, 1   ; When set to 1, "auto horizontal scroll" is active (this can be toggled mid-level)
    RAM B10Coin_Count, 1   ; Decrements until -1, you continue to get a coin until it does so
    RAM B10Coin_ID, 1   ; Forms a sort of unique ID so game knows if you've switched blocks

    RAM Player_OffScreen, 1   ; Set when Player is completely off screen

    ; FloatLevel_PlayerWaterStat: For levels that "float" and have a fixed set of water at the bottom
    ; Bit 6: Set if Player is beneath bottom of water
    ; Bit 7: Set if beneath top of water
    RAM FloatLevel_PlayerWaterStat, 1


    RAM Player_LowClearance, 1   ; Set when Player is in a "low clearance" situation (big Mario in a single block high tunnel)

    RAM PUp_StarManFlash, 1   ; Set when a Starman is bouncing about so it cycles colors! (also used to get super suit if set in advance)

    ; Player_Behind_En:
    ; Specifies whether the "Behind the scenes" effect is actually active
    ; If the Player has stepped out from behind the background, it can be
    ; still active, but he won't get the effect of it!
    RAM Player_Behind_En, 1
    RAM Player_Behind, 1   ; When non-zero, Player is "behind the scenes" (as by white block)

    RAM Player_Slippery, 1   ; 0 = Ground is not slippery, 1 = Ground is a little slippery, 2 = Ground is REALLY slippery
    RAM Player_SandSink, 1   ; Sinking in quicksand! (holds Y when quicksand hit in upper 4 bits, bit 0 sets '0' jumping, '1' sinking)

    ; Player_PartDetEn: "32 Pixel Partition Detection" enabler
    ; When set, if Player Y >= 160, Player detects bottom two rows of tiles implicitly
    ; Used with Update_Request = UPDATERASTER_32PIXPART if there's a floor
    ; (i.e. NOT used in levels with fixed water; for that, see FloatLevel_PlayerWaterStat)
    RAM Player_PartDetEn, 1
    RAM Player_InPipe, 1   ; Player is going through pipe
    RAM Player_MushFall, 1   ; Set to 0 when mushroom is to fall to the left, 1 to the right
    RAM Player_SprOff, 1   ; Player sprite offset (NOTE: Should be multiples of 4, otherwise bad unaligned stuff happens!)

    ; Strange gapping here; there's pretty much enough room for a couple more special objects
    RAM Object_SprRAM, 8   ; $058F-$0596 Sprite_RAM offset by object

    RAM SpecialObj_Var2, 8   ; $0597-$059E General purpose variable 2

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_YVelFrac, 8   ; $05A1-$05A8 Y velocity fractional accumulator

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_XVelFrac, 8   ; $05AB-$05B2 X velocity fractional accumulator

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_Var3, 8   ; $05B5-$05BC General purpose variable 3

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_YLo, 8   ; $05BF-$05C6 Y low coordinate of special object

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_XLo, 8   ; $05C9-$05D0 X low coordinate of special object

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_YVel, 8   ; $05D3-$05DA Y Velocity of special object

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2
    RAM SpecialObj_XVel, 8   ; $05DD-$05E4 X Velocity of special object

__RAM_OFFSET__ .set __RAM_OFFSET__ + 8
    RAM Misc_Counter, 1   ; Miscellaneous loop counter ?
    RAM Level_TimerMSD, 1   ; Leftmost / most significant digit on timer
    RAM Level_TimerMid, 1   ; Middle digit on timer
    RAM Level_TimerLSD, 1   ; Rightmost / least significant digit on timer
    RAM Level_TimerTick, 1   ; Timer decrementing tick
    RAM Inventory_Open, 1   ; Set when inventory panel is open, also used to dictate whether it is "opening" (1) or "closing" (0)
    RAM Level_TimerEn, 1   ; Set to disable clock (bit 7 will also disable level animations, e.g. '?s')
    RAM Kill_Tally, 1   ; Counter that increases with each successful hit of an object without touching the ground

    RAM Objects_KillTally, 5   ; $05F5-$05F9 OBJECT SLOTS 0 - 4 ONLY: Kill_Tally for a kicked shell as it hits other enemies

    RAM PlayerProj_YHi, 2   ; $05FA-$05FB Player projectile Y Hi

    ; NOTE: Since Level_AScrlConfig checks are generally implemented as "BEQ/BNE", technically ANY
    ; value enables auto scroll adjustments, but officially ASCONFIG_ENABLE is used to enable it
ASCONFIG_ENABLE     = $01   ; Enables auto scroll coordinate adjustments of any sort
ASCONFIG_HDISABLE   = $80   ; Disables horizontal auto scroll coordinate adjustment (generally if Horz Auto Scroll is not in use)
    RAM Level_AScrlConfig, 1

    RAM Cine_ToadKing, 1   ; Set to 1, initializes Toad and transformed king; set to 2 while running that cinematic
    ; The diagonal auto scroller wraps vertically several times to give the illusion
    ; of a long vertical strip.  AScrlURDiag_WrapState is set to 1 and 2 during this
    ; process which makes it possible.  Objects that need to offset themselves to
    ; cope with this behavior utilize AScrlURDiag_WrapState_Copy to stay in sync.
    RAM AScrlURDiag_WrapState_Copy, 1   ; Copy of AScrlURDiag_WrapState
    RAM AScrlURDiag_WrapState, 1


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $06xx RAM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; DURING ENDING ONLY
__RAM_OFFSET__ .set $300; $0600
    RAM Ending_CmdBuffer, 192 ; $0600-$06C0 Buffer used during ending sequence

    ; Normal purpose $06xx RAM...
__RAM_OFFSET__ .set $300; $0600

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM Level_Tile_Head, 1   ; Tile at Player's head
    RAM Level_Tile_GndL, 1   ; Tile at Player's feet left
    RAM Level_Tile_GndR, 1   ; Tile at Player's feet right
    RAM Level_Tile_InFL, 1   ; Tile "in front" of Player ("lower", at feet)
    RAM Level_Tile_InFU, 1   ; Tile "in front" of Player ("upper", at face)
    RAM Level_Tile_Whack, 1   ; Tile last hit by tail attack or shell
    RAM Level_Tile_Quad, 4   ; $0608-$060B Quadrant of tile for each of the positions above

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    ; Level_Tile_Slope: Slope of tile for each of the positions above (first byte also used by objects)
    ; 0: Slope shape at feet
    ; 1: Slope shape at head
    ; 2: "in front" of Player ("lower", at feet)
    ; 3: "in front" of Player ("upper", at face)
    RAM Level_Tile_Slope, 4   ; $060D-$0610

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Scroll_Cols2Upd, 1   ; Number of 8x8 columns to update (typically set to 32 for a full dirty update)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 6

    RAM Bonus_CoinsYVel, 6   ; $0619-$061E UNUSED Bonus game coins Y velocity
    RAM Bonus_CoinsY, 6   ; $061F-$0624 UNUSED Bonus game coins Y
    RAM Bonus_CoinsXVel, 6   ; $0625-$062A UNUSED Bonus game coins X velocity
    RAM Bonus_CoinsX, 6   ; $062B-$0630 UNUSED Bonus game coins X
    RAM Bonus_CoinsYVelFrac, 6   ; $0631-$0636 UNUSED Bonus game coins Y velocity fractional accumulator

__RAM_OFFSET__ .set __RAM_OFFSET__ + 6

    RAM Bonus_CoinsXVelFrac, 6   ; $063D-$0642 UNUSED Bonus game coins X velocity fractional accumulator

__RAM_OFFSET__ .set __RAM_OFFSET__ + 8

    RAM Object_TileFeet, 1   ; Object tile detected at "feet" of object
    RAM Object_TileWall, 1   ; Object tile detected in front of object, i.e. a wall

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Object_AttrFeet, 1   ; Object tile quadrant of Object_TileFeet
    RAM Object_AttrWall, 1   ; Object tile quadrant of Object_TileWall

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Objects_SprHVis, 8   ; $0651-$0658 Flags; Bits 7-2 set when each 8x16 sprite is horizontally off-screen (left-to-right from MSb)
    RAM Objects_SpawnIdx, 8   ; $0659-$0660 Holds the index into level data that this object was spawned from

; Objects_State
OBJSTATE_DEADEMPTY  = 0 ; Dead/Empty
OBJSTATE_INIT       = 1 ; Init
OBJSTATE_NORMAL     = 2 ; Normal (typical operation)
OBJSTATE_SHELLED    = 3 ; Shelled (shelled enemy post-stomp)
OBJSTATE_HELD       = 4 ; Held (held by Player)
OBJSTATE_KICKED     = 5 ; Kicked (kicked by Player / spinning shell)
OBJSTATE_KILLED     = 6 ; Killed (flipped over and falling off screen)
OBJSTATE_SQUASHED   = 7 ; Squashed (generally Goomba only)
OBJSTATE_POOFDEATH  = 8 ; "Poof" Death (e.g. Piranha death)
    RAM Objects_State, 8

    RAM Objects_Frame, 8   ; $0669-$0670 "Frame" of object (see ObjectGroup_PatternSets)

    RAM Level_ObjectID, 8   ; $0671-$0678 All active actor IDs

    RAM Objects_FlipBits, 8   ; $0679-$0680 Applied sprite attributes for this object (usually just horizontal/vertical flip)

    RAM Objects_SprVVis, 8   ; $0681-$0688 Flags; Bits 3-0 set when each 8x16 sprite is vertically off-screen (top-to-bottom from MSb)
    RAM Objects_Var1, 8   ; $0689-$0690 Generic variable 1 for objects
    RAM Objects_Var2, 8   ; $0691-$0698 Generic variable 2 for objects

    RAM Unused699, 1   ; Absolutely no idea, it is set once in one place and never used again... MAY be lost bonus game related?

    ; UNUSED Bonus Game Die counter
    ; While the die is rotating, just used as a counter 0 to 3 to time the rolling animation.
    ; After Player would press 'A', this value is immediately set to 0.
    ; In the case of the odd/even game, if the Player "won", it is set to 5 or 6.
    RAM Bonus_DieCnt, 1

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Score_Earned, 2   ; $069C-$069D (16-bit value) A "buffer" of score earned to be added to your total, total score stored in Player_Score
    RAM Score_Temp, 1   ; Temp used when figuring out to display a 3-byte integer worth of score

__RAM_OFFSET__ .set __RAM_OFFSET__ + 5

    RAM Player_IsHolding, 1   ; Set when Player is holding something (animation effect only)
    RAM Player_ISHolding_OLD, 1   ; Holds onto whether Player WAS holding onto something (so we can be sure to clear Player_IsHolding)

; NOTE!! These object vars are OBJECT SLOT 0 - 4 ONLY!

; Objects_Timer3: Used as the "wake up" out of shell timer
; If timer is less than $60, it decrements normally, otherwise...
;   If object is in state 2, timer decrements normally
;   If object is in state 4 (being held), timer only decrements every 4 ticks
;   In all other states, timer decrements every 2 ticks
    RAM Objects_Timer3, 5   ; $06A6-$06AA Used as the "wake up" out of shell timer
    RAM Objects_Timer4, 5   ; $06AB-$06AF "Timer" values; automatically decrements to zero (used in "shakin' awake" effect)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 3

    RAM Object_SlopeHeight, 1   ; Object calculated slope height
    RAM Buffer_Occupied, 2   ; $06B4-$06B5 Set if respective Object_BufferX/Y buffer is already taken by an object

    RAM_NOINC Player_UphillSpeedIdx      ; Override when Player_UphillFlag is set (shared with Player_Microgoomba)
    RAM Player_Microgoomba, 1   ; Microgoomba stuck to Player
    RAM Objects_InWater, 5   ; $06B7-$06BB Set when object is in water

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM SpecialObj_Var1, 8   ; $06BD-$06C4 General purpose variable 1

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM SpecialObj_Data, 8   ; $06C7-$06CE Special object "data" field, defined by object

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM SpecialObj_Timer, 8   ; $06D1-$06D8 "Timer" values; automatically decrements to zero

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM CannonFire_Var, 8   ; $06DB-$06E2
    RAM CannonFire_Timer, 8   ; $06E3-$06EA Cannon Fire timer, decrements to zero
    RAM Objects_QSandCtr, 8   ; $06EB-$06F2 When enemy has fallen into quicksand, increments until $90 which deletes it


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $07xx RAM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__RAM_OFFSET__ .set $400; $0700

    RAM TileAddr_Off, 1   ; During level loading, specifies an offset into the current Mem_Tile_Addr setting

    ; LevLoad_Unused1-4 are initialized when about to load a level,
    ; but never used.  May have been reserved or intended or
    ; even debugging, but who knows now...
    RAM LevLoad_Unused1, 1
    RAM LevLoad_Unused2, 1
    RAM LevLoad_Unused3, 1
    RAM LevLoad_Unused4, 1

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM LL_ShapeDef, 1   ; During level loading, defines a shape of something (context-specific)

    RAM Scroll_UpdAttrFlag, 1   ; Set when it is time to update attributes

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    ; Tileset values:
    ; 00 = On map
    ; 01 = Plains style; bushes, zigzag ground, blocks (includes blue-block bonus area)
    ; 02 = Mini fortress style; gray blocks, lava (and king's room)
    ; 03 = Hills style; green grass, bushes (works in desert like 2-2 also)
    ; 04 = High-Up style; "cliffside" grass, swirly clouds, wooden block style ground
    ; 05 = pipe world plant infestation
    ; 06 = Water world
    ; 07 = Toad House
    ; 08 = Vertical pipe maze
    ; 09 = desert level, sand, pyramids (and desert fortress)
    ; 10 = Airship
    ; 11 = Giant World
    ; 12 = Ice level, frozen "1" style
    ; 13 = Coin heaven / Sky level
    ; 14 = Underground
    ; 15 = Bonus game intro and N-Spade
    ; 16 = Spade game sliders
    ; 17 = N-Spade
    ; 18 = 2P vs
    RAM Level_Tileset, 1   ; Different tilesets which changes the detection and meaning in levels

    RAM_NOINC Bonus_UnusedVH         ; VRAM High address ?? Seems to only be part of an unused routine
    RAM ToadTalk_VH, 1   ; Cinematic Toad & King / "Toad House" Toad VRAM Address High

    RAM_NOINC Bonus_UnusedVL         ; VRAM Low address ?? Seems to only be part of an unused routine
    RAM ToadTalk_VL, 1   ; Cinematic Toad & King / "Toad House" Toad VRAM Address Low

    RAM_NOINC BonusText_CPos
    RAM ToadTalk_CPos, 1   ; Cinematic Toad & King / "Toad House" Toad Character Position

    RAM BonusText_CharPause, 1   ; Counter that decrements to zero between letters
    RAM Bonus_UnusedFlag, 1   ; Doesn't do much besides block an unused subroutine

    RAM Map_Pan_Count, 1   ; Map is panning, counts to zero (Scroll_LastDir sets which direction we're panning)

    ; NOTE sharing
    RAM_NOINC CineKing_Timer2        ; Timer; decrements to zero
    RAM_NOINC Bonus_Timer            ; Decrements to zero
    RAM Map_Intro_Tick, 1   ; Counts down to zero while displaying the "World X" intro

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Map_ReturnStatus, 1   ; When 0, level panel is cleared; otherwise, Player is considered to have died (decrements life!)
    RAM MaxPower_Tick, 1   ; When Player has maximum "power" charge, this counts for the flashing [P]
    RAM Player_Score, 3   ; $0715 (H)-$0717 (L) treated as 3-byte integer, with least significant zero on display not part of this value

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    ; Each byte of PatTable_BankSel sets the VROM available at
    ; 0000 (first half BG), 0800 (second half BG, typ animated),
    ; 1000 (1/4 sprites), 1400 (2/4 sprites), 1800 (3/4 sprites),
    ; and 1C00 (4/4 sprites), respectively
    RAM PatTable_BankSel, 6   ; $0719-$071E  Provides an array of 6 pages to set the entire Pattern Table [BG_Full_CHRROM_Switch]
    RAM PAGE_C000, 1   ; Page to set PRG ROM C000 (PRGROM_Change_Both)
    RAM PAGE_A000, 1   ; Page to set PRG ROM A000 (PRGROM_Change_Both)
    RAM PAGE_CMD, 1   ; When using PRGROM_Change_Both2 or PRGROM_Change_A000, this value stores the MMC3 command
    RAM Map_Prev_XOff, 2   ; $0722-$0723 (Mario/Luigi) Stores previous scroll X offset on map
    RAM Map_Prev_XHi, 2   ; $0724-$0725 (Mario/Luigi) Stores previous "hi byte" of map X
    RAM Player_Current, 1   ; Which Player is currently up (0 = Mario, 1 = Luigi)
    RAM World_Num, 1   ; Current world index (0-8, where 0 = World 1, 7 = World 8, 8 = World 9 / Warp Zone)

    ; NOTE: sharing
    RAM_NOINC World_EnterState       ; State variable during "world X" intro entrance, set to 3 when entering a level; overlaps GameOver_State
    RAM_NOINC CineKing_State         ; State of King-got-his-wand-back sequence
    RAM GameOver_State, 1   ; State variable used during "Gameover!" sequence only; overlaps World_EnterState

    RAM Map_Operation, 1   ; Map_Operation: Current "operation" happening on map (See Map_DoOperation in PRG010)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Total_Players, 1   ; Total players (0 = 1P, 1 = 2P)
    RAM Map_Unused72C, 1   ; No apparent use; only assigned to, but never read back
    RAM Bonus_DDDD, 1   ; ?? Set to '1' if you exit the unused bonus game BONUS_UNUSED_DDDD
    RAM Map_HandState, 1   ; Hand trap state

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Map_WW_Backup_Y, 1   ; Warp Whistle wind backs up the Player's map Y

    RAM Map_WW_Backup_XH, 1   ; Warp Whistle wind backs up the Player's map X Hi byte
    RAM Map_WW_Backup_X, 1   ; Warp Whistle wind backs up the Player's map X
    RAM Map_WW_Backup_UPV2, 1   ; Warp Whistle wind backs up Map_UnusedPlayerVal2

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM Player_Lives, 2   ; $0736-$0737 (Mario/Luigi) Player's lives
    RAM Map_Unused738, 1   ; Index used in some dead code in PRG011; sometimes index of unused array Map_Unused7DC6
    RAM ClearPattern, 1   ; Set by ClearPattern_ByTileset for use in Clear_Nametable_Short
    RAM PalSel_Tile_Colors, 1   ; Stores value to index which tile color set to use when palette loading routine is called
    RAM PalSel_Obj_Colors, 1   ; Stores value to index which object color set to use when palette loading routine is called
    RAM Player_FallToKing, 2   ; $073C-$073D (Mario/Luigi) Player will fall to king when Level_ExitToMap is nonzero (instead of map)
    RAM Map_Player_SkidBack, 2   ; $073E-$073F (Mario/Luigi) Set to '1' if Player died last turn or otherwise ejected (that is, they DID skid from their last turn, not necessarily currently skidding)

    RAM Map_NSpadeMatches, 1   ; Keeps count of N-Spade matches of active session (9 means game is done)
    RAM Map_NSpadeInProg, 1   ; If set, there's an N-Spade game "in progress" (so if you lose, you pick it up later)

    RAM Map_Airship_Trav, 1   ; Airship's current travel-table offset (randomly offset by 0-2, spices up life)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 2

    RAM Map_DoFortressFX, 1   ; Bust locks, build bridges, whatever after Mini-Fortress is toppled

    RAM World_Map_Power, 2   ; $0746-$0747 (Mario/Luigi) MAP Power up # (1 - big, 2 - Fire, 3 - Leaf, 4 - Frog, 5 - Tanooki, 6 - Hammer, 7 - Judgems, 8 - Pwing, 9 - Star)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Map_Unused749, 2   ; $0749-$074A (Mario/Luigi) ? Another value just set once and never read back!

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Object_VelCarry, 1   ; '1' when last Object Velocity fraction accumulation rolled over

    ; WARNING: The distance between Player/Objects_XVelFrac and Player/Objects_YVelFrac must be same as Player/Objects_X/YVel!
    RAM Player_XVelFrac, 1   ; X velocity fractional accumulator
    RAM Objects_XVelFrac, 8   ; $074E-$0755 Other object's X velocity fractional accumulator

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM THouse_UnusedFlag, 1   ; Cleared when Toad House gives you an item, but never used otherwise

                ; WARNING: Though unused, this is required for the consistent padding between XVel(Frac) and YVel(Frac)
                ; So use it if you want, but maintain the distance!!
__RAM_OFFSET__ .set __RAM_OFFSET__ + 7   ; $0758-$075E unused

    ; WARNING: The distance between Player/Objects_XVelFrac and Player/Objects_YVelFrac must be same as Player/Objects_X/YVel!
    RAM Player_YVelFrac, 1   ; Y velocity fractional accumulator
    RAM Objects_YVelFrac, 8   ; $0760-$0767 Other object's Y velocity fractional accumulator

    RAM Objects_ColorCycle, 8   ; $0768-$076F Cycles colors of object and decrements to zero (e.g. "Melting" ice block, starman, etc.)

    ; Objects_Var6: Special hardcoded behavior for the following objects ONLY:
    ; OBJ_FIRECHOMP, OBJ_CHAINCHOMPFREE, OBJ_BLOOPERCHILDSHOOT,
    ; OBJ_BLOOPERWITHKIDS, or OBJ_FIRESNAKE
    ; ... as the X/Y buffer slot they occupy (see Object_Delete)
    RAM Objects_Var6, 5   ; $0770-$0774 General purpose variable 6 (except as noted above)
    RAM Objects_TargetingXVal, 5   ; $0775-$0779 X velocity result of Object_CalcHomingVels for this object OR some other X pixel target

    RAM_NOINC King_Y             ; Y position (NOTE: shared with Objects_TargetingYVal)
    RAM Objects_TargetingYVal, 5   ; $077A-$077E Y velocity result of Object_CalcHomingVels for this object OR some other Y pixel target

    RAM_NOINC Pipe_TransYDelta       ; In-level transit pipe Y delta value (WARNING: Shared with Level_ScrollDiffV)
    RAM Level_ScrollDiffV, 1   ; Difference between desired vertical and the current Vert_Scroll (WARNING: Shared with Pipe_TransYDelta)
    RAM Level_ScrollDiffH, 1   ; Difference between desired horizontal and the current Horz_Scroll

    RAM Random_Pool, 10  ; $0781-$078A (or $0789?) Data pool for pseudo-random number generator algorithm
RandomN = Random_Pool+1         ; Pull a random number from the sequence (NOTE: RandomN+1 is also good; If you need multiple random numbers, call Randomize)

    RAM Map_PlayerLost2PVs, 1   ; When > 0, (1=Mario, 2=Luigi) doesn't lose a life for "death" exiting to map, but does lose their turn

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Player_RescuePrincess, 1   ; Player will jump to the princess rescue when Level_ExitToMap is nonzero (instead of map)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 8

    ; Objects_PlayerHitStat:
    ;   Bit 0 - Set if Player's bbox bottom is HIGHER than object's bbox bottom
    ;   Bit 1 - Set if Player's bbox left edge is to the LEFT of object's bbox left edge
    ;   Bit 4 - Set if Player tail attacked an object
    RAM Objects_PlayerHitStat, 8   ; $0796-$079D Player hit status

    ; Up to five "scores" can be displayed at once
    RAM Scores_Value, 5   ; $079E-$07A2 score "value"; '0' none, (10, 20, 40, 80, 100, 200, 400, 800, 1000, 2000, 4000, 8000, 1-up)
    RAM Scores_Counter, 5   ; $07A3-$07A7 "counter" until score disappears
    RAM Scores_Y, 5   ; $07A8-$07AC Score's Y
    RAM Scores_X, 5   ; $07AD-$07B1 Score's X

    RAM LRBounce_Y, 1   ; Left/right bouncer as sprite Y ($FF is disabled)
    RAM LRBounce_X, 1   ; Left/right bouncer as sprite X
    RAM LRBounce_Vel, 1   ; Left/right bouncer absolute value of X velocity

    ; NOTE!! These object vars are OBJECT SLOT 0 - 4 ONLY!
    RAM Objects_Slope, 5   ; $07B5-$07B9 Absolute slope calc value

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM World3_Bridge, 1   ; 0 - Bridges are down, 1 - Bridges are up

    RAM ArrowPlat_IsActive, 1   ; Set if arrow platform is active

    RAM Level_GetWandState, 1   ; See Koopaling code in PRG001

; ********************************************************************************
; The Palette_* vars here form a graphics buffer to be committed in the
; style of the Video_Upd_Table; see "Video_Upd_Table" in PRG030 for format.
    RAM_NOINC Video_DoPalUpd         ; Name consistent with Video_Upd_Table
    RAM Palette_AddrHi, 1   ; Stores high part of palette address when committing palettes
    RAM Palette_AddrLo, 1   ; Stores low part of palette address when committing palettes
    RAM Palette_BufCnt, 1   ; 32 for updating entire palette
    RAM Palette_Buffer, 32  ; $07C1-$07E0 Buffer of palette bytes to commit, used for fade in/out
    RAM Palette_Term, 1   ; Set to zero as terminator, per requirement of the Video_Upd_Table format
; ********************************************************************************

; BigQBlock_GotIt:
;   Big ? Blocks can only be retrieved once; this is a bitfield that marks which
;   ones you've gotten by setting a bit per screen space in the Big ? Block area.
;   Basically, if you reenter a Big ? Block area after opening it, it won't reappear!
;   See ObjInit_BigQBlock and ObjNorm_BigQBlock for usage...
;   NOTE: This is cleared completely upon Player death which works since levels
;   are not re-enterable, but still seems a bit extreme...
    RAM BigQBlock_GotIt, 1

__RAM_OFFSET__ .set __RAM_OFFSET__ + 13

    RAM DMC_Queue, 1   ; Stores value to play on DMC
    RAM DMC_Current, 1   ; Currently playing DMC sound

    RAM Sound_Sq1_CurFL, 1   ; Holds current "low" frequency of Square Wave 1 (Warning: Must be +4 to Sound_Sq2_CurFL, see PRG031_E808)

    RAM Music_NseStart, 1   ; Holds the starting offset of the noise track (CHECK: Reuse of $07F3, is this bad??)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Music2_Hold, 1   ; A very little used feature, Music Set 1 overrides Music Set 2, but after a M1 song finishes, it restarts the M2 song
    RAM Sound_Sq2_CurFL, 1   ; Holds current "low" frequency of Square Wave 1 (Warning: Must be +4 from Sound_Sq1_CurFL, see PRG031_E808)

    RAM Music_Sq2Patch, 1   ; Current "instrument patch" for Square 2 (only upper 4 bits stored, 0ppp 0000)
    RAM Music_Sq1Patch, 1   ; Current "instrument patch" for Square 1 (only upper 4 bits stored, 0ppp 0000)

__RAM_OFFSET__ .set __RAM_OFFSET__ + 1

    RAM Sound_Map_L2Hld, 1   ; Same as Sound_Map_LHold, used for the secondary track (Warning: Will be affected by triangle, see PRG031_E808)
    RAM Sound_Map_Len2, 1   ; Same as Sound_Map_Len, used for the secondary track
    RAM Sound_Map_LHold, 1   ; Current length setting, used as delay after each byte of map sound (changed by special bytes in range $80-$FF)
    RAM Sound_Map_Len, 1   ; Countdown tick for current note/rest that map sound effect is on
    RAM Sound_Map_Off2, 1   ; Same as Sound_Map_Off, used for the secondary track
    RAM Sound_Unused7FF, 1   ; Cleared once, never used otherwise

.endif; include guard
