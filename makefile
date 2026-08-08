ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)
 
export BUILD_EXEFS_SRC := build/exefs
 
include $(DEVKITPRO)/libnx/switch_rules

APP_TITLE 	:= MAME NX Reborn 2003 Plus
APP_AUTHOR 	:= Blaze414
APP_VERSION := 1.0.0
# Lower case, matching the file on disk. It was 'Icon.jpg', which resolves
# only because macOS is case-insensitive - the build container is Linux and
# would have silently produced an .nro with no icon on a case-sensitive tree.
ICON 		:= icon.jpg
BUILD		:=	build
DATA		:=	data
INCLUDES	:=	include
EXEFS_SRC	:=	exefs_src
ROMFS		:=	RomFS


BINDIR	  = release
OUTPUT    = mame

ifeq ($(strip $(ICON)),)
	icons := $(wildcard *.jpg)
	ifneq (,$(findstring $(TARGET).jpg,$(icons)))
		export APP_ICON := $(TOPDIR)/$(TARGET).jpg
	else
		ifneq (,$(findstring icon.jpg,$(icons)))
			export APP_ICON := $(TOPDIR)/icon.jpg
		endif
	endif
else
	export APP_ICON := $(TOPDIR)/$(ICON)
endif

ifeq ($(strip $(NO_ICON)),)
	export NROFLAGS += --icon=$(APP_ICON)
endif
 

ifneq ($(APP_TITLEID),)
	export NACPFLAGS += --titleid=$(APP_TITLEID)
endif

ifneq ($(ROMFS),)
	export NROFLAGS += --romfsdir=$(CURDIR)/$(ROMFS)
endif

# set this to mame, mess or the destination you want to build
# TARGET = mame
# TARGET = mess
# TARGET = neomame
# TARGET = cpmame
# TARGET = mmsnd
# example for a tiny compile
# TARGET = tiny
ifeq ($(TARGET),)
TARGET = mame
endif

# uncomment next line to include the debugger
# DEBUG = 1

# uncomment next line to include the symbols for symify
# SYMBOLS = 1

# uncomment next line to generate a link map for exception handling in windows
# MAP = 1

# uncomment next line to use Assembler 68000 engine
# X86_ASM_68000 = 1

# uncomment next line to use Assembler 68020 engine
# X86_ASM_68020 = 1

# uncomment next line to use DRC MIPS3 engine
# X86_MIPS3_DRC = 1

# uncomment next line to use cygwin compiler
# COMPILESYSTEM_CYGWIN	= 1


# set this the operating system you're building for
# MAMEOS = msdos
# MAMEOS = windows
ifeq ($(MAMEOS),)
MAMEOS = nx
endif

# extension for executables
EXE = .exe

# CPU core include paths
VPATH=src $(wildcard src/cpu/*)

# compiler, linker and utilities
AR = aarch64-none-elf-gcc-ar
CC = aarch64-none-elf-gcc
CXX = aarch64-none-elf-g++
LD = aarch64-none-elf-g++
ASM = @nasm
ASMFLAGS = -f coff
MD = -mkdir
RM = @rm -f
#PERL = @perl -w


ifeq ($(MAMEOS),msdos)
PREFIX = d
else
PREFIX =
endif

ifdef DEBUG
NAME = $(PREFIX)$(TARGET)$(SUFFIX)d
else
ifdef ATHLON
NAME = $(PREFIX)$(TARGET)$(SUFFIX)at
ARCH = -march=athlon
else
ifdef K6
NAME = $(PREFIX)$(TARGET)$(SUFFIX)k6
ARCH = -march=k6
else
ifdef I686
NAME = $(PREFIX)$(TARGET)$(SUFFIX)pp
ARCH = -march=pentiumpro
else
ifdef P4
NAME = $(PREFIX)$(TARGET)$(SUFFIX)p4
ARCH = -march=pentium4
else
NAME = $(PREFIX)$(TARGET)$(SUFFIX)
ARCH = -march=armv8-a
endif
endif
endif
endif
endif

# build the targets in different object dirs, since mess changes
# some structures and thus they can't be linked against each other.
OBJ = obj/$(NAME)

EMULATOR = $(BINDIR)/$(NAME).elf

DEFS = -DALIGN_SHORTS -DALIGN_INTS -DLSB_FIRST -DINLINE=__inline__ -DSWITCH  -D__SWITCH__

CFLAGS = -Isrc -Isrc/nx/compat_shim -Isrc/lib/mame-chd -Isrc/includes -Isrc/$(MAMEOS) -I$(OBJ)/cpu/m68000 -Isrc/cpu/m68000 -I$(DEVKITPRO)/libnx/include -I$(DEVKITPRO)/portlibs/switch/include
CXXFLAGS = -Isrc -Isrc/nx/compat_shim -Isrc/lib/mame-chd -Isrc/includes -Isrc/$(MAMEOS) -I$(OBJ)/cpu/m68000 -Isrc/cpu/m68000 -I$(DEVKITPRO)/libnx/include -I$(DEVKITPRO)/portlibs/switch/include
CXXFLAGS += -DNDEBUG `freetype-config --cflags`\
	$(ARCH) -w -O3 -fomit-frame-pointer -fstrict-aliasing \
	-Wall -Wno-sign-compare -Wunused \
	-Wpointer-arith -Wcast-align -Waggregate-return \
	-Wshadow -Wundef \
	-Wformat-security -Wwrite-strings \
	-Wdisabled-optimization \
	-Wno-sizeof-pointer-memaccess \
	-mtune=cortex-a57 -mtp=soft -fPIE -fomit-frame-pointer \
	-fno-exceptions -std=c++11
	
ifdef SYMBOLS
CFLAGS += -O0 -Wall -Werror -Wno-unused -g
else
CFLAGS += -DNDEBUG `freetype-config --cflags` \
	$(ARCH) -w -O3 -fomit-frame-pointer -fstrict-aliasing \
	-Wall -Wno-sign-compare -Wunused \
	-Wpointer-arith -Wbad-function-cast -Wcast-align -Waggregate-return \
	-Wshadow -Wstrict-prototypes -Wundef \
	-Wformat-security -Wwrite-strings \
	-Wdisabled-optimization \
	-Wno-sizeof-pointer-memaccess \
	-mtune=cortex-a57 -mtp=soft -fPIE -fomit-frame-pointer
endif

CFLAGSPEDANTIC = $(CFLAGS) -pedantic

ifdef SYMBOLS
LDFLAGS =
else
#LDFLAGS = -s -Wl,--warn-common
LDFLAGS = -specs=$(DEVKITPRO)/libnx/switch.specs -g -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIE -L$(DEVKITPRO)/libnx/lib -L$(DEVKITPRO)/portlibs/switch/lib
endif

ifdef MAP
MAPFLAGS = -Wl,-M >$(NAME).map
else
MAPFLAGS =
endif

# platform .mak files will want to add to this
LIBS = -lfreetype `freetype-config --libs` -lcurl \
	-lz  -lglad -lEGL -lglapi -ldrm_nouveau -lnx -lm

OBJDIRS = obj $(OBJ) $(OBJ)/cpu $(OBJ)/sound $(OBJ)/$(MAMEOS) \
	$(OBJ)/drivers $(OBJ)/machine $(OBJ)/vidhrdw $(OBJ)/sndhrdw \
	$(OBJ)/lib $(OBJ)/lib/mame-chd $(OBJ)/nx/compat_shim $(OBJ)/nx/compat_shim/utils
ifdef MESS
OBJDIRS += $(OBJ)/mess $(OBJ)/mess/systems $(OBJ)/mess/machine \
	$(OBJ)/mess/vidhrdw $(OBJ)/mess/sndhrdw $(OBJ)/mess/tools
endif

ifeq ($(TARGET),mmsnd)
OBJDIRS	+= $(OBJ)/mmsnd $(OBJ)/mmsnd/machine $(OBJ)/mmsnd/drivers $(OBJ)/mmsnd/sndhrdw
endif

# romcmp/xml2info are host-side PC utilities; building them for aarch64
# produces binaries nothing can run. Still available via `make extra`.
all:	maketree $(EMULATOR)
 
# include the various .mak files
include src/core.mak
include src/$(TARGET).mak
# fullset.mak (the ~750-driver expansion) needs function-level dedup against
# mame.mak - 5262 duplicate symbols across hundreds of file pairs, not a
# simple per-file diff. Disabled until that's done properly - see session
# notes / chat history for the investigation.
# include src/fullset.mak
include src/rules.mak
include src/nx/nx.mak

ifdef DEBUG
DBGDEFS = -DMAME_DEBUG
else
DBGDEFS =
DBGOBJS =
endif

ifdef COMPILESYSTEM_CYGWIN
CFLAGS	+= -mno-cygwin
LDFLAGS	+= -mno-cygwin
endif

extra:	$(TOOLS) $(TEXTS)

# combine the various definitions to one
CDEFS = $(DEFS) $(COREDEFS) $(CPUDEFS) $(SOUNDDEFS) $(ASMDEFS) $(DBGDEFS)
 
$(OBJ)/nx/gfx.o : src/nx/gfx.cpp
	$(CXX) $(CDEFS) $(CXXFLAGS) -MMD -MP -c src/nx/gfx.cpp -o $(OBJ)/nx/gfx.o

$(OBJ)/nx/nx_maintest.o: src/nx/nx_maintest.cpp
	$(CXX) $(CDEFS) $(CXXFLAGS) -MMD -MP -c src/nx/nx_maintest.cpp -o $(OBJ)/nx/nx_maintest.o

$(OBJ)/nx/nx_romlist.o : src/nx/nx_romlist.cpp
	$(CXX) $(CDEFS) $(CXXFLAGS) -MMD -MP -c src/nx/nx_romlist.cpp -o $(OBJ)/nx/nx_romlist.o

$(OBJ)/nx/nx_artwork.o : src/nx/nx_artwork.cpp
	$(CXX) $(CDEFS) $(CXXFLAGS) -MMD -MP -c src/nx/nx_artwork.cpp -o $(OBJ)/nx/nx_artwork.o
 
# primary target
$(EMULATOR): $(OBJS) $(COREOBJS) $(OSOBJS) $(DRVLIBS) 
# always recompile the version string
	$(CC) $(CDEFS) $(CFLAGS) -c src/version.c -o $(OBJ)/version.o
# Switch UI Elements
	@echo Linking $@...
	$(LD) $(LDFLAGS) $(OBJS) $(COREOBJS) $(OSOBJS) $(LIBS) $(DRVLIBS) -o $@ $(MAPFLAGS)

romcmp$(EXE): $(OBJ)/romcmp.o $(OBJ)/unzip.o
	@echo Linking $@...
	$(LD) $(LDFLAGS) $^ -lz -o $@

hdcomp$(EXE): $(OBJ)/hdcomp.o $(OBJ)/harddisk.o $(OBJ)/md5.o $(OBJ)/lib/mame-chd/chd.o $(OBJ)/lib/mame-chd/mame_sha1.o $(OBJ)/nx/compat_shim/utils/md5.o
	@echo Linking $@...
	$(LD) $(LDFLAGS) $^ -lz -o $@

xml2info$(EXE): src/xml2info/xml2info.c
	@echo Compiling $@...
	$(CC) -O1 -o xml2info$(EXE) $<

ifdef PERL
$(OBJ)/cpuintrf.o: src/cpuintrf.c rules.mak
	$(PERL) src/makelist.pl
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -c $< -o $@
endif

# for Windows at least, we can't compile OS-specific code with -pedantic
# Header dependency tracking. Without this the only prerequisite of an object
# is its own .c, so editing a header rebuilds nothing that includes it and the
# link silently keeps stale objects. -MMD writes a .d beside each .o listing
# the headers it read; -MP adds phony targets so a deleted header does not
# break the build. The generated .d files are pulled in at the bottom.
DEPFLAGS = -MMD -MP

$(OBJ)/$(MAMEOS)/%.o: src/$(MAMEOS)/%.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ)/%.o: src/%.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) $(DEPFLAGS) -c $< -o $@

# compile generated C files for the 68000 emulator
$(M68000_GENERATED_OBJS): $(OBJ)/cpu/m68000/m68kmake$(EXE)
	@echo Compiling $(subst .o,.c,$@)...
	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -c $*.c -o $@

# additional rule, because m68kcpu.c includes the generated m68kops.h :-/
$(OBJ)/cpu/m68000/m68kcpu.o: $(OBJ)/cpu/m68000/m68kmake$(EXE)

# generate C source files for the 68000 emulator
# m68kmake is a host codegen tool (runs on the build machine, not the
# Switch) - build it with the host's own cc, not the aarch64 cross-compiler.
# The checked-in m68kmake.exe is a prebuilt Windows PE binary and won't run
# here.
$(OBJ)/cpu/m68000/m68kmake$(EXE): src/cpu/m68000/m68kmake.c
	@echo M68K make $<...
	cc -O2 -o $(OBJ)/cpu/m68000/m68kmake$(EXE) $<
	@echo Generating M68K source files...
	$(OBJ)/cpu/m68000/m68kmake$(EXE) $(OBJ)/cpu/m68000 src/cpu/m68000/m68k_in.c

ifndef SWITCH
# generate asm source files for the 68000/68020 emulators
$(OBJ)/cpu/m68000/68000.asm:  src/cpu/m68000/make68k.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -O0 -DDOS -o $(OBJ)/cpu/m68000/make68k$(EXE) $<
	@echo Generating $@...
	@$(OBJ)/cpu/m68000/make68k$(EXE) $@ $(OBJ)/cpu/m68000/68000tab.asm 00

$(OBJ)/cpu/m68000/68020.asm:  src/cpu/m68000/make68k.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -O0 -DDOS -o $(OBJ)/cpu/m68000/make68k$(EXE) $<
	@echo Generating $@...
	@$(OBJ)/cpu/m68000/make68k$(EXE) $@ $(OBJ)/cpu/m68000/68020tab.asm 20

# generated asm files for the 68000 emulator
$(OBJ)/cpu/m68000/68000.o:  $(OBJ)/cpu/m68000/68000.asm
	@echo Assembling $<...
	$(ASM) -o $@ $(ASMFLAGS) $(subst -D,-d,$(ASMDEFS)) $<

$(OBJ)/cpu/m68000/68020.o:  $(OBJ)/cpu/m68000/68020.asm
	@echo Assembling $<...
	$(ASM) -o $@ $(ASMFLAGS) $(subst -D,-d,$(ASMDEFS)) $<
endif
 
$(OBJ)/%.a:
	@echo Archiving $@...
	$(RM) $@
	$(AR) cr $@ $^

makedir:
	@echo make makedir is no longer necessary, just type make	

$(sort $(OBJDIRS)):
	$(MD) $@	

maketree: $(sort $(OBJDIRS))
		  $(MD) release

clean:
	@echo Deleting object tree $(OBJ)...
	$(RM) -r $(OBJ)
	@echo Deleting $(EMULATOR)...
	$(RM) $(EMULATOR)

clean68k:
	@echo Deleting 68k files...
	$(RM) -r $(OBJ)/cpuintrf.o
	$(RM) -r $(OBJ)/drivers/cps2.o
	$(RM) -r $(OBJ)/cpu/m68000

check: $(EMULATOR) xml2info$(EXE)
	./$(EMULATOR) -listxml > $(NAME).xml
	./xml2info < $(NAME).xml > $(NAME).lst
	./xmllint --valid --noout $(NAME).xml

	
	
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all	:	$(BINDIR)/$(OUTPUT).pfs0 $(BINDIR)/$(OUTPUT).nro

$(BINDIR)/$(OUTPUT).pfs0	:	$(BINDIR)/$(OUTPUT).nso

$(BINDIR)/$(OUTPUT).nso	:	$(BINDIR)/$(OUTPUT).elf

ifeq ($(strip $(NO_NACP)),)
$(BINDIR)/$(OUTPUT).nro	:	$(BINDIR)/$(OUTPUT).elf $(BINDIR)/$(OUTPUT).nacp
else
$(BINDIR)/$(OUTPUT).nro	:	$(BINDIR)/$(OUTPUT).elf
endif

$(BINDIR)/$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES_BIN)

# Pull in the .d files written by -MMD so header edits rebuild their objects.
# Wildcard, not a computed list: on a clean tree there are none yet, and the
# leading - keeps a missing or malformed one from failing the build.
-include $(shell find $(OBJ) -name '*.d' 2>/dev/null)

# end of Makefile ...	

