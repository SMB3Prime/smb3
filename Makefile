.PHONY: all clean dir run

# ---- directories ----
SOURCEDIR  := src
PRGDIR     := PRG
INCDIR     := include
BUILDDIR   := build

ASFLAGS += $(addprefix -I,$(INCDIR) .)

# ---- output ----
ROM_NAME   := smb3.nes
DBG_NAME   := smb3.dbg
MAP_NAME   := $(BUILDDIR)/map.txt

# ---- sources ----
PRG_ASM := $(wildcard $(PRGDIR)/*.asm)
SRC_ASM := $(wildcard $(SOURCEDIR)/*.s)
MAIN_ASM := smb3.s

ALL_ASM := $(MAIN_ASM) $(SRC_ASM) $(PRG_ASM)

# ---- objects ----
O_FILES := \
  $(patsubst %.s,$(BUILDDIR)/%.o,$(notdir $(filter %.s,$(ALL_ASM)))) \
  $(patsubst %.asm,$(BUILDDIR)/%.o,$(notdir $(filter %.asm,$(ALL_ASM))))

VPATH := $(SOURCEDIR) $(PRGDIR) .

all: dir $(ROM_NAME)

dir:
	@mkdir -p build

clean:
	-@rm -rf $(BUILDDIR)
	-@rm -f $(ROM_NAME)
	-@rm -f $(DBG_NAME)

$(ROM_NAME): $(O_FILES)
	ld65 -m $(BUILDDIR)/map.txt \
	     --dbgfile $(DBG_NAME) \
	     -o $@ \
	     -C mmc3.cfg \
	     $^

$(BUILDDIR)/%.o: %.s
	ca65 -g $(ASFLAGS) --create-dep $(@:.o=.d) -o $@ $<

$(BUILDDIR)/%.o: %.asm
	ca65 -g $(ASFLAGS) --create-dep $(@:.o=.d) -o $@ $<

-include $(O_FILES:.o=.d)