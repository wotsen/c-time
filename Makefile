MAKE_INSTALL_PREFIX = ./install
ITEM :=
# target marcros
ifneq ($(ITEM), TEST)
TARGET := libutil_time.a
else
TARGET := test
endif
MAIN_SRC := test.c

# compile marcros
ifeq ($(ITEM), TEST)
DIRS := src ut
else
DIRS := src
endif
OBJS := 

# intermedia compile marcros
ALL_OBJS := 
CLEAN_FILES := $(TARGET) $(OBJS)
DIST_CLEAN_FILES := $(OBJS)

# recursive wildcard
rwildcard=$(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2))$(filter $(subst *,%,$(2)),$(d)))

# default target
default: show-info all

# non-phony targets
$(TARGET): build-subdirs $(OBJS) find-all-objs
	@echo -e "\t" CC $@
ifeq ($(ITEM), TEST)
	@$(CC) $(ALL_OBJS) $(MAIN_SRC) -o $@ $(CCFLAG) -lcmockery -lpthread
else
	@$(AR) $@ $(ALL_OBJS)
endif

# phony targets
.PHONY: all
all: $(TARGET)
	@echo Target $(TARGET) build finished.

.PHONY: clean
clean: clean-subdirs
	@echo CLEAN $(CLEAN_FILES)
	@rm -f $(CLEAN_FILES) test

.PHONY: distclean
distclean: clean-subdirs
	@echo CLEAN $(DIST_CLEAN_FILES)
	@rm -f $(DIST_CLEAN_FILES)

# phony funcs
.PHONY: find-all-objs
find-all-objs:
	$(eval ALL_OBJS += $(call rwildcard,$(DIRS),*.o))

.PHONY: show-info
show-info:
	@echo Building Project

.PHONY: install
install:
	mkdir $(MAKE_INSTALL_PREFIX)/include/util_time/ -p
	mkdir $(MAKE_INSTALL_PREFIX)/lib/ -p
	cp $(TARGET) $(MAKE_INSTALL_PREFIX)/lib/ -f
	cp src/util_time.h $(MAKE_INSTALL_PREFIX)/include/util_time/ -f

# need to be placed at the end of the file
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
export ROOT_DIR := $(shell pwd)
export PROJECT_PATH := $(patsubst %/,%,$(dir $(mkfile_path)))
export MAKE_INCLUDE=$(PROJECT_PATH)/mkconfig/make.global
export SUB_MAKE_INCLUDE=$(PROJECT_PATH)/mkconfig/submake.global
include $(MAKE_INCLUDE)
