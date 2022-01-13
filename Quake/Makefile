# cannibalised quakespasm makefile, with all the sdl and gl stuff removed.
# we'll use it as a library and just grab the data from this to pass it to vulkan instead.

# GNU Makefile for QuakeSpasm unix targets.
# "make DEBUG=1" to build a debug client.
# "make DO_USERDIRS=1" to enable user directories support

# Enable/Disable user directories support
DO_USERDIRS=0

### Enable/Disable codecs for streaming music support
USE_CODEC_WAVE=1
USE_CODEC_FLAC=0
USE_CODEC_MP3=1
USE_CODEC_VORBIS=1
USE_CODEC_OPUS=0
# either mikmod or xmp
USE_CODEC_MIKMOD=0
USE_CODEC_XMP=0
USE_CODEC_UMX=0

# which library to use for mp3 decoding: mad or mpg123
MP3LIB=mad
# which library to use for ogg decoding: vorbis or tremor
VORBISLIB=vorbis

# ---------------------------
# Helper functions
# ---------------------------

check_gcc = $(shell if echo | $(CC) $(1) -Werror -S -o /dev/null -xc - > /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi;)

# ---------------------------

HOST_OS := $(shell uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]')

DEBUG   ?= 0

# ---------------------------
# build variables
# ---------------------------

CC ?= gcc
LINKER = $(CC)
AR ?= ar

STRIP ?= strip
PKG_CONFIG ?= pkg-config

CPUFLAGS=
LDFLAGS =
DFLAGS ?=
CFLAGS ?= -Wall -Wno-trigraphs
CFLAGS += $(CPUFLAGS)
CFLAGS += -fPIC
ifneq ($(DEBUG),0)
DFLAGS += -DDEBUG
CFLAGS += -g
do_strip=
else
DFLAGS += -DNDEBUG
CFLAGS += -O2
CFLAGS += $(call check_gcc,-fweb,)
CFLAGS += $(call check_gcc,-frename-registers,)
cmd_strip=$(STRIP) $(1)
define do_strip
	$(call cmd_strip,$(1));
endef
endif

ifeq ($(DO_USERDIRS),1)
CFLAGS += -DDO_USERDIRS=1
endif

ifeq ($(HOST_OS),sunos)
NET_LIBS   :=-lsocket -lnsl -lresolv
else
NET_LIBS   :=
endif

ifneq ($(VORBISLIB),vorbis)
ifneq ($(VORBISLIB),tremor)
$(error Invalid VORBISLIB setting)
endif
endif
ifneq ($(MP3LIB),mpg123)
ifneq ($(MP3LIB),mad)
$(error Invalid MP3LIB setting)
endif
endif
ifeq ($(MP3LIB),mad)
mp3_obj=snd_mp3
lib_mp3dec=-lmad
endif
ifeq ($(MP3LIB),mpg123)
mp3_obj=snd_mpg123
lib_mp3dec=-lmpg123
endif
ifeq ($(VORBISLIB),vorbis)
cpp_vorbisdec=
lib_vorbisdec=-lvorbisfile -lvorbis -logg
endif
ifeq ($(VORBISLIB),tremor)
cpp_vorbisdec=-DVORBIS_USE_TREMOR
lib_vorbisdec=-lvorbisidec -logg
endif

CODECLIBS  :=
ifeq ($(USE_CODEC_WAVE),1)
CFLAGS+= -DUSE_CODEC_WAVE
endif
ifeq ($(USE_CODEC_FLAC),1)
CFLAGS+= -DUSE_CODEC_FLAC
CODECLIBS+= -lFLAC
endif
ifeq ($(USE_CODEC_OPUS),1)
# opus and opusfile put their *.h under <includedir>/opus,
# but they include the headers without the opus directory
# prefix and rely on pkg-config. ewww...
CFLAGS+= -DUSE_CODEC_OPUS
CFLAGS+= $(shell $(PKG_CONFIG) --cflags opusfile)
CODECLIBS+= $(shell $(PKG_CONFIG) --libs   opusfile)
endif
ifeq ($(USE_CODEC_VORBIS),1)
CFLAGS+= -DUSE_CODEC_VORBIS $(cpp_vorbisdec)
CODECLIBS+= $(lib_vorbisdec)
endif
ifeq ($(USE_CODEC_MP3),1)
CFLAGS+= -DUSE_CODEC_MP3
CODECLIBS+= $(lib_mp3dec)
endif
ifeq ($(USE_CODEC_MIKMOD),1)
CFLAGS+= -DUSE_CODEC_MIKMOD
CODECLIBS+= -lmikmod
endif
ifeq ($(USE_CODEC_XMP),1)
CFLAGS+= -DUSE_CODEC_XMP
CODECLIBS+= -lxmp
endif
ifeq ($(USE_CODEC_UMX),1)
CFLAGS+= -DUSE_CODEC_UMX
endif

COMMON_LIBS:= -lm

LIBS := $(COMMON_LIBS) $(NET_LIBS) $(CODECLIBS)

# ---------------------------
# targets
# ---------------------------

.PHONY:	clean debug release

DEFAULT_TARGET := libquakespasm.a

# ---------------------------
# rules
# ---------------------------

%.o:	%.c Makefile
	$(CC) $(DFLAGS) -c $(CFLAGS) -o $@ $<

# ----------------------------------------------------------------------------
# objects
# ----------------------------------------------------------------------------

MUSIC_OBJS:= bgmusic.o \
	snd_codec.o \
	snd_flac.o \
	snd_wave.o \
	snd_vorbis.o \
	snd_opus.o \
	$(mp3_obj).o \
	snd_mp3tag.o \
	snd_mikmod.o \
	snd_xmp.o \
	snd_umx.o
COMOBJ_SND := snd_dma.o snd_mix.o snd_mem.o $(MUSIC_OBJS)
SYSOBJ_NET := net_bsd.o net_udp.o
SYSOBJ_SYS := pl_linux.o
SYSOBJ_RES :=

# XXX don't want to build these:
GLOBJS = \
 	gl_refrag.o \
 	gl_rlight.o \
 	gl_rmain.o \
 	gl_fog.o \
 	gl_rmisc.o \
 	r_part.o \
 	r_world.o \
 	gl_screen.o \
 	gl_sky.o \
 	gl_warp.o \
 	gl_draw.o \
 	image.o \
 	gl_texmgr.o \
 	gl_mesh.o \
 	r_sprite.o \
 	r_alias.o \
 	r_brush.o \
 	gl_model.o

OBJS := strlcat.o \
	strlcpy.o \
	$(GLOBJS) \
	$(COMOBJ_SND) \
	$(SYSOBJ_NET) \
	net_dgrm.o \
	net_loop.o \
	net_main.o \
	chase.o \
	cl_demo.o \
	cl_input.o \
	cl_main.o \
	cl_parse.o \
	cl_tent.o \
	console.o \
	keys.o \
	menu.o \
	sbar.o \
	view.o \
	wad.o \
	cmd.o \
	common.o \
	miniz.o \
	crc.o \
	cvar.o \
	cfgfile.o \
	host.o \
	host_cmd.o \
	mathlib.o \
	pr_cmds.o \
	pr_edict.o \
	pr_exec.o \
	sv_main.o \
	sv_move.o \
	sv_phys.o \
	sv_user.o \
	world.o \
	zone.o \
	$(SYSOBJ_SYS) $(SYSOBJ_RES)

# ------------------------
# Linux build rules
# ------------------------

libquakespasm.a: $(OBJS) Makefile
	$(AR) crs $@ $(OBJS)

image.o: lodepng.c lodepng.h stb_image_write.h

release: libquakespasm.a
debug:
	$(error Use "make DEBUG=1")

clean:
	rm -f $(shell find . \( -name '*~' -o -name '#*#' -o -name '*.o' -o -name '*.res' -o -name $(DEFAULT_TARGET) \) -print)
