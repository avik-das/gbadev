## This Makefile is based on the DevkitPro template Makefile, with additions
## suggested by the "grit" manual (see
## http://www.coranac.com/man/grit/html/gritmake.htm#sec-template).
##
## The formatting has been cleaned up, but otherwise, the structure remains the
## same as the template.

.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

GRIT	:= grit

# TARGET   - name of the output
# BUILD    - directory where object and intermediate files will be placed
# SOURCES  - list of directories with source code
# INCLUDES - list of directories with extra header files
# DATA     - list of directories with binary data
# GRAPHICS - list of directories with files to be processed by grit
# MUSIC    - list of directories with files to be processed by mmutil
#
# All directories are specified relative to the project directory where the
# makefile is found.
TARGET		:= $(notdir $(CURDIR))
BUILD		:= build
SOURCES		:= src
INCLUDES	:= include
GRAPHICS	:= gfx

# -- Options for code generation ----------------------------------------------

ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O3\
		-mcpu=arm7tdmi -mtune=arm7tdmi\
		-fomit-frame-pointer\
		-ffast-math \
		$(ARCH)

CFLAGS	+=	$(INCLUDE)

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)

# Any extra libraries we wish to link with the project
LIBS	:= -lmm -lgba

# List of directories containing libraries. This must be the top level
# containing include and lib
LIBDIRS	:=	$(LIBGBA)

#------------------------------------------------------------------------------
# No real need to edit anything past this point unless you need to add
# additional rules for different file extensions
#------------------------------------------------------------------------------

ifneq ($(BUILDDIR), $(CURDIR))

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
GFXFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES	:=	$(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
endif

# Use CXX for linking C++ projects, CC for standard C
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_GFX := $(GFXFILES:.png=.o)
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_BIN) $(OFILES_GFX) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) BUILDDIR=`cd $(BUILD) && pwd` --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba

else

# -- Main targets -------------------------------------------------------------

$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SOURCES) : $(HFILES)

# Use grit to convert graphics
# With matching grit-file
%.s %.h : %.png %.grit
	$(GRIT) $< -fts

# No matching grit-file. Try using directory-level grit-file.
%.s %.h : %.png
	$(GRIT) $< -fts -ff $(<D)/$(notdir $(<D)).grit

# Build soundbank from music files
soundbank.bin soundbank.h : $(AUDIOFILES)
	@mmutil $^ -osoundbank.bin -hsoundbank.h

# The bin2o rule should be copied and modified for each extension used in the
# data directories

# Link in binary data with the .bin extension
%.bin.o	%_bin.h :	%.bin
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPSDIR)/*.d

endif
