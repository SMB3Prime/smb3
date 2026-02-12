.PHONY: all clean dir run

# ---- directories ----
SOURCEDIR  := src
PRGDIR     := PRG
INCDIR     := include
BUILDDIR   := build

ASFLAGS += $(addprefix -I,$(INCDIR) .)

# ---- output ----
ROM_STOCK  := smb3.nes
ROM_EXPD   := smb3-expanded.nes

# ---- sources ----
PRG_ASM := $(wildcard $(PRGDIR)/*.asm)
SRC_ASM := $(wildcard $(SOURCEDIR)/*.s)
MAIN_ASM := smb3.s

# target-specific variables
$(ROM_STOCK): VARIANT  := stock
$(ROM_EXPD):  VARIANT  := expanded

ALL_COMMON := $(MAIN_ASM) $(SRC_ASM) $(PRG_ASM)

# ---- objects ----
COMMON_OBJS = \
  $(patsubst %.s,$(BUILDDIR)/%.o,$(notdir $(filter %.s,$(ALL_COMMON)))) \
  $(patsubst %.asm,$(BUILDDIR)/%.o,$(notdir $(filter %.asm,$(ALL_COMMON))))

VPATH = $(SOURCEDIR) $(PRGDIR) .

all: $(ROM_STOCK) $(ROM_EXPD)

clean:
	@rm -rf $(BUILDDIR) $(ROM_STOCK) $(ROM_EXPD)

define BUILD_ROM
$1: CFG := $2/mmc3.cfg
$1: DBG := build/smb3-$2.dbg
$1: MAP := build/map-$2.txt
$1: $(COMMON_OBJS) build/header-$2.o
endef

$(eval $(call BUILD_ROM,$(ROM_STOCK),stock))
$(eval $(call BUILD_ROM,$(ROM_EXPD),expanded))

$(ROM_STOCK) $(ROM_EXPD):
	ld65 -m $(MAP) \
	     --dbgfile $(DBG) \
	     -o $@ \
	     -C $(CFG) \
	     $^

$(BUILDDIR)/header-%.o: %/header.s
	@mkdir -p $(BUILDDIR)
	ca65 -g $(ASFLAGS) -o $@ $<

$(BUILDDIR)/%.o: %.s
	@mkdir -p $(BUILDDIR)
	ca65 -g $(ASFLAGS) --create-dep $(@:.o=.d) -o $@ $<

$(BUILDDIR)/%.o: %.asm
	@mkdir -p $(BUILDDIR)
	ca65 -g $(ASFLAGS) --create-dep $(@:.o=.d) -o $@ $<

-include $(COMMON_OBJS:.o=.d)