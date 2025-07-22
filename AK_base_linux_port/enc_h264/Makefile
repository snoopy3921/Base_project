SRC_PATH=$(word 1, $(dir $(MAKEFILE_LIST)))
vpath %.c $(SRC_PATH)
vpath %.cc $(SRC_PATH)
vpath %.cpp $(SRC_PATH)
vpath %.asm $(SRC_PATH)
vpath %.S $(SRC_PATH)
vpath %.rc $(SRC_PATH)
vpath %.pc.in $(SRC_PATH)

OS=rtos
ARCH=arm

LIBPREFIX=lib
LIBSUFFIX=a
CCAS=$(CC)
CXX_O=-o $@
CXX_LINK_O=-o $@
AR_OPTS=cr $@
LINK_LOCAL_DIR=-L.
LINK_LIB=-l$(1)
CFLAGS_OPT=-O3
CFLAGS_DEBUG=-g
BUILDTYPE=Release
V=Yes
PREFIX=./install
OBJ=o
DESTDIR=
LIBDIR_NAME=lib
PROJECT_NAME=openh264
CCASFLAGS=$(CFLAGS)
STATIC_LDFLAGS=-lstdc++
STRIP ?= strip
USE_STACK_PROTECTOR = Yes
FULL_VERSION := 2.2.0
HAVE_GMP_API=No

# Configurations
ifeq ($(BUILDTYPE), Release)
CFLAGS += $(CFLAGS_OPT)
CFLAGS += -DNDEBUG
USE_ASM = Yes
ifeq ($(DEBUGSYMBOLS), True)
CFLAGS += -g
CXXFLAGS += -g
DEBUGSYMBOLS_TAG := _debug_symbols
PROCESS_FILES := True
endif
else
CFLAGS += $(CFLAGS_DEBUG)
USE_ASM = No
endif

ifeq ($(USE_ASAN), Yes)
CFLAGS += -fsanitize=address
LDFLAGS += -fsanitize=address
endif

STRIP_FLAGS := -S
ifeq (linux, $((OS)))
STRIP_FLAGS := -g
endif

# Make sure the all target is the first one
all: libraries

include $(SRC_PATH)build/platform-$(OS).mk

CFLAGS += -DGENERATED_VERSION_HEADER
LDFLAGS +=

ifeq (Yes, $(GCOV))
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -lgcov
endif

#### No user-serviceable parts below this line
ifneq ($(V),Yes)
    QUIET_CXX  = @printf "CXX\t$@\n";
    QUIET_CC   = @printf "CC\t$@\n";
    QUIET_CCAS = @printf "CCAS\t$@\n";
    QUIET_ASM  = @printf "ASM\t$@\n";
    QUIET_AR   = @printf "AR\t$@\n";
    QUIET_RC   = @printf "RC\t$@\n";
    QUIET      = @
endif


INCLUDES += -I$(SRC_PATH)codec/api/svc -I$(SRC_PATH)codec/common/inc -Icodec/common/inc

ENCODER_INCLUDES += \
    -I$(SRC_PATH)codec/encoder/core/inc \
    -I$(SRC_PATH)codec/encoder/plus/inc \
    -I$(SRC_PATH)codec/processing/interface

H264ENC_INCLUDES += $(ENCODER_INCLUDES) $(CONSOLE_COMMON_INCLUDES) -I$(SRC_PATH)codec/console/enc/inc
H264ENC_LDFLAGS = $(LINK_LOCAL_DIR) $(call LINK_LIB,encoder) $(call LINK_LIB,processing) $(call LINK_LIB,common) $(call LINK_LIB,console_common)
H264ENC_DEPS = $(LIBPREFIX)encoder.$(LIBSUFFIX) $(LIBPREFIX)processing.$(LIBSUFFIX) $(LIBPREFIX)common.$(LIBSUFFIX) $(LIBPREFIX)console_common.$(LIBSUFFIX)


.PHONY: clean $(PROJECT_NAME).pc $(PROJECT_NAME)-static.pc

generate-version:
	$(QUIET)sh $(SRC_PATH)codec/common/generate_version.sh $(SRC_PATH)

codec/encoder/plus/src/welsEncoderExt.$(OBJ): | generate-version

clean:
	$(QUIET)rm -f $(OBJS) $(OBJS:.$(OBJ)=.d) $(OBJS:.$(OBJ)=.obj) $(LIBRARIES) $(BINARIES) *.lib *.a *.dylib *.dll *.so *.so.* *.exe *.pdb *.exp *.pc *.res *.map $(SRC_PATH)codec/common/inc/version_gen.h

include $(SRC_PATH)codec/encoder/targets.mk

libraries: $(LIBPREFIX)$(PROJECT_NAME).$(LIBSUFFIX)

LIBRARIES += $(LIBPREFIX)$(PROJECT_NAME).$(LIBSUFFIX)

$(LIBPREFIX)$(PROJECT_NAME).$(LIBSUFFIX): $(ENCODER_OBJS) $(COMMON_OBJS)
	$(QUIET)rm -f $@
	$(QUIET_AR)$(AR) $(AR_OPTS) $+
ifeq (True, $(PROCESS_FILES))
	cp $@ $(LIBPREFIX)$(PROJECT_NAME)$(DEBUGSYMBOLS_TAG).$(LIBSUFFIX)
	$(STRIP) $(STRIP_FLAGS) $@ -o $@
endif

echo-plugin-name:
	@echo $(MODULE)

$(PROJECT_NAME).pc: $(PROJECT_NAME).pc.in
	@sed -e 's;@prefix@;$(PREFIX);' -e 's;@libdir@;$(PREFIX)/lib;' -e 's;@VERSION@;$(FULL_VERSION);' -e 's;@LIBS@;;' -e 's;@LIBS_PRIVATE@;$(STATIC_LDFLAGS);' < $< > $@

$(PROJECT_NAME)-static.pc: $(PROJECT_NAME).pc.in
	@sed -e 's;@prefix@;$(PREFIX);' -e 's;@libdir@;$(PREFIX)/lib;' -e 's;@VERSION@;$(FULL_VERSION);' -e 's;@LIBS@;$(STATIC_LDFLAGS);' -e 's;@LIBS_PRIVATE@;;' < $< > $@

install-headers:
	mkdir -p $(DESTDIR)$(PREFIX)/include/wels
	install -m 644 $(SRC_PATH)/codec/api/svc/codec*.h $(DESTDIR)$(PREFIX)/include/wels

install-static-lib: $(LIBPREFIX)$(PROJECT_NAME).$(LIBSUFFIX) install-headers
	mkdir -p $(DESTDIR)$(PREFIX)/$(LIBDIR_NAME)
	install -m 644 $(LIBPREFIX)$(PROJECT_NAME).$(LIBSUFFIX) $(DESTDIR)$(PREFIX)/$(LIBDIR_NAME)

install-static: install-static-lib $(PROJECT_NAME)-static.pc
	mkdir -p $(DESTDIR)$(PREFIX)/$(LIBDIR_NAME)/pkgconfig
	install -m 644 $(PROJECT_NAME)-static.pc $(DESTDIR)$(PREFIX)/$(LIBDIR_NAME)/pkgconfig/$(PROJECT_NAME).pc

ifneq ($(EXTRA_LIBRARY),)
	install -m 644 $(EXTRA_LIBRARY) $(DESTDIR)$(PREFIX)/$(LIBDIR_NAME)
endif

install: install-static-lib
	@:

-include $(OBJS:.$(OBJ)=.d)

OBJDIRS = $(sort $(dir $(OBJS)))

$(OBJDIRS):
	$(QUIET)mkdir -p $@

$(OBJS): | $(OBJDIRS)
